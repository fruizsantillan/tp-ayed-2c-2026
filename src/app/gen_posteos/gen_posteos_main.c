#include "../comun/posteo.h"
#include "../comun/aleatorio.h"
#include "vocabulario.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    unsigned    semilla;
    int         cantidad, i, id;
    const char  *salida;
    FILE        *lote;
    tPost       posteoGenerado;

    semilla  = (unsigned) time(NULL);
    salida   = "posteos.dat"; // CAMBIAR ESTO
    cantidad = CANTIDAD_REGISTROS_POSTEOS;

    // Para evitar parámetros posicionales y mejorar un poco la comprensión del pasaje de argumentos del usuario

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
            semilla = (unsigned) atoi(argv[++i]);
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
            salida = argv[++i];
        else
        {
            fprintf(stderr, "Uso: %s [-s semilla] [-o salida.dat]\n", argv[0]);
            return 1;
        }
    }
    
    fprintf(stderr, "Semilla utilizada: %u\n", semilla);

    lote = fopen(salida, "wb");
    if (!lote)
    {
        fprintf(stderr, "No se pudo crear %s\n", salida);
        return 1;
    }

    aleatorio_semilla(semilla);

    for (id = 0; id < cantidad; id++)
    {
        memset(&posteoGenerado, 0, sizeof(posteoGenerado));
        posteoGenerado.id = id;
        vocabulario_armarTexto(posteoGenerado.contenido, MAX_CARACTERES_POSTEO + 1);
        fwrite(&posteoGenerado, sizeof(posteoGenerado), 1, lote);
    }

    fclose(lote);
    return 0;
}