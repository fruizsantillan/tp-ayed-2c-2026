#include "posteo.h"

#include <stdio.h>

/*
Importante que los posteos se generen con el generador (de posteos)
porque sino quizás el id no es autoincremental, y se rompe la ubicación mediante fseek
*/

int posteo_leer(FILE *pf, int id, tPost *destino)
{
    if (fseek(pf, (long) id * (long) sizeof(tPost), SEEK_SET) != 0)
        return 0;

    return fread(destino, sizeof(tPost), 1, pf) == 1;
}