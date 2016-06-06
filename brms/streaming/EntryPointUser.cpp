extern "C"
{
	#include "clips.h"
}
#include <iostream>
#include <fstream>
#include "EntryPointUser.h"


#define LIST_MAX_COUNT 200


//pthread_mutex_t EntryPointUser::m_mutex = PTHREAD_MUTEX_INITIALIZER;

EntryPointUser::EntryPointUser():
    m_binomialTemplate("EventCharacteristicAndStatusBinomial")
    , m_3sigmaTemplate("EventCharacteristicAndStatus3sigma")
{
}


void EntryPointUser::insert(EventCharacteristicAndStatusBinomial &status)
{
    //m_binomialList.push_back(status);
    EnvIncrementGCLocks(m_theEnv);
    DATA_OBJECT theValue;
    void *templatePtr = EnvFindDeftemplate(m_theEnv,m_binomialTemplate.c_str());
    void *newFact = EnvCreateFact(m_theEnv,templatePtr);


    theValue.type = STRING;
    theValue.value = EnvAddSymbol(m_theEnv,"Characteristic And Status Stream");
    EnvPutFactSlot(m_theEnv,newFact,"name",&theValue);


    theValue.type = INTEGER;
    theValue.value = EnvAddLong(m_theEnv,status.getMaxSampleSize());
    EnvPutFactSlot(m_theEnv,newFact,"MAX_SAMPLE_SIZE",&theValue);

    theValue.type = INTEGER;
    theValue.value = EnvAddLong(m_theEnv,status.getUserCheckSize());
    EnvPutFactSlot(m_theEnv,newFact,"STATE_CHECK_SIZE",&theValue);

    theValue.type = INTEGER;
    theValue.value = EnvAddLong(m_theEnv,status.DEQUE_SIZE);
    EnvPutFactSlot(m_theEnv,newFact,"DEQUE_SIZE",&theValue);

    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(status.getType()));
    EnvPutFactSlot(m_theEnv,newFact,"type",&theValue);

    m_binomialList.insert(pair<string, EventCharacteristicAndStatusBinomial>(EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(status.getType()),status));

    m_binomiaSpeclList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);

    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> sampleList = status.getSampling();
    //queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL>::iterator itr = sampleList.begin();
	int size = sampleList.size();
	for (int i = size; i > 0; i--)
    {
        SetMFType(m_binomiaSpeclList,i,SYMBOL);
		SetMFValue(m_binomiaSpeclList,i,EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialName(sampleList.front())));
        sampleList.pop();
    }

    theValue.type = MULTIFIELD;
    theValue.value = m_binomiaSpeclList;

    SetpDOBegin(&theValue,1);
	SetpDOEnd(&theValue,size);
    EnvPutFactSlot(m_theEnv,newFact,"specificList",&theValue);

    //T.B.D status.getSpecific()
    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialName(status.getSpecificENUM()));
    EnvPutFactSlot(m_theEnv,newFact,"specific",&theValue);

    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getPercentage());
    EnvPutFactSlot(m_theEnv,newFact,"percentage",&theValue);

    theValue.type = SYMBOL;
    if (status.getUserStateCheck() == false)
    {
        theValue.value = EnvFalseSymbol(m_theEnv);
    }
    else
    {
        theValue.value = EnvTrueSymbol(m_theEnv);
    }
    EnvPutFactSlot(m_theEnv,newFact,"userStateCheck",&theValue);


    theValue.type = STRING;
    theValue.value = EnvAddSymbol(m_theEnv,status.getUserState().c_str());
    EnvPutFactSlot(m_theEnv,newFact,"userState",&theValue);

    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getOldSamplePercentage());
    EnvPutFactSlot(m_theEnv,newFact,"oldSamplePercentage",&theValue);

    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getTodaySpecificState());
    EnvPutFactSlot(m_theEnv,newFact,"todaySpecificState",&theValue);

    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialName(status.getOldSampleSpecific()));
    EnvPutFactSlot(m_theEnv,newFact,"oldSampleSpecific",&theValue);

    EnvAssignFactSlotDefaults(m_theEnv,newFact);
    EnvAssert(m_theEnv,newFact);
