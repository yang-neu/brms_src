#ifndef _H_ENTRY_POINT_Common_Data_

#define _H_ENTRY_POINT_Common_Data_
extern "C"
{
	#include "clips.h"
}

#include <map>
#include <string>
#include<iostream>
#include <string.h>
#include "EntryPoint.h"
#include "../DBAccessor.h"

using namespace std;

class EntryPointCommonData:public EntryPoint
{
public:
    enum FILE_OPERATION{
        SAVE = 0,
        LOAD
    };

    EntryPointCommonData();
	void updateAll();
	bool loadFactFile();
	bool saveFactToFile();
    bool accelFileOperation(FILE_OPERATION);
    FieldDataVec getCommonData();

private:
    static pthread_mutex_t m_mutex;
	
	const char* m_globalName;
	FieldDataVec v_fd;
	
};

#endif
