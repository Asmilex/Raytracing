# Los fundamentos

Empecemos por definir lo que es un rayo.

Un rayo es una función $P(t) = O + tD$, donde $O$ es el origin, $D$ la dirección, y $t \in \mathbb{R}$. Podemos considerarlo una interpolación entre dos puntos en el espacio, donde $t$ controla la posición en la que nos encontramos.

Por ejemplo, si $t = 0$, obtendremos el origen. Si $t = 1$, obtendremos el punto correspondiente a la dirección. Usando valores negativos vamos *hacia atrás*.

![](./img/rayo.png)

Dado que estos puntos estarán generalmente en $\mathbb{R}^3$, podemos escribirlo como

$$
P(t) = (O_x, O_y, O_z) + t (D_x, D_y, D_z)
$$

Estos rayos los *dispararemos* a través de una cámara virtual, que estará enfocando a la escena. De esta forma, los haremos rebotar con los objetos que se encuentren en el camino del rayo. A este proceso lo llamaremos **ray casting**.

> TODO foto de cámara - pixel - rayo - objeto.

Generalmente, nos quedaremos con el primer objeto que nos encontremos en su camino. Aunque, a veces, nos interesará saber todos con los que se encuentre.

Cuando un rayo impacta con un objeto, adquirirá parte de las propiedades lumínicas del punto de impacto. Por ejemplo, cuánta luz proporciona la lámpara que tiene encima la esfera de la figura anterior.

Una vez recojamos la información que nos interese, aplicaremos otro raycast desde el nuevo punto de impacto, escogiendo una nueva dirección determinada. Esta dirección dependerá del tipo de material del objeto. Y, de hecho, algunos serán capaces de invocar varios rayos.

Por ejemplo, los espejos reflejan la luz casi de forma perfecta; mientras que otros elementos como el agua o el cristal reflejan *y* refractan luz, así que necesitaremos generar dos nuevos raycast.

Usando suficientes rayos obtendremos la imagen de la escena. A este proceso de **ray casting recursivo** es lo que se conoce como ray tracing.

Como este proceso puede continuar indefinidamente, tendremos que controlar la profundidad de la recursión. A mayor profundidad, mayor calidad de imagen; pero también, mayor tiempo de ejecución.

## Eligiendo direcciones

Una de las partes más importantes de ray tracing, y a la que quizás dedicaremos más tiempo, es a la elección de la dirección.

Hay varios factores que entran en juego a la hora de decidir qué hacemos cuando impactamos con un nuevo objeto:

1. **¿Cómo es la superficie del material?** A mayor rugosidad, mayor aleatoriedad en la dirección. Por ejemplo, no es lo mismo el asfalto de una carretera que una lámina de aluminio impecable.
2. **¿Cómo de fiel es nuestra geometría?**
3. **¿Dónde se encuentran las luces en la escena?** Dependiendo de la posición, nos interesará muestrear la luz con mayor influencia.

Estas cuestiones las exploraremos a fondo en las siguientes secciones.

## Intersecciones rayo - objeto

Como dijimos al principio del capítulo, representaremos un rayo como $P(t) = (O_x, O_y, O_z) + t (D_x, D_y, D_z) = (O_x + t D_x, O_y + t D_y, O_y + t D_z)$.

Por ejemplo, tomando $O = (1, 3, 2), D = (1, 2, 1)$:

- Para $t = 0$, $P(t) = (1, 3, 2)$.
- Para $t = 1$, $P(t) = (1, 3, 2) + (1, 2, 1) = (2, 5, 3)$.

Nos resultará especialmente útil limitar los valores que puede tomar $t$. Restringiremos los posibles puntos del dominio de forma que  $t \in [t_{min}, t_{max})$, con $t_{min} < t_{max}$.

Una de las principales cuestiones que debemos hacernos es saber cuándo un rayo impacta con una superficie. Lo definiremos analíticamente.

### Superficies implícitas

