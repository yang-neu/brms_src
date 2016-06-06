package com.zipc.cockpit.server;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.StringWriter;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Iterator;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.kie.api.runtime.KieSession;
import org.w3c.dom.DOMException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import zipc.brms.EventOutput;

import com.zipc.cockpit.server.DNCockpitManager.GlobalManager;
import com.zipc.cockpit.server.utility.AccelOpenUtility;
import com.zipc.cockpit.server.utility.AimlessLevelUtility;
import com.zipc.cockpit.server.utility.DistanceUtility;
import com.zipc.cockpit.server.utility.DriverFaceDirectionUtility;
import com.zipc.cockpit.server.utility.LoadLevelUtility;
import com.zipc.cockpit.server.utility.ShiftStateReverseUtility;
import com.zipc.cockpit.server.utility.SpeedUtility;
import com.zipc.cockpit.server.utility.UtilityBase;

public abstract class InfoGetBaseThread extends Thread {

	public boolean halt_ = false;
	
	protected DNCockpitManager parent;
	protected GlobalManager globalManager;
	protected KieSession session;
	protected DataTrans dataTrans = null;
		
	protected String serverIp;
	protected int serverPortNo;
	int pollingTime;
	
	// コンストラクタ
	public InfoGetBaseThread(DNCockpitManager _parent, KieSession _session, String _serverIp, int _serverPortNo, int _pollingTime) {
		parent        = _parent;
		session       = _session;
		serverIp      = _serverIp;
		serverPortNo  = _serverPortNo;
		pollingTime   = _pollingTime;
		globalManager = _parent.getGlobalManager();
	}

	// 実行メソッド
	public void run() {
		// 通信無限ループ部
		while (!halt_) {

			// 情報生成タスク取得ポートとの接続(1秒間隔で無限リトライ)
			dataTrans = connectLoop(serverIp, serverPortNo);
			if(dataTrans == null) {
				return;
			}

			ArrayList<Integer> list = createInfoGetList();
			String recvXml = null;

			while(!halt_) {
				// 情報取得
				if(list.size() != 0) {
					recvXml = infoGet(dataTrans, list);
					if(recvXml == null) {
						break;
					}
				}
				// 取得XML解析とワーキングメモリにinsert
				synchronized(parent.getLockObjectFire())
				{	
					insertWorkingMemory(recvXml);
				}
				try {
					sleep(pollingTime);
				} catch (InterruptedException e) {
					break;
				}
			}
			dataTrans.disconnect();
		}
	}

	protected DataTrans connectLoop(String _ip, int _portNo) {
		DataTrans connectDataTrans = new DataTrans(_ip, _portNo);
		while(true) {
			if(connectDataTrans.connect() == true) {
				System.out.println("ip[" + _ip + "] port[" + _portNo + "] 接続確立");
				break;
			}
			else {
				try {
					sleep(1000);
				} catch (InterruptedException e) {
					return null;
				}
			}
		}
		return connectDataTrans;
	}

	abstract protected ArrayList<Integer> createInfoGetList();

	// スレッド中断
	public void halt() {
		halt_ = true;
		interrupt();
		System.out.println("halt : disconnect");
	}

