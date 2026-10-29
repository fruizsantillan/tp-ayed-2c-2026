#include "lru_cache.h"


void crear_lrucache (t_lru_cache *lruCache, unsigned capacidad, int cmp (const void *a, const void *b))
{
    lista_simple_crear (&(lruCache->datos));
    lruCache->capacidad = capacidad;
    lruCache->datosIngresados = 0;
    lruCache->cmp = cmp;
}

int agregar_lrucache (t_lru_cache *lruCache, const void *clave, const void *dato, size_t tamDato)
{
    if (lruCache->capacidad <= 0) // Por seguridad de mal uso del usuario.
        return ERROR_OPERACION_INVALIDA;

    if (lista_simple_esta_llena (&(lruCache->datos), dato, tamDato))
        return ERROR_SIN_MEMORIA;

    if (lista_simple_eliminar_elemento_con_clave (&(lruCache->datos), clave, lruCache->cmp) == NO_ENCONTRO)
    {
        if (lruCache->datosIngresados < lruCache->capacidad)
            lruCache->datosIngresados ++;
        else
            lista_simple_eliminar_ultimo_elemento (&(lruCache->datos));
    }

    lista_simple_insertar_al_inicio (&(lruCache->datos), dato, tamDato);

    return EXITO;
}

int obtener_lrucache (t_lru_cache *lruCache, const void *clave, void *dato, size_t tamDato)
{
    return lista_simple_buscar_elemento_y_moverlo_al_inicio (&(lruCache->datos), clave, dato, tamDato, lruCache->cmp);
}

void borrar_lrucache (t_lru_cache *lruCache, const void *clave)
{
    if (lista_simple_eliminar_elemento_con_clave (&(lruCache->datos), clave, lruCache->cmp) == ENCONTRO)
        lruCache->datosIngresados --;
}

void vaciar_lrucache (t_lru_cache *lruCache)
{
    lista_simple_vaciar (&(lruCache->datos));
    lruCache->datosIngresados = 0;
}

void mostrar_lrucache (t_lru_cache *lruCache, void mostrar (void *dato))
{
    printf ("Cantidad de datos ingresados: %u\n", lruCache->datosIngresados);
    lista_simple_map (&(lruCache->datos), mostrar);
    puts("");
}















