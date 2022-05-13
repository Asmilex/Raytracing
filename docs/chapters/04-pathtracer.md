# ¡Construyamos un path tracer!

Ahora que hemos introducido toda la teoría necesaria, es hora de ponernos manos a la obra. En este capítulo, vamos a escoger una serie de herramientas y haremos una pequeña implementación de un motor de path tracing en tiempo real.

La implementación estará basada en Vulkan, junto al pequeño framework de nvpro-samples. El motor mantendrá el mismo espíritu que la serie de [@Shirley2020RTW1], Ray Tracing In One Weekend.

Le pondremos especial atención a los conceptos claves. Vulkan tiende a crear código muy verboso, por lo que se documentarán únicamente las partes más importantes.

## Requisitos de ray tracing en tiempo real

Como es natural, el tiempo es una limitación enorme para cualquier programa en tiempo real. Mientras que en un *offline renderer* disponemos de un tiempo muy considerable por frame (desde varios segundos hasta horas), en un programa en tiempo real necesitamos que un frame salga en 16 milisegundos o menos. Este concepto se suele denominar *frame budget*: la cantidad de tiempo que disponemos para un frame.

> **Nota**: cuando hablamos del tiempo disponible para un frame, solemos utilizarmilisegundos (ms) o frames por segundo (FPS). Para que un programa en tiempo real vaya suficientemente fluido, necesitaremos que el motor corra a un mínimo de 30 FPS (que equivalen a 33 ms por frame). Hoy en día, debido al avance del área en campos como los videosjuegos, el estándar se está convirtiendo en 60 FPS (16 ms/frame).

Las nociones de los capítulos anteriores no distinguen entre un motor en tiempo real y *offline*. Como es natural, necesitaremos introducir unos pocos conceptos más para llevarlo a tiempo real. Además, existen una serie de requisitos hardware que debemos cumplir para que un motor en tiempo real con ray tracing funcione.

### Arquitecturas de gráficas

