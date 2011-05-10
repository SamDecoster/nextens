;; test token to words mapping

;;; there is an unsolved problem with �, �, �, �, and �
;;; see http://www.cstr.ed.ac.uk/projects/festival/mailing_lists/festival-talk/msg00954.html

;;  failures DEB�CLE

(define (nl::test_token)
  (nl::test_suite 
   "TOKEN"
   (list
    '(nl::test_empty)
    '(nl::test_dash)
    '(nl::test_slash) 
    '(nl::test_spell)
    '(nl::test_diacritics)
    '(nl::test_dotted_abbrevs)
    '(nl::test_email)
    '(nl::test_url)
    '(nl::test_acronyms)
    '(nl::test_numbers)
    '(nl::test_quote)
    '(nl::test_garbage)
    )))


(define (nl::test_empty)
  (nl::test_block 
   "EMPTY" 
   (list
    '(nl::test_synth_text 
      "EMPTY STRING"
      "")
   )))


(define (nl::test_dash)
  (nl::test_block 
   "DASH" 
   (list
    '(nl::test_synth_text 
      "SINGLE DASH"
      "twee-derde"
      (lambda (utt) 
	(string-equal (item.name (utt.relation.first utt 'Word)) "twee")))
    '(nl::test_synth_text 
      "MULTIPLE DASH"
      "ons-kent-ons-gevoel")
    '(nl::test_synth_text 
      "HYPHEN DASH"
      "weer- kaart")
    '(nl::test_synth_text 
      "DASH IN ENUMERATION"
      "spier-, pees-, en zenuwklachten")
   )))

(define (nl::test_slash)
  (nl::test_block 
   "SLASH" 
   (list
    '(nl::test_synth_text 
      "SINGLE SLASH"
      "/ en/of")
   ))
  )


(define (nl::test_spell)
  (nl::test_block 
   "SPELL"
   (list
    '(nl::test_synth_text 
      "LOWER CASE LETTERS"
      "a b c d e f g h i j k l m n o p q r s t u v w x y z")
    '(nl::test_synth_text 
      "UPPER CASE LETTERS"
      "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z")
    '(nl::test_synth_text 
      "LOWER CASE DIACRITICS"
      "� � � � � � � � � � � � � � � � � � � �")
    '(nl::test_synth_text 
      "UPPER CASE DIACRITICS"
      "� � � � � � � � � � � � � � � � � � � �")
    '(nl::test_synth_text 
      "DIGITS"
      "0 1 2 3 4 5 6 7 8 9")
    '(nl::test_synth_text 
      "NON-ALPHANUMERIC"
      "~ ` ! @ # $ % ^ & * ( ) - _ + = \\ | { [ ] } ; : ' \" < , > . / ?")
   )))

   
(define (nl::test_diacritics)
  (nl::test_block 
   "DIACRITICS"
   (list
    '(nl::test_synth_text 
      "A DIACRITICS"
      "M�laga, voil�, Aufkl�rung, deb�cle, M�LAGA, VOIL�, AUFKL�RUNG, DEB�CLE")
    '(nl::test_synth_text 
      "E DIACRITICS"
      "derri�re, caf�, effici�nt, enqu�te, DERRI�RE, CAF�, EFFICI�NT, ENQU�TE")
    '(nl::test_synth_text 
      "I DIACRITICS"
      "�n, �n, gere�ncarneerd, ma�tresse, �N, �N, GERE�NCARNEERD, MA�TRESSE")
    '(nl::test_synth_text 
      "O DIACRITICS"
      "overk�men, co�peratie, comp�te, OVERK�MEN, CO�PERATIE, COMP�TE")
    '(nl::test_synth_text 
      "U DIACRITICS"
      "vacu�m, cro�ton, VACU�M, CRO�TON")
   )))


(define (nl::test_url)
  (nl::test_block 
   "URL"
   (list
    '(nl::test_synth_text 
      "HTTP"
      "http://nextens.uvt.nl")
    '(nl::test_synth_text 
      "HTTP TILDE"
      "http://ilk.uvt.nl/~marsi/index.html")
    '(nl::test_synth_text 
      "FTP"
      "ftp://ilk.uvt.nl/pub/")
   )))


(define (nl::test_email)
  (nl::test_block 
   "EMAIL"
   (list
    '(nl::test_synth_text 
      "EMAIL 1"
      "e.c.marsi@uvt.nl")
    '(nl::test_synth_text 
      "EMAIL 2"
      "J.Kerkhoff@let.ru.nl")
   )))

(define (nl::test_dotted_abbrevs)
  (nl::test_block 
   "DOTTED ABBREVIATIONS"
   (list
    '(nl::test_synth_text 
      "SINGEL DOT"
      "prof. dr. ir. Akkermans")
    '(nl::test_synth_text 
      "MULTI DOT"
      "t.z.t. n.a.v. e.e.a.")
    '(nl::test_synth_text 
      "UNKNOWN"
      "x.y.z.")
   )))


(define (nl::test_acronyms)
  (nl::test_block 
   "ACRONYMS"
   (list
    '(nl::test_synth_text 
      "SPELLED ACRONYM"
      "uvt, UVT")
    '(nl::test_synth_text 
      "NON-SPELLED ACRONYM"
      "nac, NAC")
   )))

(define (nl::test_numbers)
  (nl::test_block 
   "NUMBERS"
   (list
    '(nl::test_synth_text 
      "DIGITS"
      "0, 1, 2, 3, 4, 5, 6, 7, 8, 9")
    '(nl::test_synth_text 
      "9 < INTEGERS < 100"
      "10, 11, 12, 20, 21, 99")
    '(nl::test_synth_text 
      "99 < INTEGERS < 1000"
      "100, 101, 110, 111, 121, 200, 201, 230, 233")
    '(nl::test_synth_text 
      "1000 <= INTEGERS < 1.000.000"
      "1000, 1001, 1010, 1099, 9099, 10.000, 22.222")
    '(nl::test_synth_text 
      "1.000.000 < INTEGERS < 1.000.000.000.000"
      "1.000.000, 1.234.567, 1.000.000.000, 999.999.999.999 ")
    '(nl::test_synth_text 
      "INTEGERS >  999.999.999.999"
      "1.234.456.890.123")
    '(nl::test_synth_text 
      "FLOATS"
      "0,1, 0,01, 0,10, 0,99, 0,001, 0,101, 123456,123456")
    '(nl::test_synth_text 
      "NEGATIVES"
      "-1, -10, -1000, -0,1")
   )))


(define (nl::test_quote)
  (nl::test_block 
   "SINGLE QUOTE PLUS S ('s)"
   (list
    '(nl::test_synth_text 
      "GENITIVE 'S"
      "'s ochtends, 's morgens")
    '(nl::test_synth_text 
      "PLURAL 's"
      "camera's, jan's")
   )))


(define (nl::test_garbage)
  (nl::test_block 
   "GARBAGE" 
   (list
    '(nl::test_synth_text 
      "NON-DUTCH ISO CHARS"
      "�, �ndijvie, �, �aak")
   )))


(provide 'net_nl_test_token)