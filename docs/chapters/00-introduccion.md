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

![Ray tracing en los años 80. Fuente: [@Whitted1979AnII]](./img/00/RT%20prehistórico.jpg)

En 1986 Kajiya introdujo la denominada **rendering equation** [@PBRT3e]. Esta es una ecuación que modela analíticamente la cantidad de luz de un cierto basándose en las propiedades del material y la luz que llega a dicho punto.

$$
L_o(p, \omega_o) = L_e(p, \omega_o) + \int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) L_i(p, \omega_i) \cos\theta_i\ d\omega_i}
$$

Debido a la complejidad de esta ecuación, se diseñó un algoritmo denominado **path tracing**, el cual es capaz de estimar numéricamente su valor. Su idea principal se basa en hacer rebotar rayos por la escena una y otra vez, de forma que en cada impacto se adquiera nueva información.

Los métodos de Monte Carlo proliferaron debido a su fundamente teórico, el cual es idóneo para ray tracing. Estas técnicas se basan en el uso de muestras de alguna distribución para calcular un valor determinado. En este caso, aproximan el valor que toma la integral de la rendering equation. Comenzaron siendo utilizados en 1960 para el cálculo de la radiancia generada por los fotones en simulaciones físicas, por lo que transicionaron fácilmente a ray tracing.

A finales del siglo XX ray tracing penetra de lleno en la industria. Numerosas empresas comienzan a desarrollar motores de renderizado basado en ray tracing, abandonando así rasterización. Las productoras de cine empiezan a utilizar exclusivamente medios digitales para crear películas, una forma de crear arte nunca vista hasta la fecha.

La elegancia de ray tracing reside en su naturaleza tan intuitiva. Pues claro que la respuesta a "*¿Cómo simulamos fielmente una imagen en un ordenador?*" es "*Representando la luz de forma realista*". Gracias a la física sabemos que los fotones emitidos por las fuentes de iluminación se mueven por el espacio impactando en los diferentes objetos. Y, como ocurre a menudo, la respuesta a muchos de nuestros problemas ya existe en el mundo exterior. Aprendiendo sobre cómo funciona nuestro alrededor nos permitirá modelar nuevos mundos a nuestro gusto. De esta manera, podemos dejar atrás los *hacks* que utilizábamos en rasterización; no habrá necesidad de falsificar los efectos de iluminación, puesto que ray tracing los solventa de manera natural.

Aún con todos los avances del medio, **el elefante en la sala seguía siendo el rendimiento**. Producir una única imagen podría suponer horas de cómputo; incluso días. A diferencia del universo, nosotros no podemos permitirnos el lujo de usar un elevado número de fotones, ni hacer rebotar la luz tantas veces como queramos. Nos pasaríamos una eternidad esperando. Y para ver una imagen en nuestra pantalla necesitaremos estar vivos, claro. En la práctica, esto supuso que no todos los medios pudieron pasarse a ray tracing. Aquellas industrias como la de los videojuegos, en las que se prioriza la rapidez sobre fidelidad continuaron, tuvieron que continuar usando rasterización. A fin de cuentas, solo disponen de unos escasos milisegundos para renderizar una imagen.

Sin embargo, el paso del tiempo es imparable. Las pinturas rupestres dieron paso al óleo sobre lienzo, mientras que las cámaras digitales reemplazaron a las oscuras. Es natural esperar que, en algún momento, rasterización se convierta en un algoritmo del pasado. Y ese momento es la actualidad.

En 2018 Nvidia introdujo la arquitectura de tarjetas gráficas Turing [@turing-arquitecture]. Esta arquitectura tiene la capacidad de realizar cómputos de ray tracing acelerados por hardware en gráficas de consumidor. Esto significa que ray **tracing se vuelve viable en tiempo real**. En lugar de horas, renderizar una imagen costará milisegundos.

Se da el pistoletazo de salida a una nueva transición.

## Objetivos del trabajo {.unnumbered}

En este trabajo se estudiarán los **fundamentos de ray tracing en tiempo real**. Para conseguirlo, se han propuesto los siguientes objetivos:

