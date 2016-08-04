;==================================================
; Module Define
;==================================================
(defmodule MAIN (export ?ALL))
(defmodule SpecificAgenda (import MAIN ?ALL))
(defmodule SpecificAgenda10s (import MAIN ?ALL))
(defmodule IOAgenda (import MAIN ?ALL))


;==================================================
; Global Variables
;==================================================
(deftemplate MAIN::EventOutput
	(slot speed
		(type INTEGER))
	(slot accel
		(type INTEGER))
	(slot accelOpen
		(type INTEGER))
	(slot distance
		(type INTEGER))
	(slot TTC
		(type INTEGER))
	(slot roadClassSig
		(type STRING))
;yintf add 20160304
	(slot steeringangle
		(type FLOAT))
	(slot jerk
		(type FLOAT))
	(slot steeringAngleAccel
		(type FLOAT))

	(slot suddenAccel
		(type STRING)
		(default "未定義"))
	(slot suddenAccelState
		(type STRING)
		(default "状態未判定"))
	(slot suddenAccelCount
		(type INTEGER))
;liuxin add start
	(slot suddenBrake
		(type STRING)
		(default "未定義"))
	(slot suddenBrakeState
		(type STRING)
		(default "状態未判定"))
	(slot suddenBrakeCount
		(type INTEGER))		
	(slot suddenStart
		(type STRING)
		(default "未定義"))
	(slot suddenStartState
		(type STRING)
		(default "状態未判定"))
	(slot suddenStartCount
		(type INTEGER))
;liuxin add end		
	(slot suddenReduce
		(type STRING)
		(default "未定義"))
	(slot suddenReduceState
		(type STRING)
		(default "状態未判定"))
	(slot suddenReduceCount
		(type INTEGER))
	(slot reduceStart
		(type STRING)
		(default "未定義"))
	(slot reduceStartState
		(type STRING)
		(default "状態未判定"))
	(slot reduceStartCount
		(type INTEGER))
	(slot TTCRisk
		(type STRING)
		(default "未定義"))
	(slot TTCRiskState
		(type STRING)
		(default "状態未判定"))
	(slot TTCRiskCount
		(type INTEGER))
	(slot distanceOnExpressWay
		(type STRING)
		(default "未定義"))
	(slot distanceOnExpressWayState
		(type STRING)
		(default "状態未判定"))
	(slot distanceOnExpressWayCount
		(type INTEGER))
	(slot distanceOnOpenRoad
		(type STRING)
		(default "未定義"))
	(slot distanceOnOpenRoadState
		(type STRING)
		(default "状態未判定"))
	(slot distanceOnOpenRoadCount
		(type INTEGER))
	(slot distanceofFollowStop
		(type STRING)
		(default "未定義"))
	(slot distanceofFollowStopState
		(type STRING)
		(default "状態未判定"))
	(slot distanceofFollowStopCount
		(type INTEGER))
	(slot driveScene
		(type STRING)
		(default "停車"))
	(slot withFrontCar
		(type STRING)
		(default "前車なし"))
	(slot roadClass
		(type STRING)
		(default "一般道路"))
;by sun-chl add 2016/03/07 start
	(slot shiftPosition	
		(type SYMBOL))
	(slot Acceloperation	;by sun-chl add 2016/03/01
		(type STRING)
		(default "未定義"))
	(slot AccelOpenCount	;by sun-chl add 2016/03/01
		(type INTEGER))
;by sun-chl add 2016/03/07 end
;yintf add 20160304
	(slot posture
		(type STRING)
		(default "直線姿勢"))
	(slot vehicleShake
		(type STRING)
		(default "未定義"))
	(slot vehicleShakeState
		(type STRING)
		(default "状態未判定"))
	(slot vehicleShakeCount
		(type INTEGER))
;yintf add 20160310 for p23
	(slot suddenStAngleAccel
		(type STRING)
		(default "未定義"))
	(slot suddenStAngleAccelState
		(type STRING)
		(default "状態未判定"))
	(slot suddenStAngleAccelCount
		(type INTEGER))
;sunchl add 20160315
	(slot LeftTurnCount
		(type INTEGER))
	(slot LeftTurnState
		(type STRING)
		(default "未定義"))
	(slot startDistanceCount
		(type INTEGER))
	(slot startDistanceProperty
		(type STRING)
		(default "未定義"))
	(slot startDistanceState
		(type STRING)
		(default "状態未判定"))
;sunchl add 20160317
	(slot RightTurnCount
		(type INTEGER))
	(slot RightTurnState
		(type STRING)
		(default "未定義"))
)

