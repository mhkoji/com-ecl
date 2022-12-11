(defpackage :com-ecl
  (:use :cl)
  (:export :make-acc
           :acc-put
           :acc-to-string))
(in-package :com-ecl)

(defclass acc ()
  ((values :initform nil
	   :accessor acc-values)))
(defstruct value-number num)
(defstruct value-string str)

(defun make-acc ()
  (make-instance 'acc))

(defgeneric acc-put (acc raw))

(defmethod acc-put ((acc acc) (num number))
  (push (make-value-number :num num)
	(acc-values acc)))

(defmethod acc-put ((acc acc) (str string))
  (push (make-value-string :str (copy-seq str))
	(acc-values acc)))


(defgeneric to-string (val))

(defmethod to-string ((val value-number))
  (format nil "~A" (value-number-num val)))

(defmethod to-string ((val value-string))
  (value-string-str val))

(defun acc-to-string (acc)
  (let ((vals (acc-values acc)))
    (if (null vals)
	""
	(let ((strs (mapcar #'to-string vals)))
          (with-output-to-string (s)
            (format s "~A" (car strs))
            (dolist (str (cdr strs))
              (format s ", ~A" str)))))))
