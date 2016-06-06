#include <iostream>
#include <vector>
#include <stdio.h> 
#include <math.h>
#include <string.h>
using namespace std;
	
string trim(const string& str)
{    
	string::size_type pos = str.find_first_not_of(' ');    
	if (pos == string::npos)   
	{
	         return str;    
	}   
	string::size_type pos2 = str.find_last_not_of(' '); 
	if (pos2 != string::npos) 
	{     
	   return str.substr(pos, pos2 - pos + 1);  
	}  
	
	 return str.substr(pos);
}


int split(const string& str, vector<string>& ret_, string sep = ",")
{
    if (str.empty())    
    {        
    	return 0;   
     }    
    string tmp;    
    string::size_type pos_begin = str.find_first_not_of(sep);    
    string::size_type comma_pos = 0;    
    while (pos_begin != string::npos)    
    {        
    	comma_pos = str.find(sep, pos_begin);        
    	if (comma_pos != string::npos)        
    	{            
    		tmp = str.substr(pos_begin, comma_pos - pos_begin);            
    		pos_begin = comma_pos + sep.length();        
    	}        
    	else        
    	{            
    		tmp = str.substr(pos_begin);            
    		pos_begin = comma_pos;        
    	}        
    	if (!tmp.empty())        
    	{            
    		ret_.push_back(tmp);            
    		tmp.clear();        
    	}    
    }    
    return 0;
}


string replace(const string& str, const string& src, const string& dest)
{    
	string ret;    
	string::size_type pos_begin = 0;    
	string::size_type pos       = str.find(src);    
	while (pos != string::npos)    
	{        
        //cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
		ret.append(str.data() + pos_begin, pos - pos_begin);        
		ret += dest;        
		pos_begin = pos + 1;        
		pos       = str.find(src, pos_begin);    
	}    
	if (pos_begin < str.length())    
	{
		ret.append(str.begin() + pos_begin, str.end());   
	}
	return ret;
}
#if 0
 int hexchtoi(char hexch ) 
 {  
 	char phexch[] = "ABCDEF";  
 	char qhexch[] = "abcdef";  
 	int i;  
 	for(i=0;i<6;i++){  
 		 if((hexch == phexch[i]) || (hexch == qhexch[i]))
 		     break; 
 	}  
 	if(i >= 6)
 	{   
 		return 0; 
 	}  
 	return 10+i; 
 } 
 int htoi(const char *s) 
 {  
 	int n=0; 
 	int valu=1; 
 	int i=0,j;  
 	int answer=0;  
 	if((s[0] == '0') && ((s[1] == 'x') || (s[1] == 'X')))
 	{   
 		i += 2;  
 	}  
 	while((s[i] != '\0'))
 	{   
 		if((s[i] < '0') && (s[i] > '9'))
 		{    
 			if(hexchtoi(s[i]) == 0)
 			{     
 				valu=0;   
 			  	break;    
 			}   
 		}   
 		n++;   
 		i++;  
 	}  
	if(valu != 0)
	{      
		for(j=0;j<n;j++)
		{    
			answer += ((int)pow(16,j) * hexchtoi(s[i-j-1]));   
		}   
	}  
	else  
		answer = -1;  
 	return answer; 
 } 
 #else
 int htoi(const char *s)
 {
		int i = 0;
		int j = 0;
		int n = 0;
		int digit = 0;
		int len = strlen(s);
		if (len < 2)
			return 0;
		
		if(s[i]=='0')
		{
			  ++i;
			  if(s[i] == 'X' || s[i] == 'x')
			  	++i;
		}
		#if 0
		else 
		{
			return -1;
		}
		#endif
			
        for(j = 0;j < len-i;j++)
		{ 
		   	if(s[len-j-1]<='9'&&s[len-j-1]>='0')
		    	digit = s[len-j-1]-'0';
		   	else if(s[len-j-1]<='f'&&s[len-j-1]>='a')
		    	digit=s[len-j-1]-'a'+10;
		   	else if(s[len-j-1]>='A'&&s[len-j-1]<='F')
		     	digit=s[len-j-1]-'A'+10;
		   	else 
		     	break;
	      	//n=n*16+digit;
            n +=digit<<(j*4);
	  	}
	      return n;
}
char f(int n)
{    
	return "0123456789ABCDEF"[n];
}
string itoh(int n)
{
 	int i=0;    
 	char hex[20]="";    
 	while(n)    
 	{        
 		hex[i++]=f(n%16);        
 		n/=16;   
 	}    
 	string result = hex;
 	return result;
 }    
#endif
