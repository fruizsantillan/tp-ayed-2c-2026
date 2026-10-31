#ifndef VOCABULARIO_H_INCLUDED
#define VOCABULARIO_H_INCLUDED

#include <stdlib.h>

typedef struct
{
    char    *articulo,
            *sustantivo;
} tNucleo;

void vocabulario_armarTexto(char *buffer, size_t longitudMaxima);

#endif // VOCABULARIO_H_INCLUDED