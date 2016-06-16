;==================================================
; Global value(NewSpeedAccelerationCheck.clp)
;==================================================
(defglobal MAIN ?*MaxWindow* = 500) ;500
(defglobal MAIN ?*MaxSampling* = 10) ;100
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
	(slot AccelJudgement
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
	(slot currentAveSpeed
		(type INTEGER)
		(default 0))
	(slot Sigma					;分散値
		(type INTEGER))
	(slot Cnt3Sima
		(type INTEGER))
	(slot diffAlways
		(type INTEGER)
		(default 0))  			;0:いつも通り　　1:いつもと違う
)

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




;==================================================
; Rule(NewSpeedAccelerationCheck.clp)
;==================================================
(defrule SpecificAgenda::getMaxValueofAccelScene "加速度0.85越えによる加速走行推移"
	(declare (salience 900))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration 0.85)))
	(EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ACCEL))
	=>
	;Get the max accelration of the secen
	(if (>= ?acceleration ?*PreAccel*) then
		(bind ?*PreAccel* ?acceleration)
		(printout t "++++++++getMaxValueofAccelScene " ?*PreAccel* crlf)
	)
)
	
(defrule SpecificAgenda::leaveAccelScene　　;加速シーンから抜く
	(declare (salience 900))
	
	(EventDriveScene (from entryPoint) (name "Current Scene Stream") (beforeDriveScene ACCEL)) 
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(< ?acceleration 0.75)))
	=>
	(if (> ?*PreAccel* 0) then
		;加速度最大値がある場合
				
		(makeDataWindow ?*PreAccel*)
		
		(MakeCurHistgram)
			
	
		(bind ?AccSamplingCnt (count-facts AccelPeakRawDataWithFlag))
		
		(printout t "++++++++leaveAccelScene-AccSamplingCnt/MaxSampling " ?AccSamplingCnt "/" ?*MaxSampling* crlf)
		
		;加速シーンの最大加速度
		
		;これまでのサンプル数
		(bind ?preSampleCnt (length (find-all-facts ((?x AccelPeakRawDataWithFlag)) (> ?x:preFlag 0))))
		
		;今回のサンプル数
		(bind ?curSampleCnt (length (find-all-facts ((?x AccelPeakRawDataWithFlag)) (= ?x:preFlag 0))))
		
		;急加速の割合
		(bind ?percent (cntAccSudPercent))
		
		;これまでの平均
		(bind ?preAccAvr 0)
		(if (> ?preSampleCnt 0) then
			
			(bind ?Accel 0)
			(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) (> ?x:preFlag 0))
				(bind ?a (fact-slot-value ?f acceleration))
				(bind ?Accel (+ ?Accel ?a))
			)
			(bind ?preAccAvr (/ ?Accel ?preSampleCnt))
			)
			
		;今回の平均
		(bind ?curAccAvr 0)
		(if (> ?curSampleCnt 0) then
			(bind ?Accel 0)
			(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) (= ?x:preFlag 0))
				(bind ?a (fact-slot-value ?f acceleration))
				(bind ?Accel (+ ?Accel ?a))
			)
			(bind ?curAccAvr (/ ?Accel ?curSampleCnt))
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
				(bind ?Judge "急加速しがち")
			else
				;急加速あまりしない
				(bind ?Judge "急加速しない")
			)
		 
		)
		
		;分散値
		(bind ?sigma (accelSigma))
		(bind ?aver (averageAccel))

		;3σ外数
		(bind ?tau (/ (- ?acceleration ?aver) ?sigma))
		
		;|τ|>3　
		(if (> (abs ?tau) 3) then (bind ?*TauCnt* (+ ?*TauCnt* 1)))
		
		(printout t "++++++++getAccelTau tau/TauCnt: " ?tau "/" ?*TauCnt* crlf)
		
			
		(bind ?diffAlways 0)
		(if (and (> ?*TauCnt* 3) ?*ready*) then
		;3回達成と"いつもと違う"と判定する。
			
			(bind ?diffAlways 1)	
		)
		
		(bind ?p1 (assert (Profile1Status   (MaxAccelofScene ?*PreAccel*) 
									(previousSampling ?preSampleCnt) 
									(currentSampling ?curSampleCnt)
									(PercentageofSudAcc ?percent)
									(previousAveSpeed ?preAccAvr)
									(currentAveSpeed ?curAccAvr)
									(AccelJudgement ?Judge)
									(Sigma ?sigma)
									(Cnt3Sima ?*TauCnt*)
									(diffAlways ?diffAlways)
									)))

		(foreach ?n (fact-slot-names ?p1)
			(bind ?*AccelPeakHistList* (insert$ ?*AccelPeakHistList* 1 (fact-slot-value ?p1 ?n)))
		)
		
		
		
		(bind ?id 2)
	   　(send [FIFO] putData ?*AccelPeakHistList* ?id)
	   　(printout t "++++++Insert Profile1 data to FIFO:"  ?*AccelPeakHistList* crlf)
		
		(bind ?*PreAccel* 0)
	)
)



