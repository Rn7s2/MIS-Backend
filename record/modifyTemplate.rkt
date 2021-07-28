#!/usr/local/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define template (read-json))

(query-exec mis
            (string-append "UPDATE template SET `name`=$1,`main_issue`=$2,`current_illness`=$3,"
                           "`past_illness`=$4,`family_illness`=$5,`check`=$6,"
                           "`main_diagnosis`=$7,`method`=$8,`note`=$9 WHERE id=$10")
            (hash-ref template 'name)
            (hash-ref template 'main_issue)
            (hash-ref template 'current_illness)
            (hash-ref template 'past_illness)
            (hash-ref template 'family_illness)
            (hash-ref template 'check)
            (hash-ref template 'main_diagnosis)
            (hash-ref template 'method)
            (hash-ref template 'note)
            (hash-ref template 'id))

(display "Content-type: application/json\n\n")

(disconnect mis)
