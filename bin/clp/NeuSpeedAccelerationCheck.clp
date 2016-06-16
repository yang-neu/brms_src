﻿;==================================================
; Global value(NewSpeedAccelerationCheck.clp)
;==================================================
; (defglobal SpecificAgenda ?*AccWindow* = (create$))  ;****[] width is the MaxWindow ;Read previouse data from data at IGON,Save current data at IGOFF-
; (defglobal SpecificAgenda ?*SudenWindow* = (create$))  ;****[] width is the MaxWindow ;Read previouse data from data at IGON,Save current data at IGOFF-

(defglobal SpecificAgenda ?*MaxWindow* = 500) ;500
(defglobal SpecificAgenda ?*MaxSampling* = 10) ;100
(defglobal SpecificAgenda ?*AccSamplingCnt* = 0) 
(defglobal SpecificAgenda ?*AccSamplingCntB* = 0) ;IGOFF->save2file IGON->read previous from file
(defglobal SpecificAgenda ?*AccSudCnt* = 0)			;急加速しがちCount
(defglobal SpecificAgenda ?*PreAccel* = 0)			;前回までの最大値
(defglobal SpecificAgenda ?*TauCnt* = 0)
(defglobal SpecificAgenda ?*AccelLimit* = 0.1)     ;加速シーンの閾値 0.75
(defglobal SpecificAgenda ?*SudAccelLimit* = 1.75)     ;急加速の閾値 1.75


(deftemplate MAIN::Profile1Status
	(slot speed
		(type INTEGER))
	(slot accel
		(type INTEGER))

	(slot VihicleState		;車両状態
		(type STRING)
		(default "未定義"))
	(slot AccelJudegment
		(type STRING)
		(default "状態未判定"))  ;急加速しがち/急加速しない
	(slot MaxAccelofScene
		(type INTEGER))
	(slot previousSampling
		(type INTEGER))
	(slot currentSampling
		(type INTEGER))
	(slot PercentageofSudAcc
		(type INTEGER))
	(slot previousAveSpeed
		(type INTEGER)
		(default 0))
	(slot CurrentAveSpeed
		(type INTEGER)
		(default 0))
	(slot Sigma					;分散値
		(type INTEGER))
	(slot Cnt3Sima
		(type INTEGER))
)

(defglobal MAIN
	?*Profile1Data* = (assert (Profile1Status)))

;==================================================
; Function(NewSpeedAccelerationCheck.clp)
;==================================================
 (deffunction SpecificAgenda::count-facts (?template)
 (length (find-all-facts ((?fct ?template)) TRUE)))

(deffunction SpecificAgenda::cntAccSudPercent ()

	(bind ?AccSud 0)
	(bind ?width (count-facts AccelPeakRawDataWithFlag))
	(if (= ?width 0) then
		(return 0))
	
	(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) TRUE)
		(bind ?a (fact-slot-value ?f acceleration))
		(if (> ?a ?*SudAccelLimit*) then
			(bind ?AccSud (+ ?AccSud 1))
		)
	)
	(return (/ ?AccSud ?width))
)

; (deffunction SpecificAgenda::cntAccSudPercent ()

	; (bind ?AccSud 0)
	; (bind ?width (length$ ?*SudenWindow*))
	; (if (= ?width 0) then
		; (return 0))
	
	; (foreach ?data ?*SudenWindow*
		; (bind ?AccSud (+ ?AccSud ?data))
	; )
	; (return (/ ?AccSud ?width))
; )

(deffunction SpecificAgenda::averageAccel()

	(bind ?Accel 0)
	(bind ?width (count-facts AccelPeakRawDataWithFlag))
	(if (= ?width 0) then
		(return 0))
	
	(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) TRUE)
		(bind ?a (fact-slot-value ?f acceleration))
		(bind ?Accel (+ ?Accel ?a))
	)
	(return (/ ?Accel ?width))
)

; (deffunction SpecificAgenda::averageAccel()

	; (bind ?Accel 0)
	; (bind ?width (length$ ?*AccWindow*))
	; (if (= ?width 0) then
		; (return 0))
	
	; (foreach ?data ?*AccWindow*
		; (bind ?Accel (+ ?Accel ?data))
	; )
	; (return (/ ?Accel ?width))
; )

