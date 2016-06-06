
#include <sys/stat.h>
#include <sys/time.h> 
#include <time.h> 

#include "EntryPointHistory.h"
#include "../DBAccessor.h"
#include "../ClipsRuleMgr.h"
#include "../ClipsSessionMgr.h"
#include "../common/StringUtil.h"
#include "../common/ConfigUtil.h"

EntryPointHistory::EntryPointHistory()
	: m_queryKey("time")
{
	if (ConfigUtil::GetInstance()->GetConfigStr("履歴種別") == "TIME")
	{
		m_limited.first = LIMITED_BY_TIME;
		m_limited.second.time = ConfigUtil::GetInstance()->GetConfigInt("履歴長さ");
	}
	else
	{
		m_limited.first = LIMITED_BY_NUMBER;
		m_limited.second.number = ConfigUtil::GetInstance()->GetConfigInt("履歴長さ");
	}
}

void EntryPointHistory::insert(string templateName, void *factPtr)
{
	DATA_OBJECT outputValue;
	FieldDataVec v_fd;
	FieldAndValue tmp;
	
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	if (pmgr == NULL) return ;
	
	ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();
	
	if (pRuleMgr == NULL) return ;
	
	DBAccessor* pdb = DBAccessor::GetInstance();
	if (pdb == NULL ) return;
	
	vector<Field> slots = pRuleMgr->getSchema(templateName);
	
	vector<Field>::iterator fieldItor= slots.begin();
	while(fieldItor != slots.end())
	{
		const char *slotname = (*fieldItor).name.c_str();
        //cout<<"EntryPointHistory::insert, slotname: "<<slotname<<endl;
		if (EnvGetFactSlot(m_theEnv,factPtr,slotname,&outputValue))
		{
			int type = GetpType(&outputValue);
			if ( type == FLOAT )
			{
				//cout<<"entry-point  insert action, update speed before: "<<g_speed<<endl;
				//g_speed = ValueToDouble(GetpValue(&outputValue));	
				//cout<<"entry-point  insert action, update speed after: "<<g_speed<<endl;
				tmp.type = 0;
				tmp.name = string(slotname);
				tmp.data.f_value = ValueToDouble(GetpValue(&outputValue));
				v_fd.push_back(tmp);
				
                //cout<<"EntryPointHistory::insert, slotname: "<<slotname<<"fvalue:"<<tmp.data.f_value<<endl;
			}
			else if (type == INTEGER)
			{
				tmp.type = 1;
				tmp.name = string(slotname);
				tmp.data.i_value = ValueToLong(GetpValue(&outputValue));
				v_fd.push_back(tmp);
                //cout<<"EntryPointHistory::insert, slotname: "<<slotname<<"ivalue:"<<tmp.data.i_value<<endl;
			}
			else if (type == SYMBOL || type == STRING)
			{
				tmp.type = 2;
				tmp.name = string(slotname);
				string tmpstr = ValueToString(GetpValue(&outputValue));
				memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
				v_fd.push_back(tmp);
                //cout<<"EntryPointHistory::insert, slotname: "<<slotname<<"svalue:"<<tmpstr<<endl;
				
			}
		}
		else
		{
			cout<<"EntryPointHistory::insert, EnvGetFactSlot failed name: "<<slotname<<endl;
		}
		fieldItor++;
	}
	
	if( m_tableList.find(templateName) == m_tableList.end())
	{
		vector<Field> fields = pRuleMgr->getSchema(templateName);
		TemplateMap param;
		param.insert(pair<string, vector<Field> >(templateName, fields));

		map<string, LimitedInfo>::iterator it = m_limitList.find(templateName);
		LimitedInfo li = m_limited;
		if (it != m_limitList.end())
		{
			li = it->second;
		}
		pdb->Initialize(param, li);
		m_tableList.insert(pair<string, int>(templateName, 1));
	}
	pdb->InsertFieldData(templateName, v_fd);
}