	protected ArrayList<Integer> parseNoticeXml(String _xml) {
		Document doc;
		int infoNum = 0;
		ArrayList<Integer> list = new ArrayList<Integer>();
		try {
			doc = DocumentBuilderFactory
					.newInstance()
					.newDocumentBuilder()
					.parse(new ByteArrayInputStream(_xml.getBytes("UTF-8")));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return null;
		} catch (SAXException e) {
			e.printStackTrace();
			return null;
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
			return null;
		}

		// 中身が空
		if(!doc.hasChildNodes()) {
			System.out.println("XMLデータが空です");
			return null;
		}

		Node node = doc.getFirstChild();
		if(!node.getNodeName().equals("Command")) {
			System.out.println("不正なXML ノード名：" + node.getNodeName());
			return null;
		}

		NodeList childNodeList = node.getChildNodes();
		if(childNodeList == null) {
			System.out.println("子ノードがありません");
			return null;			
		}

		for(Node childNode = childNodeList.item(0); childNode != null; childNode = childNode.getNextSibling()) {

			if(childNode.getNodeName().equals("MessageType")) {
				if(Integer.parseInt(childNode.getTextContent()) != 1) {
					System.out.println("MessageType不正：" + Integer.parseInt(childNode.getTextContent()));
					return null;
				}
			}
			if(childNode.getNodeName().equals("InfoNum")) {
				infoNum = Integer.parseUnsignedInt(childNode.getTextContent());				
				if( infoNum == 0) {
					System.out.println("InfoNum不正：" + infoNum);
					return null;
				}
			}
			if(childNode.getNodeName().equals("InfoIds")) {
				if(infoNum == 0) {
					System.out.println("InfoNum未設定：");
				}
				NodeList infoIdNodeList = childNode.getChildNodes();
				if(infoIdNodeList == null) {
					System.out.println("InfoId無し");
					return null;			
				}
				for(Node infoIdNode = infoIdNodeList.item(0); infoIdNode != null; infoIdNode = infoIdNode.getNextSibling()) {
					if(infoIdNode.getNodeName().equals("InfoId")) {
//						System.out.println("通知情報ID：" + Integer.parseUnsignedInt(infoIdNode.getTextContent()));
						list.add(Integer.parseUnsignedInt(infoIdNode.getTextContent()));
					}
				}
				if(list.size() != infoNum) {
					System.out.println("InfoId数[" + list.size() + "]!=InfoNum[" + infoNum + "]");
					return null;						
				}
			}
		}
		return list;
	}

	protected String infoGet(DataTrans _dataTrans, ArrayList<Integer> _list) {
		String sendXml = createInfoGetXml(_list);
		if(sendXml == null) {
			return null;
		}

		if( _dataTrans.send(sendXml) == false) {
			return null;
		}
		return _dataTrans.recv();
	}
	
	protected String createInfoGetXml(ArrayList<Integer> _infoIdList) {
		// XML 生成
		Document doc;
		try {
			doc = DocumentBuilderFactory
					.newInstance()
					.newDocumentBuilder()
					.newDocument();
		} catch (DOMException e) {
			e.printStackTrace();
			return null;
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
			return null;
		}

		// 中身を詰める
		Element command = doc.createElement("Command");
		doc.appendChild(command);

		Element messageType = doc.createElement("MessageType");
		messageType.appendChild(doc.createTextNode("1"));
		command.appendChild(messageType);

		Element infoNum = doc.createElement("InfoNum");
		infoNum.appendChild(doc.createTextNode(String.valueOf(_infoIdList.size())));
		command.appendChild(infoNum);

		Element infoIds = doc.createElement("InfoIds");
		command.appendChild(infoIds);

		Iterator<Integer> itr = _infoIdList.iterator();
		while(itr.hasNext()) {
			Integer infoId = itr.next();
			Element infoIdElement = doc.createElement("InfoId");
			infoIds.appendChild(infoIdElement);
			infoIdElement.appendChild(doc.createTextNode(String.valueOf(infoId)));
		}

		// XML String 変換
		StringWriter sw = new StringWriter();
		TransformerFactory tfactory = TransformerFactory.newInstance(); 
		Transformer transformer;
		try {
			transformer = tfactory.newTransformer();
		} catch (TransformerConfigurationException e) {
			e.printStackTrace();
			return null;
		}
		transformer.setOutputProperty(OutputKeys.STANDALONE, "no");
		try {
			transformer.transform(new DOMSource(doc), new StreamResult(sw));
		} catch (TransformerException e) {
			e.printStackTrace();
			return null;
		}
		return sw.toString();
	}

