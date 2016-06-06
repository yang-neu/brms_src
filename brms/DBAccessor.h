#ifndef _H_DB_ACCESSOR_

#define _H_DB_ACCESSOR_

#include <map>
#include <string>
#include<iostream>
#include <string.h>
#include <sys/time.h>
#include "Table.h"
#include "RowType.h"
#include "CompactRowType.h"
#include "IndexType.h"
#include "FifoIndexType.h"
#include "Rhref.h"
#include "./common/TimeUtil.h"
#include "./common/SysTimer.h"

using std:: map;
using std:: string;
using std:: pair;
using namespace TRICEPS_NS;

#define		EVENT_FILED_NAME_START_TIME		"start_time"
#define		EVENT_FILED_NAME_END_TIME		"end_time"

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

typedef struct {
	double start_time;
	double end_time;
}SectionTime;

enum IntervalKind {
	AFTER_CLOSED_INTERVAL = 0,  			// after:  [start, end]
	AFTER_OPEN_INTERVAL,					// after:  (start, end)
	AFTER_LEFT_CLOSED_RIGHT_OPEN_INTERVAL,	// after:  [start, end)
	AFTER_LEFT_OPEN_RIGHT_CLOSED_INTERVAL,	// after:  (start, end]
	AFTER_LEFT_CLOSED_RIGHT_INFINITE_INTERVAL,//after: [start, +｡ﾞ)
	AFTER_LEFT_OPEN_RIGHT_INFINITE_INTERVAL, //after: (start, +｡ﾞ)
	AFTER_MAX,
	BEFORE_CLOSED_INTERVAL,  				// before:  [start, end]
	BEFORE_OPEN_INTERVAL,					// before:  (start, end)
	BEFORE_LEFT_CLOSED_RIGHT_OPEN_INTERVAL,	// before:  [start, end)
	BEFORE_LEFT_OPEN_RIGHT_CLOSED_INTERVAL,	// before:  (start, end]
	BEFORE_LEFT_CLOSED_RIGHT_INFINITE_INTERVAL,//before: [start, +｡ﾞ)
	BEFORE_LEFT_OPEN_RIGHT_INFINITE_INTERVAL, //before: (start, +｡ﾞ)
	BEFORE_MAX,
};
enum LimtedType {
	NO_LIMITED = 0,
	LIMITED_BY_TIME,
	LIMITED_BY_NUMBER,
};
	
union LimitedValue {
	int	  number;  		//the number limit of row
	double time;	   //the time limit of row
};
typedef struct {
	enum IntervalKind kind;		//IntervalKind
	double start;			//start of interval 
	double end;      		//end of interval: invaild if kind is AFTER_LEFT_CLOSED_RIGHT_INFINITE_INTERVAL/AFTER_LEFT_OPEN_RIGHT_INFINITE_INTERVAL/BEFORE_LEFT_CLOSED_RIGHT_INFINITE_INTERVAL/BEFORE_LEFT_OPEN_RIGHT_INFINITE_INTERVAL
}Interval;

