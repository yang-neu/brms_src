;==================================================
; Function(RightTurnSpeed.clp)
;==================================================
;最大舵角を算出する
(deffunction SpecificAgenda::calcRightSteeringAngleMax (?acce ?calc)
	(bind ?steeringAngleMax (fact-slot-value ?calc steeringAngleMax))
	;车速list取得
	(bind ?steeringAngleList (fact-slot-value ?acce steeringAngleList))
	(bind ?length (/ (length$ ?steeringAngleList) 3))
	(loop-for-count (?i 1 ?length) do
			(if (< (abs ?steeringAngleMax) (abs (nth$  (* 3 ?i) ?steeringAngleList))) then
				(bind ?steeringAngleMax (nth$  (* 3 ?i) ?steeringAngleList))))
	(bind ?calc (modify ?calc (steeringAngleMax ?steeringAngleMax )))
	(return ?calc))
;アクセル運転状態判定
(deffunction SpecificAgenda::judRightTurnState(?calc ?data)

	(bind ?startDriveScene (fact-slot-value ?calc startDriveScene))
	(bind ?specificList (fact-slot-value ?calc specificList))
	(bind ?endDriveScene ?data)
	
	;アクセル運転状態判定
	(if (and (eq ?startDriveScene REDUCE)
		(eq ?endDriveScene ACCEL)) then
			(bind ?calc (modify ?calc (specificList SLOW_IN_FAST_OUT ?specificList)))
	else (if (and (eq ?startDriveScene REDUCE)
		(or (eq ?endDriveScene REDUCE) (eq ?endDriveScene RUNNING))) then
			(bind ?calc (modify ?calc (specificList REDUCE_TURN ?specificList)))
	else (if (and (or (eq ?startDriveScene ACCEL) (eq ?startDriveScene RUNNING))
		(or (eq ?endDriveScene ACCEL) (eq ?endDriveScene RUNNING))) then
			(bind ?calc (modify ?calc (specificList ACCEL_TURN ?specificList)))
	else 
		(bind ?calc (modify ?calc (specificList NAN ?specificList))))))
	(return ?calc))

;最頻度の特性に更新する
(deffunction SpecificAgenda::calcRightTurnProperty(?data)

	(bind ?SlowInFastOutCount 0)
	(bind ?ReduceTurnCount 0)
	(bind ?AccelTurnCount 0)
	(bind ?UndecidedCount 0)

	(bind ?dataSize 0)
	(bind ?max_sample_size (fact-slot-value ?data MAX_SAMPLE_SIZE))
	(bind ?specificList (fact-slot-value ?data specificList))
	
	;最新30サンプルを集計
	(while (and (< ?dataSize (length$ ?specificList)) (< ?dataSize ?max_sample_size)) do
		(bind ?tmpSpecific (nth$ (+ ?dataSize 1) ?specificList))
		(if (eq ?tmpSpecific SLOW_IN_FAST_OUT) then
			(bind ?SlowInFastOutCount (+ ?SlowInFastOutCount 1))
		else (if (eq ?tmpSpecific REDUCE_TURN) then
			(bind ?ReduceTurnCount (+ ?ReduceTurnCount 1))
		else (if (eq ?tmpSpecific ACCEL_TURN) then
			(bind ?AccelTurnCount (+ ?AccelTurnCount 1))
		else
			(bind ?UndecidedCount (+ ?UndecidedCount 1)))))
		(bind ?dataSize (+ ?dataSize 1)))

	(if(and (> ?SlowInFastOutCount ?ReduceTurnCount) (> ?SlowInFastOutCount ?AccelTurnCount) (> ?SlowInFastOutCount ?UndecidedCount)) then
		(bind ?data (modify ?data (specific SLOW_IN_FAST_OUT)))
	else (if(and (> ?ReduceTurnCount ?SlowInFastOutCount) (> ?ReduceTurnCount ?AccelTurnCount) (> ?ReduceTurnCount ?UndecidedCount)) then
		(bind ?data (modify ?data (specific REDUCE_TURN)))
	else (if(and (> ?AccelTurnCount ?SlowInFastOutCount) (> ?AccelTurnCount ?ReduceTurnCount) (> ?AccelTurnCount ?UndecidedCount)) then
		(bind ?data (modify ?data (specific ACCEL_TURN)))
	else 
		(bind ?data (modify ?data (specific NAN))))))
	(return ?data))
