#!/usr/bin/racket
#lang racket/base

(require json)
(require db)

(define mis (sqlite3-connect #:database "/var/lib/wwwrun/mis.db"))

(define prescription (read-json))
(define date (hash-ref prescription 'date))
(define patient (hash-ref prescription 'patient))
(define medicine-list (hash-ref prescription 'selectedMedicine))

(define has-record-id (hash-has-key? prescription 'recordID))

(cond [has-record-id (let ([record-id (hash-ref prescription 'recordID)])
                       (query-exec mis
                                   (string-append "INSERT INTO `prescription` (`record_id`,`date`,`name`,"
                                                  "`gender`,`age`,`diagnosis`) VALUES($1,$2,$3,$4,$5,$6)")
                                   record-id
                                   date
                                   (hash-ref patient 'name)
                                   (hash-ref patient 'gender)
                                   (hash-ref patient 'age)
                                   (hash-ref patient 'diagnosis)))]
      [else (query-exec mis
                        (string-append "INSERT INTO `prescription` (`date`,`name`,`gender`,"
                                       "`age`,`diagnosis`) VALUES ($1,$2,$3,$4,$5)")
                        date
                        (hash-ref patient 'name)
                        (hash-ref patient 'gender)
                        (hash-ref patient 'age)
                        (hash-ref patient 'diagnosis))])

(define id (query-value mis "SELECT MAX(id) AS MAX_ID FROM prescription"))

(define table (string-append "p_" (number->string id)))

(query-exec mis (string-append "CREATE TABLE "
                               table
                               "(`medicine_id` INTEGER NOT NULL,"
                               "`number` INTEGER NOT NULL)"))

(for ([i medicine-list])
  (let ([medicine-id (hash-ref i 'id)]
        [number (hash-ref i 'number)])
    (query-exec mis
                (string-append "INSERT INTO "
                               table
                               " VALUES ($1, $2)")
                medicine-id
                number)))

(printf "Content-type: text/html~%~%")

(disconnect mis)
