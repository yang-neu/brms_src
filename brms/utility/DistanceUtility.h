#ifndef _DISTANCE_UTILITY_H_
#define _DISTANCE_UTILITY_H_

#include <string>
#include "../event/EventDistance.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"
using namespace std;

class DistanceUtility :public UtilityBase 
{

public:
 	DistanceUtility(long long value ,string sessionID)
	{

		distance = new EventDistance(value, EventDistance::VEHICLE_FOLLOWING_DISTANCE);
		m_sessionID = sessionID;
	}

	void updateValue(long long value)
	{
		distance->updateValue(value);
	}
	 void run() {

        //string entryPointName = "Receiving Data Stream";
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(distance);

        EntryPointHistory *pSepH = dynamic_cast<EntryPointHistory *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingHName));
        pSepH->insert(distance);
		
	}
private:

	EventDistance *distance;	
};
#endif
