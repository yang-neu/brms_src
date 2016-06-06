#ifndef _H_STRING_UTIL_
#define _H_STRING_UTIL_


#include <iostream>
#include <vector>
using namespace std;
	
extern string trim(const string& str);


extern int split(const string& str, vector<string>& ret_, string sep = ",");
 


extern string replace(const string& str, const string& src, const string& dest);

extern int htoi(const char *s) ;
extern string itoh(int n);
#endif
