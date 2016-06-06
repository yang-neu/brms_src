#ifndef _EVENT_ROAD_CLASS_H_
#define _EVENT_ROAD_CLASS_H_
#include "../common/TimeUtil.h"



class EventRoadClass{
        
public: 
    
    
    enum TYPE_ROAD_CLASS
    {
			ROAD_CLASS,
	    	ROAD_CLASS_MAX
    };
    const char* m_strTypeNameList[ROAD_CLASS_MAX] = {"ROAD_CLASS"};
    enum ROAD_CLASS_KIND
    {
    	OPENROAD,
    	EXPRESSWAY,
    	ROAD_CLASS_KIND_MAX
    };
	const char* m_strKindNameList[ROAD_CLASS_KIND_MAX] = {"OPENROAD","EXPRESSWAY" };
    
     EventRoadClass()
    {
        m_canData = 0;
        m_time = 0.0;
        m_roadClass = OPENROAD;
        m_type = ROAD_CLASS;
    }
    EventRoadClass(long long value, TYPE_ROAD_CLASS type)
    {
        m_canData = (long)value;
        m_time = (double)GetSystemTime()/1000.0;
        m_roadClass = OPENROAD;
        m_type = type;
    }
   
	void updateValue(long long value)
	{
		m_canData = (long)value;
		m_time = (double)GetSystemTime()/1000.0;
	}
    double getTime() {
        return m_time;
    }
    void setTime(double value)
    {
    	m_time = value;
    } 
    void setRoadClass(ROAD_CLASS_KIND value)
    {
    	m_roadClass = value;
    }
    const char* getKindString()
	{
		if (m_roadClass < ROAD_CLASS_KIND_MAX )
			return m_strKindNameList[m_type];
		else
			return m_strKindNameList[0];
	}
    ROAD_CLASS_KIND getRoadClass()
    {
    	return m_roadClass;
    }
    void setCanData(long value)
    {
    	m_canData = value;
    }
    int getCanData()
    {
    	return (int)m_canData;
    }
    
    const char* getTypeString()
	{
		if (m_type < ROAD_CLASS_MAX )
			return m_strTypeNameList[m_type];
		else
			return m_strTypeNameList[0];
	}
    TYPE_ROAD_CLASS getType()
    {
    	return m_type;
    }
    void setType(TYPE_ROAD_CLASS value)
    {
    	m_type = value;
    }  
private:
	 TYPE_ROAD_CLASS m_type;  
    
    long m_canData;
    ROAD_CLASS_KIND m_roadClass;
    // 
    double m_time;
};
#endif

