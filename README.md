# TP AYED 2C 2026: LRU Cache

Trabajo práctico de Algoritmos y Estructuras de Datos (3640), UNLaM, 2C 2026.

- **Parte 1:** LRU Cache sobre lista simplemente enlazada.
- **Parte 2:** LRU Cache con lista doblemente enlazada + árbol binario de búsqueda como índice.
- **Parte 3:** simulación "Tweener" con lotes de prueba e informe.

Entrega y defensa: 02/11/2026.

---

## ¿Cómo usar?

### 0. Antes de empezar

- **Todos los comandos se ejecutan desde la raíz del repositorio.** Las rutas por defecto (`data/bin/posteos.dat`, `result/simulacion.csv`, etc.) son relativas a ese directorio.
- Estos directorios tienen que existir; en caso de faltar alguno, crearlo o el sistema va a enviar errores sobre la apertura de los archivos:

| Directorio | Para qué sirve |
|---|---|
| `data/bin/` | Binarios generados: `posteos.dat` y los `lote_*.dat` |
| `data/csv/` | CSV que producen los verificadores (para inspeccionar los `.dat`) |
| `data/meta/` | `log.txt`: registro de trazabilidad de cada archivo generado (fecha, tipo, ruta, semilla, cantidad de registros) |
| `result/` | `simulacion.csv`: resultados acumulados de las simulaciones |

### 1. Programas del proyecto

El proyecto no es un solo ejecutable, sino cinco programas independientes, y uno tiene dos partes; cada uno con su propio `main`. Cada programa se compila por separado, juntando sus archivos fuente:

| Programa | Qué hace | Archivos fuente que lo componen |
|---|---|---|
| `gen_posteos` | Genera `posteos.dat` (50000 posteos) | `src/app/gen_posteos/gen_posteos_main.c`, `src/app/gen_posteos/vocabulario.c`, `src/app/comun/aleatorio.c`, `src/app/comun/metalog.c` |
| `gen_lotes` | Genera un lote de usuarios (`lote_*.dat`) | `src/app/gen_lotes/gen_lotes_main.c`, `src/app/gen_lotes/distribuciones.c`, `src/app/comun/aleatorio.c`, `src/app/comun/metalog.c` |
| `verificador_posteos` | Vuelca `posteos.dat` a CSV y valida su formato | `src/app/verificador/verificador_posteos_main.c`, `src/app/verificador/verificador_comunes.c` |
| `verificador_usuarios` | Vuelca un lote de usuarios a CSV y valida su formato | `src/app/verificador/verificador_usuarios_main.c`, `src/app/verificador/verificador_comunes.c` |
| `simulador_parte1` | Simulación con la caché de la **Parte 1** | ver abajo |
| `simulador_parte2` | Simulación con la caché de la **Parte 2** | ver abajo y **definir la macro `LRU_PARTE_2`** |

Ver abajo: _archivos comunes a ambos simuladores_
`src/simulador/simulador_main.c`, `src/simulador/proveedor.c`, `src/simulador/reporte.c`, `src/app/comun/posteo.c`, `src/app/comun/aleatorio.c`, `src/app/gen_lotes/distribuciones.c`

- Para `simulador_parte1`, sumar: `src/lru_cache/parte_1/lru_cache.c` y `src/lista_simple/lista_simple.c`.
- Para `simulador_parte2`, sumar: `src/lru_cache/parte_2/lru_cache.c`, `src/lista_doble/lista_doble.c` y `src/arbol/arbol.c`.

> Las dos partes del TDA exportan las mismas primitivas (`crear_lrucache`, `agregar_lrucache`, etc.), por lo que no pueden enlazarse juntas en un mismo ejecutable. La macro `LRU_PARTE_2` le indica a `proveedor.c`/`proveedor.h` cuál de las dos implementaciones incluir (y ajusta la única diferencia de uso: la Parte 2 recibe también el tamaño de la clave).

### 2. Configurar el compilador (cualquier entorno de desarrollo)

Para cada programa hay que indicarle al compilador: la lista de archivos fuente de la tabla anterior, el nombre del ejecutable de salida y, solo para `simulador_parte2`, la definición de la macro `LRU_PARTE_2` (equivalente a `-DLRU_PARTE_2` en gcc).

Ejemplo con gcc, desde la raíz del repositorio:

