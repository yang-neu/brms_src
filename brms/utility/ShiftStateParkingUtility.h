#ifndef _SHIFT_STATE_PARKING_UTILITY_H_
#define _SHIFT_STATE_PARKING_UTILITY_H_

#include <string>
#include "../event/EventShiftState.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"

class ShiftStateParkingUtility :public  UtilityBase 
{
public:
	 ShiftStateParkingUtility(long long value, string sessionID)
	{
        event = new EventShiftState((int)value, "REVERSE");
		m_sessionID = sessionID;
	}
	void updateValue(long long value)
	{
		event->updateValue(value);
	}

    void run() {
		if (event->getValue() != 1)
		{
			return;
		}
		event->updateValue(2);
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(event);
	}
private:
	 EventShiftState *event;	
};
#endif
