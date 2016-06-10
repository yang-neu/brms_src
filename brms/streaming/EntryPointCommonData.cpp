
#include <sys/stat.h>
#include <sys/time.h> 
#include <time.h> 

#include "EntryPointCommonData.h"
#include "../ClipsRuleMgr.h"
#include "../ClipsSessionMgr.h"
#include "../common/StringUtil.h"
#include "../DBAccessor.h"


pthread_mutex_t EntryPointCommonData::m_mutex = PTHREAD_MUTEX_INITIALIZER;
EntryPointCommonData::EntryPointCommonData():
	m_globalName("AccelSpeedInfoList")
{

}

void EntryPointCommonData::updateAll()
{
	DATA_OBJECT outputValue;
    void *theMultifield;
	int datatype;
    int i=0;
	
	//get fact address of gloable
	EnvGetDefglobalValue(m_theEnv,m_globalName,&outputValue);
	
	if (GetpType(&outputValue) != MULTIFIELD) return;
    theMultifield = GetValue(outputValue);
	//void *factPtr = DOToPointer(outputValue);
	
    //long long MFLength = GetDOLength(outputValue);
	long end = GetDOEnd(outputValue);

	pthread_mutex_lock(&m_mutex);
	FieldAndValue tmp;

	for(i = GetDOBegin(outputValue); i <= end; i++)
		{
            datatype = GetMFType(theMultifield,i);
			if(datatype == FLOAT){
                tmp.name = "common";
				tmp.type = 0;
                tmp.data.f_value = ValueToDouble(GetMFValue(theMultifield,i));
				v_fd.push_back(tmp);
				cout<<"EntryPointOutput::updateAll, fvalue:"<<tmp.data.f_value<<endl;
			}
			else if(datatype == INTEGER){
                tmp.name = "common";
				tmp.type = 1;
                tmp.data.i_value = (int)ValueToLong(GetMFValue(theMultifield,i));
				v_fd.push_back(tmp);
				cout<<"EntryPointOutput::updateAll, ivalue:"<<tmp.data.i_value<<endl;
			}
            else if(datatype == SYMBOL || datatype == STRING){
                tmp.name = "common";
				tmp.type = 2;
                string tmpstr = ValueToString(GetMFValue(theMultifield,i));
				memcpy(tmp.data.s_value, tmpstr.c_str(), tmpstr.length() + 1);
				v_fd.push_back(tmp);
				cout<<"EntryPointOutput::updateAll, svalue:"<<tmpstr<<endl;
			}else{
				cout<<"EntryPointOutput::updateAll, Wrong data type!"<<endl;
			}
		}
	
    //clear the global value
    //EnvGetDefglobalValue(m_theEnv,m_globalName,&outputValue);

    pthread_mutex_unlock(&m_mutex);


}

FieldDataVec EntryPointCommonData::getCommonData()
{
     pthread_mutex_lock(&m_mutex);
	 FieldDataVec v_fd_tmp = v_fd;
	 v_fd.clear();
     pthread_mutex_unlock(&m_mutex);
	return v_fd_tmp;
}


