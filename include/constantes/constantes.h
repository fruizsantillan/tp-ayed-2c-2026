#ifndef CODIGOS_RETORNO_H_INCLUDED
#define CODIGOS_RETORNO_H_INCLUDED

/* Casi todas las cantidades salen directo de la consigna */

#define MAX_CARACTERES_POSTEO 140
#define CANTIDAD_REGISTROS_POSTEOS 50000

#define MAX_CARACTERES_NOMBRE_USUARIO 14
#define TAMANYO_FEED 20
#define MINIMO_USUARIOS 200

#define EXITO 0
#define ERROR_SIN_MEMORIA -1
#define ERROR_OPERACION_INVALIDA -2

#define ENCONTRO 1
#define NO_ENCONTRO 0

#define MINIMO(x,y) ((x)<(y) ? (x):(y))
#define CANTIDAD(v) (sizeof(v) / sizeof((v)[0]))

#endif // CODIGOS_RETORNO_H_INCLUDED
