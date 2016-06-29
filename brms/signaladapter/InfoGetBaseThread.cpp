
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "InfoGetBaseThread.h"
#include "../thirdpart/rapidxml.hpp"
#include "../thirdpart/rapidxml_utils.hpp"
#include "../thirdpart/rapidxml_print.hpp"
#include "../streaming/EntryPointOutput.h"
#include "../ClipsSessionMgr.h"
#include "../common/StringUtil.h"

using namespace rapidxml;

void* InfoGetBaseThread::startProc(void *p)
{
	InfoGetBaseThread *pIGBT = (InfoGetBaseThread *)p;
	
	if(pIGBT != NULL)
		pIGBT->run();

    return 0;
}
void InfoGetBaseThread::start()
{
		//m_fw.open("../com.log",ios::out);
		pthread_create(&m_ThdId, 0, startProc, this);	
}
	// 実行メソッド
void InfoGetBaseThread::run() 
{
		// 通信無限ループ部
		while (!halt_) {

			// 情報生成タスク取得ポートとの接続(1秒間隔で無限リトライ)
			dataTrans = connectLoop(serverIp, serverPortNo);
			if(dataTrans == NULL) {
				return;
			}

            list<int> _list ;
            createInfoGetList(_list);
			string recvXml;;
			bool ret;
			while(!halt_) {
				// 情報取得
                if(_list.size() != 0) {
                    ret = infoGet(dataTrans, _list, recvXml);
					if(ret == false) {
						break;
					}
				}
				// 取得XML解析とワーキングメモリにinsert
			
				insertWorkingMemory(recvXml);
		
			
				usleep(pollingTime*1000);
				
			}
            dataTrans->disconnect();
		}
	}

DataTrans* InfoGetBaseThread::connectLoop(string _ip, int _portNo) 
{
		DataTrans *connectDataTrans = new DataTrans(_ip, _portNo);
		while(true) {
            if(connectDataTrans->connectInter() == true) {
				cout<<"ip["<<_ip<<"] port["<<_portNo<<"] connect ok!"<<endl;
				break;
			}
			else {
				usleep(1000000);
			}
		}
		return connectDataTrans;
	}

	// スレッド中断
