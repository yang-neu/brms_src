;==================================================
; Class(AccelPeakInfo.clp)
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
; Template&global(AccelPeakInfo.clp)
; Add by liusiping@2016/06/08
;==================================================
(deftemplate MAIN::AccelPeakInfo
	(slot acceleration
		(type FLOAT)
		(default 0.0))
	(slot count
		(type INTEGER)
		(default 0)))
		
(deftemplate MAIN::AccelRawData
	(slot acceleration
		(type FLOAT)
		(default 0.0)))
		

(defglobal MAIN
	?*AccelPeakInfoList* = (create$))

;==================================================
; Rule(AccelPeakInfo.clp)
; Add by liusiping@2016/06/08
;==================================================
;(defrule MAIN::SetFactList "Save fact list info to file."
;   (declare (salience 902))
;   =>
;   (save-facts "FactListInfo.txt" local AccelPeakInfo))
   
   
;(defrule MAIN::GetFactList "Get all fact from config file."
;   (declare (salience 910))
;   =>
;   (load-facts "FactListInfo.txt"))
   
   
(defrule MAIN::CreateInstance "Create instance of class container."
   (declare (salience 920))
   =>
   (make-instance FIFO of container))
   
   
(defrule MAIN::GetAccelPeakInfo "Get all info of accel Peak from fact list."
   (declare (salience 900))
   ?AccelPeakInfoAddrList <- (AccelPeakInfo)
   =>
   (bind ?AccelInfoList (fact-slot-value ?AccelPeakInfoAddrList acceleration))
   (bind ?CountInfoList (fact-slot-value ?AccelPeakInfoAddrList count))
   (bind ?*AccelPeakInfoList* (insert$ ?*AccelPeakInfoList* 1 ?AccelInfoList ?CountInfoList))
   (printout t "Accel Peak info is:"  ?*AccelPeakInfoList* crlf))
   
   
(defrule MAIN::writeToFIFO "Save global var to FIFO"
   (declare (salience 890))
   (test (<> (length$ ?*AccelPeakInfoList*) 0))
   =>
   (send [FIFO] putData ?*AccelPeakInfoList*)
   (bind ?*AccelPeakInfoList* (create$))
   (printout t "Accel Peak info is:"  ?*AccelPeakInfoList* crlf))
   
  
;==================================================
; Function
;==================================================
(deffunction MAIN::SaveFactList()
	(printout t "***SaveFactList start***"  crlf)
	(bind ?tmp (save-facts "FactListInfo.txt" local AccelRawData))
	(printout t "***SaveFactList result is:***" ?tmp crlf)
	(do-for-all-facts ((?factlist AccelRawData)) TRUE
	  (retract ?factlist))
)

(deffunction MAIN::LoadFactList()
	(printout t "***LoadFactList start***"  crlf)
	(bind ?tmp (load-facts "FactListInfo.txt"))
	(printout t "***LoadFactList result is:***" ?tmp crlf))