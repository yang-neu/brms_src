;==================================================
; Class(AccelSpeedInfo.clp)
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
	else
		(bind ?jarname (nth$ ?self:end ?self:jarlist))
		(bind ?self:end (+ ?self:end 1))
		(if(> ?self:end 10) then (bind ?self:end 1))
		(bind ?self:count (+ ?self:count 1))
		(dynamic-put ?jarname ?inputData)
	)	
	(printout t "***setData start***"  crlf)
)










