
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
extern "C"
{
	#include "clips.h"
}
#include "SignalEntryPoint.h"


#define LIST_MAX_COUNT 300


pthread_mutex_t SignalEntryPoint::m_mutex = PTHREAD_MUTEX_INITIALIZER;

SignalEntryPoint::SignalEntryPoint():
	m_speedTemplate("EventSpeedList")
	, m_disTemplate("EventDistanceList")
	, m_roadClassTemplate("EventRoadClassList")
	, m_accelOpenTemplate("EventAccelOpenList")
	, m_aimLessTemplate("EventAimlessList")
	, m_driverFaceTemplate("EventDriverFaceList")
	, m_levelDataTemplate("EventLevelDataList")
	, m_shiftStateTemplate("EventShiftStateList")
	, m_steeringAngleTemplate("EventSteeringAngleList")
	, m_brakePressureTemplate("EventBrakePressureList")

{
}
void SignalEntryPoint::initialize(string streamName) 
{
	m_streamName = streamName;
	m_currentSpeedCnt = 1;
	m_currentDisCnt = 1;
	m_currentRoadClassCnt = 1;
	m_currentAccelCnt = 1;

    m_currentAimCnt = 1;
    m_currentDriverFaceCnt = 1;
    m_currentLevelDataCnt = 1;
    m_currentShiftStateCnt = 1;
    m_currentSteeringAngleCnt = 1;
	m_currentBrakePressureCnt = 1;
#ifndef DATA_BUFFER
	m_speedList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
	
	m_disList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
	
	m_roadClassList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
	
	m_accelList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
#endif


    //fire to exe initialize rule
   EnvIncrementGCLocks(m_theEnv);
   void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[FLUSH_TYPE_MODULE_MAIN]);

   EnvFocus(m_theEnv, theModule);
   EnvRun(m_theEnv,-1);
   EnvDecrementGCLocks(m_theEnv);
   return ;

}
void SignalEntryPoint::insert(EventSpeed *speed)
{
    if(speed == NULL) return;
	pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
	if (m_currentSpeedCnt > LIST_MAX_COUNT)
	{
        cout<<"SignalEntryPoint::insert speed more than 1000:"<<m_currentSpeedCnt<<endl;
		pthread_mutex_unlock(&m_mutex);
		return;
	}
	
    //cout<<" SignalEntryPoint::insert(EventSpeed) time: "<<speed->getTime()<<endl;
	SetMFType(m_speedList,m_currentSpeedCnt,FLOAT);
	SetMFValue(m_speedList,m_currentSpeedCnt++,EnvAddDouble(m_theEnv,speed->getTime()));
	
    //cout<<" SignalEntryPoint::insert(EventSpeed) type: "<<speed->getType()<<endl;
	SetMFType(m_speedList,m_currentSpeedCnt,SYMBOL);
	SetMFValue(m_speedList,m_currentSpeedCnt++,EnvAddSymbol(m_theEnv,speed->getTypeString()));
	
    //cout<<" SignalEntryPoint::insert(EventSpeed) speed: "<<speed->getSpeed()<<endl;
	SetMFType(m_speedList,m_currentSpeedCnt,FLOAT);
	SetMFValue(m_speedList,m_currentSpeedCnt++,EnvAddDouble(m_theEnv,speed->getSpeed()));
#else
    m_speedList.push_back(*speed);
    m_currentSpeedCnt++;
#endif
    
#if 1
    EnvIncrementGCLocks(m_theEnv);
    int bRet = SignalEntryPoint::flushSpeed();

    if ( bRet > 0)
    {
        void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[EntryPoint::FLUSH_TYPE_MODULE_SPA_500MS]);
        EnvFocus(m_theEnv, theModule);
        bRet = EnvRun(m_theEnv,-1);
    }

    //m_currentSpeedCnt = 1;
    EnvDecrementGCLocks(m_theEnv);
