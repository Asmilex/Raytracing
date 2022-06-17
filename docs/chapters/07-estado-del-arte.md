\appendix

# El presente y futuro de Ray Tracing

Después de un esfuerzo monumental como el que supone el desarrollo de un motor de rendering es satisfactorio ver el resultado; y más sabiendo que tratamos con tecnología puntera. Sin embargo, en el fondo, este trabajo no es más que un juguete. Mientras que este proyecto ha durado escasos meses, los profesionales llevan años trabajando en transporte de luz. Así que, ¡veamos qué se cuece en la industria!

En esta sección vamos a explorar ligeralmente el estado del arte. Veremos algunas de las técnicas que están cobrando fuerza en los últimos años, así como algunos renderers profesionales. Le pondremos especial atención a cómo funciona el sistema de iluminación global de Unreal Engine 5, conocido como *Lumen*.

## *Denoising*

Durante el desarrollo del trabajo aprendimos que el número de muestras no lo es todo. De hecho, es una de las partes menos importantes. Para reducir el ruido de la imagen final, resulta muchísimo más eficiente usar una buena estrategia de muestreo.

Con los avances de ray tracing en tiempo real surgió una nueva rama del tratamiento de la computación gráfica conocida como *denoising*. Este proceso consiste en eliminar el ruido de la imagen final, y es una técnica que se puede aplicar a cualquier imagen.

A continuación, veremos una introducción a las técnicas modernas que se utilizan en ray tracing para acelerar la convergencia de rayos. Nos basaremos en las notas recogidas por Alain Galvan en su blog [@alain-filtering] [@alain-denoising] para enumerar algunas de las técnicas. Todas las referencias a los trabajos originales se encuentran recogidas en dicha entrada, por lo que recomiendo leerla para comprender mejor cada técnica.

### Filtrado

Entre las **técnicas basadas en muestreo** que ya hemos estudiado se encuentran multiple importance sampling, next-event estimation, ruleta rusa y series de quasi-Monte Carlo.

Otro método muy común es el uso de **ruido azul**, o *Blue Noise* [@blue-noise]. Este tipo de ruido proporcionan valores aleatorios por píxel para crear patrones de ruido. Lo bueno de este tipo de patrones es que pueden ser tratados con mayor facilidad que otros tipos de ruido. [@nvidia-blue-noise]. Esto es debido a su distribución uniforme, que produce menos instancias de errores de alta frecuencia, lo cual hace que sean fáciles de difuminar y acumular [@alain-filtering]. Este tipo de ruido puede ser creado eficientemente mediante generadores de secuencias cuasi-aleatorias como las de Sobol.

