;==================================================
; Function(SetupCheck.clp)
;==================================================
(deffunction SpecificAgenda::getRoadClassValue(?value)
	(bind ?tmp OPENROAD)

	(switch ?value
	(case 0 then
		(bind ?*list* (modify ?*list* (roadClassSig "高速自動車道路")))
		(bind ?tmp EXPRESSWAY))
	(case 1 then
		(bind ?*list* (modify ?*list* (roadClassSig "都市高速道路")))
		(bind ?tmp EXPRESSWAY))
	(case 2 then
		(bind ?*list* (modify ?*list* (roadClassSig "国道")))
		(bind ?tmp OPENROAD))
	(case 3 then
		(bind ?*list* (modify ?*list* (roadClassSig "主要地方道")))
		(bind ?tmp OPENROAD))
	(case 4 then
		(bind ?*list* (modify ?*list* (roadClassSig "主要地方道（都道府県道）")))
		(bind ?tmp OPENROAD))
	(case 5 then
		(bind ?*list* (modify ?*list* (roadClassSig "一般道(幹線)")))
		(bind ?tmp OPENROAD))
	(case 6 then
		(bind ?*list* (modify ?*list* (roadClassSig "一般道（その他案内道）")))
		(bind ?tmp OPENROAD)))

	(return ?tmp))

(deffunction SpecificAgenda::getAccelerationValue(?after ?before ?delta)
	(return (/ (/ (* (- ?after ?before) 1000) 3600) ?delta)))
;yintf add 20160304
(deffunction SpecificAgenda::getJerkValue(?after ?before1 ?before2 ?delta1 ?delta2)
	(if (and (<> ?delta1 0) (<> ?delta2 0)) then
	(return (/ (- (/ (/ (* (- ?after ?before1) 1000) 3600) ?delta1) (/ (/ (* (- ?before1 ?before2) 1000) 3600) ?delta2)) ?delta1))
	else 
	(return 0)))
(deffunction SpecificAgenda::getSteeringAngleAccelerationValue(?after ?before ?delta)
	(return (/ (/ (* (- ?after ?before) 1000) 3600) ?delta)))
;by sun-chl chg 2016/02/29 TTCの計算方法を変更
(deffunction SpecificAgenda::getTTC(?after ?deltaDistance ?deltaTime)
	(if (or (<= ?deltaDistance 0.0) (= ?after 255)) then
		(return 999))
	(bind ?value1 (/ ?deltaDistance ?deltaTime))
	(if (or (< ?value1 -256.0) (> ?value1 256.0)) then
		(return 999))
	(bind ?value (/ ?after ?value1))
	(if (or (< ?value 0.0) (> ?value 256.0)) then
		(return 999))

	(return ?value))
	
