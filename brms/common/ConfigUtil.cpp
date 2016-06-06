#include "ConfigUtil.h"

ConfigUtil *ConfigUtil::instance = NULL;

ConfigUtil::ConfigUtil(const char *path)
{
	int i;
	for (i = 0; i < sizeof(DEFAULT_SETTING) / sizeof(DEFAULT_SETTING[0]); i++)
	{
        string key(DEFAULT_SETTING[i].key);
        string val(DEFAULT_SETTING[i].val);
        items.insert(pair<string, string>(key, val));
	}
	
	ifstream ifs(path);
	if (ifs.good())
	{
		char line[1024];
		char key[1024];
		char val[1024];
		while (!ifs.eof())
		{
			ifs.getline(line, sizeof(line));
			if (line[0] == '[')
			{
				continue;
			}
			for (i = 0; line[i]; i++)
			{
				if (line[i] == ';' || line[i] == '#')
				{
					line[i] = 0;
					break;
				}
			}
			for (i--; i >= 0; i--)
			{
				if (line[i] == ' ' || line[i] == '\t')
				{
					line[i] = 0;
				}
				else
				{
					break;
				}
			}
			if (!line[0])
			{
				continue;
			}

			string sline(line);
			istringstream issline(sline);
			
			issline.getline(key, sizeof(key), ':');
			string skey(key);
			if (skey.length() == 0)
			{
				continue;
			}
			
			issline.getline(val, sizeof(val), ':');
			string sval(val);
			if (sval.length() == 0)
			{
				//continue;
			}
			
			items.erase(skey);
			items.insert(pair<string, string>(skey, sval));
		}
	}
	ifs.close();
}

ConfigUtil::~ConfigUtil()
{
	items.clear();
}

string ConfigUtil::GetConfigStr(const char *key)
{
	return items[string(key)];
}

int ConfigUtil::GetConfigInt(const char *key)
{
	return atoi(items[string(key)].c_str());
}

ConfigUtil *ConfigUtil::GetInstance(const char *path)
{
	if (!instance)
	{
		if (path != NULL)
		{
			instance = new ConfigUtil(path);
		}
	}
	return instance;
}

int ___main(int argc, const char *argv[])
{
	ConfigUtil *config = ConfigUtil::GetInstance("setting.txt");
	
	cout << "情報集約アドレス   : " << ConfigUtil::GetInstance()->GetConfigStr("情報集約アドレス") << endl;
	cout << "情報集約通知ポート : " << ConfigUtil::GetInstance()->GetConfigInt("情報集約通知ポート") << endl;
	cout << "情報集約アドレス1  : " << ConfigUtil::GetInstance()->GetConfigStr("情報集約アドレス1") << endl;
	cout << "情報集約通知ポート1: " << ConfigUtil::GetInstance()->GetConfigInt("情報集約通知ポート1") << endl;
	
	delete(config);
}
