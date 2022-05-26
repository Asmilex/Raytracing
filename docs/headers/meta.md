---
title: Los fundamentos de Ray Tracing en tiempo real
author: Andrés Millán Muñoz
keywords: [raytracing, ray tracing, Monte Carlo, Monte Carlo integration, radiometry, path tracing, Vulkan]
link-citations: true
header-includes: |
    \usepackage{caption}
    \usepackage{subcaption}
fignos-caption-name: "Figura"
---

# Sinopsis{.unnumbered}

En este trabajo se explorarán las técnicas modernas de informática gráfica físicamente realistas basadas en *ray tracing* en tiempo real. Para ello, se utilizarán métodos de integración de Monte Carlo con el fin de disminuir el tiempo de cómputo.

Se diseñará un software basado en la interfaz de programación de aplicaciones gráficas Vulkan, utilizando como soporte un entorno de desarrollo de Nvidia conocido como nvpro-samples. El software implementará un motor gráfico basado en *path tracing*. Este motor será capaz de renderizar numerosas escenas, cambiar los parámetros del algoritmo path tracing y modificar las fuentes de iluminación en tiempo de ejecución.

Con el fin de explorar cómo afectan diferentes métodos al ruido final de la imagen, se estudiarán algunas técnicas de reducción de varianza como muestreo directo de fuentes de iluminación, muestreo por importancia o acumulación temporal. Además, el motor desarrollado se comparará con una implementación del software creado en los libros de [@Shirley2020RTW1] "Ray Tracing in One Weekend series", la cual utiliza exclusivamente la CPU. Se comporbarán las diferencias entre ambas versiones, estudiando los puntos fuertes de cada una.

*Palabras clave: raytracing, ray tracing, path tracing, métodos de Monte Carlo, integración de Monte Carlo, transporte de luz, iluminación global, Vulkan.*


# A brief overview{.unnumbered}

<!-- LTeX: language=en -->

Being able to capture a moment. Every human civilization in record has always found a way to immortalise the idiosyncrasies of its sociecy. We are not inmue to this phenomenon, so it is only natural we try to use the lastest technology to achieve the most faithful representation of our world. Computers have the ability to produce realistic images and astonishing simulations. Although, while there have been tremendous advances in the field, many of the techniques we use today are still based on the old days of computation; specially in 3D rendering.

**Rasterization** is one of those methods. It allows to transform a virtual environment to a raster image --a set of pixels--. It is an extremelly fast method given how it works, which is just a mapping of the scene geometry into a 2D plane. Its simplicity makes it a great quick way of rendering an image, but it comes at the expense of fidelity.

We can solve this issue with **Ray tracing**. Instead of projecting virtual geometry into a plane, this algorithm tries to simulate how light works by casting rays of light into the scene from the camera. These rays then intersect with objects and collect information about them. This has one key advantage over rasterization: it allows seeing parts of the scene which were not visible to the camera.

**Path tracing** takes this idea into the next level: instead of stopping in the first intersection, it makes the rays bounce freely through the scene. These rays gather information about object materials, light sources and many more elements. Then, the algorithm computes the color of the pixel by considering how much light have those rays acummulated over their traversal. Given the physically accurate nature of the algorithm, the results it produces are unmatched. But it comes with **a major problem: rendering times**.

Rendering times can be crucial depending on the task at hand. Rasterization has been traditionally used in real time applications, such as video games, while path tracing is implemented in offline renderers, like the ones used for movie productions. We then need to ask one question:

**Could it be possible to bring path tracing to real time?**

In this project we will tackle this problem. We will build a physically based rendering engine based on path tracing with modern hardware, in order to produce a software capable of producing realistic images in mere milliseconds.

As one could imagine, this task it not an easy one. In order to achieve it, we will need to understand multiple aspects of different fields: from mathematics to computer imaging, with some light notions from physics.

