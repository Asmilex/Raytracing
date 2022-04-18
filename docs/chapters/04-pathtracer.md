# ¡Construyamos un path tracer!

Ahora que hemos introducido toda la teoría necesaria, es hora de ponernos manos a la obra. En este capítulo, vamos a escoger una serie de herramientas y haremos una pequeña implementación de un motor de path tracing en tiempo real.

La implementación estará basada en Vulkan, junto al pequeño framework de nvpro-samples. El motor mantendrá el mismo espíritu que la serie de [@Shirley2020RTW1], Ray Tracing In One Weekend.

Le pondremos especial atención a los conceptos claves. Vulkan tiende a crear código muy verboso, por lo que se documentarán únicamente las partes más importantes.

## Requisitos de *real time ray tracing*

Como es natural, el tiempo es una limitación enorme para cualquier programa en tiempo real. Mientras que en un *offline renderer* disponemos de un tiempo muy considerable por frame (hablamos de varios segundos), en un programa en tiempo real necesitamos que un frame salga en 16 milisegundos o menos. Este concepto se suele denominar *frame budget*: la cantidad de tiempo que disponemos para un frame.

> **Nota**: cuando hablamos del tiempo disponible para un frame, solemos hablar en milisegundos (ms) o frames por segundo (FPS). Para que un motor vaya suficientemente fluido, necesitaremos que el motor corra a un mínimo de 30 FPS (que equivalen a 33 ms por frame). Hoy en día, debido al avance del área en campos como los videosjuegos, el estándar se está convirtiendo en 60 FPS (16 ms/frame).

Las nociones anteriores no distinguen entre un motor en tiempo real y *offline*. Como es natural, necesitaremos introducir unos pocos conceptos más para llevarlo a tiempo real. Además, existe una serie de requisitos hardware que debemos cumplir para que un motor en tiempo real con ray tracing funcione.

### Arquitecturas de gráficas

> NOTE: sería interesante enlazarlo con la sección de rendimiento.

El requisito más importante de todos es la gráfica. Para ser capaces de realizar cálculos de ray tracing en tiempo real, necesitaremos una arquitectura moderna con núcleos dedicados a este tipo de cáclulos [^4].

A día 17 de abril de 2022, para correr ray tracing en tiempo real, se necesita alguna de las siguientes tarjetas gráficas:

| **Arquitectura**              | **Fabricante** | **Modelos de gráficas**                                                                              |
|-------------------------------|----------------|------------------------------------------------------------------------------------------------------|
| **Turing**                    | Nvidia         | RTX 2060, RTX 2060 Super, RTX 2070, RTX 2070 Super, RTX 2080, RTX 2080 Super, RTX 2080 Ti, RTX Titan |
| **Ampere**                    | Nvidia         | RTX 3050, RTX 3060, RTX 3060 Ti, RTX 3070, RTX 3070 Ti, RTX 3080, RTX 3080 Ti, RTX 3090, RTX 3090 Ti |
| **RDNA2** (Navi 2X, Big Navi) | AMD            | RX 6400, RX 6500 XT, RX 6600, RX 6600 XT, RX 6700 XT, RX 6800, RX 6800 XT, RX 6900 XT                |
| **Arc Alchemist**             | Intel          | *No reveleado aún*                                                                                   |

Solo se han incluido las gráficas de escritorio de consumidor.

Para este trabajo se ha utilizado una **RTX 2070 Super**. En el capítulo de análisis del rendimiento se hablará con mayor profundidad de este apartado.

### Frameworks y API de ray tracing en tiempo real

Una vez hemos cumplido los requisitos de hardware, es hora de escoger los frameworks de trabajo.

Las API de gráficos están empezando a adaptarse a los requisitos del tiempo real, por lo que cambian frecuentemente. La mayoría adquirieron las directivas necesarias muy recientemente. Aun así, son lo suficientemente sólidas para que se pueda usar en aplicaciones empresariales de gran embergadura.

