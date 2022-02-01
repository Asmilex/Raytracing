<h1 align=center>🔦 Un TFG sobre Raytracing 🔦</h1>

<div align=center>
  <a href="https://asmilex.github.io/Raytracing"> asmilex.github.io/Raytracing </a>
</div>

<br>

<div align=center>
  <img src="https://img.shields.io/github/workflow/status/Asmilex/Raytracing/Publicar%20a%20Github%20Pages?color=00b894&label=WEB&style=for-the-badge">
  <img src="https://img.shields.io/github/workflow/status/Asmilex/Raytracing/Construir%20PDF?color=%23fdcb6e&label=PDF&style=for-the-badge">
</div>

<hr>


<p align="center" dir="auto">
  <a href="#bulb-y-esto-de-qu%C3%A9-va-exactamente">Sobre el proyecto</a> •
  <a href="#gear-instalaci%C3%B3n">Instalación</a> •
  <a href="#evergreen_tree-estructura-del-repositorio">Estructura</a> •
  <a href="#handshake-contribuyendo">Contribuyendo</a> •
  <a href="#link-enlaces-de-inter%C3%A9s">Enlaces de interés</a>
</p>

<div align=center>
  <img align=center src="https://i.imgur.com/RrWQVRE.jpg" width="600">
  <p align=center><i>En el futuro, esto será una foto mucho más chula</i></p>
</div>

## :bulb: Y esto, de qué va exactamente?

Este repositorio contiene una implementación del ray tracer de Peter Shirley en VkRay, así como la documentación asociada al desarrollo.

Ah, también es el trabajo de fin de grado de mi doble grado en Ingeniería Informática y Matemáticas 🎓.

Actualmente se encuentra en una fase temprana de desarrollo. Con un poco de suerte, ¡debería estar listo para verano de 2022!

## :gear: Instalación

Lo siento, ¡todavía no está listo! Vuelve en el futuro.

## :evergreen_tree: Estructura del repositorio

A fecha 2022-01-31,  el repositorio está compuesto por los siguientes directorios:

- [`./docs`](./docs): Documentación del proyecto. Toda la documentación está basada en markdown. Para convertirla en PDF y desplegarla en la web, se utiliza Pandoc. Tienes más información en el [README](./docs/README.md) correspondiente.
- [`./RT_in_one_weekend`](./RT_in_one_weekend): Código del ray tracer de la [serie de libros de Shirley](https://raytracing.github.io/) original en CPU.
- [`./application`](./application): El futuro ray tracer en GPU se ubicará aquí. Ahora mismo está en pañales.

![Visualización de la codebase](./docs/img/repo_diagram.svg)


## :handshake: Contribuyendo

### :open_book: Github Projects

Puedes acceder a [Github Projects](https://github.com/users/Asmilex/projects/2) para ver la gestión de las tareas. Resulta especialmente útil para saber en lo que se está trabajando actualmente.

### :eyeglasses: Guía de estilo

Cada commit debería ir identificado con un emoji antes del mensaje, así como terminar en un issue enlazado. Por ejemplo, `[📓] Descripción de función de densidad (#71)`

Estos son los iconos usados, así como su significado:

| Tipo de commit           |   Emoji    | Cómo se escribe rápidamente |
|:-------------------------|:----------:|:----------------------------|
| Documentación            | :notebook: | `:notebook:`                |
| Archivo de configuración |  :wrench:  | `:wrench:`                  |
| Integración continua     |     👷     | `:construction_worker:`     |
| Bugfix                   |   :bug:    | `:bug:`                     |

Los issues también deberían seguir el mismo estilo. Tienes un ejemplo [aquí](https://github.com/Asmilex/Raytracing/issues/4)


## :link: Enlaces de interés

En esta sección se recopilan enlaces útiles para el desarrollo del motor. Se irá actualizando conforme avance.

*(Y, seguramente, desaparezca en el futuro)*

- [Libro de Shirley](https://raytracing.github.io/)
- [Tutorial de Nvidia de VkRay](https://nvpro-samples.github.io/vk_raytracing_tutorial_KHR/)
- [Arquitectura de Turing](https://developer.nvidia.com/blog/nvidia-turing-architecture-in-depth/)
- [Nvidia design works RT tutorial](https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR)
- [Libro de Shirley implementado en NvVkRay](https://github.com/GPSnoopy/RayTracingInVulkan)
- [Métodos de Monte Carlo](http://statweb.stanford.edu/~owen/mc/)
- [Physically Based Rendering](http://www.pbr-book.org/)
- [Template de Latex ClassicThesis](https://bitbucket.org/amiede/classicthesis/downloads)
- [Lista con recursos de RT](https://www.realtimerendering.com/raytracing.html)
- [Tutorial de Vulkan](https://vulkan-tutorial.com/Introduction)
- [Vulkan con CMake](https://vulkan.lunarg.com/doc/view/1.1.108.0/windows/getting_started.html)
- [Explicación interesante sobre GI](https://www.youtube.com/watch?v=yEkryaaAsBU)
- [Ray Tracing Gems II](https://developer.nvidia.com/ray-tracing-gems-ii)
- Sobre Cmake y vcpkg:
  - [Tutorial pequeño](https://www.40tude.fr/how-to-use-vcpkg-with-vscode-and-cmake/)
  - [Sistema automatizado](https://cpptruths.blogspot.com/2019/03/bootstrapping-vcpkg-based-cmake-project.html)
  - [Blog 101](https://gamefromscratch.com/vcpkg-cpp-easy-mode-step-by-step-tutorial/)
  - [Otro blog 101](https://sam.elborai.me/blog/vscode-cpp-dev-environment-2020)
