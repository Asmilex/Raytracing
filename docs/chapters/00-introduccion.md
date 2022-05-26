# Introducción {.unnumbered}

Este trabajo puede visualizarse en la web [asmilex.github.io/Raytracing](https://asmilex.github.io/Raytracing/) o en el [PDF](https://github.com/Asmilex/Raytracing/raw/main/docs/TFG.pdf) disponible en el repositorio del trabajo [github.com/Asmilex/Raytracing](https://github.com/Asmilex/Raytracing). La página web contiene recursos adicionales como vídeos.

## Nota histórica V2 {.unnumbered}

De [@RT-history] y [@PBRT3e]

- Primer uso documentado de Ray tracing por Appel en los años 60. Dispara un rayo desde una cámara estenopeica para comprobar si un objeto está ocluído. Al mismo tiempo y por separado, se usan técnicas de MC para hacer simulaciones de luz, en las cuales se cuenta el número de fotones.
- Durante los años 70 salen muchos modelos de iluminación, como el de Phong y Gouraud.
- En los años 80 empiezan a salir imágenes hechas por ray tracing muy realistas. En estos años también se experimenta un gran crecimiento de técnicas para acelerar el trazado. Se dan cuenta de que el bottleneck es la intersección con objetos (95% del tiempo de cómputo). Bounding boxes de Kay y Kajiya. Empiezan a proliferar los métodos de MC para ray tracing. Cook et. al. y Kajiya asientan rendering equation.
- Años 90 RT se vuelve mainstream, con muchos motores comerciales utilizados por una gran variedad de empresas de diferentes sectores.

Fuente de lo siguiente: yo

- RT prácticamente inviable en tiempo real. Sectores como videojuegos utilizan rasterización. Se implementan técnicas basadas en *backeo* de fuentes con mapas de texturas prerenderizadas.
- Nvidia introduce Ray tracing acelerado por hardware en gráficas de consumidor (2018). Diferentes formas de ray tracing se vuelven viables en escasos milisegundos.
- Industria empieza a abandonar rasterización para algunos métodos. Enlazar a estado del arte.

![Ray tracing en los años 80. Fuente: [@Whitted1979AnII]](./img/00/RT%20prehistórico.jpg)



## Nota histórica {.unnumbered}

Ser capaces de capturar un momento.

Desde tiempos inmemoriales, este ha sido uno de los sueños de la humanidad. La capacidad de retener lo que ven nuestros ojos comenzó con simples pinturas ruprestres que nuestros ancestros dejaron enmarcadas en las paredes de sus hogares.

Con el tiempo, la tecnología evolucionó; lo cual propició formas más realistas de representar la realidad. El físico árabe Ibn al-Haytham, a finales de los años 900, describió el efecto de la cámara oscura [@pinhole], un efecto óptico mediante el cual se puede proyectar una imagen invertida en una pared. A inicios del siglo XVIII, Nicéphore Niépce consiguió arreglar una imagen capturada por las primeras cámaras [@history-photography]. Era una impresión primitiva, por supuesto; pero funcional. A finales de este siglo, sobre los años 1890, la fotografía se extendió rápidamente en el espacio del consumidor gracias a la compañía Kodak. Finalmente, a mediados del siglo XX la fotografía digital, la cual simplificaría muchos de los problemas de las cámaras tradicionales.






Hablando de digital. Los ordenadores personales modernos nacieron unos años más tarde. Los usuarios eran capaces de mostrar imágenes en pantalla, que cambiaban bajo demanda. Y, entonces, nos hicimos una pregunta...

¿Podríamos **simular la vida real** para mostrarla en pantalla?

Como era de esperar, esto es complicado de lograr. Para conseguirlo, hemos necesitado crear abstracciones de conceptos que nos resultan naturales, como objetos, luces y seres vivos. *"Cosas"* que un ordenador no entiende, y sin embargo, para nosotros *funcionan*.

Así, nació la geometría, los puntos de luces, texturas, sombreados, y otros elementos de un escenario digital. Pero, por muchas abstracciones elegantes que tengamos, no nos basta. Necesitamos visualizarlas. Y como podemos imaginarnos, esto es un proceso costoso.

La **rasterización** es el proceso mediante el cual estos objetos tridimensionales se transforman en bidimensionales. Proyectando acordemente el entorno a una cámara, conseguimos colorear un pixel, de forma que represente lo que se ve en ese mundo.

Aunque esta técnica es bastante eficiente en términos de computación y ha evolucionado mucho, rápidamente saturamos sus posibilidades. Conceptos como *shadow maps*, *baked lightning*, o *reflection cubemaps* intentan solventar lo que no es posible con rasterización: preguntrarnos *qué es lo que se encuentra alrededor nuestra*.

En parte, nos olvidamos de la intuitiva realidad, para centrarnos en aquello computacionalmente viable.

Y, entonces, en 1960 el trazado de rayos con una simple idea intuitiva.

## ¿Qué es ray tracing? {.unnumbered}

En resumidas cuentas, *ray tracing* (o trazado de rayos en español), se basa en disparar fotones en forma de rayo desde nuestra cámara digital y hacerlos rebotar en la escena.

De esta forma, simulamos cómo se comporta la luz. Al impactar en un objeto, sufre un cambio en su trayectoria. Este cambio origina nuevos rayos, que vuelven a dispersarse por la escena. Estos nuevos rayos dependerán de las propiedades del objeto con el que hayan impactado. Con el tiempo necesario, lo que veremos desde nuestra cámara será una representación fotorealista de lo que habita en ese universo.

Esta técnica, tan estúpidamente intuitiva, se ha hecho famosa por su simpleza y su elegancia. *Pues claro* que la respuesta a "*¿Cómo simulamos fielmente una imagen en un ordenador?*" es "*Representando la luz de forma realista*".

Aunque, quizás intuitiva no sea la palabra. Podemos llamarla *natural*, eso sí. A fin de cuentas, fue a partir del siglo XVIII cuando empezamos a entender que podíamos capturar la luz. Nuestros antepasados tenían teorías, pero no podían explicar por qué *veíamos* el mundo.

Ahora sí que sabemos cómo funciona. Entendiendo el por qué lo hace nos permitirá programarlo. Y, resulta que funciona impresionantemente bien.

Atrás se quedan los *hacks* necesarios para rasterización. Los cubemaps no son esenciales para los reflejos, y no necesitamos cámaras virtuales para calcular sombras. Ray tracing permite simular fácilmente efectos como reflejos, refracción, desenfoque de movimiento, aberración cromática... Incluso fenómenos físicos propios de las particulas y las ondas.

> Espera. Si tan bueno es, ¿por qué no lo usamos en todos lados?

Por desgracia, el elefante en la sala es el rendimiento. Como era de esperar, disparar rayos a diestro y siniestro es costoso. **Muy costoso**.

A diferencia del universo, nosotros no nos podemos permitir el lujo de usar fotones de tamaño infinitesimal y dispersiones casi infinitas. Nos pasaríamos una eternidad esperando. Y para ver una imagen en nuestra pantalla necesitaremos estar vivos, claro.

Debemos evitar la fuerza bruta. Dado que la idea es tan elegante, la respuesta no está en el *"qué"*, sino en el *"cómo"*. Si **disparamos y dispersamos rayos con cabeza** seremos capaces de obtener lo que buscamos en un tiempo razonable.

Hace unos años, al hablar de tiempo razonable, nos referiríamos a horas. Quizás días. Producir un *frame* podría suponer una cantidad de tiempo impensable para un ordenador de consumidor. Hoy en día también ocurre esto, claro está. Pero la tecnología evoluciona.

Podemos bajarlo a milisegundos.

Hemos entrado en la era del **real time ray tracing**.

## Objetivos del trabajo {.unnumbered}

Los objetivos del trabajo iniciales son los siguientes:

- Análisis de los algoritmos modernos de visualización en 3D basados en métodos de Monte Carlo.
- Revisión de las técnicas de Monte Carlo, examinando puntos fuertes y débiles de cada una. Se busca minimizar el error en la reconstrucción de la imagen y minimizar el tiempo de ejecución.
- Implementación de dichos algoritmos en hardware gráfico moderno (GPUs) específicamente diseñado para aceleración de ray tracing.
- Diseño e implementación de un software de síntesis de imágenes realistas por path tracing y muestreo directo de fuentes de luz por GPU.
- Análisis del rendimiento del motor con respecto al tiempo de ejecución y calidad de imagen.
- Comparación del motor desarrollado con una implementación por CPU.
- Investigación de las técnicas modernas y sobre el futuro del área.

Afortunadamente, **se ha conseguido realizar exitosamente cada uno de los objetivos**. Esta memoria cubrirá todo el trabajo que ha sido necesario realizar para lograrlo.

## Técnicas empleadas para la resolución {.unnumbered}

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