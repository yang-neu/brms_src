#include <math.h>
#include "CANDataParse.h"
#include "../event/CANTransmitData.h"
#include "../utility/AccelOpenUtility.h"
#include "../utility/DistanceUtility.h"
#include "../utility/RoadClassUtility.h"
#include "../utility/SpeedUtility.h"
#include "../utility/SteeringAngleCANUtility.h"
#include "../utility/ShiftStateCANUtility.h"

void CANDataParse::initialize()
{
	
}

int CANDataParse::run()
{  //(2)スレッド実行コードをrunメソッドに記載
		
	// CANデータ項目情報テーブルに基づいて解析			
	list<CANParseDefinition> dataInfo;
	// CAN IDから対象のCANデータ項目情報テーブルのリストを取得する。
	dataInfo = m_canTransData->find(m_canId);
	// CANデータをデータ項目情報テーブルに従って分解
	return parseDataInfo(dataInfo);
       
}
	
	// CANのデータ部をDataInfoリストにしたがって解読する
int CANDataParse::parseDataInfo(list<CANParseDefinition> &dataInfo)
{
	bool ret = 0;
	list<CANParseDefinition>::iterator itr;
	itr = dataInfo.begin();

	while(itr != dataInfo.end())
	{
		long long value=0;
		// DataInfoリストで指定されたバイト/ビット位置から値を取得する
		value = getValue(*itr);
		
		// データ名称から生成するfactモデルとinsert対象のEntryPointを特定する
		UtilityBase *tmpClass = NULL;
		if(paseList.find((*itr).getDataName()) != paseList.end())
		{
				tmpClass = paseList[(*itr).getDataName()];
				if (tmpClass != NULL)
					tmpClass->updateValue(value);
		}
		else
		{
			if ((*itr).getDataName() == "車速(SP1)")
				tmpClass = new SpeedUtility(value, m_sessionID);
			else if((*itr).getDataName()== "アクセル開度率")
				tmpClass = new AccelOpenUtility(value, m_sessionID);
			else if((*itr).getDataName() == "車間距離")
				tmpClass = new DistanceUtility(value, m_sessionID);
			//else if ((*itr).getDataName() == "道路種別判定信号")
			//	tmpClass = new RoadClassUtility(value, m_sessionID);
			else if((*itr).getDataName() == "舵角")
				tmpClass = new SteeringAngleCANUtility(value, m_sessionID);
			else if((*itr).getDataName() == "シフト状態")
			{
				tmpClass = new ShiftStateCANUtility(value, m_sessionID);
			}
			else
			{
			}
			if (tmpClass != NULL)
				paseList.insert(pair<string,  UtilityBase *>((*itr).getDataName(), tmpClass));
		}
		if(tmpClass != 0)
		{
			tmpClass->run();
			ret++;
		}
		itr++;
	}
	return ret;
}
long long CANDataParse::getValue(CANParseDefinition &data)
{
	long long tmpVal=0;
	char bitMask = 0x01;
	long long sumData = 0x00;
	long long maskData = 0x00;
	int dataMask = 0x00ff;
	
	int byteIndex = (int)(floor(data.getDataPosByte()));
	//int bitIndex = (int)(data.getDataPosByte() % 1.0);
	//int bitIndex = (int)fmod(data.getDataPosByte(), 1.0);
	int bitIndex = (int)(data.getDataPosByte() * 10) % 10;
	int checkByteLength = (int)ceil(((double)data.getDataLength()+(double)bitIndex)/(double)8);
	
	// 後ろのバイトから参照しながらデータを吸い上げる
	for(int i=0; i<checkByteLength;i++ )
	{
		sumData += ((dataMask & msg[byteIndex-1-i]) * (i !=0 ?(i*0x100):1 ));
	}
	// ビット位置とデータ長からマスクビットを作成する
	for(int i=0; i<data.getDataLength();i++)
	{
		maskData += (bitMask << (i+bitIndex));
	}
	
	tmpVal = sumData & maskData;
	//System.out.println("sumData : " + Integer.toHexString((int)sumData) + " maskData : "  + Integer.toHexString((int)maskData));
	//System.out.println(data.getDataName() + " : 0x" + Integer.toHexString((int)tmpVal));
	
	return tmpVal;
}
	
