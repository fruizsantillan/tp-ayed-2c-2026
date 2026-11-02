#include "proveedor.h"
#include "reporte.h"
#include "../app/comun/usuario.h"
#include "../app/gen_lotes/distribuciones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PARAM_CLAVE_LOTE_ANALIZADO  "-l"
#define PARAM_CLAVE_CAPACIDAD       "-c"
#define PARAM_CLAVE_ARCH_POSTEOS    "-p"
#define PARAM_CLAVE_SALIDA          "-o"
#define PATH_POSTEOS_DEFAULT        "data/bin/posteos.dat"
#define PATH_CSV_DEFAULT            "result/simulacion.csv"
#define MAXIMO_LOTES                8
#define MAXIMO_CAPACIDADES          16
#define MICROSEGUNDOS_POR_SEGUNDO   1000000.0

const size_t capacidadesPorDefecto[] = {10, 50, 100, 500, 1000, 2500, 5000, 10000};

typedef struct
{
    const char  *pathPosteos,
                *pathCsv,
                *lotes[MAXIMO_LOTES];
    size_t      capacidades[MAXIMO_CAPACIDADES];
    int         cantidadLotes,
                cantidadCapacidades,
                guardarCsv;
} tConfig; /* Se declara un maximo en algunas config porque puede llegar a crashear, si se envian por ejemplo 800 capacidades a estudiar */

void mostrarUso(const char *programa)
{
    fprintf(stderr, "Uso: %s -l lote [-l lote ...] [opciones]\n", programa);
    fprintf(stderr,
            "  -l lote          archivo de lote de usuarios (repetible, hasta %d)\n"
            "  -c capacidad     capacidad de la cache (repetible, hasta %d)\n"
            "                   por defecto: 10 50 100 500 1000 2500 5000 10000\n"
            "  -p posteos       archivo de posteos (por defecto %s)\n"
            "  -o csv           csv al que se agregan los resultados (por defecto %s)\n",
            MAXIMO_LOTES, MAXIMO_CAPACIDADES, PATH_POSTEOS_DEFAULT, PATH_CSV_DEFAULT);
}

int leerArgumentos(int argc, char *argv[], tConfig *cfg)
{
    int i;

    cfg->pathPosteos = PATH_POSTEOS_DEFAULT;
    cfg->pathCsv = PATH_CSV_DEFAULT;
    cfg->cantidadLotes = 0;
    cfg->cantidadCapacidades = 0;
    cfg->guardarCsv = 0;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(*(argv + i), PARAM_CLAVE_LOTE_ANALIZADO) == 0 && i + 1 < argc && cfg->cantidadLotes < MAXIMO_LOTES)
            *(cfg->lotes + cfg->cantidadLotes++) = *(argv + ++i);
        else if (strcmp(*(argv + i), PARAM_CLAVE_CAPACIDAD) == 0 && i + 1 < argc && cfg->cantidadCapacidades < MAXIMO_CAPACIDADES)
            *(cfg->capacidades + cfg->cantidadCapacidades++) = (size_t) strtoul(*(argv + ++i), NULL, 10);
        else if (strcmp(*(argv + i), PARAM_CLAVE_ARCH_POSTEOS) == 0 && i + 1 < argc)
            cfg->pathPosteos = *(argv + ++i);
        else if (strcmp(*(argv + i), PARAM_CLAVE_SALIDA) == 0 && i + 1 < argc)
            cfg->pathCsv = *(argv + ++i);
        else
        {
            mostrarUso(*argv);
            return ERROR_OPERACION_INVALIDA;
        }
    }

    if (cfg->cantidadLotes == 0)
    {
        mostrarUso(*argv);
        return ERROR_OPERACION_INVALIDA;
    }

    if (cfg->cantidadCapacidades == 0)
        for (i = 0; i < (int) CANTIDAD(capacidadesPorDefecto); i++)
            *(cfg->capacidades + cfg->cantidadCapacidades++) = *(capacidadesPorDefecto + i);

    return EXITO;
}

/* Carga todo el lote en memoria
    Con las cantidades que manejamos y los requisitos, no nos importa el tamaño ocupado */