(deffunction SpecificAgenda::getPreviousSpeed(?base ?before)
	(bind ?preTime (- ?base (/ ?before 1000)))
	;(printout t "+++++ preTime" ?preTime crlf)
	(if (< ?preTime 0) then return (create$))
	;(do-for-all-facts ((?s TableSpeed)) (> ?s:time 0.0) (printout t "[getPreviousSpeed]speed=" ?s:speed ", time="?s:time crlf "---" crlf))
	(bind ?list (find-all-facts ((?x TableSpeed)) (> ?preTime ?x:time)))
	(bind ?len (length ?list))
	(if (> ?len 0) then
		;(bind ?data (subseq$ ?list ?len ?len))   ;base-beforeの直前の速度を取得
		(foreach ?data (subseq$ ?list ?len ?len)
			(bind ?preSpeed (fact-slot-value ?data speed))
			(bind ?preTime (fact-slot-value ?data time))
			(return (create$ dumy1 dumy2 ?preSpeed dumy4 ?preTime))
		)
	else
		(return (create$))
	)
)
(deffunction SpecificAgenda::getPreviousDistance(?base ?before)
	(bind ?preTime (- ?base (/ ?before 1000)))
	;(printout t "+++++ preTime" ?preTime crlf)
	(if (< ?preTime 0) then return (create$))
	;(do-for-all-facts ((?s TableDistance)) (> ?s:time 0.0) (printout t "[getPreviousDistance]distance=" ?s:distance ", time="?s:time crlf "---" crlf))
	(bind ?list (find-all-facts ((?x TableDistance)) (> ?preTime ?x:time)))
	(bind ?len (length ?list))
	(if (> ?len 0) then
		(foreach ?data (subseq$ ?list ?len ?len)
			(bind ?preDistance (fact-slot-value ?data distance))
			(bind ?preTime (fact-slot-value ?data time))
			(return (create$ dumy1 dumy2 ?preDistance dumy4 ?preTime))
		)
	else
		(return (create$))
	)
)
(deffunction SpecificAgenda::getPreviousSteeringAngle(?base ?before)
	(bind ?preTime (- ?base (/ ?before 1000)))
	;(printout t "+++++ preTime" ?preTime crlf)
	(if (< ?preTime 0) then return (create$))
	;(do-for-all-facts ((?s TableSteeringAngle)) (> ?s:time 0.0) (printout t "[getPreviousSteeringAngle]steeringAngle=" ?s:steeringAngle ", time="?s:time crlf "---" crlf))
	(bind ?list (find-all-facts ((?x TableSteeringAngle)) (> ?preTime ?x:time)))
	(bind ?len (length ?list))
	(if (> ?len 0) then
		(foreach ?data (subseq$ ?list ?len ?len)
			(bind ?preSteeringAngle (fact-slot-value ?data steeringAngle))
			(bind ?preTime (fact-slot-value ?data time))
			(return (create$ dumy1 dumy2 ?preSteeringAngle dumy4 ?preTime))
		)
	else
		(return (create$))
	)
)
	
