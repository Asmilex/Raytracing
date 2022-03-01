# Integración de Monte Carlo

> TODO: este capítulo seguramente debería ir más tarde. De esa forma, puedo introducir otros conceptos antes. De momento, se queda aquí.

La parte más importante de nuestro ray tracer es saber calcular la luz en un punto. Para ello, necesitaríamos hallar la radianza en dicho punto mediante la *rendering equation*. Sin embargo, es *muy* difícil resolverla. Tanto computacionalmente como analíticamente. Por ello, debemos atacar el problema desde otro punto de vista.

Las técnicas de Monte Carlo nos permitirán aproximar el valor que toman mediante una estimación. Utilizando muestreo aleatorio para evaluar integrales, seremos capaces de obtener un resultado suficientemente bueno.

Una de las propiedades más improtantes que tienen es la **independencia del ratio de convergencia y la dimensionalidad del integrando**. Sin embargo, dadas $n$ nuestras, la convergencia a la solución correcta tiene un orden de $\mathcal{O}\left(n^{-1/2}\right)$. Es decir, para reducir el error a la mitad, necesitaríamos 4 veces más muestras.

## Repaso de probabilidad

Necesitaremos unas cuantas nociones de variable aleatoria para poder entender la integración de Monte Carlo, así que vamos a hacer un breve repaso.

Una **variable aleatoria** $X$ (v.a.) es, esencialmente, una regla que asigna un valor numérico a cada posibilidad de algún proceso de azar. Formalmente, una variable aleatoria es una función definida en un espacio de probabilidad $(\Omega, \mathcal{A}, P)$ asociado a un exprimento aleatorio:

$$
X: \Omega \rightarrow \mathbb{R}
$$

A $\Omega$ lo conocemos como espacio muestral (aquel conjunto de todas las posibilidades), $\mathcal{A}$ es una $\sigma$-álgebra de subconjuntos de $\Omega$ que refleja todas las posibilidades de eventos aleatorios. $P$ es una función probabilidad, que asigna a cada evento una probabilidad.

> NOTE: no sé hasta qué punto debería meterme en la definición formal de variable aleatoria. Es una movida tremenda para poca cosa que necesitamos. De momento, voy con lo más interesante.

Una variable aleatoria $X$ puede clasificarse en discreta o continua, dependiendo de cómo sea su rango $R_X = \left\{ x \in \mathbb{R} \,\middle|\, \exists \omega \in \Omega : X(\omega) = x \right\}$: discretas o continuas.

### Variables aleatorias discretas

Las v.a. discretas son aquellas cuyo rango es un conjunto discreto.

Para comprender mejor cómo funcionan, pongamos un ejemplo.

Consideremos un experimento en el que lanzamos dos dados, anotando lo que sale en cada uno. Los posibles valores que toman serán

$$
\begin{aligned}
\{ & (1, 1), (1, 2), (1, 3), (1, 4), (1, 5), (1, 6),  \\
   & (2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6),  \\
   & (3, 1), (3, 2), (3, 3), (3, 4), (3, 5), (3, 6),  \\
   & (4, 1), (4, 2), (4, 3), (4, 4), (4, 5), (4, 6),  \\
   & (5, 1), (5, 2), (5, 3), (5, 4), (5, 5), (5, 6),  \\
   & (6, 1), (6, 2), (6, 3), (6, 4), (6, 5), (6, 6)   \}
\end{aligned}
$$

Cada resultado tiene la misma probabilidad de ocurrir (claro está, si el dado no está trucado). Como hay $36$ posibilidades, la probabilidad de obtener un cierto valor es de $\frac{1}{36}$.

La v.a. $X$ denotará la suma de los valores obtenidos en cada uno. Así, por ejemplo, si los dados han dado $(1, 3)$, $X$ será $4$. En total, $X$ puede tomar todos los valores comprendidos entre $2$ y $12$. Este sería el **espacio muestral**.  Además, podemos observar que $X$ puede obtener el mismo valor para dos resultados diferentes. Por ejemplo, $(1, 2)$ suma lo mismo que $(2, 1)$. Esto nos lleva a preguntarnos... ¿cuál es la probabilidad de que $X$ adquiera un cierto valor?

La **función masa de probabilidad** nos permite conocer la probabilidad de que una variable aleatoria $X$ tome un valor $x$. Se denota por $P(X = x)$.

En este ejemplo, la probabilidad de que $X$ tome el valor $4$ es

$$
P(X = 4) = p_i \cdot \text{nº parejas que sumen 4} = \frac{1}{36} \cdot 3 = \frac{1}{12}
$$

(Las parejas serían $(1, 3), (2, 2), (3, 1)$).

Por definición, si el espacio muestral de $X$ es $x_1, \dots, x_n$, la función masa de probabilidad debe cumplir que

$$
\sum_{i = 1}^{k}{P(X = x_i)} = 1
$$

La **función de distribución** de una variable aleatoria $X$ es

$$
F_X(x) = P(X \le x) = \sum_{k = -\infty}^{x}{P(X = x)} = 1
$$

Es una función continua por la derecha y monótona no decreciente. Además, se cumple que $\lim_{x \to -\infty}{F_X} = 0$, $\lim_{x \to \infty}{F_X} = 1$

### Variables aleatorias continuas

Estas variables aleatorias tienen un rango no numerable.

<hr>

- https://www.wikiwand.com/en/Rendering_equation
- https://www.wikiwand.com/es/Variable_aleatoria
- https://www.wikiwand.com/es/Distribuci%C3%B3n_de_probabilidad#/Distribuciones_de_variable_discreta
- https://www.wikiwand.com/es/Funci%C3%B3n_de_probabilidad
- https://www.pbr-book.org/3ed-2018/contents
- https://www3.nd.edu/~dgalvin1/10120/10120_S16/Topic17_8p4_Galvin_class.pdf
- RTT Shirley.