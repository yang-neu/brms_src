
#include "Message.h"


//////////////////////////////////////////////////
//			定数数宣言

const char	*pac_mqname_list[]	=	{
            "/Ctrl_Timer",
            "/Debug_Input"
};

#ifndef _WINDOWS_PLAT_

int msgOpen(U4 u4_id, U4 u4_access, U4 u4_count, U4 u4_size, U4 u4_opt)
{
	mqd_t				mqd			= 0;		//	キュー識別子.
	struct mq_attr		attr;					//	キューの設定.
	const char			*pMQName	= NULL;		//	キューの名前.
	int					acceccFlag	= 0;		//	キューのアクセスフラグ.
	int					i_ret		= 0;
	//	キューIDが範囲外もしくは有効なbit以外が指定されている場合はエラー.
	if(		(u4_id >= EN_MSGID_END) || 
			((u4_access & (D_MSG_READ | D_MSG_WRITE | D_MSG_CREAT)) != u4_access) ){
		cout<<"msgOpen error : mqid->"<<u4_id<<"; access->"<<u4_access<<endl;
		return -1;
	}
	if((u4_opt != 0) && (u4_opt != 1)){
		return -1;
	}

	memset(&attr, 0x00, sizeof(attr));
	
	//	アクセスするメッセージキュー設定.
	pMQName = pac_mqname_list[u4_id];
	
	//	アクセス条件設定.
	acceccFlag = 0;
	if( (u4_access & (D_MSG_READ | D_MSG_WRITE)) == (D_MSG_READ | D_MSG_WRITE)){
		acceccFlag = O_RDWR;
	}
	else if(u4_access & D_MSG_READ){
		acceccFlag = O_RDONLY;
	}
	else if(u4_access & D_MSG_WRITE){	/* 今後BITが増えることを想定して、明示的にif分判定を記載 */
		acceccFlag = O_WRONLY;
	}
	if(u4_access & D_MSG_CREAT){
		acceccFlag |= O_CREAT;
	}
	
	//	作成するキューの情報を設定（メッセジキューは作成時しか指定できません。）.
	attr.mq_flags		= 0;				//	フラグ: 0 か O_NONBLOCK(mq_openで設定できないのでmq_setattr()で設定する).
	attr.mq_maxmsg		= (int)u4_count;		//	キューの最大メッセージ数.		
	attr.mq_msgsize		= (int)u4_size;		//	最大メッセージサイズ (バイト単位).
	attr.mq_curmsgs		= 0;				//	キューに現在入っているメッセージ数.
	//	キューオープン.
	mqd = mq_open(pMQName, (int)acceccFlag, S_IRWXU | S_IRWXO, &attr );
	if(mqd == (mqd_t)-1){
		//cout<<"msgOpen error:mq_open->"<<errno<<endl;
		return -1;
	}
	if(u4_opt == 1){
		attr.mq_flags		= O_NONBLOCK;
		i_ret = mq_setattr(mqd, &attr, NULL);
		if(i_ret != 0){
		//	cout<<"msgOpen error:mq_setattr->"<<errno<<endl;
		}
	}
	return (int)mqd;
}


int msgSend(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt)
{
	mqd_t	mqd		= (mqd_t)i4_hndl;					//	キュー識別子.
	int		i4_ret	= 0;
	U4		u4_ret	= 0;
	if(pa_buff == NULL){
		cout<<"msgSend error:buffer null"<<endl;
		return -1;
	}
	
	//	キューにデータ追加.
	i4_ret = mq_send(mqd, (char*)pa_buff, (size_t)u4_len, u4_opt);	
	if(i4_ret != 0){
	//	cout<<"msgSend error:mq_send->"<<errno<<endl;
		u4_ret = -1;
	}
	
	return ((int)u4_ret);
}


int msgTimedsend(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt, timespec  * s_timeout)
{
	/* 内部変数宣言 */
	mqd_t	mqd		= (mqd_t)i4_hndl;					/* キュー識別子 */
	int		i4_ret	= 0;
	U4		u4_ret	= 0;
	
	/* 引数のチェック */
	if( (NULL == pa_buff) || (NULL == s_timeout) )
	{
		cout<<"msgTimedsend error: parameter null"<<endl;
		return -1;
	}

	/* キューにデータ追加 */
	i4_ret = mq_timedsend(mqd, (char*)pa_buff, (size_t)u4_len, u4_opt, s_timeout);
	if ( 0 != i4_ret )
	{
	//	cout<<"msgTimedsend error: mq_timedsend -> "<<errno<<endl;
		u4_ret = -1;
	}
	
	return ((int)u4_ret);
}


