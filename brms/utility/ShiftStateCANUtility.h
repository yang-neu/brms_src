#ifndef _SHIFT_STATE_DRIVE_UTILITY_H_
#define _SHIFT_STATE_DRIVE_UTILITY_H_

#include <string>
#include "../event/EventShiftState.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"

class ShiftStateCANUtility :public  UtilityBase
{
public:
	ShiftStateCANUtility(long long value, string sessionID)
	{
        event = new EventShiftState((int)value, "REVERSE");
		m_sessionID = sessionID;
	}
	void updateValue(long long value)
	{
		event->updateValue(value);
	}

    void run() {
		switch ((int)event->getValue())
		{
		case 0x10: // Reverse
			event->updateValue(1);
			break;
		case 0x01: // Parking
			event->updateValue(2);
			break;
		case 0x00: // Drive
			event->updateValue(3);
			break;
		}
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(event);
	}
private:
	 EventShiftState *event;	
};
#endif
