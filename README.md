# Juego de la Vida

Este proyecto implementa el **Juego de la Vida de Conway** en C, utilizando operaciones a nivel de bits para representar de forma eficiente el tablero, y archivos para leer el estado inicial y guardar cada generación.

Proyecto implementado para el curso de organización de computadores ST0254. Desarrollado por:
- Juan José Botero
- Mateo García
- Santiago Idárraga
- Samuel Villa

## Representación binaria

Para manipular el estado de cada celda del tablero se usan operaciones bit a bit:

### `1ULL << j`

* `1ULL` es el valor `1` como un **unsigned long long** (64 bits).
* `<< j` desplaza el bit 1 hacia la izquierda `j` posiciones.

  * Por ejemplo: `1ULL << 3` genera `0b00001000`, activando el cuarto bit desde la derecha.

### `grid[i] >> j & 1`

* Extrae el bit en la posición `j` de la fila `i`.
* `grid[i] >> j` desplaza los bits para poner el bit de la columna `j` en la posición más baja.
* `& 1` enmascara todos los demás bits y deja solo el bit deseado.

Esto permite representar y manipular un tablero entero de 20x20 solo con 20 enteros de 64 bits.

---

## Estructura de funciones principales

### `escribir_tablero(FILE* f, Fila* grid, int gen)`

Guarda el estado del tablero en un archivo por generación.

* Escribe el número de generación.
* Recorre cada fila y columna del tablero.
* Usa `fputc` para imprimir:

  * `'O'` si la celda está viva (bit en 1).
  * `'.'` si está muerta (bit en 0).

### `siguiente_generacion(Fila* actual, Fila* siguiente)`

Calcula el estado del tablero para la siguiente generación.

* Recorre todas las celdas del tablero.

* Por cada celda (`i`,`j`):

  * Se cuentan los vecinos vivos en un vecindario de 3x3:

    * Esto se logra con **cuatro bucles anidados**:

      * Dos para recorrer desplazamientos `di` y `dj` (-1, 0, 1).
      * Dos principales para recorrer la posición actual `i` y `j`.
  * Se omite el centro con `if (di == 0 && dj == 0)`.
  * Se verifica que los índices vecinos estén dentro del rango del tablero.
  * Se suma el valor del bit correspondiente usando `>>` y `&`.

* Reglas:

  * Si una celda está viva y tiene 2 o 3 vecinos → sigue viva.
  * Si está muerta y tiene exactamente 3 vecinos → revive.

* El nuevo estado se construye con `siguiente[i] |= (1ULL << j);`.

### `copiar(Fila* dst, Fila* src)`

Copia el contenido de un tablero a otro.

* Usa un bucle para copiar cada fila individualmente.

### `leer_tablero(const char* nombre, Fila* grid)`

Lee el tablero inicial desde un archivo de texto:

* Cada fila debe tener exactamente 20 caracteres: `'O'` para vivo, `'.'` para muerto.
* Valida caracteres y longitud.
* Convierte cada fila a un `Fila`, encendiendo bits con `fila |= (1ULL << (N - 1 - j));` para mantener el formato desde izquierda a derecha.

### `main()`

* Lee el tablero inicial desde un archivo `tablero.txt`.
* Pide al usuario el número de generaciones a simular.
* Abre el archivo `vida.txt` para guardar la salida.
* Por cada generación:

  * Guarda el tablero actual.
  * Calcula la siguiente generación.
  * Copia el resultado al tablero actual.

---

## Archivos de entrada y salida

* **Entrada**: Debe contener 20 filas con 20 caracteres cada una.
* **Salida**: Contiene el tablero para cada generación, precedido por el número de generación.

---

## Ejemplo de entrada (`tablero.txt`)

```
....................
.................O..
................O...
................OOO.
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
....................
```

---

## Requisitos

* Compilador C (gcc recomendado).

### Compilación

```
gcc -o main main.c
```

### Ejecución

```
./main
Ingrese el numero de generaciones a simular: 10
```
