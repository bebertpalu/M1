(defun facto(n) 
	(if (= n 0)
		1
	(* n(facto(- n 1)))))
