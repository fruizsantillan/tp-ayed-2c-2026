#include "lista_simple.h"


void lista_simple_crear (t_lista_simple *pl)
{
    *pl = NULL;
}

int lista_simple_insertar_al_inicio (t_lista_simple *pl, const void *dato, size_t tamDato)
{
    t_nodo_simple *nuevoNodo;

    nuevoNodo = malloc (sizeof (t_nodo_simple));
    if (!nuevoNodo)
        return ERROR_SIN_MEMORIA;
    nuevoNodo->dato = malloc (tamDato);
    if (!nuevoNodo->dato)
    {
        free (nuevoNodo);
        return ERROR_SIN_MEMORIA;
    }

    nuevoNodo->tam = tamDato;
    memcpy (nuevoNodo->dato, dato, tamDato);
    nuevoNodo->sig = *pl;
    *pl = nuevoNodo;

    return EXITO;
}

int lista_simple_buscar_elemento_y_moverlo_al_inicio (t_lista_simple *pl, const void *clave, void *dato, size_t tamDato, int cmp (const void *a, const void *b))
{
    t_nodo_simple **auxInicio = pl, *nodoAMover;

    while (*pl != NULL)
    {
        if (cmp ((*pl)->dato, clave) == 0)
        {
            if (auxInicio != pl) // Si el elemento que se busca no esta primero.
            {
                nodoAMover = *pl;
                *pl = nodoAMover->sig;
                nodoAMover->sig = *auxInicio;
                *auxInicio = nodoAMover;
            }
            memcpy (dato, (*auxInicio)->dato, MINIMO(tamDato, (*auxInicio)->tam));
            return ENCONTRO;
        }
        pl = &((*pl)->sig);
    }

    return NO_ENCONTRO;
}

void lista_simple_map (t_lista_simple *pl, void accion (void *dato))
{
    while (*pl != NULL)
    {
        accion ((*pl)->dato);
        pl = &((*pl)->sig);
    }
}

int lista_simple_esta_llena (t_lista_simple *pl, const void *dato, size_t tamDato)
{
    t_nodo_simple *nuevoNodo;

    nuevoNodo = malloc (sizeof (t_nodo_simple));
    if (!nuevoNodo)
        return 1;
    nuevoNodo->dato = malloc (tamDato);
    if (!nuevoNodo->dato)
    {
        free (nuevoNodo);
        return 1;
    }

    free (nuevoNodo->dato);
    free (nuevoNodo);

    return 0;
}

int lista_simple_eliminar_elemento_con_clave (t_lista_simple *pl, const void *clave, int cmp (const void *a, const void *b))
{
    t_nodo_simple *nodoElim;

    while (*pl != NULL)
    {
        if (cmp ((*pl)->dato, clave) == 0)
        {
            nodoElim = *pl;
            *pl = nodoElim->sig;
            free (nodoElim->dato);
            free (nodoElim);
            return ENCONTRO;
        }
        pl = &((*pl)->sig);
    }

    return NO_ENCONTRO;
}

void lista_simple_eliminar_ultimo_elemento (t_lista_simple *pl)
{
    if (*pl == NULL)
        return;

    while ((*pl)->sig != NULL)
        pl = &((*pl)->sig);

    free ((*pl)->dato);
    free (*pl);
    *pl = NULL;
}

void lista_simple_vaciar (t_lista_simple *pl)
{
    t_nodo_simple *nodoElim;

    while (*pl != NULL)
    {
        nodoElim = *pl;
        *pl = nodoElim->sig;
        free (nodoElim->dato);
        free (nodoElim);
    }
}















