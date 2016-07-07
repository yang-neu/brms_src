#ifndef _ROAD_CLASS_UTILITY_H_
#define _ROAD_CLASS_UTILITY_H_

#include <string>
#include "../event/EventRoadClass.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"
using namespace std;

class RoadClassUtility :public UtilityBase 
{


public :
	RoadClassUtility(long long value, string sessionID)
	{

		roadClass = new EventRoadClass(value, EventRoadClass::ROAD_CLASS);
		m_sessionID = sessionID;
	}

	void updateValue(long long value)
	{
		roadClass->updateValue(value);
	}
	 void run() {

        //string entryPointName = "Receiving Data Stream";
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(roadClass);

        //EntryPointHistory *pSepH = dynamic_cast<EntryPointHistory *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingHName));
        //pSepH->insert(roadClass);
	}
private :
	
	EventRoadClass *roadClass;
	
};
#endif
