; AisleRiot - bakers_game.scm
; Copyright (C) 2001, 2003 Rosanna Yuen <zana@webwynk.net>
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

(def-save-var free-reserves 0)

(define (new-game)
  (initialize-playing-area)
  (set-ace-low)
  (make-standard-deck)
  (shuffle-deck)

  (add-normal-slot DECK)
  (add-carriage-return-slot)
  (add-normal-slot '())
  (add-carriage-return-slot)
  (add-normal-slot '())
  (add-carriage-return-slot)
  (add-normal-slot '())


  (set! VERTPOS 0)

  (add-blank-slot)
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())
  (add-normal-slot '())

  (add-carriage-return-slot)

  (add-blank-slot)
  (add-blank-slot)

  (add-extended-slot '() down)
  (add-extended-slot '() down)
  (add-extended-slot '() down)
  (add-extended-slot '() down)
  (add-extended-slot '() down)
  (add-extended-slot '() down)
  (add-extended-slot '() down)
  (add-extended-slot '() down)

  (deal-cards-face-up 0 '(12 13 14 15 16 17 18 19 12 13 14 15 16 17 18
			     19 12 13 14 15 16 17 18 19 12 13 14 15 16
			     17 18 19 12 13 14 15 16 17 18 19 12 13 14 
			     15 16 17 18 19 4 5 6 7))

  (set! free-reserves 4)

  (list 10 4))

(define (button-pressed slot-id card-list)
  (and (not (empty-slot? slot-id))
       (or (= (length card-list) 1)
	   (and (> slot-id 11)
		(< (length card-list) (+ 2 free-reserves))
		(check-same-suit-list card-list)
		(check-straight-descending-list card-list)))))

(define (droppable? start-slot card-list end-slot)
  (cond ((= start-slot end-slot)
	 #f)
	((> end-slot 11)
	 (and (or (and (empty-slot? end-slot)
		       (= (get-value (car (reverse card-list)))
			  king))
		  (and (not (empty-slot? end-slot))
		       (= (get-suit (get-top-card end-slot))
			  (get-suit (car card-list)))
		       (= (get-value (get-top-card end-slot))
			  (+ 1 (get-value (car (reverse card-list)))))))))
	((and (= (length card-list) 1)
	      (empty-slot? end-slot)
	      (> end-slot 3)
	      (< end-slot 12))
	 #t)
	((and (= (length card-list) 1)
	      (< end-slot 4))
	 (and (or (and (empty-slot? end-slot)
		       (= (get-value (car card-list)) ace))
		  (and (not (empty-slot? end-slot))
		       (= (get-suit (get-top-card end-slot))
			  (get-suit (car card-list)))
		       (= (+ 1 (get-value (get-top-card end-slot)))
			  (get-value (car card-list)))))))
	(#t #f)))

(define (button-released start-slot card-list end-slot)
  (and (droppable? start-slot card-list end-slot)
       (move-n-cards! start-slot end-slot card-list)
       (or (< start-slot 4)
           (> start-slot 11)
           (set! free-reserves (+ free-reserves 1)))
       (or (< end-slot 4)
           (> end-slot 11)
           (set! free-reserves (- free-reserves 1)))
       (or (> start-slot 3)
           (add-to-score! -1))
       (or (> end-slot 3)
           (add-to-score! 1))))

(define (button-clicked slot-id)
  #f)

(define (move-to-empty-foundation slot f-slot)
  (cond ((= f-slot 4)
	 #f)
	((empty-slot? f-slot)
	 (deal-cards slot (list f-slot)))
	(#t (move-to-empty-foundation slot (+ 1 f-slot)))))

(define (move-to-foundation slot f-slot)
  (cond ((= f-slot 4)
	 #f)
	((and (not (empty-slot? f-slot))
	      (= (get-suit (get-top-card slot))
		 (get-suit (get-top-card f-slot))))
	 (and (= (get-value (get-top-card slot))
		 (+ 1 (get-value (get-top-card f-slot))))
	      (deal-cards slot (list f-slot))))
	(#t (move-to-foundation slot (+ 1 f-slot)))))

(define (autoplay-foundations)
  (define (autoplay-foundations-tail)
    (if (or-map button-double-clicked '(4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19))
        (delayed-call autoplay-foundations-tail)
        #t))
  (if (or-map button-double-clicked '(4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19))
      (autoplay-foundations-tail)
      #f))

(define (button-double-clicked slot-id)
  (if (> slot-id 3)
      (and (not (empty-slot? slot-id))
           (or (and (= (get-value (get-top-card slot-id))
		       ace)
		    (move-to-empty-foundation slot-id 0))
	       (move-to-foundation slot-id 0))
           (add-to-score! 1)
           (or (> slot-id 11)
	       (set! free-reserves (+ 1 free-reserves))))
       (autoplay-foundations)))

(define (game-continuable)
  (and (not (game-won))
       (get-hint)))

(define (game-won)
  (and (= (length (get-cards 0)) 13)
       (= (length (get-cards 1)) 13)
       (= (length (get-cards 2)) 13)
       (= (length (get-cards 3)) 13)))

(define (check-to-foundations? slot f-slot)
  (cond ((= slot 20)
	 #f)
	((or (empty-slot? slot)
	     (= f-slot 4))
	 (check-to-foundations? (+ 1 slot) 0))
	((= (get-value (get-top-card slot)) ace)
	 (list 2 (get-name (get-top-card slot)) (_"an empty foundation")))
	((and (not (empty-slot? f-slot))
	      (= (get-suit (get-top-card slot))
		 (get-suit (get-top-card f-slot)))
	      (= (get-value (get-top-card slot))
		 (+ 1 (get-value (get-top-card f-slot)))))
	 (list 1 (get-name (get-top-card slot)) (get-name (get-top-card f-slot))))
	(#t (check-to-foundations? slot (+ 1 f-slot)))))

(define (check-for-king card-list iter slot)
  (cond ((= (length card-list) 0)
	 #f)
	((and (= (length card-list) 1)
	      (> slot 11))
	 #f)
	((= (get-value (car card-list)) king)
	 (get-name (car card-list)))
	((= iter 0)
	 #f)
	((and (> (length card-list)1)
	      (= (get-suit (car card-list))
		 (get-suit (cadr card-list)))
	      (= (+ 1 (get-value (car card-list)))
		 (get-value (cadr card-list))))
	 (check-for-king (cdr card-list) (- iter 1) slot))
	(#t #f)))

(define (check-for-spec-card card-list iter value)
  (cond ((= (length card-list) 0)
	 #f)
	((= (get-value (car card-list)) value)
	 #t)
	((= iter 0)
	 #f)
	((and (> (length card-list) 1)
	      (= (get-suit (car card-list))
		 (get-suit (cadr card-list)))
	      (= (+ 1 (get-value (car card-list)))
		 (get-value (cadr card-list))))
	 (check-for-spec-card (cdr card-list) (- iter 1) value))
	(#t #f)))

(define (check-to-tableau? slot t-slot)
  (cond ((= slot 20)
	 #f)
	((or (empty-slot? slot)
	     (= t-slot 20))
	 (check-to-tableau? (+ 1 slot) 12))
	((and (not (= slot t-slot))
	      (empty-slot? t-slot)
	      (check-for-king (get-cards slot) free-reserves slot))
	 (list 2 
	       (check-for-king (get-cards slot) free-reserves slot) 
	       (_"an empty tableau")))
	((and (not (= slot t-slot))
	      (not (empty-slot? t-slot))
	      (= (get-suit (get-top-card slot))
		 (get-suit (get-top-card t-slot)))
	      (check-for-spec-card (get-cards slot) 
				   free-reserves 
				   (- (get-value (get-top-card t-slot)) 1)))
	 (list 1 
	       (get-name (make-card (- (get-value (get-top-card t-slot)) 1)
				    (get-suit (get-top-card t-slot))))
	       (get-name (get-top-card t-slot))))
	(#t (check-to-tableau? slot (+ 1 t-slot)))))

(define (check-for-empty-reserve)
  (and (> free-reserves 0)
       (list 0 (_"Move something on to an empty reserve"))))

(define (get-hint)
  (or (check-to-foundations? 4 0)
      (check-to-tableau? 4 12)
      (check-for-empty-reserve)))

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
