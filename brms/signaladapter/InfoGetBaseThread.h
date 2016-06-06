
#ifndef _INFO_GET_BASE_THREAD_H
#define _INFO_GET_BASE_THREAD_H

#include <pthread.h>
#include <sys/types.h>  

#ifndef _WINDOWS_PLAT_
#include <netinet/in.h>
#include <sys/socket.h>
#define SOCKET int
#else
#include <stdio.h>
#include <WINSOCK2.H>
#include <windows.h>
#endif
#include <fstream>
#include <string>
#include <list>

#include "../utility/UtilityBase.h"
#include "../utility/AccelOpenUtility.h"
#include "../utility/AimlessLevelUtility.h"
#include "../utility/DistanceUtility.h"
#include "../utility/DriverFaceDirectionUtility.h"
#include "../utility/LoadLevelUtility.h"
#include "../utility/ShiftStateDriveUtility.h"
#include "../utility/ShiftStateParkingUtility.h"
#include "../utility/ShiftStateReverseUtility.h"
#include "../utility/BrakePressureUtility.h"
#include "../utility/SpeedUtility.h"
#include "../utility/SteeringAngleUtility.h"
#include "DataTrans.h"

using namespace std;

class InfoGetBaseThread 
{

public:
	bool halt_;
	// コンストラクタ
	InfoGetBaseThread(string _session, string _serverIp, int _serverPortNo, int _pollingTime) :
		halt_(false)
	{
		session       = _session;
		serverIp      = _serverIp;
		serverPortNo  = _serverPortNo;
		pollingTime   = _pollingTime;
	}
	
	// スレッド中断
	void halt() ;

	void start();
	
protected:

	// 実行メソッド
	virtual void run() ;
    static void* startProc(void *p);

	DataTrans* connectLoop(string _ip, int _portNo); 

	virtual  void createInfoGetList(list<int> &result) = 0;

    bool parseNoticeXml(char* _xml, int size, list<int> &resultList);
    bool parseNoticeXml(string xml, list<int> &resultList);

	bool infoGet(DataTrans* _dataTrans, list<int> _list, string &result);
	
	bool createInfoGetXml(list<int> _infoIdList, string &result) ;
	bool createInfoResponseXml(list<int> _infoIdList, string &result);
	void insertWorkingMemory(string _xml);
protected :
	string session;
	DataTrans* dataTrans;
		
	string serverIp;
	int serverPortNo;
	map<int, UtilityBase *> parseList;
	pthread_t m_ThdId;
private:
	int pollingTime;
    ofstream m_fw;
};
#endif
