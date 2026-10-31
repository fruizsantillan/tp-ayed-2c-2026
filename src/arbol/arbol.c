#include "arbol.h"

void arbol_crear (t_arbol *pa)
{
    *pa = NULL;
}

t_nodo_arbol **arbol_buscar_nodo (t_arbol *pa, const void *clave, int cmp (const void *a, const void *b))
{
    int rc;

    while(*pa && (rc = cmp(clave, (*pa)->ref->clave)) != 0)
    {
        if (rc < 0)
            pa = &(*pa)->izq;
        else
            pa = &(*pa)->der;
    }

    if(*pa)
        return pa;

    return NULL;
}

/* ref es el nodo de la lista; se compara por ref->clave */
int arbol_insertar (t_arbol *pa, t_nodo_doble *ref, int cmp (const void *a, const void *b))
{
    int rc;
    t_nodo_arbol *nue;

    while(*pa)
    {
        if((rc = cmp(ref->clave, (*pa)->ref->clave)) < 0)
            pa = &(*pa)->izq;
        else if(rc > 0)
            pa = &(*pa)->der;
        else
            return ERROR_CLAVE_DUPLICADA;
    }

    nue = malloc(sizeof(t_nodo_arbol));
    if(!nue)
        return ERROR_SIN_MEMORIA;

    nue->ref = ref;
    nue->izq = NULL;
    nue->der = NULL;
    *pa = nue;

    return EXITO;
}

void arbol_eliminar (t_arbol *pa, const void *clave, int cmp (const void *a, const void *b))
{
    t_nodo_arbol **pos, **remp, *elim;

    pos = arbol_buscar_nodo(pa, clave, cmp);
    if(!pos)
        return;

    elim = *pos;
    if(!elim->izq)
        *pos = elim->der;
    else if(!elim->der)
        *pos = elim->izq;
    else
    {
        remp = &elim->izq;
        while((*remp)->der)
            remp = &(*remp)->der;
        elim->ref = (*remp)->ref;
        elim = *remp;
        *remp = elim->izq;
    }
    free(elim);
}

int arbol_altura (const t_arbol *pa)
{
    int hi, hd;

    if(*pa == NULL)
        return 0;

    hi = arbol_altura(&(*pa)->izq);
    hd = arbol_altura(&(*pa)->der);

    return (hi > hd ? hi : hd) + 1;
}

/* Solo libera nodos del árbol */
void arbol_vaciar (t_arbol *pa)
{
    if(!*pa)
        return;
    arbol_vaciar(&(*pa)->izq);
    arbol_vaciar(&(*pa)->der);
    free((*pa));
    *pa = NULL;
}
