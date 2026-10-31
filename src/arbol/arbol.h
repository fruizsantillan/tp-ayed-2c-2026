#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdlib.h>
#include "../../include/constantes/constantes.h"
#include "../lista_doble/lista_doble.h"

typedef struct s_nodo_arbol
{
    t_nodo_doble *ref;    /* NO es dueño del dato: solo apunta a la lista */
    struct s_nodo_arbol *izq, *der;
} t_nodo_arbol;

typedef t_nodo_arbol *t_arbol;

void arbol_crear (t_arbol *pa);

/* Devuelve DÓNDE está el nodo (el puntero que apunta a él), o NULL si no está */
t_nodo_arbol **arbol_buscar_nodo (t_arbol *pa, const void *clave, int cmp (const void *a, const void *b));

/* ref es el nodo de la lista; se compara por ref->clave */
int arbol_insertar (t_arbol *pa, t_nodo_doble *ref, int cmp (const void *a, const void *b));

void arbol_eliminar (t_arbol *pa, const void *clave, int cmp (const void *a, const void *b));

int arbol_altura (const t_arbol *pa);

/* Solo libera nodos del árbol */
void arbol_vaciar (t_arbol *pa);

#endif // ARBOL_H_INCLUDED
