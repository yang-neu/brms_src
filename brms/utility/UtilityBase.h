#ifndef _UTILITY_BASE_H_
#define _UTILITY_BASE_H_

#include <string>

using namespace std;

class UtilityBase {
public:
	UtilityBase() :
		m_streamingName("Receiving Data Stream")
	  , m_streamingHName("Driving Hitory Stream")
	{};
	virtual ~UtilityBase() {}
	virtual void run()=0;
	virtual void updateValue(long long value) =0; 
protected:
	string m_sessionID;	
	const string m_streamingName;
	const string m_streamingHName;
};
#endif
