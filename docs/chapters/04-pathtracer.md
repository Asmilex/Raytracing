# ¡Construyamos un path tracer!

> TODO: una pequeña introducción. Hablar de los offline renderers disponibles. Tras esto, dar paso a los frameworks de real time RT.
> Esta sección estará fuertemente basada en el tutorial de Nvidia de [nvpro samples](https://nvpro-samples.github.io/vk_raytracing_tutorial_KHR/) + [mini path tracer](https://nvpro-samples.github.io/vk_mini_path_tracer/index.html).

## Requisitos de *real time ray tracing*

### Arquitecturas de gráficas

> Turing, Ampere, RDNA2, Arc...
> NOTE: sería interesante enlazarlo con la sección de rendimiento.

### Frameworks y API de ray tracing en tiempo real

> TODO: hablar de DXR, Vulkan, OptiX... Hablar de por qué he escogido Vulkan + Nvidia Designworks (nv-pro samples)

## Setup del proyecto

## RT pipeline

## Estructuras de aceleración

> TODO: En esencia, hablar de la parafernalia que montamos con el BLAS y el TLAS.

## Shaders

### Tipos de shaders en RT

### Shader binding table

## Asmiray

## Transporte de luz

Hemos llegado a una de las partes más importantes de este trabajo. Es el momento de poner en concordancia todo lo que hemos visto a lo largo de los capítulos anteriores.

Empecemos por la dispersión. ¿Recuerdas la ecuación de dispersión [@eq:scattering_equation]? Podemos estimarla utilizando Monte Carlo:

$$
\begin{aligned}
L_o(p, \omega_o) & = \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\abs{\cos\theta_i} d\omega_i} \\
                 & \approx \frac{1}{N} \sum_{j = 1}^{N}{\frac{f(p, \omega_o \leftarrow \omega_j) L_i(p, \omega_j) \abs{\cos\theta_j}}{p(\omega_j)}}
\end{aligned}
$$

### Materiales y objetos

> NOTE: esto son notas para el Andrés del futuro. Sí, lo sé, está bastante claro solo con leerlo (⊙_⊙;)

Si quiero meter las BxDFs en los materiales tal y como tenía pensado (es decir, unas cuantas flags que me indiquen la BxDF que tengo que usar), tengo que...

1. Modificar `common/obj_loader.h/MaterialObj` para meterle las flags necesarias.
2. Modificar acordemente `shaders/host_device.h/WaveFronMaterial`.
3. Secuestrar `ObjLoader::loadModel()` para indicarle los parámetros nuevos.
4. (*Creo que no hace falta tocar `HelloVk::loadModel()` de esta manera*)
5. Toquetear los shaders para que me saque las flags.

CREO que de esta manera no me va a hacer falta tocar framebuffers. Simplemente, todo dependerá de mi material y ya.

*Creo*.

<hr>

## Referencias {.unlisted .unnumbered}

- https://github.com/dannyfritz/awesome-ray-tracing
- https://www.wikiwand.com/en/Radeon
- https://www.wikiwand.com/en/List_of_Nvidia_graphics_processing_units#/GeForce_30_series
-