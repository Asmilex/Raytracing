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

Crear un software de este calibre es una tarea de una complejidad enorme. Los motores de renderización requieren un equipo de desarrollo de un tamaño considerable, una gran inversión y un esfuerzo constante. Teniendo en cuenta el contexto del proyecto, en el camino ha sido necesario tomar decisiones imperfectas. En esta sección exploraremos algunas posibles mejoras para este trabajo.

### Interfaces

La parte que más margen de mejora presenta es **la interfaz de las fuentes de luz**. En su estado actual, únicamente es posible utilizar dos tipos de fuentes externas a la escena: luces puntuales y direccionales. Aunque se muestrean de forma directa. Además, solo puede existir una única fuente de este tipo.

Este diseño propicia un error relacionado con la nitidez de la imagen. Una de las ideas clave de path tracing es generar muestras de *forma inteligente*: dado que calcular caminos es caro, tira rayos hacia zonas que aporten mucha información. En dichas zonas deben encontrarse, esencialmente, fuentes de luz. ¡Pero la interfaz **no conoce dónde se encuentran los materiales emisivos de la escena**! Eso implica que algunos rayos toman direcciones que no aportan nada. Esto lo pudimos comprobar empíricamente en la [comparativa](#comparativa-con-in-one-weekend).

Otro tipo de interfaz que necesita una mejora sustancial es la de materiales y objetos. En el estado actual del programa, los elementos de la escena son cargados desde un fichero `.obj`. Las propiedades del material son determinadas en el momento del impacto de un rayo basándonos en los parámetros del archivo de materiales asociado `.mtl`. Esta estrategia funciona suficientemente bien en este caso, pues el ámbito de desarrollo es bastante reducido.

Sin embargo, a la larga sería beneficioso **reestructurar la carga y almacenamiento de los objetos**. De esta forma, atacamos el problema anterior relacionado con los materiales emisivos, conseguiríamos mayor granularidad en los tipos de objetos y podríamos diseñar estrategias específicas para algunos tipos de materiales (como separar en diferentes capas de impacto los objetos transparentes). Esto también nos permitiría añadir nuevos tipos de materiales más fácilmente, como pueden ser objetos dieléctricos, plásticos, mezclas entre varios tipos, *subsurface scattering*...

Estas nuevas interfaces deberían ir acompañadas de una **refactorización de la clase `Engine`**. El framework que escogimos, Nvidia nvpro-samples [@nvpro-samples], está destinado a ser didáctico y no eficiente. Por tanto, el software presenta un alto acoplamiento. Separar en varias clases más reducidas, como una clase para rasterización y otra para ray tracing, sería esencial. Sin embargo, Vulkan es una API muy compleja, por lo que requeriría de una gran cantidad de trabajo.

### Nuevas técnicas de reducción de ruido

En este trabajo hemos implementado algunas técnicas de reducción de varianza del estimador de Monte Carlo para la ecuación del transporte de luz [@eq:rendering_equation], lo cual permite reducir el ruido de la imagen final. Entre estas, se encuentran muestreo por importancia, *next-event estimation* o acumulación temporal de las muestras.

Sin embargo, existen numerosas técnicas que no se han desarrollado. Entre estas, se encuentran el uso de ruido como *blue noise*, muestreo por importancia múltiple, ruleta rusa o secuencias de baja discrepancia (métodos de quasi-Monte Carlo).  Resultaría interesante ver cómo estas técnicas se comportan en comparación con las que sí hemos usado.