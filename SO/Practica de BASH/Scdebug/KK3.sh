#!/bin/bash

sleep 100

if [[ "$1" == "-h" || "$1" == "--help" ]]; then
  echo "AYUDA"
  exit 0
elif [[ "$1" == "-r" || "$1" == "--rm" ]]; then
  echo "ELIMINAR"
  exit 0
else
  echo "HOLA"
  exit 0
fi

exit 0
