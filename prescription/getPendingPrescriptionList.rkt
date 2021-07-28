#!/usr/local/bin/racket
#lang racket/base

(require racket/sequence)
(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(printf "Content-type: text/html~%~%")

(write-json (for/list ([(id name date) (in-query mis (string-append "SELECT `id`, `name`, `date` "
                                                                    "FROM prescription WHERE "
                                                                    "`is_handled`=0"))])
              (make-immutable-hasheq `((id . ,id)
                                       (name . ,name)
                                       (date . ,date)))))

(disconnect mis)
