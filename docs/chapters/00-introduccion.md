# Introducción {.unnumbered}

Este trabajo puede visualizarse en la web [asmilex.github.io/Raytracing](https://asmilex.github.io/Raytracing/) o en el [PDF](https://github.com/Asmilex/Raytracing/raw/main/docs/TFG.pdf) disponible en el repositorio del trabajo [github.com/Asmilex/Raytracing](https://github.com/Asmilex/Raytracing). La página web contiene recursos adicionales como vídeos.

## Nota histórica {.unnumbered}

Ser capaces de capturar un momento.

Desde tiempos inmemoriales, este ha sido uno de los sueños de la humanidad. La capacidad de retener lo que ven nuestros ojos comenzó con simples pinturas ruprestres que nuestros ancestros dejaron enmarcadas en las paredes de sus hogares.

Con el tiempo, la tecnología evolucionó; lo cual propició formas más realistas de representar la realidad. El físico árabe Ibn al-Haytham, a finales de los años 900, describió el efecto de la cámara oscura [@pinhole], un efecto óptico mediante el cual se puede proyectar una imagen invertida en una pared. A inicios del siglo XVIII, Nicéphore Niépce consiguió arreglar una imagen capturada por las primeras cámaras [@history-photography]. Era una impresión primitiva, por supuesto; pero funcional. A finales de este siglo, sobre los años 1890, la fotografía se extendió rápidamente en el espacio del consumidor gracias a la compañía Kodak. Finalmente, a mediados del siglo XX la fotografía digital, la cual simplificaría muchos de los problemas de las cámaras tradicionales.

Una vez entró de lleno la era digital, los ordenadores personales se volvieron una herramienta indispensable. Con ellos, los usuarios eran capaces de mostrar imágenes en pantalla, que cambiaban bajo demanda. Naturalmente, debido a nuestro afán por recrear el mundo, nos hicimos una pregunta: ¿Podríamos **simular la vida real**?

Como era de esperar, este objetivo es complicado de lograr. Para conseguirlo, hemos necesitado crear abstracciones de conceptos que nos resultan naturales, como objetos, luces y seres vivos. *"Cosas"* que un ordenador no entiende, y sin embargo, para nosotros *funcionan*. Así, nació la geometría, los puntos de luces, texturas, sombreados, y otros elementos de un escenario digital. Pero estas abstracciones por sí mismas no son suficientes. Necesitamos visualizarlas.

Para solventar este problema existen diferentes algoritmos. El más primitivo es **rasterización**, una técnica utilizada para convertir objetos tridimensionales de una escena en un conjunto de píxeles. Proyectando acordemente el entorno a una cámara, conseguimos colorear una región del espacio, de forma que en conjunto representan un punto de vista de un mundo digital. Su simplicidad lo convierte en una manera extraordinariamente rápida de conseguir una imagen. Sin embargo, su gran inconveniente es la fidelidad. Debido a su naturaleza (que se basa en una simple proyección), este algoritmo está extremadamente limitado. Para aumentar el realismo del producto final, con el tiempo se idearon métodos como *shadow mapping*, precómputo de luces, o *reflection cubemaps*, los cuales intentan solventar el problema subyacente de rasterización: conocer el entorno de la escena.

Como era de esperar, se buscaron vías alternativas a rasterización para producir una imagen. La que más destacó fue **ray tracing**. Su primer uso documentado data de los años 60, en un artículo de Appel [@RT-history]. Parte de una idea increíblemente simple: consiste en disparar un rayo desde una cámara para comprobar si un objeto está ocluído. De esta forma, se resuelve el problema de conocer qué es lo que se ve desde la cámara.

Un par de décadas más tarde, sobre 1980, comienzan a ser publicadas imágenes hechas por ray tracing muy realistas. En estos años también se experimenta un crecimiento en el número de publicaciones sobre cómo hacer más rápido ray tracing. Uno de los puntos clave fue reducir el tiempo requerido para calcular intersecciones con objetos, pues suponen hasta el 95% del cómputo total. Kay y Kajiya publican un tipo de estructura denominada *bounding box* que simplifica este problema de manera considerable.

![Ray tracing en los años 80. Fuente: [@Whitted1979AnII]](./img/00/RT%20prehistórico.jpg){#fig:rt-original width=40%}

En 1986 Kajiya introdujo la denominada **rendering equation** [@kajiya]. Esta es una ecuación que modela analíticamente la cantidad de luz de un cierto basándose en las propiedades del material y la luz que llega a dicho punto.

$$
L_o(p, \omega_o) = L_e(p, \omega_o) + \int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) L_i(p, \omega_i) \cos\theta_i\ d\omega_i}
$$

