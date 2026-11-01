#include "distribuciones.h"

#include "../../../include/constantes/constantes.h"
#include "../comun/aleatorio.h"

#include <stdlib.h>
#include <string.h>

/* Funciones internas */

int estaEnFeed(const int *feed, int usados, int id)
{
    int i;

    for (i = 0; i < usados; i++)
        if (*(feed + i) == id)
            return 1;

    return 0;
}

/* Sortea sobre todo el archivo hasta obtener un id que no este en feed */
int sortearDistintoDelFeed(const int *feed, int usados)
{
    int id;

    do
        id = (int) aleatorio_hasta(CANTIDAD_REGISTROS_POSTEOS);
    while (estaEnFeed(feed, usados, id));

    return id;
}

/* Primitivas */

int distribucion_dispersa(tUsuario *lote, size_t cantidad)
{
    size_t  u;
    int     i;

    if (!lote || cantidad == 0)
        return ERROR_OPERACION_INVALIDA;

    for (u = 0; u < cantidad; u++)
        for (i = 0; i < TAMANYO_FEED; i++)
            *((lote + u)->feed + i) = sortearDistintoDelFeed((lote + u)->feed, i);

    return EXITO;
}

int distribucion_concentrada(tUsuario *lote, size_t cantidad, const tParamsConcentrado *params)
{
    int     *idsPosteos;       /* Los primeros cantidadPopulares son los posteos que más se elegirán */
    size_t  usuarioEnGeneracion;
    int     id, auxiliarDeIntercambio;
    unsigned i, posicionDeIntercambio;

    if (!lote || cantidad == 0 || !params
        || params->cantidadPopulares < MINIMO_POSTEOS_POPULARES
        || params->cantidadPopulares > CANTIDAD_REGISTROS_POSTEOS
        || params->porcentajePopulares > 100)
        return ERROR_OPERACION_INVALIDA;

    /* Se asigna ids, un bloque de por defecto 50000 enteros */
    idsPosteos = malloc(CANTIDAD_REGISTROS_POSTEOS * sizeof(int));
    if (!idsPosteos)
        return ERROR_SIN_MEMORIA;

    /* Se rellena con todos los ids que estarán en la bbdd de posteos */
    for (i = 0; i < CANTIDAD_REGISTROS_POSTEOS; i++)
        *(idsPosteos + i) = (int) i;

    /* Mezclado aleatorio */
    for (i = 0; i < params->cantidadPopulares; i++)
    {
        posicionDeIntercambio = i + aleatorio_hasta(CANTIDAD_REGISTROS_POSTEOS - i);
        auxiliarDeIntercambio = *(idsPosteos + i);
        *(idsPosteos + i) = *(idsPosteos + posicionDeIntercambio);
        *(idsPosteos + posicionDeIntercambio) = auxiliarDeIntercambio;
    }
    /* Los populares son aleatorios, pero quedan en los primeros params->cantidadPopulares */

    for (usuarioEnGeneracion = 0; usuarioEnGeneracion < cantidad; usuarioEnGeneracion++)
    {
        for (i = 0; i < TAMANYO_FEED; i++)
        {
            /*  El params->porcentajePopulares porciento de las veces, se elige de los populares
                y el otro 1-params->porcentajePopulares, de los no populares, o restantes */
            if (aleatorio_probabilidad(params->porcentajePopulares))
            {
                do
                    id = *(idsPosteos + aleatorio_hasta(params->cantidadPopulares));
                while (estaEnFeed((lote + usuarioEnGeneracion)->feed, i, id));
            }
            else
                id = sortearDistintoDelFeed((lote + usuarioEnGeneracion)->feed, i);

            *((lote + usuarioEnGeneracion)->feed + i) = id;
        }
    }

    free(idsPosteos);

    return EXITO;
}

int distribucion_ordenada(tUsuario *lote, size_t cantidad)
{
    size_t  posteosRestantesASeleccionar, posicion, disponibles;
    int     id;

    if (!lote || cantidad == 0 || cantidad > CANTIDAD_REGISTROS_POSTEOS / TAMANYO_FEED)
        return ERROR_OPERACION_INVALIDA;

    posteosRestantesASeleccionar = cantidad * TAMANYO_FEED;
    disponibles = CANTIDAD_REGISTROS_POSTEOS;
    posicion = 0;

    for (id = 0; id < CANTIDAD_REGISTROS_POSTEOS && posteosRestantesASeleccionar > 0; id++)
    {
        if (aleatorio_hasta((unsigned) disponibles) < posteosRestantesASeleccionar)
        {
            *((lote + (posicion / TAMANYO_FEED))->feed + (posicion % TAMANYO_FEED)) = id;
            posicion++; /* Solo se avanza cuando se escribe, no pueden quedar huecos */
            posteosRestantesASeleccionar--;
        }

        disponibles--;
    }

    return EXITO;
}

int distribucion_estadisticas(const tUsuario *lote, size_t cantidad, tEstadisticasLote *estadisticas)
{
    char    *vistos;
    size_t  usuarioEnGeneracion;
    int     i, id;

    if (!lote || cantidad == 0 || !estadisticas)
        return ERROR_OPERACION_INVALIDA;

    vistos = malloc(CANTIDAD_REGISTROS_POSTEOS * sizeof(char));
    if (!vistos)
        return ERROR_SIN_MEMORIA;
    memset(vistos, 0, CANTIDAD_REGISTROS_POSTEOS * sizeof(char));

    estadisticas->accesos = cantidad * TAMANYO_FEED;
    estadisticas->idsDistintos = 0;

    for (usuarioEnGeneracion = 0; usuarioEnGeneracion < cantidad; usuarioEnGeneracion++)
        for (i = 0; i < TAMANYO_FEED; i++)
        {
            id = *((lote + usuarioEnGeneracion)->feed + i);

            if (id < 0 || id >= CANTIDAD_REGISTROS_POSTEOS)
            {
                free(vistos);
                return ERROR_OPERACION_INVALIDA;
            }

            if (!*(vistos + id))
            {
                *(vistos + id) = 1;
                estadisticas->idsDistintos++;
            }
        }

    free(vistos);

    estadisticas->techoAciertos = 1.0 - (double) estadisticas->idsDistintos / (double) estadisticas->accesos;

    return EXITO;
}