#if 0
    void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[FLUSH_TYPE_MODULE_SPA_500MS]);
    EnvFocus(m_theEnv, theModule);

    EnvRun(m_theEnv,-1);

#endif

    EnvDecrementGCLocks(m_theEnv);
}
void EntryPointUser::insert(EventCharacteristicAndStatus3sigma &status)
{
    //m_3sigmaList.push_back(status);
    EnvIncrementGCLocks(m_theEnv);
    DATA_OBJECT theValue;
    void *templatePtr = EnvFindDeftemplate(m_theEnv,m_3sigmaTemplate.c_str());
    void *newFact = EnvCreateFact(m_theEnv,templatePtr);


    theValue.type = STRING;
    theValue.value = EnvAddSymbol(m_theEnv,"Characteristic And Status Stream");
    EnvPutFactSlot(m_theEnv,newFact,"name",&theValue);


    theValue.type = INTEGER;
    theValue.value = EnvAddLong(m_theEnv,status.getMaxSampleSize());
    EnvPutFactSlot(m_theEnv,newFact,"MAX_SAMPLE_SIZE",&theValue);

    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(status.getType()));
    EnvPutFactSlot(m_theEnv,newFact,"type",&theValue);

    m_3sigmaList.insert(pair<string, EventCharacteristicAndStatus3sigma>(EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(status.getType()),status));

    m_3sigmaSpecList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);

    queue<double> sampleList = status.getSamplingList();
   // queue<double>::iterator itr = sampleList.begin();
    int size = sampleList.size();
	for (int i = size; i > 0; i--)
	{
        SetMFType(m_3sigmaSpecList,i,FLOAT);
		SetMFValue(m_3sigmaSpecList,i,EnvAddDouble(m_theEnv,sampleList.front()));
        sampleList.pop();
    }

    theValue.type = MULTIFIELD;
    theValue.value = m_3sigmaSpecList;

	SetpDOBegin(&theValue, 1);
	SetpDOEnd(&theValue, size);
    EnvPutFactSlot(m_theEnv,newFact,"samplingList",&theValue);

#if 0
    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatus3sigma::getSpec3sigmaName(status.specific));
    EnvPutFactSlot(m_theEnv,newFact,"specific",&theValue);

#endif

    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getAverage());
    EnvPutFactSlot(m_theEnv,newFact,"average",&theValue);

    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getStdDeviation());
    EnvPutFactSlot(m_theEnv,newFact,"stdDeviation",&theValue);

    theValue.type = SYMBOL;
    if (status.getUserStateCheck() == false)
    {
        theValue.value = EnvFalseSymbol(m_theEnv);
    }
    else
    {
        theValue.value = EnvTrueSymbol(m_theEnv);
    }
    EnvPutFactSlot(m_theEnv,newFact,"userStateCheck",&theValue);

    theValue.type = STRING;
    theValue.value = EnvAddSymbol(m_theEnv,status.getUserState().c_str());
    EnvPutFactSlot(m_theEnv,newFact,"userState",&theValue);

    theValue.type = INTEGER;
    theValue.value = EnvAddLong(m_theEnv,status.getOutlierValCount());
    EnvPutFactSlot(m_theEnv,newFact,"outlierValCount",&theValue);


    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getOldSampleAverage());
    EnvPutFactSlot(m_theEnv,newFact,"oldSampleAverage",&theValue);

    theValue.type = FLOAT;
    theValue.value = EnvAddDouble(m_theEnv,status.getOldSampleStdDeviation());
    EnvPutFactSlot(m_theEnv,newFact,"oldSampleStdDeviation",&theValue);


    EnvAssignFactSlotDefaults(m_theEnv,newFact);
    EnvAssert(m_theEnv,newFact);

#if 0
    void *theModule = EnvFindDefmodule(m_theEnv, m_strTypeNameList[FLUSH_TYPE_MODULE_SPA_500MS]);
    EnvFocus(m_theEnv, theModule);

    EnvRun(m_theEnv,-1);
