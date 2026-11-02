#ifndef PROVEEDOR_H_INCLUDED
#define PROVEEDOR_H_INCLUDED

#include <stdio.h>
#include <stddef.h>

#ifdef LRU_PARTE_2
#include "../lru_cache/parte_2/lru_cache.h"
#else
#include "../lru_cache/parte_1/lru_cache.h"
#endif
#include "../app/comun/posteo.h"
#include "../../include/constantes/constantes.h"

/* El simulador pide un posteo por id y se decide si el proveedor lo sirve desde la
   cache o desde el archivo, llevando la cuenta de aciertos, fallos y lecturas. */

#define ARBOL_ALTURA_HOJA 1

typedef struct
{
    FILE            *archivoPosteos;
    t_lru_cache     cache;
    int             usaCache;
    unsigned long   aciertos,
                    fallos,
                    lecturas;
} tProveedor;

/* Capacidad 0 deja al proveedor sin cache: todo se lee del archivo */
int proveedor_crear           (tProveedor *proveedor, FILE *archivoPosteos, size_t capacidad);

/* Entrega el posteo de ese id en destino. Retorna EXITO o ERROR_OPERACION_INVALIDA si fallo la lectura del archivo */
int proveedor_obtener         (tProveedor *proveedor, int id, tPost *destino);

/* Altura del arbol indice (solo parte 2 y con cache), -1 en cualquier otro caso */
int proveedor_altura          (const tProveedor *proveedor);

/* Altura minima que podria tener un arbol con la cantidad de nodos actual, -1 si no aplica */
int proveedor_alturaMinima    (const tProveedor *proveedor);

/* Libera la cache. Se puede llamar mas de una vez */
void proveedor_destruir       (tProveedor *proveedor);

/* "lru parte 1" o "lru parte 2", segun el build target */
const char *proveedor_modo    (void);

#endif /* PROVEEDOR_H_INCLUDED */