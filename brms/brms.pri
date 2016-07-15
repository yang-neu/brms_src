#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T16:16:50
#
#-------------------------------------------------
INCLUDEPATH +=  ../clips_core_source_630/core
#DEFINES += BRMS_LIBRARY
DEFINES +=  _DEBUG_CLIPS_
#DEFINES +=  _CAN_DEVICE_
DEFINES +=_PERFORMANCE_
SOURCES += ../brms/BRMS.cpp \
    ../brms/ClipsRuleMgr.cpp \
    ../brms/ClipsSessionMgr.cpp \
    ../brms/Controller.cpp \
    ../brms/common/Message.cpp \
    ../brms/common/StringUtil.cpp \
    ../brms/common/SysTimer.cpp \
    ../brms/common/win_timer.cpp \
    ../brms/common/TimeUtil.cpp \
    ../brms/common/ConfigUtil.cpp \
    ../brms/event/CANTransmitData.cpp \
    ../brms/event/EventCharacteristicAndStatus3sigma.cpp \
    ../brms/event/EventCharacteristicAndStatusBinomial.cpp \
    ../brms/extension/ExternalFunctions.cpp \
    ../brms/signaladapter/CANDataParse.cpp \
    ../brms/signaladapter/SocketReceiveThread.cpp \
    ../brms/signaladapter/InfoGetBaseThread.cpp \
    ../brms/signaladapter/DecisionAppServerThread.cpp   \
    ../brms/signaladapter/IntegrationGenerateClientThread.cpp   \
    ../brms/signaladapter/DataTrans.cpp \
    ../brms/streaming/EntryPoint.cpp \
    ../brms/streaming/EntryPointOutput.cpp \
    ../brms/streaming/EntryPointUser.cpp \
    ../brms/streaming/SignalEntryPoint.cpp \
    ../brms/thirdpart/duktape.c \
    ../brms/timeliner/timeliner.cpp \
    $$PWD/streaming/EntryPointCommonData.cpp

HEADERS +=../brms/BRMS.h \
    ../brms/ClipsRuleMgr.h \
    ../brms/ClipsSessionMgr.h \
    ../brms/Controller.h \
    ../brms/common/Message.h \
    ../brms/common/StringUtil.h \
    ../brms/common/SysTimer.h \
    ../brms/common/win_timer.h \
    ../brms/common/TimeUtil.h \
    ../brms/common/ConfigUtil.h \
    ../brms/event/CANTransmitData.h \
    ../brms/event/EventAccelOpen.h \
    ../brms/event/EventCharacteristicAndStatus3sigma.h \
    ../brms/event/EventCharacteristicAndStatusBinomial.h \
    ../brms/event/EventDistance.h \
    ../brms/event/EventRoadClass.h \
    ../brms/event/EventSpeed.h \
    ../brms/event/EventAimless.h    \
    ../brms/event/EventDriverFace.h \
    ../brms/event/EventLevelData.h    \
    ../brms/event/EventShiftState.h   \
    ../brms/event/EventSteeringAngle.h   \
    ../brms/event/EventBrakePressure.h   \
    ../brms/extension/ExternalDef.h \
    ../brms/extension/ExternalFunctions.h \
    ../brms/interface/BrmsInterface.h \
    ../brms/signaladapter/CANDataParse.h \
    ../brms/signaladapter/SocketReceiveThread.h \
    ../brms/signaladapter/InfoGetBaseThread.h   \
    ../brms/signaladapter/DataTrans.h  \
    ../brms/signaladapter/DecisionAppServerThread.h \
    ../brms/signaladapter/IntegrationGenerateClientThread.h \
    ../brms/streaming/EntryPoint.h \
    ../brms/streaming/EntryPointOutput.h \
    ../brms/streaming/EntryPointUser.h \
    ../brms/streaming/SignalEntryPoint.h \
    ../brms/utility/AccelOpenUtility.h \
    ../brms/utility/DistanceUtility.h \
    ../brms/utility/RoadClassUtility.h \
    ../brms/utility/SpeedUtility.h \
    ../brms/utility/UtilityBase.h   \
    ../brms/utility/AimlessLevelUtility.h   \
    ../brms/utility/DriverFaceDirectionUtility.h    \
    ../brms/utility/LoadLevelUtility.h  \
    ../brms/utility/ShiftStateReverseUtility.h  \
    ../brms/utility/ShiftStateDriveUtility.h  \
    ../brms/utility/ShiftStateParkingUtility.h  \
    ../brms/utility/SteeringAngleUtility.h  \
    ../brms/utility/SteeringAngleCANUtility.h  \
    ../brms/utility/BrakePressureUtility.h  \
    ../brms/thirdpart/rapidxml.hpp  \
    ../brms/thirdpart/rapidxml_iterators.hpp    \
    ../brms/thirdpart/rapidxml_print.hpp    \
    ../brms/thirdpart/rapidxml_utils.hpp \
    ../brms/thirdpart/duktape.h \
    ../brms/utility/ShiftStateDriveUtility.h \
    ../brms/utility/ShiftStateParkingUtility.h \
    ../brms/utility/ShiftStateCANUtility.h \
    ../brms/timeliner/timeliner.hpp \
    $$PWD/thirdpart/duk_config.h \
    $$PWD/streaming/EntryPointCommonData.h

OTHER_FILES +=