(deffunction SpecificAgenda::accelSigma()

	(bind ?aver (averageAccel))
	(bind ?width (count-facts AccelPeakRawDataWithFlag))
	(if (= ?width 0) then
		(return 0))

	(bind ?bigSigma 0)
	(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) TRUE)
		(bind ?a (fact-slot-value ?f acceleration))
		(bind ?bigSigma (+ ?bigSigma (* (- ?a ?aver) (- ?a ?aver))))
	)
	(return (sqrt(/ ?bigSigma ?width)))
)

(deffunction SpecificAgenda::makeDataWindow (?Accel)
	(assert (AccelPeakRawDataWithFlag (acceleration ?Accel)))
	(bind ?width (count-facts AccelPeakRawDataWithFlag))
	(if (> ?width ?*MaxWindow*) then
		;delete the oldest one
		(foreach ?f (find-fact ((?x AccelPeakRawDataWithFlag)) TRUE)
			(retract ?f)
		)
	)
	
	(printout t "++++++++AccWindow Size/MaxWindow : " ?width "/" ?*MaxWindow* crlf)
)


; (deffunction SpecificAgenda::makeDataWindow (?Accel ?Sud)
	; (bind ?width (length$ ?*AccWindow*))
	; (if (< ?width ?*MaxWindow*) then
		; (bind ?*AccWindow* (insert$ ?*AccWindow* (+ ?width 1) ?Accel))
		; (bind ?*SudenWindow* (insert$ ?*SudenWindow* (+ ?width 1) ?Sud))
	; else ;keep the width of window
		; (bind ?*AccWindow* (delete$ ?*AccWindow* 1 1))
		; (bind ?*AccWindow* (insert$ ?*AccWindow* (+ ?width 1) ?Accel))
		; (bind ?*SudenWindow* (delete$ ?*SudenWindow* 1 1))
		; (bind ?*SudenWindow* (insert$ ?*SudenWindow* (+ ?width 1) ?Sud))
	; )
	
	; (printout t "-----AccWindow : " ?*AccWindow* crlf)
	; (printout qt "-----AccWindow : " ?*AccWindow* crlf)
	; (printout t "-----SudenWindow : " ?*SudenWindow* crlf)
	; (printout qt "-----SudenWindow : " ?*SudenWindow* crlf)
; )


;==================================================
; Rule(NewSpeedAccelerationCheck.clp)
;==================================================
(defrule SpecificAgenda::getMaxValueofAccelScene
	(declare (salience 900))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration ?*AccelLimit*))) ;0.75
		;本来はDriveScene(加速)で判定するべき、Debugため仮に>0.xで判定
		;?b <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ACCEL) (value1 ~?acceleration))
	=>
	;Get the max accelration of the secen
	(if (>= ?acceleration ?*PreAccel*) then
		(bind ?*PreAccel* ?acceleration)
		(printout t "++++++++getMaxValueofAccelScene " ?*PreAccel* crlf)
	)
)
	
