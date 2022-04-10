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

> TODO: creo... que esto no debería ir aquí. Pero no quiero tampoco que el capítulo de radiometría sea un tocho impresionante.

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

## Fuentes de luz

> TODO: point lights, area lights, ambient lights...
>
> TODO: estas son notas muy puntuales (*como las luces, jeje*). Ya las revisaré más adelante.

La interfaz se encuentra en `host_device.h`. Describe cómo comunicarse con la GPU.

Ahora mismo, tenemos 3 constantes: tipo de luz:

```glsl
vec3  lightPosition;    // (x, y, z)
float lightIntensity;   // (Intensidad)
int   lightType;        // (0 => point light, 1 => area light)
```

Sería interesante añadir algunas constantes para controlar el tamaño (radio, posición, normal para las de área...)

### Point lights + spotlights

> pbr-book, point lights: *"Strictly speaking, it is incorrect to describe the light arriving at a point due to a point light source using units of radiance. Radiant intensity is instead the proper unit for describing emission from a point light source, as explained in Section 5.4. In the light source interfaces here, however, we will abuse terminology and use Sample_Li() methods to report the illumination arriving at a point for all types of light sources, dividing radiant intensity by the squared distance to the point p to convert units. Section 14.2 revisits the details of this issue in its discussion of how delta distributions affect evaluation of the integral in the scattering equation. In the end, the correctness of the computation does not suffer from this fudge, and it makes the implementation of light transport algorithms more straightforward by not requiring them to use different interfaces for different types of lights."*

```cpp
// https://github.com/mmp/pbrt-v3/blob/master/src/lights/point.h
// https://github.com/mmp/pbrt-v3/blob/master/src/lights/point.cpp

Spectrum sample_light(interaccion, vec2 u, vec3 wi, float pdf, visibility_tester) {
    wi = normalize(posicion_luz - interraccion.p);
    pdf = 1.f;
    // testeo de visibilidad. Opcional, I guess.

    return intensidad / distancia_al_cuadrado(posicion_luz, interraccion.p);
}
```

La potencia total emitida por la luz puede calcularse integrando la intensidad desprendida en toda su superficie. Asumiendo la intensidad constante:

$$
\Phi = \int_{\mathbb{S}^2}{I d\omega} = I \int_{\mathbb{S}^2}{d\omega} = 4 \pi I
$$

Las spotlights son variaciones de las point lights iluminando en un cono.

### Fuentes de área

Para simplificar la implementación, podemos asumir que son rectangulares.

Nos van a hacer falta técnicas de Monte Carlo para solucionar el problema de calcular integrales a lo largo de su superficie.

Primero, lo mejor es asumir un cuadrado, y después, extender la interfaz para meter otras formas (es decir, rectángulos. Porque lo otro sería mucha parafernalia innecesaria).

[Código fuente](https://github.com/mmp/pbrt-v3/blob/aaa552a4b9cbf9dccb71450f47b268e0ed6370e2/src/core/light.h)


<hr>

## Referencias {.unlisted .unnumbered}

- https://github.com/dannyfritz/awesome-ray-tracing
- https://www.wikiwand.com/en/Radeon
- https://www.wikiwand.com/en/List_of_Nvidia_graphics_processing_units#/GeForce_30_series