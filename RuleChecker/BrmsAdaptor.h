#ifndef BRMS_ADAPTOR_H
#define BRMS_ADAPTOR_H
#include <QVariant>
#include <QObject>
#include <QTimerEvent>
#include "../brms/BRMS.h"
#include <vector>
#include <QVector>

class BrmsAdaptor : public QObject, RouterInterface
{
    Q_OBJECT
    Q_PROPERTY (bool switchInput  WRITE setSwitchInput)
    Q_PROPERTY (bool switchIG  WRITE setSwitchIG)

    Q_PROPERTY (QString speedDebug  WRITE setSpeedDebug)
    Q_PROPERTY (QString distanceDebug WRITE setDistanceDebug NOTIFY distanceDebugChanged)
    Q_PROPERTY (QString accelDebug  WRITE setAccelDebug)
    Q_PROPERTY (int openRoadKindDebug  WRITE setOpenRoadKindDebug)

public:
    explicit BrmsAdaptor(QObject *parent = 0);
    void init(int argc, char *argv[]);
    void start();
    virtual  int printFunction(void *environment,const char *logicalName,const char *str);
   // bool switchInput() { return isDebug; }
    void setSwitchInput(bool flag) {

        if (m_brms != NULL)
            m_brms->dbgSwich(true);
    }

   // bool switchIG() { return flagIG; }
    void setSwitchIG(bool flag) {
        if (flagIG)
        {
            flagIG = false;
            if(m_brms != NULL)
                m_brms->offIG();
        }
        else
        {
            flagIG = true;
            if(m_brms != NULL)
                m_brms->onIG();
        }

    }

    //QString speedDebug() { return QString::number(m_speedDebug); }
    void setSpeedDebug(QString speed)
    {
        m_speedDebug =  speed.toDouble();
        if(m_brms != NULL)
            m_brms->setSpeedDbg(m_speedDebug);
    }
    ///QString distanceDebug() { return QString(m_distanceDebug); }
    void setDistanceDebug(QString distance)
    {
        m_distanceDebug = distance.toDouble();
        if(m_brms != NULL)
            m_brms->setDistanceDbg(m_distanceDebug);
    }
    //QString accelDebug() { return QString(m_accelDebug); }
    void setAccelDebug(QString accel)
    {
        m_accelDebug = accel.toDouble();
        if(m_brms != NULL)
            m_brms->setAccelDbg(m_accelDebug);
    }
    //int openRoadKindDebug() { return m_openRoadKindDebug ; }
    void setOpenRoadKindDebug(int kind)
    {
        m_openRoadKindDebug = kind;
        if(m_brms != NULL)
            m_brms->setRoadKindDbg(m_openRoadKindDebug);
    }


signals:

    void speedChanged(double speed);
    void distanceChanged(double distance);
    void accelChanged(double accel);
    void ttcChanged(double ttc);
    void roadClassSigChanged(QString str);
    void aChanged(double a);
    void roadKindChanged(QString str);
    void frontCarChanged(QString str);
    void driveSceneChanged(QString str);

    void accelInfoChanged(QString state, double data, int count, QString caution);
    void analysisResultChanged(double aMax, int oldNum, int thisNum, int rate, double oldAve, double thisAve, double variance, int count);
    void accelCharacteristicChanged(int state);

    void suddenAccelChanged(QString str);
    void suddenAccelStateChanged(QString str);
    void suddenAccelCountChanged(int count);

    void suddenDecelChanged(QString str);
    void suddenDecelStateChanged(QString str);
    void suddenDecelCountChanged(int count);

    void decelStartChanged(QString str);
    void decelStartStateChanged(QString str);
    void decelStartCountChanged(int count);

    void ttcRiskChanged(QString str);
    void ttcRiskStateChanged(QString str);
    void ttcRiskCountChanged(int count);

    void disOnExpressWayChanged(QString str);
    void disOnExpressWayStateChanged(QString str);
    void disOnExpressWayCountChanged(int count);

    void disOnOpenRoadChanged(QString str);
    void disOnOpenRoadStateChanged(QString str);
    void disOnOpenRoadCountChanged(int count);

    void followStopChanged(QString str);
    void followStopStateChanged(QString str);
    void followStopCountChanged(int count);

    void distanceDebugChanged(double distance);

    void updateMessage(QString str);
public slots:

protected:
    double m_speedDebug;
    double m_distanceDebug;
    double m_accelDebug;
    int m_openRoadKindDebug;
    bool isDebug;
    bool flagIG;
    QString m_sessionID = "DNCockpitKS";
    int m_TimerID;
    int m_Interval;
    void timerEvent(QTimerEvent *event);
    void updateAll();
    void updateAccelInfo();

    BRMS *m_brms;
};

#endif // BRMS_ADAPTOR_H