#endif

	pthread_mutex_unlock(&m_mutex);
	
}
void SignalEntryPoint::insert(EventDistance *distance)
{
    if(distance == NULL) return;

	pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
	if (m_currentDisCnt > LIST_MAX_COUNT)
	{
        cout<<"SignalEntryPoint::insert distance more than 1000:"<<m_currentDisCnt<<endl;
		pthread_mutex_unlock(&m_mutex);
		return;
	}
    //cout<<" SignalEntryPoint::insert(EventDistance) time: "<<distance->getTime()<<endl;
	SetMFType(m_disList,m_currentDisCnt,FLOAT);
	SetMFValue(m_disList,m_currentDisCnt++,EnvAddDouble(m_theEnv,distance->getTime()));
	
    //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
	SetMFType(m_disList,m_currentDisCnt,SYMBOL);
	SetMFValue(m_disList,m_currentDisCnt++,EnvAddSymbol(m_theEnv,distance->getTypeString()));
	
    //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
	SetMFType(m_disList,m_currentDisCnt,FLOAT);
	SetMFValue(m_disList,m_currentDisCnt++,EnvAddDouble(m_theEnv,distance->getDistance()));
#else
    m_disList.push_back(*distance);
    m_currentDisCnt++;
#endif
	pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventRoadClass *rc)
{
     if(rc == NULL) return;
	pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
	if (m_currentRoadClassCnt > LIST_MAX_COUNT)
	{
		cout<<"SignalEntryPoint::insert roadclass more than 1000:"<<m_currentRoadClassCnt<<endl;
		pthread_mutex_unlock(&m_mutex);
		return;
	}
    //cout<<" SignalEntryPoint::insert(EventRoadClass) time: "<<rc->getTime()<<endl;
	SetMFType(m_roadClassList,m_currentRoadClassCnt,FLOAT);
	SetMFValue(m_roadClassList,m_currentRoadClassCnt++,EnvAddDouble(m_theEnv,rc->getTime()));
	
    //cout<<" SignalEntryPoint::insert(EventRoadClass) type: "<<rc->getType()<<endl;
	SetMFType(m_roadClassList,m_currentRoadClassCnt,SYMBOL);
	SetMFValue(m_roadClassList,m_currentRoadClassCnt++,EnvAddSymbol(m_theEnv,rc->getTypeString()));
	
    //cout<<" SignalEntryPoint::insert(EventRoadClass) roadclass: "<<rc->getRoadClass()<<endl;
	SetMFType(m_roadClassList,m_currentRoadClassCnt,SYMBOL);
	SetMFValue(m_roadClassList,m_currentRoadClassCnt++,EnvAddSymbol(m_theEnv,rc->getKindString()));
	
    //cout<<" SignalEntryPoint::insert(EventRoadClass) CanData: "<<rc->getCanData()<<endl;
	SetMFType(m_roadClassList,m_currentRoadClassCnt,INTEGER);
	SetMFValue(m_roadClassList,m_currentRoadClassCnt++,EnvAddLong(m_theEnv,rc->getCanData()));
#else
    m_roadClassList.push_back(*rc);
    m_currentRoadClassCnt++;
#endif
	pthread_mutex_unlock(&m_mutex);
	
}
void SignalEntryPoint::insert(EventAccelOpen *accel)
{
    if(accel == NULL) return;
	pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
	if (m_currentAccelCnt > LIST_MAX_COUNT)
	{
		cout<<"SignalEntryPoint::insert accel more than 1000:"<<m_currentAccelCnt<<endl;
		pthread_mutex_unlock(&m_mutex);
		return;
	}
    //cout<<" SignalEntryPoint::insert(EventAccelOpen) time: "<<accel->getTime()<<endl;
	SetMFType(m_accelList,m_currentAccelCnt,FLOAT);
	SetMFValue(m_accelList,m_currentAccelCnt++,EnvAddDouble(m_theEnv,accel->getTime()));
	
    //cout<<" SignalEntryPoint::insert(EventAccelOpen) type: "<<accel->getType()<<endl;
	SetMFType(m_accelList,m_currentAccelCnt,SYMBOL);
	SetMFValue(m_accelList,m_currentAccelCnt++,EnvAddSymbol(m_theEnv,accel->getTypeString()));
	
    //cout<<" SignalEntryPoint::insert(EventAccelOpen) accel: "<<accel->getAccelOpen()<<endl;
	SetMFType(m_accelList,m_currentAccelCnt,FLOAT);
	SetMFValue(m_accelList,m_currentAccelCnt++,EnvAddDouble(m_theEnv,accel->getAccelOpen()));
#else
    m_accelList.push_back(*accel);
    m_currentAccelCnt++;
#endif
	pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventAimless *aimless)
{
    if(aimless == NULL) return;
    pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
    //TODO
#else
    m_aimLessList.push_back(*aimless);
    m_currentAimCnt++;
#endif
    pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventLevelData *level)
{
    if(level == NULL) return;
    pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
    //TODO
#else
    m_levelDataList.push_back(*level);
    m_currentLevelDataCnt++;
#endif
    pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventShiftState *shift)
{
    if(shift == NULL) return;
    pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
    //TODO
#else
    m_shiftStateList.push_back(*shift);
    m_currentShiftStateCnt++;
#endif
    pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventDriverFace *driverFace)
{
    if(driverFace == NULL) return;
    pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
    //TODO
#else
    m_driverFaceList.push_back(*driverFace);
    m_currentDriverFaceCnt++;
#endif
    pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventSteeringAngle *steeringAngle)
{
    if(steeringAngle == NULL) return;
    pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
    //TODO
#else
    m_steeringAngleList.push_back(*steeringAngle);
    m_currentSteeringAngleCnt++;
#endif
    pthread_mutex_unlock(&m_mutex);
}
void SignalEntryPoint::insert(EventBrakePressure *brakePressure)
{
    if(brakePressure == NULL) return;
    pthread_mutex_lock(&m_mutex);
#ifndef DATA_BUFFER
    //TODO
#else
    m_brakePressureList.push_back(*brakePressure);
    m_currentBrakePressureCnt++;
#endif
    pthread_mutex_unlock(&m_mutex);
}

