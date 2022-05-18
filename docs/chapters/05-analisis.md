# Análisis de rendimiento

En este capítulo vamos a analizar el resultado final del proyecto. Estudiaremos cómo se ve el motor, cómo rinde en términos de *frame time*, y compararemos las imágenes producidas con otras similares; tanto producidas por otros motores, como situaciones en la vida real.

## Usando el motor

Una vez se ha [compilado](#compilación-y-ejecución) el proyecto, puedes encontrar el ejecutable en `./application/bin_x64/Debug`. Abre el binario para entrar en el programa.

![Al abrir el motor, te encontrarás con una pantalla similar a esta: una escena cargada junto a un panel lateral con numerosas opciones.](./img/05/Asmiray.png)

Si alguna vez has usado un motor de renderización en 3D (como Blender, Unity, Unreal Engine o AutoCAD), el comportamiento debería resultarte familiar. El uso de nuestro progama es muy similar al de los anteriores:

- El **botón izquierdo del ratón rota** la cámara alderedor del punto de mira.
- Para acercar o alejar la cámara, utiliza la **rueda de scroll** o el **botón derecho del ratón + hacia arriba o abajo**.
- Si quieres moverte lateralmente, mantén pulsado la tecla **control** y utiliza el **botón izquierdo + arrastrar**. Alternativamente, **aprieta el click de la rueda del ratón** y múevete.
- Para girar la cámara alderedor como si de un *first person shooter* se tratara, pulsa **alt + click izquierdo**.

Puedes cambiar el modo de cámara en la pestaña "Extra" de la interfaz gráfica. Los diferentes modos alternan entre las acciones listadas anteriormente.

Para ocultar la interfaz gráfica, pulsa **F10**.

### Cambio de escena

El programa viene acompañado de varios mapas. Desafortunadamente, para cambiar de escena es necesario recompilar el programa. Las instrucciones necesarias para conseguirlo son las siguientes:

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

Ten en cuenta que las imágenes de las escenas no son definitivas. Están sujetas a cambios, pues se podría cambiar el comportamiento de los shaders.

## Path tracing showcase

> TODO: Cosas de las que tengo que hablar:
>
> - Luces
>   - Puntual
>   - Direccional
> - Cornell Boxes
>   - Cornell Box
>   - Cornell Bath
>   - Mirar la iluminación global
>   - Efectos de las pelotas en el ambiente

A lo largo de este trabajo hemos visto una gran variedad de conceptos desde el punto de vista teórico. Ahora es el momento de ponerlo en práctica.

### Materiales

Empecemos por materiales. Se han implementado unos cuantos tipos diferentes, los cuales veremos ilustrados a continuación.

Los más simples son los [difusos](#reflexión-difusa-o-lamberiana). La caja de Cornell original contiene dos objetos de este tipo:

![Materiales difusos de la escena Cornell Box original. Vemos que la luz se esparce uniformemente al rebotar en el objeto.](./img/05/Materiales%20difusos.png)

Los materiales [especulares *glossy*](#reflexión-especular-no-perfecta) han sido modificados ligeramente para simular el parámetro de *roughness* de los metales, para compararlos con los de [@Shirley2020RTW1]:

![Materiales especulares metálicos de la escena Cornell Box glossy](./img/05/Materiales%20glossy.png)

Si hay algo en lo que destaca ray tracing, es en la simulación de [espejos](#reflexión-especular-perfecta). En rasterización debemos recurrir a técnicas específicas como reflejos planares o *cubemaps*. Ray tracing solventa el problema con elegancia:

![Una caja que actúa como un espejo prácticamente perfecto en la escena `cornell_box_mirror`](./img/05/Materiales%20espejos.png)

En la siguiente escena observamos dos esferas: una que presenta refracción y otra que no. Ambas utilizan las ecuaciones de Fresnel para modelar el comportamiento de la luz.

![La esfera de la derecha refracta la luz al pasar por ella, adquiriendo en el proceso un color más oscuro. También podemos ver la esfera de la izquierda recursivamente, dentro del propio reflejo de la esfera.](./img/05/Materiales%20reflectantes.png)

Los materiales transparentes los gestiona el shader [anyhit](#tipos-de-shaders). Permite descartar las intersecciones con aquellos objetos transparentes para permitir pasar algunos rayos:

![El modelo del Wuson, pero transparente.](./img/05/Materiales%20transparentes.png)

### Fuentes de luz

En la primera versión del motor, se han implementado dos tipos de fuentes de luces:

- **Fuentes de luz puntuales** (*spotlights* en inglés): emiten luz alrededor suya.
- **Luz direccional**: imitan la luz proporcionada por algún objeto infinitamente lejano. En esencia, como si fuera un sol.

![Una fuente de luz puntual iluminando la caja de Cornell original. Vemos cómo se proyectan sombras hacian la pared.](./img/05/Spotlight.png)

![Dada la naturaleza de la caja, tanto las **luces direccionales** como las puntuales se comportan de manera muy similar en la caja](./img/05/Directional.png)

![Sin embargo, en una escena como la del edificio medieval, hay una diferencia sustancial entre ambos tipos](./img/05/Spotlight%202.png)

### PRUEBAS

![CC](https://upload.wikimedia.org/wikipedia/commons/7/70/Example.png){width=45%} \ ![DD](https://upload.wikimedia.org/wikipedia/commons/7/70/Example.png){width=45%}


![](./img/05/Directional.png){width=49%}
![](./img/05/Directional%202.png){width=49%}
\begin{figure}[!h]
\begin{subfigure}[t]{0.4\textwidth}
\caption{AAAAAAAA}
\end{subfigure}
\hfill
\begin{subfigure}[t]{0.4\textwidth}
\caption{BBBBBBBBBB}
\end{subfigure}
\caption{A single caption for the two subfigures}
\end{figure}


### FIN DE PRUEBAS

### Algunas escenas interesantes



## Rendimiento

### Diferentes escenas
### Resolución
### Profundidad de un rayo
### Acumulación temporal
### Número de muestras
### Importance sampling


## Comparativa con In One Weekend