#endif
    EnvDecrementGCLocks(m_theEnv);

}
void EntryPointUser::updateBinomial(void *factPtr, EventCharacteristicAndStatusBinomial &status)
{
    DATA_OBJECT theValue;

    //theValue.type = INTEGER;
    //theValue.value = EnvAddLong(m_theEnv,status.getMaxSampleSize());
    EnvGetFactSlot(m_theEnv,factPtr,"MAX_SAMPLE_SIZE",&theValue);
    int type = GetpType(&theValue);
    if (type == INTEGER)
    {
        int  maxSize = ValueToInteger(GetpValue(&theValue));
        status.setMaxSampleSize(maxSize);
    }

    //theValue.type = INTEGER;
    //theValue.value = EnvAddLong(m_theEnv,status.getUserCheckSize());
    EnvGetFactSlot(m_theEnv,factPtr,"STATE_CHECK_SIZE",&theValue);
    type = GetpType(&theValue);
    if (type == INTEGER)
    {
        int  stateCheckSize = ValueToInteger(GetpValue(&theValue));
        status.STATE_CHECK_SIZE = stateCheckSize;
    }


    //theValue.type = INTEGER;
    //theValue.value = EnvAddLong(m_theEnv,status.getSamplingCount());
    EnvGetFactSlot(m_theEnv,factPtr,"DEQUE_SIZE",&theValue);
    type = GetpType(&theValue);
    if (type == INTEGER)
    {
        int  dequekSize = ValueToInteger(GetpValue(&theValue));
        status.DEQUE_SIZE = dequekSize;
    }
#if 0
    m_binomiaSpeclList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);

    queue<SPECIFIC_BINOMIAL> sampleList = status.getSampling();
    queue<SPECIFIC_BINOMIAL>::iterator itr = sampleList.begin();
    int i = 1;
    while(itr != sampleList.end())
    {
        SetMFType(m_binomiaSpeclList,i,SYMBOL);
        SetMFValue(m_binomiaSpeclList,i++,EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialName(*itr)));
    }

    theValue.type = MULTIFIELD;
    theValue.value = m_binomiaSpeclList;

    SetpDOBegin(&theValue,1);
    SetpDOEnd(&theValue,i-1);
    EnvPutFactSlot(environment,newFact,"specificList",&theValue);
#else
    status.resetSample();
     EnvGetFactSlot(m_theEnv,factPtr,"specificList",&theValue);
     type = GetpType(&theValue);
     if (type == MULTIFIELD)
     {
         void *multi = GetpValue(&theValue);
         int begin = GetDOBegin(theValue);
         int end = GetDOEnd(theValue);
		 if (end - begin + 1 > status.getMaxSampleSize())
		 {
			 end = begin + status.getMaxSampleSize() - 1;
		 }
         for (int i = end; i >= begin; i--)
         {
             if ((GetMFType(multi,i) == STRING) ||
             (GetMFType(multi,i) == SYMBOL))
             {
                string tempPtr = ValueToString(GetMFValue(multi,i));
               // count += strlen(tempPtr);

                status.addSample(EventCharacteristicAndStatusBinomial::getSpecBinomialName(tempPtr));
             }
         }
         //string  specName = ValueToString();
         //status.setSpecific(EventCharacteristicAndStatusBinomial::getSpecBinomialName(specName));

     }
#endif

    //T.B.D status.getSpecific()
    //theValue.type = SYMBOL;
    //theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialName(status.getSpecificENUM()));
    EnvGetFactSlot(m_theEnv,factPtr,"specific",&theValue);
    type = GetpType(&theValue);
    if (type == SYMBOL)
    {
        string  specName = ValueToString(GetpValue(&theValue));
        status.setSpecific(EventCharacteristicAndStatusBinomial::getSpecBinomialName(specName));
    }


    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getPercentage());
    EnvGetFactSlot(m_theEnv,factPtr,"percentage",&theValue);
    type = GetpType(&theValue);
    if (type == FLOAT)
    {
        double  percentage = ValueToDouble(GetpValue(&theValue));
        status.setPercentage(percentage);
    }

