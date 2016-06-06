

#include "clips.h"
#include "stdio.h"

#if 0
double entryPoint(void * environment)
{
	if (EnvArgCountCheck(environment,"entryPoint",EXACTLY,1) == -1) return(-1.0);

	
	const char* streamName;
	streamName = EnvRtnLexeme(environment,1);
	
	printf("arg:%s", streamName);
	return 1.0;
}
#endif
void *entryPoint(void * m_theEnv)
{
	void *newFact;
	void *templatePtr;
	void *theMultifield;
	DATA_OBJECT theValue;
	char *templatename = "InputSource";


	/* Create the fact. */
	/*==================*/
	templatePtr = EnvFindDeftemplate(m_theEnv,templatename);
	newFact = EnvCreateFact(m_theEnv,templatePtr);
	if (newFact == NULL) return 0;
	

	theValue.type = INTEGER;
	theValue.value = EnvAddLong(m_theEnv,100);		
	EnvPutFactSlot(m_theEnv,newFact,"speed",&theValue);
	
	theValue.type = FLOAT;
	theValue.value = EnvAddDouble(m_theEnv,1.0);
	EnvPutFactSlot(m_theEnv,newFact,"astatus",&theValue);
	
	theValue.type = INTEGER;
	theValue.value = EnvAddLong(m_theEnv,2);		
	EnvPutFactSlot(m_theEnv,newFact,"rclass",&theValue);
	
	theValue.type = INTEGER;
	theValue.value = EnvAddLong(m_theEnv,10000);		
	EnvPutFactSlot(m_theEnv,newFact,"distance",&theValue);
	
	EnvAssignFactSlotDefaults(m_theEnv,newFact);
	EnvAssert(m_theEnv,newFact);
	return newFact;
	
}