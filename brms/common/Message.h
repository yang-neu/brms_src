#ifndef _H_MESSAGE_
#define _H_MESSAGE_

#ifndef _WINDOWS_PLAT_
#include <iostream>
#include <mqueue.h>
#include <sys/stat.h>	
//#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#define U4 unsigned int
#define U1 unsigned char

#define	D_MSG_READ			0x1			//	読み込みフラグ.
#define	D_MSG_WRITE			0x2			//	書き込みフラグ.
#define	D_MSG_CREAT			0x4			//	キュー生成（無いときにキューを作成する）.

enum {
 		EN_MSGID_TIMER	= 0	,		
        EN_MSGID_DEBUG,
 		EN_MSGID_END	
};
using namespace std;
extern int msgOpen(U4 u4_id, U4 u4_access, U4 u4_count, U4 u4_size, U4 u4_opt);
extern int msgSend(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt);
extern int msgTimedsend(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt, timespec  * s_timeout);
extern int msgGetattr(int i4_hndl, mq_attr  * s_attributes);
extern int msgRecv(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt);
extern int msgTimedRecv(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt, int ms);
extern U4 msgClose(int i4_hndl);
#endif

#endif
