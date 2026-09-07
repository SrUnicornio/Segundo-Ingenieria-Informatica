# Práctica 4: Code analyzer

Autores:

- Álvaro Pérez Ramos (alu0101574042@ull.edu.es)

## Descripción

Esta práctica implementa un analizador de código en C++ (C++17) que examina
ficheros fuente y extrae información estructural: declaraciones de variables,
bucles, condicionales, comentarios y la presencia de la función `main`.

El analizador trabaja de forma línea a línea usando expresiones regulares y
algunos contadores simples para manejar construcciones más complejas (por
ejemplo, separar declaradores que contienen plantillas STL).

Se ha diseñado con modularidad: funciones auxiliares separadas para el
análisis de variables, bucles, condicionales y comentarios.

## Cómo se ha realizado

- Lenguaje: C++17
- Sistema de compilación: CMake
- Librería de tests: GoogleTest (integrada mediante FetchContent en CMake)

Puntos clave de la implementación:

- Normalización de inicializadores: el analizador acepta inicializadores con
  `=` y con llaves `{}` (p. ej. `int a = 3;` y `int b{0};`) y almacena un valor
  normalizado internamente.
- Soporte para declaraciones múltiples en una línea mediante una función
  `MatchMultiple` que separa declaradores respetando niveles de anidamiento
  (para tratar tipos plantillados como `std::vector<int>`).
- Detección de comentarios: el analizador extrae comentarios de una sola línea
  (`//`) y comentarios multilínea (`/* ... */`), ignorando las secuencias que
  aparecen dentro de literales de cadena.
- Salida: hay una clase `CodeStructure` y se sobrecarga `operator<<` para
  imprimir un reporte con PROGRAM, DESCRIPTION, VARIABLES, STATEMENTS,
  CONDITIONALS, MAIN y COMMENTS.

## Estructura del proyecto (resumen)

- `src/` - Implementación fuente del analizador (módulos para variables,
  comentarios, detección de bucles/condicionales, etc.).
- `lib/` - Cabeceras públicas para las clases usadas por el analizador.
- `tests/` - Tests unitarios (GoogleTest) que validan los casos principales y
  algunos bordes.

## Compilar y ejecutar

Se recomienda compilar en un directorio `build` fuera de `src`.

```bash
# Desde la carpeta prct04/original/
mkdir -p build && cd build
cmake ..
cmake --build .
```

Tras la compilación, el ejecutable principal (analizador) se encuentra en
`bin/` (o según la configuración de CMake). Para ejecutar el analizador sobre
un fichero de ejemplo:

```bash
./p04_code_analyzer ../src/function_main.cc ../out.txt
```

## Ejecutar tests unitarios

Los tests están integrados con CMake y GoogleTest. Para ejecutarlos:

```bash
# Desde prct04/original/build
ctest --verbose
# o ejecutar directamente el binario de tests si existe:
# ./bin/run_tests
```

Si FetchContent descarga GoogleTest en la configuración de CMake, la primera
configuración tardará un poco en compilar las dependencias.
