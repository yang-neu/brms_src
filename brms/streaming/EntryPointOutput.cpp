
#include <sys/stat.h>
#include <sys/time.h> 
#include <time.h> 

#include "EntryPointOutput.h"
#include "../DBAccessor.h"
#include "../ClipsRuleMgr.h"
#include "../ClipsSessionMgr.h"
#include "../common/StringUtil.h"

pthread_mutex_t EntryPointOutput::m_mutex = PTHREAD_MUTEX_INITIALIZER;
EntryPointOutput::EntryPointOutput():
	m_outPutTemplate("EventOutput")
  , m_globalName("list")
{

}
void EntryPointOutput::initialize(string streamName) 
{ 
	m_streamName = streamName; 
	
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	
	ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();
	m_slots = pRuleMgr->getSchema(m_outPutTemplate);
}
void EntryPointOutput::getOutputValue(string nameID, double &value)
{
    pthread_mutex_lock(&m_mutex);
   double ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == nameID)
       {
           ret = (*m_vItor).data.f_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);

    value = ret;
}

void EntryPointOutput::getOutputValue(string nameID, int &value)
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == nameID)
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);

    value = ret;
}

void EntryPointOutput::getOutputValue(string nameID, string &value)
{
   pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == nameID)
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);

    value = ret;
}

void EntryPointOutput::updateAll()
{
	DATA_OBJECT outputValue;
	
	//get fact address of gloable
	EnvGetDefglobalValue(m_theEnv,m_globalName,&outputValue);
	
	if (GetpType(&outputValue) != FACT_ADDRESS) return;
	void *factPtr = DOToPointer(outputValue);
	
	//get fact member data
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	if (pmgr == NULL) return ;
	
	ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(m_theEnv)->getRuleMgr();
	
	if (pRuleMgr == NULL) return ;
	
	
    pthread_mutex_lock(&m_mutex);
	FieldAndValue tmp;
	vector<Field>::iterator fieldItor= m_slots.begin();
	v_fd.clear();
	while(fieldItor != m_slots.end())
	{
		const char *slotname = (*fieldItor).name.c_str();
        //cout<<"EntryPointOutput::updateAll, slotname: "<<slotname<<endl;
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
				
                //cout<<"EntryPointOutput::updateAll, slotname: "<<slotname<<"fvalue:"<<tmp.data.f_value<<endl;
			}
			else if (type == INTEGER)
			{
				tmp.type = 1;
				tmp.name = string(slotname);
				tmp.data.i_value = ValueToLong(GetpValue(&outputValue));
				v_fd.push_back(tmp);
                //cout<<"EntryPointOutput::updateAll, slotname: "<<slotname<<"ivalue:"<<tmp.data.i_value<<endl;
			}
			else if (type == SYMBOL || type == STRING)
			{
				tmp.type = 2;
				tmp.name = string(slotname);
				string tmpstr = ValueToString(GetpValue(&outputValue));
				memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length() + 1);
				v_fd.push_back(tmp);
                //cout<<"EntryPointOutput::updateAll, slotname: "<<slotname<<"svalue:"<<tmpstr<<endl;
				
			}
		}
		else
		{
			cout<<"EntryPointOutput::updateAll, EnvGetFactSlot failed name: "<<slotname<<endl;
		}
		fieldItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	
}




double EntryPointOutput::getSpeed()
{
     pthread_mutex_lock(&m_mutex);
	double ret = 0.0;
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "speed")
		{
			ret = (*m_vItor).data.f_value;
			break;
		}
		m_vItor++;
	}
     pthread_mutex_unlock(&m_mutex);
	return ret;
}
double EntryPointOutput::getDistance() 
{
    pthread_mutex_lock(&m_mutex);
	double ret = 0.0;
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "distance")
		{
			ret = (*m_vItor).data.f_value;
			break;
		}
		m_vItor++;
	}
     pthread_mutex_unlock(&m_mutex);
	return ret;
}
double EntryPointOutput::getAccel()
{
        pthread_mutex_lock(&m_mutex);
		double ret = 0.0;
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "accelOpen")
		{
			ret = (*m_vItor).data.f_value;
			break;
		}
		m_vItor++;
	}
     pthread_mutex_unlock(&m_mutex);
	return ret;
}
string EntryPointOutput::getRoadClassSig()
{
     pthread_mutex_lock(&m_mutex);
	string ret = "";
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "roadClassSig")
		{
			ret = (*m_vItor).data.s_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}
double EntryPointOutput::getA()
{
    pthread_mutex_lock(&m_mutex);
	double ret = 0.0;
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "accel")
		{
			ret = (*m_vItor).data.f_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}
string EntryPointOutput::getRoadKind()
{
     pthread_mutex_lock(&m_mutex);
	string ret = "";
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "roadClass")
		{
			ret = (*m_vItor).data.s_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}
string EntryPointOutput::getFrontCar()
{
     pthread_mutex_lock(&m_mutex);
	string ret = "";
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "withFrontCar")
		{
			ret = (*m_vItor).data.s_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}

string EntryPointOutput::getDriveScene()
{
     pthread_mutex_lock(&m_mutex);
	string ret = "";
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "driveScene")
		{
			ret = (*m_vItor).data.s_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}

string EntryPointOutput::getSuddenAccel()
{
     pthread_mutex_lock(&m_mutex);
	string ret = "";
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "suddenAccel")
		{
			ret = (*m_vItor).data.s_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}

