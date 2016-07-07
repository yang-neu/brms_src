
#ifndef _SPEED_UTILITY_H_
#define _SPEED_UTILITY_H_
#include <string>

#include "../event/EventSpeed.h"
#include "../ClipsSessionMgr.h"
//#include "../streaming/SignalEntryPoint.h"
using namespace std;

class SpeedUtility :public UtilityBase 
{
public :

	 SpeedUtility(long long value , string sessionID)
	{

		speed = new EventSpeed(value, EventSpeed::VEHICLE_SPEED_SP1);
		m_sessionID = sessionID;
	}

	void updateValue(long long value)
	{
		speed->updateValue(value);
	}
	void run() {

        //string entryPointName = "Receiving Data Stream";
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
        SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingName));
		pSep->insert(speed);

        //EntryPointHistory *pSepH = dynamic_cast<EntryPointHistory *> (pmgr->getSession(m_sessionID)->getEntryPoint(m_streamingHName));
        //pSepH->insert(speed);

	}
private :
	EventSpeed *speed;
};
#endif
