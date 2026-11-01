#include "../comun/posteo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARADOR_CSV ';'

int main(int argc, char *argv[])
{
    const char  *entrada,
                *salida;
    long        limite; // registros a volcar en el CSV, -1 significa todos
    FILE        *lote, *resultadoGenerado;
    tPost       posteoLeido;
    long        __stat_tamanyoArchivo, __stat_leidos, __stat_errores;
    int         i;

    entrada = "data/bin/posteos.dat";
    salida  = "data/csv/posteos_verificado.csv";
    limite  = -1;

    // Para evitar parámetros posicionales y mejorar un poco la comprensión del pasaje de argumentos del usuario

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
            limite = atol(argv[++i]);
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
            entrada = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
            salida = argv[++i];
        else
        {
            fprintf(stderr, "Uso: %s [-n cantidad] [-i entrada.dat] [-o salida.csv]\n", argv[0]);
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

    fseek(lote, 0, SEEK_END);
    __stat_tamanyoArchivo = ftell(lote);
    rewind(lote);

    printf("Archivo de datos: %s\n", entrada);
    printf("Tamanyo del registro: %lu bytes\n", (unsigned long) sizeof(tPost));
    printf("Tamanyo del archivo : %ld bytes\n", __stat_tamanyoArchivo);
    printf("Registros esperados : %ld\n", __stat_tamanyoArchivo / (long) sizeof(tPost));

    if ( __stat_tamanyoArchivo % (long) sizeof(tPost) != 0)
        printf("ADVERTENCIA: el tamanyo no es multiplo del registro (lote truncado o de otro formato)\n");

    fprintf(resultadoGenerado, "registro%cid%ccontenido\n", SEPARADOR_CSV, SEPARADOR_CSV); // Cabecera

    __stat_leidos  = 0;
    __stat_errores = 0;

    while ( (limite < 0 || __stat_leidos < limite) && fread(&posteoLeido, sizeof(posteoLeido), 1, lote) == 1 )
    {
        if (posteoLeido.id != __stat_leidos)
        {
            fprintf(stderr, "Registro %ld: id=%d no coincide con la posicion\n", __stat_leidos, posteoLeido.id);
            __stat_errores++;
        }

        if (!memchr(posteoLeido.contenido, '\0', sizeof(posteoLeido.contenido)))
        {
            fprintf(stderr, "Registro %ld: texto sin terminador '\\0'\n", __stat_leidos);
            __stat_errores++;
            posteoLeido.contenido[sizeof(posteoLeido.contenido) - 1] = '\0';
        }

        if (strchr(posteoLeido.contenido, SEPARADOR_CSV))
        {
            fprintf(stderr, "Registro %ld: el texto contiene el separador '%c'\n", __stat_leidos, SEPARADOR_CSV);
            __stat_errores++;
        }

        fprintf(resultadoGenerado, "%ld%c%d%c%s\n", __stat_leidos, SEPARADOR_CSV, posteoLeido.id, SEPARADOR_CSV, posteoLeido.contenido);

        __stat_leidos++;
    }

    printf("Registros volcados  : %ld\n", __stat_leidos);
    printf("Errores encontrados : %ld\n", __stat_errores);
    printf("CSV generado en     : %s\n", salida);

    fclose(lote);
    fclose(resultadoGenerado);

    return __stat_errores ? 1 : 0;
}