;==================================================
; Rule(SetupCheck.clp)
;==================================================
(defrule MAIN::初回モデルの登録
	(declare (salience 999) (auto-focus TRUE))
	
	;判定イベント系
	(not (EventSpeed (from entryPoint) (name "Driving Hitory Stream") (type VEHICLE_SPEED_SP1)))
	(not (EventSpeed (from entryPoint) (name "latestHistory Stream") (type VEHICLE_SPEED_SP1)))	
	(not (EventDistance (from entryPoint) (name "Driving Hitory Stream") (type VEHICLE_FOLLOWING_DISTANCE)))
	(not (EventDistance (from entryPoint) (name "latestHistory Stream") (type VEHICLE_FOLLOWING_DISTANCE)))
	(not (EventDistanceDiff (from entryPoint) (name "Driving Hitory Stream")))
	(not (EventRoadClass (from entryPoint) (name "latestHistory Stream") (type ROAD_CLASS)))
	;シーン系
	(not (EventDriveScene (from entryPoint) (name "Current Scene Stream")))
	(not (EventFrontCarStat (from entryPoint) (name "Current Scene Stream")))
;yintf add 20160304
	(not (EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream")))
	(not (EventSteeringAngle (from entryPoint) (name "Driving Hitory Stream") (type STEERING_ANGLE)))
	(not (EventSteeringAngle (from entryPoint) (name "latestHistory Stream") (type STEERING_ANGLE)))	

	=>
	; for DEBUG
	;(watch facts EventSpeedList EventAccelOpenList EventDistanceList EventSteeringAngleList EventShiftStateList)
	
	;(entry-point create "Driving Hitory Stream")
	
	(bind ?eventSpeed (assert (EventSpeed (name "Driving Hitory Stream") (type VEHICLE_SPEED_SP1))))
	;(EventSpeedHistory insert ?eventSpeed to entry-point "Driving Hitory Stream")
	(bind ?eventDistance (assert (EventDistance (name "Driving Hitory Stream") (type VEHICLE_FOLLOWING_DISTANCE))))
	;(EventDistanceHistory insert ?eventDistance to entry-point "Driving Hitory Stream")
	(bind ?eventDistanceDiff (assert (EventDistanceDiff (name "Driving Hitory Stream"))))
	;(EventDistanceDiffHistory insert ?eventDistanceDiff to entry-point "Driving Hitory Stream")
;yintf add 20160304
	(bind ?eventSteeringAngle (assert (EventSteeringAngle (name "Driving Hitory Stream") (type STEERING_ANGLE))))
	;(EventSteeringAngleHistory insert ?eventSteeringAngle to entry-point "Driving Hitory Stream")
	
	;by sun-chl add start 2016/02/29　EventShiftState　add
	(assert (EventShiftState (name "latestHistory Stream") (type REVERSE)))
	(assert (EventSpeed (name "latestHistory Stream") (type VEHICLE_SPEED_SP1)))
	(assert (EventDistance (name "Driving Hitory Stream") (type VEHICLE_FOLLOWING_DISTANCE)))
	(assert (EventDistance (name "latestHistory Stream") (type VEHICLE_FOLLOWING_DISTANCE)))
	(assert (EventDistanceDiff (name "Driving Hitory Stream")))
	(assert (EventRoadClass (name "latestHistory Stream") (type ROAD_CLASS)))
	(assert (EventDriveScene (name "Current Scene Stream")))
;yintf add 20160304
	(assert (EventSteeringAngle (name "latestHistory Stream") (type STEERING_ANGLE)))
	(assert (EventVehiclePostureScene (name "Current Scene Stream")))

	(assert (EventFrontCarStat (name "Current Scene Stream")))
	
	)
;by sun-chl add start 2016/02/29
(defrule SpecificAgenda::最新のシフト状態情報を取り出す
	(declare (salience 990))
	?y <- (EventShiftStateList (from entryPoint) (name "Receiving Data Stream"))
	?shift <- (EventShiftState (from entryPoint) (name "latestHistory Stream") (type REVERSE))
	(not (EventShiftState (from entryPoint) (name "Current Receiving Data Stream") (type REVERSE)))
	=>
	(bind ?dataList (fact-slot-value ?y shiftStateList))
	(bind ?length (length$ ?dataList))
	(bind ?lastest (subseq$ ?dataList (- ?length 2) ?length))
	(bind ?time (nth$ 1 ?lastest))
	(bind ?type (nth$ 2 ?lastest))
	(bind ?value (nth$ 3 ?lastest))
	(retract ?shift)
	(bind ?new (assert (EventShiftState (name "Current Receiving Data Stream") (value ?value) (time ?time) (type ?type))))
	(duplicate ?new (name "latestHistory Stream"))
	(bind ?*list* (modify ?*list* (shiftPosition ?value))))
;by sun-chl end start 2016/02/29

;yintf add 20160304
(defrule SpecificAgenda::最新のハンドル舵角情報を取り出す
	(declare (salience 990))
	
	?y <- (EventSteeringAngleList (from entryPoint) (name "Receiving Data Stream"))
	(not (EventSteeringAngle (from entryPoint) (name "Current Receiving Data Stream") (type STEERING_ANGLE)))
	=>
	;(bind ?dataList (fact-slot-value ?y steeringAngleList))
	;(bind ?length (length$ ?dataList))
	;(bind ?lastest (subseq$ ?dataList (- ?length 2) ?length))
		
	;(bind ?time (nth$ 1 ?lastest))
	;(bind ?type (nth$ 2 ?lastest))
	;(bind ?steeringangle (nth$ 3 ?lastest))

	(bind ?allSteeringAngle (find-all-facts ((?x TableSteeringAngle)) TRUE))
	(bind ?len (length$ ?allSteeringAngle))
	(bind ?latests (subseq$ ?allSteeringAngle ?len ?len))
	(bind ?steeringangle (fact-slot-value (expand$ ?latests) steeringAngle))
	(bind ?time (fact-slot-value (expand$ ?latests) time))

	(printout qt "**** 最新のハンドル舵角情報を取り出す : " ?steeringangle " ******" crlf)

	(assert (EventSteeringAngle (name "Current Receiving Data Stream") (value ?steeringangle) (time ?time) (type STEERING_ANGLE)))
	(bind ?*list* (modify ?*list* (steeringangle ?steeringangle)))

	;10秒前のfactは削除する
	;(do-for-all-facts ((?t TableSpeed)) (< ?t:time (- ?time 10.0)) (retract ?t))
	)

(defrule SpecificAgenda::最新の速度情報を取り出す
	(declare (salience 990))
	
	?y <- (EventSpeedList (from entryPoint) (name "Receiving Data Stream"))
	(not (EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1)))
	=>
	; (bind ?dataList (fact-slot-value ?y speedList))
	; (bind ?length (length$ ?dataList))
	; (bind ?lastest (subseq$ ?dataList (- ?length 2) ?length))
	; (bind ?time (nth$ 1 ?lastest))
	; (bind ?type (nth$ 2 ?lastest))
	; (bind ?speed (nth$ 3 ?lastest))
	
	
	(bind ?allSpeed (find-all-facts ((?x TableSpeed)) TRUE))
	(bind ?len (length$ ?allSpeed))
	(bind ?latests (subseq$ ?allSpeed ?len ?len))
	(bind ?speed (fact-slot-value (expand$ ?latests) speed))
	(bind ?time (fact-slot-value (expand$ ?latests) time))
	
	(printout t "+++++ Get lastest speed : " ?speed " ******" crlf)
	
	(assert (EventSpeed (name "Current Receiving Data Stream") (speed ?speed) (time ?time) ))
	(bind ?*list* (modify ?*list* (speed ?speed)))
	(printout eventoutput (str-cat "speed" " " ?speed))
	
	;10秒前のfactは削除する
	;(do-for-all-facts ((?t TableSpeed)) (< ?t:time (- ?time 10.0)) (retract ?t))
	
)

(defrule SpecificAgenda::最新のアクセル開度情報を取り出す
	(declare (salience 990))
	
	?y <- (EventAccelOpenList (from entryPoint) (name "Receiving Data Stream"))
	(not (EventAccelOpen (from entryPoint) (name "Current Receiving Data Stream") (type ACCEL_OPEN)))
	=>
	(bind ?dataList (fact-slot-value ?y accelOpenList))
	(bind ?length (length$ ?dataList))
	(bind ?lastest (subseq$ ?dataList (- ?length 2) ?length))
	
	(bind ?time (nth$ 1 ?lastest))
	(bind ?type (nth$ 2 ?lastest))
	(bind ?accelOpen (/ (nth$ 3 ?lastest) 2.0))
	
	(assert (EventAccelOpen (name "Current Receiving Data Stream") (accelOpen ?accelOpen) (time ?time) (type ?type)))
	(bind ?*list* (modify ?*list* (accelOpen ?accelOpen))))

(defrule SpecificAgenda::最新の車間距離情報を取り出す
	(declare (salience 990))
	
	?y <- (EventDistanceList (from entryPoint) (name "Receiving Data Stream"))
	(not (EventDistance (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_FOLLOWING_DISTANCE)))
	=>
	;(bind ?dataList (fact-slot-value ?y distanceList))
	;(bind ?length (length$ ?dataList))
	;(bind ?lastest (subseq$ ?dataList (- ?length 2) ?length))
	
	;(bind ?time (nth$ 1 ?lastest))
	;(bind ?type (nth$ 2 ?lastest))
	;(bind ?distance (nth$ 3 ?lastest))

	(bind ?allDistance (find-all-facts ((?x TableDistance)) TRUE))
	(bind ?len (length$ ?allDistance))
	(bind ?latests (subseq$ ?allDistance ?len ?len))
	(bind ?distance (fact-slot-value (expand$ ?latests) distance))
	(bind ?time (fact-slot-value (expand$ ?latests) time))
	
	(assert (EventDistance (name "Current Receiving Data Stream") (distance ?distance) (time ?time) (type VEHICLE_FOLLOWING_DISTANCE)))
	(bind ?*list* (modify ?*list* (distance ?distance)))

	;10秒前のfactは削除する
	;(do-for-all-facts ((?t TableDistance)) (< ?t:time (- ?time 10.0)) (retract ?t)))
	)

