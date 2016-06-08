;==================================================
; Function(SpeedAccelerationCheck.clp)
;==================================================
;急加速度頻度計算関数
(deffunction SpecificAgenda::calcAccelPercentage(?data)
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
(deffunction SpecificAgenda::calrAccelCombination(?n ?x)
	(bind ?combi 1)
	(if (and (<= 1 ?x) (< ?x ?n)) then
		(bind ?d ?x)
		(while (>= ?d 1) do
			(bind ?combi (* ?combi (/ (- (+ ?n 1) ?d) ?d)))
			(bind ?d (- ?d 1))))
	(return ?combi))

;二項検定によるドライバ状態判定関数
(deffunction SpecificAgenda::judUserAccelState(?data)
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
		(bind ?cumProb (+ ?cumProb (* (calrAccelCombination ?state_check_size ?i) (brmsPower ?p ?i) (brmsPower (- 1 ?p) (- ?state_check_size ?i)))))
		(if (>= ?cumProb 0.05) then
			(break)))
	
	;いつもと違う状態
	(bind ?todaySpecificState (fact-slot-value ?data todaySpecificState))
	(if (<> ?todaySpecificState ?cumProb) then
		(bind ?data (modify ?data (todaySpecificState ?cumProb))))
	
	(return ?data))

;==================================================
; Rule(SpeedAccelerationCheck.clp)
;==================================================
(defrule SpecificAgenda::急加速時の加速度検出
	(declare (salience 600))
	
	(format qt "******* 急加速時の加速度検出 *******" crlf)
	
	(EventAcceleration (from entryPoint) (name "Calculation Stream") (acceleration ?acceleration&:(> ?acceleration 2.22)))
	?b <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ACCEL) (value1 ~?acceleration))
	=>
	(format qt "SpecificAgenda::急加速時の加速度検出 %.1f%n" ?acceleration crlf)
	(modify ?b (value1 ?acceleration)))

(defrule SpecificAgenda::計算:急加速度頻度/ドライバ状態判定
	(declare (salience 300))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	=>
	(bind ?acce (calcAccelPercentage ?acce))
	(bind ?acce (judUserAccelState ?acce))
	(duplicate ?acce (name "Calculation Stream")))

(defrule SpecificAgenda::集計：急加速情報：サンプル未達
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION) (specificList $?specificList1) (percentage ?percentage))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type SPEED_ACCELERATION) (specificList $?specificList&:(< (length$ ?specificList) 100)))
	=>
	(printout t "①急加速判定：サンプル数未達 : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "プロファイル① 急加速特性判定：サンプル数未達 : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "□急加速頻度 : " ?percentage " %" crlf)
	(bind ?*list* (modify ?*list* (suddenAccelCount (length$ ?specificList1)))))

(defrule SpecificAgenda::集計：急加速情報：急加速しがちなタイプ
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION) (specific ?specific) (specificList $?specificList1) (percentage ?percentage1))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type SPEED_ACCELERATION) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (percentage ?percentage&:(>= ?percentage 25)))
	=>
	(printout t "①急加速判定：急加速しがちなタイプ" crlf)
	(printout qt "プロファイル① 急加速特性判定：急加速しがちなタイプ : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "□急加速頻度 : " ?percentage1 " %" crlf)
	(bind ?*list* (modify ?*list* (suddenAccel "急加速しがちなタイプ")))
	(bind ?*list* (modify ?*list* (suddenAccelCount (length$ ?specificList1))))
	(if (neq ?specific SUDDEN) then
		(modify ?acce (specific SUDDEN))))

(defrule SpecificAgenda::集計：急加速情報：急加速あまりしないタイプ
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION) (specific ?specific) (specificList $?specificList1) (percentage ?percentage1))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type SPEED_ACCELERATION) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (percentage ?percentage&:(< ?percentage 25)))
	=>
	(printout t "①急加速判定：急加速あまりしないタイプ" crlf)
	(printout qt "プロファイル① 急加速特性判定：急加速あまりしないタイプ : サンプル数 -> " (length$ ?specificList1) crlf)
	(printout qt "□急加速頻度 : " ?percentage1 " %" crlf)
	(bind ?*list* (modify ?*list* (suddenAccel "急加速あまりしないタイプ")))
	(bind ?*list* (modify ?*list* (suddenAccelCount (length$ ?specificList1))))
	(if (neq ?specific NORMAL)
		then
		(modify ?acce (specific NORMAL))))

(defrule SpecificAgenda::プロファイル1-状態判定：いつもと同じ
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type SPEED_ACCELERATION) (userStateCheck TRUE) (specificList $?specificList&:(>= (length$ ?specificList) 110)) (todaySpecificState ?todaySpecificState&:(>= ?todaySpecificState 0.05)))
	=>
	(printout t "プロファイル① 急加速状態判定：いつもと同じ" crlf)
	(printout qt "プロファイル① 急加速状態判定：いつもと同じ : 二項検定結果 " ?todaySpecificState crlf)
	(bind ?*list* (modify ?*list* (suddenAccelState "いつもと同じ"))))

(defrule SpecificAgenda::プロファイル1-状態判定：いつもと違う
	(declare (salience 200))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type SPEED_ACCELERATION))
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type SPEED_ACCELERATION) (userStateCheck TRUE) (specificList $?specificList&:(>= (length$ ?specificList) 110)) (todaySpecificState ?todaySpecificState&:(< ?todaySpecificState 0.05)))
	=>
	(printout t "プロファイル① 急加速状態判定：いつもと違う" crlf)
	(printout qt "プロファイル① 急加速状態判定：いつもと違う : 二項検定結果 " ?todaySpecificState crlf)
	(bind ?*list* (modify ?*list* (suddenAccelState "いつもと違う"))))
