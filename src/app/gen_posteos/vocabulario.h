#ifndef VOCABULARIO_H_INCLUDED
#define VOCABULARIO_H_INCLUDED

#include <stddef.h>

/*
Interfaz para armarle un contenido al post
*/

typedef struct
{
    char    *articulo,
            *sustantivo;
} tNucleo;

void vocabulario_armarTexto(char *buffer, size_t longitudMaxima);

#endif // VOCABULARIO_H_INCLUDED