enum ConditionKind
{
	DURING_NO_TIME_LIMITATION = 0,	//base.start_time < QueryEvent.start_time <= QueryEvent.end_time < base.end_time
	DURING_ONE_TIME_LIMITATION,		//(QueryEvent.start_time - base.start_time｣ｩ<= time[0]  && (base.end_time - QueryEvent.end_time) <= time[0]
	DURING_TWO_TIME_LIMITATION,		//time[0] <= (QueryEvent.start_time - base.start_time｣ｩ<= time[1]  && time[0] <= (base.end_time - QueryEvent.end_time) <= time[1]
	DURING_FOUR_TIME_LIMITATION,	//time[0] <= (QueryEvent.start_time - base.start_time｣ｩ<= time[1]  && time[2] <= (base.end_time - QueryEvent.end_time) <= time[3]
//liuxin add
	COINCIDES_NO_TIME_LIMITATION,   //base.start_time == QueryEvent.start_time <= QueryEvent.end_time == base.end_time
	COINCIDES_ONE_TIME_LIMITATION,  //abs( QueryEvent.start_time - base.start_time ) <= time[0] && abs( QueryEvent.end_time - base.end_time ) <= time[0]
	COINCIDES_TWO_TIME_LIMITATION,  //abs( QueryEvent.start_time - base.start_time ) <= time[0] && abs( QueryEvent.end_time - base.end_time ) <= time[1]
	FINISHES_NO_TIME_LIMITATION,    //base.start_time < QueryEvent.start_time && QueryEvent.end_time == base.end_time
	FINISHIES_ONE_TIME_LIMITATION,	//base.start_time < QueryEvent.start_time && abs( QueryEvent.end_time - base.end_time ) <= time[0]
	//zhcheng add
	FINISHEDBY_NO_TIME_LIMITATION,
	FINISHEDBY_ONE_TIME_LIMITATION,
	INCLUDES_NO_TIME_LIMITATION,
	INCLUDES_ONE_TIME_LIMITATION,
	INCLUDES_TWO_TIME_LIMITATION,
	INCLUDES_FOUR_TIME_LIMITATION,
	MEETS_NO_TIME_LIMITATION,
	MEETS_ONE_TIME_LIMITATION,	
	//yintf add
	METBY_NO_TIME_LIMITATION,    //abs( QueryEvent.start_time - base.end_time ) == 0
	METBY_ONE_TIME_LIMITATION,   //abs( QueryEvent.start_time - base.end_time) <= time[0]
	OVERLAPS_NO_TIME_LIMITATION,  //QueryEvent.start_time < base.start_time < QueryEvent.end_time < base.end_time
	OVERLAPS_ONE_TIME_LIMITATION, //QueryEvent.start_time < base.start_time < QueryEvent.end_time < base.end_time && 0 <= QueryEvent.end_time - base.start_time <= time[0]
	OVERLAPS_TWO_TIME_LIMITATION, //QueryEvent.start_time < base.start_time < QueryEvent.end_time < base.end_time && time[0] <= QueryEvent.end_time - base.start_time <= time[1]
	OVERLAPSBY_NO_TIME_LIMITATION, //base.start_time < QueryEvent.start_time < base.end_time < QueryEvent.end_time
	OVERLAPSBY_ONE_TIME_LIMITATION,//base.start_time < QueryEvent.start_time < base.end_time < QueryEvent.end_time && 0 <= base.end_time - QueryEvent.start_time <= time[0]	
	OVERLAPSBY_TWO_TIME_LIMITATION,//base.start_time < QueryEvent.start_time < base.end_time < QueryEvent.end_time && time[0] <= base.end_time - QueryEvent.start_time <= time[1]				
	//sunchl add
	STARTS_NO_TIME_LIMITATION,		//QueryEvent.start_time == base.start_time && QueryEvent.start_time  < base.start_time
	STARTS_ONE_TIME_LIMITATION,		//abs( QueryEvent.start_time - base.start_time ) <= 5s && QueryEvent.start_time  < base.start_time
	STARTEDBY_NO_TIME_LIMITATION,	//QueryEvent.start_time == base.start_time && QueryEvent.start_time  > base.start_time
	STARTEDBY_ONE_TIME_LIMITATION,	//abs( QueryEvent.start_time - base.start_time ) <= 5s && QueryEvent.start_time  > base.start_time
};


typedef struct {
	enum ConditionKind kind;		//ConditionKind
	double time[4];					//interval of time condition 
}QueryCondition;

//Define argument type for Initialize()
typedef map<string, vector<Field> > TemplateMap;

typedef vector<FieldAndValue>	FieldDataVec;
typedef vector< Onceref<Table> > TableVec;
typedef pair<LimtedType, LimitedValue> LimitedInfo;

typedef struct {
	string name;
	Table *table;
} TableInfo;

class DBAccessor{

private:


