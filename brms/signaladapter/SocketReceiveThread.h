#ifndef _SOCKET_RECEIVE_THREAD_H_
#define _SOCKET_RECEIVE_THREAD_H_
#include <pthread.h>
#include <sys/types.h>  
#ifndef _WINDOWS_PLAT_
#include <netinet/in.h> 
#include <sys/socket.h> 
#define SOCKET int
#else
#include <stdio.h>
#include <windows.h>
#include <WINSOCK2.H>
#endif
#include <string>

using namespace std;

class CANTransmitData;

#if 0
#define		D_MSG_NUM				(10)
#define		D_MSG_SIZE				(12)


typedef struct {
    int kind;   //0:speed; 1:distance; 2:accel; 3:openRoadKind
    union {
        double speed;
        double distance;
        double accel;
        int    openRoadKind;
    } SettingValue;
}EventSetting;

class DebugThread {

public:
    DebugThread(string sessionID);
    volatile bool halt_;
    void start();
    static void *run(void *);
    void halt();
    string getSessionID() { return m_sessionID;}
    CANTransmitData *getcanTransData() { return m_canTransData; }

    void setSpeed(double speed);
    void setDistance(double distance);
    void setAccel(double accel);
    void setRoadKind(int kind);
private:
    CANTransmitData *m_canTransData;

    pthread_t m_ThdId;

    string m_sessionID;

    static int m_snd_hndl;
    static int m_rcv_hndl;
};
#endif

class SocketReceiveThread {

public :

	volatile bool halt_;
	SocketReceiveThread(string sessionID, string serverIp, int portNo, CANTransmitData *canTransData)
	{
		server_ip = serverIp;
		server_port = portNo;
		m_canTransData = canTransData;
		halt_ = false;
		m_sessionID = sessionID;
	}
	void start();
	void work();
	static void *run(void *); 
	void halt();
	string getSessionID() { return m_sessionID;}
	CANTransmitData *getcanTransData() { return m_canTransData; }
	static const int CAN_LENGTH ;
	static const int DATA_LENGTH;
	static int ID_LENGTH ;

	static int ID_START_INDEX;
	static int ID_END_INDEX ;
	static int DATA_START_INDEX;
	static int DATA_END_INDEX ;
    static SOCKET socket_fd ;
	//InputStream ism = null;	
	// CAN儊僢僙乕僕庴怣晹
	 
	static string server_ip;
	static int server_port;
private:
	//KieSession session;
	CANTransmitData *m_canTransData;
	//DNCockpitManager parent;

	//private Socket clientSocket;


	//Socket socket = null;

	//InputStream ism = null;	
	// CAN儊僢僙乕僕庴怣晹
	pthread_t m_ThdId;

	string m_sessionID;
	
};
#endif
