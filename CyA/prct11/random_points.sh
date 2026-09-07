#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Uso: $0 <cantidad> <archivo_salida>"
    exit 1
fi

N=$1
OUT=$2

echo $N > "$OUT"

for i in $(seq 1 $N); do
    X=$((RANDOM % 201 - 100))   # -100 a 100
    Y=$((RANDOM % 201 - 100))
    echo "$X $Y" >> "$OUT"
done

echo "Puntos generados en $OUT"