	protected String createInfoResponseXml(ArrayList<Integer> _infoIdList) {
		// XML 生成
		Document doc;
		try {
			doc = DocumentBuilderFactory
					.newInstance()
					.newDocumentBuilder()
					.newDocument();
		} catch (DOMException e) {
			e.printStackTrace();
			return null;
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
			return null;
		}

		// 中身を詰める
		Element command = doc.createElement("Command");
		doc.appendChild(command);

		Element messageType = doc.createElement("MessageType");
		messageType.appendChild(doc.createTextNode("8"));
		command.appendChild(messageType);

		Element result = doc.createElement("Result");
		int invalidCount = 0;
		command.appendChild(result);

		Element datas = doc.createElement("Datas");
		command.appendChild(datas);

		Iterator<Integer> itr = _infoIdList.iterator();
		
		synchronized(parent.getLockObjectOutput()){
			while(itr.hasNext()) {
				Integer infoId = itr.next();
				Element cofnitionInfoData = doc.createElement("CognitionInfoData");
				datas.appendChild(cofnitionInfoData);
	
				int type = 0;
				int size = 0;
				int data = 0;

	       		EventOutput output = globalManager.getOutput();
				
				switch(infoId) {
				case 0x00000001 : // 認知共通：自車加速度
					type = 3;
					size = 4;
					data = (int) output.getAccel();
					break;
				case 0x0200000B : // 認知共通：脇見判定結果
					type = 3;
					size = 4;
					data = output.getInattentiveState();
					break;
				case 0x0200000C : // 認知共通：運転阻害脇見判定結果
					type = 3;
					size = 4;
					data = output.getInattentiveStateInDriving();
					break;
				case 0x03000001 : // 認知共通：相対速度
					type = 3;
					size = 4;
					data = (int) output.getSpeedRelative();
					break;
				case 0x03000002 : // 認知共通：TTC
					type = 3;
					size = 4;
					data = (int) output.getTTC();
					break;
				case 0x03000003 : // 認知共通：相対加速度
					type = 3;
					size = 4;
					data = (int) output.getSpeedRelative();
					break;
				case 0x03000004 : // 認知共通：先行車加速度
					type = 3;
					size = 4;
					data = (int) output.getSpeedAheadvehicle();
					break;
				case 0x13000002 : // 認知個別：余裕時間
					type = 3;
					size = 4;
					data = (int) output.getUsableTime();
					break;
				default :	// プロファイル情報
					if((infoId & 0xFF000000) != 0x32000000) {
						invalidCount++;
						continue;
					}
					int profileNo = ((infoId & 0x00FFFFFF) - 1) / 2 + 1;

					int profileSize = output.getProfile().size();
					if(profileNo > profileSize) {
						invalidCount++;
						continue;
					}
					
					int dataNo = (infoId & 0x00FFFFFF) % 2;
					switch(dataNo) {
					case 1 : // 特性
						type = 3;
						size = 4;
						data = output.getProfileCharacterristics(profileNo);
						break;
					case 0 : // 状態
						type = 3;
						size = 4;
						data = output.getProfileState(profileNo);
						break;
					}
				}

				Element idStr = doc.createElement("Id");
				cofnitionInfoData.appendChild(idStr);
				idStr.appendChild(doc.createTextNode(String.valueOf(infoId)));
	
				Element typeStr = doc.createElement("Type");
				cofnitionInfoData.appendChild(typeStr);
				typeStr.appendChild(doc.createTextNode(String.valueOf(type)));
	
				Element sizeStr = doc.createElement("Size");
				cofnitionInfoData.appendChild(sizeStr);
				sizeStr.appendChild(doc.createTextNode(String.valueOf(size)));
	
				// エンディアン変換(ビッグ -> リトル)
				byte[] buf = ByteBuffer.allocate(size).order(ByteOrder.LITTLE_ENDIAN).putInt(data).array();
				StringBuilder builder = new StringBuilder();
				for(byte tmp : buf) {
					builder.append(Integer.toHexString((tmp & 0xF0) >> 4));
					builder.append(Integer.toHexString(tmp & 0x0F));
				}
				
				Element dataStr = doc.createElement("Data");
				cofnitionInfoData.appendChild(dataStr);
				dataStr.appendChild(doc.createTextNode(new String(builder)));

			}
//			System.out.println("Id[" + infoId + "] Type[" + type + "] Size[" + size + "] Data[" + data + "]");
		}

		result.appendChild(doc.createTextNode(String.valueOf(_infoIdList.size()-invalidCount)));

		// XML String 変換
		StringWriter sw = new StringWriter();
		TransformerFactory tfactory = TransformerFactory.newInstance(); 
		Transformer transformer;
		try {
			transformer = tfactory.newTransformer();
		} catch (TransformerConfigurationException e) {
			e.printStackTrace();
			return null;
		}
		transformer.setOutputProperty(OutputKeys.STANDALONE, "no");
		try {
			transformer.transform(new DOMSource(doc), new StreamResult(sw));
		} catch (TransformerException e) {
			e.printStackTrace();
			return null;
		}
		return sw.toString();
	}
	