(defrule SpecificAgenda::leaveAccelScene　　;加速シーンから抜く
	(declare (salience 900))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(<= ?acceleration ?*AccelLimit*)))
	=>
	(if (> ?*PreAccel* 0) then
		;加速度最大値がある場合
				
		(makeDataWindow ?*PreAccel*)
		
		(MakeCurHistgram)
			
			
		;Sampling Counter++
		;(bind ?*AccSamplingCnt* (+ ?*AccSamplingCnt* 1))
		(bind ?AccSamplingCnt (count-facts AccelPeakRawDataWithFlag))
		
		(printout t "++++++++leaveAccelScene-AccSamplingCnt/MaxSampling " ?AccSamplingCnt "/" ?*MaxSampling* crlf)
		
		;加速シーンの最大加速度
		(bind ?*Profile1Data* (modify ?*Profile1Data* (MaxAccelofScene ?*PreAccel*)))
		
		;これまでのサンプル数
		(bind ?preSampleCnt (length (find-all-facts ((?x AccelPeakRawDataWithFlag)) (> ?x:preFlag 0))))
		(bind ?*Profile1Data* (modify ?*Profile1Data* (previousSampling ?preSampleCnt)))
		
		;今回のサンプル数
		(bind ?curSampleCnt (length (find-all-facts ((?x AccelPeakRawDataWithFlag)) (= ?x:preFlag 0))))
		(bind ?*Profile1Data* (modify ?*Profile1Data* (currentSampling ?curSampleCnt)))
		
		;急加速の割合
		(bind ?percent (cntAccSudPercent))
		(bind ?*Profile1Data* (modify ?*Profile1Data* (PercentageofSudAcc ?percent)))
		
		;これまでの平均
		(if (> ?preSampleCnt 0) then
			
			(bind ?Accel 0)
			(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) (> ?x:preFlag 0))
				(bind ?a (fact-slot-value ?f acceleration))
				(bind ?Accel (+ ?Accel ?a))
			)
			(bind ?*Profile1Data* (modify ?*Profile1Data* (previousAveSpeed (/ ?Accel ?preSampleCnt))))
		else
			(bind ?*Profile1Data* (modify ?*Profile1Data* (previousAveSpeed 0)))
		)
			
		;今回の平均
		(if (> ?curSampleCnt 0) then
			(bind ?Accel 0)
			(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) (= ?x:preFlag 0))
				(bind ?a (fact-slot-value ?f acceleration))
				(bind ?Accel (+ ?Accel ?a))
			)
			(bind ?*Profile1Data* (modify ?*Profile1Data* (currentAveSpeed (/ ?Accel ?curSampleCnt))))
		else
			(bind ?*Profile1Data* (modify ?*Profile1Data* (currentAveSpeed 0)))
		)		
		
		;必要サンプリング数に達して以降、サンプリング値が更新されるたびに、以下の判定をする。
		;加速情報[ ] > 1.7m/s^2 が何％成立しているか判定し、
		(if (> ?AccSamplingCnt ?*MaxSampling*) then 

			;the previous acceleration is the max of this secen
			;'τ＜-3、3＜τならば、はずれ値（いつもと違う状態）として判定し平均値の更新はしない


			;get total of Accsud and judge with 25％以上であれば
			;25％以上であれば、
			(if (> (cntAccSudPercent) 0.25) then
				;ドライバー特性を"急加速しがち"
				(bind ?*Profile1Data* (modify ?*Profile1Data* (AccelJudegment "急加速しがち")))
			else
				;急加速あまりしない
				(bind ?*Profile1Data* (modify ?*Profile1Data* (AccelJudegment "急加速しない")))
			)
		 
		)
		
		(bind ?*PreAccel* 0)
	)
)


(defrule SpecificAgenda::getAccelTau
	(declare (salience 800))

	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(<= ?acceleration ?*AccelLimit*)))
	;且つ、ドライバー特性　<>　未確定
	=>
	
	(bind ?sigma (accelSigma))
	(bind ?aver (averageAccel))
	(bind ?tau (/ (- ?acceleration ?aver) ?sigma))
	
	;τ>3　
	(if (> ?tau 3) then (bind ?*TauCnt* (+ ?*TauCnt* 1)))
	
	(printout t "++++++++getAccelTau tau/TauCnt: " ?tau "/" ?*TauCnt* crlf)
	
	;分散値
	(bind ?*Profile1Data* (modify ?*Profile1Data* (Sigma ?sigma)))
	(bind ?*Profile1Data* (modify ?*Profile1Data* (Cnt3Sima ?*TauCnt*)))
		
	;3σ外数

	(if (> ?*TauCnt* 3) then
	;3回達成と"いつもと違う"と判定する。
	
		;(bind ?*TauCnt* = 0)
	
	)
	
	(bind ?id 2)
   　(send [FIFO] putData ?*Profile1Data* ?id)
   　(printout t "++++++Insert Profile1 data to FIFO:"  ?*Profile1Data* crlf))
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

