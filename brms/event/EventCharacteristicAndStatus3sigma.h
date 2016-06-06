#ifndef _H_EVENT_CHARACTERISTIC_AND_STATUS_3SIGMA_
#define _H_EVENT_CHARACTERISTIC_AND_STATUS_3SIGMA_

#include <string>
#include <queue>

using namespace std;

class EventCharacteristicAndStatus3sigma {
	
public:
	 int MAX_SAMPLE_SIZE;
	

	 enum PROFILE_TYPE_THREESIGMA
	{
		TTC_OF_SPEED_DECELERATION,
		DURATION_OF_SHORTTTC_CHECK,
        STOPPED_DISTANCE,
//liuxin add start 2016.03.17
		STRAIGHT_RUN_SHAKE,
		FRONT_CAR_FOLLOW_START_TIMING,
//liuxin add end 2016.03.17
        PROFILE_TYPE_THREESIGMA_MAX
	};
		
	 enum SPECIFIC_THREESIGMA
	{
		NAN0,
		RELAX,   // 余裕をもって減速開始するタイプ
		NORELAX, // 余裕をもたず減速開始するタイプ

		FEAR_TTCSHORT,     // TTC短時間を嫌うタイプ
		FEARLESS_TTCSHORT, // TTC短時間を恐れないタイプ

		AVERAGE_SMALL,  // 余裕度 小
        AVERAGE_LARGE,  // 余裕度 大
//liuxin add start 2016.03.17
		SHAKE_SMALL,
		SHAKE_LARGE,
//liuxin add end 2016.03.17
        SPECIFIC_THREESIGMA_MAX
	};
	
     static const char* m_spec3sigmaNameList[SPECIFIC_THREESIGMA_MAX];
     static const char* m_profileType3sigma[PROFILE_TYPE_THREESIGMA_MAX];
	 PROFILE_TYPE_THREESIGMA type;
	
	 queue<double> samplingList;
	 SPECIFIC_THREESIGMA specific;
	 double average;
	 double stdDeviation;
	 bool userStateCheck;
	 string userState;
	 int outlierValCount;
	
	// 前日までのユーザ特性（状態の判定に使用）
	// 前日平均
	 double oldSampleAverage;
	// 前日標準偏差
	 double oldSampleStdDeviation;

     EventCharacteristicAndStatus3sigma()
		 : MAX_SAMPLE_SIZE(50)
		 , specific(NAN0)
		 , average(0)
		 , stdDeviation(0)
		 , userStateCheck(false)
		 , userState("状態未判定")
		 , outlierValCount(0)
		 , oldSampleAverage(0)
		 , oldSampleStdDeviation(0)
    {
    }

     static const char* getSpec3sigmaName(SPECIFIC_THREESIGMA status)
     {
         if ( status < SPECIFIC_THREESIGMA_MAX)
         {
             return m_spec3sigmaNameList[status];
         }
         else
             return NULL;
     }

     static const char* getSpec3sigmaTypeName(PROFILE_TYPE_THREESIGMA status)
     {
         if ( status < PROFILE_TYPE_THREESIGMA_MAX)
         {
             return m_profileType3sigma[status];
         }
         else
             return NULL;
     }
     void addSample(double value)
    {
    	//DEQUEサイズを超える場合、先頭を削除
    	//if(samplingList.size() >= MAX_SAMPLE_SIZE)
    	//{
    	//	samplingList.remove();
    	//}
    	samplingList.push(value);
    }
     void resetSample()
     {
 #ifndef _WINDOWS_PLAT_
         queue<double> tempList;
         samplingList.swap(tempList);
 #else
         int size = samplingList.size();
         for(int i=0; i<size; i++)
         {
             samplingList.pop();
         }
 #endif
         //samplingList.clear();
     }
     queue<double> getSamplingList()
   	{
   		return samplingList;
   	}
    // 平均μ
     double getAverage()
	{
		return average;
	}
	 void setAverage(double avera)
	{
		average = avera;
	}
	// 標準偏差σ
	 double getStdDeviation()
	{
		return stdDeviation;
	}
	 void setStdDeviation(double stdDev)
	{
		stdDeviation = stdDev;
	}
    
	 int getSamplingCount()
	{
		return samplingList.size();
	}	 
     string getSpecific()
    {
    	string txt="未定義";

		// プロファイル3
    	if(specific == RELAX)
    	{
    		txt = "余裕をもって減速開始するタイプ";
    	}
    	else if(specific == NORELAX)
    	{
    		txt = "余裕をもたず減速開始するタイプ";
    	}

		// プロファイル4
    	else if(specific == FEAR_TTCSHORT)
    	{
    		txt = "TTC短時間を嫌うタイプ";
    	}
    	else if(specific == FEARLESS_TTCSHORT)
    	{
    		txt = "TTC短時間を恐れないタイプ";
    	}

		// プロファイル7
    	else if(specific == AVERAGE_SMALL)
    	{
    		txt = "余裕度　小";
    	}
    	else if(specific == AVERAGE_LARGE)
    	{
    		txt = "余裕度　大";
    	}
    	return txt;
    }
     void setSpecific(SPECIFIC_THREESIGMA spec)
    {
    	specific = spec;
    }
    
    // はずれ値合計数
 	 int getOutlierValCount()
 	{
 		return outlierValCount;
 	}
 	 void setOutlierValCount(int count)
 	{
 		outlierValCount = count;
 	}   
	 bool getUserStateCheck()
	{
		return userStateCheck;
	}
	 void setUserStateCheck(bool check)
	{
		userStateCheck = check;
	}
     string getUserState()
    {
    	return userState;
    }
     void setUserState(bool state)
    {
    	if(state)
    	{
    		userState = "いつもと同じ";
    	}
    	else
    	{
    		userState = "いつもと違う";
    	}
    }
	 queue<double>  getAllSample() {

		queue<double> copyQueue;
#if 0
    	int i = 0;
    	if(samplingList.size()>MAX_SAMPLE_SIZE)
    	{
    		i = samplingList.size() - MAX_SAMPLE_SIZE;
    	}
    	for(; i < samplingList.size(); i++)
    	{
    		copyQueue.push((double)samplingList[i]);
    	}
#endif
    	return samplingList;
	}
    // 前日までのユーザ特性を保存
     void setOldSampleAverage(double value)
    {
    	oldSampleAverage = value;
    }
     void setOldSampleStdDeviation(double value)
    {
    	oldSampleStdDeviation = value;
    }    
    // 前日までのユーザ特性を取得
     double getOldSampleAverage()
    {
    	return oldSampleAverage;
    }
     double getOldSampleStdDeviation()
    {
    	return oldSampleStdDeviation;
    }

     PROFILE_TYPE_THREESIGMA getType()
    {
    	return type;
    }
     void setType(PROFILE_TYPE_THREESIGMA value)
    {
    	type = value;
    }
	
	 int getMaxSampleSize()
	{
		return MAX_SAMPLE_SIZE;
	}
	 void setMaxSampleSize(int value)
	{
		MAX_SAMPLE_SIZE = value;
	}

    
};
#endif
