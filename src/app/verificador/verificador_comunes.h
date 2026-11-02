#ifndef VERIFICADOR_COMUNES_H_INCLUDED
#define VERIFICADOR_COMUNES_H_INCLUDED

#include <stdio.h>

int terminoLectura                  (long leidos, long limiteALeer);
void mostrarEstadisticasPrevias     (FILE *lote, const char *nombre, size_t tamanyoRegistro);
void mostrarEstadisticasFinalizado  (const char *salida, long leidosContados, long erroresContados);

#endif // VERIFICADOR_COMUNES_H_INCLUDED