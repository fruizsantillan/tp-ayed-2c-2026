#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/constantes/constantes.h"


typedef struct s_nodo_simple
{
    void *dato;
    size_t tam;
    struct s_nodo_simple *sig;
} t_nodo_simple;

typedef t_nodo_simple *t_lista_simple;


void lista_simple_crear (t_lista_simple *pl);

int lista_simple_insertar_al_inicio (t_lista_simple *pl, const void *dato, size_t tamDato);

/// Busca el primer elemento de la lista que coincida con la clave y lo mueve al primer lugar.
int lista_simple_buscar_elemento_y_moverlo_al_inicio (t_lista_simple *pl, const void *clave, void *dato, size_t tamDato, int cmp (const void *a, const void *b));

/// Usada para debug
void lista_simple_map (t_lista_simple *pl, void accion (void *dato));

int lista_simple_esta_llena (t_lista_simple *pl, const void *dato, size_t tamDato);

/// Elimina el primer elemento de la lista que coincida con la clave.
int lista_simple_eliminar_elemento_con_clave (t_lista_simple *pl, const void *clave, int cmp (const void *a, const void *b));

void lista_simple_eliminar_ultimo_elemento (t_lista_simple *pl);

void lista_simple_vaciar (t_lista_simple *pl);


#endif // LISTA_SIMPLE_H_INCLUDED
