
Laboratorio 5

1. Descripción del contenido

1.1. Header files con declaraciones

Ruta: ./include/
SNode.h  -> Declara la estructura para los datos de un nodo
slist.h  -> Declara funciones para listas enlazadas simples
sstack.h -> Declara funciones para simular un pila (stack)
umath.h  -> Declara funciones para números con aleatorios

1.2. Source files con definiciones

Ruta: ./src/
SNode.c  -> Define la estructura para los datos de un nodo
slist.c  -> Define funciones para listas enlazadas simples
sstack.c -> Define funciones para simular un pila (stack)
umath.c  -> Define funciones para números con aleatorios

1.3. Main file con llamadas a las funciones

Ruta: ./
lab05e1singly.c     -> Usado como entrada del programa
lab05e1singly_aio.c -> All In One es ignorado por make

2. Exec files para compilar

Ruta: ./
lab05e1singly_run -> Compila con "make all" y ejecuta ./bin/
Makefile          -> Usado por "make all" para compilar

3. Exec files para ejecutar

Ruta: ./
lab05e1singly_run -> Compila con "make all" y ejecuta ./bin/
Makefile          -> Usado por "make run" para ejecutar
lab05e1singly_gcc -> Análisis de memoria con valgrind

