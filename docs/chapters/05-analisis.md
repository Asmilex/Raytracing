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
- Para acercar o alejar la cámara, utiliza la **rueda de scroll**.
- El **click derecho del ratón** desplaza lateralmente la cámara.

### Cambio de escena

El programa viene acompañado de varias escenas. Desafortunadamente, para cambiar de escena es necesario recompilar el programa. Las instrucciones necesarias para conseguirlo son las siguientes:

- Ubica la sentencia `load_scene(Scene::escena, engine);` que se encuentra en el archivo `main.cpp`.
- Cambia el valor del primer parámetro: reemplaza `Scene::escena` por alguna entrada del enumerado `Scene`. Puedes encontrar sus posiblidades en el archivo `Scenes.hpp`.
- Recompila el programa.

Las escenas son las siguientes:

| **Nombre de escena**             | **Descripción**                                                                         | **Imagen** |
|:---------------------------------|:----------------------------------------------------------------------------------------|:----------:|
| `cube_default`                   | La escena por defecto del programa. Muestra un simple cubo.                             |            |
| `any_hit`                        | Desmostración de las capacidades del shader *anyhit*.                                   |            |
| `cube_reflective`                | Ejemplifica *ray traced reflections*.                                                   |            |
| `medieval_building`              | Una sencilla escena que contiene una casa medieval con texturas.                        |            |
| `cornell_box_original`           | Una reconstrucción de la caja de Cornell original [@cornell-box-original].              |            |
| `cornell_box_mirror`             | Similar a la caja original, esta escena es una recreación de [@cornell-box-compare].    |            |
| `cornell_box_vacia`              | La caja original sin las cajitas pequeñas dentro.                                       |            |
| `cornell_box_vacia_azul_naranja` | Similar a la anterior, pero con las paredes naranjas y azules.                          |            |
| `cornell_box_esferas`            | Una caja de Cornell con esferas. Se puede comparar con [@Jensen2001, p. 107 fig. 9.10]. |            |
| `cornell_box_saturada`           | Otra caja de Cornell similar a la original, pero con las paredes saturadas.             |            |
| `cornell_box_glossy`             |                                                                                         |            |
| `cornell_box_blanca`             |                                                                                         |            |
| `cubes`                          |                                                                                         |            |



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