string EntryPointOutput::getSuddenAccelState()
{
    pthread_mutex_lock(&m_mutex);
	string ret = "";
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "suddenAccelState")
		{
			ret = (*m_vItor).data.s_value;
			break;
		}
		m_vItor++;
	}
    pthread_mutex_unlock(&m_mutex);
	return ret;
}

int EntryPointOutput::getSuddenAccelCount()
{
     pthread_mutex_lock(&m_mutex);
	int ret = 0.0;
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "suddenAccelCount")
		{
			ret = (*m_vItor).data.i_value;
			break;
		}
		m_vItor++;
	}
     pthread_mutex_unlock(&m_mutex);
	return ret;
}
double EntryPointOutput::getTTC()
{
     pthread_mutex_lock(&m_mutex);
	double ret = 0.0;
	m_vItor = v_fd.begin();
	while(m_vItor != v_fd.end())
	{
		if ((*m_vItor).name == "TTC")
		{
            ret = (*m_vItor).data.f_value;
			break;
		}
		m_vItor++;
	}
     pthread_mutex_unlock(&m_mutex);
	return ret;
}
//profile2
string EntryPointOutput::getSuddenDecel()
{
    pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "suddenReduce")
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);
   return ret;
}

string EntryPointOutput::getSuddenDecelState()
{
    pthread_mutex_lock(&m_mutex);
    string ret = "";
    m_vItor = v_fd.begin();
    while(m_vItor != v_fd.end())
    {
        if ((*m_vItor).name == "suddenReduceState")
        {
            ret = (*m_vItor).data.s_value;
            break;
        }
        m_vItor++;
    }
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int EntryPointOutput::getSuddenDecelCount()
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "suddenReduceCount")
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);
   return ret;
}

//profile3
string EntryPointOutput::getDecelStart()
{
    pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "reduceStart")
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);
   return ret;
}

string EntryPointOutput::getDecelStartState()
{
    pthread_mutex_lock(&m_mutex);
    string ret = "";
    m_vItor = v_fd.begin();
    while(m_vItor != v_fd.end())
    {
        if ((*m_vItor).name == "reduceStartState")
        {
            ret = (*m_vItor).data.s_value;
            break;
        }
        m_vItor++;
    }
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int EntryPointOutput::getDecelStartCount()
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "reduceStartCount")
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);
   return ret;
}

//profile4
string EntryPointOutput::getTTCRisk()
{
    pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "TTCRisk")
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);
   return ret;
}

string EntryPointOutput::getTTCRiskState()
{
    pthread_mutex_lock(&m_mutex);
    string ret = "";
    m_vItor = v_fd.begin();
    while(m_vItor != v_fd.end())
    {
        if ((*m_vItor).name == "TTCRiskState")
        {
            ret = (*m_vItor).data.s_value;
            break;
        }
        m_vItor++;
    }
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int EntryPointOutput::getTTCRiskCount()
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "TTCRiskCount")
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);
   return ret;
}

//profile5
string EntryPointOutput::getDisOnExpressWay()
{
    pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "distanceOnExpressWay")
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);
   return ret;
}

string EntryPointOutput::getDisOnExpressWayState()
{
    pthread_mutex_lock(&m_mutex);
    string ret = "";
    m_vItor = v_fd.begin();
    while(m_vItor != v_fd.end())
    {
        if ((*m_vItor).name == "distanceOnExpressWayState")
        {
            ret = (*m_vItor).data.s_value;
            break;
        }
        m_vItor++;
    }
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int EntryPointOutput::getDisOnExpressWayCount()
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "distanceOnExpressWayCount")
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);
   return ret;
}

//profile6
string EntryPointOutput::getDisOnOpenRoad()
{
    pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "distanceOnOpenRoad")
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);
   return ret;
}

string EntryPointOutput::getDisOnOpenRoadState()
{
    pthread_mutex_lock(&m_mutex);
    string ret = "";
    m_vItor = v_fd.begin();
    while(m_vItor != v_fd.end())
    {
        if ((*m_vItor).name == "distanceOnOpenRoadState")
        {
            ret = (*m_vItor).data.s_value;
            break;
        }
        m_vItor++;
    }
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int EntryPointOutput::getDisOnOpenRoadCount()
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "distanceOnOpenRoadCount")
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);
   return ret;
}

//profile7
string EntryPointOutput::getFollowStop()
{
    pthread_mutex_lock(&m_mutex);
   string ret = "";
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "distanceofFollowStop")
       {
           ret = (*m_vItor).data.s_value;
           break;
       }
       m_vItor++;
   }
   pthread_mutex_unlock(&m_mutex);
   return ret;
}

string EntryPointOutput::getFollowStopState()
{
    pthread_mutex_lock(&m_mutex);
    string ret = "";
    m_vItor = v_fd.begin();
    while(m_vItor != v_fd.end())
    {
        if ((*m_vItor).name == "distanceofFollowStopState")
        {
            ret = (*m_vItor).data.s_value;
            break;
        }
        m_vItor++;
    }
    pthread_mutex_unlock(&m_mutex);
    return ret;
}

int EntryPointOutput::getFollowStopCount()
{
    pthread_mutex_lock(&m_mutex);
   int ret = 0.0;
   m_vItor = v_fd.begin();
   while(m_vItor != v_fd.end())
   {
       if ((*m_vItor).name == "distanceofFollowStopCount")
       {
           ret = (*m_vItor).data.i_value;
           break;
       }
       m_vItor++;
   }
    pthread_mutex_unlock(&m_mutex);
   return ret;
}