```
gcc -Wall -o bin/gen_posteos src/app/gen_posteos/gen_posteos_main.c src/app/gen_posteos/vocabulario.c src/app/comun/aleatorio.c src/app/comun/metalog.c

gcc -Wall -o bin/gen_lotes src/app/gen_lotes/gen_lotes_main.c src/app/gen_lotes/distribuciones.c src/app/comun/aleatorio.c src/app/comun/metalog.c

gcc -Wall -o bin/verificador_posteos src/app/verificador/verificador_posteos_main.c src/app/verificador/verificador_comunes.c

gcc -Wall -o bin/verificador_usuarios src/app/verificador/verificador_usuarios_main.c src/app/verificador/verificador_comunes.c

gcc -Wall -o bin/simulador_parte1 src/simulador/simulador_main.c src/simulador/proveedor.c src/simulador/reporte.c src/app/comun/posteo.c src/app/comun/aleatorio.c src/app/gen_lotes/distribuciones.c src/lru_cache/parte_1/lru_cache.c src/lista_simple/lista_simple.c

gcc -Wall -DLRU_PARTE_2 -o bin/simulador_parte2 src/simulador/simulador_main.c src/simulador/proveedor.c src/simulador/reporte.c src/app/comun/posteo.c src/app/comun/aleatorio.c src/app/gen_lotes/distribuciones.c src/lru_cache/parte_2/lru_cache.c src/lista_doble/lista_doble.c src/arbol/arbol.c
```

> **Importante para medir tiempos:** compilar los simuladores con las **mismas opciones de optimización** en ambas partes (por ejemplo, ambos sin `-O` o ambos con `-O2`). Comparar un ejecutable optimizado contra uno que no lo está invalida la comparación Parte 1 vs. Parte 2.

### 3. Parámetros de cada programa

Todos los parámetros son opcionales salvo los marcados como *obligatorio*.

#### `gen_posteos`

| Parámetro | Descripción | Por defecto |
|---|---|---|
| `-s semilla` | Semilla del generador aleatorio (misma semilla = mismo archivo) | hora actual (se informa por pantalla) |
| `-o archivo` | Ruta del archivo de salida | `data/bin/posteos.dat` |

Cada registro es `{id: int, contenido: char[141]}` y el `id` coincide con la posición del registro en el archivo (así se accede con `fseek`). **El archivo debe generarse siempre con este programa**, o la búsqueda directa por `id` se rompe. Los textos se arman combinando al azar sujeto + verbo + objeto + cierre + hashtag (ver `vocabulario.c`).

#### `gen_lotes`

| Parámetro | Descripción | Por defecto |
|---|---|---|
| `-t tipo` | *Obligatorio.* Tipo de lote: `concentrado`, `disperso` u `ordenado` | — |
| `-s semilla` | Semilla del generador (misma semilla y parámetros = mismo lote) | hora actual (se informa) |
| `-o archivo` | Archivo de salida | `data/bin/lote_<tipo>.dat` |
| `-u usuarios` | Cantidad de usuarios (entre 200 y 999999; el lote `ordenado` admite como máximo 2500) | 2000 |
| `-c populares` | *Solo concentrado.* Cantidad de posteos "populares" (entre 20 y 50000) | 100 |
| `-p porcentaje` | *Solo concentrado.* Porcentaje (0 a 100) de posiciones del feed que se toman del conjunto de populares; el resto sale de todo el archivo | 90 |

**Los tres tipos de lote** (cada feed tiene 20 ids de posteos, sin repetidos dentro del mismo feed):

| Tipo | Qué representa | Efecto esperado en la caché |
|---|---|---|
| `concentrado` | Muchos usuarios ven los mismos posteos. Un conjunto de `-c` populares aporta el `-p`% de las posiciones. | Muchos aciertos si la capacidad se acerca a la cantidad de populares. |
| `disperso` | Los usuarios ven posteos mayormente distintos (uniforme sobre todo el archivo). | Pocos aciertos: la caché agrega costo y casi no ahorra lecturas. |
| `ordenado` | Los ids son **estrictamente crecientes en todo el lote**, no solo dentro de cada feed. | Casi sin aciertos, y en la Parte 2 el árbol índice se degenera (ver abajo). |

Al terminar imprime un resumen con los **accesos**, los **ids distintos** y el **techo de aciertos** (`1 - distintos/accesos`): lo máximo que cualquier (!) caché puede acertar con ese lote. Cada archivo generado se registra en `data/meta/log.txt`.

#### `verificador_posteos` y `verificador_usuarios`

