;==================================================
; Function(HandleSpeedCheck.clp)
;==================================================
;急加速度頻度計算関数
(deffunction SpecificAgenda::calcSteeringAngleAccelPercentage(?data)
	(bind ?suddenAccel 0)
	(bind ?normalAccel 0)
	
	;>変更:QueueからIteratorに変更
	(bind ?dataSize 0)
	(bind ?max_sample_size (fact-slot-value ?data MAX_SAMPLE_SIZE))
	(bind ?itrAccelSpecific (fact-slot-value ?data specificList))
	
	;最新100サンプルを集計
	;>変更:両端キューを使うことで最新100サンプルの下限を計算する余分な処理と余分なインスタンスコピーを減らした。
	(while (and (< ?dataSize (length$ ?itrAccelSpecific)) (< ?dataSize ?max_sample_size)) do
		(if (eq (nth$ (+ ?dataSize 1) ?itrAccelSpecific) SUDDEN) then
			(bind ?suddenAccel (+ ?suddenAccel 1))
		else
			(bind ?normalAccel (+ ?normalAccel 1)))
		(bind ?dataSize (+ ?dataSize 1)))
	
	(bind ?percentage1 (fact-slot-value ?data percentage))
	(if (= ?suddenAccel 0) then
		(bind ?percentage2 0.0)
	else (if (= ?normalAccel 0) then
		(bind ?percentage2 100.0)
	else
		(bind ?percentage2 (/ (* ?suddenAccel 100) (+ ?suddenAccel ?normalAccel)))))
	
	(if (<> ?percentage1 ?percentage2) then
		(bind ?data (modify ?data (percentage ?percentage2))))
	
	(return ?data))

;組合せ計算関数
(deffunction SpecificAgenda::calrSteeringAngleAccelCombination(?n ?x)
	(bind ?combi 1)
	(if (and (<= 1 ?x) (< ?x ?n)) then
		(bind ?d ?x)
		(while (>= ?d 1) do
			(bind ?combi (* ?combi (/ (- (+ ?n 1) ?d) ?d)))
			(bind ?d (- ?d 1))))
	(return ?combi))

;二項検定によるドライバ状態判定関数
(deffunction SpecificAgenda::judUserSteeringAngleAccelState(?data)
	(bind ?state_check_size (fact-slot-value ?data STATE_CHECK_SIZE))
	(bind ?itrAccelSpecific (fact-slot-value ?data specificList))
	(bind ?sampleingCount (length$ ?itrAccelSpecific))
	
	(if (< ?sampleingCount (fact-slot-value ?data DEQUE_SIZE)) then
		(return ?data))
	
	(bind ?cumProb 0.0)
	(bind ?p 0.0)
	(bind ?x 0)
	
	;>変更 :QueueからIteratorに変更
	(bind ?dataSize 0)
	
	;急加速しがちなタイプの場合
	(bind ?oldSampleSpecific (fact-slot-value ?data oldSampleSpecific))
	(bind ?oldSamplePercentage (fact-slot-value ?data oldSamplePercentage))
	(if (eq ?oldSampleSpecific SUDDEN) then
		(bind ?p (/ ?oldSamplePercentage 100))
		;>変更:両端キューを使うことで最新100サンプルの下限を計算する余分な処理と余分なインスタンスコピーを減らした。
		(while (and (< ?dataSize ?sampleingCount) (< ?dataSize ?state_check_size)) do
			(if (eq (nth$ (+ ?dataSize 1) ?itrAccelSpecific) SUDDEN) then
				(bind ?x (+ ?x 1)))
			(bind ?dataSize (+ ?dataSize 1)))
	
	;急加速あまりしないタイプの場合
	else
	  (bind ?p (- 1 (/ ?oldSamplePercentage 100)))
		;>変更:両端キューを使うことで最新100サンプルの下限を計算する余分な処理と余分なインスタンスコピーを減らした。
		(while (and (< ?dataSize ?sampleingCount) (< ?dataSize ?state_check_size)) do
			(if (eq (nth$ (+ ?dataSize 1) ?itrAccelSpecific) NORMAL) then
				(bind ?x (+ ?x 1)))
			(bind ?dataSize (+ ?dataSize 1))))
	
	;帰無仮説による状態判定
	(loop-for-count (?i 0 ?x) do
		(bind ?cumProb (+ ?cumProb (* (calrSteeringAngleAccelCombination ?state_check_size ?i) (brmsPower ?p ?i) (brmsPower (- 1 ?p) (- ?state_check_size ?i)))))
		(if (>= ?cumProb 0.05) then
			(break)))
	
	;いつもと違う状態
	(bind ?todaySpecificState (fact-slot-value ?data todaySpecificState))
	(if (<> ?todaySpecificState ?cumProb) then
		(bind ?data (modify ?data (todaySpecificState ?cumProb))))
	
	(return ?data))

;==================================================
; Rule(HandleSpeedCheck.clp)
;==================================================
(defrule SpecificAgenda::急ハンドル情報の舵角加速度検出
	(declare (salience 600))
    
	(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture ?posture&TURN_LEFT|TURN_RIGHT))
	(EventSteeringAngleAcceleration (from entryPoint) (name "Calculation Stream") (steeringAngleAcceleration ?steeringAngleAcceleration&:(>= (abs ?steeringAngleAcceleration) 50.0)))
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION) (p23_suddenhandleinfo ?suddenhandleinfo&:(= ?suddenhandleinfo 0.0)))
	=>
	(format t "SpecificAgenda::急ハンドル情報の舵角加速度検出 %3.2f%n" ?steeringAngleAcceleration)
	(modify ?acce (p23_suddenhandleinfo ?steeringAngleAcceleration)))

