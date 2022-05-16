# Análisis de rendimiento

En este capítulo vamos a analizar el resultado final del proyecto. Estudiaremos cómo se ve el motor, cómo rinde en términos de *frame time*, y compararemos las imágenes producidas con otras similares; tanto producidas por otros motores, como situaciones en la vida real.

## Usando el motor

Cosas de las que tengo que hablar:

- Cámara
- Keybindings
- GUI
- Cambio de escenas

Una vez se ha [compilado](#compilación-y-ejecución) el programa, puedes encontrar el ejecutable en `./application/bin_x64/Debug`. Abre el binario para entrar en el programa.

![Al abrir el motor, te encontrarás con una pantalla similar a esta: una escena cargada con un panel lateral con opciones.](./img/05/Asmiray.png)

Si alguna vez has usado un motor de renderización en 3D (como Blender, Unity, Unreal Engine o AutoCAD), el comportamiento debería resultarte familiar. El uso de nuestro progama es análogo al de los anteriores:

- El **botón izquierdo del ratón rota** la cámara alderedor del punto de mira.
- Para acercar o alejar la cámara, utiliza la **rueda de scroll** o el **botón derecho del ratón + hacia arriba o abajo**.
- Si quieres moverte lateralmente, mantén pulsado la tecla **control** y utiliza el **botón izquierdo + arrastrar**. Alternativamente, **aprieta el click de la rueda del ratón** y múevete.
- Para girar la cámara alderedor como si de un *first person shooter* se tratara, pulsa **alt + click izquierdo**.

Puedes cambiar el modo de cámara en la pestaña "Extra" de la interfaz gráfica. Los diferentes modos alternan entre las acciones listadas anteriormente.

Para ocultar la interfaz gráfica, pulsa **F10**.

### Cambio de escena

El programa viene acompañado de varias escenas. Desafortunadamente, para cambiar de escena es necesario recompilar el programa. Las instrucciones necesarias para conseguirlo son las siguientes:

- Ubica la sentencia `load_scene(Scene::escena, engine);` que se encuentra en el archivo `main.cpp`.
- Cambia el valor del primer parámetro: reemplaza `Scene::escena` por alguna entrada del enumerado `Scene`. Puedes encontrar sus posiblidades en el archivo `Scenes.hpp`.
- Recompila el programa.

Las escenas son las siguientes:

+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| **Nombre de escena** | **Descripción**                                                                                                          | **Imagen**                             |
+======================+==========================================================================================================================+========================================+
| `cube_default`       | La escena por defecto del programa. Muestra un simple cubo.                                                              | ![](./img/05/cube_default.png)         |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `any_hit`            | Desmostración de las capacidades del shader *anyhit*.                                                                    | ![](./img/05/any_hit.png)              |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cube_reflective`    | Ejemplifica *ray traced reflections*.                                                                                    | ![](./img/05/cube_reflective.png)      |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `medieval`           | Una sencilla escena que contiene una casa medieval con texturas.                                                         | ![](./img/05/medieval_building.png)    |
| `_building`          |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | Una reconstrucción de la caja de Cornell original [@cornell-box-original].                                               | ![](./img/05/cornell_box_original.png) |
| `_original`          |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | Similar a la caja original, esta escena es una recreación de [@cornell-box-compare].                                     | ![](./img/05/cornell_box_mirror.png)   |
| `_mirror`            |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | La caja original sin las cajitas pequeñas dentro.                                                                        | ![](./img/05/cornell_box_vacia.png)    |
| `_vacia`             |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | Similar a la anterior, pero con las paredes naranjas y azules.                                                           | ![](./img/05/cornell_box_vacia_an.png) |
| `_vacia_an`          |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | Una caja de Cornell con esferas. Se puede comparar con [@Jensen2001, p. 107 fig. 9.10].                                  | ![](./img/05/cornell_box_esferas.png)  |
| `_esferas`           |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | Otra caja de Cornell similar a la original, pero con las paredes saturadas.                                              | ![](./img/05/cornell_box_saturada.png) |
| `_saturada`          |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | En esta caja se encuentran dos esferas de diferente material. Se puede comparar con [@cornell-box-glossy, p. 17, fig. 6] | ![](./img/05/cornell_box_glossy.png)   |
| `_glossy`            |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cornell_box`        | Una caja vacía. Es un benchmark infernal para el ruido generado por la iluminación global.                               | ![](./img/05/cornell_box_blanca.png)   |
| `_blanca`            |                                                                                                                          |                                        |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+
| `cubes`              | Dos cubos de diferente material sobre un plano reflectante.                                                              | ![](./img/05/cubes.png)                |
+----------------------+--------------------------------------------------------------------------------------------------------------------------+----------------------------------------+

## Path tracing ON

Cosas de las que tengo que hablar:

- Materiales
  - Difusos
  - Especulares
  - Espejos
  - Transparentes (gestionado por anyhit)
- Luces
  - Puntual
  - Direccional
- Cornell Boxes
  - Cornell Box
  - Cornell Bath
  - Mirar la iluminación global
  - Efectos de las pelotas en el ambiente


## Rendimiento

## Midiendo el coste de un frame
### Resolución
### Profundidad de un rayo
### Acumulación temporal
### Número de muestras
### Diferentes escenas
### Importance sampling

## Comparativa con In One Weekend