#if 0
    theValue.type = SYMBOL;
    if (status.getUserStateCheck() == false)
    {
        theValue.value = EnvFalseSymbol(m_theEnv);
    }
    else
    {
        theValue.value = EnvTrueSymbol(m_theEnv);
    }
    EnvPutFactSlot(m_theEnv,newFact,"userStateCheck",&theValue);


    theValue.type = STRING;
    theValue.value = EnvAddSymbol(m_theEnv,status.getUserState().c_str());
    EnvPutFactSlot(m_theEnv,newFact,"userState",&theValue);
#endif
    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getOldSamplePercentage());
    EnvGetFactSlot(m_theEnv,factPtr,"oldSamplePercentage",&theValue);
    type = GetpType(&theValue);
    if (type == FLOAT)
    {
        double  percentage = ValueToDouble(GetpValue(&theValue));
        //status.setPercentage(percentage);
        status.setOldSamplePercentage(percentage);
    }


    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getTodaySpecificState());
    EnvGetFactSlot(m_theEnv,factPtr,"todaySpecificState",&theValue);
    type = GetpType(&theValue);
    if (type == FLOAT)
    {
        double  todaySpecificState = ValueToDouble(GetpValue(&theValue));
        status.setTodaySpecificState(todaySpecificState);
    }
#if 0
    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatusBinomial::getSpecBinomialName(status.getOldSampleSpecific()));
    EnvPutFactSlot(m_theEnv,newFact,"oldSampleSpecific",&theValue);

#endif
}
void EntryPointUser::update3sigma(void *factPtr, EventCharacteristicAndStatus3sigma &status)
{
    DATA_OBJECT outputValue;
    //theValue.type = INTEGER;
   // theValue.value = EnvAddLong(m_theEnv,status.getMaxSampleSize());
    EnvPutFactSlot(m_theEnv,factPtr,"MAX_SAMPLE_SIZE",&outputValue);
    int type = GetpType(&outputValue);
    if (type == INTEGER)
    {
        int maxSampleSize = ValueToInteger(GetpValue(&outputValue));
        status.setMaxSampleSize(maxSampleSize);
    }

#if 0
    m_3sigmaSpecList = EnvCreateMultifield(m_theEnv,LIST_MAX_COUNT);

    queue<double> sampleList = status.getSamplingList();
    queue<double>::iterator itr = sampleList.begin();
    int i = 1;
    while(itr != sampleList.end())
    {
        SetMFType(m_3sigmaSpecList,i,FLOAT);
        SetMFValue(m_3sigmaSpecList,i++,EnvAddDouble(m_theEnv,*itr));
    }

    theValue.type = MULTIFIELD;
    theValue.value = m_3sigmaSpecList;

    SetpDOBegin(&theValue,1);
    SetpDOEnd(&theValue,i-1);
    EnvPutFactSlot(environment,newFact,"samplingList",&theValue);
#else
    status.resetSample();
    EnvGetFactSlot(m_theEnv,factPtr,"samplingList",&outputValue);
    type = GetpType(&outputValue);
    if (type == MULTIFIELD)
    {
        void *multi = GetpValue(&outputValue);
        int begin = GetDOBegin(outputValue);
        int end = GetDOEnd(outputValue);
		if (end - begin + 1 > status.getMaxSampleSize())
		{
			end = begin + status.getMaxSampleSize() - 1;
		}
		for (int i = end; i >= begin; i--)
        {
            if (GetMFType(multi,i) == FLOAT)
            {
               double temp = ValueToDouble(GetMFValue(multi,i));
              // count += strlen(tempPtr);
               status.addSample(temp);
            }
        }
    }
#endif
#if 0
    theValue.type = SYMBOL;
    theValue.value = EnvAddSymbol(m_theEnv,EventCharacteristicAndStatus3sigma::getSpec3sigmaName(status.specific));
    EnvPutFactSlot(m_theEnv,newFact,"specific",&theValue);

#endif

    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getAverage());
    EnvGetFactSlot(m_theEnv,factPtr,"average",&outputValue);
    type = GetpType(&outputValue);
    if (type == FLOAT)
    {
        double temp = ValueToDouble(GetpValue(&outputValue));
        status.setAverage(temp);
    }

    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getStdDeviation());
    EnvGetFactSlot(m_theEnv,factPtr,"stdDeviation",&outputValue);
    type = GetpType(&outputValue);
    if (type == FLOAT)
    {
        double temp = ValueToDouble(GetpValue(&outputValue));
        status.setStdDeviation(temp);
    }
