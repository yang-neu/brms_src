#ifndef _H_CLIPS_SESSION_MGR_
#define _H_CLIPS_SESSION_MGR_
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include <string>
//#include "streaming/EntryPoint.h"
using namespace std;

class ClipsRuleMgr;
class EntryPoint;
class RouterInterface;

class ClipsSession
{
public:
	enum SessionState {
		SESSIONSTATE_IDLE,
		SESSIONSTATE_RECVING
	};

	ClipsSession();
    static int printFunction(void *environment,const char *logicalName,const char *str);
    static int queryFunction(void *environment,const char *logicalName);

public:
#ifndef _DEBUG_CLIPS_
    void initialize(string sessionID,  RouterInterface *router);
#else
    void initialize(int argc,char *argv, string sessionID,  RouterInterface *router);
#endif
	EntryPoint *getEntryPoint(string streamName) { return m_EntryPointList[streamName]; }
	void registEntryPoint(string streamName, EntryPoint *pStream);
	ClipsRuleMgr *getRuleMgr() { return  m_ruleMgr;}
	void *getClipsEnv() { return m_theEnv;}
	void setState(SessionState state) { m_state = state; }
	SessionState getState() { return m_state; }

private:
	string m_sessionID;
    static RouterInterface *m_router;
	void *m_theEnv;
	ClipsRuleMgr *m_ruleMgr;
	EntryPoint *m_entryPoint;
	map< string, EntryPoint *> m_EntryPointList;
#ifdef _DEBUG_CLIPS_
    static ofstream fw;
#endif
	SessionState m_state;
};

class ClipsSessionMgr
{
public:
	static ClipsSessionMgr* get()
	{
		if(m_instance == 0)
			m_instance = new ClipsSessionMgr();
		
		return m_instance;
	}
	
public:

#ifdef _DEBUG_CLIPS_
	bool initialize(int argc,char *argv[]);
#endif
	ClipsSession *getSession(string sessionID);
	ClipsSession *getSessionFromEnv(void *env);
    ClipsSession *createSession(string sessionID,  RouterInterface *router);
protected:
	ClipsSessionMgr();	
private:
	
private:
#ifdef _DEBUG_CLIPS_
	int m_argc;
	char m_argv[100];
#endif
	static ClipsSessionMgr* m_instance;
	map< string, ClipsSession*> m_sessionList;
	map< void*, string> m_envList;

};



#endif /* _H_CLIPS_SESSION_MGR_ */


