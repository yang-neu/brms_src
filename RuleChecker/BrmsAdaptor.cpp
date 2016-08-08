
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include "BrmsAdaptor.h"
#include "stdlib.h" //rand() for Debug
#include "time.h" //time() for Debug
#include <sstream>

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

int string2int(string str){
    stringstream ss(str);
    int ret;
    ss<<str;
    ss>>ret;
    return ret;
}
double string2double(string str) {
    stringstream ss(str);
    double ret;
    ss<<str;
    ss>>ret;
    return ret;
}
QString string2QString(string str) {
    //QTextCodec* tc = QTextCodec::codecForLocale();
    //QTextCodec *sjis = QTextCodec::codecForName("Shift-JIS");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    return QString(utf8->toUnicode(str.c_str()));
}

int BrmsAdaptor::displayData(const char *str)
{
//    qDebug()<<"logic Name:"<<logicalName<<" ->"<<str;

    if (strlen(str) == 0 || strcmp(str," ") == 0)
    {
        //qDebug()<<"BrmsAdaptor::printFunction logic Name:"<<logicalName<<" ->"<<str;
        return -1;
    }


    //strを空白区切りでdataに読み込む
    stringstream ss(str);
    string tmp;
    vector<std::string> data;

    while(getline(ss, tmp, ',')) {
        //cout << tmp << endl;
        data.push_back(tmp);
    }
    ss.str("");
    ss.clear(stringstream::goodbit);


    //データの種別を取得する
    string kind = data.at(0);
    //cout << "knid="<<kind << endl;


    //データの種別によって処理を実施する
    if("profile01" == kind) {
        updateAccelInfo(data);
    } else if("EventSpeed" == kind && 6 == data.size()) {
        emit speedChanged(string2double(data.at(3)));
    } else if("driveScene" == kind && 2 == data.size()) {
        emit driveSceneChanged(string2QString(data.at(1)));
    } else if("EventDistance" == kind && 6 == data.size()) {
        emit distanceChanged(string2double(data.at(3)));
    } else if("shiftPosition" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("EventSteeringAngle" == kind && 6 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("EventAccelOpen" == kind && 6 == data.size()) {
        emit accelChanged(string2double(data.at(3)));
    } else if("roadClassSig" == kind && 2 == data.size()) {
        emit roadClassSigChanged(string2QString(data.at(1)));
    } else if("roadClass" == kind && 2 == data.size()) {
        emit roadKindChanged(string2QString(data.at(1)));
    } else if("EventAcceleration" == kind && 2 == data.size()) {
        emit aChanged(string2double(data.at(1)));
    } else if("EventDistanceDiff" == kind && 2 == data.size()) {
        emit ttcChanged(string2double(data.at(1)));
    } else if("steeringAngleAccel" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("suddenStAngleAccelCount" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("suddenStAngleAccelState" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("LeftTurnCount" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("LeftTurnState" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("RightTurnCount" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else if("RightTurnState" == kind && 2 == data.size()) {
        //emit TBD(string2double(data.at(1)));
    } else {
        cout << "[Warning] BrmsAdaptor::displayData knid='"<<kind<<"' is undefined." << endl; 
    }


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
        //updateAll();
        //updateAccelInfo();
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
void BrmsAdaptor::updateAccelInfo(vector<std::string> data)
{
    unsigned int i = 0;
    // data_i = 0;
    //float data_f = 0;

    //vector<FieldAndValue> data = m_brms->getCommonData();
    if(data.empty())
    {
        //nothing to do
        //cout << "++++++++++Do nothing!" << endl;
    }
    else{
        if(1 == string2int(data.at(1))){
            for (i=2; i<data.size(); i=i+2)
            {
                //if(0 != data[i].type && 1 != data[i+1].type) {
                //    cout << "Error: the structure of vector is incorrect" << endl;
                //    return;
                //}

                //data_f=(float)data[i].data.f_value; //[1]accell
                //data_i=(int)data[i+1].data.i_value; //[2]count

                if(string2double(data.at(i))>=threeSigma) {
                    emit accelInfoChanged(gTripDataKind, string2double(data.at(i)), string2int(data.at(i+1)), "caution");
                }else{
                    emit accelInfoChanged(gTripDataKind, string2double(data.at(i)), string2int(data.at(i+1)), "none");
                }
                //cout << "Accel : " << data_f << " count : " << data_i << endl;
            }
            cout << "-----" << endl;
            gTripDataKind="this";

        }else if(2 == string2int(data.at(1))){
            for (i=2; i<data.size(); i=i+13)
            {
                //i=i; //(int)data[i].data.i_value; //[1]diffAlways
                int count =string2int(data.at(i+1));//(int)data[i+1].data.i_value; //[2]Cnt3Sigma
                float variance =string2double(data.at(i+2));//(float)data[i+2].data.f_value; //[3]Sigma
                float thisAve =string2double(data.at(i+3));//(float)data[i+3].data.f_value; //[4]currentAveSpeed
                float oldAve =string2double(data.at(i+4));//(float)data[i+4].data.f_value; //[5]previousAveSpeed
                float rate =string2double(data.at(i+5));//(float)data[i+5].data.f_value; //[6]PercentageofSudAcc
                int thisNum =string2int(data.at(i+6));//(int)data[i+6].data.i_value; //[7]currentSampling
                int oldNum =string2int(data.at(i+7));//(int)data[i+7].data.i_value; //[8]previousSampling
                float aMax=string2double(data.at(i+8));//(float)data[i+8].data.f_value; // [9]MaxAccelofScene
                QString state = string2QString(data.at(i+9));//(QString)data[i+9].data.s_value; //[10]AccelJudgement
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


