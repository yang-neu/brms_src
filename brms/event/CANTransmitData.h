#ifndef _CAN_TRANSMIT_DATA_H_
#define _CAN_TRANSMIT_DATA_H_
#include <string>
#include <map>
#include <list>
#include <string>
using namespace std;

class CANParseDefinition
{
public:
	enum UNIT_TYPE
	{
		degreeCelsius,
		M,
		MperSec,
		G,
		SeeNotes,
		degree,
		trip,
		msec,
		KMperHour,
		Nm,
		radperS,
		
	};
		 CANParseDefinition()
		{
		}
		int getId()
		{
			return m_id;
		}
		void setId(int id)
		{
			m_id = id;
		}
		int getCanId()
		{
			return m_canId;
		}
		void setCanId(int canId)
		{
			m_canId = canId;
		}
		void setDataPosByte(double dataPosByte)
		{
			m_dataPosByte = dataPosByte;
		}
		void setDataPosBit(int dataLength)
		{
			m_dataLength = dataLength;
		}
		void setDataName(string dataName)
		{
			m_dataName = dataName;
		}
		void setUnit(UNIT_TYPE unit)
		{
			m_unit = unit;
		}
		double getDataPosByte()
		{
			return m_dataPosByte;
		}
		int getDataLength()
		{
			return m_dataLength;
		}
		string getDataName()
		{
			return m_dataName;
		}
		UNIT_TYPE getUnit()
		{
			return m_unit;
		}
		string getModelClassName()
		{
			return m_modelClassName;
		}
		string getEntryPointName()
		{
			return m_entryPointName;
		}
		void setModelClassName(string modelClassName)
		{
			m_modelClassName = modelClassName;
		}
		void setEntryPointName(string entryPointName)
		{
			m_entryPointName = entryPointName;
		}
private:	
	int m_id;
	int m_canId;
	string m_dataName;
	double m_dataPosByte;
	int m_dataLength;
	UNIT_TYPE m_unit;
	string m_modelClassName;
	string m_entryPointName;

	
};
class CANTransmitData 
{
public:
	CANTransmitData();
	void addParseDefinition(CANParseDefinition parseDef);
	list<CANParseDefinition> find(int canId);
private:
	//Hashtable<Integer, string[]> CANIDtoModelandStreamData;
	//map<Integer, string[]> CANIDtoModelandStreamData;
	//static ArrayList<CANParseDefinition> parseDef;
	list<CANParseDefinition> m_parseDef;
	list<CANParseDefinition>::iterator m_defItor;
};
#endif