void InfoGetBaseThread::halt() 
{
		halt_ = true;
	//	interrupt();
		cout<<"halt : disconnect"<<endl;
}
bool InfoGetBaseThread::parseNoticeXml(string xml, list<int> &resultList)
{
    rapidxml::xml_document<> doc;
    //int size1 = xml.size();
    //int len1 = xml.length();

    //string _xml = xml + "";
    string _xml = xml;
    char* str = (char*)_xml.c_str();

    try {
		doc.parse<0>(str);
    }
    catch(rapidxml::parse_error *err)
    {
        cout<< "parse error"<<err->what()<<endl;
    }

   // xml_node<>* root = doc.first_node();
   // std::cout<<root->name()<<std::endl;


    int infoNum = 0;
    //list<int> list1;

    rapidxml::xml_node<char> * firstNode = doc.first_node("Command");
    if (firstNode == NULL)
    {
        cout<<"不正なXML ノード名：command"<<endl;
        return false;
    }

    for(rapidxml::xml_node<char> * node = firstNode->first_node();
        node != NULL;node = node->next_sibling())
    {
        string name = node->name();
        if(name == "MessageType" )
        {
            if (atoi(node->value()) != 1)
            {
                cout<<"MessageType不正："<<atoi(node->value())<<endl;
                return false;
            }
        }
		else if (name == "InfoNum")
        {
            infoNum = atoi(node->value());
            if ( infoNum== 0)
            {
                cout<<"InfoNum不正："<<infoNum<<endl;
                return false;
            }
        }
        else if (name == "InfoIds")
        {
            for(rapidxml::xml_node<char> * node1 = node->first_node();
                node1 != NULL;node1 = node1->next_sibling())
            {
                    string name = node1->name();
                    if (name == "InfoId")
                    {
                        resultList.push_back(atoi(node1->value()));
                    }
            }
			if (resultList.size() != (size_t)infoNum)
            {
                cout<<"InfoId数[" <<resultList.size() <<"]!=InfoNum[" << infoNum <<"]"<<endl;
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool InfoGetBaseThread::parseNoticeXml(char* xml,int size, list<int> &resultList)
{
    char tempData[4*1024];
    memset(tempData, 0, 4*1024);

    memcpy(tempData,xml,size);
    rapidxml::xml_document<> doc;
    try {
    doc.parse<0>(tempData);
    }
    catch(rapidxml::parse_error *err)
    {
        cout<< "parse error"<<err->what()<<endl;
    }

    //xml_node<>* root = doc.first_node();
    //std::cout<<root->name()<<std::endl;



	int infoNum = 0;
	//list<int> list1;
	
    rapidxml::xml_node<char> * firstNode = doc.first_node("Command");
    if (firstNode == NULL)
	{
		cout<<"不正なXML ノード名：command"<<endl;
		return false;
	}

	for(rapidxml::xml_node<char> * node = firstNode->first_node();
		node != NULL;node = node->next_sibling())
	{
		string name = node->name();
		if(name == "MessageType" )
		{
			if (atoi(node->value()) != 1)
			{
				cout<<"MessageType不正："<<atoi(node->value())<<endl;
				return false;
			}
		}
		else if (name == "InfoNum")
		{
			infoNum = atoi(node->value());
			if ( infoNum== 0)
			{
				cout<<"InfoNum不正："<<infoNum<<endl;
				return false;
			}
		}
		else if (name == "InfoIds")
		{
			for(rapidxml::xml_node<char> * node1 = node->first_node();
				node1 != NULL;node1 = node1->next_sibling())
			{
					string name = node1->name();
					if (name == "InfoId")
					{
                        resultList.push_back(atoi(node1->value()));
					}
			}
			if (resultList.size() != (size_t)infoNum)
			{
                cout<<"InfoId数[" <<resultList.size() <<"]!=InfoNum[" << infoNum <<"]"<<endl;
				return false;
			}
		}
		else 
		{
			return false;
		}
	}
	return true;	
}

bool InfoGetBaseThread::infoGet(DataTrans *_dataTrans, list<int> _list, string &result) 
{
	string sendXml;
	bool ret = createInfoGetXml(_list, sendXml);
	
	if(ret == false) {
		return false;
	}

    if( _dataTrans->sendInter(sendXml) == false) {
		return false;
	}
    ret = _dataTrans->recvInter(result);
    if(ret == false) {
        return false;
    }
	return true;
}
bool InfoGetBaseThread::createInfoGetXml(list<int> _infoIdList, string &result) 
{
	rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    
    doc.append_node(rot);
    
    rapidxml::xml_node<>* nodeCmd = doc.allocate_node(rapidxml::node_element,"Command",NULL);
	doc.append_node(nodeCmd);
	
	rapidxml::xml_node<>* nodeMT = doc.allocate_node(rapidxml::node_element,"MessageType","1");
	nodeCmd->append_node(nodeMT);
	
    char strSize[10];
    memset(strSize, 0, 10);
    //itoa(_infoIdList.size(), strSize, 10);
    snprintf(strSize, 10, "%d", _infoIdList.size());
    rapidxml::xml_node<>* nodeIN = doc.allocate_node(rapidxml::node_element,"InfoNum",strSize);
    nodeCmd->append_node(nodeIN);
    
    rapidxml::xml_node<>* nodeIFID = doc.allocate_node(rapidxml::node_element,"InfoIds",NULL);
	nodeCmd->append_node(nodeIFID);
	
	char *strInfo = new char[12 * _infoIdList.size()];
	char *p = strInfo;
	list<int>::iterator itr = _infoIdList.begin();
	while(itr != _infoIdList.end())
	{
        //itoa(*itr, p, 10);
        snprintf(p, 10, "%d", *itr);
		rapidxml::xml_node<>* nodeID = doc.allocate_node(rapidxml::node_element,"InfoId", p);
		nodeIFID->append_node(nodeID);
		itr++;
		p += 12;
	}	
    rapidxml::print(std::back_inserter(result),doc,0);
	delete[] strInfo;

	return true;
}

bool InfoGetBaseThread:: createInfoResponseXml(list<int> _infoIdList, string &result)
{
    int invalidCount = 0;
	rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    doc.append_node(rot);
    
    rapidxml::xml_node<>* nodeCmd = doc.allocate_node(rapidxml::node_element,"Command",NULL);
	doc.append_node(nodeCmd);
	
	rapidxml::xml_node<>* nodeMT = doc.allocate_node(rapidxml::node_element,"MessageType","8");
	nodeCmd->append_node(nodeMT);
	
	
	
	rapidxml::xml_node<>* nodeDatas= doc.allocate_node(rapidxml::node_element,"Datas", NULL);
	nodeCmd->append_node(nodeDatas);
	
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	//EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(session)->getEntryPoint("Data Output Stream"));
    
	list<int>::iterator itr = _infoIdList.begin();
	while(itr != _infoIdList.end())
	{
		rapidxml::xml_node<>* nodeCID = doc.allocate_node(rapidxml::node_element,"CognitionInfoData", NULL);
        nodeDatas->append_node(nodeCID);
		int infoId = *itr;
		int type = 0;
		int size = 0;
		int data = 0;
        //char type[]
        char strType[10];
        char strInfo[10];
        char strSize[10];

		switch(infoId) {
			case 0x00000001 : // 認知共通：自車加速度
				type = 3;
				size = 4;
                //data = (int) pStream.getAccel();
				break;
			case 0x0200000B : // 認知共通：脇見判定結果
				type = 3;
				size = 4;
                //data = pStream.getInattentiveState();
				break;
			case 0x0200000C : // 認知共通：運転阻害脇見判定結果
				type = 3;
				size = 4;
                //data = pStream.getInattentiveStateInDriving();
				break;
			case 0x03000001 : // 認知共通：相対速度
				type = 3;
				size = 4;
                //data = (int) pStream.getSpeedRelative();
				break;
			case 0x03000002 : // 認知共通：TTC
				type = 3;
				size = 4;
                //data = (int) pStream.getTTC();
				break;
			case 0x03000003 : // 認知共通：相対加速度
				type = 3;
				size = 4;
                //data = (int) pStream.getSpeedRelative();
				break;
			case 0x03000004 : // 認知共通：先行車加速度
				type = 3;
				size = 4;
                //data = (int) pStream.getSpeedAheadvehicle();
				break;
			case 0x13000002 : // 認知個別：余裕時間
				type = 3;
				size = 4;
                //data = (int) pStream.getUsableTime();
				break;
			default :	// プロファイル情報
				if((infoId & 0xFF000000) != 0x32000000) {
					invalidCount++;
					continue;
				}
				int profileNo = ((infoId & 0x00FFFFFF) - 1) / 2 + 1;

                int profileSize = 0;//pStream.getProfile().size();
				if(profileNo > profileSize) {
					invalidCount++;
					continue;
				}
				
				int dataNo = (infoId & 0x00FFFFFF) % 2;
				switch(dataNo) {
				case 1 : // 特性
					type = 3;
					size = 4;
                    //data = pStream.getProfileCharacterristics(profileNo);
					break;
				case 0 : // 状態
					type = 3;
					size = 4;
                    //data = pStream.getProfileState(profileNo);
					break;
				}
			}
        memset(strInfo, 0, 10);
        //itoa(infoId,strInfo, 10);
        snprintf(strInfo, 10, "%d", infoId);
        rapidxml::xml_node<>* nodeId = doc.allocate_node(rapidxml::node_element,"Id", strInfo);
		nodeCID->append_node(nodeId);
		
        memset(strType, 0, 10);
        //itoa(type, strType, 10);
        snprintf(strType, 10, "%d", type);
        rapidxml::xml_node<>* nodeType = doc.allocate_node(rapidxml::node_element,"Type", strType);
		nodeCID->append_node(nodeType);
		
        memset(strSize, 0, 10);
        //itoa(size, strSize, 10);
        snprintf(strSize, 10, "%d", size);
        rapidxml::xml_node<>* nodeSize = doc.allocate_node(rapidxml::node_element,"Size", strSize);
		nodeCID->append_node(nodeSize);
		
		// エンディアン変換(ビッグ -> リトル)
        string strData = itoh((data & 0x000000F0) >>4 ) + itoh(data & 0x0000000F)
                        + itoh((data & 0x0000F000) >>12 ) +  itoh((data & 0x00000F00) >>8)
                        + itoh((data & 0x00F00000) >>20 ) +  itoh((data & 0x000F0000) >>16)
                        + itoh((data & 0xF0000000) >>28 ) +  itoh((data & 0x0F000000) >>24);

        rapidxml::xml_node<>* nodeData = doc.allocate_node(rapidxml::node_element,"Data", strData.c_str());
		nodeCID->append_node(nodeData);
		itr++;
	}	
	
    char strNum[10];
    memset(strNum, 0, 10);
    //itoa(_infoIdList.size()-invalidCount, strNum, 10);
    snprintf(strNum, 10, "%d", _infoIdList.size());
    rapidxml::xml_node<>* nodeRslt= doc.allocate_node(rapidxml::node_element,"Result", strNum);
	nodeCmd->append_node(nodeRslt);			

    rapidxml::print(std::back_inserter(result),doc,0);
	
	return true;
}
	
void InfoGetBaseThread::insertWorkingMemory(string _xml) 
{
	xml_document<>   doc;
    char *str = (char*)_xml.c_str();
    doc.parse<0>(str);

    rapidxml::xml_node<char> * firstNode = doc.first_node("Command");
    if (firstNode == NULL)
	{
		cout<<"不正なXML ノード名：command"<<endl;
		return ;
	}

	for(rapidxml::xml_node<char> * node = firstNode->first_node();
		node != NULL;node = node->next_sibling())
	{
		string name = node->name();
		if(name == "MessageType" )
		{
			if (atoi(node->value()) != 8)
			{
				cout<<"MessageType不正："<<atoi(node->value())<<endl;
				return ;
			}
		}
		else if(name == "Result" )
		{
			int result = atoi(node->value());
			if ( result < 0)
			{
				cout<<"Result不正：" << result<<endl;
			}
		}
		
		else if(name == "Datas" )
		{
			for(rapidxml::xml_node<char> * node1 = node->first_node();
				node1 != NULL;node1 = node1->next_sibling())
			{
					string name1 = node1->name();
					if (name1 != "CognitionInfoData")
					{
						continue;
					}
					int id = 0;
					int size = 0;
					int data = 0;
                    int data1 = 0;
                    int data2 = 0;
                    int data3 = 0;
                    int data4 = 0;
					for(rapidxml::xml_node<char> * node2 = node1->first_node();
						node2 != NULL;node2 = node2->next_sibling())
					{
						string name2 = node2->name();
						
						if(name2 == "Id")
						{
							id = atoi(node2->value());
						}
						else if(name2 == "Size")
						{
							size = atoi(node2->value());
						}
						else if(name2 == "Data")
						{
							data = htoi(node2->value());
						}
						else if(name2 == "Type")
						{
							// Data Type
						}
						else
						{
							cout<<"unknown field:"<<name2<<endl;
						}
					}
					// dataをリトルエンディアンからビッグエンディアンに変換
                    data1 = ((data & 0xFF000000) >>24 ) ;
                    data2 = ((data & 0x00FF0000) >>8) ;
                    data3 = ((data & 0x0000FF00) << 8) ;
                    data4 = ((data & 0x000000FF) <<24);
                    data = (data1 | data2 | data3 | data4);
					
					UtilityBase *tmpClass=NULL;
					
                    if(parseList.find(id) != parseList.end())
					{
							tmpClass = parseList[id];
							if (tmpClass != NULL)
								tmpClass->updateValue(data);
					}
					else
					{
						switch(id) {
						case 0x20000001 : // 知覚情報：車速
	//						System.out.println("知覚情報：車速Fact登録  data[" + data + "]");
							tmpClass = new SpeedUtility(data, session);
							//globalManager.getOutput().setSpeed(data);
							break;
						case 0x20000002 : // 知覚情報：舵角
	//						System.out.println("知覚情報：舵角Fact登録  data[" + data + "]");
                            tmpClass = new SteeringAngleUtility(data, session);
							break;
						case 0x20000003 : // 知覚情報：アクセル開度
	//						System.out.println("知覚情報：アクセル開度Fact登録 data[" + data + "]");
							tmpClass = new AccelOpenUtility(data, session);
							//globalManager.getOutput().setAccelOpen(data);
							break;
						case 0x20000004 : // 知覚情報：ブレーキ開度
	//						System.out.println("知覚情報：ブレーキ開度Fact登録 data[" + data + "]");
                            tmpClass = new BrakePressureUtility(data, session);
							break;
						case 0x20000005 : // 知覚情報：シフト状態(Parking)
	//						System.out.println("知覚情報：シフト状態(Parking)Fact登録 data[" + data + "]");
							tmpClass = new ShiftStateParkingUtility(data, session);
							break;
						case 0x20000006 : // 知覚情報：シフト状態(Reverse)
	//						System.out.println("知覚情報：シフト状態(Reverse)Fact登録 data[" + data + "]");
							//globalManager.getOutput().setShiftStateReverse(data);
							tmpClass = new ShiftStateReverseUtility(data, session);
							break;
						case 0x20000007 : // 知覚情報：パーキングブレーキ状態
	//						System.out.println("知覚情報：パーキングブレーキ状態Fact登録 data[" + data + "]");
	//						tmpClass = new ParkingBreakStateUtility(data, session);
							break;
						case 0x20000008 : // 知覚情報：ストップランプSW(未使用)
	//						System.out.println("知覚情報：ストップランプWGFact登録 data[" + data + "]");
	//						tmpClass = new StopLampSWUtility(data, session);
							break;
						case 0x20000009 : // 知覚情報：シフト状態(Drive)
	//						System.out.println("知覚情報：シフト状態(Drive)Fact登録 data[" + data + "]");
							//globalManager.getOutput().setShiftStateReverse(data);
							tmpClass = new ShiftStateDriveUtility(data, session);
							break;
						case 0x23000001 : // 知覚情報：車間距離
	//						System.out.println("知覚情報：車間距離Fact登録 data[" + data + "]");
							tmpClass = new DistanceUtility(data, session);
                           // m_fw<<"=======begine once==========="<<endl;
                           // m_fw<<_xml<<endl;
                           // m_fw<<"=======end once==========="<<endl;
							//globalManager.getOutput().setDistance(data);
							break;
						case 0x22000001 : // 知覚情報：顔向き
	//						System.out.println("知覚情報：顔向きFact登録 data[" + data + "]");
							tmpClass = new DriverFaceDirectionUtility(data, session);
							//globalManager.getOutput().setFaceAngle(data);
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
							//globalManager.getOutput().setAimlessLevelStd(data);
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
							//globalManager.getOutput().setLoadLevel(data);
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
						
						if (tmpClass != NULL)
							parseList.insert(pair<int,  UtilityBase *>(id, tmpClass));
					
					}
                    if(tmpClass != NULL)
					{
                        tmpClass->run();
					}
			}
			
		}
	}
	
}
