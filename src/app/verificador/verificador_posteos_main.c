#include "../comun/posteo.h"
#include "verificador_comunes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARADOR_CSV ';'
#define MARCA_ORDEN_BYTES "\xEF\xBB\xBF"
#define PATH_ENTRADA_DEFAULT "data/bin/posteos.dat"
#define PATH_SALIDA_DEFAULT "data/csv/posteos_verificado.csv"
#define LIMITE_MOSTRAR_TODOS -1
#define PARAM_CLAVE_LIMITE "-n"
#define PARAM_CLAVE_ENTRADA "-i"
#define PARAM_CLAVE_SALIDA "-o"

/**
* @brief    Generar un CSV a partir de un archivo binario formateado de registros de posteos
*           con el objetivo de debuggear posibles problemas en la generación de archivos binarios
*           o visualizar con lo que se está trabajando.
*
*/

int main(int argc, char *argv[])
{
    const char  *entrada,
                *salida;
    long        limite; /* registros a volcar en el CSV, -1 significa todos */
    FILE        *lote,
                *resultadoGenerado;
    tPost       posteoLeido;
    long        stat_leidosCount,
                stat_erroresCount;
    int         i;

    entrada = PATH_ENTRADA_DEFAULT;
    salida  = PATH_SALIDA_DEFAULT;
    limite  = LIMITE_MOSTRAR_TODOS;

    /* Para evitar parámetros posicionales y mejorar un poco la comprensión del pasaje de argumentos del usuario */

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
            fprintf(stderr, "Uso: %s [-n cantidad] [-i entrada.dat] [-o salida.csv]\n", *argv);
            return 1;
        }
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

    /* Para tildes */
    fputs(MARCA_ORDEN_BYTES, resultadoGenerado);

    mostrarEstadisticasPrevias(lote, entrada, sizeof(tPost));

    fprintf(resultadoGenerado, "registro%cid%ccontenido\n", SEPARADOR_CSV, SEPARADOR_CSV); /* Cabecera */

    stat_leidosCount  = 0;
    stat_erroresCount = 0;

    while ( !terminoLectura(stat_leidosCount, limite) && fread(&posteoLeido, sizeof(posteoLeido), 1, lote) == 1 )
    {
        if (posteoLeido.id != stat_leidosCount)
        {
            fprintf(stderr, "Registro %ld: id=%d no coincide con la posicion\n", stat_leidosCount, posteoLeido.id);
            stat_erroresCount++;
        }

        if (!memchr(posteoLeido.contenido, '\0', sizeof(posteoLeido.contenido)))
        {
            fprintf(stderr, "Registro %ld: texto sin terminador '\\0'\n", stat_leidosCount);
            stat_erroresCount++;
            posteoLeido.contenido[sizeof(posteoLeido.contenido) - 1] = '\0';
        }

        if (strchr(posteoLeido.contenido, SEPARADOR_CSV))
        {
            fprintf(stderr, "Registro %ld: el texto contiene el separador '%c'\n", stat_leidosCount, SEPARADOR_CSV);
            stat_erroresCount++;
        }

        fprintf(resultadoGenerado, "%ld%c%d%c%s\n", stat_leidosCount, SEPARADOR_CSV, posteoLeido.id, SEPARADOR_CSV, posteoLeido.contenido);

        stat_leidosCount++;
    }

    mostrarEstadisticasFinalizado(salida, stat_leidosCount, stat_erroresCount);

    fclose(lote);
    fclose(resultadoGenerado);

    return stat_erroresCount ? 1 : 0;
}