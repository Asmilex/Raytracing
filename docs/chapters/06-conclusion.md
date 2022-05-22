# Conclusiones

Al inicio de este trabajo nos propusimos crear un motor de path tracing en tiempo real. Esta no era una tarea fácil, así que tuvimos que empezar desde las bases de la informática gráfica.

Primero estudiamos qué es exactamente el **algoritmo path tracing**, el cual es una forma de crear imágenes virtuales de entornos tridimensionales. Dado que es un método físicamente realista, necesitábamos comprender **cómo se comporta la luz**, con el fin de conocer de qué color pintar un píxel de nuestra imagen. Esencialmente, entendimos que los fotones emitidos por las fuentes de iluminación rebotan por los diferentes objetos de un entorno, adquiriendo partes de sus propiedades en el impacto.

No obstante, imitar a la realidad es una tarea titánica. Las ecuaciones radiométricas resultan computacionalmente complejas, por lo que no es posible crear una simulación perfecta de la luz. Por ese motivo recurrimos a las **técnicas de Monte Carlo**, las cuales utilizan sucesos aleatorios para estimar la cantidad de luz de un punto. Estos métodos hicieron viables ciertos cálculos necesarios para el algoritmo. Sin embargo, su naturaleza inexacta implica que existe un error de estimación. Esto nos hizo explorar algunas formas de reducir el ruido generado por las técnicas de integración de Monte Carlo.

Una vez adquirimos el fundamento teórico, **diseñamos un software** que nos permitiera poner en práctica nuestros conocimientos. Escogimos la API gráfica Vulkan y un framework de Nvidia como base para el desarrollo. Debido a la complejidad del algoritmo tuvimos que construir numerosas abstracciones que aceleraran el proceso de renderizado, basándonos en tarjetas gráficas modernas. Aprender a programar en tarjetas gráficas no es sencillo, así que tuvimos que diseñar unos programas específicos de éstas llamados *shaders*; específicamente, los shaders de ray tracing.

Al final, obtuvimos el resultado deseado: un motor de path tracing capaz de producir imágenes de escenas virtuales.

![El motor es capaz de producir preciosas imágenes de objetos físicamente realistas que se mueven en tiempo real](./img/06/Showcase.png){#fig:pathtracing_showcase}

Como en cualquier otro trabajo, durante el proceso de desarrollo tuvimos que algunas tomar decisiones técnicas que alteran la calidad del producto. Por ello, realizamos un **análisis del rendimiento** basándonos en el *frame time* --tiempo que tarda en un frame en renderizarse--, variando los parámetros de path tracing en el proceso. De esta forma, pudimos comprobar cuánto nos cuesta sacar imágenes de gran nitidez.

Finalmente, para poner en contexto el motor, comparamos nuestra implementación con el de otros autores. En este caso, con el path tracer de Peter Shirley creado en Ray Tracing In One Weekend [@Shirley2020RTW1]. Nos dimos cuenta de que, aunque nuestra versión genera las muestras de forma más naïve, la rapidez con la que rinde consigue compensar el ruido de la imagen, produciendo así resultados más nítidos.

## Posibles mejoras

- Precómputo de fuentes de luz / materiales emisivos de la escena
- Reestructuración de las interfaces
  - Materiales y escenas
    - Mejorar interfaz
    - Más tipos
  - Fuentes de luz
  - class Engine
- Más materiales
- Motion blur
- Uso de ruido (blue noise, perlin noise)
- Multiple importance sampling