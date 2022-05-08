# Métodos de Monte Carlo

Como vimos en el capítulo anterior, la clave para conseguir una imagen en nuestro ray tracer es calcular la cantidad de luz en un punto de la escena. Para ello, necesitamos hallar la radiancia en dicha posición mediante la *rendering equation*. Sin embargo, es *muy* difícil resolverla; tanto computacional como analíticamente. Por ello, debemos atacar el problema desde otro punto de vista.

Las técnicas de Monte Carlo nos permitirán aproximar el valor que toman las integrales mediante una estimación. Utilizando muestreo aleatorio para evaluar puntos de una función, seremos capaces de obtener un resultado suficientemente bueno.

Una de las propiedades que hacen interesantes a este tipo de métodos es la **independencia del ratio de convergencia y la dimensionalidad del integrando**. Sin embargo, conseguir un mejor rendimiento tiene un precio a pagar. Dadas $n$ muestras, la convergencia a la solución correcta tiene un orden de $\mathcal{O}\left(n^{-1/2}\right) = \mathcal{O}\left(\frac{1}{\sqrt{n}}\right)$. Es decir, para reducir el error a la mitad, necesitaríamos 4 veces más muestras.

En este capítulo veremos los fundamentos de la integración de Monte Carlo, cómo muestrear distribuciones específicas y métodos para afinar el resultado final.

## Repaso de probabilidad

Antes de comenzar a fondo, necesitaremos unas nociones de variable aleatoria para poder entender la integración de Monte Carlo, por lo que vamos a hacer un breve repaso.

Una **variable aleatoria** $X$ (v.a.) es, esencialmente, una regla que asigna un valor numérico a cada posibilidad de un proceso de azar. Formalmente, es una función definida en un espacio de probabilidad $(\Omega, \mathcal{A}, P)$ asociado a un experimento aleatorio:

$$
X: \Omega \rightarrow \mathbb{R}
$$

A $\Omega$ lo conocemos como espacio muestral (conjunto de todas las posibilidades), $\mathcal{A}$ es una $\sigma$-álgebra de subconjuntos de $\Omega$ que refleja todas las posibilidades de eventos aleatorios, y $P$ es una función probabilidad, que asigna a cada evento una probabilidad.

Una variable aleatoria $X$ puede clasificarse atendiendo a cómo sea su rango $R_X = \set{x \in \mathbb{R}}{\exists \omega \in \Omega \text{ tal que } X(\omega) = x}$: en discreta o continua.

### Variables aleatorias discretas

Las v.a. discretas son aquellas cuyo rango es un conjunto discreto.

Para comprender mejor cómo funcionan, pongamos un ejemplo:
Consideremos un experimento en el que lanzamos dos dados, anotando lo que sale en cada uno. Los posibles valores que toman serán

$$
\begin{aligned}
\Omega = \{ & (1, 1), (1, 2), (1, 3), (1, 4), (1, 5), (1, 6),  \\
   & (2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6),  \\
   & (3, 1), (3, 2), (3, 3), (3, 4), (3, 5), (3, 6),  \\
   & (4, 1), (4, 2), (4, 3), (4, 4), (4, 5), (4, 6),  \\
   & (5, 1), (5, 2), (5, 3), (5, 4), (5, 5), (5, 6),  \\
   & (6, 1), (6, 2), (6, 3), (6, 4), (6, 5), (6, 6)   \}
\end{aligned}
$$

Cada resultado tiene la misma probabilidad de ocurrir (claro está, si el dado no está trucado). Como hay $36$ posibilidades, la probabilidad de obtener un cierto valor es de $\frac{1}{36}$.

La v.a. $X$ denotará la suma de los valores obtenidos en cada uno. Así, por ejemplo, si al lanzar los dados hemos obtenido $(1, 3)$, $X$ tomará el valor $4$. En total, $X$ puede tomar todos los valores comprendidos entre $2$ y $12$. Cada pareja no está asociada a un único valor de $X$. Por ejemplo, $(1, 2)$ suma lo mismo que $(2, 1)$. Esto nos lleva a preguntarnos... ¿Cuál es la probabilidad de que $X$ adquiera un cierto valor?

La **función masa de probabilidad** nos permite conocer la probabilidad de que $X$ tome un cierto valor $x$. Se denota por $P(X = x)$.

También se suele usar $p_X(x)$ o, directamente $p(x)$, cuando no haya lugar a dudas. Sin embargo, en este trabajo reservaremos este nombre a otro tipo de funciones.

