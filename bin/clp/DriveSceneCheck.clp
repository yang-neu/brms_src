;==================================================
; Function(DriveSceneCheck.clp)
;==================================================
(deffunction SpecificAgenda::driveSceneChangeAction(?driveScene ?accelSpecific ?reduceSpecific)
	(bind ?scene (fact-slot-value ?driveScene driveScene))
	(bind ?value1 (fact-slot-value ?driveScene value1))
	
	(switch ?scene
	(case STOP then)
	(case START then)
	(case RUNNING then)
	(case ACCEL then
		(bind ?specificList (fact-slot-value ?accelSpecific specificList))
		(if (>= ?value1 2.2) then
			(modify ?accelSpecific (specificList SUDDEN ?specificList))
			(printout t "急加速サンプル+1" crlf)
			(printout qt "【加速走行シーン抜け】　急加速サンプル+1　/ ★最大加速度 : " ?value1 crlf)
		else
			(modify ?accelSpecific (specificList NORMAL ?specificList))
			(printout t "通常加速サンプル+1" crlf)
			(printout qt "【加速走行シーン抜け】　通常加速サンプル+1" crlf)))
	(case REDUCE then
		(bind ?specificList (fact-slot-value ?reduceSpecific specificList))
		(if (< ?value1 -1.96) then
			(modify ?reduceSpecific (specificList SUDDEN ?specificList))
			(printout t "急減速サンプル+1" crlf)
			(printout qt "【減速走行シーン抜け】　急減速サンプル+1　/ ★最低加速度 : " ?value1 crlf)
		else
			(modify ?reduceSpecific (specificList NORMAL ?specificList))
			(printout t "通常減速サンプル+1" crlf)
			(printout qt "【減速走行シーン抜け】　通常減速サンプル+1" crlf)))))

(deffunction SpecificAgenda::change-drive-scene(?state ?scene)
	(return (modify ?state (beforeDriveScene (fact-slot-value ?state driveScene)) (driveScene ?scene) (value1 0.0) (value2 0.0) (value3 0.0))))

;==================================================
; Rule(DriveSceneCheck.clp)
;==================================================
(defglobal SpecificAgenda ?*lock-on-active-前周期の運転シーン取得* = TRUE)
(defrule SpecificAgenda::前周期の運転シーン取得
	 (declare (salience 504))
	
	 ?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream"))
	 (test (eq ?*lock-on-active-前周期の運転シーン取得* TRUE))
	 =>
	 (bind ?*lock-on-active-前周期の運転シーン取得* FALSE)
	 (modify ?state (beforeDriveScene (fact-slot-value ?state driveScene))))

;by sun-chl add start 2016/02/29
(defrule SpecificAgenda::シフトポジションRによる後退推移
	(declare (salience 502))
	(EventShiftState (from entryPoint) (name "latestHistory Stream") (type REVERSE) (value REVERSE))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&STOP))
	=>
	(printout t "シフトポジションRによる後退推移：" ?driveScene " -> REVERSE" crlf)
	(printout qt "【運転シーン】 シフトポジションRによる後退推移：" ?driveScene " -> REVERSE" crlf)
	(change-drive-scene ?state REVERSE)
	(bind ?*list* (modify ?*list* (driveScene "後退"))))
;by sun-chl add end 2016/02/29
;by sun-chl chg  2016/02/29 shiftPosition add
(defrule SpecificAgenda::車速0による停車推移
	(declare (salience 503))
	
	(EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1) (speed 0.0))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&~STOP))
	(EventShiftState (from entryPoint) (name "latestHistory Stream") (type REVERSE) (value ~REVERSE))
	?accelSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	?reduceSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_DECELERATION))
	=>
	(printout t "【運転シーン】 車速0による停車推移：" ?driveScene " -> STOP" crlf)
	(driveSceneChangeAction ?state ?accelSpecific ?reduceSpecific)
	(change-drive-scene ?state STOP)
	(bind ?*list* (modify ?*list* (driveScene "停車"))))

;by sun-chl chg 2016/02/29 Shift Position add
(defrule SpecificAgenda::車速が1.6越えによる発進推移
	(declare (salience 502))

	(EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1) (speed ?speed&:(> ?speed 1.6)))
	(EventShiftState (from entryPoint) (name "latestHistory Stream") (type REVERSE) (value ~REVERSE))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&STOP))
	=>
	(printout t "車速が1.6越えによる発進推移：" ?driveScene " -> START" crlf)
	(printout qt "【運転シーン】 車速が1.6越えによる発進推移：" ?driveScene " -> START" crlf)
	(change-drive-scene ?state START)
	(bind ?*list* (modify ?*list* (driveScene "発進"))))

(defrule SpecificAgenda::車速が20越えによる定常走行推移
	(declare (salience 502))
	
	(EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1) (speed ?speed&:(> ?speed 20)))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&START))
	=>
	(printout t "車速が20越えによる定常走行推移：" ?driveScene " -> RUNNING" crlf)
	(printout qt "【運転シーン】 車速が20越えによる定常走行推移：" ?driveScene " -> RUNNING" crlf)
	(change-drive-scene ?state RUNNING)
	(bind ?*list* (modify ?*list* (driveScene "定常走行"))))
	
