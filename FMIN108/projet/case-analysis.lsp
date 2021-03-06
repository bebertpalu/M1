(defun case-analysis (expr env)
  (if (atom expr)
					;CAS ATOM
      (if (constantp expr)
					;cas litteral
	  (format t "litteral ~s ~%" expr);%->saut à la ligne
	
	(if (find expr env)
					;cas variable, def dans l'environnement
	    (format t "variable environnement ~s ~%" expr)
	  
					;cas erreur
	  (error "~s variable inconnue" expr)
	  )
	)	
    
					;CAS FONCTION
					;lie les variables
    (let ((fun (car expr)) (args (cdr expr)))
      (cond 
       (if (consp fun);fun est un cellule
					;cas des lambda fonction
	(if (eq fun 'lambda)
	    (format t "fonction lambda : ~s ~%" fun)
	  ))	
       
       (if (not (symbolp fun))
	   (error "~s n'est pas un symbol" fun)
	 (if (not (fboundp fun))
	     (error "~s fonction inconnue" fun)
	   (if (macro-function fun)
	       (case-analysis (macroexpand-1 expr))
	     (if (special-form-p fun)
		 (case fun
		       (t 
			(error "forme specialn NYI ~s" fun)
			)))
	 )))       
					;cas d'erreur
       ( t
	 (format t "fonction non reconnu : ~s ~%" expr)
	 )
       
       )
      )
    )	
  )
