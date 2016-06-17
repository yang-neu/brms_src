(defglobal MAIN ?*MaxWindow* = 500) ;500
(defglobal MAIN ?*MaxSampling* = 10) ;100
(defglobal MAIN ?*ready* = FALSE)
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
		

(defglobal MAIN
	?*AccelPeakHistList* = (create$))


;==================================================
; Fuction(AccelPeakHist.clp)
; 
;==================================================	
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
		 else (if (> ?accel 2.2) then (bind ?f 2.3))
		 else (bind ?f ?accel)
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
   　(bind ?*AccelPeakHistList* (create$))
   　(printout t "Accel Peak info is:"  ?*AccelPeakHistList* crlf)
)
	
(deffunction MAIN::MakeCurHistgram ()

	(if (not (any-factp ((?a AccelPeakHist)) TRUE)) then (assertAccelPeakHist))
		
	(do-for-all-facts ((?a AccelPeakRawDataWithFlag)) (= ?a:preFlag 0)
		(bind ?accel (fact-slot-value ?a acceleration))
		(if (< ?accel 0.8) then
			(bind ?f 0.7)
		 else (if (> ?accel 2.2) then (bind ?f 2.3))
		 else (bind ?f ?accel)
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
		
	)
	(bind ?id 1)
   　(send [FIFO] putData ?*AccelPeakHistList* ?id)
   　(bind ?*AccelPeakHistList* (create$))
   　(printout t "Accel Peak info is:"  ?*AccelPeakHistList* crlf)

)

	
;==================================================
; Rule(AccelPeakHist.clp)
; Add by liusiping@2016/06/08
;==================================================
;(defrule MAIN::SetFactList "Save fact list info to file."
;   (declare (salience 902))
;   =>
;   (save-facts "FactListInfo.txt" local AccelPeakHist))
   
   
;(defrule MAIN::GetFactList "Get all fact from config file."
;   (declare (salience 910))
;   =>
;   (load-facts "FactListInfo.txt"))

   
   
(defrule MAIN::CreateInstance "Create instance of class container."
   (declare (salience 920))
   =>
   (make-instance FIFO of container))
   
   
(defrule MAIN::GetPreAccelPeakHist "Get all info of accel Peak from fact list."
   (declare (salience 900))
   =>
   (MakePreHistgram)
)
   
   
; (defrule MAIN::writeToFIFO "Save global var to FIFO"
   ; (declare (salience 890))
   ; (test (<> (length$ ?*AccelPeakHistList*) 0))
   ; =>
   ; (bind ?id 1)
   ; (send [FIFO] putData ?*AccelPeakHistList* ?id)
   ; (bind ?*AccelPeakHistList* (create$))
   ; (printout t "Accel Peak info is:"  ?*AccelPeakHistList* crlf))
   
  
;==================================================
; Function
;==================================================
(deffunction MAIN::SaveFactList()
	(printout t "***SaveFactList start***"  crlf)
	(do-for-all-facts ((?factlist AccelPeakRawDataWithFlag)) TRUE
	  (bind ?value (fact-slot-value ?factlist acceleration))
	  (assert (AccelPeakRawData(acceleration ?value)))
	  (retract ?factlist))
	(bind ?tmp (save-facts "FactListInfo.txt" local AccelPeakRawData))
	(printout t "***SaveFactList result is:***" ?tmp crlf)
	(do-for-all-facts ((?factlist AccelPeakRawData)) TRUE
	  (retract ?factlist))
)

(deffunction MAIN::LoadFactList()
	(set-fact-duplication TRUE)
	(printout t "***LoadFactList start***"  crlf)
	(bind ?exist (load-facts "FactListInfo.txt"))
	(if ?exist then
		(if (> (length (find-all-facts ((?x AccelPeakRawData)) TRUE)) ?*MaxSampling*) then
			(bind ?*ready* TRUE)
		)
	)
	(printout t "***LoadFactList result is:***" ?exist crlf)
	
	(bind ?count (length (find-all-facts ((?x AccelPeakRawData)) TRUE)))
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

