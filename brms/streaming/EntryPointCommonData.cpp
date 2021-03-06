#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include "EntryPointCommonData.h"
#include "../ClipsRuleMgr.h"
#include "../ClipsSessionMgr.h"
#include "../common/StringUtil.h"
#include "../DBAccessor.h"


//Read data from FIFO
pthread_mutex_t EntryPointCommonData::m_mutex = PTHREAD_MUTEX_INITIALIZER;
EntryPointCommonData::EntryPointCommonData()
{

}

void EntryPointCommonData::updateAll()
{
    DATA_OBJECT outputValue;
    DATA_OBJECT insdata;
    void *theMultifield;
    void *myInstancePtr;
    int datatype;
    int i=0;

    pthread_mutex_lock(&m_mutex);

    //get pointer of FIFO
    myInstancePtr=EnvFindInstance(m_theEnv,NULL,"FIFO",TRUE);

    //get data from FIFO
    SetType(insdata,INSTANCE_ADDRESS);
    SetValue(insdata,myInstancePtr);
    EnvSend(m_theEnv,&insdata,"getData",NULL,&outputValue);

    if (GetpType(&outputValue) != MULTIFIELD) return;
    theMultifield = GetValue(outputValue);

    long end = GetDOEnd(outputValue);

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

bool EntryPointCommonData::loadFactFile()
{
    int result;
    char * fun_name="LoadFactList";
    DATA_OBJECT outputValue;

    result = EnvFunctionCall(m_theEnv,fun_name,NULL,&outputValue);

    if(result == 0)
	{
	    return TRUE;
	}
    else
	{
	    //an error occurred while opening the file
	    return FALSE;
	}

}

bool EntryPointCommonData::saveFactToFile()
{
    int result;
    char * fun_name="SaveFactList";
    DATA_OBJECT outputValue;

    result = EnvFunctionCall(m_theEnv,fun_name,NULL,&outputValue);

    if(result == 0)
	{
	    return TRUE;
	}
    else
	{
	    //an error occurred while opening the file
	    return FALSE;
	}
}

bool EntryPointCommonData::accelFileOperation(FILE_OPERATION opr)
{
    DATA_OBJECT theValue;
    void *templatePtr;
    string accelTemp = "accelFileOprationEvent";
    void * factPtr;
    string moduleName = "IOAgenda";
    void *theModule = EnvFindDefmodule(m_theEnv, moduleName.c_str());

    pthread_mutex_lock(&m_mutex);
    EnvIncrementGCLocks(m_theEnv);

    templatePtr = EnvFindDeftemplate(m_theEnv,accelTemp.c_str());
    factPtr = EnvCreateFact(m_theEnv,templatePtr);

    if (factPtr == NULL)
    {
        cout<<" EntryPointCommonData::accelFileOperation brakePressure faild: "<<endl;
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return FALSE;
    }
    SetpType(&theValue,INTEGER);
    SetpValue(&theValue,EnvAddLong(m_theEnv,opr));
    EnvPutFactSlot(m_theEnv,factPtr,"status",&theValue);
    EnvAssignFactSlotDefaults(m_theEnv,factPtr);
    EnvAssert(m_theEnv,factPtr);


    EnvFocus(m_theEnv, theModule);
    int ret = EnvRun(m_theEnv,-1);

    EnvDecrementGCLocks(m_theEnv);
    pthread_mutex_unlock(&m_mutex);

    cout<<" EntryPointCommonData::accelFileOperation (" << ret << ") fired." << endl;

    if(ret > 0){
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