;sunchl add 2016/03/22
(defrule SpecificAgenda::加速度0.85越のカウントを算出する
	(declare (salience 502))
	
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&RUNNING) (accelAccelerationCount ?accelAccelerationCount))
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration 0.85)))
	=>
	(modify ?state(accelAccelerationCount (+ ?accelAccelerationCount 1)))
	(printout t "加速度0.85越のカウントを算出する：accelAccelerationCount:" (+ ?accelAccelerationCount 1) crlf))
;sunchl add 2016/03/22
(defrule SpecificAgenda::加速度0.85以上のカウントをクリアする
	(declare (salience 502))
	
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&RUNNING) (accelAccelerationCount ?accelAccelerationCount&:(> ?accelAccelerationCount 0)))
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(<= ?acceleration 0.85)))
	=>
	(modify ?state(accelAccelerationCount 0))
	(printout t "加速度0.85以上のカウントをクリアする:" crlf))
	
;sunchl add 2016/03/22
(defrule SpecificAgenda::加速度0.75未満のカウントを算出する
	(declare (salience 502))
	
	(printout qt "**** 加速度0.75未満のカウントを算出する : ******" crlf)

	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&ACCEL) (runningAccelerationCount ?runningAccelerationCount))
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(< ?acceleration 0.75)))
	=>
	(modify ?state(runningAccelerationCount (+ ?runningAccelerationCount 1)))
	(printout t "加速度0.75未満のカウントを算出する：runningAccelerationCount:" (+ ?runningAccelerationCount 1) crlf))
;sunchl add 2016/03/22
(defrule SpecificAgenda::加速度0.75以上のカウントをクリアする
	(declare (salience 502))
	
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&ACCEL) (runningAccelerationCount ?runningAccelerationCount&:(> ?runningAccelerationCount 0)))
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(>= ?acceleration 0.75)))
	=>
	(modify ?state(runningAccelerationCount 0))
	(printout t "加速度0.75以上のカウントをクリアする:" crlf))

(defrule SpecificAgenda::加速度0.85越えによる加速走行推移
	(declare (salience 501))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration 0.85)))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&RUNNING) 
	(accelAccelerationCount ?accelAccelerationCount&:(>= ?accelAccelerationCount 3)))
	=>
	(bind ?state (modify ?state (accelAccelerationCount 0)))
	(printout t "加速度0.85越えによる加速走行推移：" ?driveScene " -> ACCEL" crlf)
	(printout qt "【運転シーン】 加速度0.85越えによる加速走行推移：" ?driveScene " -> ACCEL" crlf)
	(change-drive-scene ?state ACCEL)
	(bind ?*list* (modify ?*list* (driveScene "加速走行"))))

(defrule SpecificAgenda::加速度0.75未満による定常走行推移
	(declare (salience 501))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(< ?acceleration 0.75)))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&ACCEL) 
	(runningAccelerationCount ?runningAccelerationCount&:(>= ?runningAccelerationCount 3)))
	?accelSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	?reduceSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_DECELERATION))
	=>
	(bind ?state (modify ?state (runningAccelerationCount 0)))
	(printout t "加速度0.75未満による定常走行推移：" ?driveScene " -> RUNNING" crlf)
	(printout qt "【運転シーン】 加速度0.75未満による定常走行推移：" ?driveScene " -> RUNNING" crlf)
	(driveSceneChangeAction ?state ?accelSpecific ?reduceSpecific)
	(change-drive-scene ?state RUNNING)
	(bind ?*list* (modify ?*list* (driveScene "定常走行"))))

(defrule SpecificAgenda::アクセル開度が0による減速走行推移
	(declare (salience 500))
	
	(EventAccelOpen (from entryPoint) (name "Current Receiving Data Stream") (type ACCEL_OPEN) (accelOpen 0.0))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&ACCEL|RUNNING))
	?accelSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	?reduceSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_DECELERATION))
	=>
	(printout t "アクセル開度が0による減速走行推移：" ?driveScene " -> REDUCE" crlf)
	(printout qt "【運転シーン】 アクセル開度が0による減速走行推移：" ?driveScene " -> REDUCE" crlf)
	(driveSceneChangeAction ?state ?accelSpecific ?reduceSpecific)
	(change-drive-scene ?state REDUCE)
	(bind ?*list* (modify ?*list* (driveScene "減速走行"))))

(defrule SpecificAgenda::アクセル開度が0越えによる定常走行推移
	(declare (salience 500))
	
	(EventAccelOpen (from entryPoint) (name "Current Receiving Data Stream") (type ACCEL_OPEN) (accelOpen ?accelOpen&:(> ?accelOpen 0.0)))
	?state <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene&REDUCE))
	?accelSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	?reduceSpecific <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_DECELERATION))
	=>
	(printout t "アクセル開度が0越えによる定常走行推移：" ?driveScene " -> RUNNING" crlf)
	(printout qt "【運転シーン】 アクセル開度が0越えによる定常走行推移：" ?driveScene " -> RUNNING" crlf)
	(driveSceneChangeAction ?state ?accelSpecific ?reduceSpecific)
	(change-drive-scene ?state RUNNING)
	(bind ?*list* (modify ?*list* (driveScene "定常走行"))))