Herramientas auxiliares: convierten un `.dat` a CSV (separador `;`, con BOM que espera que se abra en Excel, si se abre por ejemplo en InfoStat puede que esté con formato corrupto) y validan su formato (ids que coinciden con la posición, cadenas terminadas en `\0`, ids de feed en rango y sin repetidos). Retornan error si encuentran inconsistencias.

| Parámetro | Descripción | Por defecto |
|---|---|---|
| `-i entrada.dat` | Archivo a verificar (*obligatorio* en `verificador_usuarios`) | `data/bin/posteos.dat` (solo posteos) |
| `-o salida.csv` | CSV de salida | `data/csv/posteos_verificado.csv` / `data/csv/usuarios_verificado.csv` |
| `-n cantidad` | Volcar solo los primeros N registros | todos |

#### `simulador_parte1` / `simulador_parte2`

| Parámetro | Descripción | Por defecto |
|---|---|---|
| `-l lote.dat` | *Obligatorio.* Lote de usuarios a procesar. **Repetible** (hasta 8) | — |
| `-c capacidad` | Capacidad de la caché. **Repetible** (hasta 16) | `10 50 100 500 1000 2500 5000 10000` |
| `-p posteos.dat` | Archivo de posteos | `data/bin/posteos.dat` |
| `-o resultados.csv` | CSV al que se **agregan** los resultados (se crea con cabecera si no existe) | `result/simulacion.csv` |

Por cada lote, el simulador mide primero la **línea base sin caché** y luego una corrida por cada capacidad pedida. Solo se cronometra el bucle que pide todos los posteos de todos los feeds; los datos no se muestran ni se guardan.

**Cómo leer la tabla de resultados:**

| Columna | Significado |
|---|---|
| `cache` | Capacidad de la caché (`sin cache` = línea base) |
| `aciertos` / `fallos` | Posteos encontrados / no encontrados en la caché |
| `lecturas` | Lecturas realizadas a `posteos.dat` |
| `acierto` | Tasa de aciertos |
| `total(s)` | Tiempo total de la simulación en segundos |
| `us/posteo`, `us/usuario` | Tiempos promedio por posteo y por usuario, calculados a partir del total (un solo posteo es menor que la resolución del reloj) |
| `altura` / `h_min` | Solo Parte 2: altura real del árbol índice vs. la mínima posible para esa cantidad de nodos. Si `altura` es mucho mayor que `h_min`, el árbol está degenerado |
| `mejora` | `(t sin caché - t con caché) / t sin caché`. Positiva: la caché acelera; negativa: la empeora |
| `*` junto al total | El tiempo es menor a 50 ms: la resolución de `clock()` puede distorsionar el resultado. Usar un lote con más usuarios |

### 4. Flujo de trabajo típico

1. Generar la base de posteos (una sola vez).
2. Generar uno o más lotes de usuarios (cada uno con `-o` distinto para no pisar el anterior, aunque si son tres archivos cada uno de un tipo distinto no hay problema y no se pisan).
3. (Opcional, pero recomendado) Verificar los `.dat` generados con los verificadores.
4. Correr el simulador de la Parte 1 y el de la Parte 2 **sobre los mismos lotes y capacidades**.
5. Analizar la tabla por pantalla y/o el CSV acumulado en `result/`.

### 5. Ejemplos concretos de uso

```
# 1) Base de posteos reproducible
gen_posteos -s 2024

# 2) Un lote de cada tipo (semillas fijas para poder regenerarlos)
gen_lotes -t concentrado -s 1 -u 2000 -c 100 -p 90
gen_lotes -t disperso    -s 2 -u 2000
gen_lotes -t ordenado    -s 3 -u 2000

# 3) Variantes del lote concentrado, guardadas en archivos distintos
gen_lotes -t concentrado -s 4 -c 20   -p 100 -o data/bin/lote_concentrado_extremo.dat
gen_lotes -t concentrado -s 5 -c 5000 -p 50  -o data/bin/lote_concentrado_suave.dat

# 4) Inspeccionar lo generado (primeros 10 registros a CSV)
verificador_posteos  -n 10
verificador_usuarios -i data/bin/lote_concentrado.dat -n 10

# 5) Simulación básica de un lote con las capacidades por defecto
simulador_parte1 -l data/bin/lote_concentrado.dat
simulador_parte2 -l data/bin/lote_concentrado.dat

# 6) Los tres lotes con capacidades elegidas (comparación completa)
simulador_parte1 -l data/bin/lote_concentrado.dat -l data/bin/lote_disperso.dat -l data/bin/lote_ordenado.dat -c 10 -c 100 -c 1000 -c 5000
simulador_parte2 -l data/bin/lote_concentrado.dat -l data/bin/lote_disperso.dat -l data/bin/lote_ordenado.dat -c 10 -c 100 -c 1000 -c 5000

# 7) Ver la altura del árbol índice en el lote ordenado (Parte 2)
simulador_parte2 -l data/bin/lote_ordenado.dat -c 100 -c 1000 -c 2500
```

