
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>   
#include <fstream> 
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "common/StringUtil.h"

#include "BRMS.h"
#include "ClipsRuleMgr.h"
#include "common/StringUtil.h"

//#ifdef _CAN_DEVICE_
#include "event/CANTransmitData.h"
#include "signaladapter/SocketReceiveThread.h"
//#else
#include "signaladapter/DecisionAppServerThread.h"
#include "signaladapter/IntegrationGenerateClientThread.h"
//#endif
#include "ClipsSessionMgr.h"
#include "streaming/SignalEntryPoint.h"
#include "streaming/EntryPointOutput.h"
#include "streaming/EntryPointUser.h"
#include "Controller.h"
#include "event/EventCharacteristicAndStatus3sigma.h"
#include "event/EventCharacteristicAndStatusBinomial.h"
#include "extension/ExternalDef.h"
#include "extension/ExternalFunctions.h"
#include "timeliner/timeliner.hpp"
#include "common/ConfigUtil.h"

#include <time.h>
#define PIPE_BUF_SIZE 100

InputSource::InputSource(string &sessionID):
	m_InputTemplateName("InputSource")
#ifdef _DEBUG_INPUT_
  ,debugThread(NULL)
#endif
  ,timeAverage(0.0)
  ,stopCount(0)
  ,nationalAverage(10.0)
  ,portNo(51000)
  ,sim(true)
//#ifdef _CAN_DEVICE_
    ,CANtransData(NULL)
    ,canReceive(NULL)
//#else
   ,m_IGClient(NULL)
  ,m_DAppServer(NULL)
//#endif
{
    isDebug = false;
	//m_RuleEng = pEng;
	m_sessionID = sessionID;

	//startUp();
	readSettingFile();
	
	if (!isSimulator())
	{
//#ifdef _CAN_DEVICE_
		if (ConfigUtil::GetInstance()->GetConfigStr("通信タイプ") == "CAN")
		{
			CANtransData = new CANTransmitData();
			readCANTransmitDataFile();
			canReceive = new SocketReceiveThread(sessionID, ConfigUtil::GetInstance()->GetConfigStr("サーバーアドレス"), portNo, CANtransData);
		}
//#else
		else
		{
			m_IGClient = new IntegrationGenerateClientThread(m_sessionID);
			m_DAppServer = new DecisionAppServerThread(m_sessionID);
		}
//#endif
	}

}


void InputSource::startUp()
{
    readUserSpecificDataFile();

	if (!isSimulator())
	{
//#ifdef _CAN_DEVICE_
        if(canReceive != NULL)
            canReceive->start();
//#else
        if (m_IGClient != NULL)
            m_IGClient->start();
        //if (m_DAppServer != NULL)
         //   m_DAppServer->start();

//#endif
	}
	else
	{
		startGetInputData();
	}
	vector<FieldAndValue> parmlist1;
	FieldAndValue tmp1;
	int kind = 1;
	tmp1.type = 1;
	tmp1.name ="kind";
	tmp1.data.i_value =kind;
	parmlist1.push_back(tmp1);
	
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	pmgr->getSession(m_sessionID)->getEntryPoint("FirstStart Stream")->insert("FirstStart", parmlist1, 0);
	//GetEngine()->InsertSourceInputFact("FirstStart", parmlist1, 0); 
}

void InputSource::stop()
{
    if (!isSimulator())
    {
//#ifdef _CAN_DEVICE_
    if (canReceive != NULL)
        canReceive->halt();
//#else
    if (m_IGClient != NULL)
        m_IGClient->halt();
    if (m_DAppServer != NULL)
        m_DAppServer->halt();
//#endif
    //canReceive
    }

}

void InputSource::switchInput(bool flag)
{
#ifdef _DEBUG_INPUT_
    //disable debug mode
    if(flag && !isDebug)
    {
        //switch to debug mode
        stop();
        isDebug = true;

        if (debugThread == NULL )
            debugThread = new DebugThread(m_sessionID);
        debugThread->start();
    }
    else if (flag && isDebug)
    {
        //switch to nomal mode
        if (debugThread != NULL)
            debugThread->halt();
        if (canReceive != NULL)
            canReceive->start();
        isDebug= false;
    }
    else
    {
        //nothing to do
    }
#endif
}

