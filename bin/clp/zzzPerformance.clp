
(defrule SpecificAgenda::performanceStart "Agendaの中で一番最初に実行されることを想定しています"
        (declare (salience 10000))
        (EventSpeedList)
        =>
        (printout t "[Performance] SpecificAgenda start: " (time) crlf)

)


(defrule SpecificAgenda::performanceEnd "Agendaの中で一番最後に実行されることを想定しています"
        (declare (salience -10000))
        (EventSpeed)
        =>
        (printout t "[Performance] SpecificAgenda end:   " (time) crlf)
)