> **Nota**(ción): Cuando $X$ tenga una cierta función masa de probabilidad, escribiremos $X \sim p_X$

En este ejemplo, la probabilidad de que $X$ tome el valor $4$ es

$$
\begin{aligned}
P(X = 4) & = \sum{\small{\text{nº parejas que suman 4}} \cdot \small{\text{probabilidad de que salga la pareja}}} \\
         & = 3 \cdot \frac{1}{36} = \frac{1}{12}
\end{aligned}
$$

Las parejas serían $(1, 3), (2, 2)$ y $(3, 1)$.

Por definición, si el conjunto de valores que puede tomar $X$ es $\{x_1, \dots, x_n\}$, la función masa de probabilidad debe cumplir que

$$
\sum_{i = 1}^{n}{P(X = x_i)} = 1
$$

Muchas veces nos interesará conocer la probabilidad de que $X$ se quede por debajo de un cierto valor $x$ (de hecho, podemos caracterizar distribuciones aleatorias gracias a esto). Para ello, usamos la **función de distribución**:

$$
F_X(x) = P(X \le x) = \sum_{\substack{k \in \mathbb{R} \\ k \le x}}{P(X = k)}
$$

Es una función continua por la derecha y monótona no decreciente. Además, se cumple que $0 \le F_X \le 1(x)$ y $\lim_{x \to -\infty}{F_X} = 0$, $\lim_{x \to \infty}{F_X} = 1$.

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

Si en las variables aleatorias discretas teníamos funciones masa de probabilidad, aquí definiremos las **funciones de densidad de probabilidad** (o simplemente, funciones de densidad). La idea es la misma: nos permite conocer la probabilidad de que nuestra variable aleatoria tome un cierto valor del espacio muestral.

Es importante mencionar que, aunque *la probabilidad de que la variable aleatoria tome un valor específico* es $0$, ya que nos encontramos en un conjunto no numerable, sí que podemos calcular la probabilidad de que se encuentre entre dos valores. Por tanto, si la función de densidad es $f_X$, entonces

$$
P(a \le X \le b) = \int_{a}^{b}{f_X(x)dx}
$$

La función de densidad tiene dos características importantes:

1. $f_X$ es no negativa; esto es, $f_X(x) \ge 0\ \forall x \in \mathbb{R}$
2. $f_X$ integra uno en todo $\mathbb{R}$:

$$
\int_{-\infty}^{\infty}{f_X(x)} = 1
$$

Estas dos propiedades caracterizan a una función de densidad; es decir, toda función $f: \mathbb{R} \rightarrow \mathbb{R}$ no negativa e integrable tal que $\int_{\infty}^{\infty}{f_X(x)} = 1$ es la función de densidad de alguna variable continua.

Intuitivamente, podemos ver esta última propiedad como *si acumulamos todos los valores que puede tomar la variable aleatoria, la probabilidad de que te encuentres en el conjunto debe ser 1*. Si tratamos con un conjunto de números reales, podemos escribir la integral como $\int_{-\infty}^{\infty}{f_X(x)} = 1$.

Una de las variables aleatorias que más juego nos darán en el futuro será la **v.a. con distribución uniforme en $[0, 1)$**. La denotaremos $\Xi \sim U\left([0, 1)\right)$. La probabilidad de que $\xi$ tome un valor es constante, por lo que podemos definir su función de densidad como