Esta es una lista de las API disponibles con capacidades de Ray Tracing disponibles para, al menos, la arquitectura Turing:

- Vulkan (los bindings de ray tracing se denominan KHR).
- Microsoft DirectX Ray Tracing (DXR), una extensión de DirectX 12.
- Nvidia OptiX.

De momento, no hay mucho donde elegir.

OptiX es la API más vieja de todas. Su primera versión salió en 2009, mientras que la última estable es de 2021. Tradicionalmente se ha usado para offline renderers, y no tiene un especial interés para este trabajo estando las otras dos disponibles.

Tanto DXR como Vulkan son los candidatos más sólidos. DXR salió en 2018, con la llegada de Turing. Es un par de años más reciente que Vulkan KHR. Cualquiera de las dos cumpliría su cometido de forma exitosa. Sin embargo, para este trabajo, **hemos escogido Vulkan** por los siguientes motivos:

- DirectX 12 está destinado principalmente a plataformas de Microsoft. Es decir, está pensado para sistemas operativos Windows 10 o mayor [^5].
- Vulkan está apoyado principalmente por AMD. Esto sigue las línas de la su política de empresa de apoyar el código abierto. Además, resulta más sencillo exportarlo a otros sistemas operativos.

Ambas API se comportan de manera muy similar, y no existe una gran diferencia entre ellas; tanto en rendimiento como en complejidad de desarrollo. Actualmente el proyecto solo compila en Windows 10 o mayor, por lo que estos dos puntos no resultan especialmente relevantes para el trabajo.

## Setup del proyecto

