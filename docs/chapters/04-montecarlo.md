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

Una variable aleatoria $X$ puede clasificarse en discreta o continua, dependiendo de cómo sea su rango $R_X = \left\{ x \in \mathbb{R} \,\middle|\, \exists \omega \in \Omega : X(\omega) = x \right\}$:

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
\begin{aligned}
P(X = 4) & = \sum{\small{\text{nº parejas que suman 4}} \cdot \small{\text{probabilidad de que salga la pareja}}} \\
         & = \frac{1}{36} \cdot 3 = \frac{1}{12}
\end{aligned}
$$

(Las parejas serían $(1, 3), (2, 2), (3, 1)$).

Por definición, si el espacio muestral de $X$ es $x_1, \dots, x_n$, la función masa de probabilidad debe cumplir

$$
\sum_{i = 1}^{n}{P(X = x_i)} = 1
$$

La **función de distribución** de una variable aleatoria $X$ es

$$
F_X(x) = P(X \le x) = \sum_{k = -\infty}^{x}{P(X = x)} = 1
$$

Es una función continua por la derecha y monótona no decreciente. Además, se cumple que $\lim_{x \to -\infty}{F_X} = 0$, $\lim_{x \to \infty}{F_X} = 1$.

En nuestro ejemplo, si consideramos $x = 3$:

$$
\begin{aligned}
F_X(x) & = \sum_{i = 1}^{3}{P(X = i)} = P(X = 1) + P(X = 2) + P(X = 3) \\
       & = \frac{1}{36} + \frac{2}{36} + \frac{3}{36} = \frac{1}{12}
\end{aligned}
$$

### Variables aleatorias continuas

Este tipo de variables aleatorias tienen un rango no numerable; es decir, el conjunto de valores que puede tomar abarca un intervalo de números.

Un ejemplo podría ser la altura de una persona.

Si en las variables aleatorias discretas teníamos funciones masa de probabilidad, aquí definireoms **funciones de densidad de probabilidad** (o simplemente, funciones de densidad). La idea es la misma: nos permite conocer la probabilidad de que nuestra variable aleatoria tome un cierto valor del espacio muestral.

Es importante mencionar que, aunque *la probabilidad de que la variable aleatoria tome un valor específico* es $0$, ya que nos encontramos en un conjunto no numerable, sí que podemos calcular la probabilidad de que se encuentre entre dos valores. Por tanto, si la función de densidad es $f_X$, entonces

$$
P(a \le X \le b) = \int_{a}^{b}{f_X(x)dx}
$$

La función de densidad tiene dos características importantes:

1. $f_X$ es no negativa; esto es, $f_X(x) \ge 0\  \forall x \in \Omega$
2. $f_X$ integra uno en todo el espacio muestral:

$$
\int_{\Omega}{f_X(x)} = 1
$$

Esta última propiedad me gusta entenderla como *si recoges todos los valores que puede tomar la variable aleatoria, la probabilidad de que te encuentres en el conjunto debe ser 1.*. Si nos encontramos en el conjunto de números reales, podemos escribir la integral como $\int_{-\infty}^{\infty}{f_X(x)} = 1$.

Una de las variables aleatorias que más juego nos darán en el futuro será la **v.a. con distribución uniforme en $[0, 1)$**. La denotaremos como $\xi$, y escribiremos $\xi \sim U\left([0, 1)\right)$. La probabilidad de que $\xi$ tome un valor es constante, por lo que podemos definir su función de densidad como

