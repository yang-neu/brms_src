;==================================================
; Class(AccelPeakHist.clp)
; Add by liusiping@2016/06/10
;==================================================
(defglobal MAIN ?*AccelPeakWindow* = (create$))  ;****[] width is the MaxWindow ;Read previouse data from data at IGON,Save current data at IGOFF-

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

(defmessage-handler container putData (?inputData) 
    (printout t "***setData start***"  crlf)
	(if (= ?self:count 10) then
		(printout t "***FIFO is full!***" crlf)
		(bind ?jarname (nth$ ?self:end ?self:jarlist))
		(bind ?self:start (+ ?self:start 1))
		(if(> ?self:start 10) then (bind ?self:start 1))
		(bind ?self:end (+ ?self:end 1))
		(if(> ?self:end 10) then (bind ?self:end 1))
		(dynamic-put ?jarname ?inputData)
	else
		(bind ?jarname (nth$ ?self:end ?self:jarlist))
		(bind ?self:end (+ ?self:end 1))
		(if(> ?self:end 10) then (bind ?self:end 1))
		(bind ?self:count (+ ?self:count 1))
		(dynamic-put ?jarname ?inputData)
	)	
	(printout t "***setData start***"  crlf)
)
  
;==================================================
; Template(AccelPeakHist.clp)
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
(deffunction assertAccelPeakHist ()
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

(deffunction MakePreHistgram ()

	(if (not (any-factp ((?a AccelPeakHist)) TRUE)) then (assertAccelPeakHist))
		
	(do-for-all-facts ((?a AccelPeakRawDataWithFlag) (> ?a:preFlag 0))
		
		(if (< ?a:acceleration 0.8) then
			(bind ?f 0.7)
		 else (if (> ?a:acceleration 2.2) then (bind ?f 2.3))
		 else (bind ?f ?a:acceleration)
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
		
		(if (> (fact-slot-value ?p acceleration) then
		)
)
	
(deffunction MakeCurHistgram ()

	(if (not (any-factp ((?a AccelPeakHist)) TRUE)) then (assertAccelPeakHist))
		
	(do-for-all-facts ((?a AccelPeakRawDataWithFlag) (= ?a:preFlag 0))
		
		(if (< ?a:acceleration 0.8) then
			(bind ?f 0.7)
		 else (if (> ?a:acceleration 2.2) then (bind ?f 2.3))
		 else (bind ?f ?a:acceleration)
		)
		
		(foreach ?p (find-all-facts ((?x AccelPeakHist)) TRUE)
			
			(if (= (round (* (fact-slot-value ?p acceleration) 10)) (round (* ?f 10))) then
				(modify ?p (count (+ (fact-slot-value ?p count) 1)))
			)
		)
	)
	
	;Add to FIFO
	;Clear AccelPeakHist Facts
)

	
;==================================================
; Rule(AccelPeakHist.clp)
; Add by liusiping@2016/06/08
;==================================================
;(defrule MAIN::SetFactList "Save fact list info to file."
;   (declare (salience 902))
;   =>
;   (save-facts "FactListInfo.txt" local AccelPeakHist))
   
   
(defrule MAIN::GetFactList "Get all fact from config file."
   (declare (salience 980))
   =>
   (load-facts "FactListInfo.txt"))
   
   
(defrule MAIN::CreateInstance "Create instance of class container."
   (declare (salience 920))
   =>
   (make-instance FIFO of container))
   
   
(defrule MAIN::GetAccelPeakHist "Get all info of accel Peak from fact list."
   (declare (salience 900))
   ?AccelPeakHistAddrList <- (AccelPeakHist)
   =>
   (bind ?AccelInfoList (fact-slot-value ?AccelPeakHistAddrList acceleration))
   (bind ?CountInfoList (fact-slot-value ?AccelPeakHistAddrList count))
   (bind ?*AccelPeakHistList* (insert$ ?*AccelPeakHistList* 1 ?AccelInfoList ?CountInfoList))
   (printout t "Accel Peak info is:"  ?*AccelPeakHistList* crlf))
   
   
(defrule MAIN::writeToFIFO "Save global var to FIFO"
   (declare (salience 890))
   =>
   (send [FIFO] putData ?*AccelPeakHistList*)
   (bind ?*AccelPeakHistList* (create$))
   (printout t "Accel Peak info is:"  ?*AccelPeakHistList* crlf))
   
   
(defrule MAIN::RestoreAccelPeakWindow "Restore the raw Accelation Peak data from file to memory"
   (declare (salience 950))
   ?AccelPeak <- (AccelPeakRawData)
   =>
   (bind ?data (fact-slot-value ?AccelPeak acceleration))
   (bind ?len (length$ ?*AccelPeakWindow*))
   (bind ?*AccelPeakWindow* (insert$ ?*AccelPeakWindow* (+ ?len 1) ?data))
   (printout t ?*AccelPeakWindow* crlf)
)

 (defrule RestoreAccelPeakWindowM
  (declare (salience 950))
   ?AccelPeak <- (AccelPeakRawData)
   =>
   ; (bind ?data (fact-slot-value ?AccelPeak acceleration))
   ; (bind ?len (length$ ?*AccelPeakWindow*))
   ; (bind ?*AccelPeakWindow* (insert$ ?*AccelPeakWindow* (+ ?len 1) ?data))
   ; (printout t ?*AccelPeakWindow* crlf)
   
   (printout t (fact-index ?AccelPeak) crlf))
)