;(defglobal MAIN
;	?*list* = (assert (EventOutput)))
(defglobal MAIN
	?*TableHistorySaveTime* = 5.0) ;1s=1.0, 60s=60.0

;==================================================
; Templete
;==================================================
; event lists
(deftemplate MAIN::EventSpeedList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type speed
	(multislot speedList))

(deftemplate MAIN::EventAccelOpenList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type accelOpen
	(multislot accelOpenList))

(deftemplate MAIN::EventDistanceList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type distance
	(multislot distanceList))

(deftemplate MAIN::EventRoadClassList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type class canData
	(multislot roadClassList))

(deftemplate MAIN::EventAimlessList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot aimlessList))

(deftemplate MAIN::EventLevelDataList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot levelDataList))

(deftemplate MAIN::EventSteeringAngleList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot steeringAngleList))

(deftemplate MAIN::EventBrakePressureList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot brakePressureList))

(deftemplate MAIN::EventShiftStateList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot shiftStateList))

(deftemplate MAIN::EventTurnSignalList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot turnSignalList))

(deftemplate MAIN::EventDriverFaceList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type value
	(multislot driverFaceList))


; events
(deftemplate MAIN::EventSpeed
	(slot from (default entryPoint))
	(slot name)
	
	(slot speed ; 走行中/停止中判断パーツ
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values VEHICLE_SPEED_SP1)
		(default VEHICLE_SPEED_SP1))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventAccelOpen
	(slot from (default entryPoint))
	(slot name)
	
	(slot accelOpen ;アクセル開度
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values ACCEL_OPEN)
		(default ACCEL_OPEN))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventDistance
	(slot from (default entryPoint))
	(slot name)
	
	(slot distance ;車間距離
		(type FLOAT)
		(default 255.0))
	(slot type
		(type SYMBOL)
		(allowed-values VEHICLE_FOLLOWING_DISTANCE)
		(default VEHICLE_FOLLOWING_DISTANCE))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventRoadClass
	(slot from (default entryPoint))
	(slot name)
	
	(slot canData
		(type INTEGER)
		(default 0))
	(slot type
		(type SYMBOL)
		(allowed-values ROAD_CLASS)
		(default ROAD_CLASS))
	(slot roadClass
		(type SYMBOL)
		(allowed-values OPENROAD EXPRESSWAY))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventAimless
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values AIMLESS_LEVEL)
		(default AIMLESS_LEVEL))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventLevelData
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type INTEGER)
		(default 0))
	(slot type
		(type SYMBOL)
		(allowed-values LOAD_LEVEL)
		(default LOAD_LEVEL))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventSteeringAngle
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values STEERING_ANGLE)
		(default STEERING_ANGLE))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventBrakePressure
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values BRAKE_PRESSURE)
		(default BRAKE_PRESSURE))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventShiftState
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type SYMBOL)
		(allowed-values REVERSE STOP DRIVE)
		(default DRIVE))
	(slot type
		(type SYMBOL)
		(allowed-values REVERSE)
		(default REVERSE))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventTurnSignal
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values TURN_SIGNAL)
		(default TURN_SIGNAL))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventDriverFace
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values FACE_ANGLE)
		(default FACE_ANGLE))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

