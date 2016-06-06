//#include <errno.h>
#include <stdio.h>   
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#ifndef _WINDOWS_PLAT_
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#else
#include <WINSOCK2.H>

#endif

#include "../common/Message.h"
#include "SocketReceiveThread.h"
#include "CANDataParse.h"

#include "../event/EventSpeed.h"
#include "../event/EventDistance.h"
#include "../event/EventAccelOpen.h"
#include "../event/EventRoadClass.h"
#include "../ClipsSessionMgr.h"
#include "../streaming/SignalEntryPoint.h"


const int SocketReceiveThread::CAN_LENGTH = 13;
const int SocketReceiveThread::DATA_LENGTH = 8;
int SocketReceiveThread::ID_LENGTH = 8;

int SocketReceiveThread::ID_START_INDEX = 2;
int SocketReceiveThread::ID_END_INDEX = 3;
int SocketReceiveThread::DATA_START_INDEX = 5;
int SocketReceiveThread::DATA_END_INDEX = 13;
SOCKET SocketReceiveThread::socket_fd = 0;
int SocketReceiveThread::server_port;
string SocketReceiveThread::server_ip;

#ifdef _DEBUG_INPUT_
int DebugThread::m_snd_hndl = -1;
int DebugThread::m_rcv_hndl = -1;

DebugThread::DebugThread(string sessionID)
{
    m_sessionID = sessionID;
}

void DebugThread::start()
{
    halt_ = false;
    m_snd_hndl	=	msgOpen(EN_MSGID_DEBUG, (D_MSG_WRITE | D_MSG_CREAT), D_MSG_NUM, D_MSG_SIZE, 0);
    pthread_create(&m_ThdId, 0, run, this);
}
void DebugThread::halt() {
        halt_ = true;

}
void DebugThread::setSpeed(double speed)
{
    EventSetting setting;
    setting.kind = 0;
    setting.SettingValue.speed = speed;
    msgSend(m_snd_hndl, (unsigned char*)&setting, sizeof(EventSetting),0);
}

void DebugThread::setDistance(double distance)
{
    EventSetting setting;
    setting.kind = 1;
    setting.SettingValue.distance = distance;
    msgSend(m_snd_hndl, (unsigned char*)&setting, sizeof(EventSetting),0);
}
void DebugThread::setAccel(double accel)
{
    EventSetting setting;
    setting.kind = 2;
    setting.SettingValue.accel = accel;
    msgSend(m_snd_hndl, (unsigned char*)&setting, sizeof(EventSetting),0);
}

void DebugThread::setRoadKind(int kind)
{
    EventSetting setting;
    setting.kind = 3;
    setting.SettingValue.openRoadKind = kind;
    msgSend(m_snd_hndl, (unsigned char*)&setting, sizeof(EventSetting),0);
}

void* DebugThread:: run(void *parm)
{
    DebugThread *pdt = (DebugThread *)parm;
    m_rcv_hndl	=	msgOpen(EN_MSGID_DEBUG, D_MSG_READ, D_MSG_NUM, D_MSG_SIZE, 0);
    EventSetting setting;

    string entryPointName = "Receiving Data Stream";
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    string sessionID = pdt->getSessionID();
    SignalEntryPoint *pSep = dynamic_cast<SignalEntryPoint *> (pmgr->getSession(sessionID)->getEntryPoint(entryPointName));
    EventSpeed *speed = NULL;
    EventDistance *distance = NULL;
    EventAccelOpen *accel = NULL;
    EventRoadClass *roadClass = NULL;



    int ret = -1;
    while(!pdt->halt_)
    {
        ret = msgTimedRecv(m_rcv_hndl, (unsigned char*)&setting, sizeof(EventSetting), 0, 100);
        if (ret == sizeof(EventSetting))
        {
            switch (setting.kind ) {
            case 0:
                if(speed == NULL )
                    speed = new EventSpeed((long long)setting.SettingValue.speed, EventSpeed::VEHICLE_SPEED_SP1);
                else
                    speed->updateValue((long long)setting.SettingValue.speed);
                pSep->insert(speed);
                break;
            case 1:
                if(distance == NULL )
                    distance = new EventDistance ((long long)setting.SettingValue.distance, EventDistance::VEHICLE_FOLLOWING_DISTANCE);
                else
                    distance->updateValue((long long)setting.SettingValue.distance);
                pSep->insert(distance);
                break;
            case 2:
                if(accel == NULL)
                    accel = new EventAccelOpen((long long)setting.SettingValue.accel, EventAccelOpen::ACCEL_OPEN);
                else
                    accel->updateValue((long long)setting.SettingValue.accel);
                pSep->insert(accel);
                break;
            case 3:
                if(roadClass == NULL)
                    roadClass = new EventRoadClass((long long)setting.SettingValue.openRoadKind, EventRoadClass::ROAD_CLASS);
                else
                    roadClass->updateValue((long long)setting.SettingValue.openRoadKind);
                pSep->insert(roadClass);
                break;
            default:
                break;
            }
        }
    }
    msgClose(m_snd_hndl);
    msgClose(m_rcv_hndl);
}
#endif

void SocketReceiveThread::start()
{
	halt_ = false;
	pthread_create(&m_ThdId, 0, run, this);	
}

void* SocketReceiveThread:: run(void *parm)
{
	SocketReceiveThread *srt = (SocketReceiveThread*)parm;
	srt->work();
	while (!srt->halt_)
	{
		usleep(1000000);
		srt->work();
	}
}

