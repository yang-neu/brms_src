

#ifndef _H_BRMS_INTERFACE_
#define _H_BRMS_INTERFACE_
#include <iostream>
#include <list>
#include <map>
#include <vector>


using namespace std;

class RouterInterface
{
public:
    enum DEBUG_TYPE{
        SOCKET_INFO = 0,
    };

    RouterInterface() {}
    virtual string getRouterName()
    {
        return m_logicalName;
    }
    virtual void setRouterName(string &name)
    {
        m_logicalName = name;
    }
    virtual int printFunction(void *environment,const char *logicalName,const char *str) = 0;
    virtual int debugInfo(DEBUG_TYPE) = 0;
private:
    string m_logicalName;
};


class BrmsInterface
{
public:
	BrmsInterface() {}
	
	virtual double getSpeed() = 0 ;
	virtual double getAccel() = 0;
	virtual double getDistance() = 0;
	virtual double getA() = 0;
	
	virtual double getTTC() = 0;
	virtual string getRoadClassSig() = 0;
	virtual string getRoadKind() = 0;
	virtual string getFrontCar() = 0;
	virtual string getDriveScene() = 0;

    //profile1
	virtual string getSuddenAccel() = 0;
	virtual string getSuddenAccelState() = 0;
	virtual int getSuddenAccelCount() = 0;

    //profile2
    virtual string getSuddenDecel() = 0;
    virtual string getSuddenDecelState() = 0;
    virtual int getSuddenDecelCount() =0;

    //profile3
    virtual string getDecelStart() = 0;
    virtual string getDecelStartState() = 0;
    virtual int getDecelStartCount() = 0;

    //profile4
    virtual string getTTCRisk() = 0;
    virtual string getTTCRiskState() = 0;
    virtual int getTTCRiskCount() = 0;

    //profile5
    virtual string getDisOnExpressWay() = 0;
    virtual string getDisOnExpressWayState() = 0;
    virtual int getDisOnExpressWayCount() = 0;

    //profile6
    virtual string getDisOnOpenRoad() = 0;
    virtual string getDisOnOpenRoadState() = 0;
    virtual int getDisOnOpenRoadCount() = 0;

    //profile7
    virtual string getFollowStop() = 0;
    virtual string getFollowStopState() = 0;
    virtual int getFollowStopCount() = 0;

    virtual void offIG() = 0;
    virtual void onIG() = 0;
    virtual void dbgSwich(bool flag) = 0;

    virtual void setSpeedDbg(double speed) = 0;
    virtual void setDistanceDbg(double distance) = 0;
    virtual void setAccelDbg(double accel) = 0;
    virtual void setRoadKindDbg(int kind) = 0;
};



#endif /* _H_BRMS_INTERFACE_ */