void InputSource::setSpeedDbg(double speed)
{
#ifdef _DEBUG_INPUT_
	if (debugThread != NULL)
        debugThread->setSpeed(speed);
#endif
}

void InputSource::setDistanceDbg(double distance)
{
 #ifdef _DEBUG_INPUT_
    if (debugThread != NULL)
        debugThread->setDistance(distance);
  #endif
}

void InputSource::setAccelDbg(double accel)
{
#ifdef _DEBUG_INPUT_
    if (debugThread != NULL)
        debugThread->setAccel(accel);
#endif
}

void InputSource::setRoadKindDbg(int kind)
{
    #ifdef _DEBUG_INPUT_
    if (debugThread != NULL)
        debugThread->setRoadKind(kind);
#endif
}

void InputSource::readUserSpecificDataFile()
{
	// 設定ファイル読み込み
	ifstream filereader;
	filereader.open("../setting/UserSpecificData.txt",ios::in);
	
	EventCharacteristicAndStatusBinomial accelSpecific ;
	EventCharacteristicAndStatusBinomial reduceSpecific ;
	EventCharacteristicAndStatus3sigma reduceStart;
	EventCharacteristicAndStatus3sigma TTCRisk;
	EventCharacteristicAndStatusBinomial distanceOnOpenroad;
	EventCharacteristicAndStatusBinomial distanceOnExpressway;
	EventCharacteristicAndStatus3sigma distanceOnFollowStop;
//liuxin add start 2016.03.17	
	EventCharacteristicAndStatus3sigma vehicleShake;
	EventCharacteristicAndStatusBinomial stopJerkBrake;
	EventCharacteristicAndStatusBinomial accelTreadTime;
	EventCharacteristicAndStatusBinomial suddenStart;
	EventCharacteristicAndStatusBinomial rightTurnSpeed;
	EventCharacteristicAndStatusBinomial leftTurnSpeed;
	EventCharacteristicAndStatusBinomial handleSpeed;
	EventCharacteristicAndStatus3sigma frontCarFollowSta;
//liuxin add end 2016.03.17	

	string line;
	cout<<"ユーザ特性ファイル読み込み開始"<<endl;
	
	while(getline(filereader, line))
	{
		vector<string> div;
		split(line, div, ":");
		if (div.size() == 0)
		{
			continue;
		}

		if ( div[0] == "急加速頻度") {
			accelSpecific.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
			{
				accelSpecific.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NORMAL);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
			{
				accelSpecific.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SUDDEN);
			}
			
			for(size_t i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
				{
					accelSpecific.addSample(EventCharacteristicAndStatusBinomial::SUDDEN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
				{
					accelSpecific.addSample(EventCharacteristicAndStatusBinomial::NORMAL);
				}
			}
			if(div.size() >= 103 )
			{
				accelSpecific.setUserStateCheck(true);
			}
		}
		else if ( div[0] =="急減速頻度") {		
			reduceSpecific.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
			{
				reduceSpecific.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NORMAL);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
			{
				reduceSpecific.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SUDDEN);
			}
			
			for(size_t i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
				{
					reduceSpecific.addSample(EventCharacteristicAndStatusBinomial::SUDDEN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
				{
					reduceSpecific.addSample(EventCharacteristicAndStatusBinomial::NORMAL);
				}
			}
			if(div.size() >= 103 )
			{
				reduceSpecific.setUserStateCheck(true);
			}
		}
		else if ( div[0] == "他車追従時の減速開始タイミング") {
			reduceStart.setOldSampleAverage(atof(div[1].c_str()));
			reduceStart.setOldSampleStdDeviation(atof(div[2].c_str()));
			
			for(size_t i=3; i<div.size();i++ )
			{
				reduceStart.addSample(atof(div[i].c_str()));
			}
			if(div.size() >= 53)
			{
				reduceStart.setUserStateCheck(true);
			}
		}
		else if ( div[0] == "TTC短時間へのリスク感覚") {
			TTCRisk.setOldSampleAverage(atof(div[1].c_str()));
			TTCRisk.setOldSampleStdDeviation(atof(div[2].c_str()));
			
			for(size_t i=3; i<div.size();i++ )
			{
				TTCRisk.addSample(atof(div[i].c_str()));
			}
			if(div.size() >= 53)
			{
				TTCRisk.setUserStateCheck(true);
			}
			
		}
		else if ( div[0] == "高速道路走行時の平均車間距離") {
			distanceOnExpressway.setOldSamplePercentage(atof(div[1].c_str()));
			if(	div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_LARGE])
			{
				distanceOnExpressway.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::MARGIN_LARGE);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM])
			{
				distanceOnExpressway.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_SMALL])
			{
				distanceOnExpressway.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::MARGIN_SMALL);
			}
			
			for(size_t i=3; i<div.size();i++ )
			{
				if(	div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_LARGE])
				{
					distanceOnExpressway.addSample(EventCharacteristicAndStatusBinomial::MARGIN_LARGE);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM])
				{
					distanceOnExpressway.addSample(EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_SMALL])
				{
					distanceOnExpressway.addSample(EventCharacteristicAndStatusBinomial::MARGIN_SMALL);
				}
			}
			if(div.size() >= 103 )
			{
				distanceOnExpressway.setUserStateCheck(true);
			}
		}
		else if ( div[0] == "一般道路走行時の平均車間距離") {
			
			distanceOnOpenroad.setOldSamplePercentage(atof(div[1].c_str()));
			if(	div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_LARGE])
			{
				distanceOnOpenroad.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::MARGIN_LARGE);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM])
			{
				distanceOnOpenroad.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_SMALL])
			{
				distanceOnOpenroad.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::MARGIN_SMALL);
			}
			
			for(size_t i=3; i<div.size();i++ )
			{
				if(	div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_LARGE])
				{
					distanceOnOpenroad.addSample(EventCharacteristicAndStatusBinomial::MARGIN_LARGE);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM])
				{
					distanceOnOpenroad.addSample(EventCharacteristicAndStatusBinomial::MARGIN_MEDIUM);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::MARGIN_SMALL])
				{
					distanceOnOpenroad.addSample(EventCharacteristicAndStatusBinomial::MARGIN_SMALL);
				}
			}
			if(div.size() >= 103 )
			{
				distanceOnOpenroad.setUserStateCheck(true);
			}
		}
		else if ( div[0] == "追従停車時の車間距離") {
			distanceOnFollowStop.setOldSampleAverage(atof(div[1].c_str()));
			distanceOnFollowStop.setOldSampleStdDeviation(atof(div[2].c_str()));

			for(size_t i=3; i<div.size();i++ )
			{
				distanceOnFollowStop.addSample(atof(div[i].c_str()));
			}
			if(div.size() >= 103 )
			{
				distanceOnFollowStop.setUserStateCheck(true);
			}
		}