(defrule SpecificAgenda::最新の走行道路情報を取り出す
	(declare (salience 990))
	
	?y <- (EventRoadClassList (from entryPoint) (name "Receiving Data Stream"))
	(not (EventRoadClass (from entryPoint) (name "Current Receiving Data Stream") (type ROAD_CLASS)))
	?latest <- (EventRoadClass (from entryPoint) (name "latestHistory Stream"))
	=>
	(bind ?dataList (fact-slot-value ?y roadClassList))
	(bind ?length (length$ ?dataList))
	(bind ?lastest (subseq$ ?dataList (- ?length 3) ?length))
	
	(bind ?time (nth$ 1 ?lastest))
	(bind ?type (nth$ 2 ?lastest))
	(bind ?class (nth$ 3 ?lastest))
	(bind ?canData (nth$ 4 ?lastest))
	
	(bind ?roadClass (getRoadClassValue ?canData))
	(bind ?x (assert (EventRoadClass (name "Current Receiving Data Stream") (canData ?canData) (time ?time) (type ?type) (roadClass ?roadClass))))
	(retract ?latest)
	(duplicate ?x (name "latestHistory Stream")))

(defrule SpecificAgenda::走行道路を判断する:高速道路
	(declare (salience 980))
	
	?x <- (EventRoadClass (name "Current Receiving Data Stream") (type ROAD_CLASS) (roadClass EXPRESSWAY))
	=>
	(bind ?*list* (modify ?*list* (roadClass "高速道路"))))

