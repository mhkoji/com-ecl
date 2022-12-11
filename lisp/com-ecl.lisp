(defpackage :com-ecl
  (:use :cl)
  (:export :make-acc
           :acc-inc
           :acc-value))
(in-package :com-ecl)

(defclass acc ()
  ((:value :initform 0
	   :accessor acc-value)))

(defun make-acc ()
  (make-instance 'acc))

(defgeneric acc-inc (acc val))

(defmethod acc-inc ((acc acc) val)
  (incf (acc-value acc) val)
  (values))
