
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>   
#include <fstream> 
#include <math.h>


#include "Controller.h"
#include "BRMS.h"
#include "streaming/EntryPoint.h"
#include "ClipsSessionMgr.h"
#include "common/Message.h"


const char* g_sessionID = "DNCockpitKS";

#ifndef _WINDOWS_PLAT_
int Controller::m_snd_hndl = -1;
int Controller::m_rcv_hndl = -1;
#else
HANDLE  Controller::m_handles[3] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE};
#endif
//pthread_cond_t Controller::m_cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t Controller::m_mutex = PTHREAD_MUTEX_INITIALIZER;
//bool Controller::m_flag = true ;
bool Controller::m_status = false;
Controller *Controller::m_instance = NULL;
Controller::Controller()
{
#ifndef _WINDOWS_PLAT_
	timerCreate(timer500msCallback, 0,  &m_timer500ms);
	timerCreate(timer10000msCallback,0,  &m_timer10000ms);
	
	m_snd_hndl	=	msgOpen(EN_MSGID_TIMER, (D_MSG_WRITE | D_MSG_CREAT), D_MSG_NUM, D_MSG_SIZE, 0);
 #else
    m_handles[0] = CreateWaitableTimer(NULL, FALSE, NULL);
    m_handles[1] = CreateWaitableTimer(NULL, FALSE, NULL);
    m_handles[2] = CreateEvent(NULL,FALSE,FALSE,NULL);
#endif
	
}

bool Controller::start()
{
	if (!m_status)
	{
		m_status = true;
		pthread_create(&m_thread, 0, schedulerProc, this);
#ifndef _WINDOWS_PLAT_
        timerStartRepeat(&m_timer500ms, 500);
        timerStartRepeat(&m_timer10000ms, 10000);
#else
        LARGE_INTEGER liDueTime500ms, liDueTime10000ms;


        liDueTime500ms.QuadPart = -5000000;
        liDueTime10000ms.QuadPart =  -100000000;

        SetWaitableTimer(m_handles[0], &liDueTime500ms, 500, NULL, NULL, FALSE);
        SetWaitableTimer(m_handles[1], &liDueTime10000ms, 10000, NULL, NULL, FALSE);
#endif
	}
	return true;
}
bool Controller::stop()
{
#ifndef _WINDOWS_PLAT_
    timerStop(&m_timer500ms);
    timerStop(&m_timer10000ms);
#else

    CancelWaitableTimer(m_handles[0]);
    CancelWaitableTimer(m_handles[1]);

    SetEvent(m_handles[2]);
#endif
    m_status = false;
	return true;
}
void *Controller::schedulerProc(void *para)
{
#ifndef _WINDOWS_PLAT_
	m_rcv_hndl	=	msgOpen(EN_MSGID_TIMER, D_MSG_READ, D_MSG_NUM, D_MSG_SIZE, 0);
	
	FireEvent eventFire;
	int ret = 0;
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	
	while(m_status)
	{
    #if 1
        ret = msgTimedRecv(m_rcv_hndl, (unsigned char*)&eventFire, sizeof(FireEvent), 0, 500);
		if (ret == sizeof(FireEvent))
		{
			switch (eventFire.eventID) {
			case FIRE_EVENT_ID_500ms :
					//cout<<"schedulerProc: 500ms fire begin"<<endl;
					pmgr->getSession(g_sessionID)->getEntryPoint("Receiving Data Stream")->flush(EntryPoint::FLUSH_TYPE_MODULE_SPA_500MS);
					//cout<<"schedulerProc: 500ms fire end"<<endl;
					
					pmgr->getSession(g_sessionID)->getEntryPoint("Data Output Stream")->updateAll();
					
					break;
			case FIRE_EVENT_ID_10000ms :
					//cout<<"schedulerProc: 10000ms fire begin"<<endl;
					pmgr->getSession(g_sessionID)->getEntryPoint("Receiving Data Stream")->flush(EntryPoint::FLUSH_TYPE_MODULE_SPA_10S);
					//cout<<"schedulerProc: 10000ms fire end"<<endl;
					
					pmgr->getSession(g_sessionID)->getEntryPoint("Data Output Stream")->updateAll();

					break;
			default:
					cout<<"schedulerProc: default"<<endl;
					break;
			}	
		}
      #else
        pmgr->getSession(g_sessionID)->getEntryPoint("Receiving Data Stream")->flush(EntryPoint::FLUSH_TYPE_MODULE_SPA_500MS);
        pmgr->getSession(g_sessionID)->getEntryPoint("Data Output Stream")->updateAll();
        usleep(500000);
      #endif
	}
    msgClose(m_snd_hndl);
	msgClose(m_rcv_hndl);
    pmgr->getSession(g_sessionID)->getEntryPoint("Characteristic And Status Stream")->saveAllSample();
#else
    DWORD dwRet = 0;
    ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
    while(m_status)
    {
        dwRet = WaitForMultipleObjects(3,m_handles,FALSE,INFINITE);

        switch(dwRet)
        {
            case WAIT_OBJECT_0:

				//cout<<"schedulerProc: 500ms fire begin"<<endl;
                pmgr->getSession(g_sessionID)->getEntryPoint("Receiving Data Stream")->flush(EntryPoint::FLUSH_TYPE_MODULE_SPA_500MS);
				//cout<<"schedulerProc: 500ms fire end"<<endl;

                pmgr->getSession(g_sessionID)->getEntryPoint("Data Output Stream")->updateAll();

				//add by liusiping @ 2016/06/10
				pmgr->getSession(g_sessionID)->getEntryPoint("Common Data Stream")->updateAll();
                break;
            case WAIT_OBJECT_0+1:
				//cout<<"schedulerProc: 10000ms fire begin"<<endl;
                pmgr->getSession(g_sessionID)->getEntryPoint("Receiving Data Stream")->flush(EntryPoint::FLUSH_TYPE_MODULE_SPA_10S);
				//cout<<"schedulerProc: 10000ms fire end"<<endl;

                pmgr->getSession(g_sessionID)->getEntryPoint("Data Output Stream")->updateAll();

				//add by liusiping @ 2016/06/10
				pmgr->getSession(g_sessionID)->getEntryPoint("Common Data Stream")->updateAll();

                break;


            case WAIT_OBJECT_0+2:
                m_status = false;
                break;
        }

    }
    for (int i=0; i<3; i++)
    {
        CloseHandle(m_handles[i]);
        m_handles[i] = INVALID_HANDLE_VALUE;
    }
    pmgr->getSession(g_sessionID)->getEntryPoint("Characteristic And Status Stream")->saveAllSample();
#endif
	return 0;
}

#ifndef _WINDOWS_PLAT_
void Controller::timer500msCallback(union sigval)
{
	FireEvent eventFire;
	eventFire.eventID = FIRE_EVENT_ID_500ms;
    msgSend(m_snd_hndl, (unsigned char*)&eventFire, sizeof(FireEvent),0);
}
void Controller::timer10000msCallback(union sigval)
{
	FireEvent eventFire;
	eventFire.eventID = FIRE_EVENT_ID_10000ms;
    msgSend(m_snd_hndl, (unsigned char*)&eventFire, sizeof(FireEvent),0);
}
#endif
