#include <stdio.h>
#ifdef _PC_
#include <sys/signal.h>
#else
#include <signal.h>
#endif
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include "SysTimer.h"

int timerCreate( TimerCallBak p_callback, void * p_callback_argv, timer_t * ps_timer_id)
{
	struct sigevent		s_sig_event;				
	int					i_ret = 0;					
     
 	int					u4_result = 0;	
	
	if(p_callback == NULL  || ps_timer_id == NULL){
     
 		return -1;
	}
	
	memset(&s_sig_event, 0x00, sizeof(s_sig_event));
	s_sig_event.sigev_notify				=	SIGEV_THREAD;
	s_sig_event.sigev_notify_function		=	p_callback;
	s_sig_event.sigev_notify_attributes		=	NULL;
	s_sig_event.sigev_value.sival_ptr		=	p_callback_argv;
	
	i_ret = timer_create(CLOCK_MONOTONIC, &s_sig_event, ps_timer_id);
	if (i_ret == -1){
 		u4_result = -1;
	}
	return u4_result;
}


 int timerStart( timer_t * ps_timer_id, int u4_timeout_msec)
{
	struct itimerspec	s_timer_spec;				
	int					i_ret = 0;				

     
 	int					u4_result = 0;	
      

	
	if(ps_timer_id == NULL){
     
 		return -1;
      
	}

	memset(&s_timer_spec, 0x00, sizeof(s_timer_spec));
	s_timer_spec.it_value.tv_sec			=	(int)(u4_timeout_msec / 1000);	
	s_timer_spec.it_value.tv_nsec			=	1000000 * (int)(u4_timeout_msec % 1000);
	s_timer_spec.it_interval.tv_sec			=	0;
	s_timer_spec.it_interval.tv_nsec		=	0;
	
	i_ret = timer_settime(*ps_timer_id, 0, &s_timer_spec, 0);
	if (i_ret == -1){
 		u4_result = -1;
	}
	return u4_result;
}
     
 int timerStop( timer_t * ps_timer_id)
{
	struct itimerspec	s_timer_spec;				
	int					i_ret = 0;					
     
 	int					u4_result = 0;	
	
	if(ps_timer_id == NULL){
     
 		return -1;
	}
	

	memset(&s_timer_spec, 0x00, sizeof(s_timer_spec));
	s_timer_spec.it_value.tv_sec			=	0;
	s_timer_spec.it_value.tv_nsec			=	0;
	s_timer_spec.it_interval.tv_sec			=	0;
	s_timer_spec.it_interval.tv_nsec		=	0;
	
	i_ret = timer_settime(*ps_timer_id, 0, &s_timer_spec, 0);
	if (i_ret == -1){
     
 		u4_result = -1;
	}
	return u4_result;
}

 int timerEnd( timer_t * ps_timer_id)
{
	int					i_ret = 0;						
     
 	int					u4_result = 0;	
      
	if(ps_timer_id == NULL){
     
 		return -1;
	}
	
	i_ret = timer_delete(*ps_timer_id);
	if (i_ret == -1){
     
 		u4_result = -1;
	}
	return u4_result;
}

 int timerStartRepeat( timer_t *ps_timer_id, int u4_interval_msec )
{
	struct itimerspec	s_timer_spec;					
	int					i_ret = 0;						
     
 	int					u4_result = 0;	
	
	if(ps_timer_id == NULL){
     
 		return -1;
	}
	

	memset(&s_timer_spec, 0x00, sizeof(s_timer_spec));
	s_timer_spec.it_value.tv_sec			=	(time_t)(u4_interval_msec / 1000);
	s_timer_spec.it_value.tv_nsec			=	1000000 * (int)(u4_interval_msec % 1000);
	s_timer_spec.it_interval.tv_sec			=	(time_t)(u4_interval_msec / 1000);
	s_timer_spec.it_interval.tv_nsec		=	1000000 * (int)(u4_interval_msec % 1000);
	
	i_ret = timer_settime(*ps_timer_id, 0, &s_timer_spec, 0);
	if (i_ret == -1){
     
 		u4_result = -1;
     
	}
	return u4_result;
}


