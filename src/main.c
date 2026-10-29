#include "lru_cache/parte_1/lru_cache.h"


typedef struct
{
    int id;
    char nombre [11];
    float sueldo;
} t_empleado;

int cmp_empleado (const void *a, const void *b);
void mostrar_empleado (void *dato);


int main()
{
    t_lru_cache lruCache;
    t_empleado vec [] =
    {
        {5, "nacho", 10.25},
        {2, "ara", 6.76},
        {2, "ara", 12.32},
        {8, "rama", 5.61},
        {1, "ale", 3.10},
        {4, "tomi", 9.99},
        {9, "uri", 1.32},
        {1, "ale", 4.10}
    };
    t_empleado *pVec = vec, datoObtenido;
    int i, clave;

    crear_lrucache (&lruCache, 5, cmp_empleado);

    for (i = 0; i < sizeof (vec) / sizeof (t_empleado); i ++)
    {
        agregar_lrucache (&lruCache, &(pVec->id), pVec, sizeof (t_empleado));
        pVec ++;
        if (i == 4) // Muestra cuando inserta 5 elementos y deberia completar la capacidad de la cache.
            mostrar_lrucache (&lruCache, mostrar_empleado);
    }
    mostrar_lrucache (&lruCache, mostrar_empleado);

    clave = 2;
    if (obtener_lrucache (&lruCache, &clave, &datoObtenido, sizeof (t_empleado)) == ENCONTRO)
    {
        printf ("Dato en la cache | ");
        mostrar_empleado (&datoObtenido);
        puts("");
        mostrar_lrucache (&lruCache, mostrar_empleado);
    }

    clave = 4;
    borrar_lrucache (&lruCache, &clave);
    mostrar_lrucache (&lruCache, mostrar_empleado);

    vaciar_lrucache (&lruCache);
    mostrar_lrucache (&lruCache, mostrar_empleado);

    return EXITO;
}


int cmp_empleado (const void *a, const void *b)
{
    int dato = ((t_empleado*)a)->id;
    int clave = *((int*)b);

    if (dato > clave)
        return 1;
    else if (dato < clave)
        return -1;
    else
        return 0;
}

void mostrar_empleado (void *dato)
{
    t_empleado emp = *((t_empleado*)(dato));
    printf ("Id: %d\tNombre: %s\tSueldo: %.2f\n", emp.id, emp.nombre, emp.sueldo);
}
















