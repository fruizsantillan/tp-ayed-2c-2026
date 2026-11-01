#include "aleatorio.h"

#include <stdlib.h>

void aleatorio_semilla(unsigned semilla)
{
    srand(semilla);
}

/*  RAND_MAX tiene un límite superior que puede generar problemas:
    se combinan dos llamadas de 15 bits para obtener 30 bits y no
    sesgar rangos grandes (ej. 50000 ids). */

unsigned aleatorio_hasta(unsigned n)
{
    unsigned valor;

    if ( n == 0 )
        return 0;

    valor = ((unsigned) (rand() & 0x7FFF) << 15) | (unsigned) (rand() & 0x7FFF); // Manipulación de bits en parte alta y parte baja; 0x7FFF es 15 bits en alto

    return valor % n;
}

int aleatorio_entre(int minimo, int maximo)
{
    int forzarOrdenCorrecto;

    if (minimo > maximo)
    {
        forzarOrdenCorrecto = minimo;
        minimo = maximo;
        maximo = forzarOrdenCorrecto;
    }

    return minimo + (int) aleatorio_hasta((unsigned) (maximo - minimo + 1));
}

int aleatorio_probabilidad(unsigned porcentaje)
{
    return aleatorio_hasta(100) < porcentaje;
}