int SignalEntryPoint::flushSpeed()
{
    int speedRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentSpeedCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_speedTemplate.c_str());
        m_speedFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_speedFact == NULL)
        {
            cout<<" SignalEntryPoint::flush speed faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }
        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));
        EnvPutFactSlot(m_theEnv,m_speedFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_speedList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentSpeedCnt-1);

        EnvPutFactSlot(m_theEnv,m_speedFact,"speedList",&theValue);
#else
        void *multiSpeedList;
        multiSpeedList = EnvCreateMultifield(m_theEnv,(m_currentSpeedCnt-1)*3);

        list<EventSpeed>::iterator iterSpeed = m_speedList.begin();
        int currentSpeedCnt = 1;
        while(iterSpeed != m_speedList.end())
        {
            EventSpeed *speed = &(*iterSpeed);

            //cout<<" SignalEntryPoint::insert(EventSpeed) time: "<<speed->getTime()<<endl;
            SetMFType(multiSpeedList,currentSpeedCnt,FLOAT);
            SetMFValue(multiSpeedList,currentSpeedCnt++,EnvAddDouble(m_theEnv,speed->getTime()));

            //cout<<" SignalEntryPoint::insert(EventSpeed) type: "<<speed->getType()<<endl;
            SetMFType(multiSpeedList,currentSpeedCnt,SYMBOL);
            SetMFValue(multiSpeedList,currentSpeedCnt++,EnvAddSymbol(m_theEnv,speed->getTypeString()));

            //cout<<" SignalEntryPoint::insert(EventSpeed) speed: "<<speed->getSpeed()<<endl;
            SetMFType(multiSpeedList,currentSpeedCnt,FLOAT);
            SetMFValue(multiSpeedList,currentSpeedCnt++,EnvAddDouble(m_theEnv,speed->getSpeed()));
            iterSpeed++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiSpeedList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentSpeedCnt-1);

        EnvPutFactSlot(m_theEnv,m_speedFact,"speedList",&theValue);
        m_speedList.clear();
        m_currentSpeedCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_speedFact);
        EnvAssert(m_theEnv,m_speedFact);
        speedRet = 1;

    }
    return speedRet;
}

