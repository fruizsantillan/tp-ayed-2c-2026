#ifndef VERIFICADOR_COMUNES_H_INCLUDED
#define VERIFICADOR_COMUNES_H_INCLUDED

int terminoLectura                  (long leidos, long limiteALeer);
void mostrarEstadisticasPrevias     (FILE *lote);
void mostrarEstadisticasFinalizado  (const char *salida, long leidosContados, long erroresContados);

#endif // VERIFICADOR_COMUNES_H_INCLUDED