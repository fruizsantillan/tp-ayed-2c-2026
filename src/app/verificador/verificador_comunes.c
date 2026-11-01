#include "verificador_comunes.h"

#include <stdio.h>
#include "../comun/usuario.h"

int terminoLectura (long leidos, long limiteALeer)
{
    return limiteALeer >= 0 && leidos >= limiteALeer;
}

void mostrarEstadisticasPrevias (FILE *lote, const char *nombre, size_t tamanyoRegistro)
{
    long tamanyoArchivo;

    if (!lote) return;

    fseek(lote, 0, SEEK_END);
    tamanyoArchivo = ftell(lote);
    rewind(lote);

    printf("Archivo de datos: %s\n", nombre);
    printf("Tamanyo del registro: %lu bytes\n", (unsigned long) tamanyoRegistro);
    printf("Tamanyo del archivo : %ld bytes\n", tamanyoArchivo);
    printf("Registros esperados : %ld\n", tamanyoArchivo / (long) tamanyoRegistro);

    if ( tamanyoArchivo % (long) tamanyoRegistro != 0)
        printf("ADVERTENCIA: el tamanyo no es multiplo del registro (lote truncado o de otro formato)\n");
}

void mostrarEstadisticasFinalizado(const char *salida, long leidosContados, long erroresContados)
{
    printf("Registros volcados  : %ld\n", leidosContados);
    printf("Errores encontrados : %ld\n", erroresContados);
    printf("CSV generado en     : %s\n", salida);
}
