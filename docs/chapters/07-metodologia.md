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

Se ha utilizado una combinación de las siguietes tipografías:

- [Crimson Pro](https://fonts.google.com/specimen/Crimson+Pro): una tipografía serif clara, legible y contemporánea. Funciona muy bien en densidades más bajas, como 11pt. Es ideal para la versión en PDF. Además, liga estupendamente con Source Sans Pro, utilizada para los títulos en la plantilla Eisvogel.
- [Fraunces](https://fonts.google.com/specimen/Fraunces): de lejos, la fuente más interesante de todo este proyecto. Es una soft-serif *old style*, pensada para títulos y similares (lo que se conoce como *display*). Es usada en los títulos de la web. Una de sus propiedades más curiosas es que modifica activamente los glifos dependiendo del valor del *optical size axis*, el peso y similares. Recomiendo echarle un ojo a su [repositorio de Github](https://github.com/undercasetype/Fraunces).
- [Rubik](https://fonts.google.com/specimen/Rubik): La elección de Rubik es peculiar. Por sí sola, no casa con el proyecto. Sin embargo, combinada con Fraunces, proporcionan un punto de elegancia y familiaridad a la web. Su principal fuerte es la facilidad para la comprensión lectora en pantallas, algo que buscamos para la página web.
- [Julia Mono](https://juliamono.netlify.app/): monoespaciada, pensada para computación científica. Llevo usándola bastante tiempo, y combia bien con Crimson Pro.
- [Jetbrains Mono](https://www.jetbrains.com/es-es/lp/mono/): otra tipografía monoespaciada open source muy sólida, producida por la compañía Jetbrains. Se utiliza en la web para los bloques de código.

Todas estas fuentes permiten un uso no comercial gratuito.

## Herramientas

> TODO: hablar de [Figma](https://www.figma.com/), LTeX... (Issue [#40](https://github.com/Asmilex/Raytracing/issues/40)), bamboo.css

## Github

> TODO - Hablar de cómo se utiliza Github y sus tecnologías para agrupar todo el trabajo. Hablar de la guía de estilos, y cómo los emojis ayudan a identificar rápidamente secciones.

[Github](https://github.com)

### Github Actions

> TODO - Hablar de cómo se usa el sistema de integración continua para construir la web y el pdf

### Github Projects

> TODO - Hablar de cómo se gestiona el trabajo mediante issues, recapitulados todos con Projects.

### Estilo de commits


<hr>

## Referencias {.unlisted .unnumbered}

\end