> Nótese como `-c` y `-p` son parámetros propios de una generación de lote concentrado.

#### Combinaciones sugeridas para el análisis

| Lote | Caché chica (≈10–100) | Caché mediana (≈500–1000) | Caché grande (≥ 2500) |
|---|---|---|---|
| `concentrado` | Aciertos parciales: solo entran algunos populares | Cubre los populares: máximo beneficio | Cubre todo: la lista simple ya paga el recorrido |
| `disperso` | Casi sin aciertos | Casi sin aciertos | Pocos aciertos; la caché agrega costo |
| `ordenado` | Sin aciertos; árbol chico | Sin aciertos; árbol degenerado | Sin aciertos; la altura del árbol ≈ cantidad de nodos |

---

## Uso en Code::Blocks

Code::Blocks llama build target a cada uno de los programas de la sección 1. La idea es tener **un único proyecto con seis de ellos**.

### Crear el proyecto y los objetivos

1. Crear un proyecto de consola C (*Console application → C*) con el archivo `.cbp` en la **raíz del repositorio**.
2. Agregar al proyecto todos los `.c` / `.h` del directorio `src/`. Inspeccionar la opción de Code::Blocks "Add files recursively" (o agregar archivos de forma recursiva) para esto.
3. Abrir **Project → Properties → Build targets** y, con **Add**, crear un objetivo por programa: `gen_posteos`, `gen_lotes`, `verificador_posteos`, `verificador_usuarios`, `simulador_parte1`, `simulador_parte2`. Para cada uno:
   - **Type:** `Console application`.
   - **Output filename:** por ejemplo `bin/<nombre>.exe`.
   - **Execution working dir:** `.` (la raíz del repositorio) si queda en `bin/Debug` o similar, no se encuentran `data/bin/posteos.dat`, `result/`, etc. El sistema falla.
4. Asignar los archivos a cada objetivo: click derecho sobre cada archivo `.c` → **Properties → Build** y tildar **únicamente** los objetivos que lo usan, según la tabla de la sección 1. (Un `main` en un objetivo que no le corresponde produce errores de *multiple definition of main*.). Nótese que no hace falta enlazar los archivos header.

### Definir `LRU_PARTE_2` (solo `simulador_parte2`)

1. **Project → Build options…**
2. Seleccionar en el panel izquierdo el objetivo `simulador_parte2` (no el proyecto raíz).
3. Pestaña **Compiler settings → #defines** y agregar una línea: `LRU_PARTE_2`
4. Verificar que `simulador_parte1` **no** la tenga.

### Pasar parámetros al programa (*Set programs' arguments*)

1. **Project → Set programs' arguments…**
2. En **Select target**, elegir el objetivo que se va a ejecutar (cada objetivo guarda sus propios argumentos).
3. En **Program arguments** escribir los parámetros, **sin el nombre del programa**. Por ejemplo:
   - `gen_lotes`: `-t concentrado -s 1 -u 2000 -c 100 -p 90`
   - `verificador_usuarios`: `-i data/bin/lote_concentrado.dat -n 10`
   - `simulador_parte2`: `-l data/bin/lote_ordenado.dat -c 100 -c 1000`
4. Aceptar y ejecutar con **Build and run** (F9). Para cambiar de ejemplo, se vuelve a abrir el diálogo y se edita el campo.

### Problemas posibles, y en dónde podría llegar a estar la solución

- **Seleccionar el objetivo correcto** en la barra de herramientas (lista desplegable *Build target*) antes de compilar o ejecutar.
- **"No se pudo abrir/crear ..."**: casi siempre es el *Execution working dir* mal configurado o un directorio de la sección 0 que no existe.
- **"Aviso: no se pudo actualizar data/meta/log.txt"**: falta crear `data/meta/`. No impide que el lote se genere, pero se pierde la trazabilidad.
- **"multiple definition of ..." en los simuladores**: se tildó en un mismo objetivo el `lru_cache.c` de ambas partes, o se mezclaron los archivos de la Parte 1 y la Parte 2.
- **Tiempos con `*`**: el total fue menor a 50 ms. Usar más usuarios (`-u`) al generar el lote.
