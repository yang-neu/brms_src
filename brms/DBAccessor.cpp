#include <math.h>
#include "DBAccessor.h"

//initialize static member
DBAccessor * DBAccessor::m_pInstance = NULL;
timer_t DBAccessor::m_timer_id = 0;
timer_t DBAccessor::m_timer_section_id = 0;
TableVec DBAccessor::m_tables;
vector<LimitedInfo>	DBAccessor::m_linfo;
vector<TableInfo *>	DBAccessor::m_tinfo;
pthread_mutex_t DBAccessor::m_lock;

const float EPSILON = 0.00001;

//max number of a table
const int	RECORD_NUM_MAX = 300;
//Implenment for Initialize
void DBAccessor::Initialize(TemplateMap &tm, LimitedInfo linfo)
{
	size_t	i, num;
	TableType *tt;
	
	RowType::FieldVec fld;
	
	for (TemplateMap::iterator it = tm.begin(); it != tm.end(); it++)
	{
//		cout<<"DBAccessor::Initialize: loop...."<<endl;
		num = (*it).second.size();
//		cout<<"DBAccessor::Initialize: num:"<<num<<endl;
		fld.clear();
		//Add time stamp
//		fld.push_back(RowType::Field("Time", Type::r_int32));
		
		//Add field that user defined			
		for ( i = 0; i < num; i++ )
		{
			if ( 1 == (*it).second[i].type )
			{
				fld.push_back(RowType::Field((*it).second[i].name, Type::r_int32));
//				cout<<"DBAccessor::Initialize:(int) i:"<<i<<"name:"<<(*it).second[i].name<<endl;
			}
			else if ( 0 == (*it).second[i].type )
			{
				fld.push_back(RowType::Field((*it).second[i].name, Type::r_float64));
//				cout<<"DBAccessor::Initialize:(float) i:"<<i<<"name:"<<(*it).second[i].name<<endl;
			}
			else if ( 2 == (*it).second[i].type )
			{
				fld.push_back(RowType::Field((*it).second[i].name, Type::r_string));
			}
		}
		
		Unit *unit = new Unit((*it).first);
		
		Onceref<RowType> rt = new CompactRowType(fld);
		
		switch ( linfo.first )
		{
			case NO_LIMITED:
			case LIMITED_BY_TIME:
                 tt = TableType::make(rt)
                                ->addSubIndex("fifo", FifoIndexType::make())
                                ->addSubIndex("reverse", FifoIndexType::make()->setReverse(true));
				break;
			case LIMITED_BY_NUMBER:
                 tt = TableType::make(rt)
                                ->addSubIndex("fifo", FifoIndexType::make()->setLimit(linfo.second.number))
                                ->addSubIndex("reverse", FifoIndexType::make()->setLimit(linfo.second.number)->setReverse(true));
				break;
			default:
				break;
		}
		
		tt->initialize();
		
		Onceref<Table> t = tt->makeTable(unit, (*it).first);
		
		pthread_mutex_lock(&m_lock);
		m_tables.push_back(t);
		m_linfo.push_back(linfo);
		TableInfo *ti = new TableInfo();
		ti->name = (*it).first;
		ti->table = t.get();
		m_tinfo.push_back(ti);
		pthread_mutex_unlock(&m_lock);
	}
	
	return;
}

//Implenment for getTableByName
Table *DBAccessor::getTableByName(string name, LimitedInfo &linfo)
{
	vector<TableInfo *>::iterator it;
	int i = 0;

	for ( it=m_tinfo.begin(); it!=m_tinfo.end(); it++ )
	{
		if ( name == (*it)->name)
		{
			linfo.first = m_linfo[i].first;
			linfo.second = m_linfo[i].second;
			return (*it)->table;
		}
		i++;
	}
	
	return NULL;
}
	
//Implenment for GetFieldData
FieldDataVec DBAccessor::GetFieldData(string estream, string key, double base, double offset)
{
	FieldDataVec fdvec;
	FieldAndValue fd;
	RowHandle *iter;
	
	RowType::FieldVec fv; 
	RowType::FieldVec::iterator it;
    int fdidx;
	intptr_t len;
	const char *data = NULL;
	double	field_time;
    LimitedInfo limited_info;
	fdvec.clear();
	
	pthread_mutex_lock(&m_lock);
	Table *t = getTableByName(estream, limited_info);

	IndexType *revidx = t->getType()->findSubIndex("reverse");
	IndexType *fifoidx = t->getType()->findSubIndex("fifo");
	
	//default return latest field data	
	iter = t->beginIdx(revidx);
	if ( iter != NULL )
	{
		//base is vaild
		if ( !((base <= EPSILON) && (base >= -EPSILON)) )
		{
			if ( offset < -EPSILON )
			{
				fdidx = t->getRowType()->findIdx(key);
				
				while (1){
					
					data = (const char *)&field_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx, data, len);
					
					field_time = *((double *)data);
					
					if ( (base + offset) >= field_time)
					{
						break;
					}
					
					iter = t->nextIdx(revidx, iter);			
					if ( iter == NULL )
					{
						pthread_mutex_unlock(&m_lock);
						return fdvec;
					}			
				}
			}
			
			if ( offset > EPSILON )
			{
				iter = t->beginIdx(fifoidx);
				
				fdidx = t->getRowType()->findIdx(key);
				
				while (1){
					
					data = (const char *)&field_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx, data, len);
					
					field_time = *((double *)data);
					
					if ( (base + offset) <= field_time)
					{
						break;
					}
					
					iter = t->nextIdx(fifoidx, iter);			
					if ( iter == NULL )
					{
						pthread_mutex_unlock(&m_lock);
						return fdvec;
					}			
				}
			}
		}
		
		SetFieldDataVec(t, iter, fdvec);
	}
	t = NULL;
	revidx = NULL;
	fifoidx = NULL;
	pthread_mutex_unlock(&m_lock);

	return fdvec;
}
void DBAccessor::SetFieldDataVec(Table *t, RowHandle *iter, FieldDataVec &fdvec)
{
	RowType::FieldVec fv; 
	FieldAndValue fd;
	RowType::FieldVec::iterator it;
	const char *data = NULL;
	intptr_t len;

	int i;
	
	fdvec.clear();
	//get all fields of the table
	fv = t->getRowType()->fields();

	i = 0;
	for ( it = fv.begin(); it != fv.end(); it++ )
	{
		//set field name 
		fd.name = (*it).name_;
		
		//set field type
		if ( (*it).type_ == Type::r_int32 )
		{
			fd.type = 1;
			
			data = (const char *)&(fd.data.i_value);
			//set field value
			t->getRowType()->getField(iter->getRow(), i, data, len);
			
			fd.data.i_value = *((int *)data);
		}
		else if ( (*it).type_ == Type::r_float64 )
		{
			fd.type = 0;

			data = (const char *)&(fd.data.f_value);
			//set field value
			t->getRowType()->getField(iter->getRow(), i, data, len);
			
			fd.data.f_value = *((double *)data);
		}
		else if ( (*it).type_ == Type::r_string )
		{
			fd.type = 2;

			memset(fd.data.s_value, 0x00, sizeof(fd.data.s_value));
			
			data = (const char *)fd.data.s_value;
			//set field value
			t->getRowType()->getField(iter->getRow(), i, data, len);
						
			memcpy(fd.data.s_value, data, strlen(data) + 1);
		}
		
		fdvec.push_back(fd);
	
		i++;
	}
	
	return;
}

