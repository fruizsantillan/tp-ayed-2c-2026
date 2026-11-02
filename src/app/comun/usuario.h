#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

#include "../../../include/constantes/constantes.h"

/* Formato de lote_*.dat: registros consecutivos de tamaño fijo sizeof(tUsuario),
   Cambiar esta estructura invalida los archivos .dat ya generados. */

typedef struct {
    int     feed    [TAMANYO_FEED];
    char    usuario [MAX_CARACTERES_NOMBRE_USUARIO + 1];
} tUsuario;

#endif // USUARIO_H_INCLUDED