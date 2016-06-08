;==================================================
; For debug@CLIPS IDE(NewSpeedAccelerationCheck.clp)
;==================================================
(defmodule MAIN (export ?ALL))
(defmodule SpecificAgenda (import MAIN ?ALL))

(deftemplate MAIN::EventSpeed
	(slot from (default entryPoint))
	(slot name)
	
	(slot speed ; 
		(type FLOAT)
		(default 0.0))
	(slot type
		(type SYMBOL)
		(allowed-values VEHICLE_SPEED_SP1)
		(default VEHICLE_SPEED_SP1))
	(slot time
		(type FLOAT)
		(default 0.0))
		;(default-dynamic (time)))
		)

(deftemplate MAIN::EventSpeedList
	(slot from (default entryPoint))
	(slot name (default "Receiving Data Stream"))
	
	; time type speed
	(multislot speedList))

(defrule MAIN::Init
	(declare (salience 999) (auto-focus TRUE))
	
	=>
	(bind ?speed1 1.1)
	(bind ?time1 0.3)
	(bind ?speed2 2.1)
	(bind ?time2 2.0)

	(bind ?type VEHICLE_SPEED_SP1)
	
	(assert (EventSpeed (name "Current Receiving Data Stream") (speed ?speed1) (time ?time1) (type ?type)))
	(assert (EventSpeed (name "latestHistory Stream") (speed ?speed2) (time ?time2) (type ?type)))
	(assert (EventSpeed (name "latestHistory Stream") (speed (+ ?speed2 2)) (time (+ ?time2 1)) (type ?type)))
	
	;1:x 2:x 3:speed 4:x 5:time
	(assert (EventSpeedList (speedList 1 2 3.2 4 0.5 )))	
	(printout t "Here is main::init" crlf)
)

(deffunction SpecificAgenda::getAccelerationValue(?after ?before ?delta)
	(return (/ (/ (* (- ?after ?before) 1000) 3600) ?delta)))
