#ifndef _ACCEL_OPEN_UTILITY_H_
#define _ACCEL_OPEN_UTILITY_H_

#include <string>
#include "../event/EventAccelOpen.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"
//#include "../streaming/EntryPointHistory.h"
using namespace std;

class AccelOpenUtility :public UtilityBase 
{
public :
	AccelOpenUtility(long long value,string sessionID)
	{

		accelOpen = new EventAccelOpen(value, EventAccelOpen::ACCEL_OPEN);
		m_sessionID = sessionID;
	}
	void updateValue(long long value)
	{
		accelOpen->updateValue(value);
	}

	 void run() {

        //string entryPointName = "Receiving Data Stream";
        //string entryPointHistoryName = "Driving Hitory Stream";
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(accelOpen);

        //EntryPointHistory *pSepH = dynamic_cast<EntryPointHistory *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingHName));
        //pSepH->insert(accelOpen);
     }
private:

	EventAccelOpen *accelOpen;
};
#endif
