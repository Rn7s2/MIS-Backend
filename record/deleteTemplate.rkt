#!/usr/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/var/lib/wwwrun/mis.db"))

(define id (hash-ref (read-json) 'id))

(query-exec mis "DELETE FROM template WHERE `id`=$1" id)

(printf "Content-type: text/html~%~%")

(disconnect mis)