; facts
(deftemplate MAIN::EventAcceleration
	(slot from (default entryPoint))
	(slot name)
	
	(slot acceleration
		(type FLOAT))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventDriveScene
	(slot from (default entryPoint))
	(slot name)
	
	(slot driveScene
		(type SYMBOL)
		(allowed-values REVERSE STOP START RUNNING ACCEL REDUCE)
		(default STOP))
	(slot value1
		(type FLOAT)
		(default 0.0))
	(slot value2
		(type FLOAT)
		(default 0.0))
	(slot value3
		(type FLOAT)
		(default 0.0))
	(slot beforeDriveScene
		(type SYMBOL)
		(allowed-values REVERSE STOP START RUNNING ACCEL REDUCE))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time)))
	(slot accelAccelerationCount
		(type INTEGER)
		(default 0))
	(slot runningAccelerationCount
		(type INTEGER)
		(default 0)))
;yintf add 20160304
;車両姿勢シーン
(deftemplate MAIN::EventVehiclePostureScene
	(slot from (default entryPoint))
	(slot name)
	
	(slot posture
		(type SYMBOL)
		(allowed-values NAN TURN_STRAIGHT TURN_LEFT TURN_RIGHT)
		(default NAN))
	(slot beforeVehiclePostureScene
		(type SYMBOL)
		(allowed-values NAN TURN_STRAIGHT TURN_LEFT TURN_RIGHT)
		(default NAN))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventJerk
	(slot from (default entryPoint))
	(slot name)
	
	(slot value
		(type FLOAT)
		(default 0.0))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

; Steering Angle Acceleration
(deftemplate MAIN::EventSteeringAngleAcceleration
	(slot from (default entryPoint))
	(slot name)
	
	(slot steeringAngleAcceleration
		(type FLOAT))
	(slot time
		(type FLOAT)
		(default-dynamic (time))))

(deftemplate MAIN::EventDistanceDiff
	(slot from (default entryPoint))
	(slot name)
	
	(slot distanceDiff ;車間距離
		(type FLOAT)
		(default 0.0))
	(slot nowDistance
		(type FLOAT)
		(default 255.0))
	(slot beforeDistance
		(type FLOAT)
		(default 255.0))
	(slot TTC
		(type FLOAT)
		(default 0.0))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time)))
	(slot span
		(allowed-values ONE_SEC FIVE_HUNDRED_MILLISEC HUNDRED_MILLISEC TTC_CHECK_SEC)
;yintf
		(default ONE_SEC)))

(deftemplate MAIN::EventFrontCarStat
	(slot from (default entryPoint))
	(slot name)
	
	(slot withFrontCar
		(type SYMBOL)
		(allowed-values WITHOUT NO_FOLLOW GAIN_FOLLOW AWAY_FOLLOW INTERRUPT))
	(slot value1
		(type FLOAT)
		(default 0.0))
	(slot value2
		(type FLOAT)
		(default 0.0))
	(slot value3
		(type FLOAT)
		(default 0.0))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))))

(deftemplate MAIN::EventCharacteristicAndStatus3sigma
	(slot from (default entryPoint))
	(slot name)
	
	(slot MAX_SAMPLE_SIZE
		(type INTEGER)
		(default 50))
	(slot type
		(type SYMBOL)
;;yintf add 20160304
	(allowed-values TTC_OF_SPEED_DECELERATION DURATION_OF_SHORTTTC_CHECK STOPPED_DISTANCE STRAIGHT_RUN_SHAKE FRONT_CAR_FOLLOW_START_TIMING))
	(multislot samplingList)
	(slot specific
		(type SYMBOL)
		(allowed-values NAN RELAX NORELAX FEAR_TTCSHORT FEARLESS_TTCSHORT AVERAGE_SMALL AVERAGE_LARGE SHAKE_SMALL SHAKE_LARGE))
	(slot average
		(type FLOAT)
		(default 0.0))
	(slot stdDeviation
		(type FLOAT)
		(default 0.0))
	(slot userStateCheck
		(allowed-values TRUE FALSE)
		(default FALSE))
	(slot userState
		(type STRING))
	(slot outlierValCount
		(type INTEGER)
		(default 0))
	(slot oldSampleAverage
		(type FLOAT)
		(default 0.0))
	(slot oldSampleStdDeviation
		(type FLOAT)
		(default 0.0))
;yintf add 20160304
	(slot p8_shakeCount
		(type FLOAT)
		(default 0.0))
	(slot sceneStartTime
		(type FLOAT)
		(default 0.0))
	(slot accumulateTime
		(type FLOAT)
		(default 0.0)))

