#ifndef POSTEO_H_INCLUDED
#define POSTEO_H_INCLUDED

#include <stdio.h>
#include "../../../include/constantes/constantes.h"

/* Formato de posteos.dat: registros consecutivos de tamaño fijo sizeof(tPost),
   donde id coincide con la posición del registro en el archivo.
   Cambiar esta estructura invalida los archivos .dat ya generados. */

typedef struct
{
    int     id;
    char    contenido[MAX_CARACTERES_POSTEO+1];
} tPost;

int posteo_leer(FILE *pf, int id, tPost *destino);

#endif // POSTEO_H_INCLUDED
