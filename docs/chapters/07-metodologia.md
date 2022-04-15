\appendix

# Metodología; o cómo se hizo este trabajo

Cualquier proyecto de una envergadura considerable necesita ser planificado con antelación. En este capítulo vamos a hablar de cómo se ha realizado este trabajo: mostraremos las herramientas usadas, los ciclos de desarrollo, integración entre documentación y path tracer, y otras influencias que han afectado al producto final.

## Influencias

Antes de comenzar con la labor, primero uno se debe hacer una simple pregunta:

> *"Y esto, ¿por qué me importa?"*

Dar una respuesta contundente a este tipo de cuestiones nunca es fácil. Sin embargo, sí que puedo proporcionar motivos por los que he querido escribir sobre ray tracing.

Una de las principales influencias ha sido [Digital Foundry](https://www.youtube.com/user/DigitalFoundry). Este grupo de divulgación se dedica al estudio de las técnicas utilizadas en el mundo de los videojuegos. El inicio de la era del ray tracing en tiempo real les llevó a dedicar una serie de vídeos y artículos a esta tecnología, y a las diferentes maneras en las que se ha implementado. Se puede ver un ejemplo en [@digital-foundry-2020].

Dado que esta área combina tanto informática, matemáticas y una visión artística, ¿por qué no explorarlo a fondo?

Ahora que se ha decidido el tema, es hora de ver cómo atacarlo.

Soy un fiel creyente del aprendizaje mediante el juego. Páginas como [Explorable Explanations](https://explorabl.es/), el [blog de Bartosz Ciechanowski](https://ciechanow.ski/lights-and-shadows/), el proyecto [*The napkin*](https://web.evanchen.cc/napkin.html) o el divulgador [3Blue1Brown](https://www.3blue1brown.com/) repercuten inevitablemente en la manera en la que te planteas cómo comunicar textos científicos. Por ello, aunque esto a fin de cuentas es un trabajo de fin de grado de una carrera, quería ver hasta dónde era capaz de llevarlo.

Otro punto importante es la *manera* de escribir. No me gusta especialmente la escritura formal. Prefiero ser distendido. Por suerte, parece que el mundo científico se está volviendo más informal [@nature-2016], así que no soy el único que aprueba esta tendencia. Además, la estructura clásica de un escrito matemático de "teorema, lema, demostración, corolario" no me agrada especialmente. He intentado preservar su estructura, pero sin ser tan explícito. Estos dos puntos, en conjunto, suponen un balance entre formalidad y distensión difícil de mantener.

## Ciclos de desarrollo

Este proyecto está compuesto por 2 grandes pilares: documentación --lo que estás leyendo, ya sea en PDF o en la web-- y software.

La metodología que se ha seguido es, en esencia, una versión de Agile muy laxa [@beck2001agile].

Para empezar, se implementaron los tres libros de Shirley de la "serie In One Weekend": In One Weekend [@Shirley2020RTW1], The Next Week [@Shirley2020RTW2], y The Rest of your Life [@Shirley2020RTW3].

Tras esto, comenzó a [desarrollarse](#setup-del-proyecto) el motor por GPU. Cuando se consiguió una base sólida (que se puede ver en [este issue del repositorio](https://github.com/Asmilex/Raytracing/issues/25)), se empezó a alternar entre escritura de documentación y desarrollo del software. A fin de cuentas, no tiene sentido implementar algo que no se conoce.

Para apoyar el desarrollo, se ha utilizado [Github](#github). Más adelante hablaremos de cómo esta plataforma ha facilitado el trabajo.

## Diseño

> TODO: hablar de paleta de colores, tipografía...

El diseño juega un papel fundamental en este proyecto. Todos los elementos visuales han sido escogidos con cuidado, de forma que se preserve la estética.

Se ha creado **un diseño que preserve el equilibrio entre la profesionalidad y la distensión**.

### Bases del diseño

Para la documentación en versión PDF, usamos como base la *template* [Eisvogel](https://github.com/Wandmalfarbe/pandoc-latex-template). Esta es una elegante plantilla fácil de usar para LaTeX. Uno de sus puntos fuertes es la personalización, la cual aprovecharemos para darle un toque diferente.

La web utiliza como base el estilo generado por Pandoc, el microframework de css [Bamboo](https://github.com/rilwis/bamboo) y unas modificaciones personales.

### Tipografías

Un apartado al que se le debe prestar especial énfasis es a la combinación de tipografías. A fin de cuentas, esto es un libro; así que escoger un tipo de letra correcto facilitará al lector comprender los conceptos. Puede parecer trivial a priori, pero es importante.

Para este trabajo, se han escogido las siguientes tipografías:

- [Crimson Pro](https://fonts.google.com/specimen/Crimson+Pro): una tipografía serif clara, legible y contemporánea. Funciona muy bien en densidades más bajas, como 11pt. Es ideal para la versión en PDF. Además, liga estupendamente con Source Sans Pro, utilizada para los títulos en la plantilla Eisvogel.
- [Fraunces](https://fonts.google.com/specimen/Fraunces): de lejos, la fuente más interesante de todo este proyecto. Es una soft-serif *old style*, pensada para títulos y similares (lo que se conoce como *display*). Es usada en los títulos de la web. Una de sus propiedades más curiosas es que modifica activamente los glifos dependiendo del valor del *optical size axis*, el peso y similares. Recomiendo echarle un ojo a su [repositorio de Github](https://github.com/undercasetype/Fraunces).
- [Rubik](https://fonts.google.com/specimen/Rubik): La elección de Rubik es peculiar. Por sí sola, no casa con el proyecto. Sin embargo, combinada con Fraunces, proporcionan un punto de elegancia y familiaridad a la web. Su principal fuerte es la facilidad para la comprensión lectora en pantallas, algo que buscamos para la página web.
- [Julia Mono](https://juliamono.netlify.app/): monoespaciada, pensada para computación científica. Llevo usándola bastante tiempo, y combia bien con Crimson Pro.
- [Jetbrains Mono](https://www.jetbrains.com/es-es/lp/mono/): otra tipografía monoespaciada open source muy sólida, producida por la compañía Jetbrains. Se utiliza en la web para los bloques de código.

Todas estas fuentes permiten un uso no comercial gratuito.

### Paleta de colores

A fin de mantener consistencia, se ha creado una paleta de colores específica.

![La paleta de colores del proyecto](./img/Paleta%20de%20colores.png)

El principal objetivo es **transmitir tranquilidad**, pero a la misma vez, **profesionalidad**. De nuevo, buscamos la idea de profesionalidad distendida que ya hemos repetido un par de veces.

Partiendo del rojo que traía Eisvogel (lo que para nosotros sería el rojo primario), se han creado el resto. En principio, con 5 tonalidades diferentes nos basta. Todas ellas vienen acompañadas de sus respectivas variaciones oscuras, muy oscuras, claras y muy claras. Corresponderían a los `color-100, color-300, color-500, color-700, color-900` que estamos acostumbrados en diseño web. Para la escala de grises, se han escogido 7 colores en vez de 9. Son más que suficientes para lo que necesitamos. Puedes encontrar las definiciones en el [fichero de estilos](https://github.com/Asmilex/Raytracing/blob/main/docs/headers/style.css).

Todos los colores que puedes ver en este documento se han extraído de la paleta. ¡La consistencia es clave!

## Flujo de trabajo y herramientas

Encontrar una herramienta que se adapte a un *workflow* es complicado. Aunque hay muchos programas maravilosos, debemos hacerlos funcionar en conjunto. En este apartado, vamos a describir cuáles son las que hemos usado.

Principalmente destacan tres de ellas: **Github**, **Pandoc** y **Figma**. La primera tendrá [su propia sección](#github), así que hablaremos de las otras.

> TODO: foto del workflow.

### Pandoc

[Pandoc](https://pandoc.org/) es una estupendísima de conversión de documentos. Se puede usar para convertir un tipo de archivo a otro. En este caso, se usa para convertir una serie de ficheros Markdown (los capítulos) a un fichero HTML (la web) y a PDF. Su punto más fuerte es que permite escribir LaTeX de forma simplificada, como si se tratara de *sugar syntax*. Combina la simplicidad de Markdown y la correctitud de LaTeX.

Su funcionamiento en este proyecto es el siguiente: Primero, recoge los capítulos que se encuentra en `docs/chapters`, usando una serie de cabeceras en YAML que especifican ciertos parámetros (como autor, fecha, título, etc.), así como scripts de Lua. Estas caceberas se encuentran en `docs/headers`. En particular:

1. `meta.md` recoge los parámetros base del trabajo.
2. `pdf.md` y `web.md` contienen algunas definiciones específicas de sus respectivos formatos. Por ejemplo, el YAML del PDF asigna las variables disponibles de la plantilla Eisvogel; mientras que para la web se incluyen las referencias a algunas bibliotecas de Javascript necesarias o los estilos (`docs/headers/style.css`, usando como base [Bamboo.css](https://github.com/rilwis/bamboo)).
3. `math.md` contiene las definiciones de LaTeX.
4. Se utilizan algunos filtros específicos de Lua para simplificar la escritura. En específico, `standard-code.lua`  formatea correctamente los bloques de código para la web.

Un fichero Makefile (`docs/Makefile`) contiene varias órdenes para generar ambos formatos. Tienen varios parámetros adicionales de por sí, como puede ser la bibliografía (`docs/chapters/bibliography.bib`).

### Figma

[Figma](https://www.figma.com/) es otro de esos programas que te hace preguntarte por qué es gratis. Es una aplicación en la web usada para diseño gráfico. Es muy potente, intuitiva, y genera unos resultados muy buenos en poco tiempo. Todos los diseños de este trabajo se han hecho con esta herramienta.

![Tablón principal del proyecto, a día 15 de abril de 2022](./img/Figma.png)

Una de las características más útiles es poder exportar rápidamente la imagen. Esto permite hacer cambios rápidos y registrarlos en el repositorio fácilmente. Además, permite instalar plugins. Uno de ellos ha resultado especialmente útil: [Latex Complete](https://www.figma.com/community/plugin/793023817364007801/LaTeX-Complete). Esto nos permite incrustar código LaTeX en el documento en forma de SVG.

### Otros programas

Como es normal, hay muchos otros programas que han intervenido en el desarrollo. Estos son algunos de ellos:

- El editor por excelencia [VSCode](https://code.visualstudio.com/). Ha facilitado en gran medida el desarrollo de la aplicación y la documentación. En particular, se ha usado una extensión denominada [Trigger task on save](https://marketplace.visualstudio.com/items?itemName=Gruntfuggly.triggertaskonsave) que compila la documentación HTML automáticamente al guardar un fichero. ¡Muy útil y rápido!
- [Vectary](https://www.vectary.com/) para hacer los diseños en 3D fácilmente. Permite exportar una escena rápidamente a png para editarla en Figma.
- Como veremos más adelante, la documentación se compila en el repositorio usando un contenedor de [Docker](https://www.docker.com/).

## Github

> TODO - Hablar de cómo se utiliza Github y sus tecnologías para agrupar todo el trabajo. Hablar de la guía de estilos, y cómo los emojis ayudan a identificar rápidamente secciones.

[Github](https://github.com)

### Integración continua con Github Actions

> TODO - Hablar de cómo se usa el sistema de integración continua para construir la web y el pdf

El contenedor se encuentra en el [repositorio de Dockerhub][https://hub.docker.com/r/asmilex/raytracing]. Esta imagen está basada en [dockershelf/latex:full](https://hub.docker.com/r/dockershelf/latex). Por desgracia, es *muy* pesada para ser un contenedor. Desafortunadamente, una instalación de LaTeX ocupa una cantidad de espacio considerable; y para compilar el PDF necesitamos una muy completa. El Dockerfile, además, carga los ficheros fuentes necesarios en el sistema. Puedes encontrar el Dockerfile [aquí](https://github.com/Asmilex/Raytracing/blob/main/Dockerfile).

### Github Projects

> TODO - Hablar de cómo se gestiona el trabajo mediante issues, recapitulados todos con Projects.

### Estilo de commits


<hr>


## Referencias {.unlisted .unnumbered}

\end