;==================================================
; Rule(RightTurnSpeed.clp)
;==================================================
(defrule SpecificAgenda::車速20㎞/h以上時：右カーブのstep開始状態検出
	(declare (salience 600))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type RIGHT_TURN_SPEED) (step ?step&:(eq ?step STEP_END)))
	(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture TURN_STRAIGHT))
	(EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (speed ?speed&: (>= ?speed 20)))
	=>
	(bind ?acce (modify ?acce (step STEP_START)))
	(bind ?acce (modify ?acce (accumulateTime 0.0)))
	(bind ?acce (modify ?acce (sceneStartTime 0.0)))
	(bind ?acce (modify ?acce (startDriveScene STOP)))
	(bind ?acce (modify ?acce (steeringAngleMax 0.0)))
	(printout t "⑯車速20㎞/h以上時：右カーブのstep開始状態検出:step:" (fact-slot-value ?acce step) crlf))

(defrule SpecificAgenda::右カーブのシーン成立時：右カーブ開始時刻検出
	(declare (salience 600))
	
	?acce <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type RIGHT_TURN_SPEED) (step ?step&:(eq ?step STEP_START)))
	(EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene))
	(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture TURN_RIGHT) (beforeVehiclePostureScene TURN_STRAIGHT))
	=>
	(bind ?acce (modify ?acce (sceneStartTime (time))))
	(bind ?acce (modify ?acce (startDriveScene ?driveScene)))
	(bind ?acce (modify ?acce (step STEP_CALC))))

(defrule SpecificAgenda::右カーブのシーン成立中の最大舵角を算出する
	(declare (salience 600))
	
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type RIGHT_TURN_SPEED) (step ?step&:(eq ?step STEP_CALC)))
	?acce <- (EventSteeringAngleList (from entryPoint) (name "Receiving Data Stream"))
	=>
	(bind ?calc (calcRightSteeringAngleMax ?acce ?calc))
	(printout t "⑯右カーブのシーン成立中の最大舵角を算出する : " (fact-slot-value ?calc steeringAngleMax) "steeringAngleMax" crlf))

(defrule SpecificAgenda::計算:右カーブの最大舵角が150度以上　かつ、シーン成立時間が2s以上の時
	(declare (salience 500))
	
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type RIGHT_TURN_SPEED) (step ?step&:(eq ?step STEP_CALC))
		(sceneStartTime ?sceneStartTime) (accumulateTime ?accumulateTime&:(>= (- (time) ?sceneStartTime) 2)) (steeringAngleMax ?steeringAngleMax&:(<= ?steeringAngleMax -150)))
	(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture TURN_STRAIGHT))
	?acce <- (EventDriveScene (from entryPoint) (name "Current Scene Stream") (driveScene ?driveScene))
	=>
	(bind ?calc (modify ?calc (accumulateTime (- (time) ?sceneStartTime))))
	(bind ?calc (judRightTurnState ?calc ?driveScene))
	(bind ?calc (calcRightTurnProperty ?calc))
	(bind ?calc (modify ?calc (step STEP_END)))
	(duplicate ?calc (name "Calculation Stream")))

(defrule SpecificAgenda::右カーブのシーンを抜けた時:step終わり状態検出
	(declare (salience 400))
	
	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Characteristic And Status Stream") (type RIGHT_TURN_SPEED) (step ?step&:(neq ?step STEP_END)))
	(or (EventSpeed (from entryPoint) (name "Current Receiving Data Stream") (speed ?speed&: (< ?speed 20)))
		(EventVehiclePostureScene (from entryPoint) (name "Current Scene Stream") (posture ?posture&:(neq ?posture TURN_RIGHT)&:(eq ?step STEP_CALC))))
	=>
	(bind ?calc (modify ?calc (step STEP_END)))
	(printout t "⑯車速20㎞/h未滿時：step終わり状態検出:step:" ?step crlf))

