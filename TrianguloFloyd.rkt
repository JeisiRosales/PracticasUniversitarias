#lang racket

;; ============================================
;;   Triángulo de Floyd en Racket
;;   Autor: Jeisi Rosales, C.I: 29.668.571
;;   Materia: Lenguajes de Programación
;; ============================================

;; (fila k inicio) construye una lista con k números consecutivos
(define (fila k inicio)
  (if (= k 0)
      '()
      (cons inicio (fila (- k 1) (+ inicio 1)))))

;; (floyd n) genera una lista de listas, cada una una fila del triángulo
(define (floyd n)
  (define (aux fila-actual siguiente)
    (if (> fila-actual n)
        '()
        (let* ([linea (fila fila-actual siguiente)]
               [nuevo-siguiente (+ siguiente (length linea))])
          (cons linea (aux (+ fila-actual 1) nuevo-siguiente)))))
  (aux 1 1))

;; Mostrar en pantalla
(define (imprimir-floyd n)
  (for-each
   (λ (fila)
     (for-each (λ (x) (display x) (display " ")) fila)
     (newline))
   (floyd n)))

;; Función main
(define (main)
  (display "Ingrese número de filas (entero >= 0): ")
  (flush-output)
  (let* ([entrada (read-line)] 
         [num-str (string-trim entrada)]
         [num-converted (string->number num-str)]
         
         ; Si la conversión falla, asumimos 0 o dejamos que el programa falle 
         [n (if num-converted 
                (inexact->exact (floor num-converted))
                0)]) ; Si la conversión falla (#f), usa 0 como fallback
    (cond
      [(< n 0) 
       (printf "El número de filas no puede ser negativo.\n")]
      [else
       (printf "Triángulo de Floyd con ~a filas:\n\n" n)
       (imprimir-floyd n)])))

(main)