Un proyecto de Vulkan necesita una cantidad de código inicial considerable. Para acelerar este trámite y partir de una base más sólida, se ha decidido usar un pequeño framework de trabajo de Nvidia llamado [nvpro-samples](https://github.com/nvpro-samples).

Esta serie de repositorios contienen proyectos de ray tracing de Nvidia con fines didácticos. Nosotros usaremos [vk_raytracing_tutorial_KHR](https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR), pues ejemplifica cómo añadir ray tracing en tiempo real a un proyecto de Vulkan.

Nuestro repositorio utiliza los citados anteriormente para compilar su proyecto. El Makefile es una modificación del que se usa para ejecutar los ejemplos de Nvidia. Por defecto, ejecuta una aplicación muy simple que muestra un cubo mediante rasterización.

## Compilación

Las dependencias necesarias para compilarlo son:

1. CMake.
2. Un driver de Nvidia compatible con la extensión `VK_KHR_ray_tracing_pipeline`.
3. El SDK de Vulkan, versión 1.2.161 o mayor.

La parte inicial del desarrollo consiste en adaptar Vulkan para usar la extensión de ray tracing, extrayendo la información de la gráfica y cargando correspondientemente el dispositivo.

Para compilarlo, ejecuta los siguientes comandos:

```sh
git clone git@github.com:Asmilex/Raytracing.git
cd .\application\vulkan_ray_tracing\
mkdir build
cd build
cmake ..
```

Si todo funciona correctamente, debería generarse un binario en `./application/bin_x64/Debug` llamado `asmiray.exe`.

## Estructuras de aceleración

El principal coste de ray tracing es el cálculo de las intersecciones con objetos; hasta un 95% del tiempo de ejecución total [@scratchapixel-2019]. Reducir el número de test de intersección es clave.

Las **estructuras de aceleración** son una forma de representar la geometría de la escena. Aunque hay varios tipos diferentes, en esencia, engloban a un objeto o varios en una estructura con la que resulta más eficiente hacer test de intersección. Son similares a los grafos de escena de un rasterizador.

Uno de los tipos más comunes es la **Bounding Volume Hierarchy (BVH)**. Fue una técnica desarrollada por Kay y Kajilla en 1986. En esencia, este método encierra un objeto en una caja (lo que se denomina una **bounding box**), de forma que el test de intersección principal se hace con la caja y no con la geometría. Si un rayo impacta en la *bounding box*, entonces se pasa a testear la geometría.

Se puede repetir esta idea repetidamente, de forma que agrupemos varias *bounding boxes*. Así, creamos una jerarquía de objetos --como si nodos de un árbol se trataran--. A esta jerarquía es a la que llamamos BVH.

Es importante crear buenas divisiones de los objetos en la BVH. Cuanto más compacta sea una BVH, más eficiente será el test de intersección.

Una forma habitual de crear la BVH es mediante la división del espacio en una rejilla. Esta técnica se llama **Axis-Aligned Bounding Box (AABB)**. Usualmente se usa el método del *slab* (también introducido por Kay y Kajilla). Se divide el espacio en una caja n-dimensional alineada con los ejes, de forma que podemos verla como $[x_0, x_1] \times$ $[y_0, y_1] \times$ $[z_0, z_1] \times \dots$ De esta forma, comprobar si un rayo impacta en una bounding box es tan sencillo como comprobar que está dentro del intervalo. Este es el método que se ha usado en Ray Tracing in One Weekend.

Vulkan gestiona las estructuras de aceleración diviéndolas en dos partes: **Top-Level Acceleration Structure** (TLAS) y **Bottom-Level Acceleration Structure** (BLAS).

![La TLAS guarda información de las instancias de un objeto, así como una referencia a BLAS que contiene la geometría correspondiente. Fuente: Nvidia](./img/04/Acceleration%20structure.png)

> TODO: Deberíamos cambiar esa foto por otra propia.

### Botom-Level Acceleration Structure (BLAS)

Las Bottom-Level Acceleration Structure almacenan la geometría de un objeto individual; esto es, los vértices y los índices de los triángulos, además de una AABB que la encapsula.

Pueden almacenar varios modelos, puesto que alojan uno o más buffers de vértices junto a sus matrices de transformación. Si un modelo se instancia varias veces *dentro de la misma BLAS*, la geometría se duplica. Esto se hace para mejorar el rendimiento.

Como regla general, cuantas menos BLAS, mejor.

El código correspondiente a la creación de la BLAS en el programa es el siguiente:


```cpp
void HelloVulkan::createBottomLevelAS() {
    // BLAS - guardar cada primitiva en una geometría

    std::vector<nvvk::RaytracingBuilderKHR::BlasInput> allBlas;
    allBlas.reserve(m_objModel.size());

    for (const auto& obj: m_objModel) {
        auto blas = objectToVkGeometryKHR(obj);

        // Podríamos añadir más geometrías en cada BLAS.
        // De momento, solo una.
        allBlas.emplace_back(blas);
    }

    m_rtBuilder.buildBlas(
        allBlas,
        VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR
    );
}
```

### Top-Level Acceleration Structure (TLAS)

Las Top-Level Acceleration Structures almacenan las instancias de los objetos, cada una con su matriz de transformación y referencia a la BLAS correspondiente.

Además, guardan información sobre el *shading*. Así, los shaders pueden relacionar la geometría intersecada y el material de dicho objeto. En esta última parte jugará un papel fundamental la [Shader Binding Table](#shader-binding-table).

En el programa hacemos lo siguiente para construir la TLAS:

```cpp
void HelloVulkan::createTopLevelAS() {
    std::vector<VkAccelerationStructureInstanceKHR> tlas;
    tlas.reserve(m_instances.size());

    for (const HelloVulkan::ObjInstance& inst: m_instances) {
        VkAccelerationStructureInstanceKHR rayInst{};

        // Posición de la instancia
        rayInst.transform = nvvk::toTransformMatrixKHR(inst.transform);

        rayInst.instanceCustomIndex = inst.objIndex;

        // returns the acceleration structure device address of the blasId. The id correspond to the created BLAS in buildBlas.
        rayInst.accelerationStructureReference = m_rtBuilder.getBlasDeviceAddress(inst.objIndex);

        rayInst.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
        rayInst.mask  = 0xFF; // Solo registramos hit si rayMask & instance.mask != 0
        rayInst.instanceShaderBindingTableRecordOffset = 0; // Usaremos el mismo hit group para todos los objetos

        tlas.emplace_back(rayInst);
    }

    m_rtBuilder.buildTlas(
        tlas,
        VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR
    );
}
```

## Ray tracing descriptor set y pipeline

Primero, debemos introducir unas nociones básicas de Vulkan sobre cómo gestiona la información que se pasa a los shaders.


Un ***resource descriptor*** (usualmente lo abreviaremos como descriptor) es una forma de cargar recursos como buffers o imágenes para que la tarjeta gráfica los pueda utilizar; concretamente, los shaders. El ***descriptor layout*** especifica el tipo de recurso que va a ser accedido. Finalmente, el ***descriptor set*** determina el buffer o imagen que se va a asociar al descriptor. Este set es el que se utiliza en los **drawing commands**. Un **pipeline** es una secuencia de operaciones que reciben una geometría y sus texturas, y la transforma en unos pixels.

Todos estos conceptos aparecen desarrollados en [@overvoorde-2022]

Tradicionalmente, en rasterización se utiliza un descriptor set por tipo de material, y consecuentemente, un pipeline por cada tipo. En ray tracing esto no es posible, puesto que no se sabe qué material se va a usar: un rayo puede impactar *cualquier* material presente en la escena, lo cual invocaría un shader específico. Debido a esto, empaquetaremos todos los recursos en un único set de descriptores.

> TODO: empezar con la doc. de la pipeline (https://www.khronos.org/blog/vulkan-ray-tracing-best-practices-for-hybrid-rendering)

## Shaders

### Shader binding table

### Tipos de shaders

#### Ray generation shader
#### Closest hit shader
#### Miss shader
#### Anyhit shader

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
- https://www.eurogamer.net/digitalfoundry-2021-the-big-intel-interview-how-intel-alchemist-gpus-and-xess-upscaling-will-change-the-market
- https://www.intel.com/content/www/us/en/products/docs/arc-discrete-graphics/overview.html
- https://www.khronos.org/registry/vulkan/specs/1.2-khr-extensions/html/chap1.html
- https://www.wikiwand.com/en/OptiX
- https://www.wikiwand.com/en/DirectX_Raytracing
- https://www.wikiwand.com/es/Valve_Corporation
- https://www.phoronix.com/scan.php?page=news_item&px=VKD3D-Proton-2.5
- https://github.com/ValveSoftware/Proton
- https://nvpro-samples.github.io/vk_raytracing_tutorial_KHR/
- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure
- https://www.khronos.org/blog/vulkan-ray-tracing-best-practices-for-hybrid-rendering
- https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies
- https://vulkan-tutorial.com/en/Uniform_buffers/Descriptor_layout_and_buffer


[^4]: Esto no es del todo cierto. Aunque generalmente suelen ser excepciones debido al coste computacional de RT en tiempo real, existen algunas implementaciones que son capaces de correrlo por software. Notablemente, el motor de Crytek, CryEngine, es capaz de mover ray tracing basado en hardware y en software [@crytek-2020]
[^5]: Afortunadamente, esto tampoco es completamente cierto. La compañía desarrolladora y distribuidora de videojuegos Valve Corporation ha creado una pieza de software fascinante: [Proton](https://github.com/ValveSoftware/Proton). Proton utiliza Wine para emular software en Linux que solo puede correr en plataformas Windows. La versión 2.5 añadió soporte para traducción de bindings de DXR a KHR, lo que permite utilizar DirectX12 ray tracing en sistemas basados en Linux. El motivo de este software es expandir el mercado de videojuegos disponibles en su consola, la Steam Deck.