#include "../comun/usuario.h"
#include "verificador_comunes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARADOR_CSV ';'
#define MARCA_ORDEN_BYTES "\xEF\xBB\xBF"
#define PATH_SALIDA_DEFAULT "data/csv/usuarios_verificado.csv"
#define LIMITE_MOSTRAR_TODOS -1
#define PARAM_CLAVE_LIMITE "-n"
#define PARAM_CLAVE_ENTRADA "-i"
#define PARAM_CLAVE_SALIDA "-o"


/**
* Generar un CSV a partir de un archivo binario formateado de registros de usuarios
* con el objetivo de debuggear posibles problemas en la generación de archivos binarios
* o visualizar con lo que se está trabajando. Especialmente útil para demostrar que
* se generan los lotes de usuarios de distintos tipos (concentrado, disperso, etc.)
*
*/

int main(int argc, char *argv[])
{
    const char  *entrada,
                *salida;
    long        limite; /* registros a volcar en el CSV, -1 significa todos */
    FILE        *lote,
                *resultadoGenerado;
    tUsuario    usuarioLeido;
    long        stat_leidosCount,
                stat_erroresCount;
    int         i, j, id;

    entrada = NULL; /* La entrada es obligatoria porque los lotes de usuarios pueden tener nombres distintos */
    salida  = PATH_SALIDA_DEFAULT;
    limite  = LIMITE_MOSTRAR_TODOS;

    /* Parámetros (el getopt) */
    for (i = 1; i < argc; i++)
    {
        if (strcmp(*(argv + i), PARAM_CLAVE_LIMITE) == 0 && i + 1 < argc)
            limite = atol(*(argv + ++i));
        else if (strcmp(*(argv + i), PARAM_CLAVE_ENTRADA) == 0 && i + 1 < argc)
            entrada = *(argv + ++i);
        else if (strcmp(*(argv + i), PARAM_CLAVE_SALIDA) == 0 && i + 1 < argc)
            salida = *(argv + ++i);
        else
        {
            fprintf(stderr, "Uso: %s -i entrada.dat [-n cantidad] [-o salida.csv]\n", *argv);
            return 1;
        }
    }

    if (!entrada)
    {
        fprintf(stderr, "Uso: %s -i entrada.dat [-n cantidad] [-o salida.csv]\n", *argv);
        return 1;
    }

    lote = fopen(entrada, "rb");
    if (!lote)
    {
        fprintf(stderr, "No se pudo abrir %s\n", entrada);
        return 1;
    }

    resultadoGenerado = fopen(salida, "wt");
    if (!resultadoGenerado)
    {
        fprintf(stderr, "No se pudo crear %s\n", salida);
        fclose(lote);
        return 1;
    }

    fputs(MARCA_ORDEN_BYTES, resultadoGenerado);

    mostrarEstadisticasPrevias(lote, entrada, sizeof(tUsuario));

    fprintf(resultadoGenerado, "registro%cusuario", SEPARADOR_CSV); /* Cabecera */
    for (i = 0; i < TAMANYO_FEED; i++)
        fprintf(resultadoGenerado, "%cfeed%02d", SEPARADOR_CSV, i + 1);
    fprintf(resultadoGenerado, "\n");

    stat_leidosCount  = 0;
    stat_erroresCount = 0;

    while ( !terminoLectura(stat_leidosCount,limite) && fread(&usuarioLeido, sizeof(usuarioLeido), 1, lote) == 1 )
    {
        if (!memchr(usuarioLeido.usuario, '\0', sizeof(usuarioLeido.usuario))) /* Si no tiene \0 no es una cadena, usar strchr sería un error */
        {
            fprintf(stderr, "Registro %ld: usuario sin terminador '\\0'\n", stat_leidosCount);
            stat_erroresCount++;
            usuarioLeido.usuario[sizeof(usuarioLeido.usuario) - 1] = '\0';
        }

        if (strchr(usuarioLeido.usuario, SEPARADOR_CSV))
        {
            fprintf(stderr, "Registro %ld: el usuario contiene el separador '%c'\n", stat_leidosCount, SEPARADOR_CSV);
            stat_erroresCount++;
        }

        for (i = 0; i < TAMANYO_FEED; i++)
        {
            id = *(usuarioLeido.feed + i);

            if (id < 0 || id >= CANTIDAD_REGISTROS_POSTEOS)
            {
                fprintf(stderr, "Registro %ld: feed[%d]=%d fuera de rango\n", stat_leidosCount, i, id);
                stat_erroresCount++;
            }

            for (j = 0; j < i; j++)
                if (*(usuarioLeido.feed + j) == id)
                {
                    fprintf(stderr, "Registro %ld: feed[%d]=%d repetido (ya estaba en feed[%d])\n", stat_leidosCount, i, id, j);
                    stat_erroresCount++;
                }
        }

        fprintf(resultadoGenerado, "%ld%c%s", stat_leidosCount, SEPARADOR_CSV, usuarioLeido.usuario);
        for (i = 0; i < TAMANYO_FEED; i++)
            fprintf(resultadoGenerado, "%c%d", SEPARADOR_CSV, *(usuarioLeido.feed + i));
        fprintf(resultadoGenerado, "\n");

        stat_leidosCount++;
    }

    mostrarEstadisticasFinalizado(salida, stat_leidosCount, stat_erroresCount);

    fclose(lote);
    fclose(resultadoGenerado);

    return stat_erroresCount ? 1 : 0;
}