(deftemplate MAIN::EventCharacteristicAndStatusBinomial
	(slot from (default entryPoint))
	(slot name)
	
	(slot MAX_SAMPLE_SIZE
		(type INTEGER)
		(default 100))
	(slot STATE_CHECK_SIZE
		(type INTEGER)
		(default 10))
	(slot DEQUE_SIZE
		(type INTEGER)
		(default 110))
	(slot type
		(type SYMBOL)
		(allowed-values SPEED_ACCELERATION SPEED_DECELERATION RUNNING_DISTANCE_ONOPENROAD RUNNING_DISTANCE_ONEXPRESSWAY ACCEL_TREAD_TIME STOP_JERK_BRAKE START_ACCELERATION STEERINGANGLE_ACCELERATION LEFR_TURN_SPEED RIGHT_TURN_SPEED))
	(multislot specificList)
	(slot specific
		(type SYMBOL)
		(allowed-values NAN SUDDEN NORMAL MARGIN_SMALL MARGIN_MEDIUM MARGIN_LARGE ACCEL_APPOSITE ACCEL_EXCESSIVELY SLOW_IN_FAST_OUT REDUCE_TURN ACCEL_TURN)
		(default NAN))
	(slot percentage
		(type FLOAT)
		(default 0.0))
;liuxin add start
	(slot accel
		(type FLOAT)
		(default 0.0))
;liuxin add end
	(slot userStateCheck
		(allowed-values TRUE FALSE)
		(default FALSE))
	(slot userState
		(type STRING))
	(slot todaySpecificState
		(type FLOAT)
		(default 0.0))
	(slot oldSamplePercentage
		(type FLOAT)
		(default 0.0))
	(slot oldSampleSpecific
		(type SYMBOL)
		(allowed-values NAN SUDDEN NORMAL MARGIN_SMALL MARGIN_MEDIUM MARGIN_LARGE ACCEL_APPOSITE ACCEL_EXCESSIVELY)
		(default NAN))
;by sun-chl add 2016/03/07 start
	(slot accelOnTime
		(type INTEGER)
		(default 0))
	(slot accelOffTime
		(type INTEGER)
		(default 0))
	(slot sceneStartTime
		(type FLOAT)
		(default 0.0))
	(slot accumulateTime
		(type FLOAT)
		(default 0.0))
	(slot accelopenSum
		(type FLOAT)
		(default 0.0))
	(slot startDriveScene
		(type SYMBOL))
	(slot steeringAngleMax
		(type FLOAT)
		(default 0.0))
	(slot step
		(type SYMBOL)
		(allowed-values STEP_END STEP_START STEP_CALC)
		(default STEP_END))
;by sun-chl add 2016/03/07 end
;yintf add 20160310
	(slot p23_suddenhandleinfo
		(type FLOAT)
		(default 0.0))
)
;by sun-chl add 2016/03/07 end

;==================================================
; NO-LOOP SUPPORT
;==================================================
(defglobal MAIN
	?*P4_LAST_SAMPLING* = 0.0)

