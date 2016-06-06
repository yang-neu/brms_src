#include "EventCharacteristicAndStatus3sigma.h"

const char* EventCharacteristicAndStatus3sigma::m_spec3sigmaNameList[SPECIFIC_THREESIGMA_MAX] = {
    "NAN0", "RELAX", "NORELAX", "FEAR_TTCSHORT", "FEARLESS_TTCSHORT", "AVERAGE_SMALL", "AVERAGE_LARGE","SHAKE_SMALL","SHAKE_LARGE"};
const char* EventCharacteristicAndStatus3sigma::m_profileType3sigma[PROFILE_TYPE_THREESIGMA_MAX] = {
    "TTC_OF_SPEED_DECELERATION", "DURATION_OF_SHORTTTC_CHECK", "STOPPED_DISTANCE","STRAIGHT_RUN_SHAKE","FRONT_CAR_FOLLOW_START_TIMING" };
