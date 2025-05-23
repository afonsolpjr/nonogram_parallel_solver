#!/bin/bash

# criar string com todos arquivos .cpp  do diretorio atual
FILES=$(find . -name "*.cpp")

g++ -o nonogram $FILES
echo "Compilação concluída."