(defrule SpecificAgenda::走行道路を判断する:一般道路
	(declare (salience 980))

	?x <- (EventRoadClass (name "Current Receiving Data Stream") (type ROAD_CLASS) (roadClass OPENROAD))
	=>
	(bind ?*list* (modify ?*list* (roadClass "一般道路"))))

(defrule SpecificAgenda::加速度を計算する
	(declare (salience 980))
	
	?afterS <- (EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1) (time ?time1))
	?latest <- (EventSpeed (from entryPoint) (name "latestHistory Stream") (type VEHICLE_SPEED_SP1))
	(not (EventAcceleration (from entryPoint) (name "Calculation Stream")))
	=>
	(bind ?beforeS (getPreviousSpeed ?time1 500.00))
	;(bind ?beforeS (EventSpeedHistory query before[500ms] ?time1 from entry-point "Driving Hitory Stream"))
	(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
		(bind ?speed1 (fact-slot-value ?afterS speed))
		(bind ?time1 (fact-slot-value ?afterS time))
		(bind ?speed2 (nth$ 3 ?beforeS))
		(bind ?time2 (nth$ 5 ?beforeS))
		
		(bind ?acceleration (getAccelerationValue ?speed1 ?speed2 (- ?time1 ?time2)))
		(assert (EventAcceleration (name "Calculation Stream") (acceleration ?acceleration)))
		(format qt "==== speed : %4.1f, accel : %3.2f%n"  ?speed1 ?acceleration)
		(printout t "+++++ speed : " ?speed1 ", accel : " ?acceleration " ******" crlf)
		
		;(EventSpeedHistory insert ?afterS to entry-point "Driving Hitory Stream")
		
		(retract ?latest);
		(duplicate ?afterS (name "latestHistory Stream"))
		
		(bind ?*list* (modify ?*list* (accel ?acceleration)))))

(defrule SpecificAgenda::車間距離差分を計算する
	(declare (salience 980))
	
	?afterS <- (EventDistance (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_FOLLOWING_DISTANCE) (time ?time1))
	?latest <- (EventDistance (from entryPoint) (name "latestHistory Stream") (type VEHICLE_FOLLOWING_DISTANCE))
	(not (EventDistanceDiff (from entryPoint) (name "Calculation Stream") (span ONE_SEC)))
	=>
	;(bind ?beforeS (EventDistanceHistory query before[900ms] ?time1 from entry-point "Driving Hitory Stream"))
	(bind ?beforeS (getPreviousDistance ?time1 900.00))
	(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
		(bind ?distance1 (fact-slot-value ?afterS distance))
		(bind ?time1 (fact-slot-value ?afterS time))
		(bind ?distance2 (nth$ 3 ?beforeS))
		(bind ?time2 (nth$ 5 ?beforeS))
		
		(bind ?TTC (getTTC ?distance1 (- ?distance2 ?distance1) (- ?time1 ?time2)))
		
		(bind ?sde (assert (EventDistanceDiff (name "Calculation Stream")
			(distanceDiff (- ?distance1 ?distance2))
			(beforeDistance ?distance2)
			(nowDistance ?distance1)
			(TTC ?TTC)
			(time ?time1)
			(span ONE_SEC))))
		;(EventDistanceDiffHistory insert ?sde to entry-point "Driving Hitory Stream")
		;(EventDistanceHistory insert ?afterS to entry-point "Driving Hitory Stream")
		
		(retract ?latest)
		(duplicate ?afterS (name "latestHistory Stream"))
		(bind ?*list* (modify ?*list* (TTC ?TTC)))))