[Chapter 1](#las-bases) will set the **fundamentals of what is ray tracing**, how does a ray work, and how can we intersect a ray with an object.

On [chapter 2](#transporte-de-luz) we will study the basics of **radiometry and light transport**, the field of physics that deals with the interaction between light and matter. We will introduce how photons work, how are they emitted from light sources and how do they travel through the environment. We will also need to understand the mathematical abstractions in which we represent these radiometric quantities. Then, we will need to analyse how light interacts with matter, which, in a nutshell, can be described by the bidirectional scattering distribution function of the material surface and the outgoing direction of photons --since light can be either scattered or transmitted--. At the end of the chapter we will obtain the most important equation in computer graphics: **the rendering equation**, which can be described as

$$
L_o(p, \omega_o) = L_e(p, \omega_o) + \int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) L_i(p, \omega_i) \cos\theta_i\ d\omega_i}
$$

Since reality is an absurdly complex --yet simple-- set of rules, we will need to find a way of making the computations viable in real time. That's why in [chapter 3](#métodos-de-monte-carlo) we will explore how random sampling can be used to generate approximations of radiometric integrals. We will start with a brief summary of random variables. Then, we will introduce **Monte Carlo techniques**, which rely on sampling random variables from certain distributions (usually the uniform between 0 and 1) and averaging these samples to obtain the expected value of some other distribution. This will allow us to compute integrals relatively easily at the cost of some error. In order to improve these estimations, we will study how different methods can be used to reduce the variance of the estimator, such as (multiple) importance sampling, roussian roulette or next-event estimation.

Once the theory is out of the way, it will be time to build an application. [Chapter 4](#construyamos-un-path-tracer) will cover the **implementation of the engine**. We will first introduce our main set of tools, which will be Vulkan graphics API and Nvidia DesignWorks' nvpro-samples framework. Then, we will learn about the structures that make real time ray tracing possible, such as the Top and Bottom-Level Acceleration Structures (TLAS and BLAS respectively), the Shader Binding Table (SBT). We will also understand how does the ray tracing pipepline works, what types of shaders are there and how can we use them to render a scene. Materials and light sources will also need to be explained, since they are a key component of the scene. Finally, we will implement some techniques which allow to reduce the noise of the final image, such as gamma correction and temporal accumulation of frames.

Now that we have a working engine, it's time to play with it. [Chapter 5](#análisis-de-rendimiento) will **exhibit the results of our work**, comparing different scenes which showcase a variety of physical phenomenons. We will also analyse how it performs in terms of image quality and frame time based on a set of parameters, like ray depth, number of samples taken on each frame and resolution. Finally, we will **compare our implementation** with Peter Shirley's engine developed in *Ray Tracing In One Weekend* series [@Shirley2020RTW1] [@Shirley2020RTW2] [@Shirley2020RTW1].

We will end the project with the [conclusions](#conclusiones) that can be drawn from the results of our work, as well as show what could be done to improve the engine in the future. To summarize, we've found that the engine could improve its sampling stategy. The images it produces are very noisy and need a considerable ammount of samples to become sharp. This is mainly due to the weak light interface that has been implemented, since it doesn't take into account emissive materials found in the scene. Nevertheless, the combination of how fast it can render a frame and the ability to mix multiple accumulated frames over time manages to overcome this issue. Other areas that could be further improved include the material interface and the main engine class, which presents a high degree of coupling.

<hr>

The contents of this document, the Vulkan project, an *In One Weekend* implementation and other utilities can be found in the following repository:

[https://github.com/Asmilex/Raytracing](https://github.com/Asmilex/Raytracing)

*Keywords: raytracing, ray tracing, path tracing, Monte Carlo methods, Monte Carlo integration, light transport, global illumination, Vulkan.*

\newpage

\vspace*{\fill}

# Dedicatoria{.unnumbered}

A mi familia por su apoyo constante y permitirme empezar una carrera que ni siquiera sabía que quería.

A Blanca, Cristina, Jorge, José *"OC"*, Lucas, Mari, Marina, Mapachana y Paula, Sergio por ayudarme con el contenido, feedback del desarrollo y diseño de la documentación.

\vspace*{\fill}
