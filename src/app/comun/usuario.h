#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

#include "../../../include/constantes/constantes.h"

typedef struct {
    int     feed    [TAMANYO_FEED];
    char    usuario [MAX_CARACTERES_NOMBRE_USUARIO + 1];
} tUsuario;

#endif // USUARIO_H_INCLUDED