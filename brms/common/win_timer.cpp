#ifdef _WINDOWS_PLAT_
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "win_timer.h"

#define TIMERID_BASE	1024

typedef void(*SIGEV_NOTIFY_FUNCTION)(union sigval);

typedef struct {
	timer_t					id;
	int						active;
	SIGEV_NOTIFY_FUNCTION	func;
	void					*param;
	long long				it_value;		// ms
	long long				it_interval;	// ms
	long long				expire;			// ms
} TimerInfo;

typedef struct {
	TimerInfo		timers[TIMER_MAX];
	int				timers_count;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
} WINTIMER;

WINTIMER wintimer;

void *timer_thread(void *args)
{
	struct timespec ts;
	TimerInfo *ti;
	TimerInfo calllist[TIMER_MAX];
	int calllistcnt;
	int ret;
	long long expire;
	long long now;
	union sigval sigval;
	int i;
	
	memset(&wintimer, 0, sizeof(wintimer));
	for (i = 0, ti = &wintimer.timers[0]; i < TIMER_MAX; i++, ti++)
	{
		ti->id = -1;
	}
	pthread_mutex_init(&wintimer.mutex, NULL);
	pthread_cond_init(&wintimer.cond, NULL);
	
	while (1)
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		now = (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		
		pthread_mutex_lock(&wintimer.mutex);
		
		expire = -1;
		calllistcnt = 0;
		for (i = 0, ti = &wintimer.timers[0]; i < TIMER_MAX; i++, ti++)
		{
			if (ti->id != -1 && ti->active)
			{
				if (expire == -1 || ti->expire < expire)
				{
					expire = ti->expire;
				}
				if (ti->expire <= now)
				{
					calllist[calllistcnt++] = *ti;

					if (ti->it_interval > 0)
					{
						ti->expire += ti->it_interval;
					}
					else
					{
						ti->active = 0;
					}
				}
			}
		}
		
		if (expire != -1)
		{
			ts.tv_sec = expire / 1000;
			ts.tv_nsec = expire % 1000 * 1000000;
			ret = pthread_cond_timedwait(&wintimer.cond, &wintimer.mutex, &ts);
		}
		else
		{
			ret = pthread_cond_wait(&wintimer.cond, &wintimer.mutex);
		}
		
		pthread_mutex_unlock(&wintimer.mutex);
		
		if (ret == 0)
		{
			//
		}
		for (i = 0, ti = &calllist[0]; i < calllistcnt; i++, ti++)
		{
			sigval.sival_ptr = ti->param;
			ti->func(sigval);
		}
	}
	
	return NULL;
}

int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid)
{
	int ret = -1;
	TimerInfo *ti;
	int i;
	
	pthread_mutex_lock(&wintimer.mutex);
	for (i = 0, ti = &wintimer.timers[0]; i < TIMER_MAX; i++, ti++)
	{
		if (ti->id == -1)
		{
			*timerid = TIMERID_BASE + i;
			ti->id = i;
			ret = 0;
			
			ti->active = 0;
			ti->func = evp->sigev_notify_function;
			ti->param = evp->sigev_value.sival_ptr;
			ti->it_value = 0;
			ti->it_interval = 0;
			ti->expire = 0;
			break;
		}
	}
	pthread_mutex_unlock(&wintimer.mutex);
	
	return ret;
}

int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *ovalue)
{
	struct timespec ts;
	long long now;

	timerid -= TIMERID_BASE;
	if (timerid < 0 || timerid >= TIMER_MAX)
	{
		return -1;
	}
	
	TimerInfo *ti = &wintimer.timers[timerid];
	if (ti->id != timerid)
	{
		return -1;
	}
	
	pthread_mutex_lock(&wintimer.mutex);
	
	ti->it_value = value->it_value.tv_sec * 1000 + value->it_value.tv_nsec / 1000000;
	ti->it_interval = value->it_interval.tv_sec * 1000 + value->it_interval.tv_nsec / 1000000;
	
	if (ti->it_value != 0)
	{
		clock_gettime(CLOCK_REALTIME, &ts);
		now = (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		
		ti->active = 1;
		ti->expire = now + ti->it_value;
		pthread_cond_signal(&wintimer.cond);
	}
	else
	{
		ti->active = 0;
	}
	
	pthread_mutex_unlock(&wintimer.mutex);
	
	return 0;
}

int timer_delete(timer_t timerid)
{
	timerid -= TIMERID_BASE;
	if (timerid < 0 || timerid >= TIMER_MAX)
	{
		return -1;
	}
	
	TimerInfo *ti = &wintimer.timers[timerid];
	if (ti->id != timerid)
	{
		return -1;
	}
	
	pthread_mutex_lock(&wintimer.mutex);
	ti->id = -1;
	pthread_mutex_unlock(&wintimer.mutex);
	
	return 0;
}

#if 0
void timeout(union sigval sigval)
{
	printf("timeout ..................\n");
}

int main(int argc, const char *argv[])
{
	pthread_t pid;
	pthread_create(&pid, NULL, timer_thread, NULL);
	
	sleep(1);
	
	timer_t tid;
	struct sigevent event;
	event.sigev_value.sival_ptr = NULL;
	event.sigev_notify_function = timeout;
	struct itimerspec its;
	its.it_value.tv_sec = 1;
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = 1;
	its.it_interval.tv_nsec = 0;
	
	timer_create(CLOCK_MONOTONIC, &event, &tid);
	timer_settime(tid, 0, &its, NULL);
	
	pthread_join(pid, NULL);
	
	return 0;
}
#endif
#endif
