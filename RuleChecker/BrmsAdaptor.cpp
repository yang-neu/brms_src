
#include <QDebug>
#include <QString>
#include "BrmsAdaptor.h"
#include "stdlib.h" //rand() for Debug
#include "time.h" //time() for Debug

BrmsAdaptor::BrmsAdaptor( QObject *parent) :
    QObject(parent), m_Interval(100),m_TimerID(0), isDebug(false), flagIG(true)
    ,m_speedDebug(0.0), m_distanceDebug(0.0), m_accelDebug(0.0), m_openRoadKindDebug(0)
{
    string logicalName = "qt";
    setRouterName(logicalName);
}
int BrmsAdaptor::printFunction(void *environment,const char *logicalName,const char *str)
{
//    qDebug()<<"logic Name:"<<logicalName<<" ->"<<str;
#if 1
    if (strlen(str) == 0)
    {
        qDebug()<<"BrmsAdaptor::printFunction logic Name:"<<logicalName<<" ->"<<str;
    }
    else
        emit updateMessage(QString(QString::fromUtf8(str, strlen(str))));
#endif
    return 1;
}
void BrmsAdaptor::start()
{
    m_TimerID = startTimer(m_Interval);
}
void BrmsAdaptor::init(int argc, char *argv[])
{
       m_brms = new BRMS();
       std::string sessionID =  m_sessionID.toStdString();
       m_brms->initialize(argc, argv,sessionID, this);

}

void BrmsAdaptor::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_TimerID)
    {
        updateAll();
        updateAccelInfo();
    }
}


void BrmsAdaptor::updateAll()
{
    emit aChanged(m_brms->getA());
    emit accelChanged(m_brms->getAccel());
    emit distanceChanged(m_brms->getDistance());
    emit driveSceneChanged(QString(QString::fromUtf8(m_brms->getDriveScene().c_str(), strlen(m_brms->getDriveScene().c_str()))));
    emit frontCarChanged(QString(QString::fromUtf8(m_brms->getFrontCar().c_str(), strlen(m_brms->getFrontCar().c_str()))));
    emit roadClassSigChanged(QString(QString::fromUtf8(m_brms->getRoadClassSig().c_str(), strlen(m_brms->getRoadClassSig().c_str()))));

    //qDebug()<<"BrmsAdaptor::updateAll ++++++++++++++++++"<<m_brms->getRoadKind().c_str();
    emit roadKindChanged(QString(QString::fromUtf8(m_brms->getRoadKind().c_str(), strlen(m_brms->getRoadKind().c_str()))));
    emit speedChanged(m_brms->getSpeed());

    emit suddenAccelChanged(QString(QString::fromUtf8(m_brms->getSuddenAccel().c_str(), strlen(m_brms->getSuddenAccel().c_str()))));
    emit suddenAccelCountChanged(m_brms->getSuddenAccelCount());
    emit suddenAccelStateChanged(QString(QString::fromUtf8(m_brms->getSuddenAccelState().c_str(), strlen(m_brms->getSuddenAccelState().c_str()))));

    emit ttcChanged(m_brms->getTTC());

    emit suddenDecelChanged(QString(QString::fromUtf8(m_brms->getSuddenDecel().c_str(), strlen(m_brms->getSuddenDecel().c_str()))));
    emit suddenDecelCountChanged(m_brms->getSuddenDecelCount());
    emit suddenDecelStateChanged(QString(QString::fromUtf8(m_brms->getSuddenDecelState().c_str(), strlen(m_brms->getSuddenDecelState().c_str()))));

    emit decelStartChanged(QString(QString::fromUtf8(m_brms->getDecelStart().c_str(), strlen(m_brms->getDecelStart().c_str()))));
    emit decelStartStateChanged(QString(QString::fromUtf8(m_brms->getDecelStartState().c_str(), strlen(m_brms->getDecelStartState().c_str()))));
    emit decelStartCountChanged(m_brms->getDecelStartCount());


    emit ttcRiskChanged(QString(QString::fromUtf8(m_brms->getTTCRisk().c_str(), strlen(m_brms->getTTCRisk().c_str()))));
    emit ttcRiskStateChanged(QString(QString::fromUtf8(m_brms->getTTCRiskState().c_str(), strlen(m_brms->getTTCRiskState().c_str()))));
    emit ttcRiskCountChanged(m_brms->getTTCRiskCount());

    emit disOnExpressWayChanged(QString(QString::fromUtf8(m_brms->getDisOnExpressWay().c_str(), strlen(m_brms->getDisOnExpressWay().c_str()))));
    emit disOnExpressWayStateChanged(QString(QString::fromUtf8(m_brms->getDisOnExpressWayState().c_str(), strlen(m_brms->getDisOnExpressWayState().c_str()))));
    emit disOnExpressWayCountChanged(m_brms->getDisOnExpressWayCount());

    emit disOnOpenRoadChanged(QString(QString::fromUtf8(m_brms->getDisOnOpenRoad().c_str(), strlen(m_brms->getDisOnOpenRoad().c_str()))));
    emit disOnOpenRoadStateChanged(QString(QString::fromUtf8(m_brms->getDisOnOpenRoadState().c_str(), strlen(m_brms->getDisOnOpenRoadState().c_str()))));
    emit disOnOpenRoadCountChanged(m_brms->getDisOnOpenRoadCount());

    emit followStopChanged(QString(QString::fromUtf8(m_brms->getFollowStop().c_str(), strlen(m_brms->getFollowStop().c_str()))));
    emit followStopStateChanged(QString(QString::fromUtf8(m_brms->getFollowStopState().c_str(), strlen(m_brms->getFollowStopState().c_str()))));
    emit followStopCountChanged(m_brms->getFollowStopCount());
}