int SignalEntryPoint::flushDistance()
{
    int distRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentDisCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_disTemplate.c_str());
        m_disFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_disFact == NULL)
        {
            cout<<" SignalEntryPoint::flush distance faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_disFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
#else
        void *multiDisList;
        multiDisList = EnvCreateMultifield(m_theEnv,(m_currentDisCnt-1)*3);

        list<EventDistance>::iterator iterDis = m_disList.begin();
        int currentDisCnt = 1;
        while(iterDis != m_disList.end())
        {
            EventDistance *distance = &(*iterDis);

			//cout<<" SignalEntryPoint::Flush(EventDistance) time: "<<distance->getTime()<<endl;
            SetMFType(multiDisList,currentDisCnt,FLOAT);
            SetMFValue(multiDisList,currentDisCnt++,EnvAddDouble(m_theEnv,distance->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiDisList,currentDisCnt,SYMBOL);
            SetMFValue(multiDisList,currentDisCnt++,EnvAddSymbol(m_theEnv,distance->getTypeString()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
            SetMFType(multiDisList,currentDisCnt,FLOAT);
            SetMFValue(multiDisList,currentDisCnt++,EnvAddDouble(m_theEnv,distance->getDistance()));
            iterDis++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiDisList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
        m_disList.clear();
        m_currentDisCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_disFact);
        EnvAssert(m_theEnv,m_disFact);
        distRet = 1;
    }
    return distRet;
}

int SignalEntryPoint::flushAccel()
{
    int accelRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentAccelCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_accelOpenTemplate.c_str());
        m_accelFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_accelFact == NULL)
        {
            cout<<" SignalEntryPoint::flush accel faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_accelFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_accelList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentAccelCnt-1);

        EnvPutFactSlot(m_theEnv,m_accelFact,"accelOpenList",&theValue);
#else
        void *multiAccelList;
        multiAccelList = EnvCreateMultifield(m_theEnv,(m_currentAccelCnt-1)*3);

        list<EventAccelOpen>::iterator iterAccel = m_accelList.begin();
        int currentAccelCnt = 1;
        while(iterAccel != m_accelList.end())
        {
            EventAccelOpen *accel = &(*iterAccel);

            //cout<<" SignalEntryPoint::insert(EventAccelOpen) time: "<<accel->getTime()<<endl;
            SetMFType(multiAccelList,currentAccelCnt,FLOAT);
            SetMFValue(multiAccelList,currentAccelCnt++,EnvAddDouble(m_theEnv,accel->getTime()));

            //cout<<" SignalEntryPoint::insert(EventAccelOpen) type: "<<accel->getType()<<endl;
            SetMFType(multiAccelList,currentAccelCnt,SYMBOL);
            SetMFValue(multiAccelList,currentAccelCnt++,EnvAddSymbol(m_theEnv,accel->getTypeString()));

            //cout<<" SignalEntryPoint::insert(EventAccelOpen) accel: "<<accel->getAccelOpen()<<endl;
            SetMFType(multiAccelList,currentAccelCnt,FLOAT);
            SetMFValue(multiAccelList,currentAccelCnt++,EnvAddDouble(m_theEnv,accel->getAccelOpen()));

            iterAccel++;
        }

        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiAccelList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentAccelCnt-1);

        EnvPutFactSlot(m_theEnv,m_accelFact,"accelOpenList",&theValue);
        m_accelList.clear();
        m_currentAccelCnt = 1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_accelFact);
        EnvAssert(m_theEnv,m_accelFact);
        accelRet = 1;
    }
    return accelRet;
}

int SignalEntryPoint::flushRoadKind()
{
    int roadRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentRoadClassCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_roadClassTemplate.c_str());
        m_roadClassFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_roadClassFact == NULL)
        {
            cout<<" SignalEntryPoint::flush roadClass faild: "<<endl;
           // EnvDecrementGCLocks(m_theEnv);
           // pthread_mutex_unlock(&m_mutex);
            return -1;
        }
        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));
        EnvPutFactSlot(m_theEnv,m_roadClassFact,"name",&theValue);
        //theValue[1].type = MULTIFIELD;
        //theValue[1].value = m_roadClassList;
#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_roadClassList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentRoadClassCnt-1);

        EnvPutFactSlot(m_theEnv,m_roadClassFact,"roadClassList",&theValue);
