#ifndef _H_SIGNAL_ENTRY_POINT_

#define _H_SIGNAL_ENTRY_POINT_

#include <map>
#include <string>
#include<iostream>
#include <string.h>
#include <pthread.h>
#include "../event/EventSpeed.h"
#include "../event/EventRoadClass.h"
#include "../event/EventDistance.h"
#include "../event/EventAccelOpen.h"
#include "../event/EventAimless.h"
#include "../event/EventLevelData.h"
#include "../event/EventShiftState.h"
#include "../event/EventDriverFace.h"
#include "../event/EventSteeringAngle.h"
#include "../event/EventBrakePressure.h"

#include "EntryPoint.h"

#define DATA_BUFFER 1

using namespace std;

class SignalEntryPoint:public EntryPoint
{
public:
	SignalEntryPoint();
	virtual void initialize(string streamName);
	virtual void insert(EventSpeed *speed);
	virtual void insert(EventDistance *distance);
	virtual void insert(EventRoadClass *rc);
	virtual void insert(EventAccelOpen *accel);
    virtual void insert(EventAimless *aimless);
    virtual void insert(EventShiftState *shift);
    virtual void insert(EventLevelData *level);
    virtual void insert(EventDriverFace *driverFace);
    virtual void insert(EventSteeringAngle *steeringAngle);
    virtual void insert(EventBrakePressure *brakePressure);

	void flush(FLUSH_TYPE type);
protected:
    int flushAccel();
    int flushSpeed();
    int flushBrakePressure();
    int flushDistance();
    int flushRoadKind();
    int flushAimless();
    int flushDriverFace();
    int flushLoadLevel();
    int flushShiftState();
    int flushSteeringAngle();

    int flushSpeedImm();

private:
	string m_speedTemplate;
	string m_disTemplate;
	string m_roadClassTemplate;
	string m_accelOpenTemplate;

    string m_aimLessTemplate;
    string m_driverFaceTemplate;
    string m_levelDataTemplate;
    string m_shiftStateTemplate;
    string m_steeringAngleTemplate;
    string m_brakePressureTemplate;

	static pthread_mutex_t m_mutex;
	int m_currentSpeedCnt;
	int m_currentDisCnt;
	int m_currentRoadClassCnt;
	int m_currentAccelCnt;

    int m_currentAimCnt;
    int m_currentDriverFaceCnt;
    int m_currentLevelDataCnt;
    int m_currentShiftStateCnt;
    int m_currentSteeringAngleCnt;
    int m_currentBrakePressureCnt;
#ifndef DATA_BUFFER
	void *m_speedList;
	void *m_disList;
	void *m_roadClassList;
    void *m_accelList;
#else
    list<EventSpeed> m_speedList;
    list<EventDistance> m_disList;
    list<EventRoadClass> m_roadClassList;
    list<EventAccelOpen> m_accelList;

    list<EventAimless> m_aimLessList;
    list<EventDriverFace> m_driverFaceList;
    list<EventLevelData> m_levelDataList;
    list<EventShiftState> m_shiftStateList;
    list<EventSteeringAngle> m_steeringAngleList;
    list<EventBrakePressure> m_brakePressureList;
#endif
	
	void *m_speedFact;
	void *m_disFact;
	void *m_roadClassFact;
	void *m_accelFact;

    void *m_aimLessFact;
    void *m_brakePressureFact;
    void *m_levelDataFact;
    void *m_shiftStateFact;
    void *m_steeringAngleFact;
    void *m_driverFaceFact;

};

#endif