void BrmsAdaptor::updateAccelInfo()
{
    QVector<double> accelInfo;

    //brmsからデータを取得
    //vector<FieldAndValue> v_data;
    //v_data = m_brms->getCommonData();
    //while(v_data.size()) {
    //    FieldAndValue value = v_data.pop_back();
    //}
    //vector<FieldAndValue>::iterator it;
    //for(it=v_data.begin(); it!=v_data.end(); it++) {
    //    printf("it=%d\n", *it.);
    //}

    int i;

     FieldAndValue data;

     int data_i;

     float data_f;

     FieldDataVec tmp = m_brms->getCommonData();

     if(tmp.empty())

     {

         //do nothing

     }

     else{

         for (i=0;i<20;i++)

         {

             data=tmp[i];

             if(data.type == 0){

                 data_f=(float)data.data.f_value;

             }else if (data.type == 1){

                 data_i=(int)data.data.i_value;

             }

             emit accelInfoChanged("old", data_f, data_i, "caution");

         }

     }


    //評価用データ for Debug
    //accelInfo << 1.7 << 24 << 1.8 << 31 << 1.9 << 3 << 2.0 << 2 << 2.1 << 1 <<
    //             1.6 << 128 << 1.5 << 71 << 1.4 << 20 << 1.3 << 89 << 1.2 << 11 <<
    //             1.1 << 90 << 1.0 << 34 << 0.9 << 5 << 0.8 << 76 << 0.4 << 54 ;

    //accelInfoChangedシグナルを送信
    while(accelInfo.length()) {
        double data = accelInfo.takeFirst();
        int count = accelInfo.takeFirst();
        emit accelInfoChanged("old", data, count, "caution");
    }

    //ランダムで今回のTrip加速度を生成 for Debug
    {
        srand((unsigned)time(NULL));

        double data = (rand()%22+1)*0.1;
        double count = (rand()%50+1);


        //printf("accel=%d count=%d\n",data, count);
        if(data>=1.8) {
            emit accelInfoChanged("this", data, count, "caution");
        }else{
            emit accelInfoChanged("this", data, count, "none");
        }
    }

}