(defrule SpecificAgenda::100ms前の車間距離差分を計算する
	(declare (salience 980))
	
	?afterS <- (EventDistance (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_FOLLOWING_DISTANCE) (time ?time1))
	(not (EventDistanceDiff (from entryPoint) (name "Calculation Stream") (span HUNDRED_MILLISEC)))
	=>
	;(bind ?beforeS (EventDistanceHistory query before[100ms] ?time1 from entry-point "Driving Hitory Stream"))
	(bind ?beforeS (getPreviousDistance ?time1 100.00))
	(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
		(bind ?distance1 (fact-slot-value ?afterS distance))
		(bind ?time1 (fact-slot-value ?afterS time))
		(bind ?distance2 (nth$ 3 ?beforeS))
		(bind ?time2 (nth$ 5 ?beforeS))
		
		(bind ?TTC (getTTC ?distance1 (- ?distance2 ?distance1) (- ?time1 ?time2)))
		
		(bind ?sde (assert (EventDistanceDiff (name "Calculation Stream")
			(distanceDiff (- ?distance1 ?distance2))
			(beforeDistance ?distance2)
			(nowDistance ?distance1)
			(TTC ?TTC)
			(time ?time1)
			(span HUNDRED_MILLISEC))))))

(defrule SpecificAgenda::500ms前の車間距離差分を計算する
	(declare (salience 980))
	
	?afterS <- (EventDistance (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_FOLLOWING_DISTANCE) (time ?time1))
	(not (EventDistanceDiff (from entryPoint) (name "Calculation Stream") (span FIVE_HUNDRED_MILLISEC)))
	=>
	;(bind ?beforeS (EventDistanceHistory query before[500ms] ?time1 from entry-point "Driving Hitory Stream"))
	(bind ?beforeS (getPreviousDistance ?time1 500.00))
	(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
		(bind ?distance1 (fact-slot-value ?afterS distance))
		(bind ?time1 (fact-slot-value ?afterS time))
		(bind ?distance2 (nth$ 3 ?beforeS))
		(bind ?time2 (nth$ 5 ?beforeS))
		
		(bind ?TTC (getTTC ?distance1 (- ?distance2 ?distance1) (- ?time1 ?time2)))
		
		(bind ?sde (assert (EventDistanceDiff (name "Calculation Stream")
			(distanceDiff (- ?distance1 ?distance2))
			(beforeDistance ?distance2)
			(nowDistance ?distance1)
			(TTC ?TTC)
			(time ?time1)
			(span FIVE_HUNDRED_MILLISEC))))))
