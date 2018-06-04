;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Your solution goes in this file ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun element-count (elem x)
    (do ((lst x)(count 0))
        ((null lst) count)
        (if (eql (car lst) elem)
            (setq count (+ 1 count))
        )
        (setq lst (cdr lst))
    )
)

(defun min-mean-max (x)
    (do ((lst x) (ret nil) (min 10000) (mean 0) (max -10000) (elemCount 0) (sum 0)) ;Initialize
        ((null lst) ret) ;Condition and return value
        (if (< (car lst) min) ;Compare to find smallest and largest elemens
            (setq min (car lst))
        )
        (if (> (car lst) max)
            (setq max (car lst))
        )
        (setq elemCount (+ 1 elemCount))
        (setq sum (+ (car lst) sum))
        (setq mean (/ sum elemCount))
        (setq ret (list min mean max))
        (setq lst (cdr lst)) ;Advance list
    )
)

(defun shift (num x)
    (do ((lst x) )
        ((or (<= num 0) (not lst) ) lst)
        (setq lst (nconc (cdr lst) (list (car lst) ) ) ) ;Append first element to cdr of list
        (setq num (- num 1))
    )
)

(defun pivot (num x)
    (do ((lst x) (lst1 nil) (lst2 nil)) ;Init
        ((null lst) (list (reverse lst1) (reverse lst2))) ;Return reversed lists to preserve order
        (cond 
            ((> num (car lst)) ;If first val < num
            (setq lst1 (cons (car lst) lst1))) ;Add to first list
            
            ((<= num (car lst)) ;If first val >= num
            (setq lst2 (cons (car lst) lst2))) ;Add to second list
        )
        (setq lst (cdr lst)) ;Advance list
    )
)

(defun break-list (num x)
    (do ((lst x) (currList nil) (ret nil))
        ((null lst) ret)
        (cond 
            ((eql (car lst) num) ;If first val == num
                (progn
                    (if currList ;Append current list to ret if it exists
                        (setq ret (append ret (list currList) )) 
                    )
                    (setq currList nil)) ;Reset
            )
            ((not (eql (car lst) num)) ;Else not equal
                (setq currList (append currList (list (car lst) ) ) ) ;Else keep appending to currList
            )    
            
        )
        (setq lst (cdr lst)) ;Advance list

        (if (and (not (car lst)) currList ) ;Check if last element and currList is not nil
            (setq ret (append ret (list currList) ))
        )
    )
)

(defun quicksort (L)
	(if (null L)
		nil
		(let* (
				(split (pivot (car L) (cdr L)) )
				(smaller (car split))
				(larger (cadr split))
			)
			(append 
				(quicksort smaller)
				(cons (car L) nil)
				(quicksort larger)
			)
		)		
	)
)


;Check that all atoms are the same except
;Symbol ? in pattern represents any single atom
;Symbol ! in pattern matches any one or more atoms
(defun match (pattern assertion)
    (if (equal pattern assertion) ;True
        (return-from match T)
    )
    (if (> (list-length pattern) (list-length assertion) ) ;Pattern cannot be greater than assertion
        (return-from match nil)
    )
    (loop while pattern do
       (setq pElem (car pattern)) ;Check against first element of pattern
       (cond  
            ((eq pElem '?) ;Curr element of pattern is ?
                (if (null assertion)  ;Empty list
                    (return-from match nil)
                )
            );Check for non nil assertion
            ((eq pElem '!) ;Curr element of pattern is !
                (loop while(eq pElem '!) do
                    (if (null assertion)
                        (return-from match nil)
                    )
                    (setq pattern (cdr pattern)) ;Move list up until it is not !
                    (setq assertion (cdr assertion))
                    (setq pElem (car pattern)) ;Assertion now must have AT LEAST that many elements until next char
                )
                (if (null (car pattern)) ;Reached the end of pattern and assertion is same length 
                    (return-from match T)
                )
                (setq nextElem pElem) ;Go until reach this element
                (if (eq pElem '?)
                    (return-from match (not (null assertion)))
                )
                (loop while (not (eq nextElem (car assertion)) ) do ;Loop until the front of the list does not equal the nextElem
                    (if (not (car assertion) ) ;Reached the end of assertion so false
                        (return-from match nil)
                    )
                    (setq assertion (cdr assertion)) ;Move assertion forward
                )
            )
            (t  ;Else regular atom
                (if (not assertion) ;Assertion is empty
                    (return-from match nil)
                )
                (if (not (eq pElem (car assertion))) ;The two are not equal so return false
                    (return-from match nil)
                )
            )
       )
       (setq assertion (cdr assertion)) ;Move both lists up
       (setq pattern (cdr pattern))
    )
   (null assertion) ;Returns whether the list is empty or not 
)

(defun matrix-add (mat1 mat2)
    (setq sum nil) ;Init sum matrix to an empty list
    (loop while mat1 do
        (setq row1 (car mat1)) ;Iterate through each row
        (setq row2 (car mat2))
        (setq sumRow nil)

        (loop while row1 do
            (setq res (+ (car row1) (car row2))) ;Add and append to sum row
            ;(print res)
            (setq sumRow (nconc sumRow (list res)))

            (setq row1 (cdr row1)) ;Next col in row
            (setq row2 (cdr row2))
        )
        (setq sum (nconc sum (list sumRow)))  ;Add the new row to matrix
        ;(print sumRow)

        (setq mat1 (cdr mat1)) ;Next row
        (setq mat2 (cdr mat2))
    ) 
    sum 
)

(defun matrix-multiply (mat1 mat2)
    ;Take transpose of matrix 2
    ;Iterate through each row combo and add product of two vals to new 
    (setq product nil)
    (setq mat2 (matrix-transpose mat2)) ;Transpose the second matrix
   
    (setq newMat2 mat2) ;Use copy to traverse
    (loop while mat1 do
        (setq row1 (car mat1))
        (setq oldRow1 row1) ;Modify copy
        (setq productRow nil) ;Start a new row

        (loop while newMat2 do ;Iterate through every row in matrix 2
            (setq sumOfProducts 0)
            (setq row2 (car newMat2))
            (loop while row1 do ;Loop thru the whole row of current first matrix and add up the products
                (setq prod (* (car row1) (car row2)))
                (setq sumOfProducts (+ sumOfProducts prod))

                (setq row1 (cdr row1)) ;Move rows up
                (setq row2 (cdr row2))
            )
            (setq productRow (nconc productRow (list sumOfProducts))) ;Append to row of product
            (setq row1 oldRow1) ;Reset row and move up to next row in first matrix
            (setq newMat2 (cdr newMat2))
        )

        ;Move up to next row in mat1 and append row to product
        (setq mat1 (cdr mat1))
        (setq product (nconc product (list productRow)))
        (setq newMat2 mat2) ;Reset mat2
    )
    product
)

(defun matrix-transpose (mat)
    ;Take the car of each list and make it a new list
    ;Append and move it each of them forward
    (setq transpose nil)

    (loop while (car mat) do
        (setq tRow nil)
        (setq currMat mat) ;Create a copy to traverse

        (loop while currMat do
            (setq val (caar currMat))
            (setq tRow (nconc tRow (list val))) ;Append the elem at the front of each list to new row
            (setq currMat (cdr currMat)) ;Next row
        )
        (setq transpose (nconc transpose (list tRow))) ;Append to new matrix

        ;Move each row up one
        (map-into mat #'rest mat) 
    )

    transpose
)