//liuxin add start 2016.03.17		
		//Profile 8
		else if ( div[0] == "車体揺れ頻度") {
			vehicleShake.setOldSampleAverage(atof(div[1].c_str()));
			vehicleShake.setOldSampleStdDeviation(atof(div[2].c_str()));
			
			for(int i=3; i<div.size();i++ )
			{
				vehicleShake.addSample(atof(div[i].c_str()));
			}
			if(div.size() >= 33)
			{
				vehicleShake.setUserStateCheck(true);
			}
		}
		//Profile 9
		else if ( div[0] =="カックンブレーキ頻度") {		
			stopJerkBrake.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
			{
				stopJerkBrake.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NORMAL);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
			{
				stopJerkBrake.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SUDDEN);
			}
			
			for(int i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
				{
					stopJerkBrake.addSample(EventCharacteristicAndStatusBinomial::SUDDEN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
				{
					stopJerkBrake.addSample(EventCharacteristicAndStatusBinomial::NORMAL);
				}
			}
			if(div.size() >= 103 )
			{
				stopJerkBrake.setUserStateCheck(true);
			}
		}
		//Profile 10
		else if ( div[0] =="アクセル踏み込み時間") {		
			accelTreadTime.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_APPOSITE])
			{
				accelTreadTime.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::ACCEL_APPOSITE);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_EXCESSIVELY])
			{
				accelTreadTime.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::ACCEL_EXCESSIVELY);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NAN0])
			{
				accelTreadTime.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NAN0);
			}
			for(int i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_EXCESSIVELY])
				{
					accelTreadTime.addSample(EventCharacteristicAndStatusBinomial::ACCEL_EXCESSIVELY);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_APPOSITE])
				{
					accelTreadTime.addSample(EventCharacteristicAndStatusBinomial::ACCEL_APPOSITE);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NAN0])
				{
					accelTreadTime.addSample(EventCharacteristicAndStatusBinomial::NAN0);
				}
			}
			if(div.size() >= 33 )
			{
				accelTreadTime.setUserStateCheck(true);
			}
		}
		//Profile 14
		else if ( div[0] =="急発進頻度") {		
			suddenStart.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
			{
				suddenStart.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NORMAL);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
			{
				suddenStart.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SUDDEN);
			}
			
			for(int i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
				{
					suddenStart.addSample(EventCharacteristicAndStatusBinomial::SUDDEN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
				{
					suddenStart.addSample(EventCharacteristicAndStatusBinomial::NORMAL);
				}
			}
			if(div.size() >= 103 )
			{
				suddenStart.setUserStateCheck(true);
			}
		}
		//Profile 16
		else if ( div[0] =="右カーブスピード") {		
			rightTurnSpeed.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT])
			{
				rightTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::REDUCE_TURN])
			{
				rightTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::REDUCE_TURN);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_TURN])
			{
				rightTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::ACCEL_TURN);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NAN0])
			{
				rightTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NAN0);
			}
			
			for(int i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT])
				{
					rightTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::REDUCE_TURN])
				{
					rightTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::REDUCE_TURN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_TURN])
				{
					rightTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::ACCEL_TURN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NAN0])
				{
					rightTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::NAN0);
				}
			}
			if(div.size() >= 103 )
			{
				rightTurnSpeed.setUserStateCheck(true);
			}
		}
		//Profile 18
		else if ( div[0] =="左カーブスピード") {		
			leftTurnSpeed.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT])
			{
				leftTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::REDUCE_TURN])
			{
				leftTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::REDUCE_TURN);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_TURN])
			{
				leftTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::ACCEL_TURN);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NAN0])
			{
				leftTurnSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NAN0);
			}
			
			for(int i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT])
				{
					leftTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::SLOW_IN_FAST_OUT);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::REDUCE_TURN])
				{
					leftTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::REDUCE_TURN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::ACCEL_TURN])
				{
					leftTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::ACCEL_TURN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NAN0])
				{
					leftTurnSpeed.addSample(EventCharacteristicAndStatusBinomial::NAN0);
				}
			}
			if(div.size() >= 103 )
			{
				leftTurnSpeed.setUserStateCheck(true);
			}
		}
		//Profile 23
		else if ( div[0] =="ハンドル操作スピード") {		
			handleSpeed.setOldSamplePercentage(atof(div[1].c_str()));
			if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
			{
				handleSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::NORMAL);
			}
			else if(div[2] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
			{
				handleSpeed.setOldSampleSpecific(EventCharacteristicAndStatusBinomial::SUDDEN);
			}
			
			for(int i=3; i<div.size();i++ )
			{
				if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::SUDDEN])
				{
					handleSpeed.addSample(EventCharacteristicAndStatusBinomial::SUDDEN);
				}
				else if(div[i] == EventCharacteristicAndStatusBinomial::m_specBinomialNameList[EventCharacteristicAndStatusBinomial::NORMAL])
				{
					handleSpeed.addSample(EventCharacteristicAndStatusBinomial::NORMAL);
				}
			}
			if(div.size() >= 103 )
			{
				handleSpeed.setUserStateCheck(true);
			}
		}
		//Profile 44
		else if ( div[0] == "前車追従時発車タイミング") {
			frontCarFollowSta.setOldSampleAverage(atof(div[1].c_str()));
			frontCarFollowSta.setOldSampleStdDeviation(atof(div[2].c_str()));
			
			for(int i=3; i<div.size();i++ )
			{
				frontCarFollowSta.addSample(atof(div[i].c_str()));
			}
			if(div.size() >= 103)
			{
				frontCarFollowSta.setUserStateCheck(true);
			}
		}
