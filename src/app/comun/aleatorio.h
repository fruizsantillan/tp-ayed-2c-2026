#ifndef ALEATORIO_H_INCLUDED
#define ALEATORIO_H_INCLUDED

/* Inicializa la secuencia. Misma semilla, mismo programa y misma plataforma
   producen la misma secuencia. */
void     aleatorio_semilla(unsigned semilla);

// Entero uniforme más chico que n
unsigned aleatorio_hasta(unsigned n);

/* Entero uniforme en [minimo, maximo], ambos inclusive. */
int      aleatorio_entre(int minimo, int maximo);

/* Devuelve 1 con probabilidad porcentaje/100 (0..100), 0 en otro caso. */
int      aleatorio_probabilidad(unsigned porcentaje);

#endif // ALEATORIO_H_INCLUDED