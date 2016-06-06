#ifndef _EVENT_BRAKE_PRESSURE_H_
#define _EVENT_BRAKE_PRESSURE_H_

#include <string>
#include "../common/TimeUtil.h"
using namespace std;

class EventBrakePressure
{

public:
 
   EventBrakePressure()
   {
      m_time = (double)GetSystemTime()/1000.0;
   }

   EventBrakePressure(long long value, string type)
   {
      m_time = (double)GetSystemTime()/1000.0;
      m_type = type;
      m_value = (double)value/1000.0; //0.001Mpa -> 1Mpa;
   }


   string getType()
   {
      return m_type;
   }

   void setType(string type)
   {
      m_type = type;
   }

   double getTime()
   {
      return m_time;
   }

   void setTime(double time)
   {
      m_time = time;
   }

   double getValue()
   {
      return m_value;
   }

   void setValue(double value)
   {
      m_value = value/1000.0; //0.001Mpa -> 1Mpa;
   }


   EventBrakePressure(double time, string type, long long value)
   {
      m_type = type;
      m_time = time;
      m_value = (double)value /1000.0; //0.001Mpa -> 1Mpa;
   }
    void updateValue(long long value)
	{
        m_value = (double)value /1000.0; //0.001Mpa -> 1Mpa
		m_time = (double)GetSystemTime()/1000.0;
	}
private:
	string m_type;
	double m_time;
	double m_value;
};
#endif
