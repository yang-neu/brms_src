
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

int BrmsAdaptor::debugInfo(DEBUG_TYPE type)
{
    switch(type)
    {
    case CLIPS_SESSIONSTATE_IDLE:
        emit socket_status_changed(QString("red"));
        emit comDataSizeChanged(0);
        break;
    case CLIPS_SESSIONSTATE_RECVING:
        emit socket_status_changed(QString("blue"));
        break;
    default:
        return -1;
    }
    return 0;
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

int BrmsAdaptor::comDataSize(unsigned int size)
{
    emit comDataSizeChanged(size);
    return 0;
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

QString gTripDataKind = "old";
float threeSigma=99.9;
void BrmsAdaptor::updateAccelInfo()
{
    unsigned int i = 0;
    int data_i = 0;
    float data_f = 0;

    vector<FieldAndValue> data = m_brms->getCommonData();
    if(data.empty())
    {
        //nothing to do
        //cout << "++++++++++Do nothing!" << endl;
    }
    else{
        if(1 == (int)data[0].data.i_value){
            for (i=1; i<data.size(); i=i+2)
            {
                if(0 != data[i].type && 1 != data[i+1].type) {
                    cout << "Error: the structure of vector is incorrect" << endl;
                    return;
                }

                data_f=(float)data[i].data.f_value; //[1]accell
                data_i=(int)data[i+1].data.i_value; //[2]count

                if(data_f>=threeSigma) {
                    emit accelInfoChanged(gTripDataKind, data_f, data_i, "caution");
                }else{
                    emit accelInfoChanged(gTripDataKind, data_f, data_i, "none");
                }
                //cout << "Accel : " << data_f << " count : " << data_i << endl;
            }
            cout << "-----" << endl;
            gTripDataKind="this";

        }else if(2 == (int)data[0].data.i_value){
            for (i=1; i<data.size(); i=i+13)
            {
                //i=i; //(int)data[i].data.i_value; //[1]diffAlways
                int count =(int)data[i+1].data.i_value; //[2]Cnt3Sigma
                float variance =(float)data[i+2].data.f_value; //[3]Sigma
                float thisAve =(float)data[i+3].data.f_value; //[4]currentAveSpeed
                float oldAve =(float)data[i+4].data.f_value; //[5]previousAveSpeed
                float rate =(float)data[i+5].data.f_value; //[6]PercentageofSudAcc
                int thisNum =(int)data[i+6].data.i_value; //[7]currentSampling
                int oldNum =(int)data[i+7].data.i_value; //[8]previousSampling
                float aMax=(float)data[i+8].data.f_value; // [9]MaxAccelofScene
                QString state = (QString)data[i+9].data.s_value; //[10]AccelJudgement
                //i++; //(QString)data[i+10].data.s_value;//[11]VihicleState
                //i++; //(float)data[i+11].data.f_value;//[12]accel
                //i++; //(float)data[i+12].data.f_value;//[13]speed
                cout << "aMax="<<aMax << ", oldNum="<<oldNum << ", thisNum="<<thisNum << ", (rate*100)="<<(rate*100) << ", oldAve="<<oldAve << ", thisAve="<<thisAve << ", variance="<<variance << ", count="<<count << endl;

                //これまでのデータからの標準範囲(±1σ), 3σの計算
                cout << "-Sigma="<<oldAve-variance << endl;
                cout << "+Sigma="<<oldAve+variance << endl;
                cout << "+3Sigma="<<oldAve+(3*variance) << endl;
                threeSigma = oldAve+(3*variance) <= 2.3 ? oldAve+(3*variance) : 2.3;

                emit analysisResultChanged(aMax, oldNum, thisNum, (rate*100), oldAve, thisAve, variance, count);
                emit accelCharacteristicChanged(state);
                emit standardRangeChanged(oldAve-variance, oldAve+variance);
            }
            cout << "-----" << endl;
        }

     }


    //評価用データ for Debug
    /*{
        QVector<double> accelInfo;
        accelInfo << 1.7 << 24 << 1.8 << 31 << 1.9 << 3 << 2.0 << 2 << 2.1 << 1 <<
                     1.6 << 128 << 1.5 << 71 << 1.4 << 20 << 1.3 << 89 << 1.2 << 11 <<
                     1.1 << 90 << 1.0 << 34 << 0.9 << 5 << 0.8 << 76 << 0.4 << 54 ;

        //accelInfoChangedシグナルを送信
        while(accelInfo.length()) {
            double data = accelInfo.takeFirst();
            int count = accelInfo.takeFirst();
            emit accelInfoChanged("old", data, count, "none");
        }
    }*/

    //ランダムで今回のTrip加速度を生成 for Debug
    /*{
        srand((unsigned)time(NULL));

        double data = (rand()%15+8)*0.1;
        double count = (rand()%50+1);

        //printf("accel=%d count=%d\n",data, count);
        if(data>=1.8) {
            emit accelInfoChanged("this", data, count, "caution");
        }else{
            emit accelInfoChanged("this", data, count, "none");
        }
    }*/

    //ランダムで分析結果を生成 for Debug
    /*{
        srand((unsigned)time(NULL));

        //emit analysisResultChanged(2.2, 500, 123, 3, 1.62, 1.99, 3.21, 3);
        emit analysisResultChanged((rand()%15+8)*0.1, (rand()%500+1), (rand()%500+1), (rand()%100+1), (rand()%220+1)*0.01, (rand()%220+1)*0.01, (rand()%300+1)*0.01, (rand()%3+1));

        emit accelCharacteristicChanged("急加速しがち");
    }*/

}


