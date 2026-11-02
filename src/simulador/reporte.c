#include "reporte.h"

#include <stdio.h>
#include <time.h>

#define ANCHO_TABLA                 110     /* Arbitrario */
#define UMBRAL_RESOLUCION_SEGUNDOS  0.05    /* Por resolución de clock() */

/* Funciones internas */

void __reporte_linea(char caracter)
{
    int i;

    for (i = 0; i < ANCHO_TABLA; i++)
        putchar(caracter);
    putchar('\n');
}

/* Mejora relativa (%) = (tiempo sin cache - tiempo con cache) / tiempo sin cache * 100.
   Positiva: la cache acelera (100 = tiempo nulo). Negativa: la cache empeora, sin cota inferior.
   Retorna 0 si no se puede calcular (base nula), 1 si dejo el valor en *mejora. */
int __reporte_mejora(const tResultado *resultado, double segundosBase, double *mejora)
{
    if (segundosBase <= 0)
        return 0;

    *mejora = (segundosBase - resultado->segundosTotal) / segundosBase * 100.0;

    return 1;
}

/* Primitivas */

void reporte_marcaTiempo(char *marca)
{
    time_t ahora;

    ahora = time(NULL);
    strftime(marca, REPORTE_TAMANYO_MARCA, "%Y-%m-%d %H:%M:%S", localtime(&ahora));
}

void reporte_encabezadoLote(const char *pathLote, const tEstadisticasLote *estadisticas, size_t usuarios, const char *modo)
{
    printf("\n");
    __reporte_linea('=');
    printf("Lote     : %s\n", pathLote);
    printf("Modo     : %s\n", modo);
    printf("Usuarios : %lu | accesos: %lu | ids distintos: %lu | techo de aciertos: %.2f%%\n",
           (unsigned long) usuarios, (unsigned long) estadisticas->accesos,
           (unsigned long) estadisticas->idsDistintos, estadisticas->techoAciertos * 100.0);
    __reporte_linea('-');
}

void reporte_titulos(void)
{
    printf("%10s %9s %9s %9s %8s %10s %10s %11s %7s %7s %10s\n",
           "cache", "aciertos", "fallos", "lecturas", "acierto",
           "total(s)", "us/posteo", "us/usuario", "altura", "h_min", "mejora");
    __reporte_linea('-');
}

void reporte_fila(const tResultado *resultado, double segundosBase)
{
    double  mejora;
    char    marca;

    marca = resultado->segundosTotal < UMBRAL_RESOLUCION_SEGUNDOS ? '*' : ' ';

    if (resultado->capacidad == 0)
        printf("%10s %9s %9s", "sin cache", "-", "-");
    else
        printf("%10lu %9lu %9lu", (unsigned long) resultado->capacidad, resultado->aciertos, resultado->fallos);

    printf(" %9lu", resultado->lecturas);

    if (resultado->capacidad == 0)
        printf(" %8s", "-");
    else
        printf(" %7.2f%%", 100.0 * (double) resultado->aciertos / (double) resultado->accesos);

    printf(" %9.4f%c %10.3f %11.2f",
           resultado->segundosTotal, marca, resultado->microsPorPosteo, resultado->microsPorUsuario);

    if (resultado->alturaArbol < 0)
        printf(" %7s %7s", "-", "-");
    else
        printf(" %7d %7d", resultado->alturaArbol, resultado->alturaMinima);

    if (__reporte_mejora(resultado, segundosBase, &mejora))
        printf(" %+9.1f%%", mejora);
    else
        printf(" %10s", "n/d");

    printf("\n");
}

void reporte_pie(void)
{
    __reporte_linea('-');
    printf("mejora   = (t sin cache - t con cache) / t sin cache: positiva, la cache acelera; negativa, la empeora\n");
    printf("altura   = altura del arbol indice; h_min = la minima posible para esa cantidad de nodos\n");
    printf("*        = total menor a 50 ms: la resolucion de clock() puede distorsionar, usar un lote con mas usuarios\n");
}

int reporte_csvPreparar(const char *pathCsv)
{
    FILE *csv;

    csv = fopen(pathCsv, "r");
    if (csv)
    {
        fclose(csv);
        return 1;
    }

    csv = fopen(pathCsv, "w");
    if (!csv)
        return 0;

    fprintf(csv, "corrida;modo;lote;usuarios;capacidad;accesos;aciertos;fallos;lecturas;"
                 "tasa_aciertos_pct;segundos;us_posteo;us_usuario;altura;altura_minima;mejora_relativa_pct\n");
    fclose(csv);

    return 1;
}

void reporte_csv(const char *pathCsv, const char *marca, const char *pathLote, const tResultado *resultado, double segundosBase)
{
    FILE    *csv;
    double  mejora;

    csv = fopen(pathCsv, "a");
    if (!csv)
        return;

    if (!__reporte_mejora(resultado, segundosBase, &mejora))
        mejora = 0;

    fprintf(csv, "%s;%s;%s;%lu;%lu;%lu;%lu;%lu;%lu;%.4f;%.6f;%.3f;%.2f;%d;%d;%.4f\n",
            marca, resultado->modo, pathLote,
            (unsigned long) resultado->usuarios, (unsigned long) resultado->capacidad,
            (unsigned long) resultado->accesos,
            resultado->aciertos, resultado->fallos, resultado->lecturas,
            100.0 * (double) resultado->aciertos / (double) resultado->accesos,
            resultado->segundosTotal, resultado->microsPorPosteo, resultado->microsPorUsuario,
            resultado->alturaArbol, resultado->alturaMinima,
            mejora);

    fclose(csv);
}