# ¡Construyamos un path tracer!

Ahora que hemos introducido toda la teoría necesaria, es hora de ponernos manos a la obra. En este capítulo escogeremos una serie de herramientas y con ellas implementaremos un pequeño motor de path tracing en tiempo real.

La implementación estará basada en Vulkan, junto al pequeño *framework* de *nvpro-samples*, que puede encontrarse en el repositorio [@nvpro-samples-tutorial]. Nuestro trabajo recogerá varias de las características que se muestran en dicho repositorio. Además, mantendremos el mismo espíritu que la serie de [@Shirley2020RTW1], Ray Tracing In One Weekend.

El resultado final puede verse en el siguiente vídeo [@video]

<iframe width="784" height="441" src="https://www.youtube.com/embed/pXrD3K69MqE" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## El algoritmo de path tracing

Hemos llegado a una de las partes más importantes de este trabajo. Es el momento de poner en concordancia todo lo que hemos visto a lo largo de los capítulos anteriores. Vamos a aplicar las [técnicas de Monte Carlo](#métodos-de-monte-carlo) a las ecuaciones vistas en [radiometría](#transporte-de-luz), teniendo en cuenta las propiedades de los diferentes materiales.

El código ilustrado en las siguientes secciones está basado en el de [@nvpro-samples-tutorial], aunque se pueden encontrar numerosísimas modificaciones en la literatura del sector.

### Estimando la rendering equation con Monte Carlo

Lo que buscamos en esta sección es aproximar el valor de la radiancia en un cierto punto, que dependerá de cada píxel dela pantalla. ¿Recuerdas la ecuación de dispersión [@eq:scattering_equation]?

$$
L_o(p, \omega_o \leftarrow \omega_i) = \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\abs{\cos\theta_i}\  d\omega_i}
$$

Recordemos que $L_o(p, \omega_o \leftarrow \omega_i)$ es la radiancia reflejada en un punto $p$ hacia la dirección $\omega_o$ desde $\omega_i$, $f(p, \omega_o \leftarrow \omega_i)$ es la función de distribución de dispersión bidireccional (i.e., cómo refleja la luz el punto) y $\cos\theta_i$ el ángulo que forman el ángulo sólido de entrada $\omega_i$ y la normal en el punto $p$, $\mathbf{n}$: $\abs{\cos\theta_i} = \abs{\omega_i \cdot \mathbf{n}}$.

Añadamos el término de radiancia emitida $L_e(p, \omega_o)$, la cantidad de randiancia emitida por el material del punto $p$:

$$
L(p, \omega_o) = L_e(p, \omega_o) + \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\cos\theta_i} d\omega_i
$$

Este valor $L(p, \omega_o)$ es la radiancia total saliente (es decir, emitida más reflejada) desde el punto $p$ hacia la dirección $\omega_o$.

Podemos aproximar el valor de la integral utilizando el estimador de Monte Carlo comúnmente considerado en la [industria](#muestreo-por-importancia-múltiple-en-transporte-de-luz), [@eq:mc_integral_importancia]:

$$
\begin{aligned}
L_o(p, \omega_o \leftarrow \omega_i) & = \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\cos\theta_i} d\omega_i \\
                 & \approx \frac{1}{N} \sum_{j = 1}^{N}{\frac{f(p, \omega_o \leftarrow \omega_j) L_i(p, \omega_j) \cos\theta_j}{\Prob{\omega_j}}}
