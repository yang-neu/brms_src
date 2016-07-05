;==================================================
; Class(AccelPeakHist.clp)
; Add by liusiping@2016/06/10
;==================================================
(defclass container (is-a USER)
(slot start
	(type INTEGER)
	(default 1))
(slot end
	(type INTEGER)
	(default 1))
(slot count
	(type INTEGER)
	(default 0))
(multislot jarlist
	(access read-only) 
	(default jar1 jar2 jar3 jar4 jar5 jar6 jar7 jar8 jar9 jar10))
(multislot jar1)
(multislot jar2)
(multislot jar3)
(multislot jar4)
(multislot jar5)
(multislot jar6)
(multislot jar7)
(multislot jar8)
(multislot jar9)
(multislot jar10))

(defmessage-handler container getData () 
    (printout t "***getData start***"  crlf)
	 (if (= ?self:count 0) then
         (printout t "***FIFO is empty!***" crlf)
         (return (create$))
      else
         (bind ?jarname (nth$ ?self:start ?self:jarlist))
	     (bind ?self:start (+ ?self:start 1))
	     (if(> ?self:start 10) then (bind ?self:start 1))
	     (bind ?self:count (- ?self:count 1))
	     (return (dynamic-get ?jarname))
	)
	(printout t "***getData end***"  crlf)
)

(defmessage-handler container putData (?inputData ?id) 
    (printout t "***setData start***"  crlf)
	(if (= ?self:count 10) then
		(printout t "***FIFO is full!***" crlf)
		(bind ?jarname (nth$ ?self:end ?self:jarlist))
		(bind ?self:start (+ ?self:start 1))
		(if(> ?self:start 10) then (bind ?self:start 1))
		(bind ?self:end (+ ?self:end 1))
		(if(> ?self:end 10) then (bind ?self:end 1))
		(bind ?inputData (insert$ ?inputData 1 ?id))
		(dynamic-put ?jarname ?inputData)
	else
		(bind ?jarname (nth$ ?self:end ?self:jarlist))
		(bind ?self:end (+ ?self:end 1))
		(if(> ?self:end 10) then (bind ?self:end 1))
		(bind ?self:count (+ ?self:count 1))
		(bind ?inputData (insert$ ?inputData 1 ?id))
		(dynamic-put ?jarname ?inputData)
	)	
	(printout t "***setData end***"  crlf)
)
  

;==================================================
; Template&global(AccelPeakHist.clp)
; Add by liusiping@2016/06/08
;==================================================
(deftemplate MAIN::AccelPeakHist
	(slot acceleration
		(type FLOAT)
		(default 0.0))
	(slot count
		(type INTEGER)
		(default 0))
)
		
(deftemplate MAIN::AccelPeakRawData
	(slot acceleration
		(type FLOAT)
		(default 0.0)))

(deftemplate MAIN::AccelPeakRawDataWithFlag
	(slot acceleration
		(type FLOAT)
		(default 0.0))
	(slot preFlag
		(type INTEGER)
		(default 0)		;0:this time data  >0: previouse data
	)	
)

(deftemplate MAIN::Profile1Status
	(slot speed
		(type FLOAT))
	(slot accel
		(type FLOAT))

	(slot VihicleState		;車両状態
		(type STRING)
		(default "未定義"))
	(slot AccelJudgement
		(type STRING)
		(default "状態未判定"))  ;急加速しがち/急加速しない
	(slot MaxAccelofScene
		(type FLOAT))
	(slot previousSampling
		(type INTEGER))
	(slot currentSampling
		(type INTEGER))
	(slot PercentageofSudAcc
		(type FLOAT))
	(slot previousAveSpeed
		(type FLOAT)
		(default 0.0))
	(slot currentAveSpeed
		(type FLOAT)
		(default 0.0))
	(slot Sigma					;分散値
		(type FLOAT))
	(slot Cnt3Sima
		(type INTEGER))
	(slot diffAlways
		(type INTEGER)
		(default 0))  			;0:いつも通り　　1:いつもと違う
)

(deftemplate MAIN::accelFileOprationEvent
	(slot status         	;0: Save 1:Load
		(type INTEGER)
		(default 1)
	)
)

(deftemplate MAIN::TableSpeed
	(slot speed 
		(type FLOAT)
		(default 0.0))
	(slot time
		(type FLOAT)
		;(default 0.0)
		(default-dynamic (time))
	)
)

