#include "lru_cache.h"


void crear_lrucache (t_lru_cache *lruCache, unsigned capacidad, int cmp (const void *a, const void *b))
{
    lista_doble_crear (&(lruCache->lista));
    arbol_crear (&(lruCache->indice));
    lruCache->capacidad = capacidad;
    lruCache->datosIngresados = 0;
    lruCache->cmp = cmp;
}

int agregar_lrucache (t_lru_cache *lruCache, const void *clave, size_t tamClave, const void *dato, size_t tamDato)
{
    t_nodo_arbol **pos;
    t_nodo_doble *nodo, *victima;
    void *nuevoDato;

    if (lruCache->capacidad == 0)
        return ERROR_OPERACION_INVALIDA;

    pos = arbol_buscar_nodo (&(lruCache->indice), clave, lruCache->cmp);

    if (pos) /* La clave ya existia: se reemplaza el dato y pasa al principio. */
    {
        nodo = (*pos)->ref;

        nuevoDato = malloc (tamDato);
        if (!nuevoDato)
            return ERROR_SIN_MEMORIA;

        memcpy (nuevoDato, dato, tamDato);
        free (nodo->dato);
        nodo->dato = nuevoDato;
        nodo->tamDato = tamDato;

        lista_doble_mover_al_inicio (&(lruCache->lista), nodo);
        return EXITO;
    }

    if (lruCache->datosIngresados == lruCache->capacidad) /* Desborda: se descarta el menos usado. */
    {
        victima = lruCache->lista.ult;

        /* Primero del arbol, porque necesita la clave que vive en la lista. */
        arbol_eliminar (&(lruCache->indice), victima->clave, lruCache->cmp);
        lista_doble_eliminar_nodo (&(lruCache->lista), victima);
        lruCache->datosIngresados --;
    }

    nodo = lista_doble_insertar_al_inicio (&(lruCache->lista), clave, tamClave, dato, tamDato);
    if (!nodo)
        return ERROR_SIN_MEMORIA;

    if (arbol_insertar (&(lruCache->indice), nodo, lruCache->cmp) != EXITO)
    {
        /* Para no dejar las dos estructuras inconsistentes. */
        lista_doble_eliminar_nodo (&(lruCache->lista), nodo);
        return ERROR_SIN_MEMORIA;
    }

    lruCache->datosIngresados ++;

    return EXITO;
}

int obtener_lrucache (t_lru_cache *lruCache, const void *clave, void *dato, size_t tamDato)
{
    t_nodo_arbol **pos;
    t_nodo_doble *nodo;

    pos = arbol_buscar_nodo (&(lruCache->indice), clave, lruCache->cmp);
    if (!pos)
        return NO_ENCONTRO;

    nodo = (*pos)->ref;
    memcpy (dato, nodo->dato, MINIMO (tamDato, nodo->tamDato));
    lista_doble_mover_al_inicio (&(lruCache->lista), nodo);

    return ENCONTRO;
}

void borrar_lrucache (t_lru_cache *lruCache, const void *clave)
{
    t_nodo_arbol **pos;
    t_nodo_doble *nodo;

    pos = arbol_buscar_nodo (&(lruCache->indice), clave, lruCache->cmp);
    if (!pos)
        return;

    nodo = (*pos)->ref; /* Se guarda antes de tocar el arbol. */

    arbol_eliminar (&(lruCache->indice), clave, lruCache->cmp);
    lista_doble_eliminar_nodo (&(lruCache->lista), nodo);
    lruCache->datosIngresados --;
}

void vaciar_lrucache (t_lru_cache *lruCache)
{
    arbol_vaciar (&(lruCache->indice)); /* Solo libera los nodos del arbol. */
    lista_doble_vaciar (&(lruCache->lista)); /* Aca se libera clave, dato y nodo. */
    lruCache->datosIngresados = 0;
}

int altura_indice_lrucache (const t_lru_cache *lruCache)
{
    return arbol_altura (&(lruCache->indice));
}

void mostrar_lrucache (t_lru_cache *lruCache, void mostrar (void *dato))
{
    t_nodo_doble *nodo;

    printf ("Cantidad de datos ingresados: %u\n", lruCache->datosIngresados);

    for (nodo = lruCache->lista.pri; nodo; nodo = nodo->sig)
        mostrar (nodo->dato);

    puts("");
}