//liuxin add end 2016.03.07		
		else
		{
			// 無視
		}
		
	}
		
	filereader.close();

	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointUser *profileStream = dynamic_cast<EntryPointUser *>(pmgr->getSession(getSession())->getEntryPoint("Characteristic And Status Stream"));
	accelSpecific.setType(EventCharacteristicAndStatusBinomial::SPEED_ACCELERATION);
    profileStream->insert(accelSpecific);
	reduceSpecific.setType(EventCharacteristicAndStatusBinomial::SPEED_DECELERATION);
    profileStream->insert(reduceSpecific);
	reduceStart.setType(EventCharacteristicAndStatus3sigma::TTC_OF_SPEED_DECELERATION);
    profileStream->insert(reduceStart);
	TTCRisk.setType(EventCharacteristicAndStatus3sigma::DURATION_OF_SHORTTTC_CHECK);
    profileStream->insert(TTCRisk);
	distanceOnOpenroad.setType(EventCharacteristicAndStatusBinomial::RUNNING_DISTANCE_ONOPENROAD);
    profileStream->insert(distanceOnOpenroad);
	distanceOnExpressway.setType(EventCharacteristicAndStatusBinomial::RUNNING_DISTANCE_ONEXPRESSWAY);
    profileStream->insert(distanceOnExpressway);
	distanceOnFollowStop.setType(EventCharacteristicAndStatus3sigma::STOPPED_DISTANCE);
	distanceOnFollowStop.setMaxSampleSize(100);
    profileStream->insert(distanceOnFollowStop);
