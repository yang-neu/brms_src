
#include <sys/stat.h>
#include <sys/time.h> 
#include <time.h> 
#include "EntryPoint.h"

EntryPoint::EntryPoint()
{
	m_multifieldPtr = NULL;
}
bool EntryPoint::insert(const char* templatename, vector<FieldAndValue> &slots, void** outPut)
{

	void *newFact;
	void *templatePtr;
	void *theMultifield;
	DATA_OBJECT theValue;
	
	struct timeval t_start,t_end; 
	double cost_time = 0; 
	//get start time 
	gettimeofday(&t_start, NULL); 
	double start = ((double)t_start.tv_sec)*1000+(double)t_start.tv_usec/1000; 
	cout<<fixed<<"=====new fact begn time:"<<start<<"========"<<endl;
	

#if 0	
	time_t now; 
	struct tm  *timenow;
	time(&now);
	timenow = localtime(&now);
	cout<<"=====new fact begin time:"<<asctime(timenow)<<"========"<<endl;
#endif
	/* Create the fact. */
	/*==================*/
	templatePtr = EnvFindDeftemplate(m_theEnv,templatename);
	newFact = EnvCreateFact(m_theEnv,templatePtr);
	if (newFact == NULL) return false;
	
	if (outPut !=0 )
		*outPut = newFact;
	
	vector<FieldAndValue>::iterator it=slots.begin();
	FieldAndValue *tmp=NULL;
	while(it != slots.end())
	{
		tmp = &(*it);
		if (tmp != NULL && tmp->type == 1)
		{
			theValue.type = INTEGER;
			theValue.value = EnvAddLong(m_theEnv,tmp->data.i_value);
			EnvPutFactSlot(m_theEnv,newFact,tmp->name.c_str(),&theValue);
		}
		else if (tmp != NULL && tmp->type == 0)
		{
			theValue.type = FLOAT;
			theValue.value = EnvAddDouble(m_theEnv,tmp->data.f_value);
			EnvPutFactSlot(m_theEnv,newFact,tmp->name.c_str(),&theValue);
		}
		else if (tmp != NULL && tmp->type == 2)
		{
			theValue.type = SYMBOL;
			theValue.value = EnvAddSymbol(m_theEnv,((char*)tmp->data.s_value));
			EnvPutFactSlot(m_theEnv,newFact,tmp->name.c_str(),&theValue);
		}
		
		else
		{
			if(tmp == NULL)
				cout<<"EntryPoint::insert::CreateFact:template name->"<<templatename<<"parameter is 0"<<endl;
			else
				cout<<"EntryPoint::insert::CreateFact:template name->"<<templatename<<"parameter not int/float/string type, type is->"<<tmp->type<<endl;
		}
		it++;
	}

	
	EnvAssignFactSlotDefaults(m_theEnv,newFact);
	EnvAssert(m_theEnv,newFact);
	
    //EnvRun(m_theEnv,-1);
#if 0	
	time(&now);
	timenow = localtime(&now);
	cout<<"=====new fact end time:"<<asctime(timenow)<<"========"<<endl;
#endif
	gettimeofday(&t_end, NULL); 
	double end = ((double)t_end.tv_sec)*1000+(double)t_end.tv_usec/1000; 
	cout<<fixed<<"=====new fact end time:"<<end<<"========"<<endl;
	cost_time = end - start; 
	cout<<fixed<<"=====new fact cost time:"<<cost_time<<"========"<<endl;
	return true;

}
//bool EntryPoint::insert(vector<FieldAndValue> &slots,  DATA_OBJECT_PTR returnValuePtr)
bool EntryPoint::insert(vector<FieldDataVec> &slotsList,  DATA_OBJECT_PTR returnValuePtr)
{
	if (returnValuePtr == NULL) return false;
	
    if (slotsList.size() == 0) return false;
	
	//void *m_multifieldPtr;
	//m_multifieldPtr = EnvCreateMultifield(m_theEnv,slots.size());
	//if (m_multifieldPtr == NULL)
    //m_multifieldPtr = EnvCreateMultifield(m_theEnv,slots.size());

    int itemSize = 0;
    vector<FieldDataVec>::iterator itList = slotsList.begin();
    if (itList != slotsList.end())
    {
        itemSize = (*itList).size();
         m_multifieldPtr = EnvCreateMultifield(m_theEnv,slotsList.size()*itemSize);
    }
    int i = 1;
    while(itList != slotsList.end())
    {
            vector<FieldAndValue>  slots = *itList;
            vector<FieldAndValue>::iterator it=slots.begin();
            FieldAndValue *tmp = NULL;

            while(it != slots.end())
            {
                tmp = &(*it);
                if (tmp != NULL && tmp->type == 1)
                {
                    SetMFType(m_multifieldPtr,i,INTEGER);
                    SetMFValue(m_multifieldPtr,i,EnvAddLong(m_theEnv,tmp->data.i_value));
                }
                else if (tmp != NULL && tmp->type == 0)
                {
                    SetMFType(m_multifieldPtr,i,FLOAT);
                    SetMFValue(m_multifieldPtr,i,EnvAddDouble(m_theEnv,tmp->data.f_value));
                }
                else if (tmp != NULL && tmp->type == 2)
                {

                    SetMFType(m_multifieldPtr,i,SYMBOL);
                    SetMFValue(m_multifieldPtr,i,EnvAddSymbol(m_theEnv,((char*)tmp->data.s_value)));
                }

                else
                {
                    if(tmp == NULL)
                        cout<<"EntryPoint::insert::CreateMulti:FieldAndValue  is 0"<<endl;
                    else
                        cout<<"EntryPoint::insert::CreateMulti:parameter not int/float/string type, type is->"<<tmp->type<<endl;
                }
                i++;
                it++;
            }
            itList++;
    }
	SetpType(returnValuePtr,MULTIFIELD);
	SetpValue(returnValuePtr,m_multifieldPtr);
	
	SetpDOBegin(returnValuePtr,1);
    SetpDOEnd(returnValuePtr,i-1);
    return true;
}
void EntryPoint::flush(FLUSH_TYPE type)
{
	
}