//Implenment for InsertFieldData
int	DBAccessor::InsertFieldData(string estream, FieldDataVec &v_fd)
{
	FdataVec dv;
	FieldDataVec::iterator it;
	double cur_time;
    LimitedInfo limited_info;
	
	pthread_mutex_lock(&m_lock);
	//get table by stream name
	Table *t = getTableByName(estream, limited_info);
	
	//set data of row
	dv.clear();
	for ( it = v_fd.begin(); it != v_fd.end(); it++ )
	{
		if ( 1 == (*it).type )
		{
			dv.push_back(Fdata(true, &((*it).data.i_value), sizeof(int)));
            //cout<<"DBAccessor::InsertFieldData: name: "<<(*it).name<< "  type: int "<<"value: "<<(*it).data.i_value<<endl;
		}
		else if ( 0 == (*it).type )
		{
			if ( limited_info.first == LIMITED_BY_TIME )
			{
				//get time value
                if ( 0 == strcmp((*it).name.c_str(), "time") )
				{
					cur_time = GetSystemTime() / 1000.0;
					//time-limited
                    if ( (cur_time - (*it).data.f_value) >  limited_info.second.time)
					{
						dv.clear();
						cout<<std::fixed<<"DBAccessor::InsertFieldData: Failed (Time Expired Event)!! cur_tim:"<<cur_time<<" limited:"<<limited_info.second.time<<" field_time:"<<(*it).data.f_value<<endl;
						break;
					}
					else
					{
						if ( 0 == m_timer_id )
						{
							timerCreate(timeLimitedTimeout, 0, &m_timer_id);
							timerStart(&m_timer_id, (limited_info.second.time - (cur_time - (*it).data.f_value)) * 1000.0);
							cout<<"DBAccessor::InsertFieldData: Start Timer for event update!!!(" << m_timer_id << ")" <<endl;
						}
					}
				}
			}
			dv.push_back(Fdata(true, &((*it).data.f_value), sizeof(double)));
            //cout<<"DBAccessor::InsertFieldData: name: "<<(*it).name<< "  type: float "<<"value: "<<(*it).data.f_value<<endl;
		}
		else if ( 2 == (*it).type )
		{
			dv.push_back(Fdata(true, &((*it).data.s_value), strlen((*it).data.s_value) + 1));
            //cout<<"DBAccessor::InsertFieldData: name: "<<(*it).name<< "  type: string "<<"value: "<<(*it).data.s_value<<endl;
		}
	}
	if ( true != dv.empty() )
	{
		//creat row
		Rowref r(t->getRowType(),  t->getRowType()->makeRow(dv));
		
		//creat row handle for row
		Rhref rh(t, t->makeRowHandle(r));
		
		//insert row handle to table
		t->insert(rh);
	}
	t = NULL;
	pthread_mutex_unlock(&m_lock);

	return 0;
}
//Implenment for GetMultiFieldDataVec
vector< FieldDataVec > DBAccessor::GetMultiFieldDataVec(string name, string key, double base, Interval intval)
{
	vector< FieldDataVec > ret_vec;
	
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx;
	intptr_t len;
	const char *data = NULL;
	double	field_time;
    LimitedInfo limited_info;

	ret_vec.clear();
	
	pthread_mutex_lock(&m_lock);
	Table *t = getTableByName(name, limited_info);

	IndexType *revidx = t->getType()->findSubIndex("reverse");
	IndexType *fifoidx = t->getType()->findSubIndex("fifo");
	
	//default return latest field data	
	iter = t->beginIdx(revidx);
	
	if ( iter != NULL )
	{
		//base is vaild
		if ( !((base <= EPSILON) && (base >= -EPSILON)) )
		{
			if ( ( intval.kind >= BEFORE_CLOSED_INTERVAL) && (intval.kind < BEFORE_MAX) )
			{
				fdidx = t->getRowType()->findIdx(key);
				
				while (1){
					
					data = (const char *)&field_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx, data, len);
					
					field_time = *((double *)data);
					
					switch ( intval.kind )
					{
						case BEFORE_CLOSED_INTERVAL:
							if ( field_time <= (base - intval.start) )
							{
								if (field_time >= (base - intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
#if 0 // 2016/03/18 order records by time asc
									ret_vec.push_back(fdvec);
#else
									ret_vec.insert(ret_vec.begin(), fdvec);
#endif
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case BEFORE_OPEN_INTERVAL:
							if ( field_time < (base - intval.start) )
							{
								if (field_time > (base - intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}						
							break;
						case BEFORE_LEFT_CLOSED_RIGHT_OPEN_INTERVAL:
							if ( field_time <= (base - intval.start) )
							{
								if (field_time > (base - intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case BEFORE_LEFT_OPEN_RIGHT_CLOSED_INTERVAL:
							if ( field_time < (base - intval.start) )
							{
								if (field_time >= (base - intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case BEFORE_LEFT_CLOSED_RIGHT_INFINITE_INTERVAL:
							if ( field_time <= (base - intval.start) )
							{
								SetFieldDataVec(t, iter, fdvec);
									
								ret_vec.push_back(fdvec);
							}
							break;
						case BEFORE_LEFT_OPEN_RIGHT_INFINITE_INTERVAL:
							if ( field_time < (base - intval.start) )
							{
								SetFieldDataVec(t, iter, fdvec);
									
								ret_vec.push_back(fdvec);
							}
							break;
						default:
							cout<<"GetMultiFieldDataVec():: invalid input:intval.kind"<<intval.kind<<endl;
							break;
					}
					
					iter = t->nextIdx(revidx, iter);			
					if ( iter == NULL )
					{
						pthread_mutex_unlock(&m_lock);
						return ret_vec;
					}			
				}
			}
			
			if ( ( intval.kind >= AFTER_CLOSED_INTERVAL) && (intval.kind < AFTER_MAX) )
			{
				iter = t->beginIdx(fifoidx);
				
				fdidx = t->getRowType()->findIdx(key);
				
				while (1){
					
					data = (const char *)&field_time;
					
	                t->getRowType()->getField(iter->getRow(), fdidx, data, len);
					
					field_time = *((double *)data);
					
					switch ( intval.kind )
					{
						case AFTER_CLOSED_INTERVAL:
							if ( field_time >= (base + intval.start) )
							{
								if (field_time <= (base + intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case AFTER_OPEN_INTERVAL:
							if ( field_time > (base + intval.start) )
							{
								if (field_time < (base + intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case AFTER_LEFT_CLOSED_RIGHT_OPEN_INTERVAL:
							if ( field_time >= (base + intval.start) )
							{
								if (field_time < (base + intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case AFTER_LEFT_OPEN_RIGHT_CLOSED_INTERVAL:
							if ( field_time > (base + intval.start) )
							{
								if (field_time <= (base + intval.end) )
								{
									SetFieldDataVec(t, iter, fdvec);
									
									ret_vec.push_back(fdvec);
								}
								else
								{
									pthread_mutex_unlock(&m_lock);
									return ret_vec;
								}
							}
							break;
						case AFTER_LEFT_CLOSED_RIGHT_INFINITE_INTERVAL:
							if ( field_time >= (base + intval.start) )
							{
								SetFieldDataVec(t, iter, fdvec);
									
								ret_vec.push_back(fdvec);
							}
							break;
						case AFTER_LEFT_OPEN_RIGHT_INFINITE_INTERVAL:
							if ( field_time > (base + intval.start) )
							{
								SetFieldDataVec(t, iter, fdvec);
									
								ret_vec.push_back(fdvec);
							}
							break;
						default:
							cout<<"GetMultiFieldDataVec():: invalid input:intval.kind"<<intval.kind<<endl;
							break;
					}
					
					iter = t->nextIdx(fifoidx, iter);
					if ( iter == NULL )
					{
						pthread_mutex_unlock(&m_lock);
						return ret_vec;
					}			
				}
			}
		}
		else
		{
			SetFieldDataVec(t, iter, fdvec);
			
			ret_vec.push_back(fdvec);
		}
	}
	t = NULL;
	revidx = NULL;
	fifoidx = NULL;
	pthread_mutex_unlock(&m_lock);

	return ret_vec;
}

void DBAccessor::timeLimitedTimeout(union sigval)
{
	vector<TableInfo *>::iterator it;
	RowHandle *iter, *iter2;
    double cur_time, field_time, diff_time = 0.0;
	int fdidx;
	int flag = 0;
    int i = 0;
    const char *data = NULL;
    intptr_t    len;

	pthread_mutex_lock(&m_lock);
	it = m_tinfo.begin();
	diff_time = 0;

	//acess every table
	while( it!=m_tinfo.end() )
	{
		Table *t= (*it)->table;
		//limited-type of the table is time-limited
        if ( m_linfo[i].first ==  LIMITED_BY_TIME)
		{
			IndexType *fifoidx = t->getType()->findSubIndex("fifo");

			//default return oldest field data	
			iter = t->beginIdx(fifoidx);
			fdidx = t->getRowType()->findIdx("time");

			while ( iter != NULL )
			{
				t->getRowType()->getField(iter->getRow(), fdidx, data, len);
				field_time = *((double *)data);
				cur_time = GetSystemTime() / 1000.0;
				//if the row is not expired
                if ( (cur_time - field_time) <  m_linfo[i].second.time )
				{
                    if ( (0 == flag) || (diff_time > m_linfo[i].second.time - (cur_time - field_time)) )
					{
                        diff_time = m_linfo[i].second.time - (cur_time - field_time);
                        flag = 1;
					}
					break;
				}
				
				iter2 = iter;
				iter = t->nextIdx(fifoidx, iter);
				t->remove(iter2);
				//cout<<std::fixed<<"DBAccessor::timeLimitedTimeout: cur_time:"<<cur_time<<" field_time:"<<field_time<<endl;
			}
		}
		i++;
		it++;
	}
	pthread_mutex_unlock(&m_lock);
	if ( 0 != flag )
	{
		//cout<<std::fixed<<"DBAccessor::timeLimitedTimeout: diff_time:"<<diff_time<<endl;
		int expire = diff_time * 1000.0;
		timerStart(&m_timer_id, expire ? expire : 1);
	}
	else
	{
		timerEnd(&m_timer_id);
		m_timer_id = 0;
	}
}

//add by sun-chl
void DBAccessor::timeLimitedTimeoutSection(union sigval)
{
	vector<TableInfo *>::iterator it;
	RowHandle *iter, *iter2;
    double cur_time, section_end_time, diff_time = 0.0;
	int fdidx;
	int flag = 0;
    int i = 0;
    const char *data = NULL;
    intptr_t    len;
	
	it = m_tinfo.begin();

	diff_time = 0;
	
	//acess every table
	while( it!=m_tinfo.end() )
	{
		Table *t = (*it)->table;
		//limited-type of the table is time-limited
        if ( m_linfo[i].first ==  LIMITED_BY_TIME)
		{
			IndexType *fifoidx = t->getType()->findSubIndex("fifo");
	
			//default return oldest field data	
			iter = t->beginIdx(fifoidx);
			fdidx = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);

			while ( iter != NULL )
			{
				
				data = (const char *)&section_end_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx, data, len);
				
				section_end_time = *((double *)data);
				cur_time = GetSystemTime() / 1000.0;
				//if the row is not expired
                if ( (cur_time - section_end_time) <  m_linfo[i].second.time )
				{
                    if ( (0 == flag) || (diff_time > m_linfo[i].second.time - (cur_time - section_end_time)) )
					{
                        diff_time = m_linfo[i].second.time - (cur_time - section_end_time);
                        flag = 1;
					}
					break;
				}
				
				iter2 = iter;
				iter = t->nextIdx(fifoidx, iter);
				t->remove(iter2);
                cout<<std::fixed<<"DBAccessor::timeLimitedTimeoutSection: cur_time:"<<cur_time<<" section_end_time:"<<section_end_time<<endl;
			}
		}
		i++;
		it++;
	}
	if ( 0 != flag )
	{
        cout<<std::fixed<<"DBAccessor::timeLimitedTimeoutSection: diff_time:"<<diff_time<<endl;
		timerStart(&m_timer_section_id, diff_time * 1000.0);
	}
	else
	{
		timerEnd(&m_timer_section_id);
		m_timer_section_id = 0;
	}
}

//Insert Section data to specific EStream add by sunchl
int	DBAccessor::InsertFieldDataSection(string estream, FieldDataVec &v_fd)
{
	FdataVec dv;
	FieldDataVec::iterator it;
	double cur_time;
    LimitedInfo limited_info;
	
	//get table by stream name
	Table *t = getTableByName(estream, limited_info);
	
	//set data of row
	dv.clear();
	for ( it = v_fd.begin(); it != v_fd.end(); it++ )
	{
		if ( 1 == (*it).type )
		{
			dv.push_back(Fdata(true, &((*it).data.i_value), sizeof(int)));
            //cout<<"DBAccessor::InsertFieldDataSection: name: "<<(*it).name<< "  type: int "<<"value: "<<(*it).data.i_value<<endl;
		}
		else if ( 0 == (*it).type )
		{
			if ( limited_info.first == LIMITED_BY_TIME )
			{
				//get time value
                if ((*it).name == EVENT_FILED_NAME_END_TIME)
				{
					cur_time = GetSystemTime() / 1000.0;
					//time-limited
                    if ( (cur_time - (*it).data.f_value) >  limited_info.second.time)
					{
						dv.clear();
						cout<<std::fixed<<"DBAccessor::InsertFieldDataSection: Section (Time Expired Event)!! cur_tim:"<<cur_time<<" limited:"<<limited_info.second.time<<" section_end_time:"<<(*it).data.f_value<<endl;
						break;
					}
					else
					{
						if ( 0 == m_timer_section_id )
						{
							timerCreate(timeLimitedTimeoutSection, 0, &m_timer_section_id);
							timerStart(&m_timer_section_id, (limited_info.second.time - (cur_time - (*it).data.f_value)) * 1000.0);
	                        cout<<"DBAccessor::InsertFieldDataSection: Start Timer for event update!!!"<<endl;
						}
					}
				}
			}
			dv.push_back(Fdata(true, &((*it).data.f_value), sizeof(double)));
            cout<<"DBAccessor::InsertFieldDataSection: name: "<<(*it).name<< "  type: double "<<"value: "<<std::fixed<<(*it).data.f_value<<endl;
		}
		else if ( 2 == (*it).type )
		{
			dv.push_back(Fdata(true, &((*it).data.s_value), strlen((*it).data.s_value) + 1));
            cout<<"DBAccessor::InsertFieldDataSection: name: "<<(*it).name<< "  type: string "<<"value: "<<(*it).data.s_value<<endl;
		}
	}
	if ( true != dv.empty() )
	{
		//creat row
		Rowref r(t->getRowType(),  t->getRowType()->makeRow(dv));
		//creat row handle for row
		Rhref rh(t, t->makeRowHandle(r));
		//insert row handle to table
		t->insert(rh);
	}

	return 0;
}

vector< FieldDataVec > DBAccessor::GetMultiSectionEvents(string name, SectionTime base, QueryCondition qcond)
{
	vector< FieldDataVec > ret_vec;
	LimitedInfo limited_info;
	ret_vec.clear();
	
	Table *t = getTableByName(name, limited_info);
    	
	switch ( qcond.kind )
	{
		case DURING_NO_TIME_LIMITATION:
			ret_vec = TimeQuery_DURING_NO_TIME(t, base);
			break;
		case DURING_ONE_TIME_LIMITATION:
			qcond.time[3] = qcond.time[0];
			ret_vec = TimeQuery_DURING(t, base, qcond);
			break;
		case DURING_TWO_TIME_LIMITATION:
			qcond.time[2] = qcond.time[0];
			qcond.time[3] = qcond.time[1];
			ret_vec = TimeQuery_DURING(t, base, qcond);
			break;
		case DURING_FOUR_TIME_LIMITATION:
			ret_vec = TimeQuery_DURING(t, base, qcond);
			break;
		case FINISHES_NO_TIME_LIMITATION:
			qcond.time[0] = 0.0;
			ret_vec = TimeQuery_FINISHES(t, base, qcond);
			break;
		case FINISHIES_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_FINISHES(t, base, qcond);
			break;
		case COINCIDES_NO_TIME_LIMITATION:
			qcond.time[0] = 0.0;
			qcond.time[1] = 0.0;
			ret_vec = TimeQuery_COINCIDES(t, base, qcond);
			break;
		case COINCIDES_ONE_TIME_LIMITATION:
			qcond.time[1] = qcond.time[0];
			ret_vec = TimeQuery_COINCIDES(t, base, qcond);
			break;
		case COINCIDES_TWO_TIME_LIMITATION:
			ret_vec = TimeQuery_COINCIDES(t, base, qcond);
			break;
		case FINISHEDBY_NO_TIME_LIMITATION:
			//qcond.time[0] = 0.0;
			ret_vec = TimeQuery_FINISHEDBY(t,base,qcond);
			break;
		case FINISHEDBY_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_FINISHEDBY(t,base,qcond);
			break;
		case INCLUDES_NO_TIME_LIMITATION:
			//qcond.time[0] = 0.0;
			ret_vec = TimeQuery_INCLUDES(t,base,qcond);
			break;
		case INCLUDES_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_INCLUDES(t,base,qcond);
			break;
		case INCLUDES_TWO_TIME_LIMITATION:
			ret_vec = TimeQuery_INCLUDES(t,base,qcond);
			break;
		case INCLUDES_FOUR_TIME_LIMITATION:
			ret_vec = TimeQuery_INCLUDES(t,base,qcond);
			break;
		case MEETS_NO_TIME_LIMITATION:
			//qcond.time[0] = 0.0;
			ret_vec = TimeQuery_MEETS(t,base,qcond);
			break;
		case MEETS_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_MEETS(t,base,qcond);
			break;
		case METBY_NO_TIME_LIMITATION:
			qcond.time[0] = 0.0;
			ret_vec = TimeQuery_METBY(t, base, qcond);	
			break;
		case METBY_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_METBY(t, base, qcond);			
			break;			
		case OVERLAPS_NO_TIME_LIMITATION:
			ret_vec = TimeQuery_OVERLAPS(t, base, qcond);
			break;
		case OVERLAPS_ONE_TIME_LIMITATION:
			qcond.time[1] = qcond.time[0];
			qcond.time[0] = 0.0;	
			ret_vec = TimeQuery_OVERLAPS(t, base, qcond);
			break;
		case OVERLAPS_TWO_TIME_LIMITATION:
			ret_vec = TimeQuery_OVERLAPS(t, base, qcond);
			break;			
		case OVERLAPSBY_NO_TIME_LIMITATION:	
			ret_vec = TimeQuery_OVERLAPPEDBY(t, base, qcond);	
			break;	
		case OVERLAPSBY_ONE_TIME_LIMITATION:
			qcond.time[1] = qcond.time[0];
			qcond.time[0] = 0.0;
			ret_vec = TimeQuery_OVERLAPPEDBY(t, base, qcond);
			break;		
		case OVERLAPSBY_TWO_TIME_LIMITATION:
			ret_vec = TimeQuery_OVERLAPPEDBY(t, base, qcond);
			break;
		case STARTS_NO_TIME_LIMITATION:
			qcond.time[0] = 0.0;
			ret_vec = TimeQuery_STARTS(t, base, qcond);
			break;
		case STARTS_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_STARTS(t, base, qcond);
			break;
		case STARTEDBY_NO_TIME_LIMITATION:
			qcond.time[0] = 0.0;
			ret_vec = TimeQuery_STARTEDBY(t, base, qcond);
			break;
		case STARTEDBY_ONE_TIME_LIMITATION:
			ret_vec = TimeQuery_STARTEDBY(t, base, qcond);
			break;
		default:
			cout<<"GetMultiSectionEvents():: invalid input:qcond.kind"<<qcond.kind<<endl;
			break;		
	}
	return ret_vec;
}

vector< FieldDataVec > DBAccessor::TimeQuery_DURING_NO_TIME(Table *t, SectionTime &base)
{
	vector< FieldDataVec > ret_vec;
	
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	ret_vec.clear();
	
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	
	iter = t->beginIdx(revidx);
	
	if ( iter == NULL )
	{
		return ret_vec;
	}
	
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	
	while (1)
	{
		data = (const char *)&field_start_time;
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
		field_start_time = *((double *)data);
		data = (const char *)&field_end_time;
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
		field_end_time = *((double *)data);
		
		if ( field_start_time > base.start_time && field_start_time <= field_end_time && field_end_time < base.end_time )
		{
			SetFieldDataVec(t, iter, fdvec);
			ret_vec.push_back(fdvec);
		}
		if ( field_end_time <= base.start_time )
		{
			return ret_vec;
		}
		
		iter = t->nextIdx(revidx, iter);
		if ( iter == NULL )
		{
			return ret_vec;
		}			
	}
}

vector< FieldDataVec > DBAccessor::TimeQuery_DURING(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	ret_vec.clear();
	
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	
	iter = t->beginIdx(revidx);
	
	if ( iter == NULL )
	{
		return ret_vec;
	}
	
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	
	while (1)
	{
		data = (const char *)&field_start_time;
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
		field_start_time = *((double *)data);
		data = (const char *)&field_end_time;
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
		field_end_time = *((double *)data);

		if ( DURING_ONE_TIME_LIMITATION == qcond.kind )
		{
			if ( (field_start_time > base.start_time) && ((field_start_time - base.start_time) <= qcond.time[0]) && (field_end_time < base.end_time) && ((base.end_time -  field_end_time) <= qcond.time[0]) )
			{
				SetFieldDataVec(t, iter, fdvec);
				ret_vec.push_back(fdvec);
			}
		}
		else
		{
			if ( ((field_start_time - base.start_time) >= qcond.time[0]) && ((field_start_time - base.start_time) <= qcond.time[1]) && ((base.end_time -  field_end_time) >= qcond.time[2]) && ((base.end_time -  field_end_time) <= qcond.time[3]))
			{
				SetFieldDataVec(t, iter, fdvec);
				ret_vec.push_back(fdvec);
			}
		}
		if ( (base.end_time - field_end_time) >  qcond.time[3])
		{
			return ret_vec;
		}
		
		iter = t->nextIdx(revidx, iter);
		if ( iter == NULL )
		{
			return ret_vec;
		}			
	}
}

vector< FieldDataVec > DBAccessor::TimeQuery_FINISHES(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	ret_vec.clear();
	
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	
	iter = t->beginIdx(revidx);
	
	if ( iter == NULL )
	{
		return ret_vec;
	}
	
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	
	while (1)
	{
		data = (const char *)&field_start_time;
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
		field_start_time = *((double *)data);
		data = (const char *)&field_end_time;
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
		field_end_time = *((double *)data);
		
		if ( (field_start_time > base.start_time) && (fabs(field_end_time - base.end_time) <= qcond.time[0]) )
		{
			SetFieldDataVec(t, iter, fdvec);
			ret_vec.push_back(fdvec);
		}
		
		if ( (base.end_time - field_end_time) >  qcond.time[0] )
		{
			return ret_vec;
		}
		
		iter = t->nextIdx(revidx, iter);
		if ( iter == NULL )
		{
			return ret_vec;
		}			
	}
}

vector< FieldDataVec > DBAccessor::TimeQuery_COINCIDES(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	ret_vec.clear();
	
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	
	iter = t->beginIdx(revidx);
	
	if ( iter == NULL )
	{
		return ret_vec;
	}
	
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	
	while (1)
	{
		data = (const char *)&field_start_time;
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
		field_start_time = *((double *)data);
		data = (const char *)&field_end_time;
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
		field_end_time = *((double *)data);
		
		if ( (fabs(field_start_time - base.start_time) <= qcond.time[0]) && (fabs(field_end_time - base.end_time) <= qcond.time[1]) )
		{
			SetFieldDataVec(t, iter, fdvec);
			ret_vec.push_back(fdvec);
		}
		
		if ( (base.end_time - field_end_time) > qcond.time[1] )
		{
			return ret_vec;
		}
		
		iter = t->nextIdx(revidx, iter);
		if ( iter == NULL )
		{
			return ret_vec;
		}			
	}
}

// add by zhang cheng finishedby includes meets
vector< FieldDataVec >DBAccessor::TimeQuery_FINISHEDBY(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;
	ret_vec.clear();
	//get index for last and first in sidelong(Table)
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	//get position for start_time and end_time in Row(Table)
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	switch ( qcond.kind )
	{
		case FINISHEDBY_NO_TIME_LIMITATION:
			for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
				{
					data = (const char *)&field_start_time;
				
					t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
				
					field_start_time = *((double *)data);
				
					data = (const char *)&field_end_time;
				
					t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
				
					field_end_time = *((double *)data);
					//$eventA.startTimestamp < $eventB.startTimestamp &&
					//$eventA.endTimestamp == $eventB.endTimestamp 
					
					if(field_start_time<base.start_time && field_end_time == base.end_time )
					{
							SetFieldDataVec(t, iter, fdvec);
							ret_vec.push_back(fdvec);
					}
					if(field_end_time < base.end_time)
					{
						return ret_vec;
					}
							 
				}
						
				break;
			case FINISHEDBY_ONE_TIME_LIMITATION:
				
				// finishedby one parameter
				//$eventA.startTimestamp < $eventB.startTimestamp &&
				//fabs( $eventA.endTimestamp - $eventB.endTimestamp ) <= 5s
				for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
				{
					data = (const char *)&field_start_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
					
					field_start_time = *((double *)data);
					
					data = (const char *)&field_end_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
					
					field_end_time = *((double *)data);
				
					if(field_start_time < base.start_time && fabs( field_end_time- base.end_time ) <= qcond.time[0])
					{						
						SetFieldDataVec(t, iter, fdvec);
						ret_vec.push_back(fdvec);
					}
					if(field_end_time<base.end_time - qcond.time[0])
					{
						return ret_vec;
					}

				}
					break;

	}
}
vector< FieldDataVec >DBAccessor::TimeQuery_INCLUDES(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;
	ret_vec.clear();
	//get index for last and first in sidelong(Table)
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	//get position for start_time and end_time in Row(Table)
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	switch ( qcond.kind )
	{
		case INCLUDES_NO_TIME_LIMITATION:
		//$eventA.startTimestamp < $eventB.startTimestamp <= 
		//$eventB.endTimestamp <$eventA.endTimestamp 
			for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
				{
					data = (const char *)&field_start_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
					
					field_start_time = *((double *)data);
					
					data = (const char *)&field_end_time;
					
					t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
					
					field_end_time = *((double *)data);
					//$eventA.startTimestamp < $eventB.startTimestamp <= $eventB.endTimestamp <$eventA.endTimestamp 
 					
					if(base.end_time < field_end_time && field_start_time < base.start_time 
						&& base.start_time <= base.end_time)
					{
						SetFieldDataVec(t, iter, fdvec);
						ret_vec.push_back(fdvec);
					}
					if(field_end_time<base.end_time)
					{
							return ret_vec;
					}
				}	
							
			break;
		case INCLUDES_ONE_TIME_LIMITATION:
			//includes one parameter
			//0 < $eventB.startTimestamp - $eventA.startTimestamp <= 5s &&
			//0 < $eventA.endTimestamp - $eventB.endTimestamp <= 5s	
			for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
			{
				data = (const char *)&field_start_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
				
				field_start_time = *((double *)data);
				
				data = (const char *)&field_end_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
				
				field_end_time = *((double *)data);
			
				if(0 < base.start_time - field_start_time && base.start_time - field_start_time <=qcond.time[0])
				{
					if(0 < field_end_time - base.end_time && field_end_time - base.end_time <= qcond.time[0])
					{
						SetFieldDataVec(t, iter, fdvec);
						ret_vec.push_back(fdvec);
					}

				}
				if(field_end_time < base.end_time)
				{
					return ret_vec;
				}
		
				
			}

				break;
		case INCLUDES_TWO_TIME_LIMITATION:
			//includes 
			//two parameter defined
			//5s <= $eventB.startTimestamp - $eventA.startTimestamp <= 10s &&
			//5s <= $eventA.endTimestamp - $eventB.endTimestamp <= 10s
			for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
			{
				data = (const char *)&field_start_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
				
				field_start_time = *((double *)data);
				
				data = (const char *)&field_end_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
				
				field_end_time = *((double *)data);
				if(qcond.time[0] <= base.start_time - field_start_time && base.start_time - field_start_time <=qcond.time[1])
				{
					if(qcond.time[0] <= field_end_time - base.end_time && field_end_time - base.end_time <=qcond.time[1])
					{
						SetFieldDataVec(t, iter, fdvec);
						ret_vec.push_back(fdvec);
					}
	
				}
				if(base.end_time > field_end_time - qcond.time[0])
				{
					return ret_vec;
				}
			}
			break;	
		case INCLUDES_FOUR_TIME_LIMITATION:
			//four parameter defined
			//includes 2 6 4 10
			//2s <= $eventB.startTimestamp - $eventA.startTimestamp <= 6s &&
			//4s <= $eventA.endTimestamp - $eventB.endTimestamp <= 10s
			for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
			{
				data = (const char *)&field_start_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
				
				field_start_time = *((double *)data);
				
				data = (const char *)&field_end_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
				
				field_end_time = *((double *)data);
				if( qcond.time[0]<=base.start_time - field_start_time && base.start_time - field_start_time <=qcond.time[1])
				{
					if(qcond.time[2] <= field_end_time - base.end_time && field_end_time - base.end_time <= qcond.time[3])
					{
						SetFieldDataVec(t, iter, fdvec);
						ret_vec.push_back(fdvec);
					}
				if(base.end_time > field_end_time - qcond.time[2] )
				{
					 return ret_vec;
				}

				}
			}
			break;
		}		
}

vector< FieldDataVec >DBAccessor::TimeQuery_MEETS(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;
	ret_vec.clear();
	//get index for last and first in sidelong(Table)
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	//get position for start_time and end_time in Row(Table)
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	switch ( qcond.kind )
		{
			case MEETS_NO_TIME_LIMITATION:
			//meets no parameter
            //fabs( $eventB.startTimestamp - $eventA.endTimestamp ) == 0
			for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
			{
				data = (const char *)&field_start_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
				
				field_start_time = *((double *)data);
				
				data = (const char *)&field_end_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
				
				field_end_time = *((double *)data);
				if(fabs(base.start_time - field_end_time)==0)
				{
					SetFieldDataVec(t, iter, fdvec);
					ret_vec.push_back(fdvec);
				}
				if(field_end_time < base.start_time)
				{
					return ret_vec;
				}
			}	
			break;
			case MEETS_ONE_TIME_LIMITATION:
			// meet one parameter
            //fabs( $eventB.startTimestamp - $eventA.endTimestamp) <= 5s
			for(iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
			{
				data = (const char *)&field_start_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
				
				field_start_time = *((double *)data);
				
				data = (const char *)&field_end_time;
				
				t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
				
				field_end_time = *((double *)data);
                if(fabs(base.start_time - field_end_time)<=qcond.time[0])
				{
					SetFieldDataVec(t, iter, fdvec);
					ret_vec.push_back(fdvec);
				}
				if(base.start_time>field_start_time+qcond.time[0])
				{
					return ret_vec;
				}
			}
				break;
		}

}

vector< FieldDataVec >DBAccessor::TimeQuery_METBY(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	RowHandle *iter;	
	FieldDataVec fdvec;
	int fdidx_st;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	IndexType *revidx = t->getType()->findSubIndex("reverse");

	ret_vec.clear();

	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	
	for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
	{
					
		data = (const char *)&field_start_time;
		
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
		
		//field_start_time = *((double *)data);
  
        //fabs( $eventA.startTimestamp - $eventB.endTimestamp) <= 5s
        if (qcond.time[0] >= fabs(field_start_time - base.end_time))
		{
			SetFieldDataVec(t, iter, fdvec);
			ret_vec.push_back(fdvec);
		}
		if (field_start_time<base.end_time -qcond.time[0] )
		{
			return ret_vec;
		}
	}

}

vector< FieldDataVec >DBAccessor::TimeQuery_OVERLAPS(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	RowHandle *iter;	
	FieldDataVec fdvec;
	int fdidx_st, fdidx_end;	
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	IndexType *revidx = t->getType()->findSubIndex("reverse");

	ret_vec.clear();

	//default return latest field data	
	iter = t->beginIdx(revidx);
	if ( iter == NULL )
	{
		return ret_vec;
	}
	
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);

	while (1)
	{
					
		data = (const char *)&field_start_time;
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);
		field_start_time = *((double *)data);
		
		data = (const char *)&field_end_time;	
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
		field_end_time = *((double *)data);
		if (OVERLAPS_NO_TIME_LIMITATION == qcond.kind)
		{
			//$eventA.startTimestamp < $eventB.startTimestamp < $eventA.endTimestamp < $eventB.endTimestamp
			if ((field_start_time < base.start_time) && (base.start_time < field_end_time)
				&& (field_end_time < base.end_time))
			{
				SetFieldDataVec(t, iter, fdvec);
				ret_vec.push_back(fdvec);
			}
			if (base.start_time >= field_end_time)
			{
				return ret_vec;
			}
		}
		else
		{   
			if ((field_start_time < base.start_time) && (base.start_time < field_end_time)
			&& (field_end_time < base.end_time) && (qcond.time[0] <= (field_end_time - base.start_time))
			&& (qcond.time[1] >= (field_end_time - base.start_time)))
			{
				SetFieldDataVec(t, iter, fdvec);
				ret_vec.push_back(fdvec);
			}
			if ((field_start_time < base.start_time) && (qcond.time[0] > (field_end_time - base.start_time)))
			{
				return ret_vec;
			}
		}
		
		iter = t->nextIdx(revidx, iter);
		if ( iter == NULL )
		{
			return ret_vec;
		}			
	
	}

}
vector< FieldDataVec >DBAccessor::TimeQuery_OVERLAPPEDBY(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	RowHandle *iter;	
	FieldDataVec fdvec;
	int fdidx_st, fdidx_end;	
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;

	IndexType *revidx = t->getType()->findSubIndex("reverse");

	ret_vec.clear();

	//default return latest field data	
	iter = t->beginIdx(revidx);
	if ( iter == NULL )
	{
		return ret_vec;
	}
	
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	while (1)
	{					
		data = (const char *)&field_start_time;	
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);	
		field_start_time = *((double *)data);
		data = (const char *)&field_end_time;
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);
		field_end_time = *((double *)data);
		//$eventB.startTimestamp < $eventA.startTimestamp < $eventB.endTimestamp < $eventA.endTimestamp
		if (OVERLAPSBY_NO_TIME_LIMITATION == qcond.kind)
		{
			if ((base.start_time < field_start_time) && (field_start_time < base.end_time)
				&& (base.end_time < field_end_time))
			{
				SetFieldDataVec(t, iter, fdvec);
				ret_vec.push_back(fdvec);
			}
			if (base.end_time >= field_end_time)
			{
				return 	ret_vec;
			}
		}
		else
		{   
			if ((base.start_time < field_start_time) && (field_start_time < base.end_time)
				&& (base.end_time < field_end_time) && (qcond.time[0] <= (base.end_time - field_start_time))
				&& (qcond.time[1] >= (base.end_time - field_start_time)))
			{
				SetFieldDataVec(t, iter, fdvec);
				ret_vec.push_back(fdvec);
			}
			if (base.end_time >= field_end_time)
			{
				return 	ret_vec;
			}
		}
		
		iter = t->nextIdx(revidx, iter);
		if ( iter == NULL )
		{
			return ret_vec;
		}			
	
	}

}

//Query Starts add by sunchl
vector< FieldDataVec > DBAccessor::TimeQuery_STARTS(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;
	ret_vec.clear();
	//get index for last and first in sidelong(Table)
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	//get position for start_time and end_time in Row(Table)
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
	{					
		data = (const char *)&field_start_time;					
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);					
		field_start_time = *((double *)data);					
		data = (const char *)&field_end_time;					
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);					
		field_end_time = *((double *)data);						
        if ((fabs(field_start_time - base.start_time) <= qcond.time[0]) && (field_end_time < base.end_time))
		{
			SetFieldDataVec(t, iter, fdvec);
			ret_vec.push_back(fdvec);
			cout<<"TimeQuery_STARTS()::field_start_time="<<field_start_time<<"base.start_time"<<base.start_time<<"qcond.time[0]="<<qcond.time[0]<<endl;
		}
		if (field_end_time <= base.start_time - qcond.time[0])
		{
			cout<<"TimeQuery_STARTS():: field_end_time > base.start_time"<<"qcond.time[0]="<<qcond.time[0]<<endl;
			return ret_vec;
		}									
	}
}

//Query Startedby add by sunchl
vector< FieldDataVec > DBAccessor::TimeQuery_STARTEDBY(Table *t, SectionTime &base, QueryCondition &qcond)
{
	vector< FieldDataVec > ret_vec;
	FieldDataVec fdvec;
	RowHandle *iter;
	
	int fdidx_st, fdidx_end;
	intptr_t len;
	const char *data = NULL;
	double	field_start_time, field_end_time;
	ret_vec.clear();
	//get index for last and first in sidelong(Table)
	IndexType *revidx = t->getType()->findSubIndex("reverse");
	//get position for start_time and end_time in Row(Table)
	fdidx_st = t->getRowType()->findIdx(EVENT_FILED_NAME_START_TIME);
	fdidx_end = t->getRowType()->findIdx(EVENT_FILED_NAME_END_TIME);
	for (iter = t->beginIdx(revidx); iter; iter = t->nextIdx(revidx, iter))
	{					
		data = (const char *)&field_start_time;					
		t->getRowType()->getField(iter->getRow(), fdidx_st, data, len);					
		field_start_time = *((double *)data);					
		data = (const char *)&field_end_time;					
		t->getRowType()->getField(iter->getRow(), fdidx_end, data, len);					
		field_end_time = *((double *)data);					
        if ((fabs(field_start_time - base.start_time) <= qcond.time[0]) && (field_end_time > base.end_time))
		{
			SetFieldDataVec(t, iter, fdvec);
			ret_vec.push_back(fdvec);
			cout<<"TimeQuery_STARTEDBY()::field_start_time="<<field_start_time<<"base.start_time"<<base.start_time<<"qcond.time[0]="<<qcond.time[0]<<endl;
		}
		if (field_end_time <= base.end_time)
		{
			cout<<"TimeQuery_STARTEDBY():: field_end_time > base.start_time"<<"qcond.time[0]="<<qcond.time[0]<<endl;
			return ret_vec;
		}								
	}
}
