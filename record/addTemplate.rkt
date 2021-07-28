#!/usr/local/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define template (read-json))

(query-exec mis
            (string-append "INSERT INTO template(`name`,`main_issue`,"
                           "`current_illness`,`past_illness`,`family_illness`,"
                           "`check`,`main_diagnosis`,`method`,`note`) VALUES "
                           "($1,$2,$3,$4,$5,$6,$7,$8,$9)")
            (hash-ref template 'name)
            (hash-ref template 'main_issue)
            (hash-ref template 'current_illness)
            (hash-ref template 'past_illness)
            (hash-ref template 'family_illness)
            (hash-ref template 'check)
            (hash-ref template 'main_diagnosis)
            (hash-ref template 'method)
            (hash-ref template 'note))

(display "Content-type: application/json\n\n")

(disconnect mis)
