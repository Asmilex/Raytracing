# Metodología de trabajo

Cualquier proyecto de una envergadura considerable necesita ser planificado con antelación. En este capítulo vamos a hablar de cómo se ha gestionado este trabajo: mostraremos las herramientas usadas, influencias, los ciclos de desarrollo, el uso de integración continua, y compenetración entre documentación y path tracer

## Influencias

Antes de comenzar con la labor, primero uno se debe hacer una simple pregunta:

> *"Y esto, ¿por qué me importa?"*

Dar una respuesta contundente a este tipo de cuestiones nunca es fácil. Sin embargo, sí que puedo proporcionar motivos por los que he querido escribir sobre ray tracing.

Una de las principales inspiraciones del proyecto ha sido [@digital-foundry]. Este grupo de divulgación se dedica al estudio de las técnicas utilizadas en el mundo de los videojuegos. El inicio de la era del ray tracing en tiempo real les llevó a dedicar una serie de vídeos y artículos a esta tecnología, y a las diferentes maneras en las que se ha implementado. Se puede ver un ejemplo en [@digital-foundry-2020].

<iframe width="784" height="441" src="https://www.youtube.com/embed/6bqA8F6B6NQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

Dado que esta área combina tanto informática, matemáticas y una visión artística, ¿por qué no explorarlo a fondo?

Ahora que se ha decidido el tema, es hora de ver cómo atacarlo.

Soy un fiel creyente del aprendizaje mediante la exploración. Conceptos que resulten tangibles, que se pueda jugar con ellos. Páginas como *Explorable Explanations* [@explorable-explanations], el blog de [@ciechanowski], el proyecto *The napkin* [@napkin] o el divulgador 3Blue1Brown [@3blue1brown] repercuten inevitablemente en la manera en la que te planteas cómo comunicar textos científicos. Por ello, aunque esto no deja de ser un mero trabajo de fin de grado de una carrera, quería ver hasta dónde era capaz de llevarlo.

Otro punto importante es la *manera* de escribir. No me gusta especialmente la escritura formal. Prefiero ser distendido. Por suerte, parece que el mundo científico se está volviendo más familiar [@nature-2016], una tendencia a la que yo me he sumado. Además, la estructura clásica de un escrito matemático de "teorema, lema, demostración, corolario" no me agrada especialmente. He intentado preservar su estructura, pero sin ser tan explícito. Durante esta memoria he intentado introducir cada concepto de la manera más natural posible, y que las demostraciones se integren con el desarrollo.

Estos dos puntos, en conjunto, suponen un balance entre formalidad y distensión difícil de mantener.

## Ciclos de desarrollo

Este proyecto está compuesto por 2 grandes pilares: documentación --lo que estás leyendo, ya sea en PDF o en la web-- y software. Podemos distinguir varias fases en la planificación del proyecto, que quedan resumidas en el diagrama de Gantt [Figuras @fig:gantt_1, @fig:gantt_2]

Para comenzar, durante el verano de 2021 se implementarían los tres libros de Shirley de la "serie In One Weekend": In One Weekend [@Shirley2020RTW1], The Next Week [@Shirley2020RTW2], y The Rest of your Life [@Shirley2020RTW3]. De esta forma, asentaríamos las bases del proyecto, acelerando así el aprendizaje.

