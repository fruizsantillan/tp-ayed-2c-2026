#ifndef REPORTE_H_INCLUDED
#define REPORTE_H_INCLUDED

#include <stddef.h>
#include "../app/gen_lotes/distribuciones.h"

#define REPORTE_TAMANYO_MARCA 20    /* "AAAA-MM-DD HH:MM:SS" + '\0' */

/* Resultado de una corrida (un lote con una capacidad de cache) */
typedef struct
{
    const char      *modo;
    size_t          capacidad,
                    usuarios,
                    accesos;
    unsigned long   aciertos,
                    fallos,
                    lecturas;
    double          segundosTotal,
                    microsPorPosteo,
                    microsPorUsuario;
    int             alturaArbol,
                    alturaMinima;
} tResultado; /* En los casos que no aplique, -1 (alturas en parte 1). Capacidad 0 infiere sin caché */

/* Marca "AAAA-MM-DD HH:MM:SS". Se toma una vez por ejecucion: todas las filas de una misma ejecucion la comparten */
void reporte_marcaTiempo        (char *marca);

/* Pantalla: encabezado del lote, titulos de la tabla, una fila por corrida, y pie con las referencias */
void reporte_encabezadoLote     (const char *pathLote, const tEstadisticasLote *estadisticas, size_t usuarios, const char *modo);
void reporte_titulos            (void);
void reporte_fila               (const tResultado *resultado, double segundosBase);
void reporte_pie                (void);

/* CSV en result/ (separador ';'). Preparar lo crea con su cabecera si no existe, o retorna 0 si no se puede escribir */
int  reporte_csvPreparar        (const char *pathCsv);
void reporte_csv                (const char *pathCsv, const char *marca, const char *pathLote, const tResultado *resultado, double segundosBase);

#endif /* REPORTE_H_INCLUDED */