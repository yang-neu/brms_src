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

#include "../brms/ClipsSessionMgr.h"
#include "../brms/streaming/EntryPointOutput.h"

#include "../brms/common/ConfigUtil.h"

#if 0
#define MEASURE_START() { \
	struct timespec ts1, ts2; \
	clock_gettime(CLOCK_REALTIME, &ts1);

#define MEASURE_END(tag) \
	clock_gettime(CLOCK_REALTIME, &ts2); \
	printf("%10lld    <<%s>>\n", (long long)((ts2.tv_sec - ts1.tv_sec) * 1000000000L + (ts2.tv_nsec - ts1.tv_nsec)), #tag); \
}
#else
#define MEASURE_START()
#define MEASURE_END(tag)
#endif

#include "timeliner.hpp"

using namespace std;

Spliter::Spliter(const char *separator)
	: separator(separator)
{
}

vector<string> &Spliter::Split(string &contents)
{
	string line;
	size_t last_pos = 0;
	size_t pos = 0;
	
	while (last_pos < contents.length())
	{
		pos = contents.find(separator, last_pos);
		if (pos != string::npos)
		{
			line = string(contents, last_pos, pos - last_pos);
			last_pos = pos + separator.length();
		}
		else
		{
			line = string(contents, last_pos);
			last_pos = contents.length();
		}
		
		items.push_back(line);
	}
	
	return items;
}

Encoder::Encoder(const char *fromcode, const char *tocode)
	: fromcode(fromcode), tocode(tocode)
{
	cd = iconv_open(tocode, fromcode);
	if (cd == (iconv_t)-1)
	{
		cout << "iconv_open error(" << errno << "):" << strerror(errno) << endl;
	}
}

Encoder::~Encoder()
{
	iconv_close(cd);
}

string Encoder::Encode(string &contents)
{
	const char *indata = contents.data();
	size_t inbytes = contents.length();
	size_t outbytes = inbytes * 3;
	char *outdata = new char[outbytes];
	
	char *inbuf = (char *)indata;
	char *outbuf = (char *)&outdata[0];
	size_t inbytesleft = inbytes;
	size_t outbytesleft = outbytes;
	
	size_t count = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
	if (inbytesleft > 0)
	{
		cout << "iconv error(" << count << " <> " << inbytes << " " << errno << "):" << strerror(errno) << endl;
	}
	
	string str(outdata, outbytes - outbytesleft);
	delete[] outdata;
	
	return str;
}

EncodeFile::EncodeFile(const char *filename, const char *inencode, const char *outencode, ios_base::openmode mode)
	: filename(filename), inencode(inencode), outencode(outencode),
	  inencoder(outencode, inencode), outencoder(inencode, outencode)
{
	if (this->filename.length() > 0)
	{
		fs.exceptions(fstream::failbit | fstream::badbit);
		fs.open(filename, mode);
	}
}

EncodeFile::~EncodeFile()
{
	fs.close();
}

string EncodeFile::Read()
{
	fs.seekg(0, fs.end);
	int length = fs.tellg();
	fs.seekg(0, fs.beg);
	
	char *buffer = new char[length];
	fs.read(buffer, length);
	
	string str(buffer, length);
	delete[] buffer;
	
	return inencoder.Encode(str);
}

void EncodeFile::Write(string &contents)
{
	string str = outencoder.Encode(contents);
	fs.write(str.data(), str.length());
}

void EncodeFile::Flush()
{
	fs.flush();
}

bool ConfigItem::ParseConfigItem(string &line)
{
	vector<string> items = Spliter(",").Split(line);
	if (items.size() < 8 || items[0].compare("No") == 0 || atoi(items[0].c_str()) <= 0)
	{
		return false;
	}
	
	No = atoi(items[0].c_str());
	Member = items[1];
	Comment = items[2];
	Type = items[3];
	Method = items[4];
	Kind = items[5];
	Calculate = items[6];
	Output = items[7].compare("レ") == 0;
	Order = items.size() > 8 ? atoi(items[8].c_str()) : 0;
	Diagram = items.size() > 9 ? items[9].compare("レ") : false;
	Judge = items.size() > 10 ? items[10].compare("レ") : false;
	
	return true;
}

string ConfigItem::Format(string val, duk_context *ctx)
{
	if (Calculate.length() == 0 || ctx == NULL)
	{
		return val;
	}
	
	string command("val = '" + val + "';" + Calculate + "; '' + val;");
	MEASURE_START();
	duk_eval_string(ctx, command.c_str());
	MEASURE_END(duk_eval_string);
	return duk_get_string(ctx, -1);
}

Configuration::Configuration()
	: ctx(NULL)
{
}

Configuration::~Configuration()
{
//	if (ctx != NULL)
//	{
//		MEASURE_START();
//		duk_destroy_heap(ctx);
//		MEASURE_END(duk_destroy_heap);
//	}
}

bool Configuration::ParseConfiguration(string &contents)
{
//	if (ctx == NULL)
//	{
//		MEASURE_START();
//		ctx = duk_create_heap_default();
//		MEASURE_END(duk_create_heap_default);
//	}
	
	items.clear();

	vector<string> lines = Spliter("\n").Split(contents);
	vector<string>::iterator it;
	for (it = lines.begin(); it != lines.end(); it++)
	{
		ConfigItem item;
		if (!item.ParseConfigItem(*it))
		{
			continue;
		}
		
		items.push_back(item);
	}
	return true;
}

