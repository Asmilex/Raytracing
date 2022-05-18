---
title: Los fundamentos de Ray Tracing
author: Andrés Millán Muñoz
keywords: [raytracing, ray tracing, Monte Carlo, Monte Carlo integration, radiometry, path tracing, Vulkan]
link-citations: true
header-includes: |
    \usepackage{caption}
    \usepackage{subcaption}
---

# Abstract{.unnumbered}

Este trabajo explorará las técnicas modernas de informática gráfica físicamente fieles basadas en *ray tracing* en tiempo real. Para ello, se usarán métodos de integración de Monte Carlo dado que disminuyen el tiempo necesario de cómputo.

Para conseguirlo, se ha diseñado un software basado en la interfaz de programación de aplicaciones gráficas Vulkan, usando como base un entorno de desarrollo de Nvidia conocido como nvpro-samples. El software implementa un motor gráfico basado en *path tracing*. Este motor será capaz de muestrear fuentes de iluminación de forma directa, lo que se conoce como *next-event estimation*. Para disminuir el tiempo de cómputo y hacerlo viable en tiempo real, se usarán técnicas de Monte Carlo para integrar radiancia. Se explorarán cómo afectan los diferentes métodos al ruido final de la imagen.

Este motor se comparará con una implementación puramente en CPU basada en el software desarrollado en los libros de [@Shirley2020RTW1] "Ray Tracing in One Weekend series". Se han estudiado las diferencias de tiempo entre una implementación y otra, sus ventajas y desventajas y el ruido de las imágenes producidas.

*Palabras clave: raytracing, ray tracing, Monte Carlo, Monte Carlo integration, radiometry, path tracing, Vulkan.*

<hr>

# A brief overview{.unnumbered}

> TODO

*Keywords: raytracing, ray tracing, Monte Carlo, Monte Carlo integration, radiometry, path tracing, Vulkan.*

\newpage

\vspace*{\fill}

# Dedicatoria{.unnumbered}

¡Parece que has llegado un poco pronto! Si lo has hecho voluntariamente, ¡muchas gracias! Este proyecto debería estar finalizado en verano de 2022. Mientras tanto, actualizaré poco a poco el contenido. Si quieres ir comprobando los progresos, puedes visitar [Asmilex/Raytracing](github.com/Asmilex/Raytracing) en Github para ver el estado del desarrollo.

Aun así, hay mucha gente que me ha ayudado a sacar este proyecto hacia delante.

Gracias, en primer lugar, a mi familia por permitirme acabar la carrera. A Blanca, Cristina, Jorge, Jose OC, Lucas, Mari, Marina y Paula, Sergio por ayudarme con el contenido, feedback del desarrollo y guía de diseño.

\vspace*{\fill}