- Analizar los algoritmos modernos de visualización en 3D basados en métodos de Monte Carlo, entre los que se encuentra path tracing.
- Revisar de las técnicas de Monte Carlo, examinando puntos fuertes y débiles de cada una. Se buscará minimizar tanto el error en la reconstrucción de la imagen como el tiempo de ejecución.
- Implementar dichos algoritmos en una tarjeta gráfica moderna específicamente diseñada para acelerar los cálculos de ray tracing.
- Diseñar e implementar de un software de síntesis de imágenes realistas por path tracing y muestreo directo de fuentes de luz por GPU.
- Analizar el rendimiento del motor con respecto al tiempo de ejecución y calidad de imagen.
- Comparación del motor desarrollado con una implementación por CPU.
- Investigación de las técnicas modernas y sobre el futuro del área.

Afortunadamente, **se ha conseguido realizar exitosamente cada uno de los objetivos**. Esta memoria cubrirá todo el trabajo que ha sido necesario realizar para lograrlo.

## Sobre esta memoria {.unnumbered}


Además del antedicho algoritmo ray tracing y su versión más pura path tracing, se han empleado técnicas de Monte Carlo para calcular la luz resultante de un punto.

En particular, con respecto a la [matemática](#integración-de-monte-carlo) empleada, estudiaremos diferentes formas de generar números aleatorios mediante distribuciones particulares, *(multiple) importance sampling*, next event estimation, ...

En un área híbrida se encuentra la [radiometría](#transporte-de-luz). Dado que estamos tratando con transporte de luz, será esencial introducir los conceptos más importantes de la radiometría. Trataremos con algunos términos como irradiancia, ángulos sólidos, radiancia, funciones de distribuciones de reflectancia y transmitancia bidireccionales, etc.

Finalmente, la parte [informática](#construyamos-un-path-tracer) usará en la API gráfica Vulkan junto a un framework de Nvidia para acelerar la adopción de ray tracing en KHR. Veremos qué se necesita para implementar ray tracing en tiempo real, lo que nos llevará aprender sobre programación en Vulkan, las estructuras de aceleración de nivel alto y bajo (TLAS y BLAS), la Shader Binding Table, comunicación con CPU y GPU, etc.

Todo este programa estará alojado en Github. En el [apéndice](#metodología-de-trabajo), aprenderemos cómo se ha usado la plataforma para integrar la documentación, el código fuente y los ciclos de desarrollo.

Como podemos ver, esta área relaciona íntimamente la matemática y la informática, con un poco de física de por medio.

## Sobre esta memoria {.unnumbered}

## Principales fuentes consultadas {.unnumbered}

Aunque en la realización de este trabajo se han utilizado múltiples fuentes de información, destacan una serie de libros por encima del resto:

- La colección de libros digitales de Peter Shirley  *Ray Tracing In One Weekend* [@Shirley2020RTW1] [@Shirley2020RTW2] [@Shirley2020RTW3]. En esencia, han sido la inspiración de todo este proyecto. Se han utilizado como introducción al área y para implementar algunos de los métodos que veremos en futuras secciones; así como comparativa final con nuestro motor. Esto significa que aparecerán múltiples veces en la memoria.
- *Physically Based Rendering: From Theory to Implementation (3rd ed.)* [@PBRT3e]. Considerado por muchos como el santo grial de la informática gráfica moderna. El capítulo "Transporte de luz" está fielmente basado en el trabajo de este libro. Además, algunas de las técnicas del capítulo "Técnicas de Monte Carlo" utilizan sus contenidos.
- La teoría de Monte Carlo ha sido sintetizada principalmente de *Métodos de Monte Carlo* [@metodos-monte-carlo] y de *Monte Carlo Theory, Methods and Examples* [@mcbook].
- *Ray Tracing Gems I* [@Haines2019] y *Ray Tracing Gems II* [@Marrs2021] una colección de artículos esenciales sobre Ray Tracing publicada por Nvidia. Una enorme variedad de expertos en el medio han participado en estos dos libros.
- Aunque no han sido tan decisivos, existen muchos otros recursos que han ayudado a solidificar y cohesionar el trabajo. Entre estos, se encuentran libros como *Graphics Codex* [@McGuire2018GraphicsCodex] y *Realistic Ray Tracing* [@ShirleyRRT]; así como los cursos *Computer Graphics and Imaging* [@berkeley-cs184], *Fundamentals of Computer Graphics* [@pellacini-marschner-2017], y *Lecture Rendering* [@tuwien].