![En comparación con el ruido blanco, el ruido azul resulta más fácil de difuminar. Fuente: @alain-filtering](./img/07/Bluenoise.png){#fig:blue-noise width=40%}

Del área del **procesamiento de señales** se han filtros Gaussianos, Medianos y Guiados para promediar regiones de baja varianza. En los últimos años, una nueva variante de este tipo de técnicas ha surgido como consecuencia del *machine learning*, el cual estudiaremos más tarde.

Las **técnicas de acumulación** permiten reutilizar información anterior para suavizar el resultado final. Este tipo de técnicas empezaron originalmente en 1988 con *irradiance caching* [@irradiance-caching].

Un método muy sencillo que hemos estudiado es la [acumulación temporal](#antialiasing-mediante-jittering-y-acumulación-temporal), pero requiere que la cámara se quede estática. En la práctica se puede lo mismo con una escena en movimiento. Algunos de los algoritmos más famosos son *Spatio-Temporal Variance Guided Filter* (SVGF), *Spatial Denoising*, Adaptive SVGF (A-SVGF) y *Temporally dense ray tracing*.

Un algoritmo que se ha utilizado exitosamente en los últimos años es *Spatiotemporal importance Resampling* (ReSTIR) [@restir], el cual es capaz de procesar millones de luces dinámicas en tiempo real en alta calidad sin necesidad de introducir estructuras de datos excesivamente complejas. Está basado en el algoritmo *Sampling Importance Resampling* (STIR). Este último se puede estudiar a fondo en [@stir]

![ReSTIR en acción. Fuente: @restir](./img/07/restir.png){ #fig:restir }

La mayor parte de estas técnicas requieren el uso de un *motion buffer*, el cual calcula el cambio en la posición de un vértice de un *frame* a otro. En la práctica suelen venir acompañados de *motion vectors*, los cuales también son usados para técnicas como temporal antialiasing (TAA) o temporal upscaling [@temporal-supersampling] (TAA es una técnica de muestreo).

### *Machine Learning* y técnicas de *super sampling*

Para cualquier persona que haya seguido el mundo de la tecnología en los últimos años, no debe resultarle sorprendente la afirmación de que las técnicas basadas en machine learning están de moda. Nuestro campo de investigación, el transporte de luz, también se ha visto beneficiado por ellas.

En la actualidad el propósito de las redes neuronales suele ser reconstruir la imagen desde una resolución considerablemente menor: en vez de renderizar una imagen a, por ejemplo, 4K, se computa primero a 1080p o menor y luego se reescala mediante inteligencia artificial. Las tecnologías más famosas que existen de este estilo son Nvidia Deep Learning Super Sampling 2.0 [@dlss] e Intel XeSS [@xess]. AMD lanzó recientemente FidelityFX Super Resolution 2.0 (FSR) [@fsr], pero no utiliza redes neuronales. Sin embargo, la consideraremos en esta sección, pues sus resultados son muy similares.

**DLSS** recibe imágenes a baja resolución y *motion vectors*, produciendo imágenes a alta resolución mediante un autoenconder convolucional. Los resultados son espectaculares, y consiguen un rendimiento mucho mayor que a resolución nativa sin perder calidad de imagen. En algunos casos, la reconstrucción acaba teniendo mayor nitidez que la imagen original, pues la red neuronal aplica antialiasing en el proceso.

![Control, de Remedy Games. Uno de los primeros videojuegos que integraron ray tracing en tiempo real basado en DX12. El rendimiento se duplica al utilizar DLSS. Fuente: @df-dlss. \newline <br> **Izquierda**: DLSS 2.0 con resolución interna a 1080p. **Derecha**: 4K nativo.](./img/07/Control.jpg){ #fig:df-dlss width=70%}

**Intel XeSS** funciona de manera similar a DLSS, aunque todavía no se conocen los detalles. Su lanzamiento es extremadamente reciente, por lo que se están explorando los resultados. Se puede leer una entrevista realizada por Digital Foundry a los autores del proyecto en [@df-xess].

Los beneficios de estas técnicas son evidentes. Tal y como descubrimos en la comparativa, la [resolución](#resolución) afecta en gran medida al rendimiento. Cuantos más píxeles tenga la imagen, mayor será el número de muestras que debamos tomar; y por lo tanto, mayor el coste de renderizar un *frame*. Bajando la resolución conseguimos una imagen con menos ruido pero poco apta para las pantallas de hoy en día. Haciendo *super sampling* solventamos este problema. Si la reconstrucción es de suficiente calidad, estaremos consiguiendo rendimiento superior a la resolución nativa.

![Comparativa entre las diferentes técnicas de reconstrucción a 4K. Presta atención a cómo son reconstruidas las vallas, así como el texto de los globos. Fuente: @df-fsr. \newline <br> **Izquierda**: Nvidia DLSS 2.3. **Centro**: AMD FSR 2.0. **Derecha**: Temporal antialiasing.](./img/07/Comparación.jpg){  #fig:dlss-fsr-taa }

## La industria del videojuego

Con la llegada de ray tracing en tiempo gracias a la arquitectura Turing en 2018, un mundo de nuevas posibilidades se abrió ante los ojos de la industria. La más beneficiada fue, sin lugar a dudas, la de los videojuegos debido a sus limitaciones del *frame* budget.

### Productos comerciales

Es mandatorio que la imagen se produzca en un margen de tiempo muy reducido; como máximo, de 33 milisegundos.  Teniendo en cuenta esta estrechísimo margen, la mayor parte de las empresas se optó por una **solución híbrida**: en vez de utilizar ray tracing puramente, este algoritmo se reserva para ciertas partes de la pipeline de procesamiento. Entre estas se encuentran los **reflejos** y la **iluminación global** [@khonos-best-practices].

En vez de recaer en técnicas antiguas como reflejos en espacio de pantalla (*screen-space*) se utiliza una forma reducida de ray tracing para computar estos efectos. En el proceso final de la pipeline híbrida de rasterización y ray tracing se combina el resultado de ambas técnicas para producir la imagen final.

![En vez de utilizar *screen-space reflections*, los cuales sufren de los problemas clásicos de rasterización debido a su naturaleza (como el *fallback* a los cubemaps cuando el ángulo es demasiado agudo), ray tracing resuelve estos efectos de forma magistral. Fuente: [@df-spiderman]](./img/07/HybridRT.jpg){ #fig:spiderman-reflections width=70%}

Algunos productos comerciales que destacan por su uso de ray tracing híbrido son *Ratchet & Clank: Rift Apart* y *Spiderman* [Figura @fig:spiderman-reflections] de Insommiac Games, *Cyberpunk 2077* de CD Project Red, Control de Remedy [Figura @df-dlss].

No obstante, existen algunas implementaciones de motores que utilizan únicamente path tracing como algoritmo de renderizado. Entre estos, se cuentran *Quake II RTX* [@Q2RTX] o *Minecraft RTX*, del cual se puede encontrar un análisis técnico en el blog de [@alain-minecraft]. Los resultados gráficos son espectaculares, aunque se necesita un hardware gráfico considerable para poder ejecutarlos.

### Consolas de nueva generación

Parte de la transición de la industria a ray tracing se debe a las capacidades de las consolas de la actual generación: **Playstation 5** de Sony [@ps5], y **Xbox Series X** y **Series S** de Microsoft [@series-x]. Ambas fueron lanzadas a finales del año 2020. Las dos consolas utilizan la arquitectura de AMD denominada RDNA 2 de AMD, la misma que se usa en sus gráficas de escritorio de última generación. PS5 empaqueta 36 unidades de cómputo (*Compute Units* CUs), con una potencia de 10.23 TFLOPS para operaciones en coma flotante de 32 bits y 21.46 TFLOPS para las de 16. Por otra parte, Series X monta 52 CUs con una potencia teórica de 12.16 TFLOPSl, mientras que Series S reduce el número de CUs a 20.

En verano de 2020 Mark Cerny habló sobre cómo se utilizaría las capacidades de la arquitectura en el futuro de la consola [@cerny-ps5]. Entre estos usos, destaca el audio, la iluminación global, las sombras, los reflejos e incluso ray puro.

Hace media década, el hecho de tener ray tracing en tiempo real en gráficas de consumidor parecía imposible; más aún en consolas. Resulta, por tanto, una evolución enorme en una industria que se ha convertido en una de las más importantes del mundo.

## Unreal Engine 5 y Lumen

Unreal Engine 5 [@UE5] es la última generación del Motor *Unreal Engine* creado por Epic Games. Es un software extremadamente complejo que empaqueta múltiples funciones, como animación, físicas, renderizado, audio, y un largo etcétera. Se usa tanto en la industria de la animación como en la de los videojuegos.

Una de las novedades de esta versión es la integración de un sistema de iluminación global, llamado **Lumen**. Junto a **Nanite**, que es un sistema de geometría virtualizada, es la caracaterística más importante que han presentado. Recientemente han detallado cómo funciona Lumen [@lumen], así que estudiaremos qué técnicas han utilizado.

![Habitación interior iluminada por el sistema Lumen](./img/07/Lumen3.png){ #fig:lumen1  width=85% }

La [iluminación global](#iluminación-global) es el efecto que produce la luz cuando los fotones que la componen rebotan dentro de una escena. En el pasado (i.e., rasterización) se ha simulado mediante técnicas como *lightmap baking*, oclusión ambiental y similares. Sin embargo, como ya explicamos en secciones anteriores, estas técnicas presentan grandes limitaciones.

Lumen simula infinitos rebotes de la luz en una escena en tiempo real, actualizando la iluminación automáticamente Esto funciona tanto para interiores como exteriores, puesto que simula la luz proveniente del cielo. Además, el sistema tiene en cuenta materiales emisivos y la niebla volumétrica.

![Una escena exterior con materiales emisivos en Unreal Engine 5. Fuente: @lumen](./img/07/Lumen4.jpg){#fig:lumen2 width=85%}

Como era de esperar, Lumen utiliza ray tracing para hacer el cálculo de la iluminación. Para conseguirlo, el sistema crea una versión simplificada de la escena en la cual resulta más fácil hacer intersecciones de rayos con objetos.

Por defecto se ha optado una forma de ray tracing basada en software, pues de esta manera se soporta más tipos de hardware. Se combinan *Mesh Distance Fields* (mallas de polígonos que resultan eficientes a la hora de intersecar) mezcladas en un *Global Distance Field* en este tipo de ray tracing. No obstante, su versión de ray tracing basado en hardware es mandatoria para conseguir algunos efectos como reflejos especulares perfectos, pues de otra manera no se podrían conseguir en tiempo real.

Una optimización clave para que este sistema funcione de forma eficiente es el uso de *surface caching*. Trabajando en conjunto junto a Nanite, Lumen crea un atlas de texturas de aquellas mallas presentes alrededor del jugador de forma que se cubran las caras visibles desde algún punto de vista. Esto simplifica el coste de la evaluación del material.

![Visualización de *Mesh Distance Fields* de una escena. Fuente: @lumen](./img/07/Lumen1.jpg){#fig:lumen3 width=65%}

Para el paso final del cálculo de la radiancia se usa un algoritmo basado en radiance caching presentado en [@siggraph2021]. Como no resulta viable tirar rayos a diestro y siniestro por toda la escena, Lumen toma una pequeña porción de muestras y combina los cálculos de radiancia con información aportada por el material.

![Lumen combina los cálculos de la iluminación global con la información del material para crear la imagen final. Fuente: @lumen](./img/07/Lumen5.png){#fig:lumen4 width=80%}

Otro punto clave del sistema es la elección de las direcciones que se van a trazar. Para escogerlas, se comprueba qué partes del último *frame* resultaron muy brillantes. Entonces, se mandan rayos hacia esas zonas, pues esto producirá resultados menos ruidos en este *frame*. Esta técnica la hemos estudiado, y se llama muestreo por importancia (de la luz entrante en este caso).

> Este punto enlaza con nuestra [comparativa con In One Weekend](#comparativa-con-in-one-weekend). Una de las conclusiones más interesantes que obtuvimos fue que utilizar muestreo por importancia de las fuentes de iluminación proporcionaba unos resultados considerablemente mejores que una estrategia de muestreo basada en direcciones aleatorias. Aunque en este caso no se muestrean las fuentes de luz, sí que se utilizan partes de la escena con mucha radiancia, por lo que el fundamento es similar.

![Lumen utiliza las partes más brillantes del último *frame* para la distribución de las nuevas direcciones de un rayo tras el impacto. Fuente: @lumen](./img/07/Lumen6.png){#fig:lumen5}

Para calcular los reflejos de superficies se ha optado por promediar la radiancia de puntos en el mismo vecindario y acumulación temporal de *frames* para suavizar el resultado.

![Nuestro método principal de reducción de ruido fue la acumulación temporal de *frames*. Lumen utiliza una forma mucho más avanzada para lograr el mismo resultado con sus reflejos. Fuente: @lumen](./img/07/Lumen2.jpg){ #fig:lumen6 }

Finalmente, es importante destacar que las imágenes no son generadas a una gran resolución. Para conseguir una imagen digna de los tiempos actuales, los motores modernos utilizan una resolución interna cercana a 720p o 1080p que es escalada mediante algún método de *upsampling*. En este caso, se ha optado por *Temporal Super Resolution*, una técnica que comenzó con Unreal Engine 4 y ha evolucionado en esta versión. Su comportamiento es similar a DLSS o FSR.

Todas estas optimizaciones son esenciales para hacer que el motor corra en tiempo real. Aunque el rendimiento es extremadamente dependiente del tipo de contenido que se esté visualizando y los parámetros del sistema, Unreal Engine 5 puede correr a 60 FPS en consolas de la generación actual.