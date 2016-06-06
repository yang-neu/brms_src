package com.zipc.cockpit.server;

import java.util.ArrayList;
import java.util.Iterator;

import org.kie.api.runtime.KieSession;


public class IntegrationGenerateClientThread extends InfoGetBaseThread {	

	// 基底クラスのプロパティは情報集約サーバ取得用で使う
	
	// 情報集約サーバ通知用
	private DataTrans noticeDataTrans = null;
	private int noticePortNo = 60100;
	
	// 情報生成サーバ取得用
	private DataTrans IGserverDataTrans = null;
	private int IGserverPortNo;
	
	// コンストラクタ
	public IntegrationGenerateClientThread(DNCockpitManager _parent,KieSession _session) {
		super(_parent, _session, "127.0.0.1", 60010, 0);
		noticePortNo = 60100;
		IGserverPortNo = 60030;
	}
	
	@Override
	public void run() {
		// 通信無限ループ部
		while (!halt_) {

			// 情報集約タスク通知ポートとの接続(1秒間隔で無限リトライ)
			noticeDataTrans = connectLoop(serverIp, noticePortNo);
			if(noticeDataTrans == null) {
				return;
			}
			System.out.println("情報集約タスク通知ポートと接続確立");
			// 情報生成タスク情報取得ポートとの接続(1秒間隔で無限リトライ）
			dataTrans = connectLoop(serverIp, serverPortNo);
			if(dataTrans == null) {
				noticeDataTrans.disconnect();
				return;
			}
			System.out.println("情報集約タスク取得ポートと接続確立");
			// 情報集約タスク情報取得ポートとの接続(1秒間隔で無限リトライ）
			IGserverDataTrans = connectLoop(serverIp, IGserverPortNo);
			if(IGserverDataTrans == null) {
				noticeDataTrans.disconnect();
				dataTrans.disconnect();
				return;
			}
			System.out.println("情報生成タスク取得ポートと接続確立");

			while(!halt_) {
				// 情報集約タスクからの通知受信
//				System.out.println("noticeDataTrans.recv start");
				String recvXml = noticeDataTrans.recv();
//				System.out.println("noticeDataTrans.recv end");
				if(recvXml == null) {
//					System.out.println("noticeDataTrans.recvXml null");
					break;
				}
				
				// TimeLiner連携初期時間設定
				if(parent.gui.starttime == 0)
				{
					parent.gui.starttime = System.currentTimeMillis();
				}

				// 通知XML解析
				ArrayList<Integer> list = parseNoticeXml(recvXml);
				if(list == null) {
//					System.out.println("noticeDataTrans.recvXml->list null");
					break;
				}
				
				// 情報集約タスクからの情報取得
				ArrayList<Integer> IAlist = createIAlist(list);
				String IArecvXml = null;
				if(IAlist.size() != 0) {
//					System.out.println("IAinfoGet start");
					IArecvXml = infoGet(dataTrans, IAlist);
//					System.out.println("IAinfoGet end");
					if(IArecvXml == null) {
//						System.out.println("infoGet IArecvXml null");
						break;
					}
				}
				// 情報生成タスクからの情報取得
//				ArrayList<Integer> IGlist = createIGlist(list);
				ArrayList<Integer> IGlist = createIGlist();
				String IGrecvXml = null;
				if(IGlist.size() != 0) {
//					System.out.println("IGinfoGet start");
					IGrecvXml = infoGet(IGserverDataTrans, IGlist);
//					System.out.println("IGinfoGet end");
					if(IGrecvXml == null) {
//						System.out.println("infoGet IGrecvXml null");
						break;
					}
				}
				// 両タスクからの取得XML解析とワーキングメモリにinsert
				synchronized(parent.getLockObjectFire())
				{	
					insertWorkingMemory(IArecvXml);
					insertWorkingMemory(IGrecvXml);
				}
			}
			noticeDataTrans.disconnect();
			dataTrans.disconnect();
			IGserverDataTrans.disconnect();
		}
	}

