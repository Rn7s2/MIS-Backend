#!/usr/local/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define record (read-json))

(query-exec mis
            "UPDATE record SET `content`=$1 WHERE `id`=$2"
            (hash-ref record 'text)
            (hash-ref record 'id))

(display "Content-type: application/json\n\n")

(disconnect mis)
