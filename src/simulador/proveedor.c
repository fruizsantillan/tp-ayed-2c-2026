#include "proveedor.h"

/* La unica diferencia de uso entre ambas partes: la parte 2 recibe tambien el tamanyo de la clave */
#ifdef LRU_PARTE_2
#define PROVEEDOR_AGREGAR(cache, clave, dato) \
    agregar_lrucache((cache), (clave), sizeof(int), (dato), sizeof(tPost))
#else
#define PROVEEDOR_AGREGAR(cache, clave, dato) \
    agregar_lrucache((cache), (clave), (dato), sizeof(tPost))
#endif

int __proveedor_compararIds(const void *a, const void *b)
{
    int idA, idB;

    idA = *(const int *) a;
    idB = *(const int *) b;

    return (idA > idB) - (idA < idB);
}

int proveedor_crear(tProveedor *proveedor, FILE *archivoPosteos, size_t capacidad)
{
    if (!archivoPosteos)
        return ERROR_OPERACION_INVALIDA;

    proveedor->archivoPosteos = archivoPosteos;
    proveedor->usaCache = capacidad > 0 ? 1 : 0;
    proveedor->aciertos = 0;
    proveedor->fallos = 0;
    proveedor->lecturas = 0;

    if (proveedor->usaCache)
        crear_lrucache(&proveedor->cache, (unsigned) capacidad, __proveedor_compararIds);

    return EXITO;
}

/*  Un acierto se detecta por el contenido: se marca destino->id con -1 (ningun
    id valido lo es) y, si la cache lo completo con el id pedido, fue acierto. */

int proveedor_obtener(tProveedor *proveedor, int id, tPost *destino)
{
    if (proveedor->usaCache)
    {
        destino->id = -1;
        obtener_lrucache(&proveedor->cache, &id, destino, sizeof(tPost));

        if (destino->id == id)
        {
            proveedor->aciertos++;
            return EXITO;
        }

        proveedor->fallos++;
    }

    proveedor->lecturas++;

    if (!posteo_leer(proveedor->archivoPosteos, id, destino))
        return ERROR_OPERACION_INVALIDA;

    if (proveedor->usaCache)
        PROVEEDOR_AGREGAR(&proveedor->cache, &id, destino);

    return EXITO;
}

int proveedor_altura(const tProveedor *proveedor)
{
#ifdef LRU_PARTE_2
    if (proveedor->usaCache)
        return altura_indice_lrucache(&proveedor->cache);
#else
    (void) proveedor; /* Es una sentencia exclusivamente para evitar warnings */
#endif

    return -1;
}

/* El happy path del árbole balanceado completamente */
int proveedor_alturaMinima(const tProveedor *proveedor)
{
#ifdef LRU_PARTE_2
    unsigned    nodos;
    int         altura;

    if (!proveedor->usaCache)
        return -1;

    nodos = proveedor->cache.datosIngresados;
    altura = 0;

    while (nodos > 1)
    {
        nodos /= 2;
        altura++;
    }

    return altura + ARBOL_ALTURA_HOJA;
#else
    (void) proveedor; /* Ídem a func anterior */
    return -1;
#endif
}

void proveedor_destruir(tProveedor *proveedor)
{
    if (proveedor->usaCache)
    {
        vaciar_lrucache(&proveedor->cache);
        proveedor->usaCache = 0;
    }
}

const char *proveedor_modo(void)
{
#ifdef LRU_PARTE_2
    return "lru parte 2";
#else
    return "lru parte 1";
#endif
}