;yintf modify 20160323
(defrule SpecificAgenda::ジャークを計算する
	(declare (salience 980))
	(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture ?posture&TURN_STRAIGHT))
	?y <- (EventSpeedList (from entryPoint) (name "Receiving Data Stream") (speedList $?speedList&:(>= (length$ ?speedList) 3)))
	(not (EventJerk (from entryPoint) (name "Calculation Stream")))
	=>
    (bind ?jerk_value 0.0)
    (bind ?Jerk_eValue 0.0)
    (bind ?length 0)
    (if (multifieldp ?speedList) then
	(bind ?length (length$ ?speedList)))
 	(loop-for-count (?i 1 (/ ?length 3)) do
    	(bind ?eventspeed (subseq$ ?speedList (- (- ?length 2) (* (- ?i 1) 3)) (- ?length (* (- ?i 1) 3))))
	    (bind ?time1 (nth$ 1 ?eventspeed))
	    (bind ?speed1 (nth$ 3 ?eventspeed))
        (if (>= ?speed1 20.0) then
    		;(bind ?beforeS (EventSpeedHistory query before[500ms] ?time1 from entry-point "Driving Hitory Stream"))
    		;(bind ?moreBeforeS (EventSpeedHistory query before[1000ms] ?time1 from entry-point "Driving Hitory Stream"))
    		(bind ?beforeS (getPreviousSpeed ?time1 500.00))
    		(bind ?moreBeforeS (getPreviousSpeed ?time1 1000.00))
    		(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5) (multifieldp ?moreBeforeS) (>= (length$ ?moreBeforeS) 5)) then
    			(bind ?speed2 (nth$ 3 ?beforeS))
    			(bind ?time2 (nth$ 5 ?beforeS))
    			(bind ?speed3 (nth$ 3 ?moreBeforeS))
    			(bind ?time3 (nth$ 5 ?moreBeforeS))
    			
	    		(bind ?jerk (getJerkValue ?speed1 ?speed2 ?speed3 (- ?time1 ?time2) (- ?time2 ?time3)))
	            (if (= ?i 1) then
	                (bind ?jerk_value ?jerk))
	    		(if (>= (abs ?jerk) 0.431) then
	                (bind ?Jerk_eValue ?jerk)
	                (break)))))
    (if (<> ?Jerk_eValue 0.0) then        
        (assert (EventJerk (name "Calculation Stream") (value ?Jerk_eValue)))
    else
        (assert (EventJerk (name "Calculation Stream") (value ?jerk_value)))))

;yintf modify 20160323
(defrule SpecificAgenda::舵角加速度を計算する
	(declare (salience 980))
	?afterS <- (EventSteeringAngle (name "Current Receiving Data Stream") (type STEERING_ANGLE) (time ?time1))
	?latest <- (EventSteeringAngle (name "latestHistory Stream") (type STEERING_ANGLE))
	(not (EventSteeringAngleAcceleration (from entryPoint) (name "Calculation Stream")))
	=>
	;(bind ?beforeS (EventSteeringAngleHistory query before[100ms] ?time1 from entry-point "Driving Hitory Stream"))
	(bind ?beforeS (getPreviousSteeringAngle ?time1 100.00))
	(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
    (bind ?steeringangle1 (fact-slot-value ?afterS value))
	(bind ?time1 (fact-slot-value ?afterS time))
	(bind ?steeringangle2 (nth$ 3 ?beforeS))
	(bind ?time2 (nth$ 5 ?beforeS))
	(bind ?acceleration (getSteeringAngleAccelerationValue ?steeringangle1 ?steeringangle2 (- ?time1 ?time2)))
	(assert (EventSteeringAngleAcceleration (name "Calculation Stream") (steeringAngleAcceleration ?acceleration)))
	(format t "==== steeringangle : %4.1f, steeringAngleAcceleration : %3.2f%n" ?steeringangle1 ?acceleration)
	
	(retract ?latest);
	(duplicate ?afterS (name "latestHistory Stream"))

	(bind ?*list* (modify ?*list* (steeringAngleAccel ?acceleration)))))