int cargarLote(const char *pathLote, tUsuario **lote, size_t *cantidad)
{
    FILE    *archivoUsuarios;
    long    tamanyo;

    archivoUsuarios = fopen(pathLote, "rb");
    if (!archivoUsuarios)
        return ERROR_OPERACION_INVALIDA;

    fseek(archivoUsuarios, 0, SEEK_END);
    tamanyo = ftell(archivoUsuarios);
    rewind(archivoUsuarios);

    if (tamanyo <= 0 || tamanyo % (long) sizeof(tUsuario) != 0)
    {
        fclose(archivoUsuarios);
        return ERROR_OPERACION_INVALIDA;
    }

    *cantidad = (size_t) tamanyo / sizeof(tUsuario);
    *lote = malloc((size_t) tamanyo);
    if (!*lote)
    {
        fclose(archivoUsuarios);
        return ERROR_SIN_MEMORIA;
    }

    if (fread(*lote, sizeof(tUsuario), *cantidad, archivoUsuarios) != *cantidad)
    {
        free(*lote);
        *lote = NULL;
        fclose(archivoUsuarios);
        return ERROR_OPERACION_INVALIDA;
    }

    fclose(archivoUsuarios);

    return EXITO;
}

/* Procesa a todos los usuarios, pidiendo cada posteo de su feed. Lo unico que se cronometra es ese bucle */
int ejecutarSimulacion(const tUsuario *lote, size_t cantidad, FILE *archivoPosteos, size_t capacidad, tResultado *resultado)
{
    tProveedor  proveedor;
    tPost       posteo;
    clock_t     inicio, fin;
    size_t      u;
    int         i, estado;

    if (proveedor_crear(&proveedor, archivoPosteos, capacidad) != EXITO)
        return ERROR_OPERACION_INVALIDA;

    estado = EXITO;
    inicio = clock();

    for (u = 0; u < cantidad && estado == EXITO; u++)
        for (i = 0; i < TAMANYO_FEED && estado == EXITO; i++)
            estado = proveedor_obtener(&proveedor, *((lote + u)->feed + i), &posteo);

    fin = clock();

    resultado->modo = proveedor_modo();
    resultado->capacidad = capacidad;
    resultado->usuarios = cantidad;
    resultado->accesos = cantidad * TAMANYO_FEED;
    resultado->aciertos = proveedor.aciertos;
    resultado->fallos = proveedor.fallos;
    resultado->lecturas = proveedor.lecturas;
    resultado->segundosTotal = (double) (fin - inicio) / CLOCKS_PER_SEC; /* time.h, pasaje a segundos */
    resultado->microsPorPosteo = resultado->segundosTotal * MICROSEGUNDOS_POR_SEGUNDO / (double) resultado->accesos;
    resultado->microsPorUsuario = resultado->segundosTotal * MICROSEGUNDOS_POR_SEGUNDO / (double) cantidad;
    resultado->alturaArbol = proveedor_altura(&proveedor);
    resultado->alturaMinima = proveedor_alturaMinima(&proveedor);

    proveedor_destruir(&proveedor);

    return estado;
}

void publicarFila(const tConfig *cfg, const char *marca, const char *pathLote, const tResultado *resultado, double segundosBase)
{
    reporte_fila(resultado, segundosBase);

    if (cfg->guardarCsv)
        reporte_csv(cfg->pathCsv, marca, pathLote, resultado, segundosBase);
}

