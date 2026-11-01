-el lote de pruebas de main corrobora los 4 casos de insercion:
1. hay espacio y no esta dup.
2. hay espacio y esta dup.
3. no hay espacio y no esta dup.
4. no hay espacio y esta dup.

-la funcion mostrar_lrucache() no la pide el tp pero la implemente para debugear lo que hace la cache. muestra al mas reciente arriba.

-defini al t_nodo como t_nodo_simple para que no entre en conflicto con el t_nodo de la lista doble ni del arbol, a los que diria de ponerles t_nodo_doble y t_nodo_arbol.

-para no hacer un #define EXITO 0, ERROR_SIN_MEM -1, ENCONTRO 1 o MINIMO(x,y) ((x)<(y) ? (x):(y)) en cada estructura de datos y que entre en conflicto, hice un archivo constantes.h que los contiene y se incluye en los demas.

# App (parte 3)

- Con la misma semilla se generan los mismos lotes, pero si se está en la misma plataforma. `rand()` no es portable.

- Separamos la capa de aplicación en tres programas independientes (generador de posteos, generador de lotes y simulador), y el simulador en dos variantes (parte 1, y parte 2). La Parte 3 incluye tareas de naturaleza distinta: generar datos de prueba (con semilla), y medir tiempos de procesamiento. La consigna habla de “el programa que genera los lotes” y de “ese mismo programa” al que se le agrega la caché primero con la Parte 1 y luego con la Parte 2.

- En cuanto al lote ordenado de usuarios, la consigna dice “los id de los feeds aparecen en orden creciente", se asume que se refiere a estrictamente creciente en todo el archivo global, no solo dentro de cada feed. Entonces, como por defecto hay 50000 posteos, no puede haber más de 2500 usuarios en este lote, para que no se repitan los posteos.

- Al igual que el generador de posts, el generador de usuarios se parametriza, con la cantidad de registros que se desea tener. La cantidad que dice la consigna de 200 puede no ser tan representativa de la degradación de performance (más que nada por el tema de la resolución de clock), entonces el valor por defecto lo fijamos en uno considerablemente más alto.

- Las ids de un mismo muro son todas distintas entre sí; no tiene sentido que un muro muestre los mismos posts más de una vez, aparte inflaría artificialmente los aciertos.

- "debe recibir una semilla [...] de manera que un mismo lote pueda volver a generarse”. Otro parámetro, que si se omite se usa time(NULL) y se imprime, igual que gen_posteos.