#else
        void *multiRoadClassList;
        multiRoadClassList = EnvCreateMultifield(m_theEnv,(m_currentRoadClassCnt-1)*4);

        list<EventRoadClass>::iterator iterRCL = m_roadClassList.begin();
        int currentRoadClassCnt = 1;
        while(iterRCL != m_roadClassList.end())
        {
            EventRoadClass *rc = &(*iterRCL);
            //cout<<" SignalEntryPoint::insert(EventRoadClass) time: "<<rc->getTime()<<endl;
            SetMFType(multiRoadClassList,currentRoadClassCnt,FLOAT);
            SetMFValue(multiRoadClassList,currentRoadClassCnt++,EnvAddDouble(m_theEnv,rc->getTime()));

            //cout<<" SignalEntryPoint::insert(EventRoadClass) type: "<<rc->getType()<<endl;
            SetMFType(multiRoadClassList,currentRoadClassCnt,SYMBOL);
            SetMFValue(multiRoadClassList,currentRoadClassCnt++,EnvAddSymbol(m_theEnv,rc->getTypeString()));

            //cout<<" SignalEntryPoint::insert(EventRoadClass) roadclass: "<<rc->getRoadClass()<<endl;
            SetMFType(multiRoadClassList,currentRoadClassCnt,SYMBOL);
            SetMFValue(multiRoadClassList,currentRoadClassCnt++,EnvAddSymbol(m_theEnv,rc->getKindString()));

            //cout<<" SignalEntryPoint::insert(EventRoadClass) CanData: "<<rc->getCanData()<<endl;
            SetMFType(multiRoadClassList,currentRoadClassCnt,INTEGER);
            SetMFValue(multiRoadClassList,currentRoadClassCnt++,EnvAddLong(m_theEnv,rc->getCanData()));

            iterRCL++;
        }

        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiRoadClassList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentRoadClassCnt-1);

        EnvPutFactSlot(m_theEnv,m_roadClassFact,"roadClassList",&theValue);
        m_roadClassList.clear();
        m_currentRoadClassCnt = 1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_roadClassFact);
        EnvAssert(m_theEnv,m_roadClassFact);
        roadRet = 1;
    }
    return roadRet;
}
int SignalEntryPoint::flushBrakePressure()
{
    int bRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentBrakePressureCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_brakePressureTemplate.c_str());
        m_brakePressureFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_brakePressureFact == NULL)
        {
            cout<<" SignalEntryPoint::flush brakePressure faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_brakePressureFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
#else
        void *multiBList;
        multiBList = EnvCreateMultifield(m_theEnv,(m_currentBrakePressureCnt-1)*3);

        list<EventBrakePressure>::iterator iter = m_brakePressureList.begin();
        int currentBrakePressureCnt = 1;
        while(iter != m_brakePressureList.end())
        {
            EventBrakePressure *breakPressure = &(*iter);

            //cout<<" SignalEntryPoint::Flush(EventBrakePressure) time: "<<breakPressure->getTime()<<endl;
            SetMFType(multiBList,currentBrakePressureCnt,FLOAT);
            SetMFValue(multiBList,currentBrakePressureCnt++,EnvAddDouble(m_theEnv,breakPressure->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiBList,currentBrakePressureCnt,SYMBOL);
            SetMFValue(multiBList,currentBrakePressureCnt++,EnvAddSymbol(m_theEnv,breakPressure->getType().c_str()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
            SetMFType(multiBList,currentBrakePressureCnt,FLOAT);
            SetMFValue(multiBList,currentBrakePressureCnt++,EnvAddDouble(m_theEnv,breakPressure->getValue()));
            iter++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiBList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentBrakePressureCnt-1);

        EnvPutFactSlot(m_theEnv,m_brakePressureFact,"brakePressureList",&theValue);
        m_brakePressureList.clear();
        m_currentBrakePressureCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_brakePressureFact);
        EnvAssert(m_theEnv,m_brakePressureFact);
        bRet = 1;
    }
    return bRet;
}
int SignalEntryPoint::flushAimless()
{
    int bRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentAimCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_aimLessTemplate.c_str());
        m_aimLessFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_aimLessFact == NULL)
        {
            cout<<" SignalEntryPoint::flush brakePressure faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_aimLessFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
#else
        void *multiAList;
        multiAList = EnvCreateMultifield(m_theEnv,(m_currentAimCnt-1)*3);

        list<EventAimless>::iterator iter = m_aimLessList.begin();
        int currentAimCnt = 1;
        while(iter != m_aimLessList.end())
        {
            EventAimless *aimLess = &(*iter);

            //cout<<" SignalEntryPoint::Flush(EventBrakePressure) time: "<<breakPressure->getTime()<<endl;
            SetMFType(multiAList,currentAimCnt,FLOAT);
            SetMFValue(multiAList,currentAimCnt++,EnvAddDouble(m_theEnv,aimLess->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiAList,currentAimCnt,SYMBOL);
            SetMFValue(multiAList,currentAimCnt++,EnvAddSymbol(m_theEnv,aimLess->getType().c_str()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
            SetMFType(multiAList,currentAimCnt,FLOAT);
            SetMFValue(multiAList,currentAimCnt++,EnvAddDouble(m_theEnv,aimLess->getValue()));
            iter++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiAList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentAimCnt-1);

        EnvPutFactSlot(m_theEnv,m_aimLessFact,"aimlessList",&theValue);
        m_aimLessList.clear();
        m_currentAimCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_aimLessFact);
        EnvAssert(m_theEnv,m_aimLessFact);
        bRet = 1;
    }
    return bRet;
}

int SignalEntryPoint::flushDriverFace()
{
    int bRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentDriverFaceCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_driverFaceTemplate.c_str());
        m_driverFaceFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_driverFaceFact == NULL)
        {
            cout<<" SignalEntryPoint::flush brakePressure faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_driverFaceFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
#else
        void *multiDList;
        multiDList = EnvCreateMultifield(m_theEnv,(m_currentDriverFaceCnt-1)*3);

        list<EventDriverFace>::iterator iter = m_driverFaceList.begin();
        int currentDriverCnt = 1;
        while(iter != m_driverFaceList.end())
        {
            EventDriverFace *driverFace = &(*iter);

            //cout<<" SignalEntryPoint::Flush(EventBrakePressure) time: "<<breakPressure->getTime()<<endl;
            SetMFType(multiDList,currentDriverCnt,FLOAT);
            SetMFValue(multiDList,currentDriverCnt++,EnvAddDouble(m_theEnv,driverFace->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiDList,currentDriverCnt,SYMBOL);
            SetMFValue(multiDList,currentDriverCnt++,EnvAddSymbol(m_theEnv,driverFace->getType().c_str()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
            SetMFType(multiDList,currentDriverCnt,FLOAT);
            SetMFValue(multiDList,currentDriverCnt++,EnvAddDouble(m_theEnv,driverFace->getValue()));
            iter++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiDList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentDriverCnt-1);

        EnvPutFactSlot(m_theEnv,m_driverFaceFact,"driverFaceList",&theValue);
        m_driverFaceList.clear();
        m_currentDriverFaceCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_driverFaceFact);
        EnvAssert(m_theEnv,m_driverFaceFact);
        bRet = 1;
    }
    return bRet;
}

int SignalEntryPoint::flushLoadLevel()
{
    int bRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentLevelDataCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_levelDataTemplate.c_str());
        m_levelDataFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_levelDataFact == NULL)
        {
            cout<<" SignalEntryPoint::flush brakePressure faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_levelDataFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
#else
        void *multiLList;
        multiLList = EnvCreateMultifield(m_theEnv,(m_currentLevelDataCnt-1)*3);

        list<EventLevelData>::iterator iter = m_levelDataList.begin();
        int currentLevelCnt = 1;
        while(iter != m_levelDataList.end())
        {
            EventLevelData *levelData = &(*iter);

            //cout<<" SignalEntryPoint::Flush(EventBrakePressure) time: "<<breakPressure->getTime()<<endl;
            SetMFType(multiLList,currentLevelCnt,FLOAT);
            SetMFValue(multiLList,currentLevelCnt++,EnvAddDouble(m_theEnv,levelData->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiLList,currentLevelCnt,SYMBOL);
            SetMFValue(multiLList,currentLevelCnt++,EnvAddSymbol(m_theEnv,levelData->getType().c_str()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
            SetMFType(multiLList,currentLevelCnt,INTEGER);
            SetMFValue(multiLList,currentLevelCnt++,EnvAddLong(m_theEnv,levelData->getValue()));
            iter++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiLList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentLevelCnt-1);

        EnvPutFactSlot(m_theEnv,m_levelDataFact,"levelDataList",&theValue);
        m_levelDataList.clear();
        m_currentLevelDataCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_levelDataFact);
        EnvAssert(m_theEnv,m_levelDataFact);
        bRet = 1;
    }
    return bRet;
}

int SignalEntryPoint::flushShiftState()
{
    int bRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentShiftStateCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_shiftStateTemplate.c_str());
        m_shiftStateFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_shiftStateFact == NULL)
        {
            cout<<" SignalEntryPoint::flush brakePressure faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_shiftStateFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

		EnvPutFactSlot(m_theEnv,m_disFact,"shiftStateList",&theValue);
#else
        void *multiSList;
        multiSList = EnvCreateMultifield(m_theEnv,(m_currentShiftStateCnt-1)*3);

        list<EventShiftState>::iterator iter = m_shiftStateList.begin();
        int currentShiftCnt = 1;
        while(iter != m_shiftStateList.end())
        {
            EventShiftState *shiftState = &(*iter);

            //cout<<" SignalEntryPoint::Flush(EventBrakePressure) time: "<<breakPressure->getTime()<<endl;
            SetMFType(multiSList,currentShiftCnt,FLOAT);
            SetMFValue(multiSList,currentShiftCnt++,EnvAddDouble(m_theEnv,shiftState->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiSList,currentShiftCnt,SYMBOL);
            SetMFValue(multiSList,currentShiftCnt++,EnvAddSymbol(m_theEnv,shiftState->getType().c_str()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
			SetMFType(multiSList,currentShiftCnt,SYMBOL);
			switch ((int)shiftState->getValue())
			{
			case 1: // REVERSE
				SetMFValue(multiSList,currentShiftCnt++,EnvAddSymbol(m_theEnv,"REVERSE"));
				break;
			case 2: // STOP
				SetMFValue(multiSList,currentShiftCnt++,EnvAddSymbol(m_theEnv,"STOP"));
				break;
			case 3: // DRIVE
			default:
				SetMFValue(multiSList,currentShiftCnt++,EnvAddSymbol(m_theEnv,"DRIVE"));
				break;
			}
			//SetMFValue(multiSList,currentShiftCnt++,EnvAddDouble(m_theEnv,shiftState->getValue()));
			iter++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiSList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentShiftCnt-1);

        EnvPutFactSlot(m_theEnv,m_shiftStateFact,"shiftStateList",&theValue);
        m_shiftStateList.clear();
        m_currentShiftStateCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_shiftStateFact);
        EnvAssert(m_theEnv,m_shiftStateFact);
        bRet = 1;
    }
    return bRet;
}

int SignalEntryPoint::flushSteeringAngle()
{
    int bRet = 0;
    DATA_OBJECT theValue;
    void *templatePtr;
    if (m_currentSteeringAngleCnt > 1)
    {
        templatePtr = EnvFindDeftemplate(m_theEnv,m_steeringAngleTemplate.c_str());
        m_steeringAngleFact = EnvCreateFact(m_theEnv,templatePtr);
        if (m_steeringAngleFact == NULL)
        {
            cout<<" SignalEntryPoint::flush brakePressure faild: "<<endl;
            //EnvDecrementGCLocks(m_theEnv);
            //pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        SetpType(&theValue,STRING);
        SetpValue(&theValue,EnvAddSymbol(m_theEnv,m_streamName.c_str()));

        EnvPutFactSlot(m_theEnv,m_steeringAngleFact,"name",&theValue);

#ifndef DATA_BUFFER
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,m_disList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,m_currentDisCnt-1);

        EnvPutFactSlot(m_theEnv,m_disFact,"distanceList",&theValue);
#else
        void *multiSList;
        multiSList = EnvCreateMultifield(m_theEnv,(m_currentSteeringAngleCnt-1)*3);

        list<EventSteeringAngle>::iterator iter = m_steeringAngleList.begin();
        int currentCnt = 1;
        while(iter != m_steeringAngleList.end())
        {
            EventSteeringAngle *steering = &(*iter);

            //cout<<" SignalEntryPoint::Flush(EventBrakePressure) time: "<<breakPressure->getTime()<<endl;
            SetMFType(multiSList,currentCnt,FLOAT);
            SetMFValue(multiSList,currentCnt++,EnvAddDouble(m_theEnv,steering->getTime()));

            //cout<<" SignalEntryPoint::insert(EventDistance) type: "<<distance->getType()<<endl;
            SetMFType(multiSList,currentCnt,SYMBOL);
            SetMFValue(multiSList,currentCnt++,EnvAddSymbol(m_theEnv,steering->getType().c_str()));

            //cout<<" SignalEntryPoint::insert(EventDistance) distance: "<<distance->getDistance()<<endl;
            SetMFType(multiSList,currentCnt,FLOAT);
            SetMFValue(multiSList,currentCnt++,EnvAddDouble(m_theEnv,steering->getValue()));
            iter++;
        }
        SetpType(&theValue,MULTIFIELD);
        SetpValue(&theValue,multiSList);

        SetpDOBegin(&theValue,1);
        SetpDOEnd(&theValue,currentCnt-1);

        EnvPutFactSlot(m_theEnv,m_steeringAngleFact,"steeringAngleList",&theValue);
        m_steeringAngleList.clear();
        m_currentSteeringAngleCnt =1;
#endif
        EnvAssignFactSlotDefaults(m_theEnv,m_steeringAngleFact);
        EnvAssert(m_theEnv,m_steeringAngleFact);
        bRet = 1;
    }
    return bRet;
}

void SignalEntryPoint::flush(FLUSH_TYPE type)
{
    extern double g_start_end_time, g_start_time;
	if (g_start_end_time == 0.0)
	{
		struct timeval t_end;
		gettimeofday(&t_end, NULL);
		g_start_end_time  = ((double)t_end.tv_sec)*1000+(double)t_end.tv_usec/1000;
		double cost_time = g_start_end_time - g_start_time;
		cout<< fixed << "=====SignalEntryPoint::flush:" << cost_time << "========" << endl;
	}

	if(type >= FLUSH_TYPE_MODULE_MAX ) return;

    if(type == FLUSH_TYPE_MODULE_SPA_10S)
    {
         EnvIncrementGCLocks(m_theEnv);
        void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[type]);

        EnvFocus(m_theEnv, theModule);
        EnvRun(m_theEnv,-1);
        EnvDecrementGCLocks(m_theEnv);
        return ;
    }

    int bRet = 0;
    int nRet = 0;

	pthread_mutex_lock(&m_mutex);

    EnvIncrementGCLocks(m_theEnv);
	

#if 0
    bRet = flushSpeed();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
#endif
#if 0    
    bRet = flushRoadKind();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;

    bRet = flushDistance();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;

    bRet = flushAccel();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;

    bRet = flushBrakePressure();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
    bRet = flushShiftState();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
    bRet = flushAimless();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
    bRet = flushDriverFace();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
    bRet = flushLoadLevel();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
    bRet = flushSteeringAngle();
    if(bRet == -1)
    {
        EnvDecrementGCLocks(m_theEnv);
        pthread_mutex_unlock(&m_mutex);
        return;
    }
    nRet +=bRet;
#ifndef DATA_BUFFER
	m_currentSpeedCnt = 1;
	m_currentDisCnt = 1;
	m_currentRoadClassCnt = 1;
	m_currentAccelCnt = 1;
#endif
#endif
#ifndef DATA_BUFFER
	void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[type]);
	
	EnvFocus(m_theEnv, theModule);
	EnvRun(m_theEnv,-1);
	

#else
    if ( nRet > 0)
    {
        void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[type]);

        EnvFocus(m_theEnv, theModule);
        nRet = EnvRun(m_theEnv,-1);

        //cout<<" SignalEntryPoint::flush  (" << nRet << ") fired" << endl;

    }
#endif
    EnvDecrementGCLocks(m_theEnv);
    //EnvFacts(m_theEnv,"wdisplay",NULL,-1,-1,-1);
#ifndef DATA_BUFFER
    //if (roadRet)
		m_roadClassList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
    //if (distRet)
		m_disList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
    //if (speedRet)
		m_speedList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
    //if (accelRet)
		m_accelList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);
#endif
	pthread_mutex_unlock(&m_mutex);
}
