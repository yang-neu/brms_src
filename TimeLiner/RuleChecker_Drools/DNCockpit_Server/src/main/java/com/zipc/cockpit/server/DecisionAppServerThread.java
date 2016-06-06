package com.zipc.cockpit.server;

import java.util.ArrayList;

import org.kie.api.runtime.KieSession;

public class DecisionAppServerThread extends InfoGetBaseThread {

	public boolean halt_ = false;
	
	// コンストラクタ
	public DecisionAppServerThread(DNCockpitManager _parent,KieSession _session) {
		super(_parent, _session, "127.0.0.1", 10100, 0);
	}

	// 実行メソッド
	public void run() {

		// サーバー起動
		dataTrans = new DataTrans(serverIp, serverPortNo);
		if(dataTrans.startup() == false) {
			return;
		}

		// 受信ループ部
		while(!halt_) {
			if(dataTrans.accept() == false) {
				return;
			}
			System.out.println("判断アプリと接続確立");
			String recvXml = dataTrans.recv();
			if(recvXml == null) {
				dataTrans.disconnect();
				continue;
			}
			// 取得要求XML解析
			ArrayList<Integer> list = parseNoticeXml(recvXml);
			if(list == null) {
				dataTrans.disconnect();
				continue;
			}

			// ワーキングメモリから情報収集
			String sendXml = createInfoResponseXml(list);

			// InfoResponse送信
			if( dataTrans.send(sendXml) == false) {
				dataTrans.disconnect();
				continue;
			}
			dataTrans.disconnect();
			System.out.println("判断アプリと接続切断");
		}
		dataTrans.shutdown();
	}
	
	@Override
	protected ArrayList<Integer> createInfoGetList() {
		// サーバー側なので実装不要
		return null;
	}
}