//condition: before/after[1ms,1s]base
//			 before/after[1ms]base
void EntryPointHistory::query(string templateName, string queryCondition, double baseTime, DATA_OBJECT_PTR returnValuePtr)
{
	if (m_tableList.find(templateName) == m_tableList.end())
		return ;
	if (returnValuePtr == NULL)
		return;
	DBAccessor* pdb = DBAccessor::GetInstance();
	if (pdb == NULL ) return ;
	
	string tmp1 = replace(queryCondition, "[", ",");
	string tmp2 = replace(tmp1, "]", ",");
	vector<string> conditions;
	split(tmp2, conditions);
	
	int type = 0;
	if(conditions.size() == 3) //before/after[1ms,1s] base
	{
		type = 1;
	}
	else if (conditions.size() == 2) //before/after[1ms] base
	{
		type =2;
	}	
	else
	{
		cout<<"EntryPointHistory::query,  failed templateName: "<<templateName<<";condition:"<<queryCondition<<endl;
		return ;
	}
	
	double timeS = 0.0;
	double timeBaseS = 0.0;
	
	timeBaseS = baseTime;

	
	if (conditions[0] == "before")
	{
		if (type == 1)
		{
            Interval intval;
            intval.kind = BEFORE_CLOSED_INTERVAL;

            if (conditions[1].find("ms") != string::npos)
            {
                int pos = conditions[1].size()-2;
                string timeNum = conditions[1].erase(pos, 2);
                timeS = atof(timeNum.c_str())/1000;
            }
            else if (conditions[1].find("s") != string::npos)
            {
                int pos = conditions[1].size()-1;
                string timeNum = conditions[1].erase(pos, 1);
                timeS = atof(timeNum.c_str());

            }
            else
            {
                return;
            }
           intval.start = timeS;

           if (conditions[2].find("ms") != string::npos)
           {
               int pos = conditions[2].size()-2;
               string timeNum = conditions[2].erase(pos, 2);
               timeS = atof(timeNum.c_str())/1000;
           }
           else if (conditions[2].find("s") != string::npos)
           {
               int pos = conditions[2].size()-1;
               string timeNum = conditions[2].erase(pos, 1);
               timeS = atof(timeNum.c_str());

           }
           else
           {
               return;
           }

           intval.end = timeS;
		   if (intval.start > timeS)
		   {
			   intval.end = intval.start;
			   intval.start = timeS;
		   }

           vector< FieldDataVec >  parmlist;
           parmlist = pdb->GetMultiFieldDataVec(templateName, m_queryKey, timeBaseS,  intval);
           EntryPoint::insert(parmlist, returnValuePtr);
            return ;
		}
		else if ( type ==2 )
		{
			if (conditions[1].find("ms") != string::npos)
			{
				int pos = conditions[1].size()-2;
				string timeNum = conditions[1].erase(pos, 2);
                timeS = (0.0-atof(timeNum.c_str()))/1000;
			}
			else if (conditions[1].find("s") != string::npos)
			{
				int pos = conditions[1].size()-1;
				string timeNum = conditions[1].erase(pos, 1);
				timeS = 0.0- atof(timeNum.c_str());
				
			}
			else
			{
				return;
			}
            vector<FieldAndValue> parmlist;
            parmlist = pdb->GetFieldData(templateName, m_queryKey, timeBaseS, timeS);

            vector< FieldDataVec >  parmlist1;
            parmlist1.push_back(parmlist);
            EntryPoint::insert(parmlist1, returnValuePtr);
		}
		else
		{
		}
	}
	else if (conditions[0] == "after")
	{
		//not implement
		return ;
	}
	else
		return ;

}

void EntryPointHistory::setExpire(string templateName, string expireCondition)
{
	size_t unit = expireCondition.find_first_of("hms");
	if (unit == string::npos)
	{
		cout << "EntryPointHistory::setExpire error condition: " << expireCondition << endl;
		return;
	}

	LimitedInfo li;
	li.first = LIMITED_BY_TIME;
	li.second.time = (double)(atoi(expireCondition.substr(0, unit).c_str()) * ((expireCondition[unit] == 'h') ? 3600 : ((expireCondition[unit] == 'm') ? 60 : 1)));

	m_limitList.insert(pair<string, LimitedInfo>(templateName, li));
}

void EntryPointHistory::insert(EventAccelOpen *accel)
{
    if (accel == NULL) return;

    string templateName="EventAccelOpen";
    FieldDataVec v_fd;
    FieldAndValue tmp;
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    if (pmgr == NULL) return ;

    ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();

    if (pRuleMgr == NULL) return ;

    DBAccessor* pdb = DBAccessor::GetInstance();
    if (pdb == NULL ) return;

    vector<Field> slots = pRuleMgr->getSchema(templateName);

    vector<Field>::iterator fieldItor= slots.begin();
    while(fieldItor != slots.end())
    {

        if((*fieldItor).name == "from")
        {
            tmp.type = 2;
            tmp.name = "from";
            string tmpstr = "entryPoint";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "name")
        {
            tmp.type = 2;
            tmp.name = "name";
            string tmpstr = "Driving Hitory Stream";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "accelOpen")
        {
            tmp.type = 0;
            tmp.name = "accelOpen";
            tmp.data.f_value = accel->getAccelOpen();
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "type")
        {
            tmp.type = 2;
            tmp.name = "type";
            string tmpstr = accel->getTypeString();
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);

        }
        else if ((*fieldItor).name == "time")
        {
            tmp.type = 0;
            tmp.name = "time";
            tmp.data.f_value = accel->getTime();
            v_fd.push_back(tmp);
        }
        else
        {
            cout<<" EntryPointHistory::insert(EventAccelOpen *accel) error field:"<<(*fieldItor).name<<endl;
        }

        fieldItor++;
    }

    if( m_tableList.find(templateName) == m_tableList.end())
    {
        vector<Field> fields = pRuleMgr->getSchema(templateName);
        TemplateMap param;
        param.insert(pair<string, vector<Field> >(templateName, fields));

		map<string, LimitedInfo>::iterator it = m_limitList.find(templateName);
		LimitedInfo li = m_limited;
		if (it != m_limitList.end())
		{
			li = it->second;
		}
		pdb->Initialize(param, li);
        m_tableList.insert(pair<string, int>(templateName, 1));
    }
    pdb->InsertFieldData(templateName, v_fd);
}