;==================================================
; Global value(AccelPeakHist.clp)
;==================================================
(defglobal MAIN ?*MaxWindow* = 500) ;500
(defglobal MAIN ?*MaxSampling* = 10) ;100
(defglobal MAIN ?*ready* = FALSE)
(defglobal MAIN ?*MaxWindow* = 500) ;500
(defglobal MAIN ?*MaxSampling* = 10) ;100
(defglobal MAIN ?*SudAccelLimit* = 1.75)     ;急加速の閾値 1.75
(defglobal MAIN
	?*AccelPeakHistList* = (create$))

(defglobal MAIN ?*AccSamplingCnt* = 0) 
(defglobal MAIN ?*AccSamplingCntB* = 0) ;IGOFF->save2file IGON->read previous from file
(defglobal MAIN ?*AccSudCnt* = 0)			;急加速しがちCount
(defglobal MAIN ?*PreAccel* = 0)			;前回までの最大値
(defglobal MAIN ?*TauCnt* = 0)
(defglobal MAIN ?*AccelLimit* = 0.1)     ;加速シーンの閾値 0.75
;==================================================
; Fuction(AccelPeakHist.clp)
; 
;==================================================	
 (deffunction MAIN::count-facts (?template)
 (length (find-all-facts ((?fct ?template)) TRUE)))


(deffunction MAIN::assertAccelPeakHist ()
	(assert (AccelPeakHist (acceleration 0.7)))
	(assert (AccelPeakHist (acceleration 0.8)))
	(assert (AccelPeakHist (acceleration 0.9)))
	(assert (AccelPeakHist (acceleration 1.0)))
	(assert (AccelPeakHist (acceleration 1.1)))
	(assert (AccelPeakHist (acceleration 1.2)))
	(assert (AccelPeakHist (acceleration 1.3)))
	(assert (AccelPeakHist (acceleration 1.4)))
	(assert (AccelPeakHist (acceleration 1.5)))
	(assert (AccelPeakHist (acceleration 1.6)))
	(assert (AccelPeakHist (acceleration 1.7)))
	(assert (AccelPeakHist (acceleration 1.8)))
	(assert (AccelPeakHist (acceleration 1.9)))
	(assert (AccelPeakHist (acceleration 2.0)))
	(assert (AccelPeakHist (acceleration 2.1)))
	(assert (AccelPeakHist (acceleration 2.2)))
	(assert (AccelPeakHist (acceleration 2.3)))
)

(deffunction MAIN::MakePreHistgram ()

	(if (not (any-factp ((?a AccelPeakHist)) TRUE)) then (assertAccelPeakHist))
		
	(do-for-all-facts ((?a AccelPeakRawDataWithFlag)) (> ?a:preFlag 0)
		(bind ?accel (fact-slot-value ?a acceleration))
		(if (< ?accel 0.8) then
			(bind ?f 0.7)
		 else (if (> ?accel 2.2) then (bind ?f 2.3)
		 else (bind ?f ?accel))
		)
		
		(foreach ?p (find-all-facts ((?x AccelPeakHist)) TRUE)
			
			(if (= (round (* (fact-slot-value ?p acceleration) 10)) (round (* ?f 10))) then
				(modify ?p (count (+ (fact-slot-value ?p count) 1)))
			)
		)
	)
	
	;Add to FIFO
	;Clear AccelPeakHist Facts
	(foreach ?p (find-all-facts ((?x AccelPeakHist)) TRUE)
		
		(if (> (fact-slot-value ?p count) 0)then
		
			(bind ?AccelInfoList (fact-slot-value ?p acceleration))
			(bind ?CountInfoList (fact-slot-value ?p count))
			(bind ?*AccelPeakHistList* (insert$ ?*AccelPeakHistList* 1 ?AccelInfoList ?CountInfoList))
			;(printout t "++++++Accel Peak info is:" ?*AccelPeakHistList* crlf)
		)
		(retract ?p)
	)
	(bind ?id 1)
   　(send [FIFO] putData ?*AccelPeakHistList* ?id)
   　(printout t "+++++MakePreHistgram:"  ?*AccelPeakHistList* crlf)
   　(bind ?*AccelPeakHistList* (create$))
)
	