(defrule SpecificAgenda::集計：右カーブスピード：サンプル未達
	(declare (salience 200))

	?margin <- (EventCharacteristicAndStatusBinomial(from entryPoint) (name "Calculation Stream") (type RIGHT_TURN_SPEED) (specificList $?specificList&:(< (length$ ?specificList) 100)))
	=>
	(printout t "⑯右カーブスピード：サンプル数未達 : サンプル数 -> " (length$ ?specificList) crlf)
	(printout qt "プロファイル⑯ 右カーブスピード：サンプル数未達 : サンプル数 ->" (length$ ?specificList) crlf)
	;(bind ?*list* (modify ?*list* (LeftTurnCount (length$ ?specificList))))
	(printout eventoutput (str-cat "RightTurnCount" " " (length$ ?specificList)))
	)

(defrule SpecificAgenda::集計：右カーブスピード：特性未定義
	(declare (salience 200))

	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type RIGHT_TURN_SPEED) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (specific NAN))
	=>
	(printout t "⑯右カーブスピード： 判定： 特性未定義" crlf)
	(printout qt "プロファイル⑯ 右カーブスピード： 特性判定： 特性未定義" crlf)
	;(bind ?*list* (modify ?*list* (LeftTurnState  "未定義")))
	(printout eventoutput "RightTurnState 未定義")
	;(bind ?*list* (modify ?*list* (LeftTurnCount (length$ ?specificList))))
	(printout eventoutput (str-cat "RightTurnCount" " " (length$ ?specificList)))
	)

(defrule SpecificAgenda::集計：右カーブスピード：スローインファストアウトカーブ
	(declare (salience 200))

	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type RIGHT_TURN_SPEED) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (specific SLOW_IN_FAST_OUT))
	=>
	(printout t "⑯右カーブスピード 判定： スローインファストアウトカーブ" crlf)
	(printout qt "プロファイル⑯右カーブスピード 特性判定： スローインファストアウトカーブ" crlf)
	;(bind ?*list* (modify ?*list* (LeftTurnState "スローインファストアウトカーブ")))
	(printout eventoutput "RightTurnState スローインファストアウトカーブ")
	;(bind ?*list* (modify ?*list* (LeftTurnCount (length$ ?specificList))))
	(printout eventoutput (str-cat "RightTurnCount" " " (length$ ?specificList)))
	)

(defrule SpecificAgenda::集計：右カーブスピード：減速カーブ
	(declare (salience 200))

	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type RIGHT_TURN_SPEED) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (specific REDUCE_TURN))
	=>
	(printout t "⑯右カーブスピード 判定： 減速カーブ" crlf)
	(printout qt "プロファイル⑯右カーブスピード 特性判定： 減速カーブ" crlf)
	;(bind ?*list* (modify ?*list* (LeftTurnState "減速カーブ")))
	(printout eventoutput "RightTurnState 減速カーブ")
	;(bind ?*list* (modify ?*list* (LeftTurnCount (length$ ?specificList))))
	(printout eventoutput (str-cat "RightTurnCount" " " (length$ ?specificList)))
	)
	
(defrule SpecificAgenda::集計：右カーブスピード：非減速カーブ
	(declare (salience 200))

	?calc <- (EventCharacteristicAndStatusBinomial (from entryPoint) (name "Calculation Stream") (type RIGHT_TURN_SPEED) (specificList $?specificList&:(>= (length$ ?specificList) 100)) (specific ACCEL_TURN))
	=>
	(printout t "⑯右カーブスピード 判定： 非減速カーブ" crlf)
	(printout qt "プロファイル⑯右カーブスピード 特性判定： 非減速カーブ" crlf)
	;(bind ?*list* (modify ?*list* (LeftTurnState "非減速カーブ")))
	(printout eventoutput "RightTurnState 非減速カーブ")
	;(bind ?*list* (modify ?*list* (LeftTurnCount (length$ ?specificList))))
	(printout eventoutput (str-cat "RightTurnCount" " " (length$ ?specificList)))
	)