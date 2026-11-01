#include "distribuciones.h"
#include "../comun/aleatorio.h"
#include "../comun/metalog.h"
#include "../comun/usuario.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PARAM_CLAVE_TIPO "-t"
#define PARAM_CLAVE_SALIDA "-o"
#define PARAM_CLAVE_SEMILLA "-s"
#define PARAM_CLAVE_USUARIOS "-u"
#define PARAM_CLAVE_CANTPOPU "-c"
#define PARAM_CLAVE_PROBPOPU "-p"
#define TIPO_CONCENTRADO                "concentrado"
#define TIPO_DISPERSO                   "disperso"
#define TIPO_ORDENADO                   "ordenado"
#define PATH_CONCENTRADO_DEFAULT        "data/bin/lote_concentrado.dat"
#define PATH_DISPERSO_DEFAULT           "data/bin/lote_disperso.dat"
#define PATH_ORDENADO_DEFAULT           "data/bin/lote_ordenado.dat"
#define USUARIOS_DEFAULT                2000
#define POPULARES_DEFAULT               100
#define PORCENTAJE_POPULARES_DEFAULT    90

/* El nombre es "usuario" + 6 digitos + '\0' = 14 bytes: entra en el char[15]
   de la consigna hasta 999999 usuarios. Pasado ese numero los nombres se
   repetirian, por eso es el unico limite superior. */
#define MAXIMO_USUARIOS                 999999

/* Todo lo configurable, en config, como en la generación de posteos */
typedef struct
{
    const char          *tipo;
    const char          *pathSalida;
    unsigned            semilla;
    unsigned long       usuarios;
    tParamsConcentrado  concentrado;
} tConfig;

void mostrarUso(const char *programa)
{
    fprintf(stderr, "Uso: %s -t concentrado|disperso|ordenado [opciones]\n", programa);
    fprintf(stderr,
            "  -s semilla       semilla del generador (por defecto: la hora actual, se informa)\n"
            "  -o archivo       archivo de salida (por defecto data/binarios/lote_<tipo>.dat)\n"
            "  -u usuarios      cantidad de usuarios, entre %d y %d (por defecto %d)\n"
            "                   el lote ordenado admite como maximo %d\n",
            MINIMO_USUARIOS, MAXIMO_USUARIOS, USUARIOS_DEFAULT,
            CANTIDAD_REGISTROS_POSTEOS / TAMANYO_FEED);
    fprintf(stderr,
            "  -c populares     (concentrado) posteos populares, entre %d y %d (por defecto %d)\n"
            "  -p porcentaje    (concentrado) %% de posiciones tomadas de los populares,\n"
            "                   entre 0 y 100 (por defecto %d)\n",
            MINIMO_POSTEOS_POPULARES, CANTIDAD_REGISTROS_POSTEOS, POPULARES_DEFAULT,
            PORCENTAJE_POPULARES_DEFAULT);
}

int leerArgumentos(int argc, char *argv[], tConfig *cfg)
{
    const char  *pathPorDefecto;
    int         i;

    cfg->tipo = NULL;
    cfg->pathSalida = NULL;
    cfg->semilla = (unsigned) time(NULL);
    cfg->usuarios = USUARIOS_DEFAULT;
    cfg->concentrado.cantidadPopulares = POPULARES_DEFAULT;
    cfg->concentrado.porcentajePopulares = PORCENTAJE_POPULARES_DEFAULT;


    for (i = 1; i < argc; i++)
    {
        if (strcmp(*(argv + i), PARAM_CLAVE_TIPO) == 0 && i + 1 < argc)
            cfg->tipo = *(argv + ++i);
        else if (strcmp(*(argv + i), PARAM_CLAVE_SALIDA) == 0 && i + 1 < argc)
            cfg->pathSalida = *(argv + ++i);
        else if (strcmp(*(argv + i), PARAM_CLAVE_SEMILLA) == 0 && i + 1 < argc)
            cfg->semilla = (unsigned) strtoul(*(argv + ++i), NULL, 10);
        else if (strcmp(*(argv + i), PARAM_CLAVE_USUARIOS) == 0 && i + 1 < argc)
            cfg->usuarios = strtoul(*(argv + ++i), NULL, 10);
        else if (strcmp(*(argv + i), PARAM_CLAVE_CANTPOPU) == 0 && i + 1 < argc)
            cfg->concentrado.cantidadPopulares = (unsigned) strtoul(*(argv + ++i), NULL, 10);
        else if (strcmp(*(argv + i), PARAM_CLAVE_PROBPOPU) == 0 && i + 1 < argc)
            cfg->concentrado.porcentajePopulares = (unsigned) strtoul(*(argv + ++i), NULL, 10);
        else
        {
            mostrarUso(*argv);
            return ERROR_OPERACION_INVALIDA;
        }
    }

    if (!cfg->tipo)
        pathPorDefecto = NULL;
    else if (strcmp(cfg->tipo, TIPO_CONCENTRADO) == 0)
        pathPorDefecto = PATH_CONCENTRADO_DEFAULT;
    else if (strcmp(cfg->tipo, TIPO_DISPERSO) == 0)
        pathPorDefecto = PATH_DISPERSO_DEFAULT;
    else if (strcmp(cfg->tipo, TIPO_ORDENADO) == 0)
        pathPorDefecto = PATH_ORDENADO_DEFAULT;
    else
        pathPorDefecto = NULL;

    if (!pathPorDefecto)
    {
        mostrarUso(*argv);
        return ERROR_OPERACION_INVALIDA;
    }

    if (!cfg->pathSalida)
        cfg->pathSalida = pathPorDefecto;

    if (cfg->usuarios < MINIMO_USUARIOS || cfg->usuarios > MAXIMO_USUARIOS)
    {
        fprintf(stderr, "La cantidad de usuarios debe estar entre %d y %d\n", MINIMO_USUARIOS, MAXIMO_USUARIOS);
        return ERROR_OPERACION_INVALIDA;
    }

    if (strcmp(cfg->tipo, TIPO_ORDENADO) == 0 && cfg->usuarios > CANTIDAD_REGISTROS_POSTEOS / TAMANYO_FEED)
    {
        fprintf(stderr, "El lote ordenado admite como maximo %lu usuarios\n", (long unsigned int) CANTIDAD_REGISTROS_POSTEOS / TAMANYO_FEED);
        return ERROR_OPERACION_INVALIDA;
    }

    return EXITO;
}