(deffunction MAIN::MakeCurHistgram ()

	(if (not (any-factp ((?a AccelPeakHist)) TRUE)) then (assertAccelPeakHist))
		
	(do-for-all-facts ((?a AccelPeakRawDataWithFlag)) (= ?a:preFlag 0)
		(bind ?accel (fact-slot-value ?a acceleration))
		(if (< ?accel 0.8) then
			(bind ?f 0.7)
		 else (if (> ?accel 2.2) then (bind ?f 2.3)
		 else (bind ?f ?accel))
		)
		
		(foreach ?p (find-all-facts ((?x AccelPeakHist)) TRUE)
			
			(if (= (round (* (fact-slot-value ?p acceleration) 10)) (round (* ?f 10))) then
				(modify ?p (count (+ (fact-slot-value ?p count) 1)))
			)
		)
	)
	
	;Add to FIFO
	;Clear AccelPeakHist Facts
	(foreach ?p (find-all-facts ((?x AccelPeakHist)) TRUE)
		
		(if (> (fact-slot-value ?p count) 0)then
		
			(bind ?AccelInfoList (fact-slot-value ?p acceleration))
			(bind ?CountInfoList (fact-slot-value ?p count))
			(bind ?*AccelPeakHistList* (insert$ ?*AccelPeakHistList* 1 ?AccelInfoList ?CountInfoList))
			;(printout t "++++++Accel Peak info is:" ?*AccelPeakHistList* crlf)
		)
		(modify ?p (count  0))
		
	)
	(bind ?id 1)
   　(send [FIFO] putData ?*AccelPeakHistList* ?id)
   　(printout t "+++++MakeCurHistgram:　"  ?*AccelPeakHistList* crlf)
   　(bind ?*AccelPeakHistList* (create$))

)

(deffunction MAIN::averageAccel ()

	(bind ?Accel 0)
	(bind ?width (count-facts AccelPeakRawDataWithFlag))
	(printout t "+++++MAIN::averageAccel width = " ?width crlf)
	(if (not (> ?width 0)) then
		(return 0))
	
	(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) TRUE)
		(bind ?a (fact-slot-value ?f acceleration))
		(bind ?Accel (+ ?Accel ?a))
	)
	(printout t "+++++MAIN::averageAccel Accel = " ?Accel crlf)
	(return (/ ?Accel ?width))
)



(deffunction MAIN::accelSigma ()

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

(deffunction MAIN::cntAccSudPercent ()

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

	
(deffunction MAIN::MakePreStatus ()

	;これまでのサンプル数
	(bind ?preSampleCnt (length (find-all-facts ((?x AccelPeakRawDataWithFlag)) (> ?x:preFlag 0))))

	;これまでの平均
	(bind ?preAccAvr 0)
	(if (> ?preSampleCnt 0) then
		
		(bind ?Accel 0)
		(foreach ?f (find-all-facts ((?x AccelPeakRawDataWithFlag)) (> ?x:preFlag 0))
			(bind ?a (fact-slot-value ?f acceleration))
			(bind ?Accel (+ ?Accel ?a))
		)
		(bind ?preAccAvr (/ ?Accel ?preSampleCnt))
	else
		(return -1)
	)
	
	;必要サンプリング数に達して以降、サンプリング値が更新されるたびに、以下の判定をする。
	;加速情報[ ] > 1.7m/s^2 が何％成立しているか判定し、
	(bind ?Judge "状態未判定")
	(if (> ?preSampleCnt ?*MaxSampling*) then 

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
	
			(bind ?p1 (assert (Profile1Status   
									(previousSampling ?preSampleCnt) 
									(previousAveSpeed ?preAccAvr)
									(AccelJudgement ?Judge)
									(Sigma ?sigma)
									)))

	(bind ?preStatus (create$))
	(foreach ?n (fact-slot-names ?p1)
		(bind ?preStatus (insert$ ?preStatus 1 (fact-slot-value ?p1 ?n)))
	)
	
	
	
	(bind ?id 2)
   　(send [FIFO] putData ?preStatus ?id)
   　(printout t "++++++Insert Previous Profile1 data to FIFO:"  ?preStatus crlf)
	
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


(defrule SpecificAgenda::getMaxValueofAccelScene "加速度0.85越えによる加速走行推移"
	(declare (salience 900))
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration 0.85)))
	(EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ACCEL))
	=>
	;Get the max accelration of the secen
	(printout t "++++++++getMaxValueofAccelScene Current： " ?acceleration crlf)
	(if (>= ?acceleration ?*PreAccel*) then
		(bind ?*PreAccel* ?acceleration)
		(printout t "++++++++getMaxValueofAccelScene Max： " ?*PreAccel* crlf)
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
		(bind ?Judge "状態未判定")
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
		(if (> ?sigma 0) then
			(bind ?tau (/ (- ?acceleration ?aver) ?sigma))
		else
			(bind ?tau 0)
		)
		;|τ|>3　
		;(if (> (abs ?tau) 3) then (bind ?*TauCnt* (+ ?*TauCnt* 1)))

		;(bind ?a (fact-slot-value ?f acceleration))
		(if (> ?a (+ ?aver (* 3 ?sigma))) then (bind ?*TauCnt* (+ ?*TauCnt* 1)) )
		;(printout t "a=" ?a ", acceleration=" ?acceleration ", aver=" ?aver ", sigma=" ?sigma ", 3sigma=" (+ ?aver (* 3 ?sigma)) ", TauCnt=" ?*TauCnt* crlf)
		
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
		(bind ?*AccelPeakHistList* (create$))
		
		(bind ?*PreAccel* 0)
	)
)