#if 0
    theValue.type = SYMBOL;
    if (status.getUserStateCheck() == false)
    {
        theValue.value = EnvFalseSymbol(m_theEnv);
    }
    else
    {
        theValue.value = EnvTrueSymbol(m_theEnv);
    }
    EnvPutFactSlot(m_theEnv,newFact,"userStateCheck",&theValue);

    theValue.type = STRING;
    theValue.value = EnvAddSymbol(m_theEnv,status.getUserState().c_str());
    EnvPutFactSlot(m_theEnv,newFact,"userState",&theValue);
#endif

    //theValue.type = INTEGER;
    //theValue.value = EnvAddLong(m_theEnv,status.getOutlierValCount());
    EnvGetFactSlot(m_theEnv,factPtr,"outlierValCount",&outputValue);
    type = GetpType(&outputValue);
    if (type == INTEGER)
    {
        int temp = ValueToInteger(GetpValue(&outputValue));
        status.setOutlierValCount(temp);
    }

    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getOldSampleAverage());
    EnvGetFactSlot(m_theEnv,factPtr,"oldSampleAverage",&outputValue);
    type = GetpType(&outputValue);
    if (type == FLOAT)
    {
        double temp = ValueToDouble(GetpValue(&outputValue));
        status.setOldSampleAverage(temp);
    }

    //theValue.type = FLOAT;
    //theValue.value = EnvAddDouble(m_theEnv,status.getOldSampleStdDeviation());
    EnvGetFactSlot(m_theEnv,factPtr,"oldSampleStdDeviation",&outputValue);
    type = GetpType(&outputValue);
    if (type == FLOAT)
    {
        double temp = ValueToDouble(GetpValue(&outputValue));
        status.setOldSampleStdDeviation(temp);
    }

}
void EntryPointUser::saveAllSample()
{
    //DATA_OBJECT outputValueFactList;
    DATA_OBJECT outputValue;

     void *templatePtr = EnvFindDeftemplate(m_theEnv,m_binomialTemplate.c_str());
    //EnvGetFactList(m_theEnv,&outputValueFactList, NULL);
    //if (GetpType(&outputValueFactList) != MULTIFIELD) return;

    //void *factPtr = DOToPointer(outputValue);
    void *factPtr = EnvGetNextFactInTemplate(m_theEnv, templatePtr, NULL);
    while(factPtr != NULL)
    {
        EnvGetFactSlot(m_theEnv,factPtr,"type",&outputValue);
        int type = GetpType(&outputValue);
        if (type == SYMBOL)
        {
            string tmpstr = ValueToString(GetpValue(&outputValue));
            if (m_binomialList.find(tmpstr) != m_binomialList.end())
            {
                updateBinomial(factPtr, m_binomialList[tmpstr]);
            }
        }

        factPtr = EnvGetNextFactInTemplate(m_theEnv, templatePtr, factPtr);
    }


   templatePtr = EnvFindDeftemplate(m_theEnv,m_3sigmaTemplate.c_str());

   factPtr = EnvGetNextFactInTemplate(m_theEnv, templatePtr, NULL);
   while(factPtr != NULL)
   {
       EnvGetFactSlot(m_theEnv,factPtr,"type",&outputValue);
       int type = GetpType(&outputValue);
       if (type == SYMBOL)
       {
           string tmpstr = ValueToString(GetpValue(&outputValue));
           if (m_3sigmaList.find(tmpstr) != m_3sigmaList.end())
           {
               update3sigma(factPtr, m_3sigmaList[tmpstr]);
           }
       }

       factPtr = EnvGetNextFactInTemplate(m_theEnv, templatePtr, factPtr);
   }

    EventCharacteristicAndStatusBinomial *accelSpecific = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::SPEED_ACCELERATION)];
    EventCharacteristicAndStatusBinomial *reduceSpecific = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::SPEED_DECELERATION)];
    EventCharacteristicAndStatus3sigma *reduceStart = &m_3sigmaList[EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(EventCharacteristicAndStatus3sigma::TTC_OF_SPEED_DECELERATION)];
    EventCharacteristicAndStatus3sigma *TTCRisk = &m_3sigmaList[EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(EventCharacteristicAndStatus3sigma::DURATION_OF_SHORTTTC_CHECK)];
    EventCharacteristicAndStatusBinomial *distanceOnOpenroad = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::RUNNING_DISTANCE_ONOPENROAD)];
    EventCharacteristicAndStatusBinomial *distanceOnExpressway = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::RUNNING_DISTANCE_ONEXPRESSWAY)];
    EventCharacteristicAndStatus3sigma *distanceOnFollowStop = &m_3sigmaList[EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(EventCharacteristicAndStatus3sigma::STOPPED_DISTANCE)];;