$$
f_\Xi(\xi) = \left\{  \begin{array}{llc}
                  1 & \text{si } \xi \in [0, 1) \\
                  0 & \text{en otro caso.}
                  \end{array}
         \right.
$$

La probabilidad de $\Xi$ tome un valor entre dos elementos $a, b \in [0, 1)$ es

$$
P(\Xi \in [a, b]) = \int_{a}^{b}{1dx} = b - a
$$

Como veremos más adelante, definiendo correctamente una función de densidad conseguiremos mejorar el rendimiento del path tracer.

La función de distribución $F_X(x)$ podemos definirla como:

$$
F_X(x) = P(X \le x) = \int_{-\infty}^{x}{f_X(t)dt}
$$

Es decir, dado un $x$, ¿cuál sería la probabilidad de que $X$ se quede por debajo de $x$?

El Teorema Fundamental del Cálculo nos permite relacionar función de distribución y función de densidad directamente:

$$
f_X(x) = \frac{dF_X(x)}{dx}
$$

### Esperanza y varianza de una variable aleatoria

La **esperanza de una variable aleatoria**, denotada $E[X]$, es una generalización de la media ponderada. Nos informa del *valor esperado* de dicha variable aleatoria.

En el caso de las variables discretas, se define como

$$
E[X] = \sum_{i}{x_i p_i}
$$

donde $x_i$ son los posibles valores que puede tomar la v.a., y $p_i$ la probabilidad asociada a cada uno de ellos; es decir, $p_i = P[X = x_i]$

Para una variable aleatoria continua real, la esperanza viene dada por

$$
E[X] = \int_{-\infty}^{\infty}{x f_X(x) dx}
$$

Pongamos un par de ejemplos del cálculo de la esperanza. En el [ejemplo de las variables discretas](#variables-aleatorias-discretas), la esperanza venía dada por

$$
E[X] = \sum_{i = 2}^{12}{i P[X = i]} = 2\frac{1}{36} + 3 \frac{2}{36} + \dots + 12 \frac{1}{36} = 7
$$

Para variables aleatorias uniformes en $(a, b)$ (es decir, $X \sim U(a, b)$), la esperanza es

$$
E[X] = \int_{a}^{b}{x \frac{1}{b - a}dx} = \frac{a + b}{2}
$$

La esperanza tiene unas cuantas propiedades que nos resultarán muy útiles. Estas son:

- **Linealidad**:
  - Si $X, Y$ son dos v.a., $E[X + Y] = E[X] + E[Y]$
  - Si $a$ es una constante, $X$ una v.a., entonces $E[aX] = aE[X]$
  - Análogamente, para ciertas $X_1, \dots, X_k$, $E\left[\sum_{i = 1}^{k}{X_i}\right] = \sum_{i = 1}^{k}{E[X_i]}$
  - Estas propiedades no necesitan que las variables aleatorias sean independientes. Este hecho será clave para las técnicas de Monte Carlo.

Además de las anteriores propiedades, existen una serie de teoremas importantes que necesitaremos más adelante:

**Ley del estadístico insconciente** (*Law of the unconscious statistician*, o LOTUS): dada una variable aleatoria $X$ y una función medible $g$, la esperanza de $g(X)$ se puede calcular como

$$
E[g(X)] = \int_{-\infty}^{\infty}{g(x) f_X(x) dx}
$${#eq:LOTUS}

**Ley (fuerte) de los grandes números**: dada una muestra de $N$ valores $X_1, \dots, X_N$ de una variable aleatoria $X$ con esperanza $E[X] = \mu$,

$$
P\left[\lim_{N \to \infty}{\frac{1}{n} \sum_{i = 1}^{N}{X_i}} = \mu \right] = 1
$$

Usando que $\bar{X}_N = \frac{1}{N} \sum_{i = 1}^{N}{X_i}$, esta ley se suele escribir como

$$
P\left[\lim_{N \to \infty}{\bar{X}_N} = \mu \right] = 1
$${#eq:ley_numeros_grandes}

Este teorema es especialmente importante. En esencia, nos dice que cuando repetimos muchas veces un experimento, al promediar los resultados obtendremos una esperanza muy cercana a la esperanza real.

**Teorema Central del Límite (CLT) para variables idéntidcamente distribuidas** [@mcbook, capítulo 2]: Sean $X_1, \dots, X_N$  muestras aleatorias simples de una variable aleatoria $X$ con esperanza $E[X] = \mu$ y varianza $Var[X] = \sigma^2$. Sea

$$
Z_N = \frac{\sum_{i = 1}^{N}{X_i - N\mu}}{\sigma \sqrt{N}}
$$

Entonces, la variable aleatoria $Z_N$ converge hacia una función de distribución normal estándar cuando $N$ es suficientemente grande:

$$
\lim_{N \to \infty}{P[Z_N \le z]} = \int_{-\infty}^{z}{\frac{1}{\sqrt{2 \pi}} e^{- \frac{x^2}{2}}dx}
$${#eq:CLT}


Será habitual encontrarnos con el problema de que no conocemos la distribución de una variable aleatoria $Y$. Sin embargo, si encontramos una transformación medible de una variable aleatoria $X$ de forma que obtengamos $Y$ (esto es, $\exists g$ función medible tal que $g(X) = Y$), entonces podemos calcular la esperanza de $Y$ fácilmente. Esta propiedad hará que las variables aleatorias con distribución uniforme adquieran muchísima importancia. Generar números aleatorios en $[0, 1)$ es muy fácil, así [que obtendremos otras vv.aa a partir de $\xi$](#método-de-la-transformada-inversa).

Otra medida muy útil de una variable aleatoria es **la varianza**. Nos permitirá medir cómo de dispersa es la distribución con respecto a su media. La denotaremos como $Var[X]$, y se define como

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
- $Var[X + Y] =$ $Var[X] + Var[Y] + 2Cov[X, Y]$, donde $Cov[X, Y]$ es la covarianza de $X$ y $Y$.
  - En el caso en el que $X$ e $Y$ sean incorreladas (es decir, la covarianza es $0$), $Var[X + Y] =$ $Var[X] + Var[Y]$.

La varianza nos será útil a la hora de medir el error cometido por una estimación de Monte Carlo.

### Estimadores

A veces, no podremos conocer de antemano el valor que toma un cierto parámetro de una distribución. Sin embargo, conocemos el tipo de distribución que nuestra variable aleatoria $X$ sigue. Los estimadores nos proporcionarán una forma de calcular el posible valor de esos parámetros a partir de una muestra de $X$.

Sea $X$ una variablea aleatoria con distribución perteneciente a una familia de distribuciones paramétricas $X \sim F \in \set{F(\theta)}{\theta \in \Theta}$. $\Theta$ es el conjunto de valores que puede tomar el parámetro. Buscamos una forma de determinar el valor de $\theta$.

Diremos que $T(X_1, \dots, X_N)$ es **un estimador de $\theta$** si $T$ toma valores en $\Theta$.

A los estimadores de un parámetro los solemos denotar con $\hat{\theta}$.

Como vemos, la definición no es muy restrictiva. Únicamente le estamos pidiendo a la función de la muestra que pueda tomar valores viables para la distribución.

Se dice que un estimador $T(X_1, \dots, X_N)$ es **insesgado** (o centrado en el parámetro $\theta$) si

$$
E[T(X_1, \dots, X_n)] = \theta\quad \forall \theta \in \Theta
$$

Naturalmente, decimos que un estimador $T(X_1, \dots, X_N)$ está **sesgado** si $E[T(X_1, \dots, X_N] \not = \theta$.

## El estimador de Monte Carlo

Tras este breve repaso de probabilidad, estamos en condiciones de definir el estimador de Monte Carlo. Primero, vamos con su versión más sencilla.

### Monte Carlo básico

Los estimadores de Monte Carlo nos permiten hallar la esperanza de una variable aleatoria, digamos, $Y$, sin necesidad de calcular explícitamente su valor. Para ello, tomamos $N$ muestras $Y_1, \dots, Y_N$ de $Y$, cuya  media vale $\mu$. Entonces, el estimador de $\mu$ [@mcbook, capítulo 2] es:

$$
\hat\mu_N = \frac{1}{N} \sum_{i = 1}^{N}{Y_i}
$${#eq:mc_simple}

La intuición del estimador es, esencialmente, la misma que la del teorema central del límite. Lo que buscamos es una forma de calcular la media de un cierto suceso aleatorio, pero lo único que podemos usar son muestras de su variable aleatoria. Promediando esas muestras, sacamos información de la distribución. En este caso, la media.

En cualquier caso, la existencia de este estimador viene dada por la ley de los grandes números (tanto débil como fuerte [@eq:ley_numeros_grandes]). Si $\mu = E[Y]$, se tiene que

$$
\lim_{N \to \infty}P\left[\abs{\hat\mu_N - \mu} \le \varepsilon\right] = 1 \quad \forall\ \varepsilon > 0
$$

o utilizando la ley de los números grandes,

$$
\lim_{N \to \infty}P\left[\abs{\hat\mu_N - \mu} = 0\right] = 1
$$

Haciendo la esperanza de este estimador, vemos que

$$
\begin{aligned}
E[\hat\mu_N] & = E\left[\frac{1}{N} \sum_{i = 1}^{N}{Y_i}\right] = \frac{1}{N} E\left[\sum_{i = 1}^{N}{Y_i}\right] \\
             & = \frac{1}{N} \sum_{i = 1}^{N}{E\left[Y_i\right]} = \frac{1}{N} \sum_{i = 1}^{N}{\mu} = \\
             & = \mu
\end{aligned}
$$

Por lo que el estimador es insesgado. Además, se tiene que la varianza es

$$
E\left[(\hat\mu_N - \mu)^2\right] = \frac{\sigma^2}{N}
$$

### Integración de Monte Carlo

Generalmente nos encontraremos en la situación en la que $Y = f(X)$, donde $X \in S \subset \mathbb{R}^d$ sigue una distribución con función de densidad $p_X(x)$ con media $\mu = E[X]$, y $f: S \rightarrow \mathbb{R}$.

Consideremos el promedio de $N$ muestras de $f(X)$:

$$
\frac{1}{N} \sum_{i = 1}^{N}{f(X_i)} \quad X_i \text{ idénticamente distribuidas}
$$

En ese caso, la esperanza es

$$
\begin{aligned}
E\left[\frac{1}{N} \sum_{i = 1}^{N}{f(X_i)} \right] & = E\left[\frac{1}{N} \sum_{i = 1}^{N}{f(X)} \right] = \\
                                                    & = \frac{1}{N} N E[f(X)] = \\
                                                    & =  E[f(X)] = \\
                                                    & = \int_S f(x) p_X(x) dx
\end{aligned}
$$

¡Genial! Esto nos da una forma de **calcular la integral de una función** usando las imágenes de $N$ muestras $f(X_1), \dots, f(X_N)$ de una variable aleatoria $X \sim p_X$. A este estimador de Monte Carlo lo llamaremos $\hat{I}_N$:

$$
\begin{aligned}
\hat{I}_N & = \frac{1}{N} \sum_{i = 1}^{N}{f(X_i)} \\
          \Rightarrow E\left[\hat{I}_N\right] & = \int_S f(x) p_X(x) dx
\end{aligned}
$${#eq:mc_integral}

Ahora nos preguntamos cómo *de lejos* se queda la estimación del valor real de la integral --la esperanza de $E[f(X)]$--. En otras palabras, ¿cuál es la varianza del estimador $\hat{I}_N$?

Para calcularla, debemos introducir dos nuevos teoremas: la desigualdad de Markov y la desigualdad de Chebyshsev [@metodos-monte-carlo, Introducción].

**Desigualdad de Markov**: Sea $X$ una variable aleatoria que toma valores no negativos, y sea $p_X$ su función de densidad. Entonces, $\forall x > 0$,

$$
\begin{aligned}
E[X]  &   =  \int_0^x t p_X(t) dt + \int_x^\infty t p_X(t) dt \ge  \int_x^\infty t p_X(t) \\
      & \ge  \int_x^\infty x p_X(t) = x P\left[X \ge x\right] \\
      & \Rightarrow P\left[X \ge x\right] \le \frac{E[X]}{x}
\end{aligned}
$${#eq:desigualdad_markov}

**Desigualdad de Chevyshev**: Sea $X$ una variable aleatoria con esperanza $\mu = E[X]$ y varianza $\sigma^2 = E[(X - \mu)^2]$. Entonces, aplicando la desigualdad de Markov [@eq:desigualdad_markov] a $D^2 = (X - \mu)^2$ se tiene que

$$
\begin{aligned}
       P\left[D^2 \ge x^2\right]         & \le \frac{\sigma^2}{x^2} \\
  \iff P\left[\abs{X - \mu} \ge x\right] & \le \frac{\sigma^2}{x^2}
\end{aligned}
$${#eq:desigualdad_chebyshev}

### COMIENZA LA PARTE VIEJA

$$
\mu = E[Y] = E[f(X)] = \int_{S}{f(x)p_X(x)dx}
$$

Lo que estamos buscando es calcular $\int_{S}{f(x)dx}$. Entonces, ¿qué ocurre si intentamos compensar en [@eq:mc_simple] con la función de densidad?

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
\hat{I}_N = \frac{1}{N} \sum_{i = 1}^{N}{\frac{f(X_i)}{p_X(X_i)}}
$$

Es importante mencionar que $p_X(x)$ debe ser distinto de 0 cuando $f$ también lo sea.

> **Nota**(ción): si te preguntas por qué lo llamamos $\hat{I}_N$, piensa que queremos calcular la intergal $I = \int_{S}{f(x)dx}$. Para ello, usamos el estimador $\hat{I}$, y marcamos explícitamente que usamos $N$ muestras.

Podemos particularizar el caso en el que nuestras muestras $X_i$ sigan una distribución uniforme en $[a, b]$. Si eso ocurre, su función de densidad es $p_X(x) = \frac{1}{b - a}$, así que podemos simplificar un poco [@eq:mc_integral]:

$$
\hat{I}_N = \frac{b - a}{N} \sum_{i = 1}^{N}{f(X_i)}
$$

Elegir correctamente la función de densidad $p_X$ será clave. Si conseguimos escogerla debidamente, reduciremos en gran medida el error que genera el estimador. Esto es lo que se conoce como [*importance sampling*](#importance-sampling).

Puesto que la varianza del estimador nos dará información sobre el error que genera, vamos a calcular $Var[\hat{I}_N]$. Para ello, usamos las propiedades que vimos en la [sección anterior](#esperanza-y-varianza-de-una-variable-aleatoria):

$$
\begin{aligned}
  Var[\hat{I}_N]
    & = Var\left[\frac{1}{N} \sum_{i = 1}^{N}{\frac{f(X_i)}{p_X(X_i)}}\right] = \\
    & = \frac{1}{N^2} Var\left[ \sum_{i = 1}^{N}{\frac{f(X_i)}{p_X(X_i)}} \right] = \\
    & = \frac{1}{N^2} N Var\left[\frac{f(X)}{p_X(X)}\right] = \\
    & = \frac{1}{N} Var\left[\frac{f(X)}{p_X(X)}\right]
\end{aligned}
$${#eq:mc_varianza}

es decir, la varianza del estimador es inversamente proporcional al número de muestras $N$.

La desviación estándar es
$$
\sqrt{Var[\hat{I}_N]} = \frac{\sqrt{Var\left[\frac{f(X)}{p_X(X)}\right]}}{\sqrt{N}}
$$

así que, como adelantamos al inicio del capítulo, la estimación tiene un error del orden $\mathcal{O}(N^{-1/2})$. Esto nos dice que, para reducir el error a la mitad, debemos tomar 4 veces más muestras.

Pongamos un ejemplo de estimador de Monte Carlo para una caja de dimensiones $\small{[x_0, x_1] \times [y_0, y_1] \times [z_0, z_1]}$. Si queremos estimar la integral de la función $f: \mathbb{R}^3 \rightarrow \mathbb{R}$

$$
\int_{x_0}^{x_1} \int_{y_0}^{y_1} \int_{z_0}^{z_1}{f(x, y, z)dx dy dz}
$$

mediante una variable aleatoria $X \sim U(\small{[x_0, x_1] \times [y_0, y_1] \times [z_0, z_1]})$ con función de densidad $p(x, y, z) = \frac{1}{x_1 - x_0} \frac{1}{y_1 - y_0} \frac{1}{z_1 - z_0}$, tomamos el estimador

$$
\hat{I}_N = \frac{1}{(x_1 - x_0) \cdot (y_1 - y_0) \cdot (z_1 - z_0)} \sum_{i = 1}^{N}{f(X_i)}
$$

Otro ejemplo clásico de estimador de Monte Carlo es calcular el valor de $\pi$. Se puede hallar integrando una función que valga $1$ en el interior de la circunferencia de radio unidad y $0$ en el exterior:

$$
\begin{aligned}
f = \begin{cases}
      1 & \text{si } x^2 + y^2 \le 1 \\
      0 & \text{en otro caso}
    \end{cases} \Longrightarrow	\pi = \int_{-1}^{1} \int_{-1}^{1}{f(x, y)}\ dxdy
\end{aligned}
$$

Para usar el estimador de [@eq:mc_integral], necesitamos saber la probabilidad de obtener un punto dentro de la circunferencia.

Bien, consideremos que una circunferencia de radio $r$ se encuentra inscrita en un cuadrado. El área de la circunferencia es $\pi r^2$, mientras que la del cuadrado es $(2r)^2 = 4r^2$. Por tanto, la probabilidad de obtener un punto dentro de la circunferencia es $\frac{\pi r^2}{4r^2} = \frac{\pi}{4}$. Podemos tomar $p(x, y) = \frac{1}{4}$, de forma que

$$
\pi \approx \frac{4}{N} \sum_{i = 1}^{N}{f(x_i, y_i)}, \text{  con } (x_i, y_i) \sim U(\small{[-1, 1] \times [-1, 1]})
$$

### TERMINA LA PARTE VIEJA

## Importance sampling

Si recordamos la varianza del estimador de Monte Carlo [@eq:mc_varianza],

$$
Var[\hat{I}_N] = \frac{1}{N} Var\left[\frac{f(X)}{p_X(X)}\right]
$$

podemos ver que depende de dos factores: el número de muestras $N$ y la varianza de $Var\left[\frac{f(X)}{p_X(X)}\right]$. Aumentar el número de muestras haría que la varianza decrezca. Sin embargo, alcanzaríamos un punto de retornos reducidos. Por tanto, vamos a centrarnos ahora en el segundo término.

En esencia, la varianza de $Var\left[\frac{f(X)}{p_X(X)}\right]$ decrecerá cuanto más cercana sea la función de probabilidad $p_X$ a la función $f(X)$.

Supongamos que $f$ es proporcional a $p_X$. Esto es, existe un $s$ tal que $f(x) = s p_X(x)$. Como $p_X$ debe integrar uno, podemos calcular el valor de $s$:

$$
\begin{aligned}
  \int_{S}{p_X(x)dx} & = \int_{S}{sf(x)dx} = 1 \quad \iff \\
  s & = \frac{1}{\int_{S}{f(x)dx}}
\end{aligned}
$$

Y entonces, se tendría que

$$
\begin{aligned}
  Var\left[\frac{f(X)}{p_X(X)}\right] & = Var\left[\frac{f(X)}{sf(X)}\right] = \\
  & = Var\left[\frac{1}{s}\right] = \\
  & = 0
\end{aligned}
$$

En la práctica, esto es inviable. El problema que queremos resolver es calcular la integral de $f$. Y para sacar $s$, necesitaríamos el valor de la integral de $f$. ¡Estamos dando vueltas!

Por fortuna, hay algoritmos que son capaces de proporcionar la constante $s$ sin necesidad de calcular la integral. Uno de los más conocidos es **Metropolis-Hastings**, el cual se basa en cadenas de Markov de Monte Carlo.

En este trabajo nos centraremos en buscar funciones de densidad $p_X$ que se aproximen a $f$ lo más fielmente posible, dentro del contexto del transporte de luz.

## Multiple importance sampling

https://graphics.stanford.edu/courses/cs348b-03/papers/veach-chapter9.pdf

## Escogiendo puntos aleatorios

Una de las partes clave del estimador de Monte Carlo [@eq:mc_integral] es saber escoger la función de densidad $p_X$ correctamente. En esta sección, veremos algunos métodos para conseguir distribuciones específicas partiendo de funciones de densidad sencillas, así como formas de elegir funciones de densidad próximas a $f$.

### Método de la transformada inversa

> **En resumen**: Para conseguir una muestra de una distribución específica $F_X$:
>
> 1. Generar un número aleatorio $\xi \sim U(0, 1)$.
> 2. Hallar la inversa de la función de distribución deseada $F_X$, denotada $F_X^{-1}(x)$.
> 3. Calcular $F_X^{-1}(\xi) = X$.

Este método nos permite conseguir muestras de cualquier distribución continua a partir de variables aleatorias uniformes, siempre que se conozca la inversa de la función de distribución.

Sea $X$ una variable aleatoria con función de distribución $F_X$[^1]. Queremos buscar una transformación $T: [0, 1] \rightarrow \mathbb{R}$ tal que $T(\xi) \stackrel{\text{\small d}}{=} X$, siendo $\xi$ una v.a. uniformemente distribuida. Para que esto se cumpla, se debe dar

$$
\begin{aligned}
F_X(x) & = P[X < x] = \\
       & = P[T(\xi) < x] = \\
       & = P(\xi < T^{-1}(x)) = \\
       & = T^{-1}(x)
\end{aligned}
$$

Este último paso se debe a que, como $\xi$ es uniforme en $(0, 1)$, $P[\xi < x] = x$. Es decir, hemos obtenido que $F_X$ es la inversa de $T$.

> TODO: dibujo similar a [este: p.52](https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-12-monte-carlo-integration/lec-12-monte-carlo-integration.pdf)

Como ejemplo, vamos a muestrear la función $f(x) =  x^2,\ x \in [0, 2]$.

Primero, normalizamos esta función para obtener una función de densidad $p_X(x)$. Es decir, buscamos $p_X(x) = c f(x)$ tal que

$$
\begin{aligned}
1 & = \int_{0}^{2}{p_X(x)dx} = \int_{0}^{2}{c f(x)dx} = c \int_{0}^{2}{f(x)dx} = \\
  & = \Eval{\frac{cx^3}{3}}{2}{3} = \frac{8c}{3} \\
  & \Rightarrow c = \frac{3}{8} \\
  & \Rightarrow p_X(x) = \frac{3x^2}{8}
\end{aligned}
$$

A continuación, integramos la función de densidad para obtener la de distribución $F_X$:

$$
\begin{aligned}
F_X(x) = \int_{0}^{x}{p_X(x)dx} = \int_{0}^{x}{\frac{3x^2}{8}} = \frac{x^3}{8}
\end{aligned}
$$

Solo nos queda conseguir la muestra. Para ello,

$$
\begin{aligned}
\xi & = F_X(x)  = \frac{x^3}{8} \quad \iff \\
x & = \sqrt[3]{8 \xi}
\end{aligned}
$$

Sacando un número aleatorio $\xi$, y pasándolo por la función obtenida, conseguimos un elemento con distribución $F(x)$.

### Método del rechazo

> **En resumen**: Para conseguir una muestra de una variable aleatoria $X$ con función de densidad $p_X$:
>
> 1. Obtener una muestra $y$ de $Y$ , y otra $\xi$ de $U(0, 1)$.
> 2. Comprobar si $\xi < \frac{p_X(y)}{Mp_Y(y)}$. Si es así, aceptarla. Si no, sacar otra muestra.

El método anterior presenta principalmente dos problemas:

1. No siempre es posible integrar una función para hallar su función de densidad.
2. La inversa de la función de distribución, $F_X^{-1}$ no tiene por qué existir.

Como alternativa, podemos usar este método (en inglés, *rejection method*). Para ello, necesitamos una variable aleatoria $Y$ con función de densidad $p_Y(y)$. El objetivo es conseguir una muestra de $X$ con función de densidad $p_X(x)$.

La idea principal es aceptar una muestra de $Y$ con probabilidad $p_X/Mp_Y$, con $1 < M < \infty$. En esencia, estamos jugando a los dardos: si la muestra de $y$ que hemos obtenido se queda por debajo de la gráfica de la función $Mp_Y < p_X$, estaremos obteniendo una de $p_X$.

> TODO dibujo de la gráfica $\frac{p_X(y)}{Mp_Y(y)}$.
>
> ¿Quizás haga falta una demostración también? No estoy satisfecho con este apartado ahora mismo. Necesita trabajo.

El algoritmo consiste en:

1. Obtener una muestra de $Y$, denotada $y$, y otra de $U(0, 1)$, llamada $\xi$.
2. Comprobar si $\xi < \frac{p_X(y)}{Mp_Y(y)}$.
   1. Si se cumple, se acepta $y$ como muestra de $p_X$
   2. En caso contrario, se rechaza $y$ y se vuelve al paso 1.

### Distribuciones unidimensionales

#### Uniformente
#### Linear
#### Gausiana
#### Basada en texturas

### Distribuciones bidimensionales
#### Uniformemente
#### Basada en mappeo de superficies a un hemisferio
#### BRDF
#### Basada en el coseno

## Técnicas de reducción de varianza basadas en muestras

### Ruleta rusa

> **Idea**: A random chance that if the luminance of a ray is less than a given $\varepsilon$ the path will be discarded. Reduces variance by accepting stronger rays more often.

### Next Event Estimation

> **Idea**: Tracing shadow rays to the light source on each bounce to see if you can terminate the current path. This involves shooting a shadow ray towards light sources, if it's occluded, terminate the ray.

### Blue noise

- https://blog.demofox.org/2020/05/16/using-blue-noise-for-raytraced-soft-shadows/
- https://alain.xyz/blog/ray-tracing-filtering

### Forced random sampling

http://drivenbynostalgia.com/ (ctrl + f -> forced random sampling)

### Sampling importance resampling

- https://blog.demofox.org/2022/03/02/sampling-importance-resampling/
- https://research.nvidia.com/sites/default/files/pubs/2020-07_Spatiotemporal-reservoir-resampling/ReSTIR.pdf

### Low discrepancy sampling

<hr>

## Referencias {.unlisted .unnumbered}

[@ShirleyRRT], [@PBRT3e], [@mcbook], [@berkeley-cs184, Monte Carlo Integration],
[@wikipedia-contributors-2021B], [@wikipedia-contributors-2021C], [@wikipedia-contributors-2022H], [@wikipedia-contributors-2022I], [@wikipedia-contributors-2022J], [@galvin-no-date], [@wikipedia-contributors-2022K], [@wikipedia-contributors-2022L], [@wikipedia-contributors-2022M], [@wikipedia-contributors-2022N],


- *(berkeley-cs184)* https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-12-monte-carlo-integration/lec-12-monte-carlo-integration.pdf
- Gems I, p.284.
- https://pellacini.di.uniroma1.it/teaching/graphics17b/lectures/12_pathtracing.pdf
- Apuntes de inferencia estadística (cómo cito este tipo de fuentes??)
- https://www.wikiwand.com/en/Metropolis%E2%80%93Hastings_algorithm

[^1]: En su defecto, si tenemos una función de densidad $f_X$, podemos hallar la función de distribución haciendo $F_X(x) = P[X < x] = \int_{x_{min}}^{x}{f_X(t)dt}$.
