#ifndef _EVENT_SPEED_H_
#define _EVENT_SPEED_H_
#include <string>
#include "../common/TimeUtil.h"
using namespace std;
class EventSpeed
{
        
    // 憱峴拞/掆巭拞敾抐僷乕僣
public :
    enum TYPE_SPEED
    {
    	VEHICLE_SPEED_SP1,
    	VEHICLE_SPEED_MAX
    };
  
	const char* m_strTypeNameList[VEHICLE_SPEED_MAX] = {"VEHICLE_SPEED_SP1"};

     EventSpeed()
    {
        m_speed = 0.0;
        m_time = 0.0;
        m_type = VEHICLE_SPEED_SP1;
    }
    EventSpeed(long long value, TYPE_SPEED type)
    {
        m_speed = (double)value/100.0; //0.01km/h->1km/h
        m_time = (double)GetSystemTime()/1000.0;
        m_type = type;
    }
    EventSpeed(string _speed) {
        m_time = (double)GetSystemTime()/1000.0;
        m_speed = atof(_speed.c_str())/100.0; //0.01km/h->1km/h;
        m_type = VEHICLE_SPEED_SP1;
    }
	void updateValue(long long value)
	{
        m_speed = (double)value/100.0; //0.01km/h->1km/h
		m_time = (double)GetSystemTime()/1000.0;
	}
    double getTime() {
        return m_time;
    }
    void setTime(double value)
    {
    	m_time = value;
    } 
    void setSpeed(double _speed)
    {
        m_speed = _speed/100.0; //0.01km/h->1km/h;
    	
    }
    double getSpeed()
    {
    	return m_speed;
    }

	const char* getTypeString()
	{
		if (m_type < VEHICLE_SPEED_MAX )
			return m_strTypeNameList[m_type];
		else
			return m_strTypeNameList[0];
	}
     TYPE_SPEED getType()
    {
    	return m_type;
    }
    void setType(TYPE_SPEED value)
    {
    	m_type = value;
    }    
private:
     double m_speed; //1km/h
	 TYPE_SPEED m_type;
     double m_time;
};
#endif

