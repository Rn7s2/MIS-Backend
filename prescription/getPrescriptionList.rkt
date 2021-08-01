#!/usr/bin/racket
#lang racket/base

(require racket/sequence)
(require json)
(require db)

(define mis (sqlite3-connect #:database "/var/lib/wwwrun/mis.db"))

(printf "Content-type: text/html~%~%")

(define res (in-query mis "SELECT `id`,`name`,`date` FROM prescription"))

(write-json
 (if (> (sequence-length res) 0)
     (for/list ([(id name date) res])
       (make-immutable-hasheq `((id . ,id)
                                (patient . ,name)
                                (date . ,date))))
     '()))

(disconnect mis)
