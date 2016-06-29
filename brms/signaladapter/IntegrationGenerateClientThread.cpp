#include "IntegrationGenerateClientThread.h"
#include "InfoGetBaseThread.h"
#include "DataTrans.h"
#include "../common/ConfigUtil.h"

#define COM_LOG

void IntegrationGenerateClientThread::run() 
{
#ifdef COM_LOG
	ofstream ofs;
	if (ConfigUtil::GetInstance()->GetConfigStr("通信ログ出力パス").length() > 0)
	{
		ofs.open(ConfigUtil::GetInstance()->GetConfigStr("通信ログ出力パス").c_str());
	}
	string sendxml;
	char id[128];
#endif
	// 通信無限ループ部
		while (!halt_) {

			// 情報集約タスク通知ポートとの接続(1秒間隔で無限リトライ)
			noticeDataTrans = connectLoop(serverIp, noticePortNo);
			if(noticeDataTrans == NULL) {
				return;
			}
			//cout<<"情報集約タスク通知ポートと接続確立"<<endl;
			cout<<"Info Notify port connected"<<endl;
			// 情報生成タスク情報取得ポートとの接続(1秒間隔で無限リトライ）
			dataTrans = connectLoop(serverIp, serverPortNo);
			if(dataTrans == NULL) {
				noticeDataTrans->disconnect();
				return;
			}
			//cout<<"情報集約タスク取得ポートと接続確立"<<endl;
			cout<<"Info Date port connected"<<endl;
			// 情報集約タスク情報取得ポートとの接続(1秒間隔で無限リトライ）
			IGserverDataTrans = connectLoop(serverIp, IGserverPortNo);
			if(IGserverDataTrans == NULL) {
				noticeDataTrans->disconnect();
				dataTrans->disconnect();
				return;
			}
			//cout<<"情報生成タスク取得ポートと接続確立"<<endl;
			cout<<"Info Create port connected"<<endl;
			ClipsSessionMgr::get()->getSession(session)->setState(ClipsSession::SESSIONSTATE_RECVING);
			while(!halt_) {
				// 情報集約タスクからの通知受信
//				System.out.println("noticeDataTrans.recv start");
                string recvXml;
                //int size = MAX_CLIENT_SIZE;
                //memset(m_clientData, 0, size);
                bool ret = noticeDataTrans->recvInter(recvXml);
#ifdef COM_LOG
				if (ofs.good())
				{
					sprintf(id, "%10.3f NT[%s:%d]", GetSystemTime() / 1000.0, serverIp.c_str(), noticePortNo);
					ofs.write(id, strlen(id));
					ofs.write(recvXml.c_str(), recvXml.length());
					ofs.write("\n", 1);
					//ofs.flush();
				}
#endif
//				System.out.println("noticeDataTrans.recv end");
				if(ret == false) {
//					System.out.println("noticeDataTrans.recvXml null");
					break;
				}
				#if 0
				// TimeLiner連携初期時間設定
				if(parent.gui.starttime == 0)
				{
					parent.gui.starttime = System.currentTimeMillis();
				}
				#endif
				
				// 通知XML解析
				list<int> _list;
                ret = parseNoticeXml(recvXml,_list);
				if(ret == false) {
//					System.out.println("noticeDataTrans.recvXml->list null");
					break;
				}

				// 情報集約タスクからの情報取得
				list<int> IAlist; 
                createIAlist(_list, IAlist);
				
				string IArecvXml;
				
                if(IAlist.size() != 0) {
                    //cout<<"IAinfoGet start"<<endl;
					ret = infoGet(dataTrans, IAlist, IArecvXml);
#ifdef COM_LOG
					if (ofs.good())
					{
						sendxml.clear();
						createInfoGetXml(IAlist, sendxml);
						sprintf(id, "%10.3f RA[%s:%d]", GetSystemTime() / 1000.0, serverIp.c_str(), serverPortNo);
						ofs.write(id, strlen(id));
						ofs.write(sendxml.c_str(), sendxml.length());
						ofs.write("\n", 1);

						sprintf(id, "%10.3f IA[%s:%d]", GetSystemTime() / 1000.0, serverIp.c_str(), serverPortNo);
						ofs.write(id, strlen(id));
						ofs.write(IArecvXml.c_str(), IArecvXml.length());
						ofs.write("\n", 1);
						//ofs.flush();
					}
#endif
					//cout<<"IAinfoGet end"<<endl;
					if(ret == false) {
						cout<<"infoGet IArecvXml null"<<endl;
						break;
					}
					// 両タスクからの取得XML解析とワーキングメモリにinsert
					insertWorkingMemory(IArecvXml);
				}
#if 0 // 取得情報なし
				// 情報生成タスクからの情報取得
				list<int> IGlist;
				createIGlist(IGlist);
				
				string IGrecvXml;
                if(IGlist.size() != 0) {
                    //cout<<"IGinfoGet start"<<endl;
					ret = infoGet(IGserverDataTrans, IGlist, IGrecvXml);
#ifdef COM_LOG
					if (ofs.good())
					{
						sendxml.clear();
						createInfoGetXml(IGlist, sendxml);
						sprintf(id, "%10.3f RG[%s:%d]", GetSystemTime() / 1000.0, serverIp.c_str(), IGserverPortNo);
						ofs.write(id, strlen(id));
						ofs.write(sendxml.c_str(), sendxml.length());
						ofs.write("\n", 1);

						sprintf(id, "%10.3f IG[%s:%d]", GetSystemTime() / 1000.0, serverIp.c_str(), IGserverPortNo);
						ofs.write(id, strlen(id));
						ofs.write(IGrecvXml.c_str(), IGrecvXml.length());
						ofs.write("\n", 1);
						//ofs.flush();
					}
#endif
					//cout<<"IGinfoGet end"<<endl;
					if(ret == false) {
						cout<<"infoGet IGrecvXml null"<<endl;
						break;
					}
					// 両タスクからの取得XML解析とワーキングメモリにinsert
					insertWorkingMemory(IGrecvXml);
				}
#endif
			}
			ClipsSessionMgr::get()->getSession(session)->setState(ClipsSession::SESSIONSTATE_IDLE);
			noticeDataTrans->disconnect();
			dataTrans->disconnect();
			IGserverDataTrans->disconnect();
		}
#ifdef COM_LOG
	ofs.close();
#endif
}

