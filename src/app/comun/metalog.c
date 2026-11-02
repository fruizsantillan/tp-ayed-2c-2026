#include "metalog.h"

#include <time.h>

FILE *metalog_abrir(const char *clase, const char *path, unsigned semilla)
{
    FILE    *log;
    char    marca[METALOG_TAMANYO_FECHA];
    time_t  ahora; /* time_t se define en time.h (en realidad en crtdefs.h) */

    log = fopen(PATH_METALOG, "a"); /* Append en texto */
    if (!log)
        return NULL;

    ahora = time(NULL);
    strftime(marca, METALOG_TAMANYO_FECHA, "%Y-%m-%d %H:%M:%S", localtime(&ahora));
    fprintf(log, "%s | %s: %s | semilla: %u", marca, clase, path, semilla);

    return log;
}

void metalog_cerrar(FILE *log)
{
    fprintf(log, "\n");
    fclose(log);
}