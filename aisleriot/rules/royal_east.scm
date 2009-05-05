; AisleRiot - royal_east.scm
; Copyright (C) 1999, 2003 Rosanna Yuen <rwsy@mit.edu>
;
; This game is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2, or (at your option)
; any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
; USA

(define BASE-VAL 0)

(define (new-game)
  (initialize-playing-area)
  (set-ace-low)
  (make-standard-deck)
  (shuffle-deck)

  (add-normal-slot DECK)
  (add-normal-slot '())

  (add-blank-slot)


  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-carriage-return-slot)

  (add-blank-slot)
  (add-blank-slot)
  (add-blank-slot)

  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-carriage-return-slot)

  (add-blank-slot)
  (add-blank-slot)
  (add-blank-slot)

  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())

  (deal-cards-face-up 0 '(2 3 5 6 7 9))

  (add-to-score! 1)

  (set! BASE-VAL (get-value (get-top-card 2)))

  (give-status-message)

  (list 6 3)
)

(define (give-status-message)
  (set-statusbar-message (string-append (get-stock-no-string)
					"   "
					(get-base-string))))

(define (get-stock-no-string)
  (string-append (_"Stock left:") " " 
		 (number->string (length (get-cards 0)))))

(define (get-base-string)
  (cond ((and (> BASE-VAL 1)
	      (< BASE-VAL 11))
	 (string-append (_"Base Card:") " " (number->string BASE-VAL)))
	((= BASE-VAL 1)
	 (_"Base Card: Ace"))
	((= BASE-VAL 11)
	 (_"Base Card: Jack"))
	((= BASE-VAL 12)
	 (_"Base Card: Queen"))
	((= BASE-VAL 13)
	 (_"Base Card: King"))
	(#t "")))

(define (button-pressed slot-id card-list)
  (and (not (empty-slot? slot-id))
       (or (= slot-id 1)
	   (= slot-id 3)
	   (= slot-id 5)
	   (= slot-id 6)
	   (= slot-id 7)
	   (= slot-id 9))))

(define (droppable? start-slot card-list end-slot)
  (cond ((or (= end-slot 2)
	     (= end-slot 4)
	     (= end-slot 8)
	     (= end-slot 10))
	 (and (or (and (empty-slot? end-slot)
		       (= (get-value (car card-list)) BASE-VAL))
		  (and (not (empty-slot? end-slot))
		       (eq? (get-suit (get-top-card end-slot))
			    (get-suit (car card-list)))
		       (or (= (+ 1 (get-value (get-top-card end-slot)))
			      (get-value (car card-list)))
			   (and (= (get-value (get-top-card end-slot)) king)
				(= (get-value (car card-list)) ace)))))))
	((> end-slot 2)
	 (and (not (= start-slot end-slot))
	      (or (empty-slot? end-slot)
		  (and (= (get-value (get-top-card end-slot)) ace)
		       (= (get-value (car card-list)) king))
		  (= (get-value (get-top-card end-slot))
		     (+ 1 (get-value (car card-list)))))))
	(#t #f)))

(define (button-released start-slot card-list end-slot)
  (if (droppable? start-slot card-list end-slot)
      (begin
         (move-n-cards! start-slot end-slot card-list)
         (if (or (= end-slot 2)
	             (= end-slot 4)
	             (= end-slot 8)
	             (= end-slot 10))
	         (add-to-score! 1)
	         #t))
	  #f))

(define (button-clicked slot-id)
  (and (= slot-id 0)
       (not (empty-slot? 0))
       (deal-cards-face-up 0 '(1))))

(define (move-to-foundation source target)
  (cond ((eq? target '()) #f)
	((and (not (empty-slot? (car target)))
	      (eq? (get-suit (get-top-card source))
		   (get-suit (get-top-card (car target))))
	      (or (= (+ 1 (get-value (get-top-card (car target))))
		     (get-value (get-top-card source)))
		  (and (= (get-value (get-top-card (car target))) king)
		       (= (get-value (get-top-card source)) ace))))
	 (add-card! (car target) (remove-card source))
	 (add-to-score! 1)
	 #t)
	(#t (move-to-foundation source (cdr target)))))
  
(define (button-double-clicked slot-id)
  (cond ((or (empty-slot? slot-id)
             (= slot-id 0)
             (= slot-id 2)
             (= slot-id 4)
             (= slot-id 8)
             (> slot-id 9))
         #f)
        ((= (get-value (get-top-card slot-id)) BASE-VAL)
         (let ((top-card (get-top-card slot-id)))
           (remove-card slot-id)
           (cond ((empty-slot? 4)
                  (move-n-cards! slot-id 4 (list top-card)))
                 ((empty-slot? 8)
                  (move-n-cards! slot-id 8 (list top-card)))
                 (#t
                  (move-n-cards! slot-id 10 (list top-card))))
           (add-to-score! 1))
	 #t)
	((move-to-foundation slot-id '(2 4 8 10)) 
	 #t)
        (#t #f)))

(define (game-continuable)
  (give-status-message)
  (and (not (game-won))
       (get-hint)))

(define (game-won)
  (and (= (length (get-cards 2)) 13)
       (= (length (get-cards 4)) 13)
       (= (length (get-cards 8)) 13)
       (= (length (get-cards 10)) 13)))

(define (dealable?)
  (if (not (empty-slot? 0))
      (list 0 (_"Deal a card"))
      #f))

(define (check-a-foundation card f-slot)
  (cond ((> f-slot 10)
	 #f)
	((or (= f-slot 6)
	     (empty-slot? f-slot))
	 (check-a-foundation card (+ 2 f-slot)))
	((eq? (get-suit card)
	      (get-suit (get-top-card f-slot)))
	 (if (or (= (get-value card)
		    (+ 1 (get-value (get-top-card f-slot))))
		 (and (= (get-value card) ace)
		      (= (get-value (get-top-card f-slot)) king)))
	     (get-top-card f-slot)
	     #f))
	(#t (check-a-foundation card (+ 2 f-slot)))))

(define (to-foundations? slot-id)
  (cond ((= slot-id 10)
	 #f)
	((or (empty-slot? slot-id)
	     (= slot-id 2)
	     (= slot-id 4)
	     (= slot-id 8))
	 (to-foundations? (+ 1 slot-id)))
	((= BASE-VAL (get-value (get-top-card slot-id)))
	 (list 2 
	       (get-name (get-top-card slot-id)) 
	       (_"an empty foundation pile")))
	((check-a-foundation (get-top-card slot-id) 2)
	 (list 1
	       (get-name (get-top-card slot-id))
	       (get-name (check-a-foundation (get-top-card slot-id) 2))))
	(#t (to-foundations? (+ 1 slot-id)))))

(define (waste-to-tableau? slot-id)
  (cond ((or (= slot-id 10)
	     (empty-slot? 1))
	 #f)
	((or (= slot-id 4)
	     (= slot-id 8))
	 (waste-to-tableau? (+ 1 slot-id)))
	((empty-slot? slot-id)
	 (list 2 (get-name (get-top-card 1)) (_"an empty tableau pile")))
	((or (and (= (get-value (get-top-card 1)) king)
		  (= (get-value (get-top-card slot-id)) ace))
	     (= (+ 1 (get-value (get-top-card 1)))
		(get-value (get-top-card slot-id))))
	 (list 1 
	       (get-name (get-top-card 1))
	       (get-name (get-top-card slot-id))))
	(#t (waste-to-tableau? (+ 1 slot-id)))))

(define (check-tslot slot1 card-list slot2)
  (cond ((or (= (length card-list) 0)
	     (check-a-foundation (car card-list) 2))
	 #t)
	((= slot2 10)
	 #f)
	((or (empty-slot? slot2)
	     (= slot2 4)
	     (= slot2 8))
	 (check-tslot slot1 card-list (+ 1 slot2)))
	((and (or (and (= (get-value (car card-list)) king)
		       (= (get-value (get-top-card slot2)) ace))
		  (= (+ 1 (get-value (car card-list)))
		     (get-value (get-top-card slot2))))
	      (check-tslot slot1 (cdr card-list) 3))
	 (list 1 
	       (get-name (get-top-card slot1)) 
	       (get-name (get-top-card slot2))))
	(#t (check-tslot slot1 card-list (+ 1 slot2)))))

(define (tableau-movement? slot-id)
  (cond ((= slot-id 10)
	 #f)
	((or (empty-slot? slot-id)
	     (= slot-id 4)
	     (= slot-id 8))
	 (tableau-movement? (+ 1 slot-id)))
	((check-tslot slot-id (get-cards slot-id) 3)
	 (check-tslot slot-id (get-cards slot-id) 3))
	(#t (tableau-movement? (+ 1 slot-id)))))

(define (get-hint)
  (or (to-foundations? 1)
      (waste-to-tableau? 3)
      (tableau-movement? 3)
      (dealable?)))

(define (get-options) 
  #f)

(define (apply-options options) 
  #f)

(define (timeout) 
  #f)

(set-features droppable-feature)

(set-lambda new-game button-pressed button-released button-clicked
button-double-clicked game-continuable game-won get-hint get-options
apply-options timeout droppable?)
