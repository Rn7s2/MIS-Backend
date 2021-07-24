(load "~/.quicklisp/setup.lisp")

(ql:quickload :sqlite)
(use-package :sqlite)

(defvar *db* (connect "~/mis.db"))

(defparameter all-template-list (execute-to-list *db* "SELECT * FROM template"))

(format t "Content-type: application/json~%~%")

(format t "[")
(let ((size (list-length all-template-list)))
  (defun print-string-value (key value &rest is-last-item)
    (format t "\"~A\":\"~A\"," key value)
    (when (not is-last-item)
      (format t ",")))
  (defun prinf-number-value (key value)
    (format t "\"~A\":~A," key value))

  (defun print-template (x)
    (format t "{")
    (print-number-value "id" (nth 1 x))
    (print-string-value "name" (nth 2 x))
    (print-string-value "main_issue" (nth 3 x))
    (print-string-value "current_illness" (nth 4 x))
    (print-string-value "past_illness" (nth 5 x))
    (print-string-value "family_illness" (nth 6 x))
    (print-string-value "check" (nth 7 x))
    (print-string-value "main_diagnosis" (nth 8 x))
    (print-string-value "method" (nth 9 x))
    (print-string-value "note" (nth 10 x) t)
    (format t "}"))

  (print-template (first all-template-list))
  (dotimes (i (- size 1))
    (format t ",")
    (print-template (nth (+ 2 i) all-template-list)))
  (format t "]"))

(disconnect *db*)
