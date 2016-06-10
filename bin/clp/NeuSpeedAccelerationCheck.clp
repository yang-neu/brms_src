;==================================================
; Global value(NewSpeedAccelerationCheck.clp)
;==================================================
(defglobal SpecificAgenda ?*dataWindow* = (create$))  ;****[] width is the MaxWindow ;Read previouse data from data at IGON,Save current data at IGOFF-

(defglobal SpecificAgenda ?*MaxWindow* = 50) ;100
(defglobal SpecificAgenda ?*MaxSampling* = 50) ;500
(defglobal SpecificAgenda ?*AccSamplingCnt* = 0) 
(defglobal SpecificAgenda ?*AccSamplingCntB* = 0) ;IGOFF->save2file IGON->read previous from file
(defglobal SpecificAgenda ?*AccSudCnt* = 0)			;急加速しがちCount
(defglobal SpecificAgenda ?*PreAccel* = 0)			;前回までの最大値

;==================================================
; Function(NewSpeedAccelerationCheck.clp)
;==================================================
(deffunction SpecificAgenda::cntAccSudPercent ()

	(bind ?AccSud 0)
	(bind ?width (length$ ?*dataWindow*))
	(if (= ?width 0) then
		(return 0)
	else
		(foreach ?data ?*dataWindow*
			(bind ?AccSud (+ ?AccSud (nth$ 2 ?data)))
		)
		(return (/ ?AccSud ?width))
	)
)

(deffunction SpecificAgenda::makeDataWindow (?Accel ?Sud)
	(bind ?width (length$ ?*dataWindow*))
	(if (< ?width ?*MaxWindow*) then
		(bind ?*dataWindow* (insert$ ?*dataWindow* (+ ?width 1) (create$ ?Accel ?Sud)))
	else ;keep the width of window
		(bind ?*dataWindow* (delete$ ?*dataWindow* 1 1))
		(bind ?*dataWindow* (insert$ ?*dataWindow* (+ ?width 1) (create$ ?Accel ?Sud)))
	)
	
	(printout t "-----makeDataWindow : " ?*dataWindow* crlf)
	(printout qt "-----makeDataWindow : " ?*dataWindow* crlf)
)

; (deffunction SpecificAgenda::count-facts-2 (?template)
; (length (find-all-facts ((?fct ?template)) TRUE)))

;==================================================
; Rule(NewSpeedAccelerationCheck.clp)
;==================================================
(defrule SpecificAgenda::getMaxValueofAccelScene
	(declare (salience 900))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration 0.75)))
		;本来はDriveScene(加速)で判定するべき、Debugため仮に>0.xで判定
		;?b <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ACCEL) (value1 ~?acceleration))
	=>
	;Get the max accelration of the secen
	(if (>= ?acceleration ?*PreAccel*) then
		(bind ?*PreAccel* ?acceleration)
	)
)
	
(defrule SpecificAgenda::leaveAccelScene
	(declare (salience 900))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(<= ?acceleration 0.75)))
	=>
	(if (> ?*PreAccel* 0) then
		;加速シーンから抜く
		
		;Sampling Counter++
		(bind ?*AccSamplingCnt* (+ ?*AccSamplingCnt* 1))
		
		;必要サンプリング数に達して以降、サンプリング値が更新されるたびに、以下の判定をする。
		;加速情報[ ] > 1.7m/s^2 が何％成立しているか判定し、
		(if (> (+ ?*AccSamplingCnt* ?*AccSamplingCntB) ?*MaxSampling*) then 
			(if (> ?acceleration 1.72) then
		
				(bind ?Accsud 1) ;急加速しがち:1,
			
			else
		 
				(bind ?Accsud 0) ;急加速あまりしない:0
			)
		 
		)
		
		;the previous acceleration is the max of this secen
		(makeDataWindow ?*PreAccel* ?Accsud)
		(bind ?*PreAccel* 0)
		
		;get total of Accsud and judge with 25％以上であれば
		;25％以上であれば、
		(if (> cntAccSudPercent 0.25) then
			;ドライバー特性を"急加速しがち"
			
		else
			;急加速あまりしない
		)
	)
)


; (defrule SpecificAgenda::MakeAccelerationList
	; (declare (salience 980))
	; ?afterS <- (EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1)); (time ?time2))
	; ?latest <- (EventSpeed (from entryPoint) (name "latestHistory Stream") (type VEHICLE_SPEED_SP1))
	; ?dataList <- (EventSpeedList (from entryPoint) (name "Receiving Data Stream"))
	; ?accList <- (AccelList (from entryPoint) (name "Profile 1"))
	; =>
	; (bind ?beforeS (fact-slot-value ?dataList speedList))

	
	; (if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
		; (bind ?speed1 (fact-slot-value ?afterS speed))
		; (bind ?time1 (fact-slot-value ?afterS time))
	    ; (printout t "multifieldp ?beforeS" ?beforeS crlf)
		; (printout t "len = " (length$ ?beforeS) crlf)
		; (bind ?speed2 (nth$ 3 ?beforeS))
		; (bind ?time2 (nth$ 5 ?beforeS))
		; (bind ?acceleration (getAccelerationValue ?speed1 ?speed2 (- ?time1 ?time2)))
		; (makeDataWindow ?acceleration)
		; (format t "==== speed : %4.1f, accel : %3.2f%n"  ?speed1 ?acceleration)
	; )

	; (printout t "here is SpecificAgenda::MakeAccelerationList" crlf)	

; )


; (defrule SpecificAgenda::CntList
	; (declare (salience 980))
	; ?list <- (EventSpeed )
	; =>
	; (bind ?cnt (count-facts-2 EventSpeed))
	; (printout t "here is SpecificAgenda::CntListList" crlf)	
	; (printout t "cnt = " ?cnt  crlf)	
	; (bind ?*cnt* (+ ?*cnt* 1))
; )


