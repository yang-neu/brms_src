#ifndef _H_ENTRY_POINT_OUTPUT_

#define _H_ENTRY_POINT_OUTPUT_
extern "C"
{
	#include "clips.h"
}

#include <map>
#include <string>
#include<iostream>
#include <string.h>
#include "EntryPoint.h"
#include "../DBAccessor.h"

using namespace std;

class EntryPointOutput:public EntryPoint
{
public:

#if 0
    enum {
        //common
        ID_NAME_SPEED = 0,
        ID_NAME_DISTANCE,
        ID_NAME_ACCEL,
        ID_NAME_TTC,
        ID_NAME_A,
        ID_NAME_ROAD_CLASS,
        ID_NAME_ROAD_KIND,
        ID_NAME_FRONT_CAR,
        ID_NAME_DRIVER_SCENE,

        //profile 1
        ID_NAME_SUDDEN_ACCEL,
        ID_NAME_SUDDEN_ACCEL_STATE,
        ID_NAME_SUDDEN_ACCEL_CNT,

        //profile 2
        ID_NAME_SUDDEN_DECCEL,
        ID_NAME_SUDDEN_DECCEL_STATE,
        ID_NAME_SUDDEN_DECCEL_CNT,

        //profile 3
        ID_NAME_DECCEL_START,
        ID_NAME_DECCEL_START_STATE,
        ID_NAME_DECCEL_START_CNT,

        //profile 4
        ID_NAME_TTC_RISK,
        ID_NAME_TTC_RISK_STATE,
        ID_NAME_TTC_RISK_CNT,

        //profile 5
        ID_NAME_DIS_ON_EXPRESS_WAY,
        ID_NAME_DIS_ON_EXPRESS_WAY_STATE,
        ID_NAME_DIS_ON_EXPRESS_WAY_CNT,

        //profile 6
        ID_NAME_DIS_ON_OPEN_ROAD,
        ID_NAME_DIS_ON_OPEN_ROAD_STATE,
        ID_NAME_DIS_ON_OPEN_ROAD_CNT,

        //profile 7
        ID_NAME_FOLLOW_STOP,
        ID_NAME_FOLLOW_STOP_STATE,
        ID_NAME_FOLLOW_STOP_CNT
    };
#endif

	EntryPointOutput();
	void updateAll();
	virtual void initialize(string strName);
    virtual void getOutputValue(string nameID, double &value);
    virtual void getOutputValue(string nameID, string &value);
    virtual void getOutputValue(string nameID, int &value);
    double getSpeed();
    double getDistance();
    double getAccel();
    double getTTC();
    string getRoadClassSig();
    double getA();
    string getRoadKind();
    string getFrontCar();
    string getDriveScene();

    //profile1
    string getSuddenAccel();
    string getSuddenAccelState();
    int getSuddenAccelCount();

    //profile2
    string getSuddenDecel() ;
    string getSuddenDecelState() ;
    int getSuddenDecelCount() ;

    //profile3
    string getDecelStart() ;
    string getDecelStartState();
    int getDecelStartCount() ;

    //profile4
    string getTTCRisk() ;
    string getTTCRiskState();
    int getTTCRiskCount() ;

    //profile5
    string getDisOnExpressWay() ;
    string getDisOnExpressWayState() ;
    int getDisOnExpressWayCount() ;

    //profile6
    string getDisOnOpenRoad();
    string getDisOnOpenRoadState() ;
    int getDisOnOpenRoadCount() ;

    //profile7
    string getFollowStop();
    string getFollowStopState() ;
    int getFollowStopCount();
	
private:
    static pthread_mutex_t m_mutex;
	string m_outPutTemplate;
	
	const char* m_globalName;
	vector<Field> m_slots;
	
	FieldDataVec v_fd;
	FieldDataVec::iterator m_vItor;
};

#endif
