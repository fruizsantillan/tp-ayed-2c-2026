#include "vocabulario.h"

#include "../comun/aleatorio.h"
#include "../../../include/constantes/constantes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const tNucleo sujetos[] =
{
    {"El", "perro"},
    {"La", "computadora"},
    {"Un", "robot"},
    {"Una", "idea"},
    {"El", "programador"},
    {"La", "estudiante"},
    {"El", "profesor"},
    {"Una", "máquina"},
    {"Un", "algoritmo"},
    {"La", "aplicación"},
    {"El", "sistema"},
    {"Una", "persona"},
    {"Un", "científico"},
    {"La", "ingeniera"},
    {"El", "usuario"},
    {"Una", "empresa"},
    {"Un", "servidor"},
    {"La", "base de datos"},
    {"El", "navegador"},
    {"Una", "variable"},
    {"Un", "proceso"},
    {"La", "inteligencia artificial"},
    {"El", "dispositivo"},
    {"Una", "red neuronal"},
    {"Un", "modelo"},
    {"La", "tecnología"},
    {"El", "compilador"},
    {"Una", "función"},
    {"Un", "estudiante"},
    {"La", "investigadora"},
    {"El", "gato"},
    {"Una", "bicicleta"},
    {"Un", "automóvil"},
    {"La", "ciudad"},
    {"El", "libro"},
    {"Una", "película"},
    {"Un", "músico"},
    {"La", "artista"},
    {"El", "arquitecto"},
    {"Una", "decisión"},
    {"Un", "proyecto"},
    {"La", "organización"},
    {"El", "equipo"},
    {"Una", "solución"},
    {"Un", "problema"},
    {"La", "información"},
    {"El", "resultado"},
    {"Una", "herramienta"},
    {"Un", "lenguaje"},
    {"La", "memoria"}
};

const tNucleo objetos[] =
{
    {"la", "montaña"},
    {"el", "algoritmo"},
    {"una", "respuesta"},
    {"un", "desafío"},
    {"la", "tecnología"},
    {"el", "problema"},
    {"una", "solución"},
    {"un", "programa"},
    {"la", "información"},
    {"el", "código"},
    {"una", "aplicación"},
    {"un", "proyecto"},
    {"la", "base de datos"},
    {"el", "archivo"},
    {"una", "variable"},
    {"un", "resultado"},
    {"la", "función"},
    {"el", "sistema"},
    {"una", "herramienta"},
    {"un", "lenguaje"},
    {"la", "interfaz"},
    {"el", "servidor"},
    {"una", "consulta"},
    {"un", "mensaje"},
    {"la", "documentación"},
    {"el", "modelo"},
    {"una", "hipótesis"},
    {"un", "experimento"},
    {"la", "memoria"},
    {"el", "dispositivo"},
    {"una", "imagen"},
    {"un", "documento"},
    {"la", "película"},
    {"el", "libro"},
    {"una", "canción"},
    {"un", "edificio"},
    {"la", "ciudad"},
    {"el", "automóvil"},
    {"una", "bicicleta"},
    {"un", "camino"},
    {"la", "puerta"},
    {"el", "puente"},
    {"una", "decisión"},
    {"un", "objetivo"},
    {"la", "estrategia"},
    {"el", "conocimiento"},
    {"una", "pregunta"},
    {"un", "concepto"},
    {"la", "realidad"},
    {"el", "futuro"}
};

const char *verbos[] =
{
    "desafía",
    "inspira",
    "transforma",
    "sorprende",
    "conquista",
    "analiza",
    "descubre",
    "construye",
    "desarrolla",
    "interpreta",
    "resuelve",
    "modifica",
    "mejora",
    "diseña",
    "programa",
    "investiga",
    "explora",
    "comprende",
    "cuestiona",
    "observa",
    "evalúa",
    "procesa",
    "genera",
    "organiza",
    "compara",
    "selecciona",
    "implementa",
    "optimiza",
    "explica",
    "presenta",
    "utiliza",
    "controla",
    "protege",
    "actualiza",
    "reinventa",
    "imagina",
    "encuentra",
    "recorre",
    "atraviesa",
    "contempla",
    "alcanza",
    "supera",
    "recuerda",
    "cuestiona",
    "reconoce",
    "representa",
    "simplifica",
    "reemplaza",
    "conecta",
    "revoluciona"
};

