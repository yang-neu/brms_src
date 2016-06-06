#ifndef _LOAD_LEVEL_UTILITY_H_
#define _LOAD_LEVEL_UTILITY_H_

#include <string>
#include "../event/EventLevelData.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"

using namespace std;

class LoadLevelUtility: public  UtilityBase 
{


public:
	 LoadLevelUtility(long long value, string sessionID)
	{

        event = new EventLevelData(value, "LOAD_LEVEL");
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
	 EventLevelData *event;

};
#endif
