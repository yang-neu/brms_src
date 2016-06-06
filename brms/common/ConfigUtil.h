#ifndef _CONFIGUTIL_H_
#define _CONFIGUTIL_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <stdlib.h>

using namespace std;

class ConfigUtil
{
	private:
		map<string, string> items;
		
		ConfigUtil(const char *path);
		
		static ConfigUtil *instance;
		
	public:
		~ConfigUtil();
		
		string GetConfigStr(const char *key);
		int GetConfigInt(const char *key);
		
		static ConfigUtil *GetInstance(const char *path = NULL);
};

const struct {
    char *key;
    char *val;
} DEFAULT_SETTING[] = {
	{"simulator",				"0"},
	{"通信タイプ",				"INFO"},
	{"サーバーアドレス",			"192.168.1.10"},
	{"ポート番号",				"51000"},
	{"情報集約アドレス",			"127.0.0.1"},
	{"情報集約通知ポート",			"60100"},
	{"情報集約取得ポート",			"60010"},
	{"情報生成取得ポート",			"60030"},
	{"TimerLinerログ出力パス",	"../timelinerdata.txt"},
	{"CLIPSログ出力パス",			"../clips.log"},
	{"通信ログ出力パス",			""},
	{"履歴種別",					"TIME"},
	{"履歴長さ",					"5"},
};

#endif