//liuxin add start 2016.03.17
	EventCharacteristicAndStatus3sigma *vehicleShake = &m_3sigmaList[EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(EventCharacteristicAndStatus3sigma::STRAIGHT_RUN_SHAKE)];
	EventCharacteristicAndStatusBinomial *stopJerkBrake = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::STOP_JERK_BRAKE)];
	EventCharacteristicAndStatusBinomial *accelTreadTime = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::ACCEL_TREAD_TIME)];
	EventCharacteristicAndStatusBinomial *suddenStart = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::START_ACCELERATION)];
	EventCharacteristicAndStatusBinomial *rightTurnSpeed = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::RIGHT_TURN_SPEED)];
	EventCharacteristicAndStatusBinomial *leftTurnSpeed = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::LEFR_TURN_SPEED)];
	EventCharacteristicAndStatusBinomial *handleSpeed = &m_binomialList[EventCharacteristicAndStatusBinomial::getSpecBinomialTypeName(EventCharacteristicAndStatusBinomial::STEERINGANGLE_ACCELERATION)];
	EventCharacteristicAndStatus3sigma *frontCarFollowSta = &m_3sigmaList[EventCharacteristicAndStatus3sigma::getSpec3sigmaTypeName(EventCharacteristicAndStatus3sigma::FRONT_CAR_FOLLOW_START_TIMING)];
//liuxin add end 2016.03.17

	queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> copyQueue = accelSpecific->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpAccelSpecific;
    int x = 0;
    int size = 0;
    if(copyQueue.size()>accelSpecific->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - accelSpecific->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x <size ; x++)
    {
        //tmpAccelSpecific.add((SPECIFIC_BINOMIAL)copyQueue.toArray()[x]);
        tmpAccelSpecific.push(copyQueue.front());
        copyQueue.pop();
    }

    x = 0;
    copyQueue = reduceSpecific->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpReduceSpecific;

    if(copyQueue.size()>reduceSpecific->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - reduceSpecific->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpReduceSpecific.push(copyQueue.front());
        copyQueue.pop();
    }

    x = 0;
    copyQueue = distanceOnOpenroad->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpOpenroad ;

    if(copyQueue.size()>distanceOnOpenroad->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - distanceOnOpenroad->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        //tmpOpenroad.add((SPECIFIC_BINOMIAL)copyQueue.toArray()[x]);
        tmpOpenroad.push(copyQueue.front());
        copyQueue.pop();
    }

    x = 0;
    copyQueue = distanceOnExpressway->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpExpressway;
    if(copyQueue.size()>distanceOnExpressway->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - distanceOnExpressway->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        //tmpExpressway.add((SPECIFIC_BINOMIAL)copyQueue.toArray()[x]);
        tmpExpressway.push(copyQueue.front());
        copyQueue.pop();
    }

    queue<double> tmpReduceStart = reduceStart->getAllSample();
    queue<double> tmpTTC = TTCRisk->getAllSample();
    queue<double> tmpFollowStop = distanceOnFollowStop->getAllSample();

