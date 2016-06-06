#ifndef _DRIVER_FACE_DIRECTION_UTILITY_H_
#define _DRIVER_FACE_DIRECTION_UTILITY_H_

#include <string>
#include "../event/EventDriverFace.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"

using namespace std;

class DriverFaceDirectionUtility :public UtilityBase 
{

public: 
	DriverFaceDirectionUtility(long long value,  string sessionID)
	{

        event = new EventDriverFace(value, "FACE_ANGLE");
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

private :
	EventDriverFace *event;
	
};
#endif
