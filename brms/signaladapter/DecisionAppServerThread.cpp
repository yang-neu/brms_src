#include "InfoGetBaseThread.h"
#include "DataTrans.h"
#include "DecisionAppServerThread.h"

// 実行メソッド
void DecisionAppServerThread::run() 
{

		// サーバー起動
		dataTrans = new DataTrans(serverIp, serverPortNo);
		if(dataTrans->startup() == false) {
			return;
		}

		// 受信ループ部
		while(!halt_) {
            if(dataTrans->acceptInter() == false) {
				return;
			}
			cout<<"判断アプリと接続確立"<<endl;
			string recvXml;
            bool ret = dataTrans->recvInter(recvXml);
			if(ret == false) {
				dataTrans->disconnect();
				continue;
			}
			// 取得要求XML解析
			list<int> _list; 
			ret = parseNoticeXml(recvXml, _list);
			if(ret == false) {
				dataTrans->disconnect();
				continue;
			}

			// ワーキングメモリから情報収集
			string sendXml; 
			ret = createInfoResponseXml(_list, sendXml);

			// InfoResponse送信
            if( dataTrans->sendInter(sendXml) == false) {
				dataTrans->disconnect();
				continue;
			}
			dataTrans->disconnect();
			cout<<"判断アプリと接続切断"<<endl;
		}
		dataTrans->shutdown();
}
	


