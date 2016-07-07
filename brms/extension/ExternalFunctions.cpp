#include <stdio.h>

#include <string> 

#include <map>
#include <vector>

extern "C"
{
	#include "clips.h"
}

#include "ExternalFunctions.h"
#include "../ClipsRuleMgr.h"
#include "../ClipsSessionMgr.h"

#include "../streaming/EntryPoint.h"
#include "../streaming/EntryPointHistory.h"

extern "C"
{
void doAction(void* env, string &templateName, DATA_OBJECT_PTR returnValuePtr);
void doInsertAction(void *env, string &templateName, void *factAdress, string &streamName);
void doQueryAction(void *env,string &templateName, string &queryCondition,double baseTime, string &streamName, DATA_OBJECT_PTR returnValuePtr);
void doExpireAction(void *env, string &templateName, string &expireCondition, string &streamName);

Ext_FuncInfo g_extFuncTable[MAX_EXT_FUNC_CNT] =	{
	//{	"", 					"entry-point", 				'b', NewEntryPoint, 			"entry-point"			},
    //{	"EventSpeed", 			"EventSpeedHistory", 		'm', EventSpeedHistory, 		"EventSpeedHistory"		},
    //{	"EventDistanceDiff", 	"EventDistanceDiffHistory", 'm', EventDistanceDiffHistory, 	"EventDistanceDiffHistory"	},
    //{	"EventDistance", 		"EventDistanceHistory", 	'm', EventDistanceHistory, 		"EventDistanceHistory"	},
	{	"EventAccelOpen", 		"EventAccelOpenHistory", 	'm', EventAccelOpenHistory, 	"EventAccelOpenHistory"	},
    {	"EventRoadClass", 		"EventRoadClassHistory", 	'm', EventRoadClassHistory, 	"EventRoadClassHistory"	},
    //{	"EventSteeringAngle", 		"EventSteeringAngleHistory", 'm', EventSteeringAngleHistory, "EventSteeringAngleHistory"	},
};

//(entry-point create streamname)
int  NewEntryPoint(void* env)
{	
	const char *action = EnvRtnLexeme(env, 1);
	string strAction (action);
	const char *streamName = EnvRtnLexeme(env, 2);
	string strStreamName(streamName);
	
	cout<<"entry-point  action :"<<strAction<<";stream name :"<<strStreamName<<endl;
	
	if (strAction == "create")
	{

        if (strStreamName != "Driving Hitory Stream")
            return FALSE;
		ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
		if (pmgr == 0) return 0;
		EntryPoint *pEntryPoint = new EntryPointHistory();
		pmgr->getSessionFromEnv(env)->registEntryPoint(strStreamName, pEntryPoint);
		pEntryPoint->setupEnv(env);
		return TRUE;
	}
	return FALSE;
}

//(EventSpeedHistory insert/query factaddress/condition to/from entry-point streamname)
//condition: before/after[1ms,1s]base
void EventSpeedHistory(void* env, DATA_OBJECT_PTR returnValuePtr)
{
    //cout<<"EventSpeedHistory  action "<<endl;
	string templateName = "EventSpeed";
	doAction(env, templateName, returnValuePtr);
}

void EventDistanceDiffHistory(void* env, DATA_OBJECT_PTR returnValuePtr)
{
    //cout<<"EventDistanceDiffHistory  action "<<endl;
    string templateName = "EventDistanceDiff";
	doAction(env, templateName, returnValuePtr);
}
//(EventDistanceHistory insert/query factaddress/condition to/from entry-point streamname)
void EventDistanceHistory(void* env, DATA_OBJECT_PTR returnValuePtr)
{
    //cout<<"EventDistanceHistory  action "<<endl;
    string templateName = "EventDistance";
    doAction(env, templateName, returnValuePtr);
}
//(EventAccelOpenHistory insert/query factaddress/condition to/from entry-point streamname)
void EventAccelOpenHistory(void* env, DATA_OBJECT_PTR returnValuePtr)
{
}
//(EventRoadClassHistory insert/query factaddress/condition basetime to/from entry-point streamname)
void EventRoadClassHistory(void* env, DATA_OBJECT_PTR returnValuePtr)
{
}
void EventSteeringAngleHistory(void* env, DATA_OBJECT_PTR returnValuePtr)
{
    //cout<<"EventDistanceHistory  action "<<endl;
    string templateName = "EventSteeringAngle";
    doAction(env, templateName, returnValuePtr);
}
void  doAction(void* env, string &templateName, DATA_OBJECT_PTR returnValuePtr)
{
	const char *action = EnvRtnLexeme(env, 1);
	string strAction (action);
	
	if (strAction == "query")
	{
		// (EventSpeedHistory query before[500ms] ?time1 from entry-point "Driving Hitory Stream")
		const char *queryCondition = EnvRtnLexeme(env, 2);
		double baseTime = EnvRtnDouble(env, 3);
		const char *streamName = EnvRtnLexeme(env, 6);
		string strQueryCondition = string(queryCondition);
		string strStreamName = string(streamName);
		//string strBaseTime = string(baseTime);
		doQueryAction(env,templateName, strQueryCondition,baseTime, strStreamName , returnValuePtr);
	}
	else if (strAction == "insert")
	{
		// (EventSpeedHistory insert ?afterS to entry-point "Driving Hitory Stream")
		DATA_OBJECT theValue;
		EnvRtnUnknown(env, 2, &theValue);
		if (GetpType(&theValue) == FACT_ADDRESS)
		{
			void *factAdress = DOPToPointer(&theValue);
			const char *streamName = EnvRtnLexeme(env, 5);
			string strStreamName = string(streamName);
			doInsertAction(env, templateName, factAdress, strStreamName);
		}
	}
	else if (strAction == "expire")
	{
		// (EventSpeedHistory expire 5m to entry-point "Driving Hitory Stream")
		const char *expireCondition = EnvRtnLexeme(env, 2);
		string strExpireCondition = string(expireCondition);
		const char *streamName = EnvRtnLexeme(env, 5);
		string strStreamName = string(streamName);
		doExpireAction(env, templateName, strExpireCondition, strStreamName);
	}
	else
	{
	}
}


void doInsertAction(void *env, string &templateName, void *factAdress, string &streamName)
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	if (pmgr == 0) return ;
	
	//ClipsRuleMgr *pRuleMgr = pmgr->getSessionFromEnv(env)->getRuleMgr();
	
	EntryPointHistory *pEntryPoint = dynamic_cast<EntryPointHistory *>(pmgr->getSessionFromEnv(env)->getEntryPoint(streamName));
	
	if (pEntryPoint == NULL)
	{
		cout<<"doInsertAction error, pEntryPoint:"<<pEntryPoint<<endl;
		return;
	}
	
	pEntryPoint->insert(templateName, factAdress);
	
}

void doQueryAction(void *env,string &templateName, string &queryCondition,double baseTime, string &streamName, DATA_OBJECT_PTR returnValuePtr)
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	if (pmgr == 0) return ;
	
	
	EntryPointHistory *pEntryPoint = dynamic_cast<EntryPointHistory *>(pmgr->getSessionFromEnv(env)->getEntryPoint(streamName));
	
	if (pEntryPoint == NULL)
	{
		cout<<"doQueryAction error, pEntryPoint:"<<pEntryPoint<<endl;
		return;
	}
	
	pEntryPoint->query(templateName, queryCondition, baseTime, returnValuePtr);
}

void doExpireAction(void *env, string &templateName, string &expireCondition, string &streamName)
{
	ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	if (pmgr == 0) return ;


	EntryPointHistory *pEntryPoint = dynamic_cast<EntryPointHistory *>(pmgr->getSessionFromEnv(env)->getEntryPoint(streamName));

	if (pEntryPoint == NULL)
	{
		cout<<"doExpireAction error, pEntryPoint:"<<pEntryPoint<<endl;
		return;
	}

	pEntryPoint->setExpire(templateName, expireCondition);
}
}

	
