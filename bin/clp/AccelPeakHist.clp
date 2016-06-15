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
		(default 0)))
		
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
   (test (<> (length$ ?*AccelPeakHistList*) 0))
   =>
   (bind ?id 1)
   (send [FIFO] putData ?*AccelPeakHistList* ?id)
   (bind ?*AccelPeakHistList* (create$))
   (printout t "Accel Peak info is:"  ?*AccelPeakHistList* crlf))
   
  
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
	(printout t "***LoadFactList start***"  crlf)
	(bind ?tmp (load-facts "FactListInfo.txt"))
	(do-for-all-facts ((?factlist AccelPeakRawData)) TRUE
	  (bind ?value (fact-slot-value ?factlist acceleration))
	  (assert (AccelPeakRawDataWithFlag(acceleration ?value)))
	  (retract ?factlist))
)