Debido a la complejidad de esta ecuación, se diseñó un algoritmo denominado **path tracing**, el cual es capaz de estimar numéricamente su valor. Su idea principal se basa en hacer rebotar rayos por la escena una y otra vez, de forma que en cada impacto se adquiera nueva información.

Los métodos de Monte Carlo proliferaron debido a su fundamento teórico, el cual es idóneo para las diferentes formas de ray tracing. Estas técnicas se basan en el uso de muestras de alguna distribución para calcular un valor determinado. En este caso, aproximan el valor que toma la integral de la rendering equation. Comenzaron siendo utilizados en 1960 para el cálculo de la radiancia generada por los fotones en simulaciones físicas, por lo que transicionaron fácilmente a ray tracing.

A finales del siglo XX path tracing penetra de lleno en la industria. Numerosas empresas comienzan a desarrollar motores de renderizado basado en dicho algoritmo, abandonando así rasterización. Las productoras de cine empiezan a utilizar exclusivamente medios digitales para crear películas, una forma de crear arte nunca vista hasta la fecha.

La elegancia de path tracing reside en su naturaleza tan intuitiva. Pues claro que la respuesta a "*¿Cómo simulamos fielmente una imagen en un ordenador?*" es "*Representando la luz de forma realista*". Gracias a la física sabemos que los fotones emitidos por las fuentes de iluminación se mueven por el espacio impactando en los diferentes objetos. Y, como ocurre a menudo, la respuesta a muchos de nuestros problemas ya existe en el mundo exterior. Aprendiendo sobre cómo funciona nuestro alrededor nos permitirá modelar nuevos mundos a nuestro gusto. De esta manera, podemos dejar atrás los *hacks* que utilizábamos en rasterización; no habrá necesidad de falsificar los efectos de iluminación, puesto que path tracing los solventa de manera natural.

Aún con todos los avances del medio, **el elefante en la sala seguía siendo el rendimiento**. Producir una única imagen podría suponer horas de cómputo; incluso días. A diferencia del universo, nosotros no podemos permitirnos el lujo de usar un elevado número de fotones, ni hacer rebotar la luz tantas veces como queramos. Nos pasaríamos una eternidad esperando. Y para ver una imagen en nuestra pantalla necesitaremos estar vivos, claro. En la práctica, esto supuso que no todos los medios pudieron pasarse a path tracing. Aquellas industrias como la de los videojuegos, en las que se prioriza la rapidez sobre fidelidad continuaron, tuvieron que continuar usando rasterización. A fin de cuentas, solo disponen de unos escasos milisegundos para renderizar una imagen.

Sin embargo, el paso del tiempo es imparable. Las pinturas rupestres dieron paso al óleo sobre lienzo, mientras que las cámaras digitales reemplazaron a las oscuras. Es natural esperar que, en algún momento, rasterización se convierta en un algoritmo del pasado. Y ese momento es la actualidad.

En 2018 Nvidia introdujo la arquitectura de tarjetas gráficas Turing [@turing-arquitecture]. Aunque por estos años ya existían implementaciones de ray tracing en gráficas (tan temprano como [@10.1145/566654.566640], [@Ertl_theray]), esta arquitectura presenta la capacidad de realizar cómputos específicos de ray tracing acelerados por hardware en gráficas de consumidor. Esto significa que **path tracing se vuelve viable en tiempo real**. En lugar de horas, renderizar una imagen costará milisegundos.

Se da el pistoletazo de salida a una nueva transición.

## Objetivos del trabajo {.unnumbered}

En este trabajo se estudiarán los **fundamentos de path tracing en tiempo real**. Para conseguirlo, se han propuesto los siguientes objetivos:

- Analizar los algoritmos modernos de visualización en 3D basados en métodos de Monte Carlo, entre los que se encuentra path tracing.
- Revisar de las técnicas de Monte Carlo, examinando puntos fuertes y débiles de cada una. Se buscará minimizar tanto el error en la reconstrucción de la imagen como el tiempo de ejecución.
- Implementar dichos algoritmos en una tarjeta gráfica moderna específicamente diseñada para acelerar los cálculos de ray tracing.
- Diseñar e implementar de un software de síntesis de imágenes realistas por path tracing y muestreo directo de fuentes de luz por GPU.
- Analizar el rendimiento del motor con respecto al tiempo de ejecución y calidad de imagen.
- Comparación del motor desarrollado con una implementación por CPU.
- Investigación de las técnicas modernas y sobre el futuro del área.