$$
f(\xi) = \left\{  \begin{array}{llc}
                  1 & \text{si } \xi \in [0, 1) \\
                  0 & \text{en otro caso.}
                  \end{array}
         \right.
$$

La probabilidad de $\xi$ tome un valor entre dos elementos $a, b \in [0, 1)$ es

$$
P(\xi \in [a, b]) = \int_{a}^{b}{1dx} = b - a
$$

Como veremos más adelante, saber definir correctamente una función de densidad nos permitirá mejorar el rendimiento del path tracer.

La función de distribución $F_X(x)$ podemos definirla como:

$$
F_X(x) = P(X \le x) = \int_{-\infty}^{x}{f_X(t)dt}
$$

Podemos concebir la función de distribución como *ir acumulando los valores que pueda tomar la variable aleatoria*: dado un cierto punto del espacio, $x$, ¿cuál sería la probabilidad de que cayéramos en algún valor que deja $x$ por debajo?

El Teorema Fundamental del Cálculo nos permite relacionar función de distribución y función de densidad directamente:

$$
f_X(x) = \frac{dF_X(x)}{dx}
$$

### Esperanza y varianza de una variable aleatoria

La esperanza de una variable aleatoria, denotada $E[X]$, es una generalización de la media ponderada. Nos informa del *valor esperado* de una variable aleatoria.

En el caso de las variables discretas, se define como

$$
E[X] = \sum{x_i p_i}
$$

donde $x_i$ son los posibles valores que puede tomar la v.a., y $p_i$ la probabilidad asociada a cada uno de ellos.

En el caso de las variables aleatorias continuas reales, la esperanza viene dada por

$$
E[X] = \int_{-\infty}^{\infty}{x f_X(x) dx}
$$

aunque, generalizando a una v.a. con espacio muestral $\Omega$, la esperanza viene dada por

$$
E[X] = \int_{\Omega}{x f_X(x) dx}
$$

En el ejemplo de las variables discretas, su esperanza venía dada por

$$
E[X] = \sum_{i = 2}^{12}{i \cdot p_i} = 2 \frac{1}{36} + 3 \frac{2}{36} + \dots + 12 \frac{1}{36} = 7
$$

Para variables aleatorias uniformes en $(a, b)$ (es decir, $X \sim U(a, b)$), la esperanza es

$$
E[X] = \int_{a}^{b}{x \cdot \frac{1}{b - a}dx} = \frac{a + b}{2}
$$

La esperanza tiene unas cuantas propiedades que nos resultarán muy útiles. Estas son:

- **Linealidad**:
  - Si $X, Y$ son dos v.a., $E[X + Y] = E[X] + E[Y]$
  - Si $a$ es una constante, $X$ una v.a., entonces $E[aX] = aE[X]$
  - Análogamente, para ciertas $X_1, \dots, X_k$, $E\left[\sum_{i = 1}^{k}{X_i}\right] = \sum_{i = 1}^{k}{E[X_i]}$
  - Estas propiedades no necesitan que las variables aleatorias sean independientes. ¡Este hecho será clave para las técnicas de Monte Carlo!.
- La **Ley del estadístico insconciente** (*Law of the unconscious statistician*, o LOTUS): dada una variable aleatoria $X$ y una función medible $g$, la esperanza de $g(X)$ se puede calcular como

$$
E[g(X)] = \int_{\Omega}{g(x) f_X(x) dx}
$$

**Esta propiedad será clave en nuestro desarrollo**.

Será habitual encontrarnos con el problema de que no conocemos la distribución de una variable aleatoria $Y$. Sin embargo, si encontramos una transformación medible de una variable aleatoria $X$ de forma que obtengamos $Y$ (esto es, $\exists g$ función medible tal que $g(X) = Y$), entonces podemos calcular la esperanza de $Y$ fácilmente. La importancia de la variable aleatoria con distribución uniforme $\xi$ se debe a esta propiedad. Generar números aleatorios en $[0, 1)$ es muy fácil, así que obtendremos otras v.a.s a partir de $\xi$. Pero no adelantemos acontecimientos, así que no variemos tanto.

Hablando de variar: **la varianza de una variable aleatoria** nos permitirá medir cómo de dispersa es la distribución con respecto a su media. La denotaremos como $Var[X]$, y se define como

$$
Var[X] = E\left[(X - E[X])^2\right]
$$

Si desarrollamos esta definición, podemos conseguir una expresión algo más agradable:

$$
\begin{aligned}
   Var[X] & = E\left[(X - E[X])^2\right] = \\
          & = E\left[X^2 + E[X]^2 - 2XE[X]\right] = \\
          & = E\left[X^2\right] + E[X]^2 - 2E[X]E[X] = \\
          & = E\left[X^2\right] - E\left[X\right]^2
\end{aligned}
$$

Hemos usado que $E[E[X]] = E[X]$ y la linealidad de la esperanza.

Enunciemos un par de propiedades que tiene, similares a la de la esperanza:


- La varianza saca constantes al cuadrado: $Var[aX] = a^2Var[X]$
- $\small{Var[X + Y] = Var[X] + Var[Y] + 2Cov[X, Y]}$, donde $Cov[X, Y]$ es la covarianza de $X$ y $Y$.
  - En el caso en el que $X$ e $Y$ sean incorreladas (es decir, la covarianza es $0$), $Var[X + Y] = Var[X] + Var[Y]$.

La varianza nos será útil a la hora de medir el error cometido por una estimación de Monte Carlo.

## El estimador de Monte Carlo

Tras este breve repaso, estamos en condiciones de definir el estimador de Monte Carlo. Primero, vamos con su versión más sencilla.

Los estimadores de Monte Carlo nos permiten hallar la esperanza de una variable aleatoria, digamos, $Y$, sin necesidad de calcular explícitamente su valor. Para ello, tomamos unas cuantas muestras $Y_1, \dots, Y_N$ que sigan la misma distribución que $Y$ con media $\mu$, y consideramos el estimador de $\mu$ [@mcbook]:

$$
\hat\mu_N = \frac{1}{N} \sum_{i = 1}^{N}{Y_i}
$${#eq:mc_simple}

Haciendo la esperanza de este estimador, vemos que

$$
\begin{aligned}
E[\hat\mu_N] & = E\left[\frac{1}{N} \sum_{i = 1}^{N}{Y_i}\right] = \frac{1}{N} E\left[\sum_{i = 1}^{N}{Y_i}\right] \\
             & = \frac{1}{N} \sum_{i = 1}^{N}{E\left[Y_i\right]} = \frac{1}{N} \sum_{i = 1}^{N}{\mu} = \\
             & = \mu
\end{aligned}
$$

A este tipo de estimadores se les llama insesgados.

Generalmente nos encontraremos en la situación en la que $Y = f(X)$, donde $X$ sigue una distribución con función de densidad $p_X(x)$, y $f: S \rightarrow \mathbb{R}$. En ese caso, sabemos que la esperanza de $Y$ se puede calcular como

$$
\mu = E[Y] = E[f(X)] = \int_{S}{f(x)p_X(x)dx}
$$

Lo que estamos buscando es calcular $\int_{S}{f(x)dx}$. Entonces, ¿qué ocurre si intentamos compensar [@eq:mc_simple] con la función de densidad?

$$
\begin{aligned}
& E\left[\frac{1}{N} \sum_{i = 1}^{N}{\frac{f(X_i)}{p_X(X_i)}}\right] = \frac{1}{N} \sum_{i = 1}^{N}{E\left[\frac{f(X_i)}{p_X(X_i)}\right]} = \\
& = \frac{1}{N} \sum_{i = 1}^{N}{\left(\int_{S}{\frac{f(x)}{p_X(x)}p_X(x)dx}\right)} = \\
& = \frac{1}{N} N \int_{S}{f(x)dx} = \\
& = \int_{S}{f(x)dx}
\end{aligned}
$$

¡Genial! Esto nos da una forma de calcular la integral de una función usando muestras de variables aleatorias con cierta distribución. Llamaremos al estimador de Monte Carlo

$$
\hat{F_N} = \frac{1}{N} \sum_{i = 1}^{N}{\frac{f(X_i)}{p_X(X_i)}}
$$

Es importante mencionar que $p_X(x)$ debe ser distinto de 0 cuando $f$ también lo sea.

Podemos particularizar el caso en el que nuestras muestras $X_i$ sigan una distribución uniforme en $[a, b]$. Si eso ocurre, su función de densidad es $p_X(x) = \frac{1}{b - a}$, así que podemos simplificar un poco nuestro estimador:

$$
\hat{F_N} = \frac{b - a}{N} \sum_{i = 1}^{N}{f(X_i)}
$$

Elegir correctamente la función de densidad $p_X$ será clave. Si conseguimos elegirla debidamente, reduciremos mucho el error que genera el estimador. Esto es lo que se conoce como *importance sampling*.

Podemos calcular el error cuadrático medio de la estimación si volvemos al estimador de la media [@eq:mc_simple], $\hat\mu_N$. Para ello, necesitamos la varianza: como $\hat\mu_N$ es insesgado, tenemos que

$$
\begin{aligned}
Var[\hat\mu_N] & = Var\left[\frac{1}{N} \sum_{i = 1}^{N}{Y_i}\right] = \frac{1}{N^2} Var\left[\sum_{i = 1}^{N}{Y_i}\right] = \\
               & = \frac{1}{N^2} \sum_{i = 1}^{N}{Var[Y_i]} = \frac{1}{N^2} N Var[Y] = \\
               & = \frac{Var[Y]}{N}
\end{aligned}
$$

El error cuadrático medio es
$$
\sqrt{Var[\hat\mu_N]} = \sqrt{\frac{Var[Y]}{N}} = \frac{\sqrt{Var[Y]}}{\sqrt{N}}
$$

así que, como adelantamos al inicio del capítulo, la estimación tiene un error del orden $\mathcal{O}(N^{-1/2})$. Esto nos dice que, para reducir el error a la mitad, debemos tomar 4 veces más muestras.

<hr>

- https://www.wikiwand.com/en/Rendering_equation
- https://www.wikiwand.com/es/Variable_aleatoria
- https://www.wikiwand.com/es/Distribuci%C3%B3n_de_probabilidad#/Distribuciones_de_variable_discreta
- https://www.wikiwand.com/es/Funci%C3%B3n_de_probabilidad
- https://www.pbr-book.org/3ed-2018/contents
- https://www.wikiwand.com/en/Expected_value
- https://www3.nd.edu/~dgalvin1/10120/10120_S16/Topic17_8p4_Galvin_class.pdf
- https://www.wikiwand.com/en/Probability_density_function
- RTT Shirley.
- https://artowen.su.domains/mc/
- https://www.wikiwand.com/es/Estimador
- Presentación que no he mirado pero que tiene buena pinta: https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-12-monte-carlo-integration/lec-12-monte-carlo-integration.pdf