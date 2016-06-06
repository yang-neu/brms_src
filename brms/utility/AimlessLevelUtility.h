#ifndef _AIMLESS_LEVEL_UTILITY_H_
#define _AIMLESS_LEVEL_UTILITY_H_

#include <string>
#include "../event/EventAimless.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"

using namespace std;

class AimlessLevelUtility :public UtilityBase  
{

public:
	AimlessLevelUtility(long long value,string sessionID)
	{
        event = new EventAimless(value, "AIMLESS_LEVEL");
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
    EventAimless *event;
	
};
#endif
