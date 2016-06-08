;==================================================
; Global value(NewSpeedAccelerationCheck.clp)
;==================================================
(deftemplate MAIN::AccelList
	(slot from (default entryPoint))
	(slot name (default "Profile 1"))
	
	;?,?,Accel
	(multislot ValuList))

(defglobal SpecificAgenda ?*MaxSampling* = 500)
(defglobal SpecificAgenda ?*cnt* = 1)
;==================================================
; Function(NewSpeedAccelerationCheck.clp)
;==================================================
(deffunction count-facts-2 (?template)
(length (find-all-facts ((?fct ?template)) TRUE)))

;==================================================
; Rule(NewSpeedAccelerationCheck.clp)
;==================================================

(defrule SpecificAgenda::MakeAccelerationList
	(declare (salience 980))
	?afterS <- (EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (type VEHICLE_SPEED_SP1)); (time ?time2))
	?latest <- (EventSpeed (from entryPoint) (name "latestHistory Stream") (type VEHICLE_SPEED_SP1))
	?dataList <- (EventSpeedList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(bind ?beforeS (fact-slot-value ?dataList speedList))

	(if (and (multifieldp ?beforeS) (>= (length$ ?beforeS) 5)) then
		(bind ?speed1 (fact-slot-value ?afterS speed))
		(bind ?time1 (fact-slot-value ?afterS time))
	    (printout t "multifieldp ?beforeS" crlf)
		(printout t "len = " (length$ ?beforeS) crlf)
		(bind ?speed2 (nth$ 3 ?beforeS))
		(bind ?time2 (nth$ 5 ?beforeS))
		(bind ?acceleration (getAccelerationValue ?speed1 ?speed2 (- ?time1 ?time2)))
		(format t "==== speed : %4.1f, accel : %3.2f%n"  ?speed1 ?acceleration)
	)
	(assert (AccelList (ValuList ?acceleration)))
	
	(printout t "here is SpecificAgenda::MakeAccelerationList" crlf)	
	(printout t "speed = " ?speed1 " time = " ?time1 crlf)	

)


(defrule SpecificAgenda::CntList
	(declare (salience 980))
	?list <- (EventSpeed )
	=>
	(bind ?cnt (count-facts-2 EventSpeed))
	(printout t "here is SpecificAgenda::CntListList" crlf)	
	(printout t "cnt = " ?cnt  crlf)	
	(bind ?*cnt* (+ ?*cnt* 1))
)


