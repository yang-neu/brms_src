#include "CANTransmitData.h"
	
CANTransmitData::CANTransmitData()
{

}

void CANTransmitData::addParseDefinition(CANParseDefinition parseDef)
{
	m_parseDef.push_back(parseDef);
}

list<CANParseDefinition> CANTransmitData::find(int canId)
{
	list<CANParseDefinition> findList ;
	m_defItor = m_parseDef.begin();
	while(m_defItor != m_parseDef.end())
	{
		if((*m_defItor).getCanId() == canId)
		{
			findList.push_back(*m_defItor);
		}
		m_defItor++;
	}
	
	return findList;
}

