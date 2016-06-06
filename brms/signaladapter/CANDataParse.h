#ifndef _CAN_DATA_PARSE_THREAD_H_
#define _CAN_DATA_PARSE_THREAD_H_
#include <stdlib.h>
#include <string.h>
#include <list>
#include <string>
#include <map>
#include "../utility/UtilityBase.h"

using namespace std;


class CANTransmitData;
class CANParseDefinition;

class CANDataParse
{

public :
	CANDataParse( string sessionID,CANTransmitData *canTransData)
	{
		m_sessionID = sessionID;
		m_canTransData = canTransData;
	
	}
	CANDataParse(string sessionID,CANTransmitData *canTransData, int canId, char* pmsg, int msglen)
	{
		m_sessionID = sessionID;
		m_canTransData = canTransData;
		m_canId = canId;
		if(pmsg != 0 && msglen >0 )
		{
			len = msglen;
			memcpy(msg, pmsg, len);
		}
		//session = session;
	}
	void initialize();
	void updateData(int canId, char* pmsg, int msglen)
	{
		m_canId = canId;
		if(pmsg != 0 && msglen >0 )
		{
			len = msglen;
			memcpy(msg, pmsg, len);
		}
	}
	int run() ;
private:
	int parseDataInfo(list<CANParseDefinition> &dataInfo);
	long long getValue(CANParseDefinition &data);
private:
	 int m_canId;
	 char msg[100];
	 int len;
	 CANTransmitData *m_canTransData;
	 string m_sessionID ;
	 map<string, UtilityBase *> paseList;
	
	//private KieSession session;
	//private DNCockpitManager parent;

};
#endif
