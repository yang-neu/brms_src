#ifndef _H_CLIPS_RULE_MGR_
#define _H_CLIPS_RULE_MGR_
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "DBAccessor.h"

using namespace std;

#if 0
typedef struct st_Field {
	string name;
	int type;   //0:float;1:int;2:string
}Field;

typedef struct st_FieldAndVaule {
	string name;
	int type;
	void* value;
}FieldAndValue;
#endif



class ClipsRuleMgr
{
public:
	ClipsRuleMgr();

public:
	bool initialize(void *env, string sessionID);
	//bool insertSourceInputFact(const char *templatename, vector<FieldAndValue> &slots, void**outPut);
	vector<Field> &getSchema(string templateName) { return tableSchema[templateName]; }
	bool run();
private:
	void getUserTemplateFiles();
	void getAllTemplatesFromClips();
	void getTemplateFields();
private:
	void *m_theEnv;
	string m_sessionID;
	list<string> m_ruleFiles;
	list<string>::iterator m_ruleFilesItor;
	
	list<string> m_templateNames;
	list<string>::iterator m_templateItor;
	
	map< string, vector<Field> > tableSchema;


};



#endif /* _H_APP_MGR_ */