void Configuration::PrintHeader(EncodeFile &file)
{
	string line;
	list<ConfigItem>::iterator it;
	for (it = items.begin(); it != items.end(); it++)
	{
		if (it != items.begin())
		{
			line += ",";
		}
		line += it->Member;
	}
	line += "\n";
	file.Write(line);
}

void Configuration::PrintData(EncodeFile &file, void *pStream)
{
	string line;
	list<ConfigItem>::iterator it;
	
	EntryPointOutput *output = (EntryPointOutput *)pStream;
	ctx = duk_create_heap_default();
	for (it = items.begin(); it != items.end(); it++)
	{
		if (it != items.begin())
		{
			line += ",";
		}
		
		string val;
		if (it->Type.compare("double") == 0)
		{
			double value;
			char buf[32];
			output->getOutputValue(it->Member, value);
			sprintf(buf, "%f", value);
			val += buf;
		}
		else if (it->Type.compare("int") == 0)
		{
			int value;
			char buf[32];
			output->getOutputValue(it->Member, value);
			sprintf(buf, "%d", value);
			val += buf;
		}
		else if (it->Type.compare("long") == 0)
		{
			int value;
			char buf[32];
			output->getOutputValue(it->Member, value);
			sprintf(buf, "%d", value);
			val += buf;
		}
		else if (it->Type.compare("String") == 0)
		{
			output->getOutputValue(it->Member, val);
		}
		line += it->Format(val, ctx);
	}
	duk_destroy_heap(ctx);
	
	line += "\n";
	file.Write(line);
	file.Flush();
}

void *OutputStateThread::Thread(void *args)
{
	OutputStateThread *ost = (OutputStateThread *)args;
	
	string contents;
	try
	{
		EncodeFile configfile("../setting/Timeliner.conf", "utf-8", "utf-8", fstream::in | fstream::binary);
		contents = configfile.Read();
	}
	catch(...)
	{
		cout << "TimeLiner configfile open failed!" << endl;
		return NULL;
	}

	ost->conf.ParseConfiguration(contents);

	string lineno("time,");
	ost->file.Write(lineno);
	ost->conf.PrintHeader(ost->file);
	
#if 0
	int ret;
	struct sigevent event;
	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = args;
	event.sigev_notify_function = TimerFire;
	timer_t tid;
	ret = timer_create(CLOCK_REALTIME, &event, &tid);
	if (ret == -1)
	{
		cout << "timer_create error(" << errno << "):" << strerror(errno) << endl;
		return NULL;
	}
	
	struct itimerspec its = {0};
	its.it_interval.tv_sec = 1;
	its.it_interval.tv_nsec = 0;
	its.it_value.tv_sec = 1;
	its.it_value.tv_nsec = 0;
	ret = timer_settime(tid, 0, &its, NULL);
	if (ret == -1)
	{
		cout << "timer_settime error(" << errno << "):" << strerror(errno) << endl;
		return NULL;
	}

	pthread_mutex_lock(&ost->mutex);
	pthread_cond_wait(&ost->cond, &ost->mutex);
	pthread_mutex_unlock(&ost->mutex);

	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 0;
	ret = timer_settime(tid, 0, &its, NULL);
#else
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	int i = 0;
	while (1)
	{
		ts.tv_sec++;

		pthread_mutex_lock(&ost->mutex);
		int ret = pthread_cond_timedwait(&ost->cond, &ost->mutex, &ts);
		pthread_mutex_unlock(&ost->mutex);
		if (ret == 0)
		{
			break;
		}

		ClipsSession *session = ClipsSessionMgr::get()->getSession(ost->m_sessionID);
		if (session->getState() != ClipsSession::SESSIONSTATE_RECVING)
		{
			continue;
		}

		EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(session->getEntryPoint("Data Output Stream"));
		char buffer[32];
		sprintf(buffer, "%d,", ++i);
		string lineno(buffer);
		ost->file.Write(lineno);
		ost->conf.PrintData(ost->file, pStream);
	}
#endif
	
	return NULL;
}

#if 0
void OutputStateThread::TimerFire(union sigval sigv)
{
	OutputStateThread *ost = (OutputStateThread *)sigv.sival_ptr;
	
	// TODO: BRMS
	//ClipsSessionMgr *pmgr = ClipsSessionMgr::get();
	//EntryPointOutput *pStream = dynamic_cast<EntryPointOutput *>(pmgr->getSession(ost->m_sessionID)->getEntryPoint("Data Output Stream"));
	//ost->conf.PrintData(ost->file, pStream);
	
	ost->conf.PrintData(ost->file, NULL);
}
#endif

OutputStateThread::OutputStateThread(string &sessionID, string logpath)
	: m_sessionID(sessionID),
	file(logpath.c_str(), "utf-8", "shift-jis", fstream::out | fstream::binary | fstream::trunc)
{
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
}

OutputStateThread::~OutputStateThread(void)
{
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

void OutputStateThread::Start(void)
{
	
	if (pthread_create(&pid, NULL, Thread, this) != 0)
	{
		cout << "pthread_create error!" << endl;
	}
}

void OutputStateThread::Stop(void)
{
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	
	pthread_join(pid, NULL);
}
#if 0
int main(int argc, const char *argv[])
{
	cout << "main ..." << endl;
	
	string sessionID;
	OutputStateThread ost(sessionID);
	ost.Start();
	
	usleep(3100000);
	
	ost.Stop();
	
	return 0;
}
#endif
