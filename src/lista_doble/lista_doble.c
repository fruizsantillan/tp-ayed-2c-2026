#include "lista_doble.h"

void lista_doble_crear (t_lista_doble *l) 
{
    l->pri = NULL;
    l->ult = NULL;    
}

t_nodo_doble *lista_doble_insertar_al_inicio (t_lista_doble *l, const void *clave, size_t tamClave, const void *dato, size_t tamDato)
{
    t_nodo_doble *nue;

    nue = malloc(sizeof(t_nodo_doble));
    if(!nue)
        return NULL;

    nue->dato = malloc(tamDato);
    if(!nue->dato)
    {
        free(nue);
        return NULL;
    }

    nue->clave = malloc(tamClave);
    if(!nue->clave) {
        free(nue->dato);
        free(nue);
        return NULL;
    }

    memcpy(nue->dato, dato, tamDato);
    memcpy(nue->clave, clave, tamClave);
    nue->tamDato = tamDato;
    nue->tamClave = tamClave;
    nue->ant = NULL;
    nue->sig = l->pri;

    if(l->pri)
        l->pri->ant = nue;
    else
        l->ult = nue;
    l->pri = nue;

    return nue;
}

void lista_doble_mover_al_inicio (t_lista_doble *l, t_nodo_doble *nodo)
{
    if(nodo == l->pri)
        return;

    nodo->ant->sig = nodo->sig;
    if(nodo->sig)
        nodo->sig->ant = nodo->ant;
    else
        l->ult = nodo->ant;

    nodo->ant = NULL;
    nodo->sig = l->pri;
    l->pri->ant = nodo;
    l->pri = nodo;
}

void lista_doble_eliminar_nodo (t_lista_doble *l, t_nodo_doble *nodo)
{
    if(nodo->ant)
        nodo->ant->sig = nodo->sig;
    else
        l->pri = nodo->sig;

    if(nodo->sig)
        nodo->sig->ant = nodo->ant;
    else
        l->ult = nodo->ant;

    free(nodo->clave);
    free(nodo->dato);
    free(nodo);
}

void lista_doble_vaciar (t_lista_doble *l)
{
    t_nodo_doble *aux;

    while(l->pri)
    {
        aux = l->pri;
        l->pri = aux->sig;
        free(aux->clave);
        free(aux->dato);
        free(aux);
    }
    
    l->ult = NULL;
}