\end{aligned}
$${#eq:rendering_equation_mc}

Con $N \in \mathbb{Z}^+$, y $\Prob{\omega}$ la función de densidad de $\omega$. Con $N$ suficientemente grande, se conseguiría un valor de radiancia relativamente acertado. Sin embargo, en algunos casos, podemos simplificar más el sumando.

Fijémonos en el denominador. Lo que estamos haciendo es tomar una muestra de un vector en la esfera. Si trabajamos con una BRDF en vez de una BSDF, usaríamos un hemisferio en vez de la esfera.

En el caso de la componente difusa, sabemos que la BRDF es $f_r(p, \omega_o \leftarrow \omega_i) = \frac{\rho}{\pi}$ aplicando reflectancia lambertiana, así que

$$
\frac{1}{N} \sum_{j = 1}^{N}{\frac{(\rho / \pi) L_i(p, \omega_j) \cos\theta_j}{\Prob{\omega_j}}}
$$

En la sección [muestreo por importancia](#muestreo-por-importancia), introducimos la idea de buscar una función proporcional a $f$ para con el fin de reducir el error. Podemos usar $\Prob{\omega} = \frac{\cos\theta}{\pi}$, de forma que

$$
\frac{1}{N} \sum_{j = 1}^{N}{\frac{(\rho / \pi) L_i(p, \omega_j) \cos\theta_j}{(\cos\theta_j / \pi)}} = \frac{1}{N} \sum_{j = 1}^{N}{L_i(p, \omega_j) \rho}
$${#eq:rendering_eq_lambertian}

Lo cual nos proporciona una expresión muy agradable para los materiales difusos.

Por lo general, no será posible simplificar hasta tal punto la expresión. Para BRDFs distintas de la difusa, algunas veces no podremos encontrar una forma de hacer muestreo por importancia de forma tan simple, en la cual la función de densidad sea proporcional a la BRDF.

### Pseudocódigo de un path tracer

Con lo que conocemos hasta ahora, podemos empezar a programar los shaders. Una primera implementación inspirada en la rendering equation [@eq:rendering_equation_mc] sería similar a lo siguiente:

```{=latex}
\begin{algorithm}[H]
\DontPrintSemicolon
\SetAlgoLined
\KwResult{Radiancia total del camino}
\SetKwInOut{Input}{Input}
\SetKwInOut{Output}{Output}
\Input{punto de origen $\mathbf{p}$, dirección del rayo $\omega$, profundidad del camino $profundidad$}
\Output{$L$}
\BlankLine
$emision = L_e(\mathbf{p}, \omega, profundidad)$\;
\If{$profundidad \le profundidad\_maxima$} {
    $\mathbf{u} =$ dirección aleatoria hacia el siguiente punto\;
    $prob = pdf(\mathbf{p}, \mathbf{u}, \omega)$ \tcp*{probabilidad de escoger la dirección $u$}
    $\mathbf{y} = intersecar(\mathbf{p}, \mathbf{u})$ \tcp*{Encontrar la geometría más cercana en dirección $u$}
    $\theta = \mathbf{n}_\mathbf{p} \cdot \mathbf{u}$\;
    \BlankLine
    $L = emision + L(\mathbf{y}, -\mathbf{u}, profundidad + 1) f_r(\mathbf{p}, \mathbf{u}, \omega) \cos\theta / prob$\;
}
\caption{$L(\mathbf{p}, \omega, profundidad)$}
\end{algorithm}
```

```{=html}
<pre class="language-cpp" tabindex="0"><code class="language-cpp"><span class="token function">L</span><span class="token punctuation">(</span>p<span class="token punctuation">,</span> omega<span class="token punctuation">,</span> profundidad<span class="token punctuation">)</span> <span class="token punctuation">{</span>
    emision <span class="token operator">=</span> <span class="token function">L_e</span><span class="token punctuation">(</span>p<span class="token punctuation">,</span> omega<span class="token punctuation">,</span> profundidad<span class="token punctuation">)</span>

    <span class="token keyword">if</span> <span class="token punctuation">(</span>profundidad <span class="token operator">&lt;=</span> profundidad_maxima<span class="token punctuation">)</span> <span class="token punctuation">{</span>
        u <span class="token operator">=</span> <span class="token function">direccion_aleatoria</span><span class="token punctuation">(</span><span class="token punctuation">)</span>
        prob <span class="token operator">=</span> <span class="token function">pdf</span><span class="token punctuation">(</span>p<span class="token punctuation">,</span> u<span class="token punctuation">,</span> omega<span class="token punctuation">)</span>   <span class="token comment">// probabilidad de escoger la dir. u</span>
        y <span class="token operator">=</span> <span class="token function">intersecar</span><span class="token punctuation">(</span>p<span class="token punctuation">,</span> u<span class="token punctuation">)</span>
        theta <span class="token operator">=</span> <span class="token function">dot</span><span class="token punctuation">(</span><span class="token function">normal</span><span class="token punctuation">(</span>p<span class="token punctuation">)</span><span class="token punctuation">,</span> u<span class="token punctuation">)</span>
        L <span class="token operator">=</span>   emision
            <span class="token operator">+</span> <span class="token function">L</span><span class="token punctuation">(</span>y<span class="token punctuation">,</span> <span class="token operator">-</span>u<span class="token punctuation">,</span> profundidad <span class="token operator">+</span> <span class="token number">1</span><span class="token punctuation">)</span> <span class="token operator">*</span> <span class="token function">f_r</span><span class="token punctuation">(</span>p<span class="token punctuation">,</span> u<span class="token punctuation">,</span> omega<span class="token punctuation">)</span> <span class="token operator">*</span> <span class="token function">cos</span><span class="token punctuation">(</span>theta<span class="token punctuation">)</span> <span class="token operator">/</span> prob
    <span class="token punctuation">}</span>

    <span class="token keyword">return</span> L
<span class="token punctuation">}</span></code></pre>
```

El término `emision` corresponde a $L_e(p, \omega_o)$. Siempre lo añadimos, pues en caso de que el objeto no emita luz, la contribución de este término sería 0.

La principal desventaja de esta implementación es que utiliza recursividad. Como bien es conocido, abusar de recursividad provoca que el tiempo de ejecución aumente significativamente.

## Requisitos de ray tracing en tiempo real

Como es natural, el tiempo es una limitación enorme para cualquier programa en tiempo real. Mientras que en un *offline renderer* disponemos de un tiempo muy considerable por *frame* (desde varios segundos hasta horas), en un programa en tiempo real necesitamos que un *frame* salga en 33.3 milisegundos o menos. Este concepto se suele denominar *frame budget*: la cantidad de tiempo que disponemos para un *frame*.

> **Nota**: cuando hablamos del tiempo disponible para un *frame*, solemos utilizar milisegundos (ms) o *frames* por segundo (FPS). Para que un programa en tiempo real vaya suficientemente fluido, necesitaremos que el motor corra a un mínimo de 30 FPS (que equivalen a 33.3 ms/*frame*). Hoy en día, debido al avance del área en campos como los videosjuegos, el estándar se está convirtiendo en 60 FPS (16.6 ms/*frame*). Aún más, en los videojuegos competitivos profesionales se han asentado los 240 FPS (4.1 ms/*frame*).

Las nociones de los capítulos anteriores no distinguen entre un motor en tiempo real y *offline*. Como es natural, necesitaremos introducir unos pocos conceptos más para llevarlo a tiempo real. Además, existen una serie de requisitos hardware que debemos cumplir para que un motor en tiempo real con ray tracing funcione.

### Arquitecturas de gráficas

El requisito más importante de todos es la gráfica. Para ser capaces de realizar cálculos de ray tracing en tiempo real en escenarios complejos, necesitaremos una arquitectura moderna con núcleos dedicados a este tipo de cáclulos [^5]. Aunque es posible implementar ray tracing en gráficas convencionales, el rendimiento no será lo suficientemente bueno como para adecuarse al nivel de fidelidad de los tiempos actuales.

A día 17 de abril de 2022, para correr ray tracing en tiempo real, se necesita alguna de las siguientes tarjetas gráficas:

| **Arquitectura**              | **Fabricante** |                                       **Modelos de gráficas**                                        |
|:------------------------------|:---------------|:----------------------------------------------------------------------------------------------------:|
| **Turing**                    | Nvidia         | RTX 2060, RTX 2060 Super, RTX 2070, RTX 2070 Super, RTX 2080, RTX 2080 Super, RTX 2080 Ti, RTX Titan |
| **Ampere**                    | Nvidia         | RTX 3050, RTX 3060, RTX 3060 Ti, RTX 3070, RTX 3070 Ti, RTX 3080, RTX 3080 Ti, RTX 3090, RTX 3090 Ti |
| **RDNA2** (Navi 2X, Big Navi) | AMD            |        RX 6400, RX 6500 XT, RX 6600, RX 6600 XT, RX 6700 XT, RX 6800, RX 6800 XT, RX 6900 XT         |
| **Arc Alchemist**             | Intel          |                                          *No reveleado aún*                                          |

Table: Arquitecturas capaces de realizar cómputos de ray tracing acelerados por hardware. {#tbl:arquitecturas}

Se puede encontrar más información sobre las diferentes arquitecturas y gráficas en el siguiente artículo de AMD Radeon [@RDNA2], Nvidia [@turing-arquitecture, @ampere-arquitecture], e [@intel-arc]. Solo se han incluido las gráficas de escritorio de consumidor.

Para este trabajo se ha utilizado una **RTX 2070 Super**. En el capítulo de análisis del rendimiento se hablará con mayor profundidad de este apartado.

### Frameworks y API de ray tracing en tiempo real

Una vez hemos cumplido los requisitos de hardware, es hora de escoger los *frameworks* de trabajo.

Las API de gráficos están empezando a adaptarse a los requisitos del tiempo real, por lo que cambian frecuentemente. La mayoría adquirieron las directivas necesarias muy recientemente. Aun así, son lo suficientemente sólidas para que se pueda usar en aplicaciones empresariales de gran embergadura.

Esta es una lista de las API disponibles con capacidades de Ray Tracing disponibles para, al menos, la arquitectura Turing:

- **Vulkan**, junto a los *bindings* de ray tracing, denominados KHR.
- Microsoft DirectX Ray Tracing (DXR), una extensión de **DirectX 12** [@directx-12].
- Nvidia **OptiX** [@optix].

De momento, no hay mucho donde elegir.

OptiX es la API más vieja de todas. Su primera versión salió en 2009, mientras que la última estable es de 2021. Tradicionalmente se ha usado para offline renderers, y no tiene un especial interés para este trabajo estando las otras dos disponibles.

Tanto DXR como Vulkan son los candidatos más sólidos. DXR salió en 2018, con la llegada de Turing. Es un par de años más reciente que Vulkan KHR. Cualquiera de las dos cumpliría su cometido de forma exitosa. Sin embargo, para este trabajo, **hemos escogido Vulkan** por los siguientes motivos:

- DirectX 12 está destinado principalmente a plataformas de Microsoft. Es decir, está pensado para sistemas operativos Windows 10 o mayor [^6].
- Vulkan, al estar apoyado principalmente por AMD y desarrollado por Khonos, es un proyecto de código. Su principal aliciente es la capacidad de correr en múltiples sistemas operativos, como Windows, distribuciones de Linux o Android.

Ambas API se comportan de manera muy similar, y no existe una gran diferencia entre ellas; tanto en rendimiento como en complejidad de desarrollo. Actualmente el proyecto solo compila en Windows 10 o mayor, por lo que estos dos puntos no resultan especialmente relevantes para el trabajo.

Si se desea, se puede encontrar una comparación más a fondo de las API en el blog de [@alain-API]. Además, el manual de Vulkan con las extensiones de KHR se puede encontrar en [@vulkan].

## Setup del proyecto

Un proyecto de Vulkan necesita una cantidad de código inicial considerable. Para acelerar este trámite y partir de una base más sólida, se ha decidido usar un pequeño *framework* de trabajo de Nvidia llamado [nvpro-samples] [@nvpro-samples].

Esta serie de repositorios de Nvidia DesignWorks contienen proyectos de ray tracing de Nvidia con fines didácticos. Nosotros usaremos **vk_raytracing_tutorial_KHR** [@nvpro-samples-tutorial], pues ejemplifica cómo añadir ray tracing en tiempo real a un proyecto de Vulkan. En particular, nosotros seguiremos las siguientes secciones, pero extendiendo el resultado final:

- El tutorial base.
- Any hit shader.
- Jitter Camera.
- Reflections.
- glTF Scene altamente modificado. A su vez, es una simplificación del repositorio `vk_raytrace`.

Estos *frameworks* contienen asimismo otras utilidades menores. Destacan **GLFW** (gestión de ventanas en C++), **imgui** (interfaz de usuario) y **tinyobjloader** (carga de `.obj` y `.mtl`).

Nuestro repositorio utiliza las herramientas citadas anteriormente para compilar su proyecto. El Makefile es una modificación del que se usa para ejecutar los ejemplos de Nvidia. Por defecto, ejecuta una aplicación muy simple que muestra un cubo mediante rasterización, la cual modificaremos hasta añadir ray tracing en tiempo real. Por tanto, la parte inicial del desarrollo consiste en adaptar Vulkan para usar la extensión de ray tracing, extrayendo la información de la gráfica y cargando correspondientemente el dispositivo.

![Por defecto, el programa muestra un cubo rasterizado muy simple. Es, prácticamente, un *hello world* gráfico](./img/04/Raster.jpg){#fig:raster width=70%}

### Un vistazo general a la estructura

La estructura final de las carpetas del repositorio con el código fuente del proyecto (es decir, la carpeta `application`) es la siguiente:

- La carpeta `application/build` contiene todo lo relacionado con CMake y el ejecutable final.
- Las dependencias del proyecto se encuentran en el repositorio `application/nvpro_core`. Se descargan automáticamente seguir las instrucciones de compilación.
- En `application/vulkan_ray_tracing/media/` se encuentran todos los archivos `.obj`, `.mtl` y las texturas.
- La subcarpeta `application/vulkan_ray_tracing/src` contiene el código fuente de la propia aplicación.
  - Toda la implementación relacionada con el motor (y por tanto, Vulkan), se halla en `engine.h/cpp`. Una de las desventajas de seguir un *framework* "de juguete" es que el acoplamiento es considerablemente alto. Más adelante comentaremos los motivos.
  - Los parámetros de la aplicación (como tamaño de pantalla y otras estructuras comunes) se encuetran en `globals.hpp`.
  - La carga de escenas y los objetos se gestionan en `scene.hpp`.
  - En `main.cpp` se gestiona tanto el punto de entrada de la aplicación como la actualización de la interfaz gráfica.
  - La carpeta `application/vulkan_ray_tracing/src/shaders` contiene todos los shaders; tanto de rasterización, como de ray tracing.
    - Para ray tracing, se utilizan los `raytrace.*`, `pathtrace.glsl` (que contiene el grueso del path tracer).
    - En rasterización se usan principalmente `frag_shader.frag`, `passthrough.vert`, `post.frag`, `vert_shader.vert`.
    - El resto de shaders son archivos comunes a ambos o utilidades varias, como pueden ser `sampling.glsl` (donde se implementan distribuciones aleatorias) o `random.glsl` (que contiene generadores de números aleatorios).
  - Finalmente, la carpeta `application/vulkan_ray_tracing/src/spv` contiene los shaders compilados a SPIR-V.

El diagrama {@fig:estructura_repo} permite visualizar los puntos anteriores, así como la estructura general del repositorio.

![Estructura del repositorio](./img/04/repo_diagram.png){#fig:estructura_repo width=90%}

### Diagramas

Teniendo en cuenta que utilizamos un *framework* que no está pensado para producción y la naturaleza de Vulkan, realizar un diagrama de clase es muy complicado. Sin embargo, podemos ilustrar las clases más importantes de la aplicación: la el motor [@fig:diagrama-clases-engine] y la de escenas [@fig:diagrama-clases-scenes] . En las secciones posteriores detallaremos algunos de los miembros de estas.

Una figura que se asemeja a un diagrama de secuencia específico para el loop de ray tracing puede encontrarse en [@fig:pipeline].

![Diagrama de clases para Engine](./img/08/Mermaid%20engine.png){#fig:diagrama-clases-engine}

![Diagrama de clases para Scenes](./img/08/Mermaid%20scenes.png){#fig:diagrama-clases-scenes width=60%}

## Compilación y ejecución

Las dependencias necesarias son:

1. **CMake**.
2. Un **driver de Nvidia** compatible con la extensión `VK_KHR_ray_tracing_pipeline`.
3. El SDK de **Vulkan**, versión 1.2.161 o mayor.

Ejecuta los siguientes comandos desde la terminal para compilar el proyecto:

```sh
git clone --recursive --shallow-submodules https://github.com/Asmilex/Raytracing.git
cd .\Raytracing\application\vulkan_ray_tracing\
mkdir build
cd build
cmake ..
cmake --build .
```

Si todo funciona correctamente, debería generarse un binario en `./application/bin_x64/Debug` llamado `asmiray.exe`. Desde la carpeta en la que deberías encontrarte tras seguir las instrucciones, puedes conseguir ejecutarlo con

```sh
..\..\bin_x64\Debug\asmiray.exe
```

## Estructuras de aceleración

El principal coste de ray tracing es el cálculo de las intersecciones con objetos; hasta un 95% del tiempo de ejecución total [@scratchapixel-2019]. Reducir el número de test de intersección es clave.

Las **estructuras de aceleración** son una forma de representar la geometría de la escena. Aunque existen diferentes tipos, en esencia, todos engloban a uno o varios objetos en una estructura con la que resulta más eficiente hacer test de intersección en términos del tiempo. Fueron introducidos por primera vez en [@kajiya-bb].

Uno de los tipos más comunes (y el que se usa en [@Shirley2020RTW2]) es la **Bounding Volume Hierarchy (BVH)**. Fue una técnica desarrollada por [@kajiya-bb]. Este método encierra un objeto en una caja (denomina una **bounding box**), de forma que el test de intersección principal se hace con la caja y no con la geometría. Si un rayo impacta en la *bounding box*, entonces se pasa a testear la geometría.

Se puede repetir esta idea repetidamente, de forma que agrupemos varias *bounding boxes*. Así, creamos una jerarquía de objetos --como si nodos de un árbol se trataran--. A esta jerarquía es a la que llamamos BVH.

Es importante crear buenas divisiones de los objetos en la BVH. Cuanto más compacta sea una BVH, más eficiente será el test de intersección.

Una forma habitual de crear la BVH es mediante la división del espacio en una rejilla. Esta técnica se llama **Axis-Aligned Bounding Box (AABB)**. Usualmente se usa el método del *slab* (también introducido por Kay y Kajilla). Se divide el espacio en una caja n-dimensional alineada con los ejes, de forma que podemos verla como $[x_0, x_1] \times$ $[y_0, y_1] \times$ $[z_0, z_1] \times \dots$ De esta forma, comprobar si un rayo impacta en una bounding box es tan sencillo como comprobar que está dentro del intervalo. Este es el método que se ha usado en Ray Tracing in One Weekend.

Vulkan gestiona las estructuras de aceleración diviéndolas en dos partes: **Top-Level Acceleration Structure** (TLAS) y **Bottom-Level Acceleration Structure** (BLAS).

![La TLAS guarda información de las instancias de un objeto, así como una referencia a BLAS que contiene la geometría correspondiente. Fuente: [@nvpro-samples-tutorial]](./img/04/Acceleration%20structure.png){#fig:TLAS width=70%}

### Botom-Level Acceleration Structure (BLAS)

Las **estructuras de aceleración de bajo nivel** (*Bottom-Level Acceleration Structure*, BLAS) almacenan la geometría de un objeto individual; esto es, los datos de los vértices y los índices de los triángulos, además de una AABB que la encapsula.

Pueden almacenar varios modelos, puesto que alojan uno o más buffers de vértices junto a sus matrices de transformación. Si un modelo es instanciado varias veces *dentro de la misma BLAS*, la geometría se duplica. Esto se hace para mejorar el rendimiento.

Como regla general, cuantas menos BLAS, mejor [@nvidia-best-practices].

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

Un ***resource descriptor*** (usualmente lo abreviaremos como descriptor) es una forma de cargar recursos como buffers o imágenes para que la tarjeta gráfica los pueda utilizar; concretamente, los shaders. El ***descriptor layout*** especifica el tipo de recurso que va a ser accedido, mientras que el ***descriptor set*** determina el buffer o imagen que se va a asociar al descriptor. Este set es el que se utiliza en los **drawing commands**. Un **pipeline** es una secuencia de operaciones que reciben una geometría y sus texturas, y la transforma en unos pixels.

Si necesitas más información, todos estos conceptos aparecen desarrollados extensamente en [@overvoorde-2022, Descriptor layout and buffer].

Tradicionalmente, en rasterización se utiliza un descriptor set por tipo de material, y consecuentemente, un pipeline por cada tipo. En ray tracing esto no es posible, puesto que **no se sabe qué material** se va a usar: un rayo puede impactar en *cualquier* material presente en la escena, lo cual invocaría un shader específico. Debido a esto, empaquetaremos todos los recursos en un único set de descriptores.

### Tipos de shaders

El pipeline de ray tracing soporta varios tipos de shaders diferentes que cubren la funcionalidad esencial de un ray tracer:

- **Ray generation shader**: es el punto de inicio del viaje de un rayo. Calcula punto de inicio y procesa el resultado final. Idealmente, la llamada a la función `traceRayEXT()`, la cual se encarga de generar un nuevo rayo, solo ocurre desde este shader. La implementación se encuentra en `application/vulkan_ray_tracing/src/shaders/raytrace.rgen`.
- **Closest hit shader**: se ejecuta en la primera intersección con alguna geometría válida de la escena. Se pueden trazar rayos recursivamente desde aquí (por ejemplo, para calcular oclusión ambiental). El archivo correspondiente es `application/vulkan_ray_tracing/src/shaders/raytrace.rchit`.
- **Any-hit shader**: similar al closest hit, pero invocado en cada intersección del camino del rayo que cumpla $t \in [t_{min}, t_{max})$. Es comúnmente utilizado en los cálculos de transparencias (*alpha-testing*). Puedes comprobarlo en `application/vulkan_ray_tracing/src/shaders/raytrace_rahit.glsl`.
- **Miss shader**: si el rayo no choca con ninguna geometría, se ejecuta este shader. Normalmente, añade una pequeña contribución ambiental al rayo. Se halla en `application/vulkan_ray_tracing/src/shaders/raytrace.rmiss`.
- **Intersection shader**: este shader es algo diferente al resto. Su función es calcular el punto de impacto de un rayo con una geometría. Por defecto se utiliza un test triángulo-rayo. En nuestro path tracer lo dejaremos por defecto, pero podríamos definir algún método como los que vimos en la sección [intersecciones rayo-objeto](#intersecciones-rayo---objeto).

Existe otro tipo de shader adicional denominado **callable shader**. Este es un shader que se invoca desde otro shader, a modo de subrutina. Por ejemplo, un shader de intersección puede invocar a un shader de oclusión. Otro ejemplo sería un closest hit que reemplaza un bloque if-else por un shader para hacer cálculos de iluminación. Este tipo de shaders no se han implementado en el path tracer, pero se podrían añadir con un poco de trabajo.

### Traspaso de información entre shaders

En ray tracing, los shaders por sí solos no pueden realizar todos los cálculos necesarios para conseguir la imagen final. Necesitaremos enviar información externa a estos. Para conseguirlo tenemos diferentes mecanismos:

El primero de ellos son las **push constants**. Estas son variables que se pueden traspasar a los shaders (es decir, de CPU a GPU), pero que no se pueden modificar entre fases. Únicamente podemos mandar un pequeño número de variables, el cual se puede consultar mediante `VkPhysicalDeviceLimits.maxPushConstantSize`. Además, es importante tener en cuenta el alineamiento de las estructuras almacenadas.

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

¿Y si queremos pasar información mutable entre shaders?

Para eso están los **payloads**. Cada rayo puede llevar información adicional, que se conoce como carga. En esencia, es como una pequeña mochila: el rayo puede recoger información de un shader y pasarlo a otro. Esto resulta *muy* útil, por ejemplo, a la hora de calcular la radiancia de un camino, o saber desde qué punto venía el rayo. Se crean mediante la estructura `rayPayloadEXT`, y se reciben en otro shader mediante `rayPayloadInEXT`. Es importante controlar que el tamaño de la carga no sea excesivamente grande.

Los payloads se definen de la siguiente manera:

```cpp
struct HitPayload
{
    vec3 hit_value;
    vec3 weight;

    vec3 ray_origin;
    vec3 ray_dir;

    int  depth;
    uint seed;
};
```

### La Shader Binding Table

Para solventar el problema de determinar el material de un objeto en ray tracing vamos usar la **Shader Binding Table** (SBT). Esta estructura permitirá cargar el shader correspondiente dependiendo de dónde impacte un rayo.

Para cargar esta estructura, se debe hacer lo siguiente:

1. Cargar y compilar cada shader en un `VkShaderModule`.
2. Juntar los cada `VkShaderModule` en un array `VkPipelineShaderStageCreateInfo`.
3. Crear un array de `VkRayTracingShaderGroupCreateInfoKHR`. Cada elemento se convertirá al final en una entrada de la Shader Binding Table.
4. Compilar los dos arrays anteriores más un pipeline layout para generar pipeline de ray tracing mediante la función `vkCreateRayTracingPipelineKHR`.
5. Conseguir los *handlers* de los shaders usando `vkGetRayTracingShaderGroupHandlesKHR`.
6. Alojar un buffer con el bit `VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR` y copiar los *handlers*.

![La Shader Binding Table permite selccionar un tipo de shader dependiendo del objeto en el que se impacte. Para ello, se genera un rayo desde el shader `raygen`, el cual viaja a través de la Acceleration Structure. Dependiendo de dónde impacte, se utiliza un `closest hit`, `any hit`, o `miss` shaders. Fuente: [@GemsII-SBT, p. 194]](./img/04/Pipeline.png){#fig:pipeline}

Cada entrada de la SBT contiene un *handler* y una serie de parámetros embebidos. A esto se le conoce como **Shader Record**. Estos records se clasifican en:

- **Ray generation record**: contiene el *handler* del ray generation shader.
- **Hit group record**: se encargan de los *handler*s del closest hit, anyhit (opcional), e intersection (opcional).
- **Miss group record**: se encarga del miss shader.
- **Callable group record**: para los shaders de tipo callable.

Una de las partes más difíciles de la SBT es saber cómo se relacionan *record* y geometría. Es decir, cuando un rayo impacta en una geometría, ¿a qué record de la SBT llamamos? Esto se determina mediante los parámetros de la instancia, la llamada a *trace rays*, y el orden de la geometría en la BLAS.

Para conocer a fondo cómo funciona la Shader Binding Table, puedes visitar [@GemsII-SBT, p. 193] o [@shader-binding-table].

### Creación de la ray tracing pipeline

El código de la creación de la pipeline está encapsulado en la función `Engine::createRtPipeline()`, que se puede consultar en el archivo `application/vulkan_ray_tracing/src/engine.cpp`.

En esencia, este método realiza las siguientes tareas:

1. Define las fases o *stages* que tendrán los shaders.
2. Prepara las estructuras `VkPipelineShaderStageCreateInfo` para almacenar la información de cada fase.
3. Carga cada archivo de shader compilado `.spv` en la estructura junto con sus parámetros correctos.
4. Configura correctamente cada *shader group*.
5. Prepara las *push constants*.
6. Hace el setup del *pipeline layout* junto a sus *descriptor sets*.
7. Limpia la información innecesaria creada por la función.

## Materiales y objetos

El formato de materiales y objetos usados es el **Wavefront** (`.obj`). Aunque es un sistema relativamente antiguo y sencillo, se han usado definiciones específicas en los materiales para adaptarlo a *Physically Based Rendering*. Entre los parámetros del archivo de materiales `.mtl`, destacan:

- $K_a \in [0, 1]^3$: representa el color ambiental. Dado que esto es un path tracer físicamente realista, no se usará.
- $K_d \in [0, 1]^3$: componente difusa.
- $K_s \in [0, 1]^3$: componente especular. Viene acompañada del exponente especular $N_s \in [0, 1000]$. Usualmente, $N_s = 10$. Controla los brillos en los modelos de Blinn-Phong.
- $d \in [0, 1]$ (*dissolve*): representa la transparencia. Alternativamente, se usa $T_r = 1 - d$.
- $T_f \in [0, 1]^3$: filtro de transmisión.
- $N_i \in [0.001, 10]$: índice de refracción. Usualmente $N_i = 1$.
- $K_e \in [0, 1]^3$: componente emisiva (PBR).
- Todos los valores con tres componentes pueden presentar un *texture map*.

Todos estos parámetros son opcionales y se pueden omitir, pero lo normal es incluir los tres primeros ($K_a$, $K_d$, $K_s$).

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
  float ior;
  float dissolve;
  int   illum;
  int   textureId;
};
```

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
| `10`       | Puede arrojar sombras a superficies invisibles.                          |

Table: tabla de valores para el parámetro `illum`. {#tbl:illum}

## Fuentes de luz

La última estructura de datos importante que debemos estudiar es la utilizada para las fuentes de luz. Desafortunadamente, en este trabajo no se ha implementado una abstracción sólida.

Se ha reaprovechado la definición del [rasterizador por defecto](#setup-del-proyecto) para que tanto el path tracer como el anterior utilicen fácilmente iluminación estática.

La idea básica es que, en vez de depender de los elementos de la escena para proporcionar luz, se conozca una fuente de iluminación en todo momento. Dicha fuente puede ser puntual o direccional, y puede ser controlada mediante la interfaz. El estado de la fuente se traspasa a los shaders mediante una push constant:

```cpp
struct PushConstantRay
{
    ...
    vec3  light_position;
    float light_intensity;
    int   light_type;
};
```

El parámetro `light_intensity` corresponde a la potencia $\Phi$, y el tipo `light_type` puede ser `0` para puntual o `1` para direccional.

Claramente esta decisión técnica favocere facilidad de implementación en detrimento de flexibilidad, solidez y correctitud. Esta interfaz es una de las áreas de futura mejora, y haría falta una revisión considerable. Sin embargo, por el momento, funciona.

La implementación en los shaders es muy sencilla. Podemos usar lo aprendido en [muestreo directo de fuentes de luz](#next-event-estimation-o-muestreo-directo-de-fuentes-de-luz). En el closest hit, primero calculamos la información relativa a la posición y la intensidad de la luz:

```glsl
vec3 L;
float light_intensity = pcRay.light_intensity;
float light_distance = 100000.0;

float pdf_light       = 1;  // prob. de escoger ese punto de la fuente de luz
float cos_theta_light = 1;  // Ángulo entre la la dir. del rayo y luz.

if (pcRay.light_type == 0) {         // Point light
    vec3 L_dir = pcRay.light_position - world_position;  // vector hacia la luz

    light_distance   = length(L_dir);
    light_intensity = pcRay.light_intensity / (light_distance * light_distance);
    L               = normalize(L_dir);
    // Solo tenemos un punto => pdf light = 1, cos_theta light = 1.
    cos_theta_light = dot(L, world_normal);
}
else if (pcRay.light_type == 1) {    // Directional light
    L = normalize(pcRay.light_position);
    cos_theta_light = dot(L, world_normal);
}
```

Sin embargo, esto no es suficiente. Se nos olvida comprobar un detalle sumamente importante:

¿Se ve la fuente de luz desde el punto de intersección?

Si no es así, ¡no tiene sentido que calculemos la influencia luminaria de la fuente! La carne de burro no se transparenta, después de todo. A no ser que sea un toro hecho de algún material que presente transmitancia [@fig:materiales_transparentes], en cuyo caso se debería refractar acordemente el rayo de luz.

Volviendo al tema: este tipo de problemas de oclusión se suelen resolver mediante algún tipo de test de visibilidad. El más habitual es usar **shadow rays**. En la preparación de la [pipeline](#la-ray-tracing-pipeline) se fija este tipo de shaders junto a los del tipo miss:

```cpp
// void Engine::createRtPipeline()
// ...
stage.module = nvvk::createShaderModule(m_device,
    nvh::loadFile("spv/raytrace.rmiss.spv", true, defaultSearchPaths, true)
);
stage.stage   = VK_SHADER_STAGE_MISS_BIT_KHR;
stages[eMiss] = stage;

// El segundo miss shader se invoca cuando un shadow ray no ha colisionado con la geometría.
// Simplemente, indica que no ha habido oclusión.
stage.module = nvvk::createShaderModule(m_device,
    nvh::loadFile("spv/raytraceShadow.rmiss.spv", true, defaultSearchPaths, true
));
stage.stage    = VK_SHADER_STAGE_MISS_BIT_KHR;
stages[eMiss2] = stage;

// Resto de shaders...
```

La continuación del código quedaría de la siguiente forma:

```glsl
if (dot(normal, L) > 0) {
    // Preparar la invocación del shadow ray
    float tMin = 0.001;
    float tMax = light_distance;

    vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
    vec3 ray_dir = L;

    uint flags = gl_RayFlagsSkipClosestHitShaderEXT;
    prdShadow.is_hit = true;
    prdShadow.seed = prd.seed;

    traceRayEXT(topLevelAS,
        flags,       // rayFlags
        0xFF,        // cullMask
        1,           // sbtRecordOffset => invocar el shader de sombras
        0,           // sbtRecordStride
        1,           // missIndex
        origin,      // ray origin
        tMin,        // ray min range
        ray_dir,     // ray direction
        tMax,        // ray max range
        1            // payload (location = 1)
    );

    float attenuation = 1;

    if (!prdShadow.is_hit) {
        hit_value = hit_value + light_intensity*BSDF*cos_theta_light / pdf_light;
    }
    else {
        attenuation = 1.0 / (1.0 + light_distance);
    }
}
```

Y con esto, hemos conseguido añadir dos tipos de fuentes de iluminación.

Debemos destacar que el programa **no conoce la posición de los objetos emisivos de la escena**. Esto significa que no podemos muestrearlos explícitamente. En la [comparativa](#análisis-de-rendimiento) comprobaremos cómo afecta esto a la calidad de imagen. Además, en la conclusión hablaremos de cómo se podría solventar este problema.

## Implementación eficiente del algoritmo sin recursividad

El código de la sección ["Pseudocódigo de un path tracer"](#pseudocódigo-de-un-path-tracer) tiene el problema de que utiliza recursividad. En la implementación de los shaders, esto supondría generar rayos desde el closest hit. Para evitarlo, reestructuraremos el código de forma que únicamente se lancen desde el raygen y calculemos la radiancia total en dicho shader.

El código resultante sería el siguiente: por una parte, una función se encarga de generar los rayos, denominada `pathtrace()`:

```cpp
vec3 pathtrace(vec4 ray_origin, vec4 ray_dir, float t_min, float t_max, uint ray_flags) {
    // Inicializar el payload correctamente
    prd.depth      = 0;
    prd.hit_value  = vec3(0);
    prd.ray_origin = ray_origin.xyz;
    prd.ray_dir    = ray_dir.xyz;
    prd.weight     = vec3(0);
    // prd.seed ya estaba inicializado

    vec3 current_weight = vec3(1);
    vec3 hit_value      = vec3(0);

    // Evitar llamadas recursivas a traceRayEXT() desde el closest hit.
    for (; prd.depth < pcRay.max_depth; prd.depth++) {
        traceRayEXT(topLevelAS, // acceleration structure
            ray_flags,          // rayFlags
            0xFF,               // cullMask
            0,                  // sbtRecordOffset
            0,                  // sbtRecordStride
            0,                  // missIndex
            prd.ray_origin,     // ray origin
            t_min,              // ray min range
            prd.ray_dir,        // ray direction
            t_max,              // ray max range
            0                   // payload (location = 0)
        );

        hit_value      += prd.hit_value * current_weight;
        current_weight *= prd.weight;
    }

    return hit_value;
}
```

Y por otro lado, otra función debe almacenar correctamente la información del punto de impacto, así como la radiancia de ese punto. Corresponde al closest hit:

```cpp
closest_hit() {
    // Sacar información sobre el punto de impacto: material, normal...
    WaveFrontMaterial mat = material(world_position);

    // Preparar información para el raygen
    prd.hit_value      = material.emision;
    prd.ray_origin     = world_position;
    prd.ray_dir        = siguiente_direccion(material)
    float probabilidad = pdf(prd.ray_dir);

    // Calcular la radiancia
    float cos_theta = dot(prd.ray_dir, normal);
    vec3 BRDF       = BRDF(material)

    prd.weight = (BRDF * cos_theta) / pdf

    return prd
}
```

El código final es considerablemente más complejo que este que se presenta.  Sin embargo, se han obviado algunos detalles de implementación con la intención de ser explicativo.

Esta versión no es tan intuitiva. ¿Por qué este último genera el mismo resultado que el de [la versión recursiva](#pseudocódigo-de-un-path-tracer)?

Analicemos lo está ocurriendo.

Sea $h$ el *hit value* (que simboliza la radiancia), $w$ el peso, $f_i$ la BRDF (o en su defecto, BTDF/BSDF), $i$, $e_i$ la emisión, $\cos\theta_i$ el coseno del ángulo que forman la nueva dirección del rayo y la normal, y $p_i$ la función de densidad que, dada una dirección, proporciona la probabilidad de que se escoja. El subíndice denota el $i$-ésimo punto de impacto.

En esencia, este algoritmo está descomponiendo lo que recogemos en `weight`, que es $f_i \cos\theta_i / p_i$. Inicialmente, para el primer envío del rayo, $h = (0, 0, 0)$, $w = (1, 1, 1)$. Tras trazar el primer rayo, se tiene que

$$
\begin{aligned}
    h & = 0 + e_1 w = e_1, \\
    w & = \frac{f_1 \cos\theta_1}{p_1}
\end{aligned}
$$

Tras el segundo rayo, obtenemos

$$
\begin{aligned}
    h & = e_1 + e_2 w = \\
      & = e_1 + e_2 \frac{f_1 \cos\theta_1}{p_1}, \\
    w & = \frac{f_1 \cos\theta_1}{p_1} \frac{f_2 \cos\theta_2}{p_2}
\end{aligned}
$$

Y para el tercero

$$
\begin{aligned}
    h & = e_1 + e_2 \frac{f_1 \cos\theta_1}{p_1} + e_3 w = \\
      & = e_1 + e_2 \frac{f_1 \cos\theta_1}{p_1} + e_3 \frac{f_1 \cos\theta_1}{p_1} \frac{f_2 \cos\theta_2}{p_2} = \\
      & = e_1 + \frac{f_1 \cos\theta_1}{p_1}\textcolor{verde-oscurisimo}{\left(e_2 + e_3 \frac{f_2 \cos\theta_2}{p_2}\right)}, \\
    w & = \frac{f_1 \cos\theta_1}{p_1} \frac{f_2 \cos\theta_2}{p_2} \frac{f_3 \cos\theta_3}{p_3}
\end{aligned}
$$

El $\textcolor{verde-oscurisimo}{\text{término que acompaña}}$ a $\frac{f_1 \cos\theta_1}{p_1}$ es la radiancia del tercer punto de impacto. Por tanto, a la larga, se tendrá que $h$ estima correctamente la radiancia de un punto. Con esto, podemos afirmar que

$$
h \approx \frac{1}{N} \sum_{j = 1}^{N}{\frac{f(p, \omega_o \leftarrow \omega_j) L_i(p, \omega_j) \cos\theta_j}{\Prob{\omega_j}}}
$$

Este algoritmo supone una mejora de hasta 3 veces mayor rendimiento que el recursivo [@nvpro-samples-tutorial, glTF Scene].


## Antialiasing mediante jittering y acumulación temporal

Normalmente, mandamos los rayos desde el centro de un pixel. Podemos conseguir una mejora sustancial de la calidad con un pequeño truco: en vez de generarlos siempre desde el mismo sitio, le aplicamos una pequeña perturbación (*jittering*). Así, tendremos una variación de colores para un mismo pixel, por lo que podemos hacer una ponderación de todos ellos. A este proceso lo que llamamos **supersampling mediante jittering**.

Si conforme pasa el tiempo utilizamos la información de las imágenes anteriores para renderizar el frame actual, podemos conseguir un resultado aún mejor que aplicando solo *supersampling*. Es decir, promediando el color de los últimos $N$ *frames* para generar el frame actual. Esta técnica se llama **acumulación temporal**.

Es importante destacar que la acumulación temporal solo es válido cuando la **cámara se queda estática**. Al cambiar de posición, la información del píxel se ve alterada significativamente, por lo que debemos reconstruir las muestras desde el principio[^7].

La implementación es muy sencilla. Está basada en el tutorial de [@nvpro-samples-tutorial, jitter camera]. Debemos modificar tanto el motor como los shaders para llevar el recuento del número de *frames* en las push constants.

Definimos el número máximo de *frames* que se pueden acumular:

```cpp
// engine.h
class Engine {
    //...
    int m_maxAcumFrames {100};
}
```

Las push constant deberán llevar un registro del *frame* en el que se encuentran, así como un número máximo de muestras a acumular para un pixel:

```cpp
// host_device.h
struct PushConstantRay {
    //...
    int   frame;
    int   nb_samples
}
```

El número de *frame* se reseteará cuando la cámara se mueva, la ventana se reescale, o se produzca algún efecto similar en la aplicación.

Finalmente, en los shaders podemos implementar lo siguiente:

```glsl
// raytrace.rgen
vec3 pixel_color = vec3(0);

for (int smpl = 0; smpl < pcRay.nb_samples; smpl++) {
    pixel_color += sample_pixel(image_coords, image_res);
}

pixel_color = pixel_color / pcRay.nb_samples;

if (pcRay.frame > 0) {
    vec3 old_color = imageLoad(image, image_coords).xyz;
    vec3 new_result = mix(
        old_color,
        pixel_color,
        1.f / float(pcRay.frame + 1)
    );

    imageStore(image, image_coords, vec4(new_result, 1.f));
}
else {
    imageStore(image, image_coords, vec4(pixel_color, 1.0));
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

En la sección de la [comparativa](#comparativa-con-in-one-weekend) estudiaremos a fondo los efectos de esta técnica.

## Corrección de gamma

Con el código de la sección [anterior](#antialiasing-mediante-jittering-y-acumulación-temporal), existe un problema con los colores finales. El algoritmo de pathtracing no limita el máximo valor que puede tomar un camino. Sin embargo, Vulkan espera que la terna RGB provista esté en $[0, 1]^3$. Esto implica que los colores acabarán quemados.

![Fíjate en la parte de la izquierda. La pared roja aparece demasiado brillante; especialmente, aquella impactada por la fuente de luz.](./img/04/Quemado.png){#fig:quemado width=50%}

Podemos corregir este problema mediante la acotación máxima de la terna RGB producida por el algoritmo. Sin embargo, esto ocasiona que la proporción de valores finales no sea correcta. Por ejemplo, si un camino genera la terna $c = (15, 5, 7)$, el resultado de $\max{(1, c_i)}, i = 1, 2, 3$ sería $(1, 1, 1)$.

Podemos corregir este problema mediante **corrección de gamma**. Esta es una operación no lineal utilizada en fotografía para corregir la luminacia, con el fin de compensar la percepción no lineal del brillo por parte de los humanos. En este caso, lo haremos al estilo [@Shirley2020RTW1]: tras tomar las muestras, aplicaremos una corrección para $\gamma = 2.2$, lo cual implica elevar cada componente del píxel a la potencia $\frac{1}{2.2}$; es decir, $(r_f, g_f, b_f) = (r^{\frac{1}{2.2}}, g^{\frac{1}{2.2}}, b^{\frac{1}{2.2}})$.

Tras esto, limitaremos el valor máximo de cada componente a 1 con la operación $clamp()$.

```glsl
vec3 pixel_color = vec3(0);

for (int smpl = 0; smpl < pcRay.nb_samples; smpl++) {
    pixel_color += sample_pixel(image_coords, image_res);
}

pixel_color = pixel_color / pcRay.nb_samples;

if (USE_GAMMA_CORRECTION == 1) {
    pixel_color = pow(pixel_color, vec3(1.0 / 2.2));  // Gamma correction for 2.2
    pixel_color = clamp(pixel_color, 0.0, 1.0);
}
```

![Con la corección de gamma aplicada, vemos que los colores de la foto no son tan intensos.](./img/04/Corrección%20de%20gamma.png){#fig:correccion_gamma width=50%}

> Espera. Esa no parece la misma escena. ¿No han cambiado los colores demasiado?

¡Bien visto! Es cierto que los colores se ven significativamente alterados. Esto es debido a la conversión de un espacio lineal de respuesta de radiancia a uno logarítmico. Algunos autores como Íñigo Quílez (coautor de la página Shader Toy) prefieren asumir esta deficiencia, y modificar los materiales acordemente a esto [@gamma-correction, The Color Space].

Nosotros no nos preocuparemos especialmente por esto. Este no es un trabajo sobre teoría del color, aunque nos metamos en varias partes en ella. El área de *tone mapping* es extensa y merecería su propio estudio.

Es importante mencionar que sin acumulación temporal, el código anterior produciría variaciones significativas para pequeños movimientos. Hay otras formas de compensarlo, como dividir por el valor promedio de las muestras más brillantes. Nosotros hemos optado por mezclar los píxeles generados a lo largo del tiempo.





[^5]: Esto no es del todo cierto. Aunque generalmente suelen ser excepciones debido al coste computacional de RT en tiempo real, existen algunas implementaciones que son capaces de correrlo por software. Notablemente, el motor de Crytek, CryEngine, es capaz de mover ray tracing basado en hardware y en software [@crytek-2020]
[^6]: Afortunadamente, esto tampoco es completamente cierto. La compañía desarrolladora y distribuidora de videojuegos Valve Corporation [@valve] ha creado una pieza de software fascinante: Proton [@proton]. Proton utiliza Wine para emular software en Linux que solo puede correr en plataformas Windows. La versión 2.5 añadió soporte para traducción de bindings de DXR a KHR, lo que permite utilizar DirectX12 ray tracing en sistemas basados en Linux. El motivo de este software es expandir el mercado de videojuegos disponibles en su consola, la Steam Deck.
[^7]: A no ser que se utilicen *motion vectors*, los cuales codifican información sobre el movimiento de un objeto al pasar de un *frame* a otro. Estos permiten implementar técnicas como *temporal antialiasing*, los cuales veremos en una sección posterior.