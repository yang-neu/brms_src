;==================================================
; Rule(AccelSpeedInfo.clp)
; Add by liusiping@2016/06/08
;==================================================
(defrule SetFactList "Save fact list info to file."
   (declare (salience 902))
   =>
   (save-facts "FactListInfo.txt" local AccelSpeedInfo))
   
   
(defrule GetFactList "Get all fact from config file."
   (declare (salience 901))
   =>
   (load-facts "FactListInfo.txt")
   (bind ?*AccelSpeedInfoList* (create$)))
   
   
(defrule GetAccelSpeedInfo "Get all info of accel speed from fact list."
   (declare (salience 900))
   ?AccelSpeedInfoAddrList <- (AccelSpeedInfo)
   =>
   (bind ?AccelInfoList (fact-slot-value ?AccelSpeedInfoAddrList acceleration))
   (bind ?CountInfoList (fact-slot-value ?AccelSpeedInfoAddrList count))
   (bind ?*AccelSpeedInfoList* (insert$ ?*AccelSpeedInfoList* 1 ?AccelInfoList ?CountInfoList))
   (printout t "Accel speed info is:"  ?*AccelSpeedInfoList* crlf))
   

  
;==================================================
; Template(AccelSpeedInfo.clp)
; Add by liusiping@2016/06/08
;==================================================
(deftemplate MAIN::AccelSpeedInfo
	(slot acceleration
		(type FLOAT)
		(default 0.0))
	(slot count
		(type INTEGER)
		(default 0)))

		
(defglobal MAIN
	?*AccelSpeedInfoList* = 0)
