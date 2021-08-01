#!/usr/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/var/lib/wwwrun/mis.db"))

(define id (hash-ref (read-json) 'id))
(define table (string-append "p_" (number->string id)))

(for ([(medicine-id number) (in-query mis (string-append "SELECT * FROM " table))])
  (let ([medicine-table (string-append "m_" (number->string medicine-id))])
    (query-exec mis "UPDATE medicine SET number=number-$1 WHERE id=$2" number medicine-id)
    (query-exec mis (string-append "INSERT INTO "
                                   medicine-table
                                   " VALUES (date('now'),$1)") (- 0 number))))

(query-exec mis "UPDATE prescription SET is_handled=1 WHERE id=$1" id)

(printf "Content-type: text/html~%~%")

(disconnect mis)
