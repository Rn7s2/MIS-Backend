#!/usr/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/var/lib/wwwrun/mis.db"))

(define id (hash-ref (read-json) 'id))

(display "Content-type: text/html\n\n")

(display (query-value mis "SELECT `content` FROM record WHERE id=$1" id))

(disconnect mis)
