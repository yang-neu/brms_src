
#ifndef _H_EXTERNAL_FUNCTIONS_
#define _H_EXTERNAL_FUNCTIONS_



#include <stdio.h>

#include <string> 

#include <map>
#include <vector>
#include "ExternalDef.h"

using namespace std;
	
extern "C"	
{

extern Ext_FuncInfo g_extFuncTable[MAX_EXT_FUNC_CNT];

extern int NewEntryPoint(void* env);
extern void EventSpeedHistory(void* env, DATA_OBJECT_PTR returnValuePtr);
extern void EventDistanceDiffHistory(void* env, DATA_OBJECT_PTR returnValuePtr);	
extern void EventDistanceHistory(void* env, DATA_OBJECT_PTR returnValuePtr);
extern void EventAccelOpenHistory(void* env, DATA_OBJECT_PTR returnValuePtr);
extern void EventRoadClassHistory(void* env, DATA_OBJECT_PTR returnValuePtr);	
extern void EventSteeringAngleHistory(void* env, DATA_OBJECT_PTR returnValuePtr);
}


#endif
