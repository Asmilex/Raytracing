\appendix

# El presente y futuro de Ray Tracing

Cosas de las que quiero hablar:

- Este trabajo es estado del arte
- Hablar de algunos offline renderers
- Denoising & filtering muy importantes
  - Enlazar con acumulación temporal
- Series X & PS5
- Juegos de útlima generación
  - Control
  - Minecraft RTX
  - Cyberpunk
  - Quake II RTX y Doom RTX

Después de un esfuerzo monumental como el que supone el desarrollo de un motor de rendering es satisfactorio ver el resultado; y más sabiendo que tratamos con tecnología puntera. Sin embargo, en el fondo, este trabajo no es más que un juguete. Mientras que este proyecto ha durado escasos meses, los profesionales llevan años trabajando en transporte de luz. Así que, ¡veamos qué se cuece en la industria!

En esta sección vamos a explorar ligeralmente el estado del arte. Veremos algunas de las técnicas que están cobrando fuerza en los últimos años, así como algunos renderers profesionales. Le pondremos especial atención a cómo funciona el sistema de iluminación global de Unreal Engine 5, conocido como *Lumen*.

## *Denoising*

Durante el desarrollo del trabajo aprendimos que el número de muestras no lo es todo. De hecho, es una de las partes menos importantes. Para reducir el ruido de la imagen final, resulta muchísimo más eficiente usar una buena estrategia de muestreo.

Con los avances de ray tracing en tiempo real surgió una nueva rama del tratamiento de la computación gráfica conocida como *denoising*. Este proceso consiste en eliminar el ruido de la imagen final, y es una técnica que se puede aplicar a cualquier imagen.

A continuación, veremos una introducción a las técnicas modernas que se utilizan en ray tracing para acelerar la convergencia de rayos. Nos basaremos en las notas recogidas por Alain Galvan en su blog [@alain-filtering] [@alain-denoising] para enumerar algunas de las técnicas. Todas las referencias a los trabajos originales se encuentran recogidas en dicha entrada, por lo que recomiendo leerla para comprender mejor cada técnica.

## Filtrado

Entre las **técnicas basadas en muestreo** que ya hemos estudiado se encuentran multiple importance sampling, next-event estimation, ruleta rusa y series de quasi-Monte Carlo.

Otro método muy común es el uso de **ruido azul**, o *Blue Noise* [@blue-noise]. Este tipo de ruido proporcionan valores aleatorios por píxel para crear patrones de ruido. Lo bueno de este tipo de patrones es que pueden ser tratados con mayor facilidad que otros tipos de ruido. [@nvidia-blue-noise]. Esto es debido a su distribución uniforme, que produce menos instancias de errores de alta frecuencia, lo cual hace que sean fáciles de difuminar y acumular [@alain-filtering]. Este tipo de ruido puede ser creado eficientemente mediante generadores de secuencias cuasi-aleatorias como las de Sobol.

