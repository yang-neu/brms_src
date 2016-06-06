#include <stdio.h>   
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


#include "DataTrans.h"


DataTrans::DataTrans(string _ip, int _portNo) 
{

        m_socket = INVALID_SOCKET;
		memset(&appAckHeader[0], 0, HEADER_SIZE);
		memset(&appAckData[0], 0, HEADER_SIZE + ACK_SIZE);
		memset(&m_send_buffer[0], 0, SEND_BUFFER_SIZE);
        memset(&m_rcv_buffer[0], 0, RCV_BUFFER_SIZE);
		ip = _ip;
		portNo   = _portNo;

		myServerIp = _ip;
		myServerPortNo = _portNo;
		
		appAckHeader[0] = '@';
		appAckHeader[1] = 0x00;
		appAckHeader[2] = 0x00;
		appAckHeader[3] = 0x00;
		appAckHeader[4] = 0x03;

		memcpy(appAckData, appAckHeader, HEADER_SIZE);

#ifdef _WINDOWS_PLAT_
        WORD versionRequired;
        WSADATA wsaData;
        versionRequired=MAKEWORD(2,0);
        int err=WSAStartup(versionRequired,&wsaData);
#endif
}

bool DataTrans:: connectInter()
{

    while(m_socket == INVALID_SOCKET) {
        m_socket = socket(AF_INET,SOCK_STREAM,0);
	}
	
	struct sockaddr_in server_addr;
	
	memset(&server_addr,0,sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(myServerPortNo);  
#ifdef _WINDOWS_PLAT_
   server_addr.sin_addr.S_un.S_addr=inet_addr(myServerIp.c_str());
#else
    if( inet_pton(AF_INET,myServerIp.c_str(),&server_addr.sin_addr) <=0 ) {
            cout<<"inet_pton error for "<<myServerIp<<endl;  
            return false;
    }  
#endif
    if( connect(m_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))<0) {
		//cout << "connect(" << myServerIp << ":" << myServerPortNo << ") error(" << errno << "):" << strerror(errno) << endl;

#ifndef _WINDOWS_PLAT_ // 2016/03/24 Close and reopen socket for QNX invalid param
		close(m_socket);
		m_socket = INVALID_SOCKET;
#endif
		return false;
	}

	return true;
}

void DataTrans:: disconnect() 
{
    if(m_socket != INVALID_SOCKET) {
#ifndef _WINDOWS_PLAT_
        close(m_socket);
#else
        closesocket(m_socket);
#endif
        m_socket = INVALID_SOCKET;
	}
}