Afortunadamente, **se ha conseguido realizar exitosamente cada uno de los objetivos**. Esta memoria cubrirá todo el trabajo que ha sido necesario realizar para lograrlo.

## Sobre esta memoria {.unnumbered}

Esta memoria recapitulará todas las técnicas utilizadas para resolver el problema propuesto. En los primeros capítulos, estudiaremos los fundamentos teóricos, mientras que en los posteriores construiremos una implementación de ray tracing, la cual analizaremos con detalle para finalizar.

El [capítulo 1](#las-bases) **sentará las bases de ray tracing**: qué es un rayo exactamente, cómo podemos representarlo matemáticamente y cuáles son las ecuaciones que nos permiten modelar la propagación e impacto con diferentes objetos.

En el [capítulo 2](#transporte-de-luz) introduciremos los fundamentos de **la radiometría y el transporte de luz**, el área de la física que se encarga de la interacción entre la luz y la materia. Estudiaremos cómo funcionan los fotones, cómo son emitidos desde las denominadas fuentes de iluminación, y cómo se propagan por el medio. Para conseguirlo, necesitaremos construir ciertas abstracciones que representen propiedades radiométricas. Entre las más importantes se encuentran la potencia, la intensidad radiante y la radiancia. También será necesario el concepto de ángulos sólidos, los cuales generalizan la concepción clásica de ángulo planar.

Será entonces cuando aprendamos cómo interacciona la luz con la materia. Esto nos llevará a crear una familia específica de funciones denominadas *bidirectional distribution functions*, las cuales describen cómo cambian los fotones cuando impactan con una superficie. Asimismo, será importante conocer la dirección de salida de estos, por lo que habrá que estudiar los fenómenos de reflexión y refracción. Al final del capítulo obtendremos la ecuación del transporte de luz o *rendering equation*.

Esta ecuación modela fielmente cuánta luz existe en un punto dependiendo de su entorno. Sin embargo, es prácticamente imposible resolverla analíticamente. Por ello, con el fin de poder realizar los cálculos en tiempo real, en el [capítulo 3](#métodos-de-monte-carlo) exploraremos las **técnicas de Monte Carlo**. Estas técnicas se basan en el uso de muestreo aleatorio. A partir de promediar muestras de una variable aleatoria seremos capaces de determinar, primero, la media de una transformación de una v.a.; y después, el valor de una integral. Esto nos permitirá estimar el valor de la ecuación del transporte de luz.

Sin embargo, muestrear sin cabeza no producirá resultados especialmente buenos. Por ello, comprobaremos cómo algunas técnicas reducen la varianza del estimador de Monte Carlo; y con ello, el ruido de la imagen final. Entre los métodos estudiados se encuentran el muestreo (múltiple) por importancia, la ruleta rusa, el muestreo directo de fuentes de iluminación o los métodos de Quasi-Monte Carlo. Todos estos los acabaremos enfocando al área que estamos explorando.

Cuando hayamos acabado con la teoría física y matemática, será el momento de producir la aplicación. El [capítulo 4](#construyamos-un-path-tracer) cubirá la **construcción de un motor de renderizado** físicamente realista. Presentaremos algunas herramientas clave en la resolución del problema; entre las que se encuentran Vulkan, una interfaz de programación de aplicaciones gráfica, y el framework para Vulkan Ray tracing de Nvidia DesignWorks denominado *nvpro-samples*.

Debido a la gran complejidad del problema, será necesario introducir algunas estructuras clave que habilitan la ejecución de ray tracing en tiempo real. Las dos más destacables son la *Top* y *Bottom-Level Acceleration Structures* (TLAS y BLAS, respectivamente), que albergan información sobre la geometría de una escena; y la *Shader Binding Table* (SBT), una estructura que permite seleccionar shaders dinámicamente a partir de la intersección de los rayos.

Será entonces cuando hablemos de cómo funciona la *ray tracing pipeline*, qué tipos de shaders existen y cómo podemos usarlos para renderizar una escena. Además, hablaremos sobre cómo hemos representado los materiales y las fuentes de iluminación. Finalmente, veremos algunas técnicas adicionales que hemos usado para reducir el ruido de la imagen, como la corrección de gamma y acumulación temporal de frames.

Una vez tengamos un motor funcional, será hora de jugar con él. El [capítulo 5](#análisis-de-rendimiento) mostrará los **resultados de nuestro trabajo**. Visualizaremos algunos fenómenos físicos que hemos estudiado a lo largo de la memoria. Estos serán encapsulados en aproximadamente una docena de escenas, centrándose cada una en cierta particularidad; como puede ser la iluminación global, la refracción y reflexión, o el comportamiento de materiales específicos.

También analizaremos cómo rinde el motor en términos de calidad visual de imagen y tiempo de renderizado. Para ello, comprobaremos cómo varían estas dos métricas al cambiar los parámetros del algoritmo path tracing y las técnicas de reducción de ruido. Entre estos parámetros, se encuentran el número de muestras del estimador de Monte Carlo, la profunidad de rebotes de un rayo, la acumulación temporal y la resolución.

Para poner en contexto el rendimiento, compararemos nuestra implementación con el path tracer desarollado en los libros de Peter Shirley *Ray Tracing In One Weekend* series [@Shirley2020RTW1] [@Shirley2020RTW2] [@Shirley2020RTW1].

Terminaremos el grueso del trabajo con el [capítulo de conclusiones](#conclusiones). Reflexionaremos sobre todo lo que hemos aprendido; desde los éxitos logrados hasta las dificultades que presenta este complejo algoritmo. Además, explicaremos cómo se podría mejorar este trabajo, analizando de qué pie cojea la implementación.

En los anexos se puede encontrar contenido adicional. El [primer anexo](#el-presente-y-futuro-de-ray-tracing) cubre el **estado del arte** del área --aunque este trabajo es prácticamente estado del arte--. En el [segundo](#metodología-de-trabajo) hablaremos sobre **cómo se ha realizado este trabajo**: desde las principales influencias hasta el diseño gráfico; pasando por el presupeusto y los ciclos de desarrollo. Por último, se ha incluído un glosario de [términos y conceptos](#glosario-de-términos) para facilitar la lectura de este documento.

## Principales fuentes consultadas {.unnumbered}

Aunque en la realización de este trabajo se han utilizado múltiples fuentes de información, destacan una serie de libros por encima del resto:

- La colección de libros digitales de Peter Shirley  *Ray Tracing In One Weekend* [@Shirley2020RTW1] [@Shirley2020RTW2] [@Shirley2020RTW3]. En esencia, han sido la inspiración de todo este proyecto. Se han utilizado como introducción al área y para implementar algunos de los métodos que veremos en futuras secciones; así como comparativa final con nuestro motor. Esto significa que aparecerán múltiples veces en la memoria.
- *Physically Based Rendering: From Theory to Implementation (3rd ed.)* [@PBRT3e]. Considerado por muchos como el santo grial de la informática gráfica moderna. El capítulo "Transporte de luz" está fielmente basado en el trabajo de este libro. Además, algunas de las técnicas del capítulo "Técnicas de Monte Carlo" utilizan sus contenidos.
- La teoría de Monte Carlo ha sido sintetizada principalmente de *Métodos de Monte Carlo* [@metodos-monte-carlo] y de *Monte Carlo Theory, Methods and Examples* [@mcbook].
- *Ray Tracing Gems I* [@Haines2019] y *Ray Tracing Gems II* [@Marrs2021] una colección de artículos esenciales sobre Ray Tracing publicada por Nvidia. Una enorme variedad de expertos en el medio han participado en estos dos libros.
- Aunque no han sido tan decisivos, existen muchos otros recursos que han ayudado a solidificar y cohesionar el trabajo. Entre estos, se encuentran libros como *Graphics Codex* [@McGuire2018GraphicsCodex] y *Realistic Ray Tracing* [@ShirleyRRT]; así como los cursos *Computer Graphics and Imaging* [@berkeley-cs184], *Fundamentals of Computer Graphics* [@pellacini-marschner-2017], y *Lecture Rendering* [@tuwien]. Además, la recolección de técnicas y estudios de Alain Galvan han supuesto un gran refuerzo [@alain-API] [@alain-materials]  [@alain-debug] [@alain-denoising] [@alain-filtering].