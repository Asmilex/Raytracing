# Introducción

Este trabajo puede visualizarse en la web [asmilex.github.io/Raytracing](https://asmilex.github.io/Raytracing/) o en el [PDF](https://github.com/Asmilex/Raytracing/raw/main/docs/TFG.pdf) disponible en el repositorio del trabajo [Asmilex/Raytracing](https://github.com/Asmilex/Raytracing).

La página web contiene la versión más actualizada, además de recursos adicionales como vídeos.

## Nota histórica

Ser capaces de capturar un momento.

Desde siempre, este ha sido uno de los sueños de la humanidad. La capacidad de retener lo que ven nuestros ojos comenzó con simples pinturas ruprestres. Con el tiempo, el arte evolucionó, así como la capacidad de retratar nuestra percepción con mayor fidelidad.

A inicios del siglo XVIII, se caputaron las primeras imágenes con una cámara gracias a Nicéphore Niépce. Sería una imagen primitiva, claro; pero era funcional. Gracias a la compañía Kodak, la fotografía se extendió al consumidor rápidamente sobre 1890. Más tarde llegaría la fotografía digital, la cual simplificaría muchos de los problemas de las cámaras tradicionales.

Hablando de digital. Los ordenadores personales modernos nacieron unos años más tarde. Los usuarios eran capaces de mostrar imágenes en pantalla, que cambiaban bajo demanda. Y, entonces, nos hicimos una pregunta...

¿Podríamos **simular la vida real** para mostrarla en pantalla?

Como era de esperar, esto es complicado de lograr. Para conseguirlo, hemos necesitado crear abstracciones de conceptos que nos resultan naturales, como objetos, luces y seres vivos. *"Cosas"* que un ordenador no entiende, y sin embargo, para nosotros *funcionan*.

Así, nació la geometría, los puntos de luces, texturas, sombreados, y otros elementos de un escenario digital. Pero, por muchas abstracciones elegantes que tengamos, no nos basta. Necesitamos visualizarlas. Y como podemos imaginarnos, esto es un proceso costoso.

La **rasterización** es el proceso mediante el cual estos objetos tridimensionales se transforman en bidimensionales. Proyectando acordemente el entorno a una cámara, conseguimos colorear un pixel, de forma que represente lo que se ve en ese mundo.

> TODO insertar imagen rasterización.
>
> NOTE ¿quizás debería extender un poco más esta parte? Parece que se queda algo coja la explicación.

Aunque esta técnica es bastante eficiente en términos de computación y ha evolucionado mucho, rápidamente saturamos sus posibilidades. Conceptos como *shadow maps*, *baked lightning*, o *reflection cubemaps* intentan solventar lo que no es posible con rasterización: preguntrarnos *qué es lo que se encuentra alrededor nuestra*.

En parte, nos olvidamos de la intuitiva realidad, para centrarnos en aquello computacionalmente viable.

Y, entonces, en 1960 el trazado de rayos con una simple idea intuitiva.

## ¿Qué es ray tracing?

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

## Objetivos del trabajo

Los objetivos del trabajo iniciales son los siguientes:

- Análisis de los algoritmos modernos de visualización en 3D basados en métodos de Monte Carlo.
- Revisión de las técnicas de Monte Carlo, examinando puntos fuertes y débiles de cada una. Se busca minimizar el error en la reconstrucción de la imagen y minimizar el tiempo de ejecución.
- Implementación de dichos algoritmos en hardware gráfico moderno (GPUs) específicamente diseñado para aceleración de ray tracing.
- Diseño e implementación de un software de síntesis de imágenes realistas por path tracing y muestreo directo de fuentes de luz por GPU.
- Análisis del rendimiento del motor con respecto al tiempo de ejecución y calidad de imagen.
- Comparación del motor desarrollado con una implementación por CPU.
- Investigación de las técnicas modernas y sobre el futuro del área.

> TODO: determinar si lo siguiente es cierto.

Afortunadamente, **se ha conseguido realizar exitosamente cada uno de los objetivos**. Esta memoria cubrirá todo el trabajo que ha sido necesario realizar para lograrlo.

## Técnicas empleadas para la resolución

> TODO: echarle un ojo a esto cuando termine el trabajo.

Además del antedicho algoritmo ray tracing y su versión más pura path tracing, se han empleado técnicas de Monte Carlo para calcular la luz resultante de un punto.

En particular, con respecto a la [matemática](#integración-de-monte-carlo) empleada, estudiaremos diferentes formas de generar números aleatorios mediante distribuciones particulares, *(multiple) importance sampling*, next event estimation, ...

En un área híbrida se encuentra la [radiometría](#transporte-de-luz). Dado que estamos tratando con transporte de luz, será esencial introducir los conceptos más importantes de la radiometría. Trataremos con algunos términos como irradiancia, ángulos sólidos, radiancia, funciones de distribuciones de reflectancia y transmitancia bidireccionales, etc.

Finalmente, la parte [informática](#construyamos-un-path-tracer) usará en la API gráfica Vulkan junto a un framework de Nvidia para acelerar la adopción de ray tracing en KHR. Veremos qué se necesita para implementar ray tracing en tiempo real, lo que nos llevará aprender sobre programación en Vulkan, las estructuras de aceleración de nivel alto y bajo (TLAS y BLAS), la Shader Binding Table, comunicación con CPU y GPU, etc.

Todo este programa estará alojado en Github. En el [apéndice](#metodología-de-trabajo), aprenderemos cómo se ha usado la plataforma para integrar la documentación, el código fuente y los ciclos de desarrollo.

Como podemos ver, esta área relaciona íntimamente la matemática y la informática, con un poco de física de por medio.

## Principales fuentes consultadas

Esencialmente, este trabajo ha sido posible gracias a los siguientes recursos:

- La serie de libros de *Ray Tracing* de *Peter Shirley*, conocidos como "Ray tracing In One Weekend Series" [@Shirley2020RTW1], [@Shirley2020RTW2], [@Shirley2020RTW3]. El motor desarrollado en estos libros es el que se utilizará para la comparación.
- Physically Based Rendering: From Theory to Implementation (3rd ed.) [@PBRT3e], considerado como el santo grial de la informática gráfica moderna.
- Ray Tracing Gems I y II [@Haines2019], [@Marrs2021], una colección de papers esenciales sobre ray tracing publicada por Nvidia.
- El autor [Károly Zsolnai, de Two Minute Papers](https://users.cg.tuwien.ac.at/zsolnai/). No solo ha inspirado parte del trabajo, sino que su curso sobre transporte de luz de la [universidad de Austria](https://www.cg.tuwien.ac.at/courses/Rendering/VU.SS2019.html) ha sido una gran fuente de información para el trabajo.

> TODO: tengo que ver exactamente cómo cito esa fuente anterior.

<hr>

## Referencias {.unlisted .unnumbered}

[@wikipedia-contributors-2022A], [@wikipedia-contributors-2022B], [@wikipedia-contributors-2022C], [@wikipedia-contributors-2022D], [@caulfield-2020], [@wikipedia-contributors-2022E], [@unknown-author-no-date], [@Haines2019]

- https://www.cg.tuwien.ac.at/courses/Rendering/VU.SS2019.html