![En comparación con el ruido blanco, el ruido azul resulta más fácil de difuminar. Fuente: @alain-filtering](./img/07/Bluenoise.png){#fig:blue-noise width=40%}

Del área del **procesamiento de señales** se han filtros Gaussianos, Medianos y Guiados para promediar regiones de baja varianza. En los últimos años, una nueva variante de este tipo de técnicas ha surgido como consecuencia del *machine learning*, el cual estudiaremos más tarde.

Las **técnicas de acumulación** permiten reutilizar información anterior para suavizar el resultado final. Este tipo de técnicas empezaron originalmente en 1988 con *irradiance caching* [@irradiance-caching].

Un método muy sencillo que hemos estudiado es la [acumulación temporal](#antialiasing-mediante-jittering-y-acumulación-temporal), pero requiere que la cámara se quede estática. En la práctica se puede lo mismo con una escena en movimiento. Algunos de los algoritmos más famosos son *Spatio-Temporal Variance Guided Filter* (SVGF), *Spatial Denoising*, Adaptive SVGF (A-SVGF) y *Temporally dense ray tracing*.

Un algoritmo que se ha utilizado exitosamente en los últimos años es *Spatiotemporal importance Resampling* (ReSTIR) [@restir], el cual es capaz de procesar millones de luces dinámicas en tiempo real en alta calidad sin necesidad de introducir estructuras de datos excesivamente complejas. Está basado en el algoritmo *Sampling Importance Resampling* (STIR). Este último se puede estudiar a fondo en [@stir]

![ReSTIR en acción. Fuente: @restir](https://research.nvidia.com/sites/default/files/styles/wide/public/publications/piratesTeaser_nvrSite_scaled.png?itok=ZlOnc27K)

La mayor parte de estas técnicas requieren el uso de un *motion buffer*, el cual calcula el cambio en la posición de un vértice de un frame a otro. En la práctica suelen venir acompañados de *motion vectors*, los cuales también son usados para técnicas como temporal antialiasing (TAA) o temporal upscaling [@temporal-supersampling] (TAA es una técnica de muestreo).

## Machine Learning

DLSS, Intel Open Image Denoise (https://github.com/OpenImageDenoise/oidn)

-
- https://alain.xyz/blog/ray-tracing-denoising
- https://alain.xyz/blog/ray-tracing-filtering
- STIR /
- ReSTIR https://research.nvidia.com/sites/default/files/pubs/2020-07_Spatiotemporal-reservoir-resampling/ReSTIR.pdf
- DLSS https://www.nvidia.com/es-es/geforce/technologies/dlss, https://www.youtube.com/watch?v=YWIKzRhYZm4
- FSR https://www.youtube.com/watch?v=y2RR2770H8E
- Blue noise
  - https://blog.demofox.org/2020/05/16/using-blue-noise-for-raytraced-soft-shadows/
  - https://alain.xyz/blog/ray-tracing-filtering
  - Minecraft RTX usa blue noise para el sampling: https://alain.xyz/blog/frame-analysis-minecraftrtx/assets/diffuse-1spp.jpg

## La industria del videojuego

- Hybrid RT
  - https://www.khronos.org/blog/vulkan-ray-tracing-best-practices-for-hybrid-rendering
- Productos comerciales
  - Control
    - Híbrido
    - https://alain.xyz/blog/frame-analysis-control
    - https://www.youtube.com/watch?v=blbu0g9DAGA
  - Minecraft RTX
    - Path tracing
    - https://alain.xyz/blog/frame-analysis-minecraftrtx
    - https://www.youtube.com/watch?v=s_eeWr622Ss
    - https://www.youtube.com/watch?v=TVtSsJf86_Y
  - Cyberpunk 2077
    - Híbrido
  - Quake II RTX
    - Path tracing
  - Doom RTX
    - Path tracing
  - Metro Exodus

## Offline renderers

- https://www.disneyanimation.com/technology/hyperion/
- https://arnoldrenderer.com/
- Unity https://unity.com/es
- Unreal Engine 5

## Unreal Engine 5 & Lumen

- https://www.unrealengine.com/en-US/tech-blog/unreal-engine-5-goes-all-in-on-dynamic-global-illumination-with-lumen
- https://twitter.com/Yurukuyaru/status/1523643949826588674

Unreal Engine 5 [@UE5] es la última generación del Motor *Unreal Engine* creado por Epic Games. Es un software extremadamente complejo que empaqueta múltiples funciones, como animación, físicas, renderizado, audio, y un largo etcétera. Se usa tanto en la industria de la animación como en la de los videojuegos.

Una de las novedades de esta versión es la integración de un sistema de iluminación global, llamado **Lumen**. Junto a **Nanite**, que es un sistema de geometría virtualizada, es la caracaterística más importante que han presentado. Recientemente han detallado cómo funciona Lumen [@lumen], así que estudiaremos qué técnicas han utilizado.

![Habitación interior iluminada por el sistema Lumen](https://cdn2.unrealengine.com/lumen-unreal-engine-5-image12-1999x1130-4bfd11e88c9d.png){ #fig:lumen1  width=70% }

La [iluminación global](#iluminación-global) es el efecto que produce la luz cuando los fotones que la componen rebotan dentro de una escena. En el pasado (i.e., rasterización) se ha simulado mediante técnicas como *lightmap baking*, oclusión ambiental y similares. Sin embargo, como ya explicamos en secciones anteriores, estas técnicas presentan grandes limitaciones.

Lumen simula infinitos rebotes de la luz en una escena en tiempo real, actualizando la iluminación automáticamente Esto funciona tanto para interiores como exteriores, puesto que simula la luz proveniente del cielo. Además, el sistema tiene en cuenta materiales emisivos y la niebla volumétrica.

![Una escena exterior con materiales emisivos en Unreal Engine 5. Fuente: @lumen](https://cdn2.unrealengine.com/lumen-unreal-engine-5-image16-1999x1125-48a619533ccb.jpg){#fig:lumen2}

Como era de esperar, Lumen utiliza ray tracing para hacer el cálculo de la iluminación. Para conseguirlo, el sistema crea una versión simplificada de la escena en la cual resulta más fácil hacer intersecciones de rayos con objetos.

Por defecto se ha optado una forma de ray tracing basada en software, pues de esta manera se soporta más tipos de hardware. Se combinan *Mesh Distance Fields* (mallas de polígonos que resultan eficientes a la hora de intersecar) mezcladas en un *Global Distance Field* en este tipo de ray tracing. No obstante, su versión de ray tracing basado en hardware es mandatoria para conseguir algunos efectos como reflejos especulares perfectos, pues de otra manera no se podrían conseguir en tiempo real.

Una optimización clave para que este sistema funcione de forma eficiente es el uso de *surface caching*. Trabajando en conjunto junto a Nanite, Lumen crea un atlas de texturas de aquellas mallas presentes alrededor del jugador de forma que se cubran las caras visibles desde algún punto de vista. Esto simplifica el coste de la evaluación del material.

![Visualización de *Mesh Distance Fields* de una escena. Fuente: @lumen](./img/07/Lumen1.jpg){#fig:lumen3}

Para el paso final del cálculo de la radiancia se usa un algoritmo basado en radiance caching presentado en [@siggraph2021]. Como no resulta viable tirar rayos a diestro y siniestro por toda la escena, Lumen toma una pequeña porción de muestras y combina los cálculos de radiancia con información aportada por el material.

![Lumen combina los cálculos de la iluminación global con la información del material para crear la imagen final. Fuente: @lumen](https://cdn2.unrealengine.com/lumen-unreal-engine-5-image24-1920x1080-3158475cf4a8.png){#fig:lumen4 width=70%}

Otro punto clave del sistema es la elección de las direcciones que se van a trazar. Para escogerlas, se comprueba qué partes del último frame resultaron muy brillantes. Entonces, se mandan rayos hacia esas zonas, pues esto producirá resultados menos ruidos en este frame. Esta técnica la hemos estudiado, y se llama **muestreo por importancia** (de la luz entrante en este caso).

> Este punto enlaza con nuestra [comparativa con In One Weekend](#comparativa-con-in-one-weekend). Una de las conclusiones más interesantes que obtuvimos fue que utilizar muestreo por importancia de las fuentes de iluminación proporcionaba unos resultados considerablemente mejores que una estrategia de muestreo basada en direcciones aleatorias. Aunque en este caso no se muestrean las fuentes de luz, sí que se utilizan partes de la escena con mucha radiancia, por lo que el fundamento es similar.

![Lumen utiliza las partes más brillantes del último frame para la distribución de las nuevas direcciones de un rayo tras el impacto. Fuente: @lumen](https://cdn2.unrealengine.com/lumen-unreal-engine-5-image1-1999x1109-d3b299670791.png){#fig:lumen5}

Para calcular los reflejos de superficies se ha optado por promediar la radiancia de puntos en el mismo vecindario y acumulación temporal de frames para suavizar el resultado.

![Nuestro método principal de reducción de ruido fue la acumulación temporal de frames. Lumen utiliza una forma mucho más avanzada para lograr el mismo resultado con sus reflejos. Fuente: @lumen](./img/07/Lumen2.jpg)

Finalmente, es importante destacar que las imágenes no son generadas a una gran resolución. Para conseguir una imagen digna de los tiempos actuales, los motores modernos utilizan una resolución interna cercana a 720p o 1080p que es escalada mediante algún método de *upsampling*. En este caso, se ha optado por *Temporal Super Resolution*, una técnica que comenzó con Unreal Engine 4 y ha evolucionado en esta versión. Su comportamiento es similar a DLSS o FSR.

Todas estas optimizaciones son esenciales para hacer que el motor corra en tiempo real. Aunque el rendimiento es extremadamente dependiente del tipo de contenido que se esté visualizando y los parámetros del sistema, Unreal Engine 5 puede correr a 60 FPS en consolas de la generación actual.