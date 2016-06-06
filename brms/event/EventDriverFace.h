#ifndef _EVENT_DRIVER_FACE_H_
#define _EVENT_DRIVER_FACE_H_

#include <string>
#include "../common/TimeUtil.h"
using namespace std;

class EventDriverFace
{
public:

   EventDriverFace()
   {
       m_time = (double)GetSystemTime()/1000.0;
   }

   EventDriverFace(long long value, string type)
   {
      m_type = type;
      m_time = (double)GetSystemTime()/1000.0;
      m_value = (double)value;
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
      m_value = value;
   }

   EventDriverFace(string type, double time, long long value)
   {
      m_type = type;
      m_time = time;
      m_value = (double)value;
   }
    void updateValue(long long value)
	{
		m_value = (double)value;
		m_time = (double)GetSystemTime()/1000.0;
	}
private:
	string m_type;
	double m_time;
	double m_value;
};
#endif