//liuxin add start 2016.03.17
	//Profile 8
	vehicleShake.setType(EventCharacteristicAndStatus3sigma::STRAIGHT_RUN_SHAKE);
	vehicleShake.setMaxSampleSize(30);
    profileStream->insert(vehicleShake);
    //Profile 9
    stopJerkBrake.setType(EventCharacteristicAndStatusBinomial::STOP_JERK_BRAKE);
    profileStream->insert(stopJerkBrake);
    //Profile 10
    accelTreadTime.setType(EventCharacteristicAndStatusBinomial::ACCEL_TREAD_TIME);
    accelTreadTime.setMaxSampleSize(30);
    profileStream->insert(accelTreadTime);
    //Profile 14
    suddenStart.setType(EventCharacteristicAndStatusBinomial::START_ACCELERATION);
    profileStream->insert(suddenStart);
    //Profile 16
    rightTurnSpeed.setType(EventCharacteristicAndStatusBinomial::RIGHT_TURN_SPEED);
    profileStream->insert(rightTurnSpeed);
    //Profile 18
    leftTurnSpeed.setType(EventCharacteristicAndStatusBinomial::LEFR_TURN_SPEED);
    profileStream->insert(leftTurnSpeed);
    //Profile 23
    handleSpeed.setType(EventCharacteristicAndStatusBinomial::STEERINGANGLE_ACCELERATION);
    profileStream->insert(handleSpeed);
    //Profile 44
	frontCarFollowSta.setType(EventCharacteristicAndStatus3sigma::FRONT_CAR_FOLLOW_START_TIMING);
	frontCarFollowSta.setMaxSampleSize(100);
    profileStream->insert(frontCarFollowSta);
//liuxin add end 2016.03.17    
}
	
// ユーザ特性ファイル書き込み
void InputSource::writeUserSpecificDataFile()
{
	//ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	//EntryPointUser *profileStream = dynamic_cast<EntryPointUser *>(pmgr->getSession(getSession())->getEntryPoint("Characteristic And Status Stream"));
}

