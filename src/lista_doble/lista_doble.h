#ifndef LISTA_DOBLE_H_INCLUDED
#define LISTA_DOBLE_H_INCLUDED

#include <stdlib.h>
#include <string.h>

typedef struct s_nodo_doble
{
    void *clave;
    size_t tamClave;
    void *dato;
    size_t tamDato;
    struct s_nodo_doble *ant, *sig;
} t_nodo_doble;

typedef struct
{
    t_nodo_doble *pri;   /* mas reciente */
    t_nodo_doble *ult;   /* menos usado  */
} t_lista_doble;

void lista_doble_crear (t_lista_doble *l);

t_nodo_doble *lista_doble_insertar_al_inicio (t_lista_doble *l, const void *clave, size_t tamClave, const void *dato, size_t tamDato);

void lista_doble_mover_al_inicio (t_lista_doble *l, t_nodo_doble *nodo);

void lista_doble_eliminar_nodo (t_lista_doble *l, t_nodo_doble *nodo);

void lista_doble_vaciar (t_lista_doble *l);

#endif // LISTA_DOBLE_H_INCLUDED
