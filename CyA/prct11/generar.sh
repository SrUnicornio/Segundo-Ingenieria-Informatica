#!/bin/bash

# -----------------------------------------
# Script automático para EMST
# Uso:
#   ./generar.sh <ruta/al/input.txt>
# -----------------------------------------

if [ $# -ne 1 ]; then
    echo "Uso: $0 <archivo_input>"
    exit 1
fi

INPUT="$1"

# Comprobar existencia
if [ ! -f "$INPUT" ]; then
    echo "Error: el archivo '$INPUT' no existe."
    exit 1
fi

# Nombre base y extensión
FILENAME=$(basename "$INPUT")          # perro.txt
BASENAME="${FILENAME%.*}"              # perro
EXT="${FILENAME##*.}"                  # txt

# REGLA DE RENOMBRADO:
# si BASENAME empieza por "input", lo reemplazamos por "output"
if [[ "$BASENAME" == input* ]]; then
    OUTNAME="output${BASENAME:5}"
else
    OUTNAME="$BASENAME"
fi

# Directorios destino
OUT_TXT="Ejemplos/outputs/${OUTNAME}.${EXT}"
OUT_DOT="Ejemplos/dot/${OUTNAME}.dot"
OUT_PDF="Ejemplos/pdf/${OUTNAME}.pdf"

# Crear carpetas si no existen
mkdir -p Ejemplos/outputs Ejemplos/dot Ejemplos/pdf

# Ejecutar el programa principal
./build/p11_AlgortimosVoraces "$INPUT" "$OUT_TXT" -d "$OUT_DOT"

# Comprobar que .dot existe
if [ ! -f "$OUT_DOT" ]; then
    echo "Error: No se pudo generar el archivo DOT."
    exit 1
fi

# Generar PDF
neato "$OUT_DOT" -Tpdf -o "$OUT_PDF"

echo "---------------------------------------"
echo "Proceso completado correctamente"
echo " Input:   $INPUT"
echo " Output:  $OUT_TXT"
echo " DOT:     $OUT_DOT"
echo " PDF:     $OUT_PDF"
echo "---------------------------------------"
