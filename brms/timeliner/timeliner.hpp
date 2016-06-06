#ifndef _TIMELINER_HPP_
#define _TIMELINER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>

#include <stdlib.h>
#include <iconv.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "../brms/thirdpart/duktape.h"

using namespace std;

class Spliter
{
	private:
		string separator;
		vector<string> items;
	
	public:
		Spliter(const char *separator);
		
		vector<string> &Split(string &contents);
};

class Encoder
{
	private:
		string fromcode;
		string tocode;
		
		iconv_t cd;
	
	public:
		Encoder(const char *fromcode, const char *tocode);
		
		~Encoder();
		
		string Encode(string &contents);
};

class EncodeFile
{
	private:
		string filename;
		string inencode;
		string outencode;
		Encoder inencoder;
		Encoder outencoder;
		
		fstream fs;
	
	public:
		EncodeFile(const char *filename, const char *inencode, const char *outencode, ios_base::openmode mode);
		
		~EncodeFile();
		
		string Read();
		
		void Write(string &contents);
		
		void Flush();
};

class ConfigItem
{
	public:
		int No;
		string Member;
		string Comment;
		string Type;
		string Method;
		string Kind;
		string Calculate;
		bool Output;
		int Order;
		bool Diagram;
		bool Judge;
		
		bool ParseConfigItem(string &line);
		
		string Format(string val, duk_context *ctx);
};

class Configuration
{
	public:
		list<ConfigItem> items;
		duk_context *ctx;
		
		Configuration();
		
		~Configuration();
		
		bool ParseConfiguration(string &contents);
		
		void PrintHeader(EncodeFile &file);
		
		void PrintData(EncodeFile &file, void *pStream);
};

class OutputStateThread
{
	private:
		pthread_t pid;
		pthread_cond_t cond;
		pthread_mutex_t mutex;

		string m_sessionID;

		EncodeFile file;
		Configuration conf;
		
		static void *Thread(void *args);
		
		static void TimerFire(union sigval sigv);
	
	public:
		OutputStateThread(string &sessionID, string logpath);
		
		~OutputStateThread(void);
		
		void Start(void);
		
		void Stop(void);
};
#endif