void EntryPointHistory::insert(EventSpeed *speed)
{
    if (speed == NULL) return;

    string templateName="EventSpeed";
    FieldDataVec v_fd;
    FieldAndValue tmp;
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    if (pmgr == NULL) return ;

    ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();

    if (pRuleMgr == NULL) return ;

    DBAccessor* pdb = DBAccessor::GetInstance();
    if (pdb == NULL ) return;

    vector<Field> slots = pRuleMgr->getSchema(templateName);

    vector<Field>::iterator fieldItor= slots.begin();
    while(fieldItor != slots.end())
    {

        if((*fieldItor).name == "from")
        {
            tmp.type = 2;
            tmp.name = "from";
            string tmpstr = "entryPoint";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "name")
        {
            tmp.type = 2;
            tmp.name = "name";
            string tmpstr = "Driving Hitory Stream";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "speed")
        {
            tmp.type = 0;
            tmp.name = "speed";
            tmp.data.f_value = speed->getSpeed();
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "type")
        {
            tmp.type = 2;
            tmp.name = "type";
            string tmpstr = speed->getTypeString();
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);

        }
        else if ((*fieldItor).name == "time")
        {
            tmp.type = 0;
            tmp.name = "time";
            tmp.data.f_value = speed->getTime();
            v_fd.push_back(tmp);
        }
        else
        {
            cout<<" EntryPointHistory::insert(EventSpeed *speed) error field:"<<(*fieldItor).name<<endl;
        }

        fieldItor++;
    }

    if( m_tableList.find(templateName) == m_tableList.end())
    {
        vector<Field> fields = pRuleMgr->getSchema(templateName);
        TemplateMap param;
        param.insert(pair<string, vector<Field> >(templateName, fields));

		map<string, LimitedInfo>::iterator it = m_limitList.find(templateName);
		LimitedInfo li = m_limited;
		if (it != m_limitList.end())
		{
			li = it->second;
		}
		pdb->Initialize(param, li);
        m_tableList.insert(pair<string, int>(templateName, 1));
    }
    pdb->InsertFieldData(templateName, v_fd);
}

void EntryPointHistory::insert(EventDistance *distance)
{
    if (distance == NULL) return;

    string templateName="EventDistance";
    FieldDataVec v_fd;
    FieldAndValue tmp;
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    if (pmgr == NULL) return ;

    ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();

    if (pRuleMgr == NULL) return ;

    DBAccessor* pdb = DBAccessor::GetInstance();
    if (pdb == NULL ) return;

    vector<Field> slots = pRuleMgr->getSchema(templateName);

    vector<Field>::iterator fieldItor= slots.begin();
    while(fieldItor != slots.end())
    {

        if((*fieldItor).name == "from")
        {
            tmp.type = 2;
            tmp.name = "from";
            string tmpstr = "entryPoint";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "name")
        {
            tmp.type = 2;
            tmp.name = "name";
            string tmpstr = "Driving Hitory Stream";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "distance")
        {
            tmp.type = 0;
            tmp.name = "distance";
            tmp.data.f_value = distance->getDistance();
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "type")
        {
            tmp.type = 2;
            tmp.name = "type";
            string tmpstr = distance->getTypeString();
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);

        }
        else if ((*fieldItor).name == "time")
        {
            tmp.type = 0;
            tmp.name = "time";
            tmp.data.f_value = distance->getTime();
            v_fd.push_back(tmp);
        }
        else
        {
            cout<<" EntryPointHistory::insert(EventDistance *distance) error field:"<<(*fieldItor).name<<endl;
        }

        fieldItor++;
    }

    if( m_tableList.find(templateName) == m_tableList.end())
    {
        vector<Field> fields = pRuleMgr->getSchema(templateName);
        TemplateMap param;
        param.insert(pair<string, vector<Field> >(templateName, fields));

		map<string, LimitedInfo>::iterator it = m_limitList.find(templateName);
		LimitedInfo li = m_limited;
		if (it != m_limitList.end())
		{
			li = it->second;
		}
		pdb->Initialize(param, li);
        m_tableList.insert(pair<string, int>(templateName, 1));
    }
    pdb->InsertFieldData(templateName, v_fd);
}