Generalmente, cuando hablemos de superficies, nos referiremos [superficies diferenciables](https://www.wikiwand.com/en/Differential_geometry_of_surfaces), pues nos interesará conocer el vector normal en cada punto.

Una superficie implícita es una superficie en un espacio euclidiano definida como

$$
F(x, y, z) = 0
$$

Esta ecuación implícita define una serie de puntos del espacio $\mathbb{R}^3$ que se encuentran en la superficie.

Por ejemplo, la esfera se define como $x^2 + y^2 + z^2 - 1 = 0$.

Consideremos una superficie $S$ y un punto regular de ella $P$; es decir, un punto tal que el gradiente de $F$ en $P$ no es 0. Se define el vector normal $\mathbf{n}$ a la superficie en ese punto como

$$
\mathbf{n} = \nabla F(P) = \left( \frac{\partial F(P)}{\partial x}, \frac{\partial F(P)}{\partial y}, \frac{\partial F(P)}{\partial z}\right )
$$

> TODO: dibujo de la normal a una superficie.

Dado un punto $Q \in \mathbb{R}^3$, queremos saber dónde interseca un rayo $P(t)$. Es decir, para qué $t$ se cumple que $F(P(t)) = 0 \iff F(O + tD) = 0$.

Consideremos por ejemplo un plano, como en [@ShirleyRRT]. Para ello, nos tomamos un punto $Q_0$ del plano y un vector normal a la superficie $\mathbf{n}$.

La ecuación implícita del plano será

$$
F(Q) = (Q - Q_0) \cdot \mathbf{n} = 0
$$

Si pinchamos nuestro rayo en la ecuación,

$$
\begin{aligned}
F(P(t)) & = (P(t) - Q_0) \cdot \mathbf{n} \\
        & = (O + tD - Q_0) \cdot \mathbf{n} = 0 \\
\end{aligned}
$$

Resolviendo para $t$, esto se da si

$$
\begin{aligned}
O \cdot \mathbf{n} + tD \cdot \mathbf{n} - Q_0 \cdot \mathbf{n} & = 0 & \iff \\
tD \cdot \mathbf{n} & = Q_0 \cdot \mathbf{n} - O \cdot \mathbf{n} & \iff \\
t & = \frac{Q_0 \cdot \mathbf{n} - O \cdot \mathbf{n}}{D \cdot \mathbf{n}}
\end{aligned}
$$

Es decir, hemos obtenido el único valor de $t$ para el cual el rayo toca la superficie.

Debemos tener en cuenta el caso para el cual $D \cdot \mathbf{n} = 0$. Esto solo se da si la dirección y el vector normal a la superficie son paralelos.

> TODO: dibujo de dos rayos con un plano: uno corta a la superficie, mientras que el otro es paralelo.

### Superficies paramétricas

Otra forma de definir una superficie en el espacio es mediante un subconjunto $D \sub \mathbb{R}^2$ y una serie de funciones, $f, g, h: D \rightarrow \mathbb{R}^3$, de forma que

$$
(x, y, z) = \left( f(u, v), g(u, v), h(u, v) \right) \\
$$

> En informática gráfica, hacemos algo similar cuando mapeamos una textura a una superficie. Se conoce como UV mapping

Demos un par de ejemplos de superficies paramétricas:
- El grafo de una función $f: D \rightarrow \mathbb{R}^3$,
$$
G(f) = \left\{(x, y, f(x, y)) \,\middle|\,  (x, y) \in D\right\}
$$
define una superficie diferenciable siempre que $f$ también lo sea.
- Usando coordenadas esféricas $(r, \theta, \phi)$, podemos parametrizar la esfera como $(x, y, z) = (\cos\phi\sin\theta, \sin\phi\sin\theta, \cos\theta)$ (

> TODO añadir imagen de coordenadas esféricas. U otro capítulo con coordenadas.
>
> NOTE: estoy usando (radial, polar, azimuthal). $\theta$ corresponde con la apertura con respecto a la vertical)

<hr>

Fuentes que he usado y que debería pasar:

- https://www.wikiwand.com/en/Implicit_surface
- https://www.wikiwand.com/en/Parametric_surface
