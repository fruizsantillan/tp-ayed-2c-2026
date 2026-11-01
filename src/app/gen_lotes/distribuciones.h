#ifndef DISTRIBUCIONES_H_INCLUDED
#define DISTRIBUCIONES_H_INCLUDED

#include <stddef.h>
#include "../comun/usuario.h"
#include "../../../include/constantes/constantes.h"

/* Las distribuciones SOLO completan los feeds de un arreglo en memoria:
   no abren archivos ni imprimen, y no tocan el nombre del usuario ni el
   relleno de la estructura (eso es responsabilidad del programa que las usa).
   Todas toman los numeros aleatorios de aleatorio.h: se debe llamar antes a
   aleatorio_semilla() para que el lote sea reproducible.
 
   Retornan EXITO, ERROR_SIN_MEMORIA o ERROR_OPERACION_INVALIDA. */
 
/*  Una cantidad de populares menor que el feed haria imposible armar un feed
    sin repetidos solo con posteos populares. */

#define MINIMO_POSTEOS_POPULARES TAMANYO_FEED

typedef struct{
    unsigned    cantidadPopulares,
                porcentajePopulares; /* Probabilidad de que una posicion del feed se obtenga del conjunto de populares */
} tParamsConcentrado;

typedef struct{
    size_t  accesos,
            idsDistintos;
    double  techoAciertos;
} tEstadisticasLote;

/* La más simple, rellena el lote con ids cualquiera, que difieren entre ellas */
int  distribucion_dispersa      (tUsuario *lote, size_t cantidad);

/* Basándose en unos parámetros que definen un conjunto de "posteos populares", elige mayormente de ese conjunto*/
int  distribucion_concentrada   (tUsuario *lote, size_t cantidad, const tParamsConcentrado *params);

/* Todos los posteos son seleccionados en orden —no necesariamente contiguos */
int  distribucion_ordenada      (tUsuario *lote, size_t cantidad);

/* Calcula estadísticas sobre un lote de usuarios */
int  distribucion_estadisticas  (const tUsuario *lote, size_t cantidad, tEstadisticasLote *est);

#endif // DISTRIBUCIONES_H_INCLUDED