void InputSource::readSettingFile()
{
#if 0
	ifstream filereader;
	filereader.open("../setting/setting.txt",ios::in);

	string line;

	
	while(getline(filereader, line))
	{
		vector<string> div;
		split(line, div, ":");
	
		if (div[0] == "simulator")
			sim = atoi(div[1].c_str());
		
		else if (div[0] == "ユーザ平均停車時間")
			timeAverage = atof(div[1].c_str());
		
		else if (div[0] =="ユーザ停車回数")
			stopCount = atoi(div[1].c_str());
		
		else if (div[0] == "全国平均停車時間")
			nationalAverage = atof(div[1].c_str());
		
		else if (div[0] == "ポート番号")
			portNo = atoi(div[1].c_str());
		
		else
		{

		}
	
	}
	filereader.close();
#else
	ConfigUtil *config = ConfigUtil::GetInstance();
	sim = config->GetConfigInt("simulator");
	timeAverage = config->GetConfigInt("ユーザ平均停車時間");
	stopCount = config->GetConfigInt("ユーザ停車回数");
	nationalAverage = config->GetConfigInt("全国平均停車時間");
	portNo = config->GetConfigInt("ポート番号");
#endif
}

//#ifdef _CAN_DEVICE_
void InputSource :: readCANTransmitDataFile()
{
		ifstream filereader;
		filereader.open("../setting/CANTransmitDataInf.csv",ios::in);  
		string line;
		
		while(getline(filereader, line))
		{
			CANParseDefinition canData;
			vector<string> ret_;
			split(line, ret_);	

			canData.setId(atoi(ret_[0].c_str()));
			#if 0
			char intc[2];
			intc[1] = '\0';
			int intret = 0;
			int len = ret_[1].size();
			
			for(int i=0; i< len-1; i++)
			{
				intc[0] = ret_[1].c_str()[i];
				intret += atoi(intc)*pow(16,(len-1-i));
			}
			#endif
			int intret = htoi(ret_[1].c_str());
			canData.setCanId(intret);
			cout<<"canID:"<<intret<<endl;
			canData.setDataName(ret_[2]);
			canData.setDataPosByte(atof(ret_[3].c_str()));
			canData.setDataPosBit(atoi(ret_[4].c_str()));
			canData.setModelClassName(ret_[5]);
			canData.setEntryPointName(ret_[6]);
			CANtransData->addParseDefinition(canData);
			
		}
		filereader.close();
		
}
//#endif
void *InputSource::getData(void *para)
{
	if (para == 0) return 0;
	InputSource *is = (InputSource*)para;
	

	if (is->isSimulator())
	{
		int res = 0;

		const char *fifo_name = "/tmp/can_fifo";
	    
		int pipe_fd = -1;
		int open_mode = O_RDONLY;


		//char buffer[PIPE_BUF_SIZE + 1];
		STInputSource tmpSource;
		memset(&tmpSource, 0, sizeof(tmpSource));

		cout<<"Process "<<getpid()<<" opening FIFO O_RDONLY"<<endl;
	    pipe_fd = open(fifo_name, open_mode);
	    cout<<"Process "<<getpid()<<"result "<< pipe_fd<<endl;
		if(pipe_fd != -1)
	    {
			while(true)
			{
				cout<<"Process "<<getpid()<<"read data begin"<<endl;
		
				res = read(pipe_fd, &tmpSource, sizeof(STInputSource));
				cout<<"Process "<<getpid()<<"read data end:"<<res<<endl;
				vector<FieldAndValue> parmlist;
				FieldAndValue tmp;
				tmp.type = 0;
				tmp.name ="speed";
				tmp.data.f_value = tmpSource.speed;
				parmlist.push_back(tmp);
				
				tmp.type = 0;
				tmp.name ="astatus";
				tmp.data.f_value =tmpSource.astatus;
				parmlist.push_back(tmp);
				
				tmp.type = 1;
				tmp.name ="rclass";
				tmp.data.i_value =tmpSource.roadclass;
				parmlist.push_back(tmp);
				
				tmp.type = 0;
				tmp.name ="distance";
				tmp.data.f_value =tmpSource.distance;
				parmlist.push_back(tmp);
				
				ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
				pmgr->getSession(is->getSession())->getEntryPoint("InputSource Stream")->insert(is->m_InputTemplateName.c_str(), parmlist, 0);
				//is->GetEngine()->InsertSourceInputFact(is->m_InputTemplateName.c_str(), parmlist, 0); 
				//sleep(1); 
			}
		}
		else
		{
			cout<<"Process "<<getpid()<<"open pipe failed"<<endl;
		}
		
	}
	else
	{
		
	}
	
	
	cout<<"InputSource::GetData:finish"<<endl;
	return 0;
}
void InputSource::startGetInputData()
{
	int ret = pthread_create(&m_ThdId, 0, getData, this);
	if (ret != 0)
	{
		cout << "thread getData create failed!" << endl;
	}
}

