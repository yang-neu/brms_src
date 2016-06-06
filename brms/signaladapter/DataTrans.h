#ifndef _DATA_TRANS_H_
#define _DATA_TRANS_H_

#include <string>

#ifndef _WINDOWS_PLAT_
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#else
#include <stdio.h>
#include <windows.h>
#include <WINSOCK2.H>
#endif

using namespace std;

#define HEADER_SIZE 5
#define ACK_SIZE 3
#define SEND_BUFFER_SIZE 10*1024
#define RCV_BUFFER_SIZE 10*1024

class DataTrans 
{	
public:
	// コンストラクタ(通信先情報か、自サーバ情報を引数にとる)
	DataTrans(string _ip, int _portNo);

	bool startup();
    bool acceptInter();
	void shutdown();
    bool connectInter();
	void disconnect();
    bool sendInter(string _sendXml);

    bool recvInter(char *resultXml, int &size);
    bool recvInter(string &resultXml);

private:
	bool sendAppAck();
	
	bool recvAppAck();

private:

	// 通信先タスクのIPとPort
    SOCKET m_socket;
	string ip;
	int portNo;

	// 自サーバー側
    SOCKET serverSocket;
	string myServerIp;
	int myServerPortNo;
	
		// AppAckヘッダ
	char appAckHeader[HEADER_SIZE];
	char appAckData[HEADER_SIZE + ACK_SIZE];
	
	char m_send_buffer[SEND_BUFFER_SIZE];
    char m_rcv_buffer[RCV_BUFFER_SIZE];
};
#endif
