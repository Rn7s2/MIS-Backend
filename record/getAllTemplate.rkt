#!/usr/local/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define all-template-data (query-rows mis "SELECT * FROM template"))

(display "Content-type: application/json\n\n")
(write-json (for/list ([i all-template-data])
              (make-immutable-hasheq `((id . ,(vector-ref i 0))
                                       (name . ,(vector-ref i 1))
                                       (main_issue . ,(vector-ref i 2))
                                       (current_illness . ,(vector-ref i 3))
                                       (past_illness . ,(vector-ref i 4))
                                       (family_illness . ,(vector-ref i 5))
                                       (check . ,(vector-ref i 6))
                                       (main_diagnosis . ,(vector-ref i 7))
                                       (method . ,(vector-ref i 8))
                                       (note . ,(vector-ref i 9))))))

(disconnect mis)