BRMS::BRMS()
	:m_timeliner(NULL)
{
      m_statusIG = false;
	  ConfigUtil::GetInstance("../setting/setting.txt");
}
bool BRMS::setupSession(string &sessionID)
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPoint *pStream;
	
	pStream = new EntryPoint();
	pmgr->getSession(sessionID)->registEntryPoint("FirstStart Stream", pStream);	
	pStream->initialize("FirstStart Stream");
	
	pStream = new EntryPoint();
	//InputSource Stream for simulator
	pmgr->getSession(sessionID)->registEntryPoint("InputSource Stream", pStream);
	pStream->initialize("InputSource Stream");
	
	pStream = new SignalEntryPoint();
	pmgr->getSession(sessionID)->registEntryPoint("Receiving Data Stream", pStream);
	pStream->initialize("Receiving Data Stream");
	
    pStream = new EntryPointUser();
	pmgr->getSession(sessionID)->registEntryPoint("Characteristic And Status Stream", pStream);
	pStream->initialize("Characteristic And Status Stream");
	
	pStream = new EntryPointOutput();
	pmgr->getSession(sessionID)->registEntryPoint("Data Output Stream", pStream);
	pStream->initialize("Data Output Stream");
	return true;
}
extern "C" void SetupClipsUserFunc(CLIPS_USER_FP newEntryPoint,Ext_FuncInfo *extFuncTable, int cnt);
double g_start_time = 0.0;
double g_start_end_time = 0.0;
bool BRMS::initialize(int argc,char *argv[], string &sessionID, RouterInterface *router)
{
    struct timeval t_start;

    //get start time
#ifndef _WINDOWS_PLAT_
    gettimeofday(&t_start, NULL);
#else
    mingw_gettimeofday(&t_start, NULL);
#endif
    g_start_time = ((double)t_start.tv_sec)*1000+(double)t_start.tv_usec/1000;
    cout<<fixed<<"=====BRMS::initialize:"<<g_start_time<<"========"<<endl;

#ifdef _WINDOWS_PLAT_
	pthread_t pid;
	pthread_create(&pid, NULL, timer_thread, NULL);
#endif

#if 0
    time_t now;
    struct tm  *timenow;
    time(&now);
    timenow = localtime(&now);
    cout<<"=====new fact begin time:"<<asctime(timenow)<<"========"<<endl;
#endif
    m_statusIG = true;
    SetupClipsUserFunc((CLIPS_USER_FP)NewEntryPoint, g_extFuncTable, MAX_EXT_FUNC_CNT);
	
	//cout<<"initialize 0"<<"DNCockpitKS"<<endl;
	bool ret = false;
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	//cout<<"initialize 1"<<endl;

	if (pmgr == 0) return false;

#ifdef _DEBUG_CLIPS_
	pmgr->initialize(argc,argv);
#endif
	cout<<"initialize 2"<<endl;

	m_sessionID = string(sessionID);
	cout<<"initialize 5"<<endl;

    pmgr->createSession(m_sessionID, router);
	//cout<<"initialize 3"<<endl;

	setupSession(m_sessionID);
	//cout<<"initialize 4"<<endl;

	Controller *ctrl = Controller::get();
	//cout<<"initialize 6"<<endl;

	m_inputSource = new InputSource (sessionID);
	//cout<<"initialize 7"<<endl;

	m_inputSource->startUp();
	//cout<<"initialize 8"<<endl;

    //sleep(5);
	ctrl->start();
	//cout<<"initialize 9"<<endl;

	// Start Timeliner Thread
	string logpath = ConfigUtil::GetInstance()->GetConfigStr("TimerLinerログ出力パス");
	if (logpath != "")
	{
		m_timeliner = new OutputStateThread(sessionID, logpath);
		m_timeliner->Start();
	}

	return ret;
}
 double BRMS::getSpeed() 
 {
 	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getSpeed();
 }
 double BRMS::getAccel()
 {
 	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getAccel();
 }
 double BRMS::getDistance()
 {
 	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getDistance();
 }
 double BRMS::getA()
 {
 	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getA();
 }
 string BRMS::getRoadKind()
 {
 	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getRoadKind();
 }
 
 
	