(defrule SpecificAgenda::計算:ハンドル操作スピード/ドライバ状態判定
	(declare (salience 300))

	(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture TURN_STRAIGHT) (beforeVehiclePostureScene ?posture&TURN_LEFT|TURN_RIGHT))
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION) (p23_suddenhandleinfo ?suddenhandleinfo) (specificList $?specificList))
	(test (eq ?*no-loop-ハンドル操作スピード-ドライバ状態判定* TRUE))
	=>
    (bind ?*no-loop-ハンドル操作スピード-ドライバ状態判定* FALSE)
	(if (>= (abs ?suddenhandleinfo) 50.0) then
		(bind ?acce (modify ?acce (p23_suddenhandleinfo 0.0) (specificList SUDDEN ?specificList)))
	else
		(bind ?acce (modify ?acce (p23_suddenhandleinfo 0.0) (specificList NORMAL ?specificList))))
	(bind ?acce (calcSteeringAngleAccelPercentage ?acce))
	(bind ?acce (judUserSteeringAngleAccelState ?acce))
	(duplicate ?acce (name "Calculation Stream")))

(defrule SpecificAgenda::集計：急ハンドル情報：サンプル未達
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION) (specificList $?specificList1) (percentage ?percentage))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type STEERINGANGLE_ACCELERATION) (specificList $?specificList&:(< (length$ ?specificList) 100)))
	=>
	(printout t "急ハンドル判定：サンプル数未達 : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "プロファイル23 急ハンドル特性判定：サンプル数未達 : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "急ハンドル情報 : " ?percentage " %" crlf)
	;(bind ?*list* (modify ?*list* (suddenStAngleAccelCount (length$ ?specificList1))))
	(printout eventoutput (str-cat "suddenStAngleAccelCount" " " (length$ ?specificList1)))
	)

(defrule SpecificAgenda::集計：急ハンドル情報：急ハンドル多い
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION) (specific ?specific) (specificList $?specificList1) (percentage ?percentage1))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type STEERINGANGLE_ACCELERATION) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (percentage ?percentage&:(>= ?percentage 10)))
	=>
	(printout t "急ハンドル判定：急ハンドル多い" crlf)
	(printout qt "プロファイル23 急ハンドル特性判定：急ハンドル多い : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "急ハンドル情報 : " ?percentage1 " %" crlf)
	;(bind ?*list* (modify ?*list* (suddenStAngleAccel "急ハンドル多い")))
	(printout eventoutput "suddenStAngleAccel 急ハンドル多い")
	;(bind ?*list* (modify ?*list* (suddenStAngleAccelCount (length$ ?specificList1))))
	(printout eventoutput (str-cat "suddenStAngleAccelCount" " " (length$ ?specificList1)))
	(if (neq ?specific SUDDEN) then
		(modify ?acce (specific SUDDEN))))

(defrule SpecificAgenda::集計：急ハンドル情報：急ハンドル少ない
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION) (specific ?specific) (specificList $?specificList1) (percentage ?percentage1))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type STEERINGANGLE_ACCELERATION) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (percentage ?percentage&:(< ?percentage 10)))
	=>
	(printout t "急ハンドル判定：急ハンドル少ない" crlf)
	(printout qt "プロファイル23 急ハンドル特性判定：急ハンドル少ない : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "急ハンドル情報 : " ?percentage1 " %" crlf)
	;(bind ?*list* (modify ?*list* (suddenStAngleAccel "急ハンドル少ない")))
	(printout eventoutput "suddenStAngleAccel 急ハンドル少ない")
	;(bind ?*list* (modify ?*list* (suddenStAngleAccelCount (length$ ?specificList1))))
	(printout eventoutput (str-cat "suddenStAngleAccelCount" " " (length$ ?specificList1)))
	(if (neq ?specific NORMAL)
		then
		(modify ?acce (specific NORMAL))))

(defrule SpecificAgenda::プロファイル23-状態判定：いつもと同じ
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type STEERINGANGLE_ACCELERATION) (userStateCheck TRUE) (specificList $?specificList&:(>= (length$ ?specificList) 110)) (todaySpecificState ?todaySpecificState&:(>= ?todaySpecificState 0.05)))
	=>
	(printout t "プロファイル23 急ハンドル状態判定：いつもと同じ" crlf)
	(printout qt "プロファイル23 急ハンドル状態判定：いつもと同じ : 二項検定結果 " ?todaySpecificState crlf)
	;(bind ?*list* (modify ?*list* (suddenStAngleAccelState "いつもと同じ")))
	(printout eventoutput "suddenStAngleAccelState いつもと同じ")
	)

(defrule SpecificAgenda::プロファイル23-状態判定：いつもと違う
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type STEERINGANGLE_ACCELERATION))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type STEERINGANGLE_ACCELERATION) (userStateCheck TRUE) (specificList $?specificList&:(>= (length$ ?specificList) 110)) (todaySpecificState ?todaySpecificState&:(< ?todaySpecificState 0.05)))
	=>
	(printout t "プロファイル23 急ハンドル状態判定：いつもと違う" crlf)
	(printout qt "プロファイル23 急ハンドル状態判定：いつもと違う : 二項検定結果 " ?todaySpecificState crlf)
	;(bind ?*list* (modify ?*list* (suddenStAngleAccelState "いつもと違う")))
	(printout eventoutput "suddenStAngleAccelState いつもと違う")
	)