	protected void insertWorkingMemory(String _xml) {
//		System.out.println("insertWorkingMemory");
		Document doc;
		int result = 0;

		if(_xml == null) {
			return;
		}

		try {
			doc = DocumentBuilderFactory
					.newInstance()
					.newDocumentBuilder()
					.parse(new ByteArrayInputStream(_xml.getBytes("UTF-8")));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return;
		} catch (SAXException e) {
			e.printStackTrace();
			return;
		} catch (IOException e) {
			e.printStackTrace();
			return;
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
			return;
		}

		// 中身が空
		if(!doc.hasChildNodes()) {
			System.out.println("XMLデータが空です");
			return;
		}

		Node node = doc.getFirstChild();
		if(!node.getNodeName().equals("Command")) {
			System.out.println("不正なXML ノード名：" + node.getNodeName());
			return;
		}

		NodeList childNodeList = node.getChildNodes();
		if(childNodeList == null) {
			System.out.println("子ノードがありません");
			return;
		}

		for(Node childNode = childNodeList.item(0); childNode != null; childNode = childNode.getNextSibling()) {

			if(childNode.getNodeName().equals("MessageType")) {
				if(Integer.parseInt(childNode.getTextContent()) != 8) {
					System.out.println("MessageType不正：" + Integer.parseInt(childNode.getTextContent()));
					return;
				}
			}
			if(childNode.getNodeName().equals("Result")) {
				result = Integer.parseUnsignedInt(childNode.getTextContent());				
				if( result < 0) {
					System.out.println("Result不正：" + result);
					return;
				}
			}
			if(childNode.getNodeName().equals("Datas")) {
				if(result == 0) {
					System.out.println("Result未設定：");
				}
				NodeList cognitionInfoDataNodeList = childNode.getChildNodes();
				if(cognitionInfoDataNodeList == null) {
					System.out.println("cognitionInfoData無し");
					return;
				}
				for(Node cognitionInfoDataNode = cognitionInfoDataNodeList.item(0); cognitionInfoDataNode != null; cognitionInfoDataNode = cognitionInfoDataNode.getNextSibling()) {
					if(!cognitionInfoDataNode.getNodeName().equals("CognitionInfoData")) {
						continue;
					}
					NodeList dataList = cognitionInfoDataNode.getChildNodes();
					if(dataList == null) {
						System.out.println("cognitionInfoData内のデータ無し");						
						return;
					}
					int id = 0;
//					int type = 0;
					int size = 0;
					int data = 0;
					for(Node dataNode = dataList.item(0); dataNode != null; dataNode = dataNode.getNextSibling()) {
						switch(dataNode.getNodeName()) {
						case "Id" :
							id = Integer.parseUnsignedInt(dataNode.getTextContent());
							break;
//						case "Type" :
//							type = Integer.parseUnsignedInt(dataNode.getTextContent());							
//							break;
						case "Size" :
							size = Integer.parseUnsignedInt(dataNode.getTextContent());							
							break;
						case "Data" :
							data = Integer.parseUnsignedInt(dataNode.getTextContent(),16);
							break;
						}
					}

					// dataをリトルエンディアンからビッグエンディアンに変換
//					System.out.print("Id[" + id + "] Type["+ type + "] Size[" + size + "] Data[" + data + "]");
					ByteBuffer buffer = ByteBuffer.allocate(size);
					buffer.putInt(data);
					byte[] littleEndianByte = buffer.array();
					ByteBuffer bigEndianBuf = ByteBuffer.wrap(littleEndianByte);
					bigEndianBuf.order(ByteOrder.LITTLE_ENDIAN);
					data = bigEndianBuf.getInt();
//					System.out.println("->[" + data + "]");

					UtilityBase tmpClass=null;
					switch(id) {
					case 0x20000001 : // 知覚情報：車速
//						System.out.println("知覚情報：車速Fact登録  data[" + data + "]");
						tmpClass = new SpeedUtility(data, session);
						globalManager.getOutput().setSpeed(data);
						break;
					case 0x20000002 : // 知覚情報：舵角
//						System.out.println("知覚情報：舵角Fact登録  data[" + data + "]");
//						tmpClass = new RudderAngleUtility(data, session);
						break;
					case 0x20000003 : // 知覚情報：アクセル開度
//						System.out.println("知覚情報：アクセル開度Fact登録 data[" + data + "]");
						tmpClass = new AccelOpenUtility(data, session);
						globalManager.getOutput().setAccelOpen(data);
						break;
					case 0x20000004 : // 知覚情報：ブレーキ開度
//						System.out.println("知覚情報：ブレーキ開度Fact登録 data[" + data + "]");
//						tmpClass = new BrakeOpenUtility(data, session);
						break;
					case 0x20000005 : // 知覚情報：シフト状態(Parking)
//						System.out.println("知覚情報：シフト状態(Parking)Fact登録 data[" + data + "]");
//						tmpClass = new ShiftStateParkingUtility(data, session);
						break;
					case 0x20000006 : // 知覚情報：シフト状態(Reverse)
//						System.out.println("知覚情報：シフト状態(Reverse)Fact登録 data[" + data + "]");
						tmpClass = new ShiftStateReverseUtility(data, session);
						globalManager.getOutput().setShiftStateReverse(data);
						break;
					case 0x20000007 : // 知覚情報：パーキングブレーキ状態
//						System.out.println("知覚情報：パーキングブレーキ状態Fact登録 data[" + data + "]");
//						tmpClass = new ParkingBreakStateUtility(data, session);
						break;
					case 0x20000008 : // 知覚情報：ストップランプSW(未使用)
//						System.out.println("知覚情報：ストップランプWGFact登録 data[" + data + "]");
//						tmpClass = new StopLampSWUtility(data, session);
						break;
					case 0x23000001 : // 知覚情報：車間距離
//						System.out.println("知覚情報：車間距離Fact登録 data[" + data + "]");
						tmpClass = new DistanceUtility(data, session);
						globalManager.getOutput().setDistance(data);
						break;
					case 0x22000001 : // 知覚情報：顔向き
//						System.out.println("知覚情報：顔向きFact登録 data[" + data + "]");
						tmpClass = new DriverFaceDirectionUtility(data, session);
						globalManager.getOutput().setFaceAngle(data);
						break;
					case 0x22000002 : // 知覚情報：顔位置(中心のX座標)
//						System.out.println("知覚情報：顔位置(中心のX座標)Fact登録 data[" + data + "]");
//						tmpClass = new DriverFacePositionXUtility(data, session);
						break;
					case 0x22000003 : // 知覚情報：顔位置(中心のY座標)
//						System.out.println("知覚情報：顔位置(中心のY座標)Fact登録 data[" + data + "]");
//						tmpClass = new DriverFacePositionYUtility(data, session);
						break;
					case 0x22000004 : // 知覚情報：顔位置(幅)
//						System.out.println("知覚情報：顔位置(幅)Fact登録 data[" + data + "]");
//						tmpClass = new DriverFaceWidthUtility(data, session);
						break;
					case 0x22000005 : // 知覚情報：顔位置(高さ)
//						System.out.println("知覚情報：顔位置(高さ)Fact登録 data[" + data + "]");
//						tmpClass = new DriverFaceHeightUtility(data, session);
						break;
					case 0x00000001 : // 認知共通：自車加速度
					case 0x00000002 : // 認知共通：運転シーンの状態推定
					case 0x00000003 : // 認知共通：自車加速度(1s)
					case 0x00000004 : // 認知共通：自車加速度(0s)
						break;
					case 0x02000001 : // 認知共通：漫然度レベル(std)
//						System.out.println("認知共通：漫然度レベル(std)Fact登録 data[" + data + "]");
						tmpClass = new AimlessLevelUtility(data, session);
						globalManager.getOutput().setAimlessLevelStd(data);
						break;
					case 0x02000002 : // 認知共通：漫然度レベル(low)
					case 0x02000003 : // 認知共通：漫然度レベル(high)
					case 0x02000004 : // 認知共通：漫然度フラグ(std)
					case 0x02000005 : // 認知共通：漫然度フラグ(low)
					case 0x02000006 : // 認知共通：漫然度フラグ(high)
					case 0x02000007 : // 認知共通：正規化漫然ポイント(std)
					case 0x02000008 : // 認知共通：正規化漫然ポイント(low)
					case 0x02000009 : // 認知共通：正規化漫然ポイント(high)
						break;
					case 0x0200000A : // 認知共通：負荷レベル
//						System.out.println("認知共通：負荷レベルFact登録 data[" + data + "]");
						tmpClass = new LoadLevelUtility(data, session);
						globalManager.getOutput().setLoadLevel(data);
						break;
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
						break;
					}
					if(tmpClass != null)
					{
						tmpClass.run();
					}
				}
			}
		}
		return;
	}
}
