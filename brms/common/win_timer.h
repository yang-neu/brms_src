#ifndef _WIN_TIMER_H_
#define _WIN_TIMER_H_

#ifdef _WINDOWS_PLAT_
#include <pthread.h>
#include <pthread_time.h>

#define TIMER_MAX		10
#define SIGEV_THREAD	0

typedef int timer_t;

union sigval {
	int    sival_int;    // Integer signal value. [NOUSE]
	void  *sival_ptr;    // Pointer signal value.
};

struct sigevent {
	int                    sigev_notify;            // Notification type. [only SIGEV_THREAD]
	int                    sigev_signo;             // Signal number. [NOUSE]
	union sigval           sigev_value;             // Signal value. 
	void(*sigev_notify_function)(union sigval);     // Notification function. 
	pthread_attr_t        *sigev_notify_attributes; // Notification attributes.[NOUSE]
};

void *timer_thread(void *args);
int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *ovalue);
int timer_delete(timer_t timerid);
#endif

#endif