void SocketReceiveThread::work()
{

	struct sockaddr_in server_addr;
  	int rcvCount=0;
  	char dataBuff[CAN_LENGTH];
  	char tmpBuff[DATA_LENGTH];
  	
  	memset(dataBuff, 0, sizeof(dataBuff));
  	memset(tmpBuff, 0, sizeof(tmpBuff));
	CANDataParse *workTh = new CANDataParse(getSessionID(), getcanTransData());
	int err;
#ifdef _WINDOWS_PLAT_
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired=MAKEWORD(2,0);
	err = WSAStartup(versionRequired,&wsaData);
#endif
    if( (socket_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
			return;
     }  
    memset(&server_addr,0,sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(server_port);  
 #ifdef _WINDOWS_PLAT_
	server_addr.sin_addr.S_un.S_addr=inet_addr(server_ip.c_str());
 #else
	if( inet_pton(AF_INET,server_ip.c_str(),&server_addr.sin_addr) <=0 ) {
		return;
    }  
#endif
#if 0
    DWORD TimeOut=6000; //设置发送超时6秒
    if(::setsockopt(socket_fd,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR){
		return;
    }
    TimeOut=6000;//设置接收超时6秒
    if(::setsockopt(socket_fd,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR){
		return0;
    }
#endif
   // unsigned long ul = 1;
    //err = ioctlsocket(socket_fd, FIONBIO, (unsigned long*)&ul);
   // if(err==SOCKET_ERROR)
   //     return;

	//cout << "connecting to " << server_ip << ":" << server_port << " ..." << endl;
	err = connect(socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if( err<0) {
#ifdef _WINDOWS_PLAT_
		//cout << "connect failed!(err:" << WSAGetLastError() << ")" << endl;
#else
		//cout << "connect failed!(err:" << errno << " " << strerror(errno) << ")" << endl;
#endif
#if 0
            struct timeval timeout ;
            fd_set r;

            FD_ZERO(&r);
            FD_SET(socket_fd, &r);
            timeout.tv_sec = 25; //连接超时15秒
            timeout.tv_usec =0;
            err = select(0, 0, &r, 0, &timeout);
            if ( err <= 0 )
            {
                ::closesocket(socket_fd);
				return;
            }
            //一般非阻塞模式套接比较难控制，可以根据实际情况考虑 再设回阻塞模式
            unsigned long ul1= 0 ;
            err = ioctlsocket(socket_fd, FIONBIO, (unsigned long*)&ul1);
            if(err ==SOCKET_ERROR)
            {
                ::closesocket (socket_fd);
				return;
            }
#endif
			 return;
    }  

	cout << "connect success!(" << server_ip << ":" << server_port << ")" << endl;
	ClipsSessionMgr::get()->getSession(m_sessionID)->setState(ClipsSession::SESSIONSTATE_RECVING);

	int recvRet = 0;
	int cnt = 0;
    // TCP/IP Request wait
	while (!halt_)  {
#ifdef RECV_PERF
		static double max = -1, min = -1, total = 0;
		static int count = 0, events = 0;
		double start, time;
#endif
		while(cnt <CAN_LENGTH)
		{
			recvRet = recv( socket_fd,dataBuff,CAN_LENGTH-cnt,0);
#ifdef RECV_PERF
			start = GetSystemTime();
#endif
			if(recvRet <= 0)
				break;
			cnt += recvRet;
		}
		if(recvRet <= 0)
			break;
		// DATA Fieldを抽出
		for(int i=0, j=DATA_START_INDEX; j<DATA_END_INDEX;i++,j++)
		{
			tmpBuff[i] = dataBuff[j];
		}

		// CAN IDを抽出
		int canId=0x000FF;
		canId = (canId & dataBuff[ID_START_INDEX])*0x100 + (canId & dataBuff[ID_END_INDEX]);

		if (workTh ==0)
			 workTh = new CANDataParse(getSessionID(), getcanTransData(), canId, tmpBuff, DATA_LENGTH);
		//ためしに同期でやってみる
		else
			workTh->updateData(canId, tmpBuff, DATA_LENGTH);
		int ret = workTh->run();
		if (ret > 0)
		{
#ifdef RECV_PERF
			time = GetSystemTime() - start;
			if (max == -1 || time > max)
			{
				max = time;
			}
			if (min == -1 || time < min)
			{
				min = time;
			}
			total += time;
			count++;
			events += ret;
			if (count % 1000 == 0)
			{
				char buffer[128];
				sprintf(buffer, "%d\t%d\t%.6f\t%.6f\t%.6f\t%.6f", count, events, max / 1000, min / 1000, total / 1000, total / 1000 / count);
				cout << buffer << endl;
			}
#endif
		}
		cnt = 0;
    }
	cout << "disconnect!(" << server_ip << ":" << server_port << ")" << endl;
	ClipsSessionMgr::get()->getSession(m_sessionID)->setState(ClipsSession::SESSIONSTATE_IDLE);

 #ifdef _WINDOWS_PLAT_
    closesocket(socket_fd);
    WSACleanup();

 #else
    close(socket_fd);  
 #endif
    if (workTh != 0)
	{
		delete workTh;
		workTh = 0;
	}
}

void SocketReceiveThread::halt() {
        halt_ = true;
        
}

