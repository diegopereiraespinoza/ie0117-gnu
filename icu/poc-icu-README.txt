
Proyecto Prueba de Concepto para i18n
Localización (L10n) e Internacionalización (i18n)

1. Descripción del contenido

1.1. Header files con declaraciones

Ruta: ./include/
upath.h  -> Declara funciones para resolver rutas absolutas
ustrc.h  -> Declara funciones para procesar texto UTF-8/UNICODE
ulang.h  -> Declara funciones para idiomas del sistema operativo
ugtk3.h  -> Declara funciones para graficos y control de eventos
uresb.h  -> Declara funciones para imprimir archivos de idiomas

1.2. Source files con definiciones

Ruta: ./src/
upath.h  -> Define funciones para resolver rutas absolutas
ustrc.h  -> Define funciones para procesar texto UTF-8/UNICODE
ulang.h  -> Define funciones para idiomas del sistema operativo
ugtk3.h  -> Define funciones para graficos y control de eventos
uresb.h  -> Define funciones para imprimir archivos de idiomas

1.3. Main file con llamadas a las funciones

Ruta: ./
icu_main.c        -> Usado como entrada del programa
icu_aio.c         -> All In One es ignorado por make

2. Exec files para compilar

Ruta: ./
icu_run_make lang -> Compila con "make all" y ejecuta ./bin/
Makefile          -> Usado por "make all" para compilar

3. Exec files para ejecutar

Ruta: ./
icu_run_make lang -> Compila con "make all" y ejecuta ./bin/
Makefile          -> Usado por "make run" para ejecutar
icu_run_gcc lang  -> Imprime contenido del idioma argumento
icu_run_aio lang  -> Análisis de memoria con valgrind

