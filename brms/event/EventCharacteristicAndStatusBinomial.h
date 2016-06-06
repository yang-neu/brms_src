#ifndef _H_EVENT_CHARACTERISTIC_AND_STATUS_BINOMIAL_
#define _H_EVENT_CHARACTERISTIC_AND_STATUS_BINOMIAL_

#include <string>
#include <queue>

using namespace std;

 class EventCharacteristicAndStatusBinomial
 {
public:
	 int MAX_SAMPLE_SIZE;
	 int STATE_CHECK_SIZE;
	 int DEQUE_SIZE;
	
	 enum PROFILE_TYPE_BINOMIAL
	{
		SPEED_ACCELERATION,
		SPEED_DECELERATION,
		RUNNING_DISTANCE_ONOPENROAD,
		RUNNING_DISTANCE_ONEXPRESSWAY,
//liuxin add start 2016.03.17
		ACCEL_TREAD_TIME,
		STOP_JERK_BRAKE,
		START_ACCELERATION,
		STEERINGANGLE_ACCELERATION,
		LEFR_TURN_SPEED,
		RIGHT_TURN_SPEED,
//liuxin add end 2016.03.17
        PROFILE_TYPE_BINOMIAL_MAX
	};
	
	 enum SPECIFIC_BINOMIAL
	{
		NAN0,
		
		// プロファイル1,2 
		SUDDEN,
		NORMAL,
		
		// プロファイル 5,6
		MARGIN_SMALL,  // 余裕度 小
		MARGIN_MEDIUM, // 余裕度 中
		MARGIN_LARGE,  // 余裕度 大
//liuxin add start 2016.03.17
		// プロファイル 10
		ACCEL_APPOSITE,     // 適切運転
		ACCEL_EXCESSIVELY,  // 踏み過ぎ運転
		// プロファイル 16,18
		SLOW_IN_FAST_OUT,   // スローインファストアウトカーブ
		REDUCE_TURN,        // 減速カーブ
		ACCEL_TURN,         // 非減速カーブ
//liuxin add end 2016.03.17		
		SPECIFIC_BINOMIAL_MAX
	};
	static const char* m_specBinomialNameList[SPECIFIC_BINOMIAL_MAX];
    static const char* m_profileTypeBinomial[PROFILE_TYPE_BINOMIAL_MAX];
	
    static const char* getSpecBinomialName(SPECIFIC_BINOMIAL status)
    {
        if ( status < SPECIFIC_BINOMIAL_MAX)
        {
            return m_specBinomialNameList[status];
        }
        else
            return NULL;
    }
    static SPECIFIC_BINOMIAL getSpecBinomialName(string name)
    {
        int i = 0;
        for (; i<SPECIFIC_BINOMIAL_MAX; i++)
        {
            if (name == string(m_specBinomialNameList[i]))
            {
                break;
            }
        }
        return (SPECIFIC_BINOMIAL)i;
    }

    static const char* getSpecBinomialTypeName(PROFILE_TYPE_BINOMIAL status)
    {
        if ( status < PROFILE_TYPE_BINOMIAL_MAX)
        {
            return m_profileTypeBinomial[status];
        }
        else
            return NULL;
    }
	PROFILE_TYPE_BINOMIAL type;

    int  getUserCheckSize()
    {
        return STATE_CHECK_SIZE;
    }
	 bool getUserStateCheck()
	{
		return m_userStateCheck;
	}
	 void setUserStateCheck(bool userStateCheck)
	{
		m_userStateCheck = userStateCheck;
	}
	EventCharacteristicAndStatusBinomial()
		: MAX_SAMPLE_SIZE(100)
		, STATE_CHECK_SIZE(10)
		, DEQUE_SIZE(MAX_SAMPLE_SIZE + STATE_CHECK_SIZE)
		, specific(NAN0)
		, percentage(0)
		, m_userStateCheck(false)
		, userState("状態未判定")
		, todaySpecificState(0.0)
		, oldSamplePercentage(0.0)
		, oldSampleSpecific(NAN0)
    {
    	
    }
    void resetSample()
    {
  #ifndef _WINDOWS_PLAT_
        queue<SPECIFIC_BINOMIAL> tempList;

        specificList.swap(tempList);
  #else
        int size = specificList.size();
        for(int i=0; i<size; i++)
        {
            specificList.pop();
        }
  #endif
        //specificList.clear();
    }
     void addSample(SPECIFIC_BINOMIAL sampleData)
    {
    	//DEQUEサイズを超える場合、先頭を削除
    	//if(specificList.size() >= DEQUE_SIZE)
    	//{
    	//	specificList.remove();
    	//}
    	specificList.push(sampleData);
    } 
     queue<SPECIFIC_BINOMIAL> getSampling()
    {
    	return specificList;
    }
    
     int getSamplingCount()
    {
    	return specificList.size();
    } 
     double getPercentage()
    {
    	return percentage;
    }
     void setPercentage(double value)
    {
    	percentage = value;
    }
     string getSpecific() // 画面表示用文字列
    {
    	string txt="未定義";

    	// プロファイル 1,2
    	if(specific == NORMAL)
    	{
    		txt = "急加速あまりしないタイプ";
    	}
    	else if(specific == SUDDEN)
    	{
    		txt = "急加速しがちなタイプ";
    	} 
    	
		// プロファイル 5,6
    	else if(specific == MARGIN_SMALL)
    	{
    		txt = "余裕度　小";
    	}
    	else if(specific == MARGIN_MEDIUM)
    	{
    		txt = "余裕度　中";
    	}
    	else if(specific == MARGIN_LARGE)
    	{
    		txt = "余裕度　大";
    	}
    	return txt;
    }
     SPECIFIC_BINOMIAL getSpecificENUM()
    {
    	return specific;
    }
     void setSpecific(SPECIFIC_BINOMIAL spec)
    {
    	specific = spec;
    }
     double getTodaySpecificState()
    {
    	return todaySpecificState;
    }
     void setTodaySpecificState(double value)
    {
    	todaySpecificState = value;
    }
     string getUserState()
    {
    	return userState;
    }
     void setUserState(bool state)// 画面表示用文字列
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
     void setOldSamplePercentage(double value)
    {
    	oldSamplePercentage = value;
    }
     void setOldSampleSpecific(SPECIFIC_BINOMIAL value)
    {
    	oldSampleSpecific = value;
    }
     double getOldSamplePercentage()
    {
    	return oldSamplePercentage;
    }
     SPECIFIC_BINOMIAL getOldSampleSpecific()
    {
    	return oldSampleSpecific;
    }
     PROFILE_TYPE_BINOMIAL getType()
    {
    	return type;
    }
     void setType(PROFILE_TYPE_BINOMIAL value)
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
private:
	 queue<SPECIFIC_BINOMIAL> specificList;
	 SPECIFIC_BINOMIAL specific;
	 double percentage;
	 bool m_userStateCheck;
	 string userState="状態未判定";
	 double todaySpecificState;
	
	// 前日までのユーザ特性（状態の判定に使用）
	// 前日までの特性％
	 double oldSamplePercentage;
	// 前日までの急加速判定
	 SPECIFIC_BINOMIAL oldSampleSpecific;
};

#endif
