#ifndef _DECISION_APP_SERVER_H_
#define _DECISION_APP_SERVER_H_
//class InfoGetBaseThread;
#include "InfoGetBaseThread.h"

class DecisionAppServerThread: public  InfoGetBaseThread 
{
public:
	 bool halt_;
	
	// コンストラクタ
	DecisionAppServerThread(string session):
		InfoGetBaseThread(session, "127.0.0.1", 10100, 0)
	  , halt_(false)
     {

	}

	// 実行メソッド
	virtual void run();

	
	

	virtual  void createInfoGetList(list<int> &result) {
		// サーバー側なので実装不要
        return ;
	}
};
#endif