;==================================================
; Extenal Function
;==================================================
(deffunction MAIN::SaveFactList()
	(bind ?l (count-facts AccelPeakRawDataWithFlag))
	(printout t "+++++SaveFactList start*** " ?l  crlf)
	(bind ?ret FALSE)
	(bind ?cnt 0)

	(do-for-all-facts ((?factlist AccelPeakRawDataWithFlag)) TRUE
	  (bind ?value (fact-slot-value ?factlist acceleration))
	  (assert (AccelPeakRawData(acceleration ?value)))
	)
	
	(while (and (not ?ret) (< ?cnt 3)) do
		(bind ?cnt (+ ?cnt 1))
		;(system "ren FactListInfo.txt FactListInfo_%date:~0,4%_%date:~5,2%_%date:~8,2%_%time:~0,2%%time:~3,2%%time:~6,2%%time:~9,3%.txt >null")
		(bind ?ret (save-facts "FactListInfo.txt" local AccelPeakRawData))
		(printout t "+++++SaveFactList result is:***" ?ret crlf)
	)
	
	(if ?ret then
		(do-for-all-facts ((?factlist AccelPeakRawData)) TRUE
			(retract ?factlist))
		(do-for-all-facts ((?factlist AccelPeakRawDataWithFlag)) TRUE
			(retract ?factlist))
	)
)

(deffunction MAIN::LoadFactList()
	(set-fact-duplication TRUE)
	(printout t "+++++LoadFactList start***"  crlf)
	
	(if (> (length (find-all-facts ((?x AccelPeakRawDataWithFlag)) TRUE )) 0) then 
		(printout t "+++++LoadFactList Nothing loaded***"  crlf)
		(return 0)
	)
	
	(bind ?exist (load-facts "FactListInfo.txt"))
	(if ?exist then
		(if (> (length (find-all-facts ((?x AccelPeakRawData)) TRUE)) ?*MaxSampling*) then
			(bind ?*ready* TRUE)
		)
	)
	
	(bind ?count (length (find-all-facts ((?x AccelPeakRawData)) TRUE)))
	(printout t "+++++LoadFactList result " ?count " is loaded *** " ?exist crlf)

	(if(> ?count ?*MaxWindow* ) then
	    (bind ?iterator (- ?count ?*MaxWindow* ))
	else
		(bind ?iterator 0))
		
	(do-for-all-facts ((?factlist AccelPeakRawData)) TRUE
		(if(> ?iterator 0) then
			(bind ?iterator (- ?iterator 1))
		else
			(bind ?value (fact-slot-value ?factlist acceleration))
			(assert (AccelPeakRawDataWithFlag(acceleration ?value) (preFlag 1))))
		(retract ?factlist)
    )
)


;==================================================
; Rule(AccelPeakHist.clp)
;==================================================
(defrule MAIN::CreateInstance "Create instance of class container."
   (declare (salience 920))
   =>
   (make-instance FIFO of container))
   
   
(defrule MAIN::GetPreAccelPeakHist "Get all info of accel Peak from fact list."
   (declare (salience 900))
   =>
   (MakePreHistgram)
   (MakePreStatus)
)

(defrule IOAgenda::accelFileOpration
	(declare (salience 990))
	?f <- (accelFileOprationEvent (status ?s))
	=>
	(printout t "+++++IOAgenda::accelFileOpration : status = " ?s crlf)
	(if (= ?s 1) then
		(LoadFactList)
	else
		(SaveFactList)
	)
	
	(retract ?f)
	
)

