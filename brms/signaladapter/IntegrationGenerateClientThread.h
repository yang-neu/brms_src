#ifndef _INTEGRATION_GENERATE_CLIENT_H_
#define _INTEGRATION_GENERATE_CLIENT_H_

#include <list>
#include <string>
#include "InfoGetBaseThread.h"
#include "../brms/common/ConfigUtil.h"

#define MAX_CLIENT_SIZE 4*1024
class DataTrans;

using namespace std;
 class IntegrationGenerateClientThread: public InfoGetBaseThread 
{	
	// コンストラクタ
public: 
	IntegrationGenerateClientThread(string _session)
		: InfoGetBaseThread(_session, ConfigUtil::GetInstance()->GetConfigStr("情報集約アドレス"), ConfigUtil::GetInstance()->GetConfigInt("情報集約取得ポート"), 0)
		, noticeDataTrans(NULL)
		, noticePortNo(ConfigUtil::GetInstance()->GetConfigInt("情報集約通知ポート"))
		, IGserverDataTrans(NULL)
		, IGserverPortNo(ConfigUtil::GetInstance()->GetConfigInt("情報生成取得ポート"))
	{
	}
	
	void run();
	

private:
	void createIAlist (list<int> _list, list<int> &result) ;
	void createIGlist (list<int> &result);
	

protected:
	void createInfoGetList(list<int> &result);
private:
	DataTrans *noticeDataTrans ;
	int noticePortNo;
	
	// 情報生成サーバ取得用
	DataTrans *IGserverDataTrans;
	int IGserverPortNo;
    char m_clientData[MAX_CLIENT_SIZE];
};
#endif
