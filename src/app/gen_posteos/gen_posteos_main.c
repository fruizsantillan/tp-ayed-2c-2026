#include "../../../include/constantes/constantes.h"
#include "../comun/aleatorio.h"
#include "../comun/metalog.h"
#include "../comun/posteo.h"
#include "vocabulario.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PATH_SALIDA_DEFAULT "data/bin/posteos.dat"
#define PARAM_CLAVE_SEMILLA "-s"
#define PARAM_CLAVE_SALIDA "-o"

typedef struct
{
    const char  *pathSalida;
    unsigned    semilla;
} tConfig;

void mostrarUso(const char *programa)
{
    fprintf(stderr, "Uso: %s [opciones]\n", programa);
    fprintf(stderr,
            "  -s semilla       semilla del generador (por defecto: la hora actual, se informa)\n"
            "  -o output        directorio de salida (por defecto %s)\n",
            PATH_SALIDA_DEFAULT);
}

/*
 * GEN_POSTEOS: Generador del archivo de posteos de Tweener
 *
 * Genera 50000 posteos de texto de hasta 140 caracteres, armados al azar
 * combinando sujeto, verbo, objeto, cierre y un hashtag (ver vocabulario.c).
 * Cada registro es {id: int, contenido: char[141]} y el id coincide con la
 * posicion del registro en el archivo, lo que permite buscarlo con fseek.
 * Por eso el archivo debe generarse SIEMPRE con este programa.
 * - Mismo lote de posteos reproducible con la misma semilla (en la misma plataforma)
 * - Se puede especificar la semilla y el archivo de salida
 * - Se imprime la semilla utilizada para poder reproducir el mismo lote
*/

int main(int argc, char *argv[])
{
    tConfig     cfg;
    int         cantidad,
                i,
                id;
    FILE        *lote,
                *archivoLog;
    tPost       posteoGenerado;

    cfg.semilla = (unsigned) time(NULL);
    cfg.pathSalida = PATH_SALIDA_DEFAULT;
    cantidad = CANTIDAD_REGISTROS_POSTEOS;


    for (i = 1; i < argc; i++)
    {
        if (strcmp(*(argv + i), "-s") == 0 && i + 1 < argc)
            cfg.semilla = (unsigned) strtoul(*(argv + ++i), NULL, 10);
        else if (strcmp(*(argv + i), PARAM_CLAVE_SALIDA) == 0 && i + 1 < argc)
            cfg.pathSalida = *(argv + ++i);
        else
        {
            mostrarUso(*argv);
            return 1;
        }
    }

    fprintf(stderr, "Semilla utilizada: %u\n", cfg.semilla);

    lote = fopen(cfg.pathSalida, "wb");
    if (!lote)
    {
        fprintf(stderr, "No se pudo crear %s\n", cfg.pathSalida);
        return 1;
    }

    aleatorio_semilla(cfg.semilla);

    for (id = 0; id < cantidad; id++)
    {
        memset(&posteoGenerado, 0, sizeof(posteoGenerado));
        posteoGenerado.id = id;
        vocabulario_armarTexto(posteoGenerado.contenido, MAX_CARACTERES_POSTEO + 1);
        fwrite(&posteoGenerado, sizeof(posteoGenerado), 1, lote);
    }

    fclose(lote);

    archivoLog = metalog_abrir("datos", cfg.pathSalida, cfg.semilla);
    if (archivoLog)
    {
        fprintf(archivoLog, " | registros: %d | tamanyo_registro: %lu", cantidad, (unsigned long) sizeof(tPost));
        metalog_cerrar(archivoLog);
    }
    else
        fprintf(stderr, "Aviso: no se pudo actualizar %s\n", PATH_METALOG);

    return 0;
}