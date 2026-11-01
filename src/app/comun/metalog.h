#ifndef METALOG_H_INCLUDED
#define METALOG_H_INCLUDED

#include <stdio.h>

#define PATH_METALOG "data/meta/log.txt"
#define METALOG_TAMANYO_FECHA 20    /* "AAAA-MM-DD HH:MM:SS" + '\0' */

/*
Interfaz para centralizar y trazar los metadatos de todos los archivos base que se van generando
*/

FILE *metalog_abrir  (const char *clase, const char *path, unsigned semilla);
void  metalog_cerrar (FILE *log);

#endif // METALOG_H_INCLUDED
