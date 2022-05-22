---
title: Los fundamentos de Ray Tracing
author: Andrés Millán Muñoz
keywords: [raytracing, ray tracing, Monte Carlo, Monte Carlo integration, radiometry, path tracing, Vulkan]
link-citations: true
header-includes: |
    \usepackage{caption}
    \usepackage{subcaption}
fignos-caption-name: "Figura"
---

# Abstract{.unnumbered}

En este trabajo se explorarán las técnicas modernas de informática gráfica físicamente realistas basadas en *ray tracing* en tiempo real. Para ello, se utilizarán métodos de integración de Monte Carlo con el fin de disminuir el tiempo de cómputo.

Se diseñará un software basado en la interfaz de programación de aplicaciones gráficas Vulkan, utilizando como soporte un entorno de desarrollo de Nvidia conocido como nvpro-samples. El software implementará un motor gráfico basado en *path tracing*. Este motor será capaz de renderizar numerosas escenas, cambiar los parámetros del algoritmo path tracing y modificar las fuentes de iluminación en tiempo de ejecución.

Con el fin de explorar cómo afectan diferentes métodos al ruido final de la imagen, se estudiarán algunas técnicas de reducción de varianza como muestreo directo de fuentes de iluminación, muestreo por importancia o acumulación temporal. Además, el motor desarrollado se comparará con una implementación puramente en CPU basada en el software creado en los libros de [@Shirley2020RTW1] "Ray Tracing in One Weekend series". Se comporbarán las diferencias entre ambas versiones, estudiando los puntos fuertes de cada una.

*Palabras clave: raytracing, ray tracing, path tracing, métodos de Monte Carlo, integración de Monte Carlo, transporte de luz, iluminación global, Vulkan.*

<hr>

# A brief overview{.unnumbered}

> TODO

*Keywords: raytracing, ray tracing, path tracing, Monte Carlo methods, Monte Carlo integration, light transport, global illumination, Vulkan.*

\newpage

\vspace*{\fill}

# Dedicatoria{.unnumbered}

¡Parece que has llegado un poco pronto! Si lo has hecho voluntariamente, ¡muchas gracias! Este proyecto debería estar finalizado en verano de 2022. Mientras tanto, actualizaré poco a poco el contenido. Si quieres ir comprobando los progresos, puedes visitar [Asmilex/Raytracing](github.com/Asmilex/Raytracing) en Github para ver el estado del desarrollo.

Aun así, hay mucha gente que me ha ayudado a sacar este proyecto hacia delante.

Gracias, en primer lugar, a mi familia por permitirme acabar la carrera. A Blanca, Cristina, Jorge, Jose OC, Lucas, Mari, Marina, Mapachana y Paula, Sergio por ayudarme con el contenido, feedback del desarrollo y guía de diseño.

\vspace*{\fill}
