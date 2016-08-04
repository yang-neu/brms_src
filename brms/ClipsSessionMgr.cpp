
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h> 
#include <time.h> 
#include "interface/BrmsInterface.h"
#include "ClipsSessionMgr.h"

#include "ClipsRuleMgr.h"
#include "common/ConfigUtil.h"



extern "C"
{
	#include "clips.h"
}


ofstream ClipsSession::fw;
#include "streaming/EntryPoint.h"
RouterInterface* ClipsSession::m_router = NULL;

int ClipsSession::queryFunction(void *environment,const char *logicalName)
{
    if (strcmp(logicalName,"qt") == 0
    	|| strcmp(logicalName,"eventoutput") == 0
#ifdef _DEBUG_CLIPS_
		|| (fw.good() &&
			(strcmp(logicalName,"stdout") == 0
			|| strcmp(logicalName,"werror") == 0
			|| strcmp(logicalName,"wwarning") == 0
			|| strcmp(logicalName,"wdisplay") == 0
			|| strcmp(logicalName,"wdialog") == 0
			|| strcmp(logicalName,"wtrace") == 0
			|| strcmp(logicalName,"DEBUG") == 0))
#endif
            )
        { return(TRUE); }
    return(FALSE);
}
int ClipsSession::printFunction(void *environment,const char *logicalName,const char *str)
{
    if (m_router != NULL && string(logicalName) == m_router->getRouterName())
    {
		static char log_line[1024];
		static int log_len = 0;

		int len = strlen(str);
		if (log_len + len >= sizeof(log_line))
		{
			log_len = 0;
			return FALSE;
		}

		memcpy(&log_line[log_len], str, len);
		log_len += len;
		if (str[len - 1] == '\n')
		{
			log_line[log_len - 1] = 0;
			m_router->printFunction(environment, logicalName, log_line);
			log_len = 0;
		}

		return TRUE;

    } else if(m_router != NULL && string(logicalName) == "eventoutput") {
    	m_router->displayData(str);
    	return TRUE;
    }

 #ifdef _DEBUG_CLIPS_
	else if (fw.good() &&
		(string(logicalName) == "stdout"
		|| strcmp(logicalName,"werror") == 0
		|| strcmp(logicalName,"wwarning") == 0
		|| strcmp(logicalName,"wdisplay") == 0
		|| strcmp(logicalName,"wdialog") == 0
		|| strcmp(logicalName,"wtrace") == 0
		|| strcmp(logicalName,"DEBUG") == 0))
	{

        //cout<<str;
        fw<<str;
        fw.flush();
        return 1;
    }
 #endif
    return FALSE;
}

ClipsSession::ClipsSession()
	: m_state(SESSIONSTATE_IDLE)
{
}
#ifndef _DEBUG_CLIPS_
void ClipsSession::initialize(string sessionID, RouterInterface *router)
#else
void ClipsSession::initialize(int argc,char *argv, string sessionID, RouterInterface *router)
#endif
{
	cout<<"ClipsSession::initialize 0 sessionID:"<<sessionID<<endl;
	m_sessionID = sessionID;
		cout<<"ClipsSession::initialize 1"<<endl;
	m_theEnv = CreateEnvironment();
	cout<<"ClipsSession::initialize 2"<<endl;
#ifdef _DEBUG_CLIPS_
    cout<<"ClipsSession::initialize 3"<<endl;

	if (ConfigUtil::GetInstance()->GetConfigStr("CLIPSログ出力パス").length() > 0)
	{
		fw.open(ConfigUtil::GetInstance()->GetConfigStr("CLIPSログ出力パス").c_str(), ios::out);
#if 1
		EnvWatch(m_theEnv,"globals");
		EnvWatch(m_theEnv, "rules");
		EnvWatch(m_theEnv, "facts");
		EnvWatch(m_theEnv,"activations");
		EnvWatch(m_theEnv,"focus");
		EnvWatch(m_theEnv,"deffunctions");
		EnvWatch(m_theEnv,"compilations");
#endif
		EnvAddRouter(m_theEnv,"DEBUG",999,queryFunction,printFunction,NULL,NULL,NULL);
		//EnvAddRouter(m_theEnv,"wtrace",1000,queryFunction,printFunction,NULL,NULL,NULL);
		//EnvActivateRouter(m_theEnv,"wdialog");
		EnvActivateRouter(m_theEnv,"DEBUG");
#endif
#ifdef _PERFORMANCE_
		//for performance
		//EnvWatch(m_theEnv,"statistics");
#endif
	}

 #if 0
	char *tmp[1];
	tmp[0] = argv;
	RerouteStdin(m_theEnv,1,tmp);
#else
    if (router != NULL)
    {
        m_router = router;
        //const char *routerName=m_router->getRouterName().c_str();
        string routerName = m_router->getRouterName();
		EnvAddRouter(m_theEnv,routerName.c_str(),30,queryFunction,printFunction,NULL,NULL,NULL);
		EnvActivateRouter(m_theEnv, routerName.c_str());
    }
#endif	
	cout<<"ClipsSession::initialize 4"<<endl;	
	EnvReset(m_theEnv);

	
	cout<<"ClipsSession::initialize 5"<<endl;
	m_ruleMgr = new ClipsRuleMgr();
	if (m_ruleMgr != NULL) 
		m_ruleMgr->initialize(m_theEnv, m_sessionID);
}


void ClipsSession::registEntryPoint(string streamName, EntryPoint *pStream)
{	
	if (pStream != NULL)
	{
		pStream->setupEnv(m_theEnv);
		m_EntryPointList.insert(pair<string, EntryPoint *>(streamName, pStream));
	}
}

ClipsSessionMgr* ClipsSessionMgr::m_instance = NULL;
ClipsSessionMgr::ClipsSessionMgr()
{
}
ClipsSession *ClipsSessionMgr::getSessionFromEnv(void *env)
{
	ClipsSession *pRet = NULL;
	if (env != NULL && m_envList[env].size() > 0)
	{
		string sessionID = m_envList[env];
		pRet = m_sessionList[sessionID];
	}
	return pRet;
}
ClipsSession *ClipsSessionMgr::getSession(string sessionID)
{
	return m_sessionList[sessionID];
}
ClipsSession *ClipsSessionMgr::createSession(string sessionID,  RouterInterface *router)
{
	ClipsSession *session = 0;
		cout<<"createSession 0"<<endl;
	if (sessionID.size() > 0)
	{
			cout<<"createSession 1"<<endl;
		session = new ClipsSession();
#ifdef _DEBUG_CLIPS_
			cout<<"createSession 2"<<endl;
        session->initialize(m_argc, m_argv, sessionID, router);
#else
			cout<<"createSession 3"<<endl;
        session->initialize(sessionID, router);
#endif
			cout<<"createSession4"<<endl;
		m_sessionList.insert(pair<string, ClipsSession *>(sessionID, session));
			cout<<"createSession 5"<<endl;
		m_envList.insert(pair<void*, string>(session->getClipsEnv(), sessionID));
		cout<<"createSession 6"<<endl;
	}
	
	return session;
}

	
#ifdef _DEBUG_CLIPS_
bool ClipsSessionMgr::initialize(int argc,char *argv[])
{

	if ( argc > 0 )
	{
		m_argc = argc;
		memcpy(&m_argv[0], argv[0], strlen(argv[0]));
	}
	return true;		
}
#endif
