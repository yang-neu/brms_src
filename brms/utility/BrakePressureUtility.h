#ifndef _BRAKE_PRESSURE_UTILITY_H_
#define _BRAKE_PRESSURE_UTILITY_H_

#include <string>
#include "../event/EventBrakePressure.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"

class BrakePressureUtility :public  UtilityBase
{
public:
     BrakePressureUtility(long long value, string sessionID)
	{

        event = new EventBrakePressure(value, "BRAKE_PRESSURE");
		m_sessionID = sessionID;
	}
	void updateValue(long long value)
	{
        event->updateValue(value);
	}

    void run() {

		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(event);
	}
private:
	 EventBrakePressure *event;	
};
#endif
