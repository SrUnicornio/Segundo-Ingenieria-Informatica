# Práctica 11 — Algoritmos Voraces (EMST)

## Autor

* Álvaro Pérez Ramos
* alu0101574042@ull.edu.es

## Euclidean Minimum Spanning Tree (Árbol Generador Mínimo Euclidiano)

Este proyecto implementa un algoritmo voraz basado en **Kruskal** para calcular
el **Árbol Generador Mínimo Euclidiano (EMST)** de un conjunto de puntos en 2D.

El programa:
- Lee un conjunto de puntos desde un fichero.
- Calcula el EMST usando distancias euclidianas.
- Genera un fichero de salida con el conjunto de aristas.
- (Opcional) Genera un fichero `.dot` para visualizar el árbol.
- Incluye scripts auxiliares.

---

## Estructura del proyecto

```
.
├── CMakeLists.txt
├── src/
├── lib/
├── build/
├── Ejemplos/
│   ├── inputs/
│   ├── outputs/
│   ├── dot/
│   ├── pdf/
├── generar.sh
├── random_points.sh
└── README.md
```

---

## Compilación

```bash
mkdir -p build
cd build
cmake ..
make
```

El ejecutable queda en:

```bash
build/p11_AlgortimosVoraces
```

---

## Uso del programa principal

```bash
./p11_AlgortimosVoraces <input.txt> <output.txt> [-d archivo.dot]
```

Ejemplo:

```bash
./p11_AlgortimosVoraces puntos.txt arbol.txt
```

Con salida DOT:

```bash
./p11_AlgortimosVoraces puntos.txt arbol.txt -d arbol.dot
```

---

## Script automatizado: [generar.sh](./generar.sh)

Este script procesa todo automáticamente:

```bash
./generar.sh Ejemplos/inputs/perro.txt
```

Produce:

```
Ejemplos/outputs/<nombre>.txt
Ejemplos/dot/<nombre>.dot
Ejemplos/pdf/<nombre>.pdf
```

### Reglas de renombrado

Si el fichero empieza por **input**, se cambia por **output**:

| Input          | Output         |
|----------------|----------------|
| input.txt      | output.txt     |
| input_3.txt    | output_3.txt   |
| input3.txt     | output3.txt    |
| leon.txt       | leon.txt       |
| perro.txt      | perro.txt      |

---

## Generador de puntos aleatorios — [random_points.sh](./random_points.sh)

Permite generar ficheros válidos de prueba automáticamente.

### Uso

```bash
./random_points.sh <cantidad> <archivo_salida>
```

Ejemplo:

```bash
./random_points.sh 50 Ejemplos/inputs/random50.txt
```

Genera un fichero con N puntos aleatorios dentro del rango [-100, 100].

---

## Formato del fichero de entrada

El fichero debe tener:

```
N
x1 y1
x2 y2
...
```

---

## Visualización del árbol (DOT → PDF)

Si quieres generar un PDF manualmente:

```bash
neato archivo.dot -Tpdf -o salida.pdf
```

El script `generar.sh` ya hace esto automáticamente.

---

## Errores detectados por el programa

El programa valida:

- Número incorrecto de puntos.
- Faltan líneas.
- Líneas corruptas.
- Hay más puntos de los declarados.

Se muestra un mensaje detallado indicando la línea del fallo.

---
