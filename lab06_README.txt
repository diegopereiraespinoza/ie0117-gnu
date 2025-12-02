
Lab06
Ordenamiento de estructuras de datos
Function pointers para el ordenamiento

1. Descripción del contenido

1.1. Header files con declaraciones

Ruta: ./include/
umath.h     -> Declara funciones para operaciones con random
Person.h    -> Declara funciones para estructura de Personas
usort.h     -> Declara funciones para ordenamiento de estructuras

1.2. Source files con definiciones

Ruta: ./src/
umath.h     -> Define funciones para operaciones con random
Person.h    -> Define funciones para estructura de Personas
usort.h     -> Define funciones para ordenamiento de estructuras

1.3. Main file con llamadas a las funciones

Ruta: ./
lab06e1fp_main.c        -> Usado como entrada del programa
lab06e1fp_aio.c         -> All In One es ignorado por make

2. Exec files para compilar

Ruta: ./
lab06e1fp_run_make      -> Compila con "make all" y ejecuta ./bin/
Makefile                -> Usado por "make all" para compilar

3. Exec files para ejecutar

Ruta: ./
lab06e1fp_run_make      -> Compila con "make all" y ejecuta ./bin/
Makefile                -> Usado por "make run" para ejecutar
lab06e1fp_run_gcc       -> Ejecuta ./bin/lab06e1fp_main sin usar make
lab06e1fp_run_aio       -> Ejecuta ./bin/lab06e1fp_aio sin usar make
lab06e1fp_run_valgrind  -> Análisis de memoria con valgrind

