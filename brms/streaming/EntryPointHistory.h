#ifndef _H_ENTRY_POINT_HISTORY_

#define _H_ENTRY_POINT_HISTORY_
extern "C"
{
	#include "clips.h"
}

#include <map>
#include <string>
#include<iostream>
#include <string.h>
#include <vector>
#include "EntryPoint.h"
#include "../DBAccessor.h"
#include "../event/EventSpeed.h"
#include "../event/EventRoadClass.h"
#include "../event/EventDistance.h"
#include "../event/EventAccelOpen.h"
#include "../event/EventSteeringAngle.h"
using namespace std;

class EntryPointHistory:public EntryPoint
{
public:
	EntryPointHistory();
	virtual void insert(string templateName, void *fact);
	virtual void query(string templateName, string queryCondition, double baseTime, DATA_OBJECT_PTR returnValuePtr);
	virtual void setExpire(string templateName, string expireCondition);
    virtual void insert(EventAccelOpen *accel);
    virtual void insert(EventSpeed *speed);
    virtual void insert(EventDistance *distance);
    virtual void insert(EventRoadClass *rc);
    virtual void insert(EventSteeringAngle *angle);
protected :
	bool getQueryCondition(string queryCondition, double );
private:
	const char* m_queryKey;
	LimitedInfo m_limited;
	map<string, LimitedInfo> m_limitList;
	map<string, int> m_tableList;
};

#endif
