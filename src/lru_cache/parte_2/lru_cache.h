#ifndef LRU_CACHE_H_INCLUDED
#define LRU_CACHE_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/constantes/constantes.h"
#include "../../lista_doble/lista_doble.h"
#include "../../arbol/arbol.h"


typedef struct
{
    t_lista_doble lista;    /* ordena por uso y es duena de la clave y del dato */
    t_arbol indice;         /* ordena por clave y solo apunta a la lista */
    unsigned capacidad;
    unsigned datosIngresados;
    int (*cmp)(const void*, const void*);   /* compara clave contra clave */
} t_lru_cache;


void crear_lrucache (t_lru_cache *lruCache, unsigned capacidad, int cmp (const void *a, const void *b));

int agregar_lrucache (t_lru_cache *lruCache, const void *clave, size_t tamClave, const void *dato, size_t tamDato);

int obtener_lrucache (t_lru_cache *lruCache, const void *clave, void *dato, size_t tamDato);

void borrar_lrucache (t_lru_cache *lruCache, const void *clave);

void vaciar_lrucache (t_lru_cache *lruCache);

/* Altura del arbol indice */
int altura_indice_lrucache (const t_lru_cache *lruCache);

/* Usada para debug */
void mostrar_lrucache (t_lru_cache *lruCache, void mostrar (void *dato));


#endif /* LRU_CACHE_H_INCLUDED */
