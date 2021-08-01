#!/usr/bin/racket
#lang racket/base

(require srfi/13)
(require json)
(require db)

(define mis (sqlite3-connect #:database "/var/lib/wwwrun/mis.db"))

(define text (hash-ref (read-json) 'text))
(define id (let ([tmp (query-value mis "SELECT MAX(id) FROM record")])
             (if (number? tmp)
                 tmp
                 1)))

(define (set-content key val)
  (define str (substring text (string-contains text (string-append key "："))))
  (define (multi-apply f x n)
    (define (multi-apply-helper n ret)
      (if (<= n 0)
          ret
          (multi-apply-helper (- n 1) (f ret))))
    (multi-apply-helper n x))
  (define begin-str (multi-apply (lambda (x)
                                   (substring x (+ 1 (string-contains x ">")))) str 5))
  (string-append (substring text 0 (string-contains text begin-str))
                 val
                 (substring begin-str (string-contains begin-str "<"))))

(query-exec mis "INSERT INTO record VALUES ($1,$2)" id (set-content "就诊号" (number->string id)))

(display "Content-type: text/html\n\n")

(disconnect mis)
