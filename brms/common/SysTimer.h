
#ifndef _H_SYS_TIMER_
#define _H_SYS_TIMER_

#ifndef _WINDOWS_PLAT_
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#else
#include "win_timer.h"
#endif

typedef void (*TimerCallBak)(union sigval val);

extern int timerCreate( TimerCallBak p_callback, void * p_callback_argv, timer_t * ps_timer_id);
extern int timerStart( timer_t * ps_timer_id, int u4_timeout_msec);   
extern int timerStop( timer_t * ps_timer_id);
extern int timerEnd( timer_t * ps_timer_id);
extern int timerStartRepeat( timer_t *ps_timer_id, int u4_interval_msec );
#endif