	DBAccessor()
	{
		//clear the map that stores tables 
		m_tables.clear();
		//clear the map that stores limted type of tables 
		m_linfo.clear();

		m_tinfo.clear();

		pthread_mutex_init(&m_lock, NULL);
	}
	
	void SetFieldDataVec(Table *t, RowHandle *iter, FieldDataVec &fdvec);
	Table *getTableByName(string name, LimitedInfo &linfo);

    static void timeLimitedTimeout(union sigval);
    
	//add by suncl
	static void timeLimitedTimeoutSection(union sigval);
	
    static timer_t m_timer_id;
    
    //add by suncl
    static timer_t m_timer_section_id;

	static DBAccessor *m_pInstance;

    static TableVec	m_tables;
	
    static vector<LimitedInfo>	m_linfo;

	static vector<TableInfo *>	m_tinfo;

	static pthread_mutex_t m_lock;
public:
	//Creat instance only once
	static DBAccessor * GetInstance()
	{
		if ( NULL == m_pInstance )
		{
			m_pInstance = new DBAccessor();
		}
		
		return m_pInstance;
	}
	
	//Creat tables according to templates
	void Initialize(TemplateMap &tm, LimitedInfo linfo);
	
	//Get specificed field data of specific EStream
	//EStream: name of stream
	//key: name for query key(time); "": invaild
	//base:  0: get latest event; >0: the time of base event;
	//offset: 0: invaild;  <0: before;  >0: after;
	FieldDataVec GetFieldData(string EStream, string key, double base, double offset);
	
	//Get specificed records of specific EStream
	//name: name of stream
	//key: name for query key(time); "": invaild
	//base:  0: get latest event; >0: the time of base event;
	//intval: specificed inverval; invaild if base is 0;
	vector< FieldDataVec > GetMultiFieldDataVec(string name, string key, double base, Interval intval);
	
	//get section events
	vector< FieldDataVec > GetMultiSectionEvents(string name, SectionTime base, QueryCondition qcond);
	
	//Insert field data to specific EStream
	int	InsertFieldData(string EStream, FieldDataVec &v_fd);
	
	//Insert Section data to specific EStream add by sunchl
	int	InsertFieldDataSection(string EStream, FieldDataVec &v_fd);
	//liuxin add
	vector< FieldDataVec > TimeQuery_DURING_NO_TIME(Table *t, SectionTime &base);
	vector< FieldDataVec > TimeQuery_DURING(Table *t, SectionTime &base, QueryCondition &qcond);
	vector< FieldDataVec > TimeQuery_FINISHES(Table *t, SectionTime &base, QueryCondition &qcond);
	vector< FieldDataVec > TimeQuery_COINCIDES(Table *t, SectionTime &base, QueryCondition &qcond);
	//add by zhang cheng
	vector< FieldDataVec > TimeQuery_FINISHEDBY(Table *t, SectionTime &base, QueryCondition &qcond);
	vector< FieldDataVec > TimeQuery_INCLUDES(Table *t, SectionTime &base, QueryCondition &qcond);
	vector< FieldDataVec > TimeQuery_MEETS(Table *t, SectionTime &base, QueryCondition &qcond);
	//add by yintf
	vector< FieldDataVec > TimeQuery_METBY(Table *t, SectionTime &base, QueryCondition &qcond);
	vector< FieldDataVec > TimeQuery_OVERLAPS(Table *t, SectionTime &base, QueryCondition &qcond);
	vector< FieldDataVec > TimeQuery_OVERLAPPEDBY(Table *t, SectionTime &base, QueryCondition &qcond);
	
	//Query Starts add by sunchl
	vector< FieldDataVec > TimeQuery_STARTS(Table *t, SectionTime &base, QueryCondition &qcond);
	//Query Startedby add by sunchl
	vector< FieldDataVec > TimeQuery_STARTEDBY(Table *t, SectionTime &base, QueryCondition &qcond);
	
};

#endif
