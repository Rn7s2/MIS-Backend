#!/usr/local/bin/racket
#lang racket/base

(require srfi/13)
(require json)
(require db)

(define mis (sqlite3-connect #:database "/home/www/mis.db"))

(define all-record-data (query-rows mis "SELECT * FROM record"))

(display "Content-type: application/json\n\n")
(write-json (for/list ([i all-record-data])
              (define (get-content key)
                (define text (vector-ref i 1))
                (define str (substring text (string-contains text (string-append key "："))))
                (define (multi-apply f x n)
                  (define (multi-apply-helper n ret)
                    (if (<= n 0)
                        ret
                        (multi-apply-helper (- n 1) (f ret))))
                  (multi-apply-helper n x))
                (define begin-str (multi-apply (lambda (x)
                                                 (substring x (+ 1 (string-contains x ">")))) str 5))
                (substring begin-str 0 (string-contains begin-str "<")))
              (make-immutable-hasheq `((id . ,(vector-ref i 0))
                                       (patient . ,(get-content "姓名"))
                                       (date . ,(get-content "日期"))))))

(disconnect mis)