	private ArrayList<Integer> createIAlist (ArrayList<Integer> _list) {
		ArrayList<Integer> list = new ArrayList<Integer>();

		Iterator<Integer> itr = _list.iterator();
		
		while(itr.hasNext()) {
			Integer infoId = itr.next();
			switch(infoId) {
			case 0x20000001 : // 知覚情報：車速
			case 0x20000002 : // 知覚情報：舵角
			case 0x20000003 : // 知覚情報：アクセル開度
			case 0x20000004 : // 知覚情報：ブレーキ開度
			case 0x20000007 : // 知覚情報：パーキングブレーキ状態
			case 0x20000008 : // 知覚情報：ストップランプSW
			case 0x23000001 : // 知覚情報：車間距離
				list.add(infoId);
				break;
			case 0x20000005 : // 知覚情報：シフト状態
				list.add(infoId);
				list.add(infoId+1);
				break;
			case 0x22000001 : // 知覚情報：顔向き
				list.add(infoId);
				break;
			case 0x22000002 : // 知覚情報：顔位置
				list.add(infoId);
				list.add(infoId+1);
				list.add(infoId+2);
				list.add(infoId+3);
				break;
			case 0x30000001 : // デバッグ用の一括通知
				list.add(0x20000001);
				list.add(0x20000002);
				list.add(0x20000003);
				list.add(0x20000004);
				list.add(0x20000005);
				list.add(0x20000006);
				list.add(0x20000007);
				list.add(0x20000008);
				list.add(0x23000001);
				break;
			}
		}
		return list;
	}

/*
	private ArrayList<Integer> createIGlist (ArrayList<Integer> _list) {
		ArrayList<Integer> list = new ArrayList<Integer>();

		Iterator<Integer> itr = _list.iterator();
		
		while(itr.hasNext()) {
			Integer infoId = itr.next();
			switch(infoId) {
			case 0x00000001 : // 認知共通：自車加速度
			case 0x00000002 : // 認知共通：運転シーンの状態推定
			case 0x00000003 : // 認知共通：自車加速度(1s)
			case 0x00000004 : // 認知共通：自車加速度(0s)
			case 0x02000001 : // 認知共通：漫然度レベル(std)
			case 0x02000002 : // 認知共通：漫然度レベル(low)
			case 0x02000003 : // 認知共通：漫然度レベル(high)
			case 0x02000004 : // 認知共通：漫然度フラグ(std)
			case 0x02000005 : // 認知共通：漫然度フラグ(low)
			case 0x02000006 : // 認知共通：漫然度フラグ(high)
			case 0x02000007 : // 認知共通：正規化漫然ポイント(std)
			case 0x02000008 : // 認知共通：正規化漫然ポイント(low)
			case 0x02000009 : // 認知共通：正規化漫然ポイント(high)
			case 0x0200000A : // 認知共通：負荷レベル
			case 0x0200000B : // 認知共通：脇見判定結果
			case 0x0200000C : // 認知共通：運転阻害脇見判定結果
			case 0x03000001 : // 認知共通：相対速度
			case 0x03000002 : // 認知共通：TTC
			case 0x03000003 : // 認知共通：相対加速度
			case 0x03000004 : // 認知共通：先行車加速度
			case 0x10000001 : // 認知個別：減速区間情報抽出
			case 0x12000001 : // 認知個別：前方車潜在リスク見落とし有無
			case 0x12000002 : // 認知個別：減速開始時のTTC
			case 0x12000003 : // 認知個別：TTCの下限ピーク
			case 0x12000004 : // 認知個別：減速開始時の車間時間
			case 0x12000005 : // 認知個別：減速区間の車間時間下限ピーク
			case 0x12000006 : // 認知個別：停車時の車間距離
			case 0x12000007 : // 認知個別：減速区間の自車減速度上限ピーク
			case 0x12000008 : // 認知個別：減速区間内初期ブレーキ時のTTC
			case 0x12000009 : // 認知個別：安全運転特性診断結果
			case 0x1200000A : // 認知個別：減速開始時のTTC(累積平均)
			case 0x1200000B : // 認知個別：TTCの下限ピーク(累積平均)
			case 0x1200000C : // 認知個別：安全運転特性診断結果(累積平均)
			case 0x1200000D : // 認知個別：運転特性累積回数
			case 0x13000001 : // 認知個別：前方車の潜在リスク有無
			case 0x13000002 : // 認知個別：余裕時間
				list.add(infoId);
				break;
			}
		}
		return list;
	}
*/
	private ArrayList<Integer> createIGlist () {
		ArrayList<Integer> list = new ArrayList<Integer>();

//		list.add(0x00000001); // 認知共通：自車加速度
//		list.add(0x00000002); // 認知共通：運転シーンの状態推定
//		list.add(0x00000003); // 認知共通：自車加速度(1s)
//		list.add(0x00000004); // 認知共通：自車加速度(0s)
		list.add(0x02000001); // 認知共通：漫然度レベル(std)
//		list.add(0x02000002); // 認知共通：漫然度レベル(low)
//		list.add(0x02000003); // 認知共通：漫然度レベル(high)
//		list.add(0x02000004); // 認知共通：漫然度フラグ(std)
//		list.add(0x02000005); // 認知共通：漫然度フラグ(low)
//		list.add(0x02000006); // 認知共通：漫然度フラグ(high)
//		list.add(0x02000007); // 認知共通：正規化漫然ポイント(std)
//		list.add(0x02000008); // 認知共通：正規化漫然ポイント(low)
//		list.add(0x02000009); // 認知共通：正規化漫然ポイント(high)
		list.add(0x0200000A); // 認知共通：負荷レベル
//		list.add(0x0200000B); // 認知共通：脇見判定結果
//		list.add(0x0200000C); // 認知共通：運転阻害脇見判定結果
//		list.add(0x03000001); // 認知共通：相対速度
//		list.add(0x03000002); // 認知共通：TTC
//		list.add(0x03000003); // 認知共通：相対加速度
//		list.add(0x03000004); // 認知共通：先行車加速度
//		list.add(0x10000001); // 認知個別：減速区間情報抽出
//		list.add(0x12000001); // 認知個別：前方車潜在リスク見落とし有無
//		list.add(0x12000002); // 認知個別：減速開始時のTTC
//		list.add(0x12000003); // 認知個別：TTCの下限ピーク
//		list.add(0x12000004); // 認知個別：減速開始時の車間時間
//		list.add(0x12000005); // 認知個別：減速区間の車間時間下限ピーク
//		list.add(0x12000006); // 認知個別：停車時の車間距離
//		list.add(0x12000007); // 認知個別：減速区間の自車減速度上限ピーク
//		list.add(0x12000008); // 認知個別：減速区間内初期ブレーキ時のTTC
//		list.add(0x12000009); // 認知個別：安全運転特性診断結果
//		list.add(0x1200000A); // 認知個別：減速開始時のTTC(累積平均)
//		list.add(0x1200000B); // 認知個別：TTCの下限ピーク(累積平均)
//		list.add(0x1200000C); // 認知個別：安全運転特性診断結果(累積平均)
//		list.add(0x1200000D); // 認知個別：運転特性累積回数
//		list.add(0x13000001); // 認知個別：前方車の潜在リスク有無
//		list.add(0x13000002); // 認知個別：余裕時間

		return list;
	}

	
	@Override
	protected ArrayList<Integer> createInfoGetList() {
		// 情報取得IDは情報集約タスクから指定されるため、このメソッドは使えない
		return null;
	}
}