/* Un lote contra la linea base y contra cada capacidad pedida */
int procesarLote(const tConfig *cfg, FILE *archivoPosteos, const char *marca, const char *pathLote)
{
    tUsuario            *lote;
    size_t              cantidad;
    tEstadisticasLote   estadisticas;
    tResultado          base, resultado; /* Se debe persistir el resultado del procesamiento base (sin caché) porque es a partir de la cual se miden las mejoras o empeoramientos de rendimiento */
    int                 c, estado;

    if (cargarLote(pathLote, &lote, &cantidad) != EXITO)
    {
        fprintf(stderr, "No se pudo cargar el lote %s (inexistente, vacio o de otro formato)\n", pathLote);
        return ERROR_OPERACION_INVALIDA;
    }

    /* distribucion_estadisticas realiza validaciones generales (principalmente el rango de ids) sobre el lote, retornando no EXITO en caso de no calificar */
    if (distribucion_estadisticas(lote, cantidad, &estadisticas) != EXITO)
    {
        fprintf(stderr, "El lote %s no es valido (ids fuera de rango o sin memoria)\n", pathLote);
        free(lote);
        return ERROR_OPERACION_INVALIDA;
    }

    reporte_encabezadoLote(pathLote, &estadisticas, cantidad, proveedor_modo());
    reporte_titulos();

    /* Sin esta pasada, que no se mide, las mejoras_relativas de las caches serían erróneas, porque la primera lectura de posteos.dat es más penalizada que las siguientes */
    estado = ejecutarSimulacion(lote, cantidad, archivoPosteos, 0, &base);

    if (estado == EXITO)
        estado = ejecutarSimulacion(lote, cantidad, archivoPosteos, 0, &base);

    if (estado == EXITO)
        publicarFila(cfg, marca, pathLote, &base, base.segundosTotal);

    for (c = 0; c < cfg->cantidadCapacidades && estado == EXITO; c++)
    {
        estado = ejecutarSimulacion(lote, cantidad, archivoPosteos, *(cfg->capacidades + c), &resultado);

        if (estado == EXITO)
            publicarFila(cfg, marca, pathLote, &resultado, base.segundosTotal);
    }

    if (estado == EXITO)
        reporte_pie();
    else
        fprintf(stderr, "Fallo la lectura de %s: se aborta el lote %s\n", cfg->pathPosteos, pathLote);

    free(lote);

    return estado;
}

/*
 * SIMULADOR: simulacion del feed de Tweener con y sin LRU Cache
 *
 * Para cada lote (-l) mide el procesamiento completo de sus usuarios, pidiendo
 * todos los posteos de cada feed a posteos.dat, primero sin cache (linea base)
 * y luego con una cache de cada capacidad (-c). Los datos no se muestran ni se
 * guardan: solo se cronometra.
 * - Un ejecutable por parte (parte 1: lista simple, parte 2: lista doble + arbol):
 *   ambas exportan las mismas primitivas, asi que no pueden enlazarse juntas.
 *   El build target de la parte 2 define LRU_PARTE_2.
 * - Tiempos por posteo y por usuario: promedio a partir del tiempo total, porque
 *   un solo posteo puede ser menor que la resolucion de clock().
 * - Por cada corrida informa aciertos, fallos y lecturas al archivo de posteos
 *   (y la altura del arbol indice, en la parte 2).
 * - Cada fila tambien se agrega a un csv en result/ (-o), con una marca de tiempo
 *   comun a toda la ejecucion, el modo y el path del lote, para poder rastrear su origen.
 * - posteos.dat solo se abre en modo lectura.
*/

int main(int argc, char *argv[])
{
    tConfig cfg;
    FILE    *archivoPosteos;
    char    marca[REPORTE_TAMANYO_MARCA];
    int     l, lotesFallidos;

    if (leerArgumentos(argc, argv, &cfg) != EXITO)
        return 1;

    archivoPosteos = fopen(cfg.pathPosteos, "rb");
    if (!archivoPosteos)
    {
        fprintf(stderr, "No se pudo abrir %s\n", cfg.pathPosteos);
        return 1;
    }

    cfg.guardarCsv = reporte_csvPreparar(cfg.pathCsv);
    if (!cfg.guardarCsv)
        fprintf(stderr, "Aviso: no se puede escribir %s, los resultados solo se muestran por pantalla\n", cfg.pathCsv);

    reporte_marcaTiempo(marca);

    lotesFallidos = 0;

    for (l = 0; l < cfg.cantidadLotes; l++)
        if (procesarLote(&cfg, archivoPosteos, marca, *(cfg.lotes + l)) != EXITO)
            lotesFallidos++;

    fclose(archivoPosteos);

    return lotesFallidos > 0;
}