/* Las distribuciones validan sus propios parametros (ordenado, populares, porcentaje) */
int armarLote(const tConfig *cfg, tUsuario *lote)
{
    unsigned long   i;
    int             resultado;

    aleatorio_semilla(cfg->semilla);

    if (strcmp(cfg->tipo, TIPO_CONCENTRADO) == 0)
        resultado = distribucion_concentrada(lote, cfg->usuarios, &cfg->concentrado);
    else if (strcmp(cfg->tipo, TIPO_DISPERSO) == 0)
        resultado = distribucion_dispersa(lote, cfg->usuarios);
    else
        resultado = distribucion_ordenada(lote, cfg->usuarios);

    if (resultado != EXITO)
        return resultado;

    for (i = 0; i < cfg->usuarios; i++)
        sprintf((lote + i)->usuario, "usuario%06lu", (i + 1) % (MAXIMO_USUARIOS + 1));

    return EXITO;
}

/*
 * GEN_LOTES: Generador de lotes de usuarios (feeds de Tweener)
 *
 * Genera un archivo binario de usuarios {usuario: char[15], feed: int[20]}.
 * Cada feed lista ids de posteos (posiciones en posteos.dat), sin repetidos
 * dentro del mismo feed. Los usuarios se llaman usuario000001, usuario000002, ...
 * - concentrado: muchos usuarios ven los mismos posteos. Un conjunto de posteos
 *   populares aporta el porcentaje -p de las posiciones; el resto sale de todo el archivo.
 * - disperso: posteos mayormente distintos, uniforme sobre todo el archivo.
 * - ordenado: ids estrictamente crecientes en TODO el lote (no solo en cada feed).
 * - Mismo lote reproducible con la misma semilla y parametros (en la misma plataforma)
 * - Se imprime la semilla utilizada y, al terminar, un resumen con los parametros,
 *   los accesos, los ids distintos y el techo de aciertos (1 - distintos/accesos):
 *   lo maximo que cualquier cache puede acertar con ese lote.
 * - Se genera un lote por ejecucion; para repetir con otros parametros, usar -o
 *   con otro nombre y no pisar el anterior
*/

int main(int argc, char *argv[])
{
    tConfig             cfg;
    tEstadisticasLote   estadisticas;
    tUsuario            *lote;
    FILE                *archivoUsuarios,
                        *archivoLog;

    if (leerArgumentos(argc, argv, &cfg) != EXITO)
        return 1;

    fprintf(stderr, "Semilla utilizada: %u\n", cfg.semilla);

    lote = malloc(cfg.usuarios * sizeof(tUsuario));
    if (!lote)
    {
        fprintf(stderr, "Sin memoria para %lu usuarios\n", cfg.usuarios);
        return 1;
    }
    memset(lote, 0, cfg.usuarios * sizeof(tUsuario));

    if (armarLote(&cfg, lote) != EXITO)
    {
        fprintf(stderr, "No se pudo generar el lote %s: revise los parametros (ver restricciones en el uso)\n", cfg.tipo);
        free(lote);
        return 1;
    }

    archivoUsuarios = fopen(cfg.pathSalida, "wb");
    if (!archivoUsuarios)
    {
        fprintf(stderr, "No se pudo crear %s\n", cfg.pathSalida);
        free(lote);
        return 1;
    }

    fwrite(lote, sizeof(tUsuario), cfg.usuarios, archivoUsuarios);
    fclose(archivoUsuarios);

    printf("Lote %s -> %s\n", cfg.tipo, cfg.pathSalida);
    printf("  semilla: %u | usuarios: %lu", cfg.semilla, cfg.usuarios);
    if (strcmp(cfg.tipo, TIPO_CONCENTRADO) == 0)
        printf(" | populares: %u | porcentaje: %u%%",
               cfg.concentrado.cantidadPopulares, cfg.concentrado.porcentajePopulares);
    printf("\n");

    if (distribucion_estadisticas(lote, cfg.usuarios, &estadisticas) == EXITO)
        printf("  accesos: %lu | ids distintos: %lu | techo de aciertos: %.2f%%\n",
               (unsigned long) estadisticas.accesos, (unsigned long) estadisticas.idsDistintos,
               estadisticas.techoAciertos * 100.0);

    free(lote);

    archivoLog = metalog_abrir("lote", cfg.pathSalida, cfg.semilla);
    if (archivoLog)
    {
        fprintf(archivoLog, " | registros: %d | tamanyo_registro: %lu", cfg.usuarios, (unsigned long) sizeof(tUsuario));
        metalog_cerrar(archivoLog);
    }
    else
        fprintf(stderr, "Aviso: no se pudo actualizar %s\n", PATH_METALOG);

    return 0;
}
