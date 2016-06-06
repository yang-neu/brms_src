#ifndef _H_ENTRY_POINT_USER_

#define _H_ENTRY_POINT_USER_

#include <map>
#include <string>
#include <iostream>
#include <list>
#include <queue>
#include <string.h>
#include <pthread.h>
#include "../event/EventCharacteristicAndStatus3sigma.h"
#include "../event/EventCharacteristicAndStatusBinomial.h"
#include "EntryPoint.h"

using namespace std;

class EntryPointUser:public EntryPoint
{
public:
	EntryPointUser();
    //virtual void initialize(string streamName);
    virtual void insert(EventCharacteristicAndStatusBinomial &status);
    virtual void insert(EventCharacteristicAndStatus3sigma &status);
    virtual void saveAllSample();
private:
    void updateBinomial(void *factPtr, EventCharacteristicAndStatusBinomial &status);
    void update3sigma(void *factPtr, EventCharacteristicAndStatus3sigma &status);
    string m_binomialTemplate;
    string m_3sigmaTemplate;
    //string m_disTemplate;
    //string m_roadClassTemplate;
    //string m_accelOpenTemplate;
	
    //static pthread_mutex_t m_mutex;
    //int m_currentSpeedCnt;
    //int m_currentDisCnt;
    //int m_currentRoadClassCnt;
    //int m_currentAccelCnt;
	
    //void *m_speedList;
    //void *m_disList;
    //void *m_roadClassList;
    //void *m_accelList;
	
	
    //void *m_speedFact;
    //void *m_disFact;
    //void *m_roadClassFact;
    //void *m_accelFact;
    //const char* m_globalName="user";
   // vector<Field> m_slots;

    FieldDataVec v_fd;
    FieldDataVec::iterator m_vItor;

    map<string, EventCharacteristicAndStatusBinomial> m_binomialList;
    map<string, EventCharacteristicAndStatus3sigma> m_3sigmaList;

    void *m_binomiaSpeclList;
    void *m_3sigmaSpecList;
};

#endif
