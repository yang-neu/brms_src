#ifndef _H_DB_ACCESSOR_

#define _H_DB_ACCESSOR_

#include "./common/SysTimer.h"
#include <string>
#include <vector>
#include <list>

using std:: string;
using std:: vector;

//Define name and type for filed st_Field
typedef struct {
	string name;
	int type;   //0:double;1:int;2:string
}Field;

//Define data for filed st_FieldAndVaule
typedef struct {
	string name;
	int type;
	union{
		int i_value;
		double f_value;
		char s_value[100];
	}data;
	
}FieldAndValue;

typedef vector<FieldAndValue>	FieldDataVec;


#endif
