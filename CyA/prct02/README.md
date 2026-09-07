# Practica 02: Strings
Repositorio de la practica 2 de la asignatura de Computacion y Algoritmos (CyA) del grado de Ingenieria Informatica en la Universidad de La Laguna (ULL).
## Autor
- Álvaro Pérez Ramos

## ¿Qué se pide?
1. El objetivo de la práctica es trabajar conceptos básicos sobre símbolos, alfabetos, cadenas y lenguajes, a través del diseño de las clases correspondientes en C++. Además de repasar estos conceptos teóricos, se propone que el alumnado utilice este ejercicio para poner en práctica aspectos generales relacionados con el desarrollo de programas en C++

2. Teniendo en cuenta las propiedades de las cadenas y el comportamiento de las mismas, se propone desarrollar un programa en C++ que dado un fichero de entrada con la especificación de cadenas junto con sus respectivos alfabetos, realice un conjunto de comprobaciones básicas sobre ellas. Para ser coherentes con las definiciones anteriores, cabe destacar que en aquellas operaciones sobre cadenas en las que el resultado obtenido sea un conjunto de cadenas, deberíamos manejar dicho resultado como un lenguaje. El programa recibirá por línea de comandos el nombre del fichero de entrada, el nombre del fichero de salida y un código de operación:

    >`./ p02_strings filein.txt fileout.txt opcode`

3. Como mínimo se ha de crear 5 opcodes,
   ```txt
   1. Alfabeto: Mostrará los alfabetos correspondiente a cada línea leida por el fichero
   2. Longitud: nos dirá la longitud de la cadena correspondiente a cada línea leida por el fichero
   3. Inversa: calculará la inversa de la cadena correspondiente a cada línea leida por el fichero
   4. Prefijo: Mostrará los prefijos asociados a una cadena.
   5. Sufijo: Mostrará los sufijos asociados a una cadena.
   ```

## Compilación
 Se ha usado `cmake` para la compilacion de esta práctica
```bash
mkdir -p build
cd build
cmake ..
make
```

# Ejecución
>`./build/p02_strings filein.txt fileout.txt opcode`