> TODO: Esta [página](https://alain.xyz/blog/comparison-of-modern-graphics-apis) es maravillosa *chef kiss*

El requisito más importante de todos es la gráfica. Para ser capaces de realizar cálculos de ray tracing en tiempo real, necesitaremos una arquitectura moderna con núcleos dedicados a este tipo de cáclulos [^4].

A día 17 de abril de 2022, para correr ray tracing en tiempo real, se necesita alguna de las siguientes tarjetas gráficas:

| **Arquitectura**              | **Fabricante** | **Modelos de gráficas**                                                                              |
|:------------------------------|:---------------|:----------------------------------------------------------------------------------------------------:|
| **Turing**                    | Nvidia         | RTX 2060, RTX 2060 Super, RTX 2070, RTX 2070 Super, RTX 2080, RTX 2080 Super, RTX 2080 Ti, RTX Titan |
| **Ampere**                    | Nvidia         | RTX 3050, RTX 3060, RTX 3060 Ti, RTX 3070, RTX 3070 Ti, RTX 3080, RTX 3080 Ti, RTX 3090, RTX 3090 Ti |
| **RDNA2** (Navi 2X, Big Navi) | AMD            | RX 6400, RX 6500 XT, RX 6600, RX 6600 XT, RX 6700 XT, RX 6800, RX 6800 XT, RX 6900 XT                |
| **Arc Alchemist**             | Intel          | *No reveleado aún*                                                                                   |

Se puede encontrar más información en las respectivas páginas de las compañías o los artículos de Wikipedia [@wikipedia-radeon], [@wikipedia-nvidia], [@intel-arc]. Solo se han incluido las gráficas de escritorio de consumidor.

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
- Vulkan, al estar apoyado principalmente por AMD y desarrollado por Khonos, es un proyecto de código. Su principal aliciente es la capacidad de correr en múltiples sistemas operativos, como Windows, distribuciones de Linux o Android.

Ambas API se comportan de manera muy similar, y no existe una gran diferencia entre ellas; tanto en rendimiento como en complejidad de desarrollo. Actualmente el proyecto solo compila en Windows 10 o mayor, por lo que estos dos puntos no resultan especialmente relevantes para el trabajo.

Si se desea, se puede encontrar una comparación más a fondo de las API en el blog de [@alain-API]

## Setup del proyecto

Un proyecto de Vulkan necesita una cantidad de código inicial considerable. Para acelerar este trámite y partir de una base más sólida, se ha decidido usar un pequeño framework de trabajo de Nvidia llamado [nvpro-samples] [@nvpro-samples].

Esta serie de repositorios de Nvidia DesignWorks contienen proyectos de ray tracing de Nvidia con fines didácticos. Nosotros usaremos **vk_raytracing_tutorial_KHR** [@nvpro-samples-tutorial], pues ejemplifica cómo añadir ray tracing en tiempo real a un proyecto de Vulkan. Estos frameworks contienen asimismo otras utilidades menores. Destacan **GLFW** (gestión de ventanas en C++), **imgui** (interfaz de usuario) y **tinyobjloader** (carga de `.obj` y `.mtl`).

Nuestro repositorio utiliza las herramientas citadas anteriormente para compilar su proyecto. El Makefile es una modificación del que se usa para ejecutar los ejemplos de Nvidia. Por defecto, ejecuta una aplicación muy simple que muestra un cubo mediante rasterización, la cual modificaremos hasta añadir ray tracing en tiempo real. Por tanto, la parte inicial del desarrollo consiste en adaptar Vulkan para usar la extensión de ray tracing, extrayendo la información de la gráfica y cargando correspondientemente el dispositivo.

![Por defecto, el programa muestra un cubo rasterizado muy simple. Es, prácticamente, un *hello world* gráfico](./img/04/Raster.jpg)

### Vistazo general a la estructura

La estructura final del proyecto (es decir, la carpeta `application`) es la siguiente:

- La carpeta `application/build` contiene todo lo relacionado con CMake y el ejecutable final.
- Las dependencias del proyecto se encuentran en el repositorio `application/nvpro_core`. Se descargan automáticamente seguir las instrucciones de compilación.
- En `application/vulkan_ray_tracing/media/` se encuentran todos los archivos `.obj`, `.mtl` y las texturas.
- La subcarpeta `application/vulkan_ray_tracing/src` contiene el código fuente de la propia aplicación.
  - Toda la implementación relacionada con el motor (y por tanto, Vulkan), se halla en `engine.h/cpp`. Una de las desventajas de seguir un framework "de juguete" es que el acoplamiento es considerablemente alto. Más adelante comentaremos los motivos.
  - Los parámetros de la aplicación (como tamaño de pantalla y otras estructuras comunes) se encuetran en `globals.hpp`.
  - La carga de escenas y los objetos se gestionan en `scene.hpp`.
  - En `main.cpp` se gestiona tanto el punto de entrada de la aplicación como la actualización de la interfaz gráfica.
  - La carpeta `application/vulkan_ray_tracing/src/shaders` contiene todos los shaders; tanto de rasterización, como de ray tracing.
    - Para ray tracing, se utilizan los `raytrace.*`, `pathtrace.glsl` (que contiene el grueso del path tracer).
    - En rasterización se usan principalmente `frag_shader.frag`, `passthrough.vert`, `post.frag`, `vert_shader.vert`.
    - El resto de shaders son archivos comunes a ambos o utilidades varias, como pueden ser `sampling.glsl` (donde se implementan distribuciones aleatorias) o `random.glsl` (que contiene generadores de números aleatorios).
  - Finalmente, la carpeta `application/vulkan_ray_tracing/src/spv` contiene los shaders compilados a SPIR-V.

## Compilación

Las dependencias necesarias son:

1. **CMake**.
2. Un **driver de Nvidia** compatible con la extensión `VK_KHR_ray_tracing_pipeline`.
3. El SDK de **Vulkan**, versión 1.2.161 o mayor.

Ejecuta los siguientes comandos desde la terminal para compilar el proyecto:

```sh
$ git clone --recursive --shallow-submodules https://github.com/Asmilex/Raytracing.git
$ cd .\Raytracing\application\vulkan_ray_tracing\
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

Si todo funciona correctamente, debería generarse un binario en `./application/bin_x64/Debug` llamado `asmiray.exe`. Desde la carpeta en la que deberías encontrarte tras seguir las instrucciones, puedes conseguir ejecutarlo con

```sh
$ ..\..\bin_x64\Debug\asmiray.exe
```

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
void Engine::createBottomLevelAS() {
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
void Engine::createTopLevelAS() {
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

## La ray tracing pipeline

### Descriptores y conceptos básicos

Primero, debemos introducir unas nociones básicas de Vulkan sobre cómo gestiona la información que se pasa a los shaders.

Un ***resource descriptor*** (usualmente lo abreviaremos como descriptor) es una forma de cargar recursos como buffers o imágenes para que la tarjeta gráfica los pueda utilizar; concretamente, los shaders. El ***descriptor layout*** especifica el tipo de recurso que va a ser accedido. Finalmente, el ***descriptor set*** determina el buffer o imagen que se va a asociar al descriptor. Este set es el que se utiliza en los **drawing commands**. Un **pipeline** es una secuencia de operaciones que reciben una geometría y sus texturas, y la transforma en unos pixels.

Si necesitas más información, todos estos conceptos aparecen desarrollados extensamente en [@overvoorde-2022]

Tradicionalmente, en rasterización se utiliza un descriptor set por tipo de material, y consecuentemente, un pipeline por cada tipo. En ray tracing esto no es posible, puesto que no se sabe qué material se va a usar: un rayo puede impactar *cualquier* material presente en la escena, lo cual invocaría un shader específico. Debido a esto, empaquetaremos todos los recursos en un único set de descriptores.

### La Shader binding table

Para solucionar esto, vamos a crear la **Shader Binding Table** (SBT). Esta estructura permitirá cargar el shader correspondiente dependiendo de dónde impacte un rayo.

Para cargar esta estructura, se debe hacer lo siguiente:

1. Cargar y compilar cada shader en un `VkShaderModule`.
2. Juntar los cada `VkShaderModule` en un array `VkPipelineShaderStageCreateInfo`.
3. Crear un array de `VkRayTracingShaderGroupCreateInfoKHR`. Cada elemento se convertirá al final en una entrada de la Shader Binding Table.
4. Compilar los dos arrays anteriores más un pipeline layout para generar un `vkCreateRayTracingPipelineKHR`.
5. Conseguir los *handlers* de los shaders usando `vkGetRayTracingShaderGroupHandlesKHR`.
6. Alojar un buffer con el bit `VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR` y copiar los *handlers*.

![La Shader Binding Table permite selccionar un tipo de shader dependiendo del objeto en el que se impacte. Para ello, se genera un rayo desde el shader `raygen`, el cual viaja a través de la Acceleration Structure. Dependiendo de dónde impacte, se utiliza un `closest hit`, `any hit`, o `miss` shaders. Fuente: Nvidia](./img/04/Pipeline.png)

Cada entrada de la SBT contiene un handler y una serie de parámetros embebidos. A esto se le conoce como **Shader Record**. Estos records se clasifican en:

- **Ray generation record**: contiene el handler del ray generation shader.
- **Hit group record**: se encargan de los handlers del closest hit, anyhit (opcional), e intersection (opcional).
- **Miss group record**: se encarga del miss shader.
- **Callable group record**.

Una de las partes más difíciles de la SBT es saber cómo se relacionan record y geometría. Es decir, cuando un rayo impacta en una geometría, ¿a qué record de la SBT llamamos? Esto se determina mediante los parámetros de la instancia, la llamada a *trace rays*, y el orden de la geometría en la BLAS.

![Fuente: https://www.willusher.io/](./img/04/SBT.png)

#### Cálculo de la entrada de la SBT

El principal problema es el cálculo del índice en los hit groups.

Llamemos al índice de cada instancia de una geometría en la BLAS $\mathbb{G}_{\text{ID}}$. A cada instancia se le puede asignar un desplazamiento con respecto a la SBT ($\mathbb{I}_{\text{offset}}$) desde donde empieza la subtabla de hit groups.

> TODO: esto se deja temporal de momento. No sé hasta qué punto me convence poner todo esto. Lo veo importante, pero no sé si *tan* importante. El recurso que estoy usando es https://www.willusher.io/graphics/2019/11/20/the-sbt-three-ways, por si al final decidimos escribirlo.


### Tipos de shaders

El pipeline soporta varios tipos de shaders diferentes que cubren la funcionalidad esencial de un ray tracer:

- **Ray generation shader**: es el punto de inicio del viaje de un rayo. Calcula punto de inicio y procesa el resultado final. Idealmente, solo se invocan rayos desde aquí. Se suele invocar
- **Closest hit shader**: este shader se ejecuta cuando un rayo impacta en una geometría por primera vez. Se pueden trazar rayos recursivamente desde aquí (por ejemplo, para calcular oclusión ambiental).
- **Any-hit shader**: similar al closest hit, pero invocado en cada intersección del camino del rayo que cumpla $t \in [t_{min}, t_{max})$. Es comúnmente utilizado en los cálculos de transparencias (*alpha-testing*).
- **Miss shader**: si el rayo no choca con ninguna geometría --pega con el infinito--, se ejecuta este shader. Normalmente, añade una pequeña contribución ambiental al rayo.
- **Intersection shader**: este shader es algo diferente al resto. Su función es calcular el punto de impacto de un rayo con una geometría. Por defecto se utiliza un test triángulo - rayo. En nuestro path tracer lo dejaremos por defecto, pero podríamos definir algún método como los que vimos en la sección [intersecciones rayo - objeto](#intersecciones-rayo---objeto).

Este es el código de los shaders del path tracer: [Raygen](https://github.com/Asmilex/Raytracing/blob/main/application/vulkan_ray_tracing/src/shaders/raytrace.rgen), [Closest hit](https://github.com/Asmilex/Raytracing/blob/main/application/vulkan_ray_tracing/src/shaders/raytrace.rchit), [Miss](https://github.com/Asmilex/Raytracing/blob/main/application/vulkan_ray_tracing/src/shaders/raytrace.rmiss), [Any-hit](https://github.com/Asmilex/Raytracing/blob/main/application/vulkan_ray_tracing/src/shaders/raytrace_rahit.glsl).

Existe otro tipo de shader adicional denominado **callable shader**. Este es un shader que se invoca desde otro shader. Por ejemplo, un shader de intersección puede invocar a un shader de oclusión. Otro ejemplo sería un closest hit que reemplaza un bloque if-else por un shader para hacer cálculos de iluminación. Este tipo de shaders no se han implementado en el path tracer, pero se podrían añadir con un poco de trabajo.

### Traspaso de información entre shaders

En ray tracing, los shaders por sí solos no pueden realizar todos los cálculos necesarios. Por ello, necesitaremos enviar información de uno a otro. Tenemos diferentes mecanismos para conseguirlo:

El primero de ellos son las **push constansts**. Estas son variables que se pueden enviar a los shaders (es decir, de CPU a GPU), pero que no se pueden modificar. Únicamente podemos mandar un pequeño número de variables, el cual se puede consultar mediante `VkPhysicalDeviceLimits.maxPushConstantSize`.

Nuestro path tracer tiene implementado actualmente (19 de abril de 2022) las siguientes constantes:

```cpp
struct PushConstantRay {
    vec4  clearColor;     // Color ambiental
    vec3  lightPosition;
    float lightIntensity;
    int   lightType;
    int   maxDepth;       // Cuántos rebotes máximos permitimos
    int   nb_samples;     // Para antialiasing
    int   frame;          // Para acumulación temporal
};
```


Las push constants son, como dice su nombre, constantes. ¿Y si queremos pasar información mutable entre shaders?.

Para eso están los **payloads**. Específicamente, cada rayo puede llevar información adicional. Como una pequeña mochila. Esto resulta *muy* útil, por ejemplo, a la hora de calcular la radiancia de un camino. Se crean mediante la estructura `rayPayloadEXT`, y se reciben en otro shader mediante `rayPayloadInEXT`. Es importante controlar que el tamaño de la carga no sea excesivamente grande.

### Creación de la ray tracing pipeline

El código de la creación de la pipeline lo encapsula la función `createRtPipeline()`, que se puede consultar [aquí](https://github.com/Asmilex/Raytracing/blob/6409feb628cc048186f6279b921ebe24e9337b6a/application/vulkan_ray_tracing/src/hello_vulkan.cpp#L763)

## Transporte de luz en la práctica

### Estimando la rendering equation con Monte Carlo

Hemos llegado a una de las partes más importantes de este trabajo. Es el momento de poner en concordancia todo lo que hemos visto a lo largo de los capítulos anteriores.

Empecemos por la dispersión. ¿Recuerdas la ecuación de dispersión [@eq:scattering_equation]?

$$
L_o(p, \omega_o \leftarrow \omega_i) = \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\cos\theta_i} d\omega_i
$$

Añadamos el término de radiancia emitida:

$$
L_o(p, \omega_o \leftarrow \omega_i) = L_e(p, \omega_o) + \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\cos\theta_i} d\omega_i
$$

Podemos aproximar el valor de la integral utilizando Monte Carlo:

$$
\begin{aligned}
L_o(p, \omega_o \leftarrow \omega_i) & = \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\cos\theta_i} d\omega_i \\
                 & \approx \frac{1}{N} \sum_{j = 1}^{N}{\frac{f(p, \omega_o \leftarrow \omega_j) L_i(p, \omega_j) \cos\theta_j}{p(\omega_j)}}
\end{aligned}
$${#eq:rendering_equation_mc}

Fijémonos en el denominador. Lo que estamos haciendo es tomar una muestra de un vector en la esfera. Si trabajamos con una BRDF en vez de una BSDF, usaríamos un hemisferio en vez de la esfera.

En el caso de la componente difusa, sabemos que la BRDF es $f_r(p, \omega_o \leftarrow \omega_i) = \frac{\rho}{\pi}$, así que

$$
\frac{1}{N} \sum_{j = 1}^{N}{\frac{(\rho / \pi) L_i(p, \omega_j) \cos\theta_j}{p(\omega_j)}}
$$

¿Recuerdas la sección de [importance sampling](#muestreo-por-importancia)? La idea es buscar una función proporcional a $f$ para reducir el error. Podemos usar $p(\omega) = \frac{\cos\theta}{\pi}$, de forma que

$$
\frac{1}{N} \sum_{j = 1}^{N}{\frac{(\rho / \pi) L_i(p, \omega_j) \cos\theta_j}{(\cos\theta_j / \pi)}} = \frac{1}{N} \sum_{j = 1}^{N}{L_i(p, \omega_j) \rho}
$${#eq:rendering_eq_lambertian}

Lo cual nos proporciona una expresión muy agradable para los materiales difusos. Solo es cuestión de calcular la radiancia del punto.

### Pseudocódigo de un path tracer

Con lo que conocemos hasta ahora, podemos empezar a programar los shaders. Una primera implementación basada en [@eq:rendering_equation_mc] sería similar a lo siguiente:

```cpp
vec3 pathtrace(Rayo r, profundidad) {
    if (profundidad == profundidad_maxima) {
        return contribucion_ambiental;
    }

    r.trazar_rayo()

    if (!r.ha_impactado()) {
        return contribucion_ambiental;
    }

    hit_info = r.hit_info
    material = hit_info.material
    emision = material.emision

    nuevo_rayo = Rayo(
        origen = hit_info.punto_impacto,
        direccion = random_direction(hit_info.normal)
    )

    cos_theta = dot(nuevo_rayo.direccion, hit_info.normal)
    BRDF = ... // BRDF específica del material
    pdf = ... // función de densidad que nos informa de la probabilidad de la nueva dirección.

    return emission + (BRDF * pathtrace(nuevo_rayo, profundidad + 1) * cos_theta) / pdf;
}
```
El término `emission` corresponde a $L_e(p, \omega_o)$. Siempre lo añadimos, pues en caso de que el objeto no emita luz, la contribución de este término sería 0.

La principal desventaja de esta implementación es que utiliza recursividad. Como bien es conocido, abusar de recursividad provoca que el tiempo de ejecución aumente significativamente. Además, con la implementación anterior, necesariamente se generan rayos desde el closest hit shader, lo cual no es ideal.

### Evitando la recursividad

Podemos evitar los problemas de la implementación anterior con una pequeña modificación. En vez de calcular la radiancia desde el closest hit, nos traemos la información necesaria al raygen shader, y calculamos la radiancia total desde allí.

Para conseguirlo, debemos hacer que el `HitPayload` almacene dos nuevos parámetros: `weight` y `hit_value`, así como el nuevo origen y la dirección del rayo.

El pseudocódigo sería el siguiente:

```cpp
// Raygen shader
vec3 pathtrace() {
    HitPayload prd;

    // Inicializar parámetros del primer rayo
    prd.hit_value  = vec3(0);
    prd.weight     = vec3(0);
    prd.ray_origin = r.origin;
    prd.ray_dir    = r.dir;

    vec3 current_weight = vec3(1);
    vec3 hit_value      = vec3(0);

    for (profundidad in [0, profunidad_maxima]) {
        closest_hit(prd.ray_origin, prd.ray_dir);
        // La llamada del rayo almacena nueva información en el HitPayload prd, que podremos usar ahora

        hit_value = hit_value + prd.hit_value * current_weight;
        current_weight = current_weight * prd.weight;
    }

    return hit_value;
}

// Closest hit shader
vec3 closest_hit() {
    // Sacar información sobre el punto de impacto: material, normal...
    vec3 normal = ... // normal en punto
    Material m = ... // Material de la superficie impactada.

    // Preparar información para el raygen
    prd.ray_origin = ... // Punto de impacto.
    prd.ray_dir = direccion_aleatoria() // ¡Dependiente del material!

    float cos_theta = dot(prd.ray_dir, normal);
    vec3 BRDF = ... // BRDF del material
    vec3 pdf  = ... // Probabilidad de que se escoja la dirección prd.ray_dir

    prd.hit_value = m.emision // Radiancia generada por el propio material; e.g. luces. Es el término L_e
    prd.weight = (BRDF * cos_theta) / pdf

    // La devolución de prd es implícita.
}
```

Esta versión no es tan intuitiva, así que ejemplifiquemos lo está ocurriendo.

Sea $h$ el hit_value, $w$ el peso, $f_i$ la BRDF $i$, $e_i$ la emisión, $\cos\theta_i$ el coseno del ángulo que forman la nueva dirección del rayo y la normal, y $p_i$ la función de densidad que, dada una dirección, proporciona la probabilidad de que se escoja. El subíndice denota el $i$-ésimo punto de impacto.

En esencia, estamos descomponiendo lo que recogemos en `weight`, que es $f_i \cos\theta_i / p_i$

Inicialmente, para el primer envío del rayo, $h = (0, 0, 0)$, $w = (1, 1, 1)$. Tras trazar el primer rayo, se tiene que

$$
\begin{aligned}
    h & = 0 + e_1 w = e_1 \\
    w & = 1 \frac{f_1 \cos\theta_1}{p_1}
\end{aligned}
$$

Tras el segundo rayo, obtenemos

$$
\begin{aligned}
    h & = e_1 + e_2 w = \\
      & = e_1 + e_2 \frac{f_1 \cos\theta_1}{p_1} \\
    w & = \frac{f_1 \cos\theta_1}{p_1} \frac{f_2 \cos\theta_2}{p_2}
\end{aligned}
$$

Y para el tercero

$$
\begin{aligned}
    h & = e_1 + e_2 \frac{f_1 \cos\theta_1}{p_1} + e_3 w = \\
      & = e_1 + e_2 \frac{f_1 \cos\theta_1}{p_1} + e_3 \frac{f_1 \cos\theta_1}{p_1} \frac{f_2 \cos\theta_2}{p_2} = \\
      & = e_1 + \frac{f_1 \cos\theta_1}{p_1}\textcolor{verde-oscurisimo}{\left(e_2 + e_3 \frac{f_2 \cos\theta_2}{p_2}\right)} \\
    w & = \frac{f_1 \cos\theta_1}{p_1} \frac{f_2 \cos\theta_2}{p_2} \frac{f_3 \cos\theta_3}{p_3}
\end{aligned}
$$

El $\textcolor{verde-oscurisimo}{\text{término que acompaña}}$ a $\frac{f_1 \cos\theta_1}{p_1}$ es la radiancia del tercer punto de impacto. Por tanto, a la larga, se tendrá que $h$ estima correctamente la radiancia de un punto. Con esto, podemos afirmar que

$$
h \approx \frac{1}{N} \sum_{j = 1}^{N}{\frac{f(p, \omega_o \leftarrow \omega_j) L_i(p, \omega_j) \cos\theta_j}{p(\omega_j)}}
$$

Este algoritmo supone una mejora de hasta 3 veces mayor rendimiento que el recursivo.

### Antialiasing mediante jittering y acumulación temporal

Normalmente, mandamos los rayos desde el centro de un pixel. Podemos conseguir una mejora sustancial de la calidad con un pequeño truco: en vez de generarlos siempre desde el mismo sitio, le aplicamos una pequeña perturbación (*jittering*). Así, tendremos diferentes colores para un mismo pixel, por lo que podemos hacer una ponderación del color que se obtiene (a lo que llamamos *acumulación temporal*).

Es importante destacar que el efecto de esta técnica solo es válido cuando la **cámara se queda estática**.

La implementación es muy sencilla. Debemos modificar tanto el motor como los shaders para llevar una cuenta de ciertos frames, definiendo un máximo de frames que se pueden acumular:

```cpp
// engine.h
class Engine {
    //...
    int m_maxAcumFrames {20};
}
```

Las push constant deberán llevar un registro del frame en el que se encuentran, así como un número máximo de muestras a acumular para un pixel:


```cpp
// host_device.h
struct PushConstantRay {
    //...
    int   frame;
    int   nb_samples
}
```

El número de frame se reseteará cuando la cámara se mueva, la ventana se reescale, o se produzca algún efecto similar en la aplicación.

Finalmente, en los shaders podemos implementar lo siguiente:

```glsl
// raytrace.rgen
vec3 pixel_color = vec3(0);

for (int smpl = 0; smpl < pcRay.nb_samples; smpl++) {
    pixel_color += sample_pixel(image_coords, image_res);
}

pixel_color = pixel_color / pcRay.nb_samples;

if (!primer_frame) {
    guardar una mezcla de las anteriores imágenes junto con la actual
}
else {
    guardar la imagen directamente
}

```

```glsl
// pathtrace.glsl
vec3 sample_pixel() {
    float r1 = rnd(prd.seed);
    float r2 = rnd(prd.seed);

    // Subpixel jitter: mandar el rayo desde una pequeña perturbación del pixel para aplicar antialiasing
    vec2 subpixel_jitter = pcRay.frame == 0
        ? vec2(0.5f, 0.5f)
        : vec2(r1, r2);

    const vec2 pixelCenter = vec2(image_coords.xy) + subpixel_jitter;

    // ...

    vec3 radiance = pathtrace(rayo);
}
```


### Materiales y objetos

El sistema de materiales y objetos usados es el **Wavefront** (`.obj`). Aunque es un sistema relativamente antiguo y sencillo, se han usado definiciones específicas en los materiales para adaptarlo a Physically Based Rendering. Entre los parámetros del archivo de materiales `.mtl`, destacan:

- $K_a \in [0, 1]^3$: representa el color ambiental. Dado que esto es un path tracer físicamente realista, no se usará.
- $K_d \in [0, 1]^3$: componente difusa.
- $K_s \in [0, 1]^3$: componente especular. Viene acompañada del exponente especular $N_s \in [0, 1000]$. Usualmente, $N_s = 10$. Controla los brillos en los modelos de Blinn-Phong.
- $d \in [0, 1]$ (*dissolve*): representa la transparencia. Alternativamente, se usa $T_r = 1 - d$.
- $T_f \in [0, 1]^3$: filtro de transmisión.
- $N_i \in [0.001, 10]$: índice de refracción. Usualmente $N_i = 1$.
- $K_e \in [0, 1]^3$: componente emisiva (PBR).
- Todos los valores con tres componentes pueden presentar un *texture map*.

Existe un parámetro adicional llamado `illum`. Controla el modelo de iluminación usado. Nosotros lo usaremos para distinguir tipos diferentes de materiales. Los códigos representan lo siguiente:

| **Modelo** | **Color**                    | **Reflejos**         | **Transparencias** |
|:-----------|------------------------------|----------------------|--------------------|
| `0`        | Difusa                       | No                   | No                 |
| `1`        | Difusa, ambiental            | No                   | No                 |
| `2`        | Difusa, especular, ambiental | No                   | No                 |
| `3`        | Difusa, especular, ambiental | Ray traced           | No                 |
| `4`        | Difusa, especular, ambiental | Ray traced           | Cristal            |
| `5`        | Difusa, especular, ambiental | Ray traced (Fresnel) | No                 |
| `6`        | Difusa, especular, ambiental | Ray traced           | Refracción         |
| `7`        | Difusa, especular, ambiental | Ray traced (Fresnel) | Refracción         |
| `8`        | Difusa, especular, ambiental | Sí                   | No                 |
| `9`        | Difusa, especular, ambiental | Sí                   | Cristal            |
| `10`       |  Sombras arrojadizas                                                     |

```c++
// host_device.h
struct WaveFrontMaterial
{
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  vec3  transmittance;
  vec3  emission;
  float shininess;
  float ior;       // index of refraction
  float dissolve;  // 1 == opaque; 0 == fully transparent
  int   illum;     // illumination model (see http://www.fileformat.info/format/material/)
  int   textureId;
};
```

## Fuentes de luz

> TODO: point lights, area lights, ambient lights...
>
> TODO: estas son notas muy puntuales (*como las luces, jeje*). Ya las revisaré más adelante.
>
> TODO: 01_lights.pdf tiene información útil sobre muestreo directo de fuents de luces.

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
- Ray tracing gems II, p.241.
- https://www.willusher.io/graphics/2019/11/20/the-sbt-three-ways


[^4]: Esto no es del todo cierto. Aunque generalmente suelen ser excepciones debido al coste computacional de RT en tiempo real, existen algunas implementaciones que son capaces de correrlo por software. Notablemente, el motor de Crytek, CryEngine, es capaz de mover ray tracing basado en hardware y en software [@crytek-2020]
[^5]: Afortunadamente, esto tampoco es completamente cierto. La compañía desarrolladora y distribuidora de videojuegos Valve Corporation ha creado una pieza de software fascinante: [Proton](https://github.com/ValveSoftware/Proton). Proton utiliza Wine para emular software en Linux que solo puede correr en plataformas Windows. La versión 2.5 añadió soporte para traducción de bindings de DXR a KHR, lo que permite utilizar DirectX12 ray tracing en sistemas basados en Linux. El motivo de este software es expandir el mercado de videojuegos disponibles en su consola, la Steam Deck.