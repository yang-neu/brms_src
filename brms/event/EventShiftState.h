#ifndef _EVENT_SHIFT_STATE_H_
#define _EVENT_SHIFT_STATE_H_

#include <string>
#include "../common/TimeUtil.h"
using namespace std;

class EventShiftState
{

public:

   EventShiftState()
   {
      m_time = (double)GetSystemTime()/1000.0;
   }

   EventShiftState(long long value, string type)
   {
      m_time = (double)GetSystemTime()/1000.0;
      m_type = type;
	  m_value = (int)value;
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

   void setValue(int value)
   {
      m_value = value;
   }


   EventShiftState(double time, string type, long long value)
   {
      m_type = type;
      m_time = time;
	  m_value = (int)value;
   }
    void updateValue(long long value)
	{
		m_value = (int)value;
		m_time = (double)GetSystemTime()/1000.0;
	}
private:
	string m_type;
	double m_time;
	int m_value;
};
#endif