//liuxin add start 2016.03.17
	//Profile 9
	x = 0;
    copyQueue = stopJerkBrake->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpStopJerkBrake;

    if(copyQueue.size()>stopJerkBrake->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - stopJerkBrake->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpStopJerkBrake.push(copyQueue.front());
        copyQueue.pop();
    }
    //Profile 10
	x = 0;
    copyQueue = accelTreadTime->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpAccelTreadTime;

    if(copyQueue.size()>accelTreadTime->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - accelTreadTime->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpAccelTreadTime.push(copyQueue.front());
        copyQueue.pop();
    }
    //Profile 14
	x = 0;
    copyQueue = suddenStart->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpSuddenStart;

    if(copyQueue.size()>suddenStart->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - suddenStart->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpSuddenStart.push(copyQueue.front());
        copyQueue.pop();
    }
    //Profile 16
	x = 0;
    copyQueue = rightTurnSpeed->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpRightTurnSpeed;

    if(copyQueue.size()>rightTurnSpeed->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - rightTurnSpeed->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpRightTurnSpeed.push(copyQueue.front());
        copyQueue.pop();
    }
    //Profile 18
	x = 0;
    copyQueue = leftTurnSpeed->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpLeftTurnSpeed;

    if(copyQueue.size()>leftTurnSpeed->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - leftTurnSpeed->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpLeftTurnSpeed.push(copyQueue.front());
        copyQueue.pop();
    }
    //Profile 23
	x = 0;
    copyQueue = handleSpeed->getSampling();
    queue<EventCharacteristicAndStatusBinomial::SPECIFIC_BINOMIAL> tmpHandleSpeed;

    if(copyQueue.size()>handleSpeed->MAX_SAMPLE_SIZE)
    {
        x = copyQueue.size() - handleSpeed->MAX_SAMPLE_SIZE;
    }
    size = copyQueue.size();
    for(; x < size; x++)
    {
        tmpHandleSpeed.push(copyQueue.front());
        copyQueue.pop();
    }
    //Profile 8,44
    queue<double> tmpVehicleShake = vehicleShake->getAllSample();
    queue<double> tmpFrontCarFollowSta = frontCarFollowSta->getAllSample();
//liuxin add end 2016.03.17
    ofstream fw;
    fw.open("../setting/UserSpecificData.txt",ios::out);
    // 設定ファイル書き込み

    //fw.write("急加速頻度:" +ftoa(accelSpecific->getPercentage())+":" + accelSpecific->getSpecific());
    fw<<"急加速頻度:"<<accelSpecific->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(accelSpecific->getSpecificENUM());
    size = tmpAccelSpecific.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpAccelSpecific.front()));
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpAccelSpecific.front());
        tmpAccelSpecific.pop();
    }
    //fw.write("\n");
    fw<<endl;

    //fw.write("急減速頻度:" + ftoa(reduceSpecific->getPercentage())+":" + reduceSpecific->getSpecific());
    fw<<"急減速頻度:"<<reduceSpecific->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(reduceSpecific->getSpecificENUM());
    size = tmpReduceSpecific.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpReduceSpecific.front()));
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpReduceSpecific.front());
        tmpReduceSpecific.pop();
    }
    //fw.write("\n");
    fw<<endl;

    //fw.write("他車追従時の減速開始タイミング:" + ftoa(reduceStart->getAverage()) + ":" + ftoa(reduceStart->getStdDeviation()));
    fw<<"他車追従時の減速開始タイミング:"<<reduceStart->getAverage()<<":"<<reduceStart->getStdDeviation();
    size = tmpReduceStart.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + ftoa(tmpReduceStart.front()));
        fw<<":"<<tmpReduceStart.front();
        tmpReduceStart.pop();
    }
    //fw.write("\n");
    fw<<endl;

    //fw.write("TTC短時間へのリスク感覚:" + ftoa(TTCRisk->getAverage()) + ":" + ftoa(TTCRisk->getStdDeviation()));
    fw<<"TTC短時間へのリスク感覚:"<<TTCRisk->getAverage()<<":"<<TTCRisk->getStdDeviation();
    size = tmpTTC.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + ftoa(tmpTTC.front()));
        fw<<":"<<tmpTTC.front();
        tmpTTC.pop();
    }
    //fw.write("\n");
    fw<<endl;

    //fw.write("一般道路走行時の平均車間距離:" + ftoa(distanceOnOpenroad->getPercentage()) + ":" + distanceOnOpenroad->getSpecific());
    fw<<"一般道路走行時の平均車間距離:"<<distanceOnOpenroad->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(distanceOnOpenroad->getSpecificENUM());
    size = tmpOpenroad.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpOpenroad.front()));
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpOpenroad.front());
        tmpOpenroad.pop();
    }
    //fw.write("\n");
    fw<<endl;

    //fw.write("高速道路走行時の平均車間距離:" + ftoa(distanceOnExpressway->getPercentage()) + ":" + distanceOnExpressway->getSpecific());
    fw<<"高速道路走行時の平均車間距離:"<<distanceOnExpressway->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(distanceOnExpressway->getSpecificENUM());
    size =  tmpExpressway.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpExpressway.front()));
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpExpressway.front());
        tmpExpressway.pop();
    }
    //fw.write("\n");
    fw<<endl;

    //fw.write("追従停車時の車間距離:" + ftoa(distanceOnFollowStop.getAverage()) + ":" + ftoa(distanceOnFollowStop->getStdDeviation()));
    fw<<"追従停車時の車間距離:"<<distanceOnFollowStop->getAverage()<<":"<<distanceOnFollowStop->getStdDeviation();
    size = tmpFollowStop.size();
    for(int i=0; i<size;i++)
    {
        //fw.write(":" + ftoa(tmpFollowStop.front()));
        fw<<":"<<tmpFollowStop.front();
        tmpFollowStop.pop();
    }
	fw<<endl;

