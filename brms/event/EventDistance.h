#ifndef _EVENT_DISTANCE_H_
#define _EVENT_DISTANCE_H_
#include <string>
#include "../common/TimeUtil.h"
using namespace std;
class EventDistance {
 
 public :       
    // 幵娫嫍棧
   
    
    enum TYPE_DISTANCE
    {
    	VEHICLE_FOLLOWING_DISTANCE,
    	VEHICLE_FOLLOWING_DISTANCE_MAX
    };

    const char* m_strTypeNameList[VEHICLE_FOLLOWING_DISTANCE_MAX] = {"VEHICLE_FOLLOWING_DISTANCE"};
    EventDistance()
    {
        m_distance = 255.0 /100.0; //0.01m ->1m ;
        m_time = (double)GetSystemTime()/1000.0;
        m_type = VEHICLE_FOLLOWING_DISTANCE;
    }
    EventDistance(long long value, TYPE_DISTANCE type)
    {
        m_distance = (double)value /100.0; //0.01m ->1m
        m_time = (double)GetSystemTime()/1000.0;
        m_type = type;
    }
    EventDistance( string distance) {
        m_time = (double)GetSystemTime()/1000.0;
        m_distance = atof(distance.c_str()) /100.0; //0.01m ->1m ;
        m_type = VEHICLE_FOLLOWING_DISTANCE;
    }
	void updateValue(long long value)
	{
        m_distance = (double)value /100.0; //0.01m ->1m ;
		m_time = (double)GetSystemTime()/1000.0;
	}
    double getTime() {
        return m_time;
    }
    void setTime(double value)
    {
    	m_time = value;
    } 
    void setDistance(double distance)
    {
        m_distance = distance /100.0; //0.01m ->1m ;
    }
    double getDistance()
    {
    	return m_distance;
    }
     const char* getTypeString()
	{
		if (m_type < VEHICLE_FOLLOWING_DISTANCE_MAX )
			return m_strTypeNameList[m_type];
		else
			return m_strTypeNameList[0];
	}
    TYPE_DISTANCE getType()
    {
    	return m_type;
    }
    void setType(TYPE_DISTANCE value)
    {
    	m_type = value;
    }
private:
     TYPE_DISTANCE m_type;
     double m_distance; //m

    // 
     double m_time;
};
#endif

