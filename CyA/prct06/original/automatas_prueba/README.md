# Pruebas de Autómatas (DFAs y NFAs)

Este repositorio contiene ficheros de prueba para el programa de simulación de autómatas finitos. Se incluyen tanto DFAs como NFAs, así como casos con errores para probar la validación del programa.

## Organización de los ficheros

Los ficheros se encuentran en la carpeta `test/`:

- `dfa_correct_X.fa` : DFAs correctos (X = 1..10)  
- `dfa_error_X.fa` : DFAs con errores (X = 1..10)  
- `nfa_correct_X.fa` : NFAs correctos (X = 1..7)  
- `nfa_error_X.fa` : NFAs con errores (X = 1..5)  

Cada archivo sigue el formato `.fa` esperado por el programa.

## Descripción de los DFAs correctos

1. `dfa_correct_1.fa` – Cadenas sobre `{a, b}` con número de “a” par.
2. `dfa_correct_2.fa` – Cadenas sobre `{a, b}` con longitud impar.  
3. `dfa_correct_3.fa` – Cadenas sobre `{a, b}` con número de “a” par o longitud impar.  
4. `dfa_correct_4.fa` – Cadenas sobre `{a, b}` con número de “a” par y longitud impar.
5. `dfa_correct_5.fa` – Cadenas sobre `{0, 1}` con longitud entre 2 y 5.  
6. `dfa_correct_6.fa` – Cadenas sobre `{0, 1}` con al menos dos ceros consecutivos.  
7. `dfa_correct_7.fa` – Cadenas sobre `{0, 1}` con como máximo dos ceros.  
8. `dfa_correct_8.fa` – Cadenas sobre `{0, 1}` con longitud múltiplo de 3.  
9. `dfa_correct_9.fa` – Cadenas sobre `{0, 1}` con longitud no múltiplo de 3.  
10. `dfa_correct_10.fa` – Cadenas sobre `{x, y, z}` sin dos símbolos iguales 

## Descripción de los DFAs con errores

1. `dfa_error_1.fa` – Estado inicial inexistente.  
2. `dfa_error_2.fa` – Estado final inexistente.  
3. `dfa_error_3.fa` – Símbolo no pertenece al alfabeto.  
4. `dfa_error_4.fa` – Estado destino vacío.  
5. `dfa_error_5.fa` – Más transiciones de las declaradas.
6. `dfa_error_6.fas` – Erorr en la extención del archivo
7. `dfa_error_7.fa` – Menos transiciones de los declarados
8. `dfa_error_8.fa` – Menos estados de los declarados
9. `dfa_error_9.fa` – Más estados de los declarados
10. `dfa_error_10.fa` – Más de un estado inicial.
11. `dfa_error_11.fa` – Apuntar a estados inexistentes

## Descripción de los NFAs correctos

1. `nfa_correct_1.fa` – Cadenas sobre `{a, b}` que empiezan por “a”.  
2. `nfa_correct_2.fa` – Cadenas sobre `{a, b}` que terminan en “bb”.  
3. `nfa_correct_3.fa` – Cadenas sobre `{a, b}` que empiezan por “a” o terminan en “bb”.  
4. `nfa_correct_4.fa` – Cadenas sobre `{a, b}` que empiezan por “a” y terminan en “bb”.  
5. `nfa_correct_5.fa` – Cadenas sobre `{a, b}` con número de “a” par o longitud impar.  
6. `nfa_correct_6.fa` – Cadenas sobre `{x, y, z}` con al menos dos símbolos iguales consecutivos.  
7. `nfa_correct_7.fa` – Cadenas sobre `{x, y, z}` que empiezan y terminan con el mismo 

## Descripción de los NFAs con errores

1. `nfa_error_1.fa` – Estado inicial inexistente.  
2. `nfa_error_2.fa` – Estado final inexistente.  
3. `nfa_error_3.fa` – Símbolo no pertenece al alfabeto.  
4. `nfa_error_4.fa` – Estado destino vacío.  
5. `nfa_error_5.fa` – Más transiciones de las declaradas.
6. `nfa_error_6.fas` – Erorr en la extención del archivo
7. `nfa_error_7.fa` – Menos transiciones de los declarados
8. `nfa_error_8.fa` – Menos estados de los declarados
9. `nfa_error_9.fa` – Más estados de los declarados
10. `nfa_error_10.fa` – Más de un estado inicial.
11. `nfa_error_11.fa` – Apuntar a estados inexistentes