double BRMS::getTTC()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getTTC();
}
string BRMS::getRoadClassSig()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getRoadClassSig();
}

string BRMS::getFrontCar()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getFrontCar();
}
string BRMS::getDriveScene()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getDriveScene();
}
string BRMS::getSuddenAccel()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getSuddenAccel();
}
string BRMS::getSuddenAccelState()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getSuddenAccelState();
}
int BRMS::getSuddenAccelCount()
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));
	
	return pStream->getSuddenAccelCount();
}


string BRMS::getSuddenDecel()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getSuddenDecel();
}

string BRMS::getSuddenDecelState()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getSuddenDecelState();
}

int BRMS::getSuddenDecelCount()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getSuddenDecelCount();
}

//profile3
string BRMS::getDecelStart()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDecelStart();
}

string BRMS::getDecelStartState()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDecelStartState();
}

int BRMS::getDecelStartCount()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDecelStartCount();
}

//profile4
string BRMS::getTTCRisk()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getTTCRisk();
}

string BRMS::getTTCRiskState()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getTTCRiskState();
}

int BRMS::getTTCRiskCount()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getTTCRiskCount();
}

//profile5
string BRMS::getDisOnExpressWay()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDisOnExpressWay();
}

string BRMS::getDisOnExpressWayState()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDisOnExpressWayState();
}

int BRMS::getDisOnExpressWayCount()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDisOnExpressWayCount();
}

//profile6
string BRMS::getDisOnOpenRoad()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDisOnOpenRoad();
}

string BRMS::getDisOnOpenRoadState()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDisOnOpenRoadState();
}

int BRMS::getDisOnOpenRoadCount()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getDisOnOpenRoadCount();
}

//profile7
string BRMS::getFollowStop()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getFollowStop();
}

string BRMS::getFollowStopState()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getFollowStopState();
}

int BRMS::getFollowStopCount()
{
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(m_sessionID)->getEntryPoint("Data Output Stream"));

    return pStream->getFollowStopCount();
}

void BRMS::onIG()
{
    if (m_statusIG) return;
    m_statusIG = true;
    Controller *ctrl = Controller::get();
    if (ctrl != NULL)
        ctrl->start();
    if (m_inputSource != NULL)
        m_inputSource->startUp();
	if (m_timeliner != NULL)
		m_timeliner->Start();

}
void BRMS::offIG()
{
    if (!m_statusIG) return;

    m_statusIG = false;
    if (m_inputSource != NULL)
        m_inputSource->stop();

    Controller *ctrl = Controller::get();
    if (ctrl != NULL)
        ctrl->stop();
	if (m_timeliner != NULL)
		m_timeliner->Stop();
}
void BRMS::dbgSwich(bool flag)
{
    if (m_inputSource != NULL)
        m_inputSource->switchInput(flag);
}
void BRMS::setSpeedDbg(double speed)
{
    if (m_inputSource != NULL)
        m_inputSource->setSpeedDbg(speed);
}

void BRMS::setDistanceDbg(double distance)
{
    if (m_inputSource != NULL)
        m_inputSource->setDistanceDbg(distance);
}

void BRMS::setAccelDbg(double accel)
{
    if (m_inputSource != NULL)
        m_inputSource->setAccelDbg(accel);
}

void BRMS::setRoadKindDbg(int kind)
{
    if (m_inputSource != NULL)
        m_inputSource->setRoadKindDbg(kind);
}