const char *cierres[] =
{
    "sin dudarlo",
    "en tiempo récord",
    "con mucho estilo",
    "cada mañana",
    "contra todo pronóstico",
    "antes del amanecer",
    "al final del día",
    "con gran precisión",
    "sin perder tiempo",
    "de manera inesperada",
    "con total confianza",
    "en cuestión de segundos",
    "con mucha paciencia",
    "sin cometer errores",
    "bajo la lluvia",
    "durante la noche",
    "frente a todos",
    "desde la distancia",
    "con una sonrisa",
    "por primera vez",
    "una vez más",
    "sin mirar atrás",
    "con enorme entusiasmo",
    "de forma automática",
    "en el momento indicado",
    "cuando nadie lo esperaba",
    "con resultados sorprendentes",
    "en medio del caos",
    "con ayuda de la ciencia",
    "gracias a la tecnología",
    "siguiendo su intuición",
    "con una estrategia brillante",
    "a una velocidad increíble",
    "más allá de sus límites",
    "en completo silencio",
    "con absoluta determinación",
    "para cambiar el futuro",
    "sin ninguna explicación",
    "delante de la multitud",
    "en un instante"
};

const char *tags[] =
{
    "programacion",
    "tecnologia",
    "innovacion",
    "reflexion",
    "random",
    "ciencia",
    "informatica",
    "inteligenciaArtificial",
    "desarrollo",
    "software",
    "algoritmos",
    "datos",
    "aprendizaje",
    "creatividad",
    "futuro",
    "robotica",
    "automatizacion",
    "investigacion",
    "educacion",
    "conocimiento",
    "curiosidad",
    "inspiracion",
    "motivacion",
    "productividad",
    "ingenieria",
    "estadistica",
    "matematica",
    "codigo",
    "sistemas",
    "internet",
    "ciberseguridad",
    "soluciones",
    "proyectos",
    "ideas",
    "descubrimiento",
    "experimentos",
    "pensamiento",
    "actualidad",
    "mundoDigital",
    "vidaCotidiana"
};

/* Función interna para elegir un núcleo al azar */

const tNucleo *__vocabulario_elegirNucleo(const tNucleo *opciones, size_t cantidadOpciones)
{
    size_t posicionAleatoria;

    posicionAleatoria = aleatorio_hasta(cantidadOpciones);

    return opciones + posicionAleatoria;
}

void vocabulario_armarTexto(char *buffer, size_t longitudMaxima)
{
    const tNucleo   *sujeto,
                    *objeto;
    const char      *verbo,
                    *cierre,
                    *tag;
    size_t          longitud;

    do
    {
        sujeto = __vocabulario_elegirNucleo(sujetos, CANTIDAD(sujetos));
        objeto = __vocabulario_elegirNucleo(objetos, CANTIDAD(objetos));
        verbo  = verbos[aleatorio_hasta(CANTIDAD(verbos))];
        cierre = cierres[aleatorio_hasta(CANTIDAD(cierres))];
        tag    = tags[aleatorio_hasta(CANTIDAD(tags))];

        longitud = strlen(sujeto->articulo) + strlen(sujeto->sustantivo)
                 + strlen(verbo)
                 + strlen(objeto->articulo) + strlen(objeto->sustantivo)
                 + strlen(cierre) + strlen(tag)
                 + 5 + 3;
    }
    while (longitud >= longitudMaxima); // >= porque falta lugar para el '\0' y sprintf no lo verifica

    sprintf(buffer, "%s %s %s %s %s %s. #%s",
            sujeto->articulo, sujeto->sustantivo, verbo,
            objeto->articulo, objeto->sustantivo, cierre, tag);
}