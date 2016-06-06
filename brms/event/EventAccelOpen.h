#ifndef _EVENT_ACCEL_OPEN_H_
#define _EVENT_ACCEL_OPEN_H_
#include <string>
#include "../common/TimeUtil.h"
using namespace std;
class EventAccelOpen 
{
        
    // 傾僋僙儖奐搙
public:
    enum TYPE_ACCELOPEN
    {
    	ACCEL_OPEN,
    	ACCEL_OPEN_MAX	
    };
	const char* m_strTypeNameList[ACCEL_OPEN_MAX] = {"ACCEL_OPEN"};
	EventAccelOpen()
    {
        m_accelOpen = 0.0;
        m_time = 0.0;
        m_type = ACCEL_OPEN;
    }
    EventAccelOpen(long long value, TYPE_ACCELOPEN type)
    {
        m_accelOpen = (double)value / 10.0; //0.1% -> 1%
        m_time = (double)GetSystemTime()/1000.0;
        m_type = type;
    }
    EventAccelOpen(string accelOpen) {
    	
        m_accelOpen = atof(accelOpen.c_str())*2.0;
        m_time = (double)GetSystemTime()/1000.0;
        m_type = ACCEL_OPEN;
    }
	void updateValue(long long value)
	{
        m_accelOpen = (double)value /10.0; //0.1% -> 1%
		m_time = (double)GetSystemTime()/1000.0;
	}
    double getTime() {
        return m_time;
    }
    void setTime(double value)
    {
    	m_time = value;
    } 
    void setAccelOpen(double accelOpen)
    {
        m_accelOpen = accelOpen/10.0; //0.1% -> 1%;
    }
    double getAccelOpen()
    {
    	return m_accelOpen;
    }
    const char* getTypeString()
	{
		if (m_type < ACCEL_OPEN_MAX )
			return m_strTypeNameList[m_type];
		else
			return m_strTypeNameList[0];
	}
    TYPE_ACCELOPEN getType()
    {
    	return m_type;
    }
   void setType(TYPE_ACCELOPEN value)
    {
    	m_type = value;
    }
private:
	 	double m_accelOpen;
		TYPE_ACCELOPEN m_type;
    	// 
    	 double m_time;
};
#endif