![Diagrama de Gantt de la primera parte del desarrollo. Los libros de Peter Shirley servirían como introducción al trabajo](./img/08/Diagrama%20de%20Gantt%201.png){#fig:gantt_1}

Tras esto, comenzaría a desarrollarse el motor por GPU. Cuando se consiguiera una base sólida, se empezaría a alternar entre escritura de la memoria y el software. Es importante documentar lo que se realiza, pues no se puede implementar algo que no se entiende.

![Diagrama de Gantt de la segunda parte del desarrollo. Durante el segundo cuatrimestre se trabajaría tanto la memoria como el software](./img/08/Diagrama%20de%20Gantt%202.png){#fig:gantt_2}

Sin embargo, esto era únicamente una planificación. Como todos sabemos, en la práctica los planes no suelen salir a la perfección. ¿Ha sido este un caso de una preparación desastrosa?

Por fortuna, la idea inicial **se ha asemejado mucho a la realidad**. Algunas fases han sido más rápidas que otras, mientras que otras partes han costado más trabajo. Los commits del repositorio ayudan a clasificar el tipo de trabajo, pero, en resumidas cuentas:

- In One Weekend terminó de desarrollarse considerablemente antes, el 21 de agosto. A excepción de la última parte del desarrollo de la memoria, no requirió de más tiempo.
- El diseño de la memoria y la integración continua tuvieron dos fases: un sprint inicial donde se deja asentado el 70% del trabajo, y pequeñas mejoras incrementales en los siguientes meses. Debemos destacar que, conforme se mejoraba el diseño, se añadían nuevas herramientras necesarias para su construcción. Es por ello que la integración continua necesitó varios arreglos, tanto al dockerfile como a los Actions.
- La implementación inicial del motor necesitó un tiempo considerablemente menor del previsto. No obstante, el tiempo de perfeccionamiento aumentó, y el desarrollo final concluyó cerca del 20 de mayo de 2022. Prácticamente todas las características básicas fueron implementadas, a excepción de algunos detalles. No dio mucho tiempo a extenderlo más allá de esto.

Con respecto a la metodología de trabajo que se ha seguido es podemos decir que es, esencialmente, **una versión de Agile muy laxa** [@beck2001agile]. Apoyándonos en las herramientas ofrecidas por [Github](#github), diseñamos un sistema de requisitos mediante issues, tanto para la memoria como para el software. Más adelante veremos más a fondo cómo esta herramienta ha facilitado enormemente el desarrollo.

## Presupuesto

A la hora de desarrollar un proyecto de software, es importante realizar una estimación del coste del trabajo. En otro caso, se corre el riesgo de que no se pueda llegar a cumplir el objetivo. En este caso, **el proyecto ha tenido un coste de 10314 €**, que puede desglosarse de la siguiente manera:

- El **coste del software** en total ha sido de **0 €**. Las herramientas utilizadas para el desarrollo son todas de código abierto, por lo que su uso es gratuito. Aunque se comentarán individualmente en una sección posterior, las más importantes han sido:
  - Pandoc.
  - LaTeX.
  - Vulkan.
  - NVIDIA DesignWorks Samples framework.
  - Visual Studio Code.
  - Git y Github (repositorios, Actions, Issues, Projects).
  - Figma.
  - Docker.
- Con respecto al **hardware**, el precio total asciende a **2214 €**. Se han utilizado dos máquinas principalmente: una *custom build* de última generación para soportar el software y un portátil para trabajar en remoto:
  - **PC** *custom build* (1414 €):
    - **CPU**: Intel core i5 12600K (310 €).
    - **Disipador de CPU**: Arctic Freezer 34 eSports DUO (50 €).
    - **Placa base**: B660M DS3H AX DDR4 (130 €).
    - **GPU**: KFA2 GeForce RTX 2070 Super (500 €).
    - **RAM**: Crucial Ballistix 2x8GB DDR4 3200 MHz (82 €).
    - **Caja**: NZXT S340 (70 €).
    - **NVME SSD**: Kioxia Exceria Plus G2 (72 €).
    - **SATA3 SSD**: Kingston A400 SSD 480 GB (50 €).
    - **HDD**: WDC 500 GB (40 €).
    - **Fuente de alimentación**: Corsair RM650x 80 PLUS Gold (110 €).
  - **Portátil**: Xiaomi Mi Notebook Pro (8250U) (800 €).
- Atendiendo al apartado de recursos humanos, se estima un coste total de **8100 €**. Se calcula a partir de que, como alumno, recibo un sueldo de 18 €/h. Teniendo en cuenta que el número de créditos del Trabajo de Fin de Grado son 18, y que un crédito son 25 horas de estudio individual, se ha trabajado un total de 450 horas en el proyecto.

## Diseño

El diseño gráfico juega un papel fundamental en este proyecto. Todos los elementos visuales han sido escogidos con cuidado, de forma que se logre un **equilibrio entre la profesionalidad y la distensión**. Esta identidad visual se extiende a todos los aspectos del trabajo.

### Bases del diseño

Para la documentación en versión PDF, usamos como base la plantilla Eisvogel, de [@eisvogel]. Uno de sus puntos fuertes es la personalización, la cual aprovecharemos para darle un toque diferente.

La web utiliza como base el estilo generado por Pandoc, el microframework de CSS Bamboo de [@bamboo] y unas modificaciones personales.

### Tipografías

Un apartado al que se le debe prestar especial énfasis es a la combinación de tipografías. A fin de cuentas, nuestro objetivo es comunicar al lector nuevos conceptos de la manera más amigable posible. Escoger un tipo de letra correcto, aunque pueda parecer irrelevante a priori, facilitará la compresión de estas nociones.

Para este trabajo, se han escogido las siguientes tipografías:

- **Crimson Pro**, por [@crimson-pro]: una tipografía serif clara, legible y contemporánea. Funciona muy bien en densidades más bajas, como 11pt. Es ideal para la versión en PDF. Además, liga estupendamente con Source Sans Pro, utilizada para los títulos en la plantilla Eisvogel.
- **Fraunces**, por [@fraunces]: de lejos, la fuente más interesante de todo este proyecto. Es una soft-serif *old style*, pensada para títulos y similares (lo que se conoce como *display*). Es usada en los títulos de la web. Una de sus propiedades más curiosas es que modifica activamente los glifos dependiendo del valor del *optical size axis*, el peso y similares. Recomiendo echarle un ojo a su repositorio de Github, pues incluyen detalles sobre la implementación.
- **Rubik**, por [@rubik]: La elección de Rubik es peculiar. Por sí sola, no casa con el proyecto. Sin embargo, combinada con Fraunces, proporcionan un punto de elegancia y familiaridad a la web. Su principal fuerte es la facilidad para la comprensión lectora en pantallas, algo que buscamos para la página web.
- **Julia Mono**, por [@julia-mono]: monoespaciada, pensada para computación científica. Combia bien con Crimson Pro.
- **Jetbrains Mono**, por [@jetbrains-mono]: otra tipografía monoespaciada open source muy sólida, producida por la compañía Jetbrains. Se utiliza en la web para los bloques de código.

Todas estas fuentes permiten un uso no comercial gratuito.

![Showcase de las tipografías utilizadas](./img/08/Tipografías.png){ #fig:tipografías width=70% }

### Paleta de colores

A fin de mantener consistencia, se ha creado una paleta de colores específica.

![La paleta de colores del proyecto](./img/08/Paleta%20de%20colores.png){ #fig:paleta_de_colores width=400px }

El principal objetivo es **transmitir tranquilidad**, pero a la misma vez, **profesionalidad**. De nuevo, buscamos la idea de profesionalidad distendida que ya hemos repetido un par de veces.

Partiendo del rojo que traía Eisvogel (lo que para nosotros sería el rojo primario), se han creado el resto. En principio, con 5 tonalidades diferentes nos basta. Todas ellas vienen acompañadas de sus respectivas variaciones oscuras, muy oscuras, claras y muy claras. Corresponderían a los `color-100, color-300, color-500, color-700, color-900` que estamos acostumbrados en diseño web. Para la escala de grises, se han escogido 7 colores en vez de 9. Son más que suficientes para lo que necesitamos. Puedes encontrar las definiciones en el fichero de estilos, ubicado en `./docs/headers/style.css`.

Todos los colores que puedes ver en este documento se han extraído de la paleta. ¡La consistencia es clave!

## Flujo de trabajo y herramientas

Encontrar una herramienta que se adapte a un *workflow* es complicado. Aunque hay muchos programas maravilosos, debemos hacerlos funcionar en conjunto. En este apartado, vamos a describir cuáles son las que hemos usado.

Principalmente destacan tres de ellas: **Github**, **Pandoc** y **Figma**. La primera tendrá [su propia sección](#github), así que hablaremos de las otras.

### Pandoc

**Pandoc** [@pandoc] es una estupendísima de conversión de documentos. Se puede usar para convertir un tipo de archivo a otro. En este caso, se usa para convertir una serie de ficheros Markdown (los capítulos) a un fichero HTML (la web) y a PDF. Su punto más fuerte es que permite escribir LaTeX de forma simplificada, como si se tratara de *sugar syntax*. Combina la simplicidad de Markdown y la correctitud de LaTeX.

Su funcionamiento en este proyecto es el siguiente: Primero, recoge los capítulos que se encuentra en `docs/chapters`, usando una serie de cabeceras en YAML que especifican ciertos parámetros (como autor, fecha, título, etc.), así como scripts de Lua. Estas caceberas se encuentran en `docs/headers`. En particular:

1. `meta.md` recoge los parámetros base del trabajo.
2. `pdf.md` y `web.md` contienen algunas definiciones específicas de sus respectivos formatos. Por ejemplo, el YAML del PDF asigna las variables disponibles de la plantilla Eisvogel; mientras que para la web se incluyen las referencias a algunas bibliotecas de Javascript necesarias o los estilos (`docs/headers/style.css`, usando como base Bamboo.
3. `math.md` contiene las definiciones de LaTeX.
4. Se utilizan algunos filtros específicos de Lua para simplificar la escritura. En específico, `standard-code.lua` formatea correctamente los bloques de código para la web.

Un fichero Makefile (`docs/Makefile`) contiene varias órdenes para generar ambos formatos. Tienen varios parámetros adicionales de por sí, como puede ser la bibliografía (`docs/chapters/bibliography.bib`).

### Figma

**Figma** [@figma] es otro de esos programas que te hace preguntarte por qué es gratis. Es una aplicación web usada para diseño gráfico. Es muy potente, intuitiva, y genera unos resultados muy buenos en poco tiempo. Todos los diseños de este trabajo se han hecho con esta herramienta.

![Tablón principal del proyecto de Figma, a día 15 de abril de 2022](./img/08/Figma.png){#fig:figma}

Una de las características más útiles es poder exportar rápidamente la imagen. Esto permite hacer cambios rápidos y registrarlos en el repositorio fácilmente. Además, permite instalar plugins. Uno de ellos ha resultado especialmente útil: Latex Complete [@latex-complete]. Esto nos permite incrustar código LaTeX en el documento en forma de SVG.

### Otros programas

Como es normal, hay muchos otros programas que han intervenido en el desarrollo. Estos son algunos de ellos:

- El editor por excelencia **Visual Studio Code** [@vscode]. Ha facilitado en gran medida el desarrollo de la aplicación y la documentación. En particular, se ha usado una extensión denominada *Trigger Task on Save* [@trigger-task] que compila la documentación HTML automáticamente al guardar un fichero. ¡Muy útil y rápido!
- **Vectary** [@vectary] para hacer los diseños en 3D fácilmente. Permite exportar una escena rápidamente a png para editarla en Figma.
- Como veremos más adelante, la documentación se compila en el repositorio usando un contenedor de **Docker** [@docker]
- Cualquier proyecto informático debería usar **git**. Este no es una excepción.

## Github

La página **Github** [@github] ha alojado prácticamente todo el contenido del trabajo; desde el programa, hasta la documentación online. El repositorio se puede consultar en `github.com/Asmilex/Raytracing` [@asmilex-raytracing-repo].

Se ha escogido Github en vez de sus competidores (como Gitlab o Bitbucket) por los siguientes motivos:

1. Llevo usándola toda la carrera.
2. Los repositorios de Nvidia se encontraban en Github, por lo que resulta más fácil sincronizarlos.
3. La documentación se puede desplegar usando Github Pages.
4. Los Github Actions son particularmente cómodos y sencillos de usar.

Entremos en detalle en algunos de los puntos anteriores:

### Integración continua con Github Actions y Github Pages

Cuando hablamos de **integración continua**, nos referimos a ciertos programas que corren en un repositorio y se encargan de hacer ciertas transformaciones al código, de forma que este se prepare para su presentación final. En esencia, automatizan algunas tareas habituales de un desarrollo de software. [@jj]

En este trabajo lo usaremos para compilar la documentación. De esta forma, no necesitamos lidiar con "proyecto final", "proyecto final definitivo", "proyecto final final v2", etc. Simplemente, cuando registremos un cambio en los ficheros Markdown (lo que se conoce en git como un `commit`), y lo subamos a Github (acción de `push`), se ejecutará un denominado `Action` que operará sobre nuestros archivos.

Tendremos dos tipos de `Actions`: uno que se encarga de compilar la web, y otro el PDF. En esencia, operan de la siguiente manera:

1. Comprueba si se ha modificado algún fichero `.md` en el último commit subido. Si no es el caso, para.
2. Si sí se ha modificado, accede a la carpeta del repositorio y compila la documentación mediante `pandoc`.
   1. La web se genera en `docs/index.html`. Publica la web a Github Pages.
   2. El PDF se crea en `docs/TFG.pdf`
3. Commitea los archivos y termina.

![La pestaña de Github Actions permite controlar con facilidad el resultado de un *workflow* y cuánto tarda en ejecutarse](./img/08/Github%20Actions.png){#fig:github_actions}

El *workflow* de la web corre automáticamente, mientras que para generar el PDF hace falta activación manual. Aunque no es *del todo* correcto almacenar ficheros binarios en un repositorio de git, no me resulta molesto personalmente. Así que, cuando considero que es el momento oportuno, lo hago manualmente. Además, también se activa por cada *release* que se crea.

![Diagrama con los *workflows*](./img/08/Actions.png){#fig:actions-workflow}

Volviendo a la web, Github permite alojar páginas web para un repositorio. Activando el parámetro correcto en las opciones del repositorio, y configurándolo debidamente, conseguimos que lea el archivo `index.html` generado por el Action y lo despliegue. Esto es potentísimo: con solo editar una línea de código y subir los cambios, conseguimos que la web se actualice al instante.

Para generar los archivos nos hace falta una distribución de LaTeX, Pandoc, y todas las dependencias (como filtros). Como no encontré ningún contenedor que sirviera mi propósito, decidí crear uno. Se encuentra en el repositorio de Dockerhub [@asmilex-raytracing-docker]. Esta imagen está basada en `dockershelf/latex:full` [@dockershelf]. Por desgracia, es *muy* pesada para ser un contenedor. Desafortunadamente, una instalación de LaTeX ocupa una cantidad de espacio considerable; y para compilar el PDF necesitamos una muy completa, por lo que debemos lidiar con este *overhead*. Puedes encontrar el Dockerfile en `./Dockerfile`.

### Issues y Github Projects

Las tareas pendientes se gestionan mediante issues. Cada vez que se tenga un objetivo particular para el desarrollo, se anota un issue. Cuando se genere un commit que avance dicha tarea, se etiqueta con el número correspondiente al issue. De esta forma, todas las confirmaciones relacionadas con la tarea quedan recogidas en la página web.

Esto permite una gestión muy eficiente de los principales problemas y objetivos pendientes de la aplicación.

![Pestaña de issues, día 16 de abril de 2022](./img/08/Issues.png){ #fig:github_issues width=80%}

Los issues se agrupan en *milestones*, o productos mínimamente viables. Estos issues suelen estar relacionados con algún apartado importante del desarrollo.

![Los *milestones* agrupan una serie de issues relacionados con un punto clave del desarrollo](./img/08/Milestones.png){ #fig:github_milestones width=80% }

De esta forma, podemos ver todo lo que queda pendiente para la fecha de entrega.

Para añadir mayor granularidad a la gestión de tareas y proporcionar una vista informativa, se utiliza Github Projects. En esencia, esta aplicación es un acompañante del repositorio estilo Asana.

![Projects agrupa los issues y les asigna prioridades](./img/08/Projects.png){   #fig:github_projects width=80% }

Una de las alternativas que se planteó al inicio fue **Linear** [@linear], una aplicación de gestión de issues similar a Projects. Sin embargo, la conveniencia de tener Projects integrado en Github supuso un punto a favor para este gestor. De todas formas, el equipo de desarrollo se compone de una persona, así que no hace falta complicar excesivamente el *workflow*.

El desarrollo general de la documentación no ha seguido este sistema de issues, pues está sujeta a cambios constantes y cada commit está marcado con `[:notebook:]`. No obstante, ciertos problemas relacionados con ella, como puede ser el formato de entrega, sí que quedan recogidos como un issue.

Finalmente, cuando se produce un cambio significativo en la aplicación (como puede ser una refactorización, una implementación considerablemente más compleja...) se genera una nueva rama. Cuando se ha cumplido el objetivo, se *mergea* la rama con la principal `main` mediante un *pull request*. Esto proporciona un mecanismo de robustez ante cambios complejos.

### Estilo de commits

Una de los detalles que has podido apreciar si has entrado al repositorio es un estilo de commit un tanto inusual. Aunque parece un detalle de lo más insustancial, añadir emojis a los mensajes de commits añade un toque particular al repositorio, y permite identificar rápidamente el tipo de cambio.

Cada uno tiene un significado particular. En esta tabla se recogen sus significados:

![Los emojis permiten reconocer el objetivo de cada commit. Esta tabla recoge el significado de cada uno](./img/08/Commits.png){#fig:github_emojis}