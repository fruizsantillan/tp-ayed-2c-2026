#ifndef LRU_CACHE_H_INCLUDED
#define LRU_CACHE_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/constantes/constantes.h"

#include "../../lista_simple/lista_simple.h"


typedef struct
{
    t_lista_simple datos;
    unsigned capacidad;
    unsigned datosIngresados;
    int (*cmp)(const void*, const void*);
} t_lru_cache;


void crear_lrucache (t_lru_cache *lruCache, unsigned capacidad, int cmp (const void *a, const void *b));

int agregar_lrucache (t_lru_cache *lruCache, const void *clave, const void *dato, size_t tamDato);

int obtener_lrucache (t_lru_cache *lruCache, const void *clave, void *dato, size_t tamDato);

void borrar_lrucache (t_lru_cache *lruCache, const void *clave);

void vaciar_lrucache (t_lru_cache *lruCache);

/// Usada para debug
void mostrar_lrucache (t_lru_cache *lruCache, void mostrar (void *dato));


#endif // LRU_CACHE_H_INCLUDED