void EntryPointHistory::insert(EventRoadClass *rc)
{
    if (rc == NULL) return;

    string templateName="EventRoadClass";
    FieldDataVec v_fd;
    FieldAndValue tmp;
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    if (pmgr == NULL) return ;

    ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();

    if (pRuleMgr == NULL) return ;

    DBAccessor* pdb = DBAccessor::GetInstance();
    if (pdb == NULL ) return;

    vector<Field> slots = pRuleMgr->getSchema(templateName);

    vector<Field>::iterator fieldItor= slots.begin();
    while(fieldItor != slots.end())
    {

        if((*fieldItor).name == "from")
        {
            tmp.type = 2;
            tmp.name = "from";
            string tmpstr = "entryPoint";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "name")
        {
            tmp.type = 2;
            tmp.name = "name";
            string tmpstr = "Driving Hitory Stream";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "canData")
        {
            tmp.type = 0;
            tmp.name = "canData";
            tmp.data.f_value = rc->getCanData();
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "type")
        {
            tmp.type = 2;
            tmp.name = "type";
            string tmpstr = rc->getTypeString();
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);

        }
        else if ((*fieldItor).name == "time")
        {
            tmp.type = 0;
            tmp.name = "time";
            tmp.data.f_value = rc->getTime();
            v_fd.push_back(tmp);
        }
        else
        {
            cout<<" EntryPointHistory::insert(EventRoadClass *rc) error field:"<<(*fieldItor).name<<endl;
        }

        fieldItor++;
    }

    if( m_tableList.find(templateName) == m_tableList.end())
    {
        vector<Field> fields = pRuleMgr->getSchema(templateName);
        TemplateMap param;
        param.insert(pair<string, vector<Field> >(templateName, fields));

		map<string, LimitedInfo>::iterator it = m_limitList.find(templateName);
		LimitedInfo li = m_limited;
		if (it != m_limitList.end())
		{
			li = it->second;
		}
		pdb->Initialize(param, li);
        m_tableList.insert(pair<string, int>(templateName, 1));
    }
    pdb->InsertFieldData(templateName, v_fd);
}

void EntryPointHistory::insert(EventSteeringAngle *angle)
{
    if (angle == NULL) return;

    string templateName="EventSteeringAngle";
    FieldDataVec v_fd;
    FieldAndValue tmp;
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    if (pmgr == NULL) return ;

    ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();

    if (pRuleMgr == NULL) return ;

    DBAccessor* pdb = DBAccessor::GetInstance();
    if (pdb == NULL ) return;

    vector<Field> slots = pRuleMgr->getSchema(templateName);

    vector<Field>::iterator fieldItor= slots.begin();
    while(fieldItor != slots.end())
    {

        if((*fieldItor).name == "from")
        {
            tmp.type = 2;
            tmp.name = "from";
            string tmpstr = "entryPoint";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "name")
        {
            tmp.type = 2;
            tmp.name = "name";
            string tmpstr = "Driving Hitory Stream";
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "value")
        {
            tmp.type = 0;
            tmp.name = "value";
            tmp.data.f_value = angle->getValue();
            v_fd.push_back(tmp);
        }
        else if ((*fieldItor).name == "type")
        {
            tmp.type = 2;
            tmp.name = "type";
            string tmpstr = angle->getType();
            memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length()+1);
            v_fd.push_back(tmp);

        }
        else if ((*fieldItor).name == "time")
        {
            tmp.type = 0;
            tmp.name = "time";
            tmp.data.f_value = angle->getTime();
            v_fd.push_back(tmp);
        }
        else
        {
            cout<<" EntryPointHistory::insert(EventRoadClass *rc) error field:"<<(*fieldItor).name<<endl;
        }

        fieldItor++;
    }

    if( m_tableList.find(templateName) == m_tableList.end())
    {
        vector<Field> fields = pRuleMgr->getSchema(templateName);
        TemplateMap param;
        param.insert(pair<string, vector<Field> >(templateName, fields));

		map<string, LimitedInfo>::iterator it = m_limitList.find(templateName);
		LimitedInfo li = m_limited;
		if (it != m_limitList.end())
		{
			li = it->second;
		}
		pdb->Initialize(param, li);
        m_tableList.insert(pair<string, int>(templateName, 1));
    }
    pdb->InsertFieldData(templateName, v_fd);
}