//liuxin add start 2016.03.17
	//Profile 8
    fw<<"車体揺れ頻度:"<<vehicleShake->getAverage()<<":"<<vehicleShake->getStdDeviation();
    size = tmpVehicleShake.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<tmpVehicleShake.front();
        tmpVehicleShake.pop();
    }
    fw<<endl;

    //Profile 9
    fw<<"カックンブレーキ頻度:"<<stopJerkBrake->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(stopJerkBrake->getSpecificENUM());
    size = tmpStopJerkBrake.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpStopJerkBrake.front());
        tmpStopJerkBrake.pop();
    }
    fw<<endl;

    //Profile 10
    fw<<"アクセル踏み込み時間:"<<accelTreadTime->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(accelTreadTime->getSpecificENUM());
    size = tmpAccelTreadTime.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpAccelTreadTime.front());
        tmpAccelTreadTime.pop();
    }
    fw<<endl;

    //Profile 14
    fw<<"急発進頻度:"<<suddenStart->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(suddenStart->getSpecificENUM());
    size = tmpSuddenStart.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpSuddenStart.front());
        tmpSuddenStart.pop();
    }
    fw<<endl;

    //Profile 16
    fw<<"右カーブスピード:"<<rightTurnSpeed->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(rightTurnSpeed->getSpecificENUM());
    size = tmpRightTurnSpeed.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpRightTurnSpeed.front());
        tmpRightTurnSpeed.pop();
    }
    fw<<endl;

    //Profile 18
    fw<<"左カーブスピード:"<<leftTurnSpeed->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(leftTurnSpeed->getSpecificENUM());
    size = tmpLeftTurnSpeed.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpLeftTurnSpeed.front());
        tmpLeftTurnSpeed.pop();
    }
    fw<<endl;

    //Profile 23
    fw<<"ハンドル操作スピード:"<<handleSpeed->getPercentage()<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(handleSpeed->getSpecificENUM());
    size = tmpHandleSpeed.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<EventCharacteristicAndStatusBinomial::getSpecBinomialName(tmpHandleSpeed.front());
        tmpHandleSpeed.pop();
    }
    fw<<endl;

    //Profile 44
    fw<<"前車追従時発車タイミング:"<<frontCarFollowSta->getAverage()<<":"<<frontCarFollowSta->getStdDeviation();
    size = tmpFrontCarFollowSta.size();
    for(int i=0; i<size;i++)
    {
        fw<<":"<<tmpFrontCarFollowSta.front();
        tmpFrontCarFollowSta.pop();
    }
    fw<<endl;
//liuxin add end 2016.03.17    

	fw.flush();
    //ファイルを閉じる
    fw.close();

}
