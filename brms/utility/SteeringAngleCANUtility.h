#ifndef _STEERING_ANGLE_UTILITY_H_
#define _STEERING_ANGLE_UTILITY_H_

#include <string>
#include "../event/EventSteeringAngle.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"
#include "../streaming/EntryPointHistory.h"
class SteeringAngleCANUtility :public  UtilityBase
{
public:
	SteeringAngleCANUtility(long long value, string sessionID)
	{
        event = new EventSteeringAngle(value, "STEERING_ANGLE");
		m_sessionID = sessionID;
	}
	void updateValue(long long value)
	{
        event->updateValue(value);
	}

    void run() {
		int canValue = (int)(event->getValue() * 10.0);
		if (canValue > 0x7FF)
		{
			canValue -= 0x1000;
		}
		event->updateValue(canValue * 15);

		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(event);

        EntryPointHistory *pSepH = dynamic_cast<EntryPointHistory *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingHName));
        pSepH->insert(event);
	}
private:
	 EventSteeringAngle *event;	
};
#endif