void IntegrationGenerateClientThread::createIAlist (list<int> _list, list<int> &result) 
{
	list<int>::iterator itr = _list.begin();
	
	int infoId;
    while(itr != _list.end())
	{
		infoId = (*itr);
		switch(infoId) {
		case 0x20000001 : // 知覚情報：車速
		case 0x20000002 : // 知覚情報：舵角
		case 0x20000003 : // 知覚情報：アクセル開度
		case 0x20000004 : // 知覚情報：ブレーキ開度
		case 0x20000007 : // 知覚情報：パーキングブレーキ状態
		case 0x20000008 : // 知覚情報：ストップランプSW
		case 0x23000001 : // 知覚情報：車間距離
			result.push_back(infoId);
			break;
		case 0x20000005 : // 知覚情報：シフト状態
			result.push_back(infoId);
			result.push_back(infoId+1);
			result.push_back(infoId+4);
			break;
		case 0x22000001 : // 知覚情報：顔向き
			result.push_back(infoId);
			break;
		case 0x22000002 : // 知覚情報：顔位置
			result.push_back(infoId);
			result.push_back(infoId+1);
			result.push_back(infoId+2);
			result.push_back(infoId+3);
			break;
		case 0x30000001 : // デバッグ用の一括通知
			result.push_back(0x20000001);
			result.push_back(0x20000002);
			result.push_back(0x20000003);
			result.push_back(0x20000004);
			result.push_back(0x20000005);
			result.push_back(0x20000006);
			result.push_back(0x20000007);
			result.push_back(0x20000008);
			result.push_back(0x23000001);
			break;
		}
		itr++;
	}

}


void IntegrationGenerateClientThread::createIGlist (list<int> &result)  
{

//		result.push_back(0x00000001); // 認知共通：自車加速度
//		result.push_back(0x00000002); // 認知共通：運転シーンの状態推定
//		result.push_back(0x00000003); // 認知共通：自車加速度(1s)
//		result.push_back(0x00000004); // 認知共通：自車加速度(0s)
		result.push_back(0x02000001); // 認知共通：漫然度レベル(std)
//		result.push_back(0x02000002); // 認知共通：漫然度レベル(low)
//		result.push_back(0x02000003); // 認知共通：漫然度レベル(high)
//		result.push_back(0x02000004); // 認知共通：漫然度フラグ(std)
//		result.push_back(0x02000005); // 認知共通：漫然度フラグ(low)
//		result.push_back(0x02000006); // 認知共通：漫然度フラグ(high)
//		result.push_back(0x02000007); // 認知共通：正規化漫然ポイント(std)
//		result.push_back(0x02000008); // 認知共通：正規化漫然ポイント(low)
//		result.push_back(0x02000009); // 認知共通：正規化漫然ポイント(high)
		result.push_back(0x0200000A); // 認知共通：負荷レベル
//		result.push_back(0x0200000B); // 認知共通：脇見判定結果
//		result.push_back(0x0200000C); // 認知共通：運転阻害脇見判定結果
//		result.push_back(0x03000001); // 認知共通：相対速度
//		result.push_back(0x03000002); // 認知共通：TTC
//		result.push_back(0x03000003); // 認知共通：相対加速度
//		result.push_back(0x03000004); // 認知共通：先行車加速度
//		result.push_back(0x10000001); // 認知個別：減速区間情報抽出
//		result.push_back(0x12000001); // 認知個別：前方車潜在リスク見落とし有無
//		result.push_back(0x12000002); // 認知個別：減速開始時のTTC
//		result.push_back(0x12000003); // 認知個別：TTCの下限ピーク
//		result.push_back(0x12000004); // 認知個別：減速開始時の車間時間
//		result.push_back(0x12000005); // 認知個別：減速区間の車間時間下限ピーク
//		result.push_back(0x12000006); // 認知個別：停車時の車間距離
//		result.push_back(0x12000007); // 認知個別：減速区間の自車減速度上限ピーク
//		result.push_back(0x12000008); // 認知個別：減速区間内初期ブレーキ時のTTC
//		result.push_back(0x12000009); // 認知個別：安全運転特性診断結果
//		result.push_back(0x1200000A); // 認知個別：減速開始時のTTC(累積平均)
//		result.push_back(0x1200000B); // 認知個別：TTCの下限ピーク(累積平均)
//		result.push_back(0x1200000C); // 認知個別：安全運転特性診断結果(累積平均)
//		result.push_back(0x1200000D); // 認知個別：運転特性累積回数
//		result.push_back(0x13000001); // 認知個別：前方車の潜在リスク有無
//		result.push_back(0x13000002); // 認知個別：余裕時間


}

	

void IntegrationGenerateClientThread:: createInfoGetList(list<int> &result) 
{
		// 情報取得IDは情報集約タスクから指定されるため、このメソッドは使えない
		return ;
}
