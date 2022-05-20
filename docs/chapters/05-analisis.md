# Análisis de rendimiento

En este capítulo vamos a analizar el resultado final del proyecto. Estudiaremos cómo se ve el motor, cómo rinde en términos de *frame time*, y compararemos las imágenes producidas con otras similares; tanto producidas por otros motores, como situaciones en la vida real.

## Usando el motor

Una vez se ha [compilado](#compilación-y-ejecución) el proyecto, puedes encontrar el ejecutable en `./application/bin_x64/Debug`. Abre el binario para entrar en el programa.

![Al abrir el motor, te encontrarás con una pantalla similar a esta: una escena cargada junto a un panel lateral con numerosas opciones.](./img/05/Asmiray.png){#fig:asmiray width=70%}

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

A lo largo de este trabajo hemos visto una gran variedad de conceptos desde el punto de vista teórico. Ahora es el momento de ponerlo en práctica.

### Materiales

Empecemos por materiales. Se han implementado unos cuantos tipos diferentes, los cuales veremos ilustrados a continuación.

Los más simples son los [difusos](#reflexión-difusa-o-lamberiana). La caja de Cornell original contiene dos objetos de este tipo:

![Materiales difusos de la escena Cornell Box original. Vemos que la luz se esparce uniformemente al rebotar en el objeto.](./img/05/Materiales%20difusos.png){#fig:materiales_difusos width=70%}

Los materiales [especulares *glossy*](#reflexión-especular-no-perfecta) han sido modificados ligeramente para simular el parámetro de *roughness* de los metales, para compararlos con los de [@Shirley2020RTW1]:

![Materiales especulares metálicos de la escena Cornell Box glossy](./img/05/Materiales%20glossy.png){#fig:materiales_glossy width=70%}

Si hay algo en lo que destaca ray tracing, es en la simulación de [espejos](#reflexión-especular-perfecta). En rasterización debemos recurrir a técnicas específicas como reflejos planares o *cubemaps*. Ray tracing solventa el problema con elegancia:

![Una caja que actúa como un espejo prácticamente perfecto en la escena `cornell_box_mirror`](./img/05/Materiales%20espejos.png){#fig:materiales_espejos width=70%}

En la siguiente escena observamos dos esferas: una que presenta refracción y otra que no. Ambas utilizan las ecuaciones de Fresnel para modelar el comportamiento de la luz.

![La esfera de la derecha refracta la luz al pasar por ella, adquiriendo en el proceso un color más oscuro. También podemos ver la esfera de la izquierda recursivamente, dentro del propio reflejo de la esfera.](./img/05/Materiales%20reflectantes.png){#fig:reflectantes width=70%}

Los materiales transparentes los gestiona el shader [anyhit](#tipos-de-shaders). Permite descartar las intersecciones con aquellos objetos transparentes para permitir pasar algunos rayos:

![El modelo del Wuson, pero transparente.](./img/05/Materiales%20transparentes.png){#fig:materiales_transparentes width=70%}

### Fuentes de luz

En la primera versión del motor, se han implementado dos tipos de fuentes de luces: puntuales y direccionales.

Las **fuentes de luz puntuales** (*spotlights* en inglés) emiten luz alrededor suya, como si de pequeños soles se trataran. La figura [@fig:spotlights] muestra cómo se comportan en la caja de Cornell original.

![Una fuente de luz puntual iluminando la caja de Cornell original. Vemos cómo se proyectan sombras hacian la pared.](./img/05/Spotlight.png){#fig:spotlights width=60%}

Por otro lado, las **luces direccionales**: imitan la luz proporcionada por algún objeto infinitamente lejano. Puedes ver un ejemplo en la figura [@fig:directional_lights].

![La caja de Cornell original iluminada por una **luz direccional** ](./img/05/Directional.png){#fig:directional_lights width=60%}

Como las puntuales se comportan de manera muy similar en la caja de Cornell, podemos referirnos a la escena del edificio medieval para ver una diferencia más sustancial [@fig:spotlights_medieval, @fig:directional_medieval]. En este caso, se aprecia el radio de influencia de la luz puntual.

![Luz puntual en la escena `medieval_building`.](./img/05/Spotlight%202.png){#fig:spotlights_medieval width=60%}

![Luz direccional en la escena `medieval_building`.](./img/05/Directional%202.png){#fig:directional_medieval width=60%}

### Oclusión ambiental

![](./img/05/Ambient%20occlusion%201.png){#fig:ambient_occlusion_1 width=60%}

![](./img/05/Ambient%20occlusion%202.png){#fig:ambient_occlusion_2 width=60%}

## Rendimiento

Path tracing es un algoritmo costoso. Teniendo en cuenta que tratamos de desarrollar una aplicación en tiempo real, debemos prestar especial atención al coste de renderizar un *frame*. En esta sección vamos a hacer una comparativa de las diferentes opciones que se han implementado en el motor, estudiando la relación calidad de imagen y rendimiento.

Utilizaremos principalmente dos escenas: `cornell_box_original` y `cornell_box_esferas`. Esto es debido a que ofrecen cierta complejidad y los materiales de los objetos permiten estudiar los parámetros del motor.

Para los análisis del rendimiento, se ha utilizado un procesador **Intel i5 12600K**, una tarjeta gráfica Nvidia **2070 Super** con un ligero overclock a 1900MHz y **2x8GB DDR4 3200MHz** de RAM. A no ser que se diga lo contrario, todas las imágenes tienen una resolución de 1280 x 720. Con el fin de realizar una comparación justa, se ha implementado un modo de benchmarking que se puede activar en el archivo `globals.hpp`.

### Número de muestras

El principal parámetro que podemos variar es el número de muestras por píxel. En un estimador de Monte Carlo [@eq:mc_integral], $\hat{I}_N = \frac{1}{N} \sum_{i = 1}^{N}{f(X_i)}$, corresponde a $N \in \mathbb{N}$.

![Para conseguir esta gráfica, iniciamos la escena `cornell_box_original`, y sin mover la cámara, vamos cambiando el número de muestras.](./img/graficas/CB_original_comparativa_samples.png){#fig:grafica_samples}

La figura [@fig:grafica_samples] muestra cómo afecta al rendimiento el valor de $N$. Vemos cómo un número bajo de muestras (alrededor de 5) produce un frametime de aproximadamente 12 milisegundos, lo cual corresponde a 83 frames por segundo. Duplicando $N$ hasta las 10 muestras, produce un aumento del frametime hasta los 20 ms de media (50 FPS). Algo similar pasa con el resto de valores: 15 muestras suponen una media de 28 ms (35 FPS) y 20 muestras unos 35 ms (28 FPS). Sacamos en claro que, en esta escena, **no debemos aumentar las muestras a un valor superior a 20**, pues entraríamos en terreno de renderizado en diferido. No debemos superar la barrera de los 33 milisegundos, pues supondría una tasa de refresco de imagen inferior a los 30 FPS.

Podemos concluir que, en esta escena, el **coste de una muestra por píxel** es de aproximadamente **2 milisegundos**. Este valor puede ser hallado promediando el coste medio de cada frame en cada valor del parámetro.

El número de muestras tiene un grandísimo efecto en la calidad de imagen. Volviendo a la escena anterior, podemos ver cómo cambia el ruido al variar el parámetro `samples`. Para las siguientes imágenes, se ha deshabilitado la [acumulación temporal](#acumulación-temporal), pues en esencia, proporcionaría un mayor número de muestras en el tiempo.

Con una única muestra por píxel, la imagen final aparece muy ruidosa. Aumentarlo a 5 mejora bastante la situación, pero sigue habiendo demasiado ruido [@fig:samples_1]

![**Izquierda**: 1 muestra. **Derecha**: 5 muestras](./img/05/1,%205%20samples.png){#fig:samples_1}

De forma similar, aumentarlo a 10 y a 20 implica un aumento de la calidad visual significativa.

![**Izquierda**: 10 muestras. **Derecha**: 20 muestras](./img/05/10,%2020%20samples.png){#fig:samples_2}

No obstante, el cambio de 10 a 20 no es tan significativo como de 1 a 5. Esto sugiere que debemos usar otras técnicas para reducir la varianza del estimador. ¡La fuerza bruta no suele ser la solución!

### Profundidad de un rayo

Una de las decisiones que tenemos que tomar en el diseño del algoritmo es saber cuándo cortar un camino. Hay varias formas de hacerlo, aunque destacan principalmente dos: fijar un valor máximo de profundidad o la [ruleta rusa](#ruleta-rusa).

Analicemos la primera opción, que es la que hemos implementado nosotros. Para ello, usaremos a la escena `cornell_box_esferas`, pues los materiales reflectivos y refractantes de las esferas nos servirán de ayuda para estudiar el coste de un camino.

![Coste de un frame en función de la profundidad de ](./img/graficas/CB_original_comparativa_depth.png){#fig:grafica_depth}

En esta figura [@fig:grafica_depth] ocurre algo similar a [@fig:grafica_samples]: como es evidente, aumentar la profundidad de un rayo aumenta el coste de renderizar un frame. Sin embargo, hay algunos matices que debemos estudiar con más detalle.

El primero es que cambiar la profundidad no es tan costoso como aumentar el número de muestras. Aún quintuplicando el valor por defecto de 10 rebotes a 50, vemos que el motor se mantiene por debajo de los 33 milisegundos. Para una profundidad de 10, el coste de un frame es de 19 milisegundos (52 FPS), mientras que para 50 es de 28 milisegundos (35 FPS). Tomando un valor intermedio de 20, el coste se vuelve de 24 milisegundos (41 FPS).

Llaman la atención las variaciones en el frametime conforme aumenta la profundidad. Para un valor de `depth = 10`, observamos que oscila entre los 18 y los 20 milisegundos. Sin embargo, para los otros dos valores de 20 y 50 son habituales picos de varios frames, llegando hasta los 5 milisegundos. Además, se aprecia cierta inconsistencia. Sin embargo, esto no resulta un problema, pues la oscilación media es de unos 3 milisegundos aproximadamente, lo cual supone un decremento de unos 5 frames por segundo como máximo.

La naturaleza de la escena afecta en gran medida al resultado. Por mera probabilidad, cuando un rayo rebota *dentro* de la caja, puede salir disparado hacia muchas direcciones. Destacarían en este caso dos situaciones:

- El rayo continúa rebotando en la caja, impactando múltiples veces en las esferas. Esto hace que aumente el coste del camino.
- Se escapa de la caja, llegando hasta el infinito y cortando el camino. En este caso, no se alcanza la profundidad máxima, y el camino se vuelve más barato.

La diferencia de rendimiento es sustancial. Pero, **¿merece la pena el coste adicional?**.

Para responder a esta pregunta, primero debemos conocer cómo actúa este parámetro. Empezando con un número extremadamente bajo para los rebotes, vemos que parte de la escena ni siquiera se renderiza [@fig:depth_1].

![`depth = 1`](./img/05/1%20bounce.png){#fig:depth_1 width=70%}

Aumentar el número de rebotes progresivamente permite que el camino adquiera mayor información. Con dos rebotes, permitimos que un camino adquiera información sobre la caja por dentro, así como un reflejo primitivo en las esferas [@fig:depth_2].

![`depth = 2`](./img/05/2%20bounces.png){#fig:depth_2 width=70%}

Con 3 rebotes, la esfera izquierda refleja casi en su totalidad la esfera, pero vemos que el reflejo de la esfera derecha *dentro* de la izquierda está oscurecido [@fig:depth_3].

![`depth = 3`](./img/05/3%20bounces.png){#fig:depth_3 width=70%}

Subiéndolo a 4 rebotes [@fig:depth_4] se arregla mayoritariamente esto.

![`depth = 4`](./img/05/4%20bounces.png){#fig:depth_4 width=70%}

En esta escena, aumentar más allá de 5 o 6 rebotes produce una situación de retornos reducidos. La calidad de imagen no aumenta prácticamente nada, pero el coste se vuelve muy elevado [@fig:depth_20, @fig:depth_50].

![`depth = 20`](./img/05/20%20bounces.png){#fig:depth_20 width=70%}

![`depth = 50`](./img/05/50%20bounces.png){#fig:depth_50 width=70%}

### Acumulación temporal

La acumulación temporal proporcionará una mejora enorme de la calidad visual sin perder rendimiento. Sin embargo, tiene como contrapartida que necesita dejar la cámara estática. Dependiendo de la situación esto podría ser un motivo factor no negociable, pero en nuestro caso, nos servirá.

Utilizando una única muestra, pero un valor de acumulación temporal de 100 frames máximos, proporciona una imagen sin apenas ruido [@fig:acumulacion_temp]

![1 muestra, acumulación temporal de 100 frames. A diferencia de @fig:samples_1, el resultado es impecable.](./img/05/Acumulación%20temporal%201.png){#fig:acumulacion_temp width=40%}

Subiendo los parámetros a 200 frames de acumulación temporal y 10 muestras, se obtiene una imagen muy buena[@fig:acumulacion_temp_2]

![10 muestras, acumulación temporal de 200 frames.](./img/05/Acumulación%20temporal%202.png){#fig:acumulacion_temp_2 width=40%}

El tremendo efecto de esta técnica es debido a que actúa como normalización entre imágenes. Interpolando linealmente los resultados de diferentes frames, con el tiempo se conseguirá una foto de lo que se debe ver realmente, eliminando así el ruido y las luciérnagas.

### Resolución

Como se ha mencionado en la introducción, todas las escenas anteriores se han renderizado a 720p. Podemos controlar la resolución interna del motor desde el archivo `globals.hpp`. Veamos cómo escala al variarla.

![Tiempo de renderización de un frame dependiendo de la resolución](./img/graficas/CB_original_comparativa_resolucion.png){#fig:resolucion}

A 720p, la escena `cornell_box_original` corre a 105 FPS (9.6 ms/frame), mientras que a 1080p, el motor corre a 47FPS (21 ms/frame) y a 1440p, a 28 FPS (36 ms/frame). Como vemos, la resolución tiene un gran impacto en el rendimiento. El cambio de 720p a 1080p implica un aumento del 125% en el número de píxeles a dibujar, por lo que es natural que el coste sea proporcional a esta cantidad. 1440p tiene 1.7 veces más píxeles que 1080p.

En la práctica, ray tracing no suele utilizar resoluciones internas tan grandes. Se aplican otro tipo de técnicas para reducir el ruido, como veremos en el capítulo de estado del arte.

### Importance sampling

El [muestreo por importancia](#muestreo-por-importancia) consiste en tomar una función de densidad proporcional a la función a integrar para reducir la varianza. Se ha implementado muestreo por importancia para los materiales difusos. En particular, se ha utilizado dos estrategias diferentes para escoger direcciones aleatorias, que pueden observarse en la figura [@fig:importance_sample].

![**Izquierda**: *hemisphere sampling*. **Derecha**: *Cosine-Weighted Hemisphere Sampling*. <br> Escena: `cornell_box_original`, 10 rebotes, acumulación temporal off, 10 muestras](./img/05/Importance%20sample.png){#fig:importance_sample}

Para facilitar la comparativa, desde una posición estática se ha tomado una foto, y hemos ampliado la caja de la derecha digitalmente. De esta forma, podemos ver cómo la caja de la derecha contiene una menor cantidad de ruido, aún manteniendo los mismos parámetros de renderizado.

## Comparativa con In One Weekend