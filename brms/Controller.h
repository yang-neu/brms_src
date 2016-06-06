#ifndef _H_CONTROLLER_
#define _H_CONTROLLER_

#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>

#ifndef _WINDOWS_PLAT_
#include <sys/time.h>
#include "common/SysTimer.h"

#else
#include <windows.h>

#endif

#define		D_MSG_NUM				(10)											
#define		D_MSG_SIZE				(4)										
using namespace std;

enum FIRE_EVENT_ID {
 	FIRE_EVENT_ID_500ms,
	FIRE_EVENT_ID_10000ms 
};

 #ifndef _WINDOWS_PLAT_
typedef struct {
	int eventID;
}FireEvent;

#endif

class Controller
{
public:
	static Controller *get()
	{
		if (m_instance == NULL)
			m_instance = new Controller();
		return m_instance;
	}
	bool start();
	bool stop();
	static void *schedulerProc(void *para);
 #ifndef _WINDOWS_PLAT_
	static void timer500msCallback(union sigval);
	static void timer10000msCallback(union sigval);
  #endif
protected:
	Controller();
private:
	static bool m_status;
	//static int m_timer500ms;
	//static int m_timer10000ms;
	 pthread_t m_thread;
	//static pthread_cond_t m_cond;
	//static pthread_mutex_t m_mutex;
	//static bool m_flag ;
#ifndef _WINDOWS_PLAT_
	timer_t m_timer500ms;
    timer_t m_timer10000ms;
    static int m_snd_hndl;
    static int m_rcv_hndl;
#else
    static HANDLE  m_handles[3]; //0:500ms; 1:10s; 2:stop event
#endif

	static Controller *m_instance;
};

#endif /* _H_CONTROLLER_ */


