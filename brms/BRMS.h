#ifndef _H_BRMS_
#define _H_BRMS_

#include <string>
#include <vector>
#include <pthread.h>
#include "interface/BrmsInterface.h"
#include "DBAccessor.h"

using namespace std;
typedef struct {
	float speed;
	float astatus;
	float distance;
	int   roadclass;
}STInputSource;


//class RuleEngine;
//#ifdef _CAN_DEVICE_
class CANTransmitData;
class SocketReceiveThread;
//#else
class IntegrationGenerateClientThread;
class DecisionAppServerThread;
//#endif
#ifdef _DEBUG_INPUT_
class DebugThread;
#endif
class OutputStateThread;
class InputSource
{
public:
	InputSource(string &);
	static void *getData(void *para);
	const string m_InputTemplateName;
	string getSession() { return m_sessionID; }
	bool	isSimulator() {return sim;}
	void startUp();
    void stop();
    void switchInput(bool flag);
    void setSpeedDbg(double speed);
    void setDistanceDbg(double distance);
    void setAccelDbg(double accel);
    void setRoadKindDbg(int kind);


private:
	
	void startGetInputData();
	void readSettingFile();
	void readUserSpecificDataFile();
	void writeUserSpecificDataFile();
//#ifdef _CAN_DEVICE_
	void readCANTransmitDataFile();
//#endif
	string m_sessionID;
	pthread_t m_ThdId;

	double timeAverage;
	int stopCount;
	double nationalAverage;
	int portNo;
	bool sim;
    bool isDebug;
//#ifdef _CAN_DEVICE_
    CANTransmitData *CANtransData;
	SocketReceiveThread *canReceive;
//#else
    IntegrationGenerateClientThread * m_IGClient;
    DecisionAppServerThread* m_DAppServer;
//#endif
#ifdef _DEBUG_INPUT_
	DebugThread *debugThread;
#endif
};

class BRMS :public BrmsInterface
{
public:
	BRMS();

public:
    bool initialize(int argc,char *argv[], string &sessionID,  RouterInterface *router);
	//RuleEngine *getEngine() { return m_RuleEng; }
	
    //add by liusiping @ 2016/06/10
    virtual vector<FieldAndValue> getCommonData();

	//BrmsInterface implement
	//virtual int getSceneResult() { return 0;}
	virtual double getSpeed() ;
	virtual double getAccel() ;
	virtual double getDistance() ;
	virtual double getA() ;
	
	virtual double getTTC();
	virtual string getRoadClassSig();
	virtual string getRoadKind();
	virtual string getFrontCar();
	virtual string getDriveScene();

    //profile1
	virtual string getSuddenAccel();
	virtual string getSuddenAccelState();
	virtual int getSuddenAccelCount();

    //profile2
    virtual string getSuddenDecel() ;
    virtual string getSuddenDecelState() ;
    virtual int getSuddenDecelCount() ;

    //profile3
    virtual string getDecelStart() ;
    virtual string getDecelStartState();
    virtual int getDecelStartCount() ;

    //profile4
    virtual string getTTCRisk() ;
    virtual string getTTCRiskState();
    virtual int getTTCRiskCount() ;

    //profile5
    virtual string getDisOnExpressWay() ;
    virtual string getDisOnExpressWayState() ;
    virtual int getDisOnExpressWayCount() ;

    //profile6
    virtual string getDisOnOpenRoad();
    virtual string getDisOnOpenRoadState() ;
    virtual int getDisOnOpenRoadCount() ;

    //profile7
    virtual string getFollowStop();
    virtual string getFollowStopState() ;
    virtual int getFollowStopCount();

    virtual void offIG();
    virtual void onIG();
    virtual void dbgSwich(bool flag) ;

    virtual void setSpeedDbg(double speed);
    virtual void setDistanceDbg(double distance);
    virtual void setAccelDbg(double accel);
    virtual void setRoadKindDbg(int kind);
	//virtual int get
private:
	//RuleEngine *m_ruleEng;
    bool setupSession(string &sessionID);
    bool m_statusIG;
	InputSource *m_inputSource;
	string m_sessionID;
	OutputStateThread *m_timeliner;
};



#endif /* _H_BRMS_ */


