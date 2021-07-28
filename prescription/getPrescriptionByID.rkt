#!/usr/local/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define id (hash-ref (read-json) 'id))

(define prescription (query-maybe-row mis "SELECT * FROM prescription WHERE id=$1" id))

(printf "Content-type: application/json~%~%")

(write-json (let ([patient (make-immutable-hasheq `((name . ,(vector-ref prescription 4))
                                                    (gender . ,(vector-ref prescription 5))
                                                    (age . ,(vector-ref prescription 6))
                                                    (diagnosis . ,(vector-ref prescription 7))))]
                  [record-id (if (sql-null? (vector-ref prescription 1))
                                 ""
                                 (vector-ref prescription 1))]
                  [is-handled (vector-ref prescription 2)]
                  [date (vector-ref prescription 3)]
                  [selected-medicine (for/list ([(medicine-id number)
                                                 (in-query mis (string-append "SELECT * FROM p_"
                                                                              (number->string id)))])
                                       (make-immutable-hasheq `((id . ,medicine-id)
                                                                (number . ,number))))])
              (make-immutable-hasheq `((id . ,id)
                                       (recordID . ,record-id)
                                       (isHandled . ,is-handled)
                                       (date . ,date)
                                       (patient . ,patient)
                                       (selectedMedicine . ,selected-medicine)))))

(disconnect mis)
