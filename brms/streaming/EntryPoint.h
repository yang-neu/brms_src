#ifndef _H_ENTRY_POINT_

#define _H_ENTRY_POINT_

#include <map>
#include <string>
#include<iostream>
#include <vector>
#include <string.h>
extern "C"
{
	#include "clips.h"
}

#include "../DBAccessor.h"

using namespace std;

class EntryPoint
{
public:
	enum FLUSH_TYPE {
		FLUSH_TYPE_MODULE_MAIN,
		FLUSH_TYPE_MODULE_SPA_500MS,
		FLUSH_TYPE_MODULE_SPA_10S,
        FLUSH_TYPE_MODULE_SPA_Immediate,
		FLUSH_TYPE_MODULE_MAX
	};
		
	const char* m_strTypeNameList[FLUSH_TYPE_MODULE_MAX] = {
		"MAIN",
		"SpecificAgenda",
        "SpecificAgenda10s",
        "SpecificAgenda",

	};
	EntryPoint();
	virtual void initialize(string streamName) { m_streamName = streamName; }
	virtual void setupEnv(void *env) { m_theEnv = env; }
    bool insert(const char* templatename, vector<FieldAndValue> &slots, void** outPut);

    //bool insert(vector<FieldAndValue> &slots,  DATA_OBJECT_PTR returnValuePtr);
    bool insert(vector<FieldDataVec> &slotsList,  DATA_OBJECT_PTR returnValuePtr);
	virtual void flush(FLUSH_TYPE type);
	virtual void updateAll() { return ; }
    virtual void saveAllSample() { return ;}
protected :
	string m_streamName;
	void *m_theEnv;
	void *m_multifieldPtr;
	
};

#endif