bool DataTrans::sendInter(string _sendXml)
{
	char *sendData = &m_send_buffer[0];
	bool newmem = false;
    if (_sendXml.size() + HEADER_SIZE > SEND_BUFFER_SIZE)
	{
        sendData = (char *)malloc(_sendXml.size() + HEADER_SIZE);
		newmem = true;
	}
	if (sendData != NULL)
	{
		sendData[0] = '@';
		// サイズ(ビッグエンディアン)
        sendData[1] = (char) ((_sendXml.size()&0xFF000000) >> 24);
        sendData[2] = (char) ((_sendXml.size()&0x00FF0000) >> 16);
        sendData[3] = (char) ((_sendXml.size()&0x0000FF00) >> 8);
        sendData[4] = (char)  (_sendXml.size()&0x000000FF);
		
        memcpy(&sendData[5], _sendXml.c_str(), _sendXml.size());
		
        send(m_socket, sendData, _sendXml.size() + HEADER_SIZE, 0);
		
		// AppAck受信
		if(recvAppAck() == false) {
			if (newmem)
			{
				free(sendData);
			}
			return false;
		}
	}
	else
	{
		return false;
	}
	if (newmem)
	{
		free(sendData);
	}
	return true;
	
	
}
bool DataTrans::recvInter(string &resultXml)
{
        if (m_socket == INVALID_SOCKET)
            return false;

        char dataBuff[HEADER_SIZE];
        memset(&dataBuff[0], 0, HEADER_SIZE);

        int transSize = recv( m_socket,dataBuff,HEADER_SIZE,0);

        // ヘッダ読込ができなければ異常終了
        if(transSize != HEADER_SIZE) {
            return false;
        }

        // AppAckなら異常終了
        if( memcmp(dataBuff, appAckHeader, HEADER_SIZE) == 0)
        {
            cout<<"受信データのヘッダ異常[" << appAckHeader <<"]　AppAckが来ている"<<endl;;
        }

        // 受信データサイズ取得(ビッグエンディアン)
        int dataSize = 0;
        dataSize += (dataBuff[1] & 0xFF) << 24;
        dataSize += (dataBuff[2] & 0xFF) << 16;
        dataSize += (dataBuff[3] & 0xFF) << 8;
        dataSize += (dataBuff[4] & 0xFF);


        // 受信データサイズがある場合は受信
        if(dataSize > 0 ) {
            //byte[] data = new byte[dataSize];
            memset(&m_rcv_buffer[0], 0, RCV_BUFFER_SIZE);
            transSize = recv( m_socket,m_rcv_buffer,dataSize,0);

            if(transSize != dataSize) {
                cout<<"受信データサイズ異常　ヘッダ上サイズ[" << dataSize << "]　readできたサイズ[" <<transSize << "]"<<endl;
                return false;
            }
            resultXml = m_rcv_buffer;
        }

        // AppAck送信
        if( sendAppAck() == false) {
            return false;
        }
        return true;
}
bool DataTrans::recvInter(char *resultXml, int &size)
{
        if (m_socket == INVALID_SOCKET)
			return false;
		
		char dataBuff[HEADER_SIZE];
		memset(&dataBuff[0], 0, HEADER_SIZE);
	
        int transSize = recv( m_socket,dataBuff,HEADER_SIZE,0);
		
		// ヘッダ読込ができなければ異常終了
		if(transSize != HEADER_SIZE) {
			return false;
		}
		
		// AppAckなら異常終了
        if( memcmp(dataBuff, appAckHeader, HEADER_SIZE) == 0)
        {
			cout<<"受信データのヘッダ異常[" << appAckHeader <<"]　AppAckが来ている"<<endl;;
		}
		
		// 受信データサイズ取得(ビッグエンディアン)
		int dataSize = 0;
		dataSize += (dataBuff[1] & 0xFF) << 24;
		dataSize += (dataBuff[2] & 0xFF) << 16;
		dataSize += (dataBuff[3] & 0xFF) << 8;
		dataSize += (dataBuff[4] & 0xFF);
		
        if (dataSize >= size )
        {
            cout<<"data buffer too small:"<<size<<";data size:"<<dataSize<<endl;
            return false;
        }
	
		// 受信データサイズがある場合は受信
        if(dataSize > 0 && resultXml != NULL) {
			//byte[] data = new byte[dataSize];
            transSize = recv( m_socket,resultXml,dataSize,0);
		
			if(transSize != dataSize) {
				cout<<"受信データサイズ異常　ヘッダ上サイズ[" << dataSize << "]　readできたサイズ[" <<transSize << "]"<<endl;
				return false;
			}
           size =  dataSize;
		}
        else if (dataSize > 0 && resultXml == NULL)
        {
            cout<<"error databuffer"<<endl;
            return false;
        }
        else
        {

        }
		// AppAck送信
		if( sendAppAck() == false) {
			return false;
		}
		return true;
}
bool  DataTrans::sendAppAck() 
{
    if (m_socket == INVALID_SOCKET)
		return false;
	
    int size = send(m_socket, appAckData,  HEADER_SIZE + ACK_SIZE, 0);
	
	if (size != HEADER_SIZE + ACK_SIZE)
		return false;
	
	return true;
}
	
bool  DataTrans::recvAppAck() 
{
	

		// ヘッダ受信
		char header[HEADER_SIZE];
		char data[ACK_SIZE];
		int transSize;
		
		memset(&header[0], 0, HEADER_SIZE);
		memset(&data[0], 0, ACK_SIZE);
	
        transSize = recv( m_socket,header,HEADER_SIZE,0);
		// ヘッダ読込ができなければ異常終了
		if(transSize != HEADER_SIZE) {
			return false;
		}
		// AppAck以外なら異常終了
        if( memcmp(header, appAckHeader, HEADER_SIZE) != 0)
        {
            return false;
        }



        transSize = recv( m_socket,data,ACK_SIZE,0);
	
		// データ読込ができなければ異常終了
		if(transSize != ACK_SIZE) {
			return false;
		}
		
		return true;
}

bool  DataTrans:: startup() 
{	
	struct sockaddr_in server_addr;
	if(( serverSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 )
	{  
        return false;
    }  
  
	memset(&server_addr,0,sizeof(server_addr));  
	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	server_addr.sin_port = htons(myServerPortNo);  
 
    if( bind(serverSocket,(sockaddr*) &server_addr,sizeof(server_addr)) < 0 ) 
    {  
          return false;
    }  
  
    if( listen(serverSocket,10) < 0 ) 
    {  
          return false;
    }  
	return true;
}

bool DataTrans::acceptInter()
{
	struct sockaddr_in remote_addr;
#ifdef _WINDOWS_PLAT_
	int sin_size = sizeof(struct sockaddr_in);
    if(( m_socket = accept(serverSocket,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )
	{  
	     return false;
	}  
#else
	socklen_t sin_size = sizeof(struct sockaddr_in);
	if(( m_socket = accept(serverSocket,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )
	{
		 return false;
	}
#endif

	return true;
}

void DataTrans::shutdown() 
{
    if(m_socket != INVALID_SOCKET)
	{
#ifdef _WINDOWS_PLAT_
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = INVALID_SOCKET;
	}
    if(serverSocket != INVALID_SOCKET)
	{
 #ifdef _WINDOWS_PLAT_
        closesocket(serverSocket);
 #else
		close(serverSocket);
 #endif
        serverSocket = INVALID_SOCKET;
	}
}
