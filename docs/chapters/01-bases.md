# Las bases

Empecemos por definir lo que es un rayo.

Un rayo [@Shirley2020RTW1] es una función $P(t) = o + td$, donde $o$ es el origin, $d$ la dirección, y $t \in \mathbb{R}$. Podemos considerarlo una interpolación entre dos puntos en el espacio, donde $t$ controla la posición en la que nos encontramos.

Por ejemplo, si $t = 0$, obtendremos el origen. Si $t = 1$, obtendremos el punto correspondiente a la dirección. Usando valores negativos vamos *hacia atrás*.

![El parámetro $t$ nos permite controlar los puntos del rayo](./img/01/Rayo%20básico.png){#fig:rayo_basico width=70% }

Dado que estos puntos estarán generalmente en $\mathbb{R}^3$, podemos escribirlo como

$$
P(t) = (o_x, o_y, o_z) + t (d_x, d_y, d_z)
$${#eq:rayo}

Estos rayos los *dispararemos* a través de una cámara virtual, que estará enfocando a la escena. De esta forma, los haremos rebotar con los objetos que se encuentren en el camino del rayo. a este proceso lo llamaremos **ray casting**.

![Diagrama de ray casting](./img/01/Ray%20casting.png){#fig:ray_casting width=80%}

Generalmente, nos quedaremos con el primer objeto que nos encontremos en su camino. Aunque, a veces, nos interesará saber todos con los que se encuentre.

cuando un rayo impacta con un objeto, adquirirá parte de las propiedades lumínicas del punto de impacto. Por ejemplo, cuánta luz proporciona la lámpara que tiene encima la esfera de la figura anterior.

Una vez recojamos la información que nos interese, aplicaremos otro raycast desde el nuevo punto de impacto, escogiendo una nueva dirección determinada. Esta dirección dependerá del tipo de material del objeto. Y, de hecho, algunos serán capaces de invocar varios rayos.

Por ejemplo, los espejos reflejan la luz casi de forma perfecta; mientras que otros elementos como el agua o el cristal reflejan *y* refractan luz, así que necesitaremos generar dos nuevos raycast.

Usando suficientes rayos obtendremos la imagen de la escena. a este proceso de **ray casting recursivo** es lo que se conoce como ray tracing.

como este proceso puede continuar indefinidamente, tendremos que controlar la profundidad de la recursión. a mayor profundidad, mayor calidad de imagen; pero también, mayor tiempo de ejecución.

## Eligiendo direcciones

Una de las partes más importantes de ray tracing, y a la que quizás dedicaremos más tiempo, es a la elección de la dirección.

Hay varios factores que entran en juego a la hora de decidir qué hacemos cuando impactamos con un nuevo objeto:

1. **¿cómo es la superficie del material?** a mayor rugosidad, mayor aleatoriedad en la dirección. Por ejemplo, no es lo mismo el asfalto de una carretera que una lámina de aluminio impecable.
2. **¿cómo de fiel es nuestra geometría?**
3. **¿Dónde se encuentran las luces en la escena?** Dependiendo de la posición, nos interesará muestrear la luz con mayor influencia.

Estas cuestiones las exploraremos a fondo en las siguientes secciones.

## Intersecciones rayo - objeto

como dijimos al principio del capítulo, representaremos un rayo como

$$
\begin{aligned}
P(t) & = (o_x, o_y, o_z) + t (d_x, d_y, d_z) = \\
& = (o_x + t d_x, o_y + t d_y, o_y + t d_z)
\end{aligned}
$$

Por ejemplo, tomando $o = (1, 3, 2), d = (1, 2, 1)$:

- Para $t = 0$, $P(t) = (1, 3, 2)$.
- Para $t = 1$, $P(t) = (1, 3, 2) + (1, 2, 1) = (2, 5, 3)$.

Nos resultará especialmente útil limitar los valores que puede tomar $t$. Restringiremos los posibles puntos del dominio de forma que $t \in [t_{min}, t_{max})$, con $t_{min} < t_{max}$. En general, nos interesará separarnos de las superficies un pequeño pero no despreciable $\varepsilon$ para evitar errores de redondeo.

![Separarnos un poquito del origen evitará errores de coma flotante](./img/01/Límites%20de%20un%20rayo.png){#fig:limites_rayo}

Una de las principales cuestiones que debemos hacernos es saber cuándo un rayo impacta con una superficie. Lo definiremos analíticamente.

### Superficies implícitas

Generalmente, cuando hablemos de superficies, nos referiremos superficies diferenciables [@wikipedia-contributors-2022O], pues nos interesará conocer el vector normal en cada punto.

Una superficie implícita es una superficie en un espacio euclidiano definida como

$$
F(x, y, z) = 0
$$

Esta ecuación implícita define una serie de puntos del espacio $\mathbb{R}^3$ que se encuentran en la superficie.

Por ejemplo, la esfera se define como $x^2 + y^2 + z^2 - 1 = 0$.

consideremos una superficie $S$ y un punto regular de ella $p$; es decir, un punto tal que el gradiente de $F$ en $p$ no es 0. Se define el vector normal $\mathbf{n}$ a la superficie en ese punto como

$$
\mathbf{n} = \nabla F(p) = \left( \frac{\partial F(p)}{\partial x}, \frac{\partial F(p)}{\partial y}, \frac{\partial F(p)}{\partial z}\right )
$${#eq:normal_superficie}

> TODO: dibujo de la normal a una superficie.

Dado un punto $q \in \mathbb{R}^3$, queremos saber dónde interseca un rayo $P(t)$. Es decir, para qué $t$ se cumple que $F(P(t)) = 0 \iff F(o + td) = 0$.

consideremos ahora un plano. Para ello, nos tomamos un punto $Q_0$ del plano y un vector normal a la superficie $\mathbf{n}$. La ecuación implícita del plano será [@ShirleyRRT]

$$
F(Q) = (q - q_0) \cdot \mathbf{n} = 0
$$

Si pinchamos nuestro rayo en la ecuación,

$$
\begin{aligned}
F(P(t)) & = (P(t) - q_0) \cdot \mathbf{n} \\
        & = (o + td - q_0) \cdot \mathbf{n} = 0 \\
\end{aligned}
$$

Resolviendo para $t$, esto se da si

$$
\begin{aligned}
o \cdot \mathbf{n} + td \cdot \mathbf{n} - Q_0 \cdot \mathbf{n} & = 0 & \iff \\
td \cdot \mathbf{n} & = Q_0 \cdot \mathbf{n} - o \cdot \mathbf{n} & \iff \\
t & = \frac{Q_0 \cdot \mathbf{n} - o \cdot \mathbf{n}}{d \cdot \mathbf{n}}
\end{aligned}
$$

Es decir, hemos obtenido el único valor de $t$ para el cual el rayo toca la superficie.

Debemos tener en cuenta el caso para el cual $d \cdot \mathbf{n} = 0$. Esto solo se da si la dirección y el vector normal a la superficie son paralelos.

> TODO: dibujo de dos rayos con un plano: uno corta a la superficie, mientras que el otro es paralelo.

### Superficies paramétricas

Otra forma de definir una superficie en el espacio es mediante un subconjunto $S \subset \mathbb{R}^2$ y una serie de funciones, $f, g, h: S \rightarrow \mathbb{R}^3$, de forma que

$$
(x, y, z) = \left( f(u, v), g(u, v), h(u, v) \right) \\
$$

> En informática gráfica, hacemos algo similar cuando mapeamos una textura a una superficie. Se conoce como **UV mapping**.

Demos un par de ejemplos de superficies paramétricas:

- El grafo de una función $f: S \rightarrow \mathbb{R}^3$,
$$
G(f) = \set{(x, y, f(x, y))}{(x, y) \in S}
$$
define una superficie diferenciable siempre que $f$ también lo sea.
- Usando coordenadas esféricas $(r, \theta, \phi)$, podemos parametrizar la esfera como $(x, y, z) = (\cos\phi\sin\theta, \sin\phi\sin\theta, \cos\theta)$

> TODO añadir imagen de coordenadas esféricas. U otro capítulo con coordenadas.
>
> NOTE: estoy usando (radial, polar, azimuthal). $\theta$ corresponde con la apertura con respecto a la vertical

El vector normal $\mathbf{n}$ a la superficie en un punto $(u, v)$ del dominio viene dado por

$$
\mathbf{n}(u, v) =
        \left( \frac{\partial f}{\partial u}, \frac{\partial g}{\partial u}, \frac{\partial h}{\partial u} \right)
                \times
        \left( \frac{\partial f}{\partial v}, \frac{\partial g}{\partial v}, \frac{\partial h}{\partial v} \right)
$$

Encontrar el punto de intersección de una superficie paramétrica con un rayo es sencillo. Basta con encontrar aquellos puntos $(u, v)$ y $t$ para los que

$$
\begin{aligned}
o_x + td_x & = f(u, v) \\
o_y + td_y & = g(u, v) \\
o_z + td_z & = h(u, v) \\
\end{aligned}
$$

Es posible que el rayo no impacte en ningún punto. En ese caso, el sistema de ecuaciones no tendría solución. Otra posibilidad es que intersequen en varios puntos.

### Intersecciones con esferas

Estudiemos ahora cómo intersecan una esfera con nuestro rayo. Una esfera de centro $c$ y radio $r$ viene dada por aquellos puntos $p = (x, y, z)$ que cumplen

$$
(p - c) \cdot (p - c) = r^2
$$

Podemos reescribir esta ecuación en términos de sus coordenadas para obtener

$$
(x - c_x)^2 + (y - c_y)^2 + (z - c_z)^2 = r^2
$$

Veamos para qué valores de $t$ de nuestro rayo se cumple esa ecuación:

$$
\begin{aligned}
(P(t) - c) \cdot (P(t) - c) & = r^2 & \iff \\
(o + td - c) \cdot (o + td - c) & = r^2 & \iff \\
\end{aligned}
$$

Aplicando las propiedades del producto escalar de la conmutatividad ($a \cdot b = b \cdot a$) y la distributiva ($a \cdot (b + c) = a \cdot b + a \cdot c$), podemos escribir

$$
\begin{aligned}
((o - c) + td) \cdot ((o - c) + td) & = r^2 & \iff \\
(o - c)^2 + 2 \cdot (o - c) \cdot td + (td)^2 & = r^2 & \iff \\
d^2t^2 + 2 d \cdot (o - c)t + (o - c)^2 - r^2 & = 0 & \iff \\
\end{aligned}
$$

Así que tenemos una ecuación de segundo grado. Resolviéndola, nos salen nuestros puntos de intersección:

$$
t = \frac{
    - d \cdot (o - c) \pm \sqrt{(d \cdot (o - c))^2 - 4 (d^2)((o - c)^2 - r^2)}
}{
    2 d^2
}
$$

Debemos distinguir tres casos, atiendiendo al valor que toma el discriminante $\Delta = \small{(D \cdot (o - c))^2 - 4 (d^2)((o - c)^2 - r^2)}$:

1. Si $\Delta < 0$, $\sqrt{\Delta} \notin \mathbb{R}$, y el rayo no impacta con la esfera
2. Si $\Delta = 0$, el rayo impacta en un punto, que toma el valor $t = \frac{-d \cdot (o - c)}{2 d \cdot d}$. Digamos que *pegaría* justo en el borde.
3. Si $\Delta > 0$, existen dos soluciones. En ese caso, el rayo atraviesa la esfera.

![Puntos de intersección con una esfera.](./img/01/Intersección%20rayo%20-%20esfera.png){#fig:interseccion_esfera width=60%}

Para estos dos últimos, si consideramos $t_0$ cualquier solución válida, el vector normal resultante viene dado por

$$
\mathbf{n} = 2 (P(t_0) - c)
$$

o, normalizando,

$$
\hat{\mathbf{n}} = \frac{(P(t_0) - c)}{r}
$$

### Intersecciones con triángulos

Este tipo de intersecciones serán las más útiles en nuestro path tracer. Generalmente, nuestras geometrías estarán compuestas por mallas de triángulos, así que conocer dónde impacta nuestro rayo será clave. Empecemos por la base:

Un triángulo viene dado por tres puntos, $a, b$, y $c$; correspondientes a sus vértices. Para evitar casos absurdos, supongamos que estos puntos son afinmente independientes; es decir, que no están alineados.

#### coordenadas baricéntricas

Podemos describir los puntos contenidos en el plano que forman estos vertices mediante **coordenadas baricéntricas**. Este sistema de coordenadas expresa cada punto del plano como una combinación convexa de los vértices [@wikipedia-contributors-2022G]. Es decir, que para cada punto $P$ del triángulo existen $\alpha, \beta$ y $\gamma$ tales que $\alpha + \beta + \gamma = 1$ y

$$
P = \alpha a + \beta b + \gamma c
$$

> TODO: triángulo con coordenadas baricéntricas.

Debemos destacar que existen dos grados de libertad debido a la restricción de que las coordenadas sumen 1.

Una propiedad de estas coordenadas que nos puede resultar útil es que un punto $p$ está contenido en el triángulo si y solo si $0 < \alpha, \beta, \gamma < 1$.

Esta propiedad y la restricción de que sumen 1 nos da una cierta intuición de cómo funcionan. Podemos ver las coordenadas baricéntricas como la contribución de los vértices a un punto $p$. Por ejemplo, si $\alpha = 0$, eso significa que el punto viene dado por $\beta b + \gamma c$; es decir, una combinación lineal de $b$ y $c$. Se encuentra en la recta que generan.

Por proponer otro ejemplo, si alguna de las coordenadas fuera mayor que 1, eso significaría que el punto estaría más allá del triángulo.

> TODO: dibujo con explicación de cómo funciona (libreta Shinrin - Yoku)

#### calculando la intersección

Podemos eliminar una de las varibales escribiendo $\alpha = 1 - \beta - \gamma$, lo que nos dice

$$
\begin{aligned}
p & = (1 - \beta - \gamma) a + \beta b + \gamma c \\
  & = a + (b - a) \beta + (c - a) \gamma
\end{aligned}
$$

bajo la restricción

$$
\begin{aligned}
\beta + \gamma & < 1 \\
0 & < \beta          \\
0 & < \gamma
\end{aligned}
$$ {#eq:beta_gamma}

Un rayo $P(t) = o + td$ impactará en un punto del triángulo si se cumple

$$
P(t) = o + td = a + (b - a) \beta + (c - a) \gamma
$$

cumpliendo [@eq:beta_gamma]. Podemos expandir la ecuación anterior en sus coordenadas para obtener

$$
\begin{aligned}
o_x + td_x & = a_x + (b_x - a_x) \beta + (c_x - a_x) \gamma \\
o_y + td_y & = a_y + (b_y - a_y) \beta + (c_y - a_y) \gamma \\
o_z + td_z & = a_z + (b_z - a_z) \beta + (c_z - a_z) \gamma \\
\end{aligned}
$$

Reordenamos:

$$
\begin{aligned}
(a_x - b_x) \beta + (a_x - c_x) \gamma+ td_x & = a_x - o_x \\
(a_y - b_y) \beta + (a_y - c_y) \gamma+ td_y & = a_y - o_y \\
(a_z - b_z) \beta + (a_z - c_z) \gamma+ td_z & = a_z - o_z
\end{aligned}
$$

Lo que nos permite escribir el sistema en forma de ecuación:

$$
\begin{pmatrix}
        a_x - b_x & a_x - c_x & d_x \\
        a_y - b_y & a_y - c_y & d_y \\
        a_z - b_z & a_z - c_z & d_z
\end{pmatrix}
\begin{pmatrix}
        \beta \\ \gamma \\ t
\end{pmatrix}
=
\begin{pmatrix}
        a_x - o_x \\ a_y - o_y \\ a_z - o_z
\end{pmatrix}
$$

calcular rápidamente la solución a un sistema de ecuaciones lineales es un problema habitual. En [@ShirleyRRT] se utiliza la regla de cramer para hacerlo, esperando que el compilador optimice las variables intermedias creadas. Nosotros no nos tendremos que preocupar de esto en particular, ya que el punto de impacto lo calculará la GPU gracias a las herramientras aportadas por KHR [@vulkan, Ray Traversal].

Para obtener el vector normal, podemos hacer el producto vectorial de dos vectores que se encuentren en el plano del triángulo. como, por convención, los vértices se guardan en sentido antihorario visto desde fuera del objeto, entonces

$$
\mathbf{n} = (b - a) \times (c - a)
$$