(defrule SpecificAgenda::古いTableSppedを削除する
	(declare (salience -99))
	(EventSpeedList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(bind ?tartgetTime (- (time) ?*TableHistorySaveTime*))
	(do-for-all-facts ((?t TableSpeed)) (< ?t:time ?tartgetTime) (retract ?t))
)
(defrule SpecificAgenda::古いTableDistanceを削除する
	(declare (salience -99))
	(EventDistanceList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(bind ?tartgetTime (- (time) ?*TableHistorySaveTime*))
	(do-for-all-facts ((?t TableDistance)) (< ?t:time ?tartgetTime) (retract ?t))
)
(defrule SpecificAgenda::古いTableSteeringAngleを削除する
	(declare (salience -99))
	(EventSteeringAngleList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(bind ?tartgetTime (- (time) ?*TableHistorySaveTime*))
	(do-for-all-facts ((?t TableSteeringAngle)) (< ?t:time ?tartgetTime) (retract ?t))
)
(defrule SpecificAgenda::Receiving_Data_StreamのspeedObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 1 ****" crlf)
	?x <- (EventSpeedList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのeventAccelOpenを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 2 ****" crlf)
	?x <- (EventAccelOpenList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのEventDistanceを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 3 ****" crlf)
	?x <- (EventDistanceList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのEventRoadClassを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 4 ****" crlf)
	?x <- (EventRoadClassList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのSteeringAngleObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 5 ****" crlf)
	?x <- (EventSteeringAngleList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのshiftPositionObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 6 ****" crlf)
	
	?x <- (EventShiftStateList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのaimlessObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 7 ****" crlf)
	
	?x <- (EventAimlessList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのlevelDataObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 8 ****" crlf)
	?x <- (EventLevelDataList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのbrakePressureObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 9 ****" crlf)
	?x <- (EventBrakePressureList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのturnSignalObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 10 ****" crlf)
	?x <- (EventTurnSignalList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Receiving_Data_StreamのdriverFaceObjectを全て削除する
	(declare (salience -100))
	;(printout qt "**** MAIN::reset-no-loop 11 ****" crlf)
	?x <- (EventDriverFaceList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(retract ?x)
	(reset-no-loop))

(defrule SpecificAgenda::Current_Receiving_Data_StreamのeventAccelOpenを全て削除する
	(declare (salience -200))
	
	?e <- (EventAccelOpen (from entryPoint) (name "Current Receiving Data Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Current_Receiving_Data_StreamのspeedObjectを全て削除する
	(declare (salience -200))
	
	?e <- (EventSpeed (from entryPoint) (name "Current Receiving Data Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Current_Receiving_Data_StreamのEventRoadClassを全て削除する
	(declare (salience -200))
	
	?e <- (EventRoadClass (from entryPoint) (name "Current Receiving Data Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Current_Receiving_Data_StreamのEventDistanceを全て削除する
	(declare (salience -200))
	
	?e <- (EventDistance (from entryPoint) (name "Current Receiving Data Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Current_Receiving_Data_StreamのSteeringAngleObjectを全て削除する
	(declare (salience -200))
	
	?e <- (EventSteeringAngle (from entryPoint) (name "Current Receiving Data Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Current_Receiving_Data_StreamのshiftPositionObjectを全て削除する
	(declare (salience -200))
	?e <- (EventShiftState (from entryPoint) (name "Current Receiving Data Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Calculation_StreamのeventAccelerationを全て削除する
	(declare (salience -300))
	
	?acce <- (EventAcceleration (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?acce))

(defrule SpecificAgenda::Calculation_StreamのEventDistanceDiffを全て削除する
	(declare (salience -300))
	
	?e <- (EventDistanceDiff (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Calculation_StreamのeventJerkを全て削除する
	(declare (salience -300))
	
	?jerk <- (EventJerk (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?jerk))

(defrule SpecificAgenda::Calculation_StreamのeventSteeringAngleAccelerationを全て削除する
	(declare (salience -300))
	
	?acce <- (EventSteeringAngleAcceleration (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?acce))

(defrule SpecificAgenda::Calculation_StreamのEventCharacteristicAndStatusBinomialを全て削除する
	(declare (salience -400))
	
	;(printout qt "**** Calculation_StreamのEventCharacteristicAndStatusBinomialを全て削除する : ******" crlf)

	?e <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda10s::SpecificAgenda10sのCalculation_StreamのEventCharacteristicAndStatusBinomialを全て削除する
	(declare (salience -400))
	
	?e <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda::Calculation_StreamのEventCharacteristicAndStatus3sigmaを全て削除する
	(declare (salience -400))
	
	?e <- (EventCharacteristicAndStatus3sigma (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?e))

(defrule SpecificAgenda10s::SpecificAgenda10sのCalculation_StreamのEventCharacteristicAndStatus3sigmaを全て削除する
	(declare (salience -400))
	
	?e <- (EventCharacteristicAndStatus3sigma (from entryPoint) (name "Calculation Stream"))
	=>
	(retract ?e))