int msgGetattr(int i4_hndl, mq_attr  * s_attributes)
{
	/* 内部変数宣言 */
	mqd_t	mqd		= (mqd_t)i4_hndl;				/* キュー識別子 */
	int		i4_ret	= 0;							/* 関数処理結果 */
	U4		u4_ret	= 0;					/* 関数処理結果 */
	
	/* 引数のチェック */
	if( NULL == s_attributes )
	{
		cout<<"msgGetattr error: parameter null"<<endl;
		return -1;
	}

	/* メッセージキュー情報取得 */
	i4_ret = mq_getattr(mqd, s_attributes);
	if ( 0 != i4_ret )
	{
	//	cout<<"msgGetattr error: mq_getattr -> "<<errno<<endl;
		u4_ret = -1;
	}
	
	return ((int)u4_ret);
}


int msgRecv(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt)
{
	mqd_t			mqd			= (mqd_t)i4_hndl;					//	キュー識別子.
	unsigned int	priority	= 0;
	int				i4_ret = 0;
	if(pa_buff == NULL){
		cout<<"msgRecv error: parameter null"<<endl;
		return -1;
	}
	
	
	//	キューからデータ取得.
	i4_ret = mq_receive(mqd, (char*)pa_buff, (unsigned int)u4_len, &priority);
	if(i4_ret == -1){
	//	if(errno == EAGAIN){
			//	ノンブロッキング時はデータなしのときはこのエラー番号なのでデータ長０で応答する.
	//		i4_ret = 0;
//	}
	//	else {
	//		cout<<"msgRecv error: mq_receive -> "<<errno<<endl;
//	}
	}
	
	return i4_ret;
}
int msgTimedRecv(int i4_hndl, U1 *pa_buff, U4 u4_len, U4 u4_opt, int ms)
{
    mqd_t			mqd			= (mqd_t)i4_hndl;					//	キュー識別子.
    unsigned int	priority	= 0;
    int				i4_ret = 0;
    struct timespec s_timeout;

    if(pa_buff == NULL){
        cout<<"msgRecv error: parameter null"<<endl;
        return -1;
    }
    U4 u4_time = ms * 1000000L;
    /* リアルタイム・システムの時間を得ます(ns) */
    i4_ret = clock_gettime( CLOCK_REALTIME, &s_timeout);
    if ( -1 == i4_ret )
    {
        return -1;
    }

    /* nanoseconds > 1s */
    if ( 1000000000L <= ((U4)s_timeout.tv_nsec + u4_time % 1000000000L) )
    {
        s_timeout.tv_sec += ((int)u4_time / 1000000000L) + 1;
        s_timeout.tv_nsec = s_timeout.tv_nsec + ((int)u4_time % 1000000000L) - 1000000000L;
    }
    /* nanoseconds < 1s */
    else
    {
        s_timeout.tv_sec  += (int)(u4_time / 1000000000L);
        s_timeout.tv_nsec += (int)(u4_time % 1000000000L);
    }

    //	キューからデータ取得.
    i4_ret = mq_timedreceive(mqd, (char*)pa_buff, (unsigned int)u4_len, &priority, &s_timeout);
    if(i4_ret == -1){
    //	if(errno == EAGAIN){
            //	ノンブロッキング時はデータなしのときはこのエラー番号なのでデータ長０で応答する.
    //		i4_ret = 0;
//	}
    //	else {
    //		cout<<"msgRecv error: mq_receive -> "<<errno<<endl;
//	}
    }

    return i4_ret;
}

U4 msgClose(int i4_hndl)
{
	U4		i4_ret		= 0;
	mqd_t	mqd			= (mqd_t)i4_hndl;					//	キュー識別子.
	int		i_ret		= 0;
	//	キューをクローズ.
	i_ret = mq_close(mqd);
	if(i_ret != 0){
	//	cout<<"msgClose error: mq_close -> "<<errno<<endl;
		i4_ret = -1;
	}
	
	return (U4)i4_ret;
}
#endif

