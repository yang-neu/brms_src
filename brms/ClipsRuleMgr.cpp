
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h> 
#include <time.h> 

#include "ClipsRuleMgr.h"

#define _DEBUG_ 1

extern "C"
{
	#include "clips.h"
}


ClipsRuleMgr::ClipsRuleMgr()
{
}

bool ClipsRuleMgr::run()
{
	EnvRun(m_theEnv,-1);
	return true;
}

//	void AddExampleFact2(void *environment, const char* templatename)


void ClipsRuleMgr::getUserTemplateFiles()
{
	char *pwd="./clp";
    DIR *dp;
	struct dirent *entry;
    struct stat statbuf;
	m_ruleFiles.push_back("Basic.clp");
	if((dp = opendir(pwd)) == NULL) {
        cerr<<"ClipsRuleMgr::GetUserTemplateFiles:cannot open directory: "<<pwd<<endl;
        return;
    }
    chdir(pwd);
     while((entry = readdir(dp)) != NULL) {
 #ifndef _WINDOWS_PLAT_
        lstat(entry->d_name,&statbuf);
 #else
         stat(entry->d_name,&statbuf);
 #endif
        if(S_ISDIR(statbuf.st_mode)) {
           cout<<"ClipsRuleMgr::GetUserTemplateFiles:get folder:"<<entry->d_name<<" ignore"<<endl;
        }
        else 
	    {
	   	if(string(entry->d_name) == string("Basic.clp"))
		{
		}
		else 
			m_ruleFiles.push_back(entry->d_name);
	    }
    }

	chdir("..");
    closedir(dp);

}
void ClipsRuleMgr::getAllTemplatesFromClips()
{
	void *firstT;
	firstT = EnvGetNextDeftemplate(m_theEnv, 0);
	cout<<"ClipsRuleMgr::GetAllTemplatesFromClips:first template name:"<<EnvGetDeftemplateName(m_theEnv, firstT)<<endl;
	while((firstT = EnvGetNextDeftemplate(m_theEnv,firstT)) != 0)
	{
		m_templateNames.push_back(EnvGetDeftemplateName(m_theEnv, firstT));
		cout<<"ClipsRuleMgr::GetAllTemplatesFromClips:template name:"<<EnvGetDeftemplateName(m_theEnv, firstT)<<endl;
	}
}	


void ClipsRuleMgr::getTemplateFields()
{
		void *templatePtr;
		DATA_OBJECT theValue;
		void *theMultifield;
		
		DATA_OBJECT theValueOfType;
		void *theMultifieldOfType;
		int cnt1 = 0;
		int i = 1;
		int fieldType;
		void *fieldValue;
		m_templateItor = m_templateNames.begin();
		while(m_templateItor!=m_templateNames.end())
		{	
			cout<<"Rule Engine IntializeStream::m_templateNames:" <<*m_templateItor<<endl;
			templatePtr = EnvFindDeftemplate(m_theEnv,(*m_templateItor).c_str());
			EnvDeftemplateSlotNames(m_theEnv, templatePtr, &theValue);
			
			if (GetpType(&theValue) == MULTIFIELD)
			{
				cnt1 = GetpDOLength(&theValue);
				//theMultifield = theValue.value;
				theMultifield=GetValue(theValue);
				vector<Field> fields;
				for (i=1; i<=cnt1; i++)
				{
					
					fieldType = GetMFType(theMultifield,i);
					if (fieldType == SYMBOL)
					{
						Field tmp;
						fieldValue = (void*)ValueToString(GetMFValue(theMultifield,i));
						tmp.name = string((char*)fieldValue);
						tmp.type = 2;
						EnvDeftemplateSlotTypes(m_theEnv,templatePtr,(char *)fieldValue, &theValueOfType);
						if (GetpType(&theValueOfType) == MULTIFIELD)
						{
							GetpDOLength(&theValueOfType);
							theMultifieldOfType = theValueOfType.value;
							
							
							///default contraict to first one
							fieldType = GetMFType(theMultifieldOfType,1);	
						
							if (fieldType == SYMBOL)
							{
									fieldValue = GetMFValue(theMultifieldOfType,1);
									if (string(ValueToString(fieldValue)) == string("FLOAT"))
									{
										tmp.type = 0;
									}
									else if (string(ValueToString(fieldValue)) == string("INTEGER"))
									{
										tmp.type = 1;
									}
									else
									{
										tmp.type = 2;
									}
							}
							else
							{
									cout<<"Rule Engine IntializeStream::EnvDeftemplateSlotTypes get unknown field type when get slot type:" <<fieldType<<endl;
							}
						}
						
						fields.push_back(tmp);
					}
					else
					{
						cout<<"Rule Engine IntializeStream::get unknown field type when get slot name:" <<fieldType<<endl;
					}
				}
				tableSchema.insert(pair<string, vector<Field> >(*m_templateItor, fields));	
			}
			else
			{
					cout<<"Rule Engine IntializeStream:::EnvDeftemplateSlotNames return not multifiled" <<endl;
			}
		
			m_templateItor++;
		}
		
		
		//call streaming interface to set tableSchema here
		
}
bool ClipsRuleMgr::initialize(void *env, string sessionID)
{
		cout<<"ClipsRuleMgr::initialize 0"<<endl;
		getUserTemplateFiles();
		cout<<"ClipsRuleMgr::initialize 1"<<endl;
		m_theEnv = env;
		m_sessionID = sessionID;

		//rule files should use defmodule to group  T.B.D
				
		m_ruleFilesItor = m_ruleFiles.begin();
		while(m_ruleFilesItor!=m_ruleFiles.end())
		{
			EnvLoad(m_theEnv,string("./clp/" + *m_ruleFilesItor).c_str());
			cout<<"Rule Engine Initialize::load clp file:" <<*m_ruleFilesItor<<endl;
			m_ruleFilesItor++;
		}
		
		EnvReset(m_theEnv);
		
		getAllTemplatesFromClips();
		
		getTemplateFields();
		
		return true;
		
}