(defglobal MAIN
	?*no-loop-計算:平均/標準偏差TTC・二項検定によるはずれ値判定* = TRUE
	?*no-loop-計算:車間時間短時間へのリスク感覚：平均車間時間・標準偏差車間時間・二項検定によるはずれ値判定* = TRUE
	?*no-loop-追従停車時の平均車間距離の計算* = TRUE
	?*no-loop-高速道路の車間距離の検出/余裕度大* = TRUE
	?*no-loop-高速道路の車間距離の検出/余裕度中* = TRUE
	?*no-loop-高速道路の車間距離の検出/余裕度小* = TRUE
	?*no-loop-高速道路の車間距離計算* = TRUE
	?*no-loop-一般道路の車間距離の検出/余裕度大* = TRUE
	?*no-loop-一般道路の車間距離の検出/余裕度中* = TRUE
	?*no-loop-一般道路の車間距離の検出/余裕度小* = TRUE
	?*no-loop-一般道路の車間距離計算* = TRUE
	?*no-loop-シーン成立時:アクセルオン検出* = TRUE
	?*no-loop-シーン成立時:アクセルオフ検出* = TRUE
	?*no-loop-揺れる運転回数を更新する* = TRUE
	?*no-loop-【前車追従】、車速30㎞/h以上、車間時間_1.65以下_2s未満による測定開始* = TRUE
	?*no-loop-左カーブのシーン成立中の最大舵角を算出する* = TRUE
	?*no-loop-右カーブのシーン成立中の最大舵角を算出する* = TRUE
	?*no-loop-ハンドル操作スピード-ドライバ状態判定* = TRUE   
	?*no-loop-計算:平均/標準偏差車間距離・二項検定によるはずれ値判定* = TRUE
)

(deffunction MAIN::reset-no-loop()
    ;(printout qt "**** MAIN::reset-no-loop ****" crlf)
	(bind ?*no-loop-計算:平均/標準偏差TTC・二項検定によるはずれ値判定* TRUE)
	(bind ?*no-loop-計算:車間時間短時間へのリスク感覚：平均車間時間・標準偏差車間時間・二項検定によるはずれ値判定* TRUE)
	(bind ?*no-loop-追従停車時の平均車間距離の計算* TRUE)
	(bind ?*no-loop-高速道路の車間距離の検出/余裕度大* TRUE)
	(bind ?*no-loop-高速道路の車間距離の検出/余裕度中* TRUE)
	(bind ?*no-loop-高速道路の車間距離の検出/余裕度小* TRUE)
	(bind ?*no-loop-高速道路の車間距離計算* TRUE)
	(bind ?*no-loop-一般道路の車間距離の検出/余裕度大* TRUE)
	(bind ?*no-loop-一般道路の車間距離の検出/余裕度中* TRUE)
	(bind ?*no-loop-一般道路の車間距離の検出/余裕度小* TRUE)
	(bind ?*no-loop-一般道路の車間距離計算* TRUE)
	(bind ?*no-loop-シーン成立時:アクセルオン検出* TRUE)
	(bind ?*no-loop-シーン成立時:アクセルオフ検出* TRUE)
	(bind ?*no-loop-揺れる運転回数を更新する* TRUE)
	(bind ?*no-loop-【前車追従】、車速30㎞/h以上、車間時間_1.65以下_2s未満による測定開始* TRUE)
	(bind ?*no-loop-左カーブのシーン成立中の最大舵角を算出する* TRUE)
	(bind ?*no-loop-右カーブのシーン成立中の最大舵角を算出する* TRUE)
	(bind ?*no-loop-ハンドル操作スピード-ドライバ状態判定* TRUE)
	(bind ?*no-loop-計算:平均/標準偏差車間距離・二項検定によるはずれ値判定* TRUE)
)
;==================================================
; Function
;==================================================
(deffunction MAIN::brmsPower(?x ?y)
	(printout qt "**** MAIN::brmsPower ****" crlf)
	(if (and (= ?x 0) (<= ?y 0)) then
		(return 0.0)
	else
		(return (** ?x ?y)))
)
;Sqrt
(deffunction MAIN::brmsSqrt(?x)
    (printout qt "**** MAIN::brmsSqrt ****" crlf)
	(return (sqrt (abs ?x)))
)

