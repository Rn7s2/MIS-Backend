#!/usr/local/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define id (hash-ref (read-json) 'id))

(query-exec mis "DELETE FROM prescription WHERE id=$1" id)

(printf "Content-type: text/html~%~%")

(disconnect mis)
