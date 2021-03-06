# Métodos de Monte Carlo

Como vimos en el capítulo anterior, la clave para conseguir una imagen en nuestro ray tracer es calcular la cantidad de luz en un punto de la escena. Para ello, necesitamos hallar la radiancia en dicha posición mediante la *rendering equation*. Sin embargo, es *muy* difícil resolverla; tanto computacional como analíticamente. Por ello, debemos atacar el problema desde otro punto de vista.

En este capítulo veremos los fundamentos de la **integración de Monte Carlo**, cómo muestrear distribuciones específicas y métodos para afinar el resultado final. Estas técnicas nos permitirán aproximar el valor que toman las integrales mediante una estimación. Utilizando muestreo aleatorio para evaluar puntos de una función, seremos capaces de obtener un resultado suficientemente bueno.

Una de las propiedades que hacen interesantes a este tipo de métodos es la **independencia del ratio de convergencia y la dimensionalidad del integrando**. Sin embargo, conseguir un mejor rendimiento tiene un precio a pagar. Dadas $n$ muestras, la convergencia a la solución correcta tiene un orden de $\mathcal{O}\left(n^{-1/2}\right) = \mathcal{O}\left(\frac{1}{\sqrt{n}}\right)$. Es decir, para reducir el error a la mitad, necesitaríamos 4 veces más muestras. Esto hará que busquemos otras formas de reducir la varianza del estimador.

## Repaso de probabilidad

Antes de comenzar a fondo, necesitaremos unas nociones de variable aleatoria para poder entender la integración de Monte Carlo, por lo que vamos a hacer un breve repaso.

Una **variable aleatoria** $X$ (v.a.) es, esencialmente, una regla que asigna un valor numérico a cada posibilidad de un proceso de azar. Formalmente, es una función definida en un espacio de probabilidad $(\Omega, \mathcal{A}, P)$ asociado a un experimento aleatorio:

$$
X: \Omega \rightarrow \mathbb{R}
$$

A $\Omega$ lo conocemos como espacio muestral (conjunto de todas las posibilidades), $\mathcal{A}$ es una $\sigma$-álgebra de subconjuntos de $\Omega$ que refleja todas las posibilidades de eventos aleatorios, y $P$ es una función probabilidad, que asigna a cada evento una probabilidad.

Una variable aleatoria $X$ puede clasificarse atendiendo a cómo sea su rango $R_X = \set{x \in \mathbb{R}}{\exists \omega \in \Omega \text{ tal que } X(\omega) = x}$: en discreta o continua, dependiendo de si $X$ toma valores en un conjunto numerable o no numerable.

### Variables aleatorias discretas

Las v.a. discretas son aquellas cuyo rango es un conjunto discreto.

Para comprender mejor cómo funcionan, pongamos un ejemplo:
Consideremos un experimento en el que lanzamos dos dados, anotando lo que sale en cada uno. Los posibles valores que toman serán [@galvin-no-date]:

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

La **función masa de probabilidad** nos permite conocer la probabilidad de que $X$ tome un cierto valor $x$. Se denota por $\Prob{X = x}$.

También se suele usar $p_X(x)$ o, directamente $p(x)$, cuando no haya lugar a dudas. Sin embargo, en este trabajo reservaremos este nombre a otro tipo de funciones.

> **Nota**(ción): Cuando $X$ tenga una cierta función masa de probabilidad, escribiremos $X \sim p_X$

En este ejemplo, la probabilidad de que $X$ tome el valor $4$ es

$$
\begin{aligned}
\Prob{X = 4} & = \sum{\small{\text{nº parejas que suman 4}} \cdot \small{\text{probabilidad de que salga la pareja}}} \\
         & = 3 \cdot \frac{1}{36} = \frac{1}{12}
\end{aligned}
$$

Las parejas serían $(1, 3), (2, 2)$ y $(3, 1)$.

Por definición, si el conjunto de valores que puede tomar $X$ es $\{x_1, \dots, x_n\}$, la función masa de probabilidad debe cumplir que

$$
\sum_{i = 1}^{n}{\Prob{X = x_i}} = 1
$$

Siendo $\Prob{X = x_i} \ge 0$, pues la f.m.p. es no negativa.

Muchas veces nos interesará conocer la probabilidad de que $X$ se quede por debajo o igual que un cierto valor $x$ (de hecho, podemos caracterizar distribuciones aleatorias gracias a esto). Para ello, usamos la **función de distribución**:

$$
F_X(x) = \Prob{X \le x} = \sum_{\substack{k \in \mathbb{R} \\ k \le x}}{\Prob{X = k}}
$$

Es una función continua por la derecha y monótona no decreciente. Además, se cumple que $0 \le F_X(x) \le 1$ y $\lim_{x \to -\infty}{F_X} = 0$, $\lim_{x \to \infty}{F_X} = 1$.

En nuestro ejemplo, si consideramos $x = 3$:

$$
\begin{aligned}
F_X(x) & = \sum_{i = 1}^{3}{\Prob{X = i}} = \Prob{X = 1} + \Prob{X = 2} + \Prob{X = 3} \\
       & = \frac{1}{36} + \frac{2}{36} + \frac{3}{36} = \frac{1}{12}
\end{aligned}
$$

### Variables aleatorias continuas

Este tipo de variables aleatorias tienen un rango no numerable; es decir, el conjunto de valores que puede tomar abarca un intervalo de números.

Un ejemplo podría ser la altura de una persona.

Si en las variables aleatorias discretas teníamos funciones masa de probabilidad, aquí definiremos las **funciones de densidad de probabilidad** (o simplemente, funciones de densidad). La idea es la misma: nos permite conocer la probabilidad de que nuestra variable aleatoria tome un cierto valor del espacio muestral.

Es importante mencionar que, aunque *la probabilidad de que la variable aleatoria tome un valor específico* es $0$, ya que nos encontramos en un conjunto no numerable, sí que podemos calcular la probabilidad de que se encuentre entre dos valores. Por tanto, si la función de densidad es $f_X$, entonces

$$
\Prob{a \le X \le b} = \int_{a}^{b}{f_X(x)dx}
$$

La función de densidad tiene dos características importantes:

1. $f_X$ es no negativa; esto es, $f_X(x) \ge 0\ \forall x \in \mathbb{R}$
2. $f_X$ integra uno en todo $\mathbb{R}$:

$$
\int_{-\infty}^{\infty}{f_X(x)\ dx} = 1
$$

Estas dos propiedades caracterizan a una función de densidad; es decir, toda función $f: \mathbb{R} \rightarrow \mathbb{R}$ no negativa e integrable tal que $\int_{-\infty}^{\infty}{f_X(x)\ dx} = 1$ es la función de densidad de alguna variable continua.

> Intuitivamente, podemos ver esta última propiedad como *si acumulamos todos los valores que puede tomar la variable aleatoria, la probabilidad de que te encuentres en el conjunto debe ser 1*

Una de las variables aleatorias que más juego nos darán en el futuro será la **v.a. con distribución uniforme en $[0, 1)$**. La denotaremos $\Xi \sim \mathcal{U}\left([0, 1)\right)$. La probabilidad de que $\Xi$ tome un valor es constante, por lo que podemos definir su función de densidad como

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
F_X(x) = \Prob{X \le x} = \int_{-\infty}^{x}{f_X(t)dt}
$$

Es decir, dado un $x$, ¿cuál sería la probabilidad de que $X$ se quede por debajo de $x$?

Al igual que ocurre en el caso discreto, $F_X$ toma valores entre 0 y 1 ($0 \le F_X(x) \le 1$) y sus límites laterales coinciden con las cotas anteriores ($\lim_{x \to -\infty}{F_X} = 0$, $\lim_{x \to \infty}{F_X} = 1$).

El Teorema Fundamental del Cálculo nos permite relacionar función de distribución y función de densidad directamente:

$$
f_X(x) = \frac{dF_X(x)}{dx}
$$

### Esperanza y varianza de una variable aleatoria

La **esperanza de una variable aleatoria**, denotada $\E{X}$, es una generalización de la media ponderada. Nos informa del *valor esperado* de dicha variable aleatoria.

En el caso de las variables discretas, se define como

$$
\E{X} = \sum_{i}{x_i p_i}
$$

donde $x_i$ son los posibles valores que puede tomar la v.a., y $p_i$ la probabilidad asociada a cada uno de ellos; es decir, $p_i = P[X = x_i]$

Para una variable aleatoria continua real, la esperanza viene dada por

$$
\E{X} = \int_{-\infty}^{\infty}{x f_X(x) dx}
$$

Pongamos un par de ejemplos del cálculo de la esperanza. En el [ejemplo de las variables discretas](#variables-aleatorias-discretas), la esperanza venía dada por

$$
\E{X} = \sum_{i = 2}^{12}{i P[X = i]} = 2\frac{1}{36} + 3 \frac{2}{36} + \dots + 12 \frac{1}{36} = 7
$$

Para variables aleatorias uniformes en $(a, b)$ (es decir, $X \sim \mathcal{U}(a, b)$), la esperanza es

$$
\E{X} = \int_{a}^{b}{x \frac{1}{b - a}dx} = \frac{a + b}{2}
$$

La esperanza tiene unas cuantas propiedades que nos resultarán muy útiles. Estas son:

- **Linealidad**:
  - Si $X, Y$ son dos v.a., $\E{X + Y} = \E{X} + \E{Y}$
  - Si $a$ es una constante, $X$ una v.a., entonces $\E{aX} = a\E{X}$
  - Análogamente, para ciertas $X_1, \dots, X_k$, $\E{\sum_{i = 1}^{k}{X_i}} = \sum_{i = 1}^{k}{\E{X_i}}$

Estas propiedades no necesitan que las variables aleatorias sean independientes, lo cual supondrá un punto clave para las técnicas de Monte Carlo.

Será habitual encontrarnos con el problema de que no conocemos la distribución de una variable aleatoria $Y$. Sin embargo, si encontramos una transformación medible de una variable aleatoria $X$ de forma que obtengamos $Y$ (esto es, $\exists g$ función medible tal que $g(X) = Y$), entonces podemos calcular la esperanza de $Y$ fácilmente. Esta propiedad hará que las variables aleatorias con distribución uniforme adquieran muchísima importancia. Generar números aleatorios en $[0, 1)$ es muy fácil, así [que obtendremos otras vv.aa a partir de $\Xi$](#método-de-la-transformada-inversa).

Otra medida muy útil de una variable aleatoria es **la varianza**. Nos permitirá medir cómo de dispersa es la distribución con respecto a su media. La denotaremos como $Var[X]$, y se define como

$$
Var[X] = \E{(X - \E{X})^2}
$$

Si desarrollamos esta definición, podemos conseguir una expresión algo más agradable:

$$
\begin{aligned}
   Var[X] & = \E{(X - \E{X})^2} = \\
          & = \E{X^2 + \E{X}^2 - 2X\E{X}} = \\
          & = \E{X^2} + \E{X}^2 - 2\E{X}\E{X} = \\
          & = \E{X^2\right] - E\left[X}^2
\end{aligned}
$$

Hemos usado que $\E{\E{X}} = \E{X}$ y la linealidad de la esperanza.

Enunciemos un par de propiedades que tiene, similares a la de la esperanza:

- La varianza saca constantes al cuadrado: $Var[aX] = a^2Var[X]$
- $Var[X + Y] =$ $Var[X] + Var[Y] + 2Cov[X, Y]$, donde $Cov[X, Y]$ es la covarianza de $X$ y $Y$.
  - En el caso en el que $X$ e $Y$ sean incorreladas (es decir, la covarianza es $0$), $Var[X + Y] =$ $Var[X] + Var[Y]$.

La varianza nos será útil a la hora de medir el error cometido por una estimación de Monte Carlo.

### Teoremas importantes

Además de las anteriores propiedades, existen una serie de teoremas esenciales que necesitaremos más adelante:

**Ley del estadístico insconciente** (*Law of the unconscious statistician*, o LOTUS): dada una variable aleatoria $X$ y una función medible $g$, la esperanza de $g(X)$ se puede calcular como

$$
\E{g(X)} = \int_{-\infty}^{\infty}{g(x) f_X(x) dx}
$${#eq:LOTUS}

**Ley (fuerte) de los grandes números**: dada una muestra de $N$ valores $X_1, \dots, X_N$ de una variable aleatoria $X$ con esperanza $\E{X} = \mu$,

$$
\Prob{\lim_{N \to \infty}{\frac{1}{N} \sum_{i = 1}^{N}{X_i}} = \mu } = 1
$$

Usando que $\bar{X}_N = \frac{1}{N} \sum_{i = 1}^{N}{X_i}$, esta ley se suele escribir como

$$
\Prob{\lim_{N \to \infty}{\bar{X}_N} = \mu } = 1
$${#eq:ley_numeros_grandes}

Este teorema es especialmente importante. En esencia, nos dice que cuando repetimos muchas veces un experimento, al promediar los resultados obtendremos una esperanza muy cercana a la esperanza real.

**Teorema Central del Límite (CLT) para variables idénticamente distribuidas** [@mcbook, capítulo 2]: Sean $X_1, \dots, X_N$  muestras aleatorias simples de una variable aleatoria $X$ con esperanza $\E{X} = \mu$ y varianza $Var[X] = \sigma^2$. Sea

$$
Z_N = \frac{\sum_{i = 1}^{N}{X_i - N\mu}}{\sigma \sqrt{N}}
$$

Entonces, la variable aleatoria $Z_N$ converge hacia una función de distribución normal estándar cuando $N$ es suficientemente grande:

$$
\lim_{N \to \infty}{P[Z_N \le z]} = \int_{-\infty}^{z}{\frac{1}{\sqrt{2 \pi}} e^{- \frac{x^2}{2}}dx}
$${#eq:CLT}

### Estimadores

A veces, no podremos conocer de antemano el valor que toma un cierto parámetro de una distribución. Sin embargo, conocemos el tipo de distribución que nuestra variable aleatoria $X$ sigue. Los estimadores nos proporcionarán una forma de calcular el posible valor de esos parámetros a partir de una muestra de $X$.

Sea $X$ una variablea aleatoria con distribución perteneciente a una familia de distribuciones paramétricas $X \sim F \in \set{F(\theta)}{\theta \in \Theta}$. $\Theta$ es el conjunto de valores que puede tomar el parámetro. Buscamos una forma de determinar el valor de $\theta$.

Diremos que $T(X_1, \dots, X_N)$ es **un estimador de $\theta$** si $T$ toma valores en $\Theta$.

A los estimadores de un parámetro los solemos denotar con $\hat{\theta}$.

Como vemos, la definición no es muy restrictiva. Únicamente le estamos pidiendo a la función de la muestra que pueda tomar valores viables para la distribución.

Se dice que un estimador $T(X_1, \dots, X_N)$ es **insesgado** (o centrado en el parámetro $\theta$) si

$$
\E{T(X_1, \dots, X_n)} = \theta\quad \forall \theta \in \Theta
$$

Naturalmente, decimos que un estimador $T(X_1, \dots, X_N)$ está **sesgado** si $\E{T(X_1, \dots, X_N)} \not = \theta$.

## El estimador de Monte Carlo

Tras este breve repaso de probabilidad, estamos en condiciones de definir el estimador de Monte Carlo. Primero, vamos con su versión más sencilla.

### Monte Carlo básico

Los estimadores de Monte Carlo nos permiten hallar la esperanza de una variable aleatoria, digamos, $Y$, sin necesidad de calcular explícitamente su valor. Para ello, tomamos $N$ muestras $Y_1, \dots, Y_N$ de $Y$, cuya  media vale $\mu$. Entonces, el estimador de $\mu$ [@mcbook, capítulo 2] es:

$$
\hat\mu_N = \frac{1}{N} \sum_{i = 1}^{N}{Y_i}
$${#eq:mc_simple}

La intuición del estimador es, esencialmente, la misma que la del teorema central del límite. Lo que buscamos es una forma de calcular el valor promedio de un cierto suceso aleatorio, pero lo único que podemos usar son muestras de su variable aleatoria. Promediando esas muestras, sacamos información de la distribución. En este caso, la media.

En cualquier caso, la existencia de este estimador viene dada por la ley de los grandes números (tanto débil como fuerte [@eq:ley_numeros_grandes]). Si $\mu = \E{Y}$, se tiene que

$$
\lim_{N \to \infty}\Prob{\abs{\hat\mu_N - \mu} \le \varepsilon} = 1 \quad \forall\ \varepsilon > 0
$$

o utilizando la ley de los números grandes,

$$
\lim_{N \to \infty}\Prob{\abs{\hat\mu_N - \mu} = 0} = 1
$$

Haciendo la esperanza de este estimador, vemos que

$$
\begin{aligned}
\E{\hat\mu_N} & = \E{\frac{1}{N} \sum_{i = 1}^{N}{Y_i}\right] = \frac{1}{N} E\left[\sum_{i = 1}^{N}{Y_i}} \\
             & = \frac{1}{N} \sum_{i = 1}^{N}{\E{Y_i}} = \frac{1}{N} \sum_{i = 1}^{N}{\mu} = \\
             & = \mu
\end{aligned}
$$

Por lo que el estimador es insesgado. Además, se tiene que la varianza es

$$
\E{(\hat\mu_N - \mu)^2} = \frac{\sigma^2}{N}
$$

Un ejemplo clásico de estimador de Monte Carlo es calcular el valor de $\pi$. Se puede hallar integrando una función que valga $1$ en el interior de la circunferencia de radio unidad y $0$ en el exterior:

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
\pi \approx \frac{4}{N} \sum_{i = 1}^{N}{f(x_i, y_i)}, \text{  con } (x_i, y_i) \sim \mathcal{U}(\small{[-1, 1] \times [-1, 1]})
$$

### Integración de Monte Carlo

Generalmente nos encontraremos en la situación en la que $Y = f(X)$, donde $X \in S \subset \mathbb{R}^d$ sigue una distribución con función de densidad $p_X(x)$ con media $\mu = \E{X}$, y $f: S \rightarrow \mathbb{R}$.

> **Nota**(ción): nos encontramos ante un caso de *"choque de notaciones"*. Tradicionalmente las funciones de densidad utilizan $f$, pero en transporte de luz se suele reservar esta letra para las BRDF y/u otras funciones genéricas, dejando la letra $p$ para las funciones de densidad.
>
> En este capítulo usaré la notación $p_X$ para dejar claro de que estamos hablando de una función de densidad, manteniendo $f$ para la transformación de la variable aleatoria.

Consideremos el promedio de $N$ muestras de $f(X)$:

$$
\frac{1}{N} \sum_{i = 1}^{N}{f(X_i)}
$$

siendo $X_1 \dots X_N$ idénticamente distribuidas. En ese caso, la esperanza es

$$
\begin{aligned}
\E{\frac{1}{N} \sum_{i = 1}^{N}{f(X_i)} \right] & = E\left[\frac{1}{N} \sum_{i = 1}^{N}{f(X)} } = \\
                                                    & = \frac{1}{N} N \E{f(X)} = \\
                                                    & =  \E{f(X)} = \\
                                                    & = \int_S f(x) p_X(x) dx
\end{aligned}
$$

¡Genial! Esto nos da una forma de **calcular la integral de una función** usando las imágenes de $N$ muestras $f(X_1), \dots, f(X_N)$ de una variable aleatoria $X \sim p_X$. A este estimador de Monte Carlo lo llamaremos $\hat{I}_N$:

$$
\begin{aligned}
\hat{I}_N & = \frac{1}{N} \sum_{i = 1}^{N}{f(X_i)} \\
          \Rightarrow \E{\hat{I}_N} & = \int_S f(x) p_X(x) dx
\end{aligned}
$${#eq:mc_integral}

> **Nota**(ción): si te preguntas por qué lo llamamos $\hat{I}_N$, piensa que queremos calcular la intergal $I = \int_{S}{f(x)p_X(x)dx}$. Para ello, usamos el estimador $\hat{I}$, y marcamos explícitamente que usamos $N$ muestras.

La varianza del estimador se puede calcular fácilmente utilizando las propiedades que vimos en la [sección de la varianza](#esperanza-y-varianza-de-una-variable-aleatoria):

$$
\begin{aligned}
  \Var{\hat{I}_N}
    & = \Var{\frac{1}{N} \sum_{i = 1}^{N}{f(X_i)}} = \\
    & = \frac{1}{N^2} \Var{ \sum_{i = 1}^{N}{f(X_i)}} = \\
    & = \frac{1}{N^2} N \Var{f(X)} = \\
    & = \frac{1}{N} \Var{f(X)}
\end{aligned}
$${#eq:mc_varianza}

Como es natural, el número de muestras que usemos será clave para la proximidad de la estimación. ¿Cómo *de lejos* se queda del valor real de la integral $\E{f(X)}$? Es decir; ¿cómo modifica $N$ la varianza del estimador $\Var{\hat{I}_N}$?

Para comprobarlo, debemos introducir dos nuevos teoremas: la desigualdad de Markov y la desigualdad de Chebyshsev [@metodos-monte-carlo, Introducción].

**Desigualdad de Markov**: Sea $X$ una variable aleatoria que toma valores no negativos, y sea $p_X$ su función de densidad. Entonces, $\forall x > 0$,

$$
\begin{aligned}
\E{X}  &   =  \int_0^x t p_X(t)\ dt + \int_x^\infty t p_X(t)\ dt \ge  \int_x^\infty t p_X(t)\ dt \\
      & \ge  \int_x^\infty x p_X(t)\ dt = x \Prob{X \ge x} \\
      & \Rightarrow \Prob{X \ge x} \le \frac{\E{X}}{x}
\end{aligned}
$${#eq:desigualdad_markov}

**Desigualdad de Chebyshev**: Sea $X$ una variable aleatoria con esperanza $\mu = \E{X}$ y varianza $\sigma^2 = \E{(X - \mu)^2}$. Entonces, aplicando la desigualdad de Markov [@eq:desigualdad_markov] a $D^2 = (X - \mu)^2$ se tiene que

$$
\begin{aligned}
       \Prob{D^2 \ge x^2}         & \le \frac{\sigma^2}{x^2} \\
  \iff \Prob{\abs{X - \mu} \ge x} & \le \frac{\sigma^2}{x^2}
\end{aligned}
$${#eq:desigualdad_chebyshev}

Ahora que tenemos estas dos desigualdades, apliquemos la de Chebyshev a [@eq:mc_integral] con $\sigma^2 = \Var{\hat{I}_N}$, $x^2 = \sigma^2/\varepsilon, \varepsilon > 0$:

$$
\Prob{\abs{\hat{I}_N - \E{\hat{I}_N}} \ge \left(\frac{Var[\hat{I}_N]}{\varepsilon}\right)^{1/2}} \le \varepsilon
$$

Esto nos dice que, usando un número de muestras relativamente grande ($N >> \frac{1}{\varepsilon}$), es prácticamente imposible que el estimador se aleje de $\E{f(X)}$.

La desviación estándar puede calcularse fácilmente a partir de la varianza:

$$
\sqrt{Var[\hat{I}_N]} = \frac{\sqrt{\Var{f(X)}}}{\sqrt{N}}
$${#eq:desviacion_estandar}

así que, como adelantamos al inicio del capítulo, la estimación tiene un error del orden $\mathcal{O}(N^{-1/2})$. Esto nos dice que, para reducir el error a la mitad, debemos tomar 4 veces más muestras.

Es importante destacar la **ausencia del parámetro de la dimensión**. Sabemos que $X \in S \subset \mathbb{R}^d$, pero en ningún momento aparece $d$ en la expresión de la desviación estándar [@eq:desviacion_estandar]. Este hecho es una de las ventajas de la integración de Monte Carlo.


#### Un ejemplo práctico en R

Hagamos un ejemplo práctico para visualizar lo que hemos aprendido en el software estadístico **R**.

Supongamos que queremos integrar la función $f: [0, 1] \rightarrow \mathbb{R}$, $f(x) = 2x^4$. Es decir, queremos calcular

$$
\int_0^1{2x^4\ dx}
$$

El valor de esta integral es $2 \left[\frac{x^5}{5}\right]_0^1 = 2/5 = 0.4$.

Primero, definimos la función $f$:

```r
f <- function(x) {
  2 * x^4 * (x > 0 & x < 1)
}
```

Tomamos N muestras en el intervalo $[0, 1]$ de forma uniforme:

```r
N <- 1000
x <- runif(N)       # x1, ...., xn
f_x <- sapply(x, f) # f(x1), ..., f(xn)
mean(f_x)           # -> 0.3891845
```

Observamos que el valor se queda muy cerca de $0.4$. El error en este caso es $0.4 - 0.3891845 = 0.01081546$.

Es interesante estudiar cómo de rápido converge el estimador al valor de la integral. Con el siguiente código, podemos caclular el error en función del número de muestras $N$:

```r
# Calcular la media y su error
estimacion <- cumsum(f_x) / (1:N)
error <- sqrt(cumsum((f_x - estimacion)^2)) / (1:N)

# Gráfico
plot(1:N, estimacion,
    type = "l",
    ylab = "Aproximación y límites del error (1 - alpha = 0.975)",
    xlab = "Número de simulaciones",
)
z <- qnorm(0.025, lower.tail = FALSE)
lines(estimacion - z * error, col = "blue", lwd = 2, lty = 3)
lines(estimacion + z * error, col = "blue", lwd = 2, lty = 3)
abline(h = 0.4, col = 2)
```

Este código produce la siguiente gráfica:

![Error de la simulación para el estimador de la integral $\int_0^1{2x^4\ dx}$](./img/03/Error%20simulación.png){#fig:error_simulacion}{width=80%}

Se puede ver cómo debemos usar un número considerable de muestras, alrededor de 200, para que el error se mantenga bajo control. Aún así, aumentar el tamaño de $N$ no disminuye necesariamente el error; nos encontramos en una situación de rendimientos decrecientes.

## Técnicas de reducción de varianza

### Muestreo por importancia

Como hemos visto, $\Var{\hat{I}_N}$ depende del número de muestras $N$ y de $\Var{f(X)}$. Aumentar el tamaño de $N$ es una forma fácil de reducir la varianza, pero rápidamente llegaríamos a una situación de retornos reducidos [@PBRT3e, The Monte Carlo Estimator]. ¿Podemos hacer algo con el término $\Var{f(X)}$?

Vamos a jugar con él.

La integral que estamos evaluando ahora mismo es $\int_S{f(x)p_X(x)}dx$, con $p_X$ una función de densidad sobre $S \subset \mathbb{R}^d$ $\Rightarrow p_X(x) = 0\ \forall x \notin S$. Ahora bien, si $q_X$ es otra función de densidad en $\mathbb{R}^d$, entonces [@mcbook, Importance Sampling]:

$$
I = \int_S{f(x)p_X(x)dx} = \int_S{\frac{f(x)p_X(x)}{q_X(x)}q_X(x)dx} = \E{\frac{f(X)p_X(X)}{q_X(X)}}
$$

Esta última esperanza depende de $q_X$. Nuestro objetivo era encontrar $\E{f(X)}$, pero podemos hacerlo tomando un término nuevo para muestrear desde $q_X$ en vez de $p_X$. Al factor $\frac{p_X}{q_X}$ lo llamamos **cociente de probabilidad**, con $q_X$ la **distribución de importancia** y $p_X$ la **distribución nominal**.

No es necesario que $q_X$ sea positiva en todo punto. Con que se cumpla que $q_X(x) > 0$ cuando $f(x)p_X(x) \not = 0$ es suficiente. Es decir, para $Q = \set{x}{q_X(x) > 0 }$, tenemos que $x \in Q$ cuando $f(x)p_X(x) \not = 0$. Así, si $x \in S \cap Q^c \Rightarrow f(X) = 0$, mientras que si $x \in S^c \cap Q \Rightarrow p_X(X) \neq 0$. Entonces,

$$
\begin{aligned}
\E{\frac{f(X)p_X(X)}{q_X(X)}} & = \int_Q{\frac{f(x)p_X(x)}{q_X(x)}q_X(x)dx} = \int_Q{f(x)p_X(x)dx} = \\
                              & = \int_Q{f(x)p_X(x)dx} + \int_{S^c \cap Q}{f(x)p_X(x)dx} - \int_{S \cap Q^c}{f(x)p_X(x)dx} =  \\
                              & = \int_S{f(x)p_X(x)dx} = \\
                              & = \E{f(X)}
\end{aligned}
$$

De esta forma, hemos llegado al **estimador de Monte Carlo por importancia**:

$$
\tilde{I}_N = \frac{1}{N} \sum_{i=1}^N{\frac{f(X_i)p_X(X_i)}{q_X(X_i)}}, \quad X_i \sim q_X
$${#eq:mc_integral_importancia}

> **Nota**(ción): ¡fíjate en el gusanito! $\hat{I}_N$ [@eq:mc_integral] y $\tilde{I}_N$ tienen la misma esperanza, pero son estimadores diferentes.

Vamos a calcular ahora la varianza de este estimador. Sea $\mu = \E{f(X)}$

$$
\begin{aligned}
\Var{\tilde{I}_N} & = \frac{1}{N} \left( \int_Q{\left(\frac{f(x)p_X(x)}{q_X(x)}\right)^2q_X(x) dx} - \mu^2  \right) = \\
                  & = \frac{1}{N} \textcolor{verde-oscurisimo}{\left( \int_Q{\frac{\left(f(x)p_X(x)\right)^2}{q_X(x)} dx} - \mu^2  \right)} = \\
                  & = \frac{\textcolor{verde-oscurisimo}{\sigma^2_q}}{N}
\end{aligned}
$$

La clave de este método reside en escoger una buena distribución de importancia. Puede probarse que la función de densidad que minimiza $\sigma^2_q$ es proporcional a $\abs{f(x)}p_X(x)$ [@mcbook, p.6].

#### Muestreo por importancia en transporte de luz

Esta técnica es especialmente importante en nuestra área de estudio. En transporte de luz, buscamos calcular el valor de la rendering equation [@eq:rendering_equation]. Específicamente, de la integral

$$
\int_{H^2(\mathbf{n})}{BSDF(p, \omega_o \leftarrow \omega_i) L_i(p, \omega_i) \cos\theta_i\ d\omega_i}
$$

que se suele representar como una simple integral sobre un cierto conjunto $\int_S{f(x)dx}$. En la literatura se usa una versión modificada de muestreo por importancia:

$$
\tilde{I}_N = \frac{1}{N} \sum_{i=1}^N{\frac{f(X_i)}{p_X(X_i)}}
$${#eq:mc_integral_tl}

para que, utilizando muestras $X_i \sim p_X$, $\E{\frac{f}{p_X}} = \int_S{\frac{f}{p_X}p_X}$ y así se evalúe directamente la integral de $f$. En cualquiera de los casos, el fundamento teórico es el mismo [@berkeley-mc-lecture].

Esta forma de escribir el estimador nos permite amenizar algunos casos particulares. Por ejemplo, si usamos muestras $X_i$ que sigan una distribución uniforme en $[a, b]$, entonces, su función de densidad es $p_X(x) = \frac{1}{b - a}$. Esto da lugar a

$$
\tilde{I}_N = \frac{b - a}{N} \sum_{i = 1}^{N}{f(X_i)}
$$

**En lo que resta de capítulo, se utilizará indistintamente $\frac{1}{N} \sum_{i=1}^N{\frac{f(X_i)p_X(X_i)}{q_X(X_i)}}$ o $\frac{1}{N} \sum_{i=1}^N{\frac{f(X_i)}{p_X(X_i)}}$ según convenga**. ¡Tenlo en cuenta!

Usando esta expresión, la distribución de importancia $p_X$ que hace decrecer la varianza es aquella proporcional a $f$. Es decir, supongamos que $f \propto p_X$. Esto es, existe un $s$ tal que $f(x) = s p_X(x)$. Como $p_X$ debe integrar uno, podemos calcular el valor de $s$:

$$
\begin{aligned}
  \int_{S}{p_X(x)dx} & = \int_{S}{sf(x)dx} = 1 \quad \iff \\
  s & = \frac{1}{\int_{S}{f(x)dx}}
\end{aligned}
$$

Y entonces, se tendría que

$$
\begin{aligned}
  \Var{\frac{f(X)}{p_X(X)}\right] & = Var\left[\frac{f(X)}{sf(X)}} = \\
  & = \Var{\frac{1}{s}} = \\
  & = 0
\end{aligned}
$$

En la práctica, esto es inviable. El problema que queremos resolver es calcular la integral de $f$. Y para sacar $s$, necesitaríamos el valor de la integral de $f$. ¡Estamos dando vueltas!

Por fortuna, hay algoritmos que son capaces de proporcionar la constante $s$ sin necesidad de calcular la integral. Uno de los más conocidos es **Metropolis-Hastings**, del cual hablaremos posteriormente.

En este trabajo nos centraremos en buscar funciones de densidad $p_X$ que se aproximen a $f$ lo más fielmente posible, dentro del contexto del transporte de luz.

Pongamos un ejemplo de estimador de Monte Carlo para una caja de dimensiones $\small{[x_0, x_1] \times [y_0, y_1] \times [z_0, z_1]}$. Si queremos estimar la integral de la función $f: \mathbb{R}^3 \rightarrow \mathbb{R}$

$$
\int_{x_0}^{x_1} \int_{y_0}^{y_1} \int_{z_0}^{z_1}{f(x, y, z)dx dy dz}
$$

mediante una variable aleatoria $X \sim \mathcal{U}(\small{[x_0, x_1] \times [y_0, y_1] \times [z_0, z_1]})$ con función de densidad $p(x, y, z) = \frac{1}{x_1 - x_0} \frac{1}{y_1 - y_0} \frac{1}{z_1 - z_0}$, tomamos el estimador

$$
\tilde{I}_N = \frac{1}{(x_1 - x_0) \cdot (y_1 - y_0) \cdot (z_1 - z_0)} \sum_{i = 1}^{N}{f(X_i)}
$$

### Muestreo por importancia múltiple

Las técnicas de [muestreo por importancia](#muestreo-por-importancia) nos proporcionan estimadores para una integral de la forma $\int{f(x)dx}$. Sin embargo, es frecuente encontrarse un producto de dos funciones, $\int{f(x)g(x)dx}$. Si tuviéramos una forma de coger muestras para $f$, y otra para $g$, ¿cuál deberíamos usar?

Se puede utilizar un nuevo estimador de Monte Carlo, que viene dado por [@PBRT3e]

$$
\frac{1}{N_f} \sum_{i = 1}^{N_f}{\frac{f(X_i)g(X_i)w_f(X_i)}{p_f(X_i)}} + \frac{1}{N_g} \sum_{j = 1}^{N_g}{\frac{f(Y_j)g(Y_j)w_g(Y_j)}{p_g(Y_j)}}
$$

donde $N_f$ y $N_g$ son el número de muestras tomadas para $f$ y $g$ respectivamente, $p_f, p_g$ las funciones de densidad respectivas y $w_f, w_g$ funciones de peso escogidas tales que la esperanza del estimador sea $\int{f(x)g(x)dx}$.

Estas funciones peso suelen tener en cuenta todas las formas diferentes que hay de generar muestras para $X_i$ e $Y_j$. Por ejemplo, una de las que podemos usar es la heurística de balanceo:

$$
w_s(x) = \frac{N_s p_s(x)}{\sum_{i}{N_i p_i(x)}}
$$

Una modificación de esta es la heurística potencial (*power heuristic*):

$$
w_s(x, \beta) = \frac{\left(N_s p_s(x)\right)^\beta}{\sum_{i}{\left(N_i p_i(x)\right)^\beta}}
$$

la cual reduce la varianza con respecto a la heurística de balanceo. Un valor para $\beta$ habitual es $\beta = 2$.

#### Muestreo por importancia múltiple en transporte de luz

Si queremos evaluar la contribución de luz en un punto teniendo en cuenta la luz directa, la expresión utilizada es

$$
L_o(p, \omega_o) = \int_{S^2}{f(p, \omega_o \leftarrow \omega_i) L_{directa}(p, \omega_i) \cos\theta_i\ d\omega_i}
$$

Si utilizáramos muestreo por importancia basándonos en las distribuciones de $L_{directa}$ o $f$ por separado, algunas de las dos no rendiría especialmente bien. Combinando ambas mediante muestreo por importancia múltiple se conseguiría un mejor resultado.

![Muestreo por importancia múltiple en transporte de luz ilustrado. Fuente: [@robust-monte-carlo, Multiple Importance Sampling]](./img/03/Multiple%20importance%20sampling.png){ #fig:multiple_importance_sampling width=60% }

### Otras técnicas de reducción de varianza en transporte de luz

Hasta ahora, la principal técnica estudiada ha sido muestreo por importancia (sea o no múltiple). Esto no quiere decir que sea la única. Al contrario; esas dos son de las más sencillas que se pueden usar.

En esta sección vamos a ver de forma breve otras formas de reducir la varianza de un estimador, centrádonos específicamente en el contexto de transporte de luz.

#### Ruleta rusa

Un problema habitual en la práctica es saber cuándo terminar la propagación de un rayo. Una solución simple es utilizar un parámetro de profundidad --lo cual hemos implementado en el motor--. Otra opción más eficiente es utilizar el método de **ruleta rusa**.

En esencia, la idea es que se genere un número aleatorio $\xi \in [0, 1)$. Si $\xi < p_i$, el camino del rayo se continúa, pero multiplicando la radiancia acumulada por $L_i(p, \omega_o \leftarrow \omega_i)/p_i$. En otro caso (i.e., si $\xi \ge p_i$), el rayo se descarta. Esto hace que se acepten caminos más fuertes, rechazando aquellas rutas con excesivo ruido.

Más información puede encontrarse en [@PBRT3e, Russian Roulette and Splitting].

#### Next event estimation, o muestreo directo de fuentes de luz

Esta técnica recibe dos nombres. Tradicionalmente, se la conocía como muestreo directo de fuentes de luz, pero en los últimos años ha adoptado el nombre de *next event estimation*. Esencialmente, se trata de utilizar las luces de la escena para calcular la radiancia de un punto.

Podemos dividir la rendering equation [@eq:rendering_equation] en dos sumandos [@carlos-path-tracing]:

$$
L(p, \omega_o) = L_e(p, \omega_o) + \underbrace{L_{directa}(p, \omega_o) + L_{indirecta}(p, \omega_o)}_{\text{La parte integral de la rendering equation}}
$${#eq:next_event_estimation}

siendo $L_e$ la radiancia emitida por la superficie, $L_{directa}$ la radiancia reflejada debida únicamente a la incidente proveniente de las fuentes de iluminación y $L_{indirecta}$ la radiancia indirecta.

$$
\begin{aligned}
  L_{directa}   & = \int_{S^2}{f(p, \omega_o \leftarrow \omega_i) L_{e}(y, -\omega_i) \cos\theta_i\ d\omega_i} \\
  L_{indirecta} & = \int_{S^2}{f(p, \omega_o \leftarrow \omega_i) L_{r}(y, \omega_o \leftarrow \omega_i) \cos\theta_i\ d\omega_i}
\end{aligned}
$${#eq:l_dir_l_indir}

siendo $y$ el primer punto visible desde $p$ en la dirección $\omega_i$ situado en una fuente de luz.

En cada punto de intersección $p$, escogeremos aleatoriamente un punto $y$ en la fuente de luz, y calcularemos $L_{directa}$. Esta integral es fácil de conseguir con las técnicas que ya conocemos. Sin embargo, $L_{indirecta}$ cuesta más trabajo. Al aparecer la radiancia reflejada en el punto $p, L_r(p, \omega_o \leftarrow \omega_i)$, necesitaremos evaluarla de forma recursiva trazando rayos en la escena.

Aunque estamos haciendo más cálculos en cada punto de la cadena de ray trace, al evaluar por separado $L_{directa}$ y $L_{indirecta}$ conseguimos reducir considerablemente la varianza. Por tanto, suponiendo fija la varianza, el coste computacional de un camino es mayor, pero el coste total es más bajo.

Esta técnica requiere conocer si desde el punto $p$ se puede ver $y$ en la fuente de luz. Es decir, ¿hay algún objeto en medio de $p$ e $y$? Para ello, se suele utilizar lo que se conocen como ***shadow rays***. Dispara uno de estos rayos para conocer si está ocluido.

![El muestreo directo de fuentes de luz cambia la forma de calcular la radiancia en un punto, pero mejora considerablemente el ruido de una imagen. Fuente: [@carlos-path-tracing]](./img/03/Next%20event%20estimation.png){ #fig:next_event_estimation width=67% }

Si quieres informarte más sobre esta técnica, puedes leer [@GemsI-IS].

#### Quasi-Monte Carlo

Generalmente, en los estimadores de Monte Carlo se utilizan variables aleatorias distribuidas uniformemente a las que se le aplican transformaciones, pues resulta más sencillo generar un número aleatorio de la primera manera que de la segunda. La idea de los quasi-Monte Carlo es muestrear puntos que, de la manera posible, se extiendan uniformemente en $[0, 1]^d$; evitando así clústeres y zonas vacías [@mcbook, Quasi-Monte Carlo].

Existen varias formas de conseguir esto. Algunas de las más famosas son las secuencias de Sobol, que son computacionalmente caras pero presentan menores discrepancias; o las series de Halton, que son más fáciles de conseguir.

Se puede estudiar el tema en profundidad en [@quasi-monte-carlo]

![Comparativa entre diferentes métodos de quasi-aleatoriedad. Fuente: [@quasi-monte-carlo]](./img/03/Quasi-Monte%20Carlo.png){#fig:quasimontecarlo}

## Escogiendo puntos aleatorios

Una de las partes clave del estimador de Monte Carlo [@eq:mc_integral] es saber escoger la función de densidad $p_X$ correctamente. En esta sección, veremos algunos métodos para conseguir distribuciones específicas partiendo de funciones de densidad sencillas, así como formas de elegir funciones de densidad próximas a $f$. Los dos métodos principales que estudiaremos se han extraído del libro [@PBRT3e, Sampling Random Variables]

### Método de la transformada inversa

Este método nos permite conseguir muestras de cualquier distribución continua a partir de variables aleatorias uniformes, siempre que se conozca la inversa de la función de distribución.

Sea $X$ una variable aleatoria con función de distribución $F_X$[^3]. Queremos buscar una transformación $T: [0, 1] \rightarrow \mathbb{R}$ tal que $T(\xi) \stackrel{\text{\small d}}{=} X$, siendo $\xi$ una v.a. uniformemente distribuida. Para que esto se cumpla, se debe dar

$$
\begin{aligned}
F_X(x) & = P[X < x] = \\
       & = P[T(\xi) < x] = \\
       & = P(\xi < T^{-1}(x)) = \\
       & = T^{-1}(x)
\end{aligned}
$$

Este último paso se debe a que, como $\xi$ es uniforme en $(0, 1)$, $P[\xi < x] = x$. Es decir, hemos obtenido que $F_X$ es la inversa de $T$.

**En resumen**: Para conseguir una muestra de una distribución específica $F_X$, podemos seguir el siguiente algoritmo:

1. Generar un número aleatorio $\xi \sim \mathcal{U}(0, 1)$.
2. Hallar la inversa de la función de distribución deseada $F_X$, denotada $F_X^{-1}(x)$.
3. Calcular $F_X^{-1}(\xi) = X$.

#### Ejemplo práctico de la transformada inversa para $x^2$

Como ejemplo, vamos a muestrear la función $f(x) =  x^2,\ x \in [0, 2]$ [@berkeley-cs184, Monte Carlo Integration].

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

#### Ejemplo práctico del método de la transformada inversa en R

Aunque el ejemplo anterior nos enseña cómo calcular a mano una función sencilla, resulta algo difícil de visualizar qué es lo que está ocurriendo. Por ello, vamos a utilizar el programa **R** para dar otro ejemplo.

Consideremos la distribución exponencial de parámetro $\lambda$, la cual tiene función de densidad y de distribución

$$
\begin{aligned}
  f(x) & = \lambda e^{-\lambda x}, \quad & x \ge 0 \\
  F(x) & =  1 - e^{-\lambda x},    \quad & x \ge 0
\end{aligned}
$$

La función inversa se calcula tal que

$$
\begin{aligned}
  \xi & = F(x)        = 1 - e^{-\lambda x}  & \iff \\
    x & = F^{-1}(\xi) = - \frac{log(1 - \xi)}{\lambda} &
\end{aligned}
$$

Generemos ahora $\xi_1, \dots, \xi_n$ valores en $\mathcal{U}(0, 1)$ y obtengamos las imágenes inversas $X_i = -\frac{log(\xi_i)}{\lambda}$.

Para el ejemplo, fijemos $\lambda = 1.5$ y calculemos los valores para $X_i$:

```R
F <- function(xi, lambda) {
  -log(xi) / lambda
}

N <- 1000
xi <- runif(N)
lambda <- 1.5
x <- F(xi, lambda)
```

Es fácil comprobar que los valores generados se asemejan fielmente a la función de densidad exponencial:

```R
hist(x, freq = FALSE, breaks = 'FD', main = 'Método de la inversa para la exponencial', ylim = c(0, 1.5))
lines(density(x), col = 'blue')
curve(dexp(x, rate = lambda), add = TRUE, col = 2)
```

![Histograma del método de la función inversa.](./img/03/metodo_inversa.png){#fig:metodo_inversa_R width=70%}

### Método del rechazo

El método anterior presenta principalmente dos problemas:

1. No siempre es posible integrar una función para hallar su función de densidad.
2. La inversa de la función de distribución, $F_X^{-1}$ no tiene por qué existir.

Como alternativa, existe el **método del rechazo** o **aceptación-rechazo** (en inglés, *rejection method*) [@mcbook, Non-uniform random variables]. Necesitamos una variable aleatoria $Y$ con función de densidad $p_Y(y)$ de la cual resulta sencillo generar muestras. El objetivo es conseguir muestras de $X \sim p_X$.

Como premisa, debemos buscar un $M \in [1, \infty)$ tal que

$$
p_X(x) \le M p_Y(x) \quad \forall x \in \mathbb{R}
$$

La idea principal es sacar una muestra de $Y$ y aceptarla con probabilidad $p_X/Mp_Y$. En otro caso, desecharla y volver a sacar otra. Para evitar casos absurdos, se puede especificar que $p_Y(y)$ sea 0 cuando $p_X(y)$ lo sea.

El valor más pequeño que podemos tomar para $M$ es $\sup_x{\frac{p_X(x)}{p_Y(x)}}$. De hecho, debemos buscar cotas próximas a este supremo, pues hará que el método sea más eficiente.

En esencia, estamos jugando a los dardos: si la muestra de $y$ que hemos obtenido se queda por debajo de la gráfica de la función $Mp_Y < p_X$, estaremos obteniendo una de $p_X$.

El algoritmo consiste en:

1. Obtener una muestra de $Y \sim p_Y$, denotada $y$, y otra de $\mathcal{U}(0, 1)$, denominada $\xi$.
2. Comprobar si $\xi \le \frac{p_X(y)}{Mp_Y(y)}$.
   1. Si se cumple, se acepta $y$ como muestra de $p_X$
   2. En caso contrario, se rechaza $y$ y se vuelve al paso 1.

Probemos por qué este algoritmo produce una muestra de la función de densidad $p_X$:

Sea $Y \sim p_Y$, $\xi \sim \mathcal{U}(0, 1)$ independientes. Dado $Y = y$, el candidato es aceptado si se cumple que $\xi \le \frac{p_X(y)}{Mp_Y(y)}$. La probabilidad de que $Y$ sea aceptado es:

$$
\int_{-\infty}^{\infty}{p_Y(y) \frac{p_X(y)}{Mp_Y(y)} dy} = \int_{-\infty}^{\infty}{\frac{p_X(y)}{M} dy} = \frac{1}{M}
$$

Ahora bien, para cualquier $x \in \mathbb{R}$

$$
\begin{aligned}
\Prob{X \le x} & = \int_{-\infty}^{x}{
  \underbrace{p_Y(y) \frac{p_X(y)}{Mp_Y(y)} dy}_{\text{Si } y \text{ se acepta}}
  + \underbrace{\left(1 - \frac{1}{M}\right) \Prob{X \le x}}_{\text{Si } y \text{ se rechaza}}
} =  \\
  & = \frac{1}{M} \int_{-\infty}^{x}{p_X(y) dy} + \left(1 - \frac{1}{M}\right) \Prob{X \le x} \\
  & = \int_{-\infty}^{\infty}{p_X(y) dy}
\end{aligned}
$$

Lo que nos dice que $X \sim p_X$ como queríamos comprobar.

#### Ejemplo práctico del método del rechazo en R

De la misma forma que hicimos con el [método de la inversa](#ejemplo-práctico-del-método-de-la-transformada-inversa-en-r), implementaremos un ejemplo gráfico de esta técnica en el software R. En este caso, generaremos valores de una distribución Beta a partir de la uniforme. Es decir, podemos tomar

$$
\begin{aligned}
p_X (x) & = \frac{\Gamma(a + b)}{\Gamma(a) \Gamma(b)} x^{a - 1} (1 - x)^{b - 1}, \quad 0 \le x \le 1; a = 2, b = 6 \\
p_Y (x) & = 1, \quad 0 \le x \le 1
\end{aligned}
$$

Como $M$ podemos tomar

$$
M = \sup_{x}\frac{p_Y(x)}{p_X(x)} = \sup_{x}{p_X(x)}
$$

En la sección anterior dijimos que este algoritmo es "como jugar a los dardos". Pues bien, la figura [@fig:metodo_rechazo_grafica] muestra la diana. El siguiente fragmento de código de R calcula este valor:

```R
a <- 2
b <- 6
resultado <- optimize(
    f = function(x) { dbeta(x, shape1 = a, shape2 = b) },
    maximum = TRUE,
    interval = c(0, 1)
)

# $maximum
# [1] 0.1666692
#
# $objective
# [1] 2.813143

M <- resultado$objective
```

![Podemos ver la función de densidad objetivo y la de densidad reescalada de la que tomamos muestras.](./img/03/metodo_rechazo_grafica.png){#fig:metodo_rechazo_grafica width=60%}

Para resolver el problema planteado, podemos usar el siguiente código:

```R
N <- 1000
x <- double(N)

p_X <- function(x) dbeta(x, shape1 = a, shape2 = b)
p_Y <- function(x) 1

valores_generados <- 0

for (i in 1:N) {
    xi <- runif(1)
    y <- runif(1)
    valores_generados <- valores_generados + 1

    while (xi > p_X(y) / (M * p_Y(y))) {
        # Seguir generando hasta que aceptemos uno
        xi <- runif(1)
        y <- runif(1)
        valores_generados <- valores_generados + 1
    }

    # Aceptar el valor
    x[i] <- y
}
```

El hecho de que exista una posibilidad de que falle evidencia que el algoritmo no es muy eficiente. De hecho, podemos ver una medida de las veces que ha fallado:

```R
valores_generados
# [1] 2906
```

Es decir, para sacar 1000 muestras válidas ha hecho falta generar 2906; casi 3 veces más de las que queríamos.

Finalmente, veamos cómo de buena es la aproximación:

```R
hist(x, freq = FALSE, breaks = 'FD', main = 'Método del rechazo para la distribución Beta(a = 2, b = 6)')
lines(density(x), col = 'blue')
curve(dbeta(x, shape1 = a, shape2 = b), add = TRUE, col = 2)
```

![Histograma del método de rechazo.](./img/03/metodo_rechazo.png){#fig:metodo_rechazo_R width=70%}

## Cadenas de Markov y ecuaciones integrales de Fredholm de tipo 2

A lo largo de este capítulo hemos descrito las técnicas que se utilizan para aproximar los valores de una integral. En esta sección vamos a dar un salto de abstracción, e introduciremos brevemente cómo funcionan las cadenas de Markov, y cómo se pueden usar para aproximar soluciones de un tipo de ecuación integral denominada "ecuación de Fredholm de tipo 2". Nos basaremos en los contenidos de [@montecarlo-fredholm] y [@markov-chains].

Este tipo de ecuaciones resulta de interés puesto que la ecuación del transporte de luz es un caso particular de estas ecuaciones, pero con dominio infinito-dimensional.


### Ecuaciones de Fredholm

Una **ecuación de Fredholm de tipo 2** es una ecuación integral de la forma

$$
u(x) = f(x) + \lambda \int_{a}^{b}{k(x, t)u(t)\ dt}
$${#eq:fredholm_2}

Donde $f(x) \in L_2\left([a, b]\right)$, $k(x, t) \in L_2\left([a, b] \times [a, b]\right)$, el cual se denomina "kernel" de la ecuación de Fredholm; y $u(x) \in L_2\left([a, b]\right)$, la cual es la función que queremos obtener.

El espacio $L_2([a, b])$ se define como

$$
L_2([a, b]) = \set{f \in L([a, b])}{\int_{a}^{b}{\abs{f(t)}^2\ dt} < \infty}
$$

Particularizaremos el caso $a = 0, b = 1$, $\lambda = 1$, $0 \le x \le 1$ en lo que queda de sección.

### Cadenas de Markov

Las cadenas de Markov son un tipo específico de proceso estocástico. Un **proceso estocástico** o aleatorio es una familia de variables aleatorias indexadas por otro conjunto. Si el conjunto es discreto, escribimos la familia como $\left\{X(n)\right\}_{n \in \mathbb{N}}$, mientras que si es continuo, notaremos $\left\{X(t)\right\}_{t \ge 0}$.

La distribución de los procesos estocásticos discretos en tiempo[^4] sobre un conjunto numerable $S$ se viene caracterizado como

$$
\Prob{X(n) = i_n, \dots, X(0) = i_0}
$${#eq:proceso_estocastico_discreto}

con $i_n, \dots, i_0 \in S$, $n \in \mathbb{N}$.

De la definición elemental de la probabilidad condicionada se sigue que

$$
\begin{aligned}
        & \Prob{X(n) = i_n, \dots, X(0) = i_0} = \\
=\      & \Prob{X(n) = i_n \vert X(n - 1) = i_{n - 1}, \dots, X(0) = i_0} \cdot \\
\cdot\  & \Prob{X(n - 1) = i_{n - 1} \vert X(n - 2) = i_{n-2}, \dots, X(0) = i_0} \cdot \\
\cdot\  & \dots \\
\cdot\  & \Prob{X(1) = i_1 \vert X(0) = i_0} \cdot \\
\cdot\  & \Prob{X(0) = i_0}
\end{aligned}
$${#eq:proceso_estocastico_discreto_prob}

Una **cadena de Markov en tiempo discreto** (CMTD) sobre un conjunto numerable $S$ es un proceso estocástico que satisface la **propiedad de Markov**; esto es,

$$
\begin{aligned}
  & \Prob{X(n) = i_n \vert X(n - 1) = i_{n - 1}, \dots, X(0) = i_0} = \\
= & \Prob{X(n) = i_n \vert X(n - 1) = i_{n - 1}}
\end{aligned}
$${#eq:propiedad_markov}

para cualquier $i_n, \dots, i_0 \in S$, $n \in \mathbb{N}$. Esta propiedad nos dice que el estado en el paso $n$ solo depende del estado inmediatamente anterior.

Resultará cómoda la siguiente notación

$$
P_{i, j}(n - 1) = \Prob{X(n) = j \vert X(n - 1) = i}
$$

Se puede simplificar la expresión [@eq:proceso_estocastico_discreto_prob] utilizando la propiedad de Markov [@eq:propiedad_markov] y la anterior notación, de forma que obtenemos

$$
\begin{aligned}
  & \Prob{X(n) = i_n, \dots, X(0) = i_0} =  \\
= & P_{i_{n-1}, i_n}(n - 1) \cdot P_{i_{n-2}, i_{n-1}}(n - 2) \cdot \dots \cdot P_{i_0, i_{1}}(0) \cdot \Prob{X(0) = i_0}
\end{aligned}
$$

A cada $P_{i, j}(n)$ lo llamaremos **probabilidad de transición en un paso en el instante $n$**, mientras que a $\Prob{X(0) = i_0}$ se le denominará la **distribución inicial de probabilidades**, denotado tal que

$$
\phi(i_0) = \Prob{X(0) = i_0}
$$

La distribución inicial de probabilidades proporciona la probabilidad de que la cadena se inicie en cualquier de los estados posibles.

Las **cadenas de Markov discretas homogéneas en tiempo** son aquellas para las que las probabilidades de transición no dependen de la indexación temporal $n \in \mathbb{N}$; es decir, $P_{i, j}(n) = P_{i, j} \quad \forall n \in \mathbb{N}$.

Por lo tanto, para este tipo de cadenas tenemos que

$$
\begin{aligned}
\Prob{X(n) = i_n, \dots, X(0) = i_0} = P_{i_{n-1}, i_n} \cdot \dots \cdot P_{i_0, i_{1}} \cdot \phi(i_0)
\end{aligned}
$${#eq:cadena_markov_homogenea}

De manera análoga, se pueden definir las **cadenas de Markov en tiempo continuo homogéneas** (CMTC homogéneas) sobre un conjunto numerable $S$ como una familia de variables aleatorias $\left\{X(t)\right\}_{t \ge 0}$ sobre un espacio de probabilidad $(\Omega, \mathbb{F}, P)$ de forma que

$$
\begin{aligned}
    & \Prob{X(s+t) = j \vert X(s) = i, X(u)\ 0 \le u \le s} = \\
  = & \Prob{X(s+t) = j \vert X(s) = i}
\end{aligned}
$$

Con $t,s \ge 0$. La distribución de la cadena viene determinada por la distribución inicial

$$
\phi(i) = \Prob{X(0) = i}
$$

y la probabilidad de transición

$$
P_{i, j} = \Prob{X(t + s) = j \vert X(s) = i}
$$

Las cadenas de Markov en tiempo continuo homogéneas cumplen la siguiente propiedad:

$$
\begin{aligned}
P_{i, j}(s + t) & = \sum_{k = 1}^{N}{P_{i, k}(s)P_{k, j}(t)} = \\
                & = \sum_{k = 1}^{N}{P_{i, k}(t)P_{k, j}(s)}
\end{aligned}
$$

donde $1 \le i \le N$, $t, s \ge 0$.

Un algoritmo basado en cadenas de Markov muy famoso es el **muestreo de Metrópolis** [@PBRT3e, Metropolis Light Transport]. Una de sus grandes ventajas es que puede generar muestras que sigan la distribución de valores de una función $f$. Para hacerlo, no necesita nada más que evaluar dicha función, sin necesidad de integrarla. Además, en cada iteración del algoritmo se consiguen muestras usables, a diferencia de otros método como aceptación-rechazo. Tal es su potencia que ha creado un área específica en la informática gráfica, denominado *Metropolis light transport*.

### Resolviendo las ecuaciones de Fredholm utilizando cadenas de Markov continuas

En el trabajo de [@montecarlo-fredholm] los autores muestran una forma de resolver las ecuaciones integrales [@eq:fredholm_2] utilizando cadenas de Markov continuas en el espacio $[0, 1]$.

Consideremos la ecuación [@eq:fredholm_2] en forma de funcional

$$
u(x) = f(x) + (Ku)(x)
$$

o, directamente, omitiendo $x$:

$$
u = f + Ku
$$

siendo $K$ el operador integral de la ecuación original:

$$
(Ku)(x) = \int_{0}^{1}{k(x, t)u(t)dt}
$$

Tenemos que $(Ku)(x)$ será la primera iteración de $u$ con respecto al kernel $k$. La segunda iteración viene dada por

$$
K((Ku))(x) = (K^2u)(x) = \int_0^1 \int_0^1 {k(x, t)k(t, t_1)u(t_1) dtdt_1}
$$

Así, de forma de recursiva, se obtiene que

$$
K(K^{n-1}u)(x) = (K^n u)(x) = \int_0^1 {k(x, t_{n-1})K^{n-1}u(t_{n-1}) dt_{n-1}}
$${#eq:markov_recursividad_kernel}


Si asumimos que

$$
\abs{K} = \sup_{[0, 1]}{\int_0^1{\abs{k(x, t)} dt} < 1}
$$

entonces, se puede resolver la ecuación de Fredholm [@eq:fredholm_2] aplicando la fórmula recursiva

$$
u^{(n+1)} = Ku^{(n)} + f, \quad n = 1, 2, \dots
$$

Si $u^{(0)} = 0$, $K^0 \equiv 0$, entonces, la ecuación anterior se transforma en

$$
u^{(n + 1)} = f+ Kf + \dots + K^nf = \sum_{m = 0}^{n}{K^mf}
$${#eq:recursividad_n_mas_1}

Está claro que

$$
\lim_{n \to \infty} u^{(n)} = \lim_{n \to \infty} \sum_{m = 0}^{n}{K^mf} = u
$$

Cada $u^n$ puede calcularse utilizando una cadena de Markov para hallar su solución. Para ello, consideramos una cadena continua de Markov con probabilidad de transición $P_{i, j}$ y distribución inicial $\phi(i)$, satisfaciendo que

$$
\begin{aligned}
  \int_0^1{P_{i, j}\ dj} &  = 1 \\
  \int_0^1{\phi(i)\ di}  & = 1 \\
\end{aligned}
$${#eq:propiedad_cadenas_markov}

Consideramos ahora una función de pesos $W_m$ para la cadena de Markov, que viene dada por la recursión

$$
\begin{aligned}
W_m & = W_{m-1} \frac{k(x_{m -1}, x_m)}{P_{x_{m-1}, x_m}}, \quad m = 1, 2, \dots \\
W_0 & = 1
\end{aligned}
$${#eq:pesos_gamma_n}

Definamos ahora una variable aleatoria $\Gamma_n(h)$, la cual está asociada al camino aleatorio $x_0 \rightarrow x_1 \rightarrow \dots \rightarrow x_n$:

$$
\Gamma_n(h) = \frac{h(x_0)}{\phi(x_0)} = \sum_{m = 0}^{n}{W_mf(x_m)}
$${#eq:gamma_n}

donde $h(x)$ es una función real.

Consideremos ahora el producto escalar de dos funciones $h(x), u(x)$, el cual se define como

$$
\langle h, u \rangle = \int_0^1{h(x)u(x) dx}
$${#eq:producto_escalar_funcional}

El problema ahora se convierte en encontrar el valor del producto $\langle h, u \rangle$, siendo $u(x)$ la función solución de la ecuación de Fredholm.

**Teorema:** el valor del producto escalar $\langle h, u^{(n + 1)} \rangle$ puede ser estimado mediante la esperanza de la variable aleatoria $\Gamma_n(h)$; es decir,

$$
\E{\Gamma_n(h)} = \langle h, u^{(n + 1)} \rangle
$$

Veamos por qué ocurre esto. Cada camino aleatorio $x_0 \rightarrow x_1 \rightarrow \dots \rightarrow x_n$ tiene una probabilidad de

$$
\Prob{X(0) = x_0, X(1) = x_1, \dots, X(n) = x_n} = \phi(x_0) P_{x_0, x_1} P_{x_1, x_2} \dots P_{x_{n-1}, x_n}
$$

Como la variable aleatoria $\Gamma_n(h)$ está definida a lo largo del camino anterior, se tiene que

$$
\E{\Gamma_n(h)} = \int_0^1 \int_0^1 \dotsc \int_0^1 {\Gamma_n(h) \phi(x_0) P_{x_0, x_1} P_{x_1, x_2} \dots P_{x_{n-1}, x_n} \,dx_0dx_1\dots dx_n}
$$

Usando [@eq:pesos_gamma_n] y [@eq:gamma_n], se puede obtener que

$$
\begin{aligned}
\E{\Gamma_n(h)}
  & = \E{\frac{h(x_0)}{\phi(x_0)} \sum_{m = 0}^{n}{W_mf(x_m)}} = \\
  & = \int_0^1 \int_0^1 \dotsc \int_0^1 {
        h(x_0) \sum_{m = 0}^{n} \Upsilon \Psi f(x_m) \phi(x_m) \,dx_0dx_1\dots dx_n
      }
\end{aligned}
$$

siendo

$$
\begin{aligned}
  \Upsilon & = k(x_0, x_1)  k(x_1, x_2)  \dots k(x_{m-1}, x_m), \\
  \Psi     & = P_{x_{m}, x_{m+1}} \dots P_{x_{n-1}, x_n}
\end{aligned}
$$

Usando [@eq:propiedad_cadenas_markov], la integral anterior puede reescribirse como

$$
\E{\Gamma_n(h)} = \sum_{m = 0}^{n} \int_0^1 \int_0^1 \dotsc \int_0^1 {
        h(x_0) \Upsilon f(x_m) \,dx_0dx_1\dots dx_n
}
$$

Y finalmente, si tenemos en cuenta la n-ésima iteración [@eq:markov_recursividad_kernel], así como [@eq:recursividad_n_mas_1] y la definición de producto escalar $\langle h, u \rangle$, deducimos

$$
\E{\Gamma_n(h)} = \left \langle h(x), \sum_{m = 0}^n {(K^mf)(x)} \right \rangle = \langle h(x), u^{(n+1)}(x) \rangle
$$

Lo cual concluye la prueba.

Este teorema nos dice que $\Gamma_n(h)$ es un estimador insesgado del producto escalar $\langle h(x), u^{(n+1)}(x) \rangle$.

Los autores proponen una forma basada en Monte Carlo basado en la simulación de una cadena de Markov continua para estimar el valor de $\langle h(x), u^{(n+1)}(x) \rangle$, el cual presentaremos a continuación:

Consideremos una cadena de Markov continua con función de probabilidad de transición

$$
P_{x, y} = p(x) \delta(y - x) + (1 - p(x))g(y), \qquad x, y \in [0, 1]
$$

siendo $\delta(y - x)$ la delta de Dirac en $x$, $g(x)$ una función de densidad en $[0, 1]$, y $p(x)$ una función tal que $0 < p(x) < 1$ y $\int_0^1\frac{g(x)}{1 - p(x)} dx \le \infty$.

Para estimar $\langle h, u^{(n+1)}$, segumos el siguiente proceso:

1. Escoger un cierto $n \in \mathbb{N}$ y simular $N$ caminos independientes de longitud $n$,

$$
x_0^{(s)} \rightarrow x_1^{(s)} \rightarrow \dots \rightarrow x_n^{(s)}, \qquad s = 1(1)/N
$$

de la cadena de Markov presentada anteriormente.

2. Definir la variable aleatoria $\Gamma^{(s)}_n(h)$ asociado a este camino,  de forma que

$$
\Gamma^{(s)}_n(h) = \frac{h(x_0)}{p(x_0)} \sum_{m = 0}^n W^{(s)}_m f(x_m)
$$

donde

$$
\begin{aligned}
  W_m & = \frac{
    k\left(x^{(s)}_0, x^{(s)}_1\right) k\left(x^{(s)}_1, x^{(s)}_2\right) \dots k\left(x^{(s)}_{n - 1}, x^{(s)}_n\right)
  }{
    P_{x^{(s)}_0, x^{(s)}_1} P_{x^{(s)}_1, x^{(s)}_2} \dots P_{x^{(s)}_{n - 1}, x^{(s)}_n}
  }, \quad m = 1, 2, \dots \\
  W^{(s)}_0 & = 1
\end{aligned}
$$

3. Evaluar la media muestral

$$
\frac{1}{N} \sum_{s = 1}^N \Gamma^{(s)}_n(h) \approx \langle h, u^{(n+1)} \rangle
$$

El paso (1) puede ser realizado mediante el siguiente algoritmo:

```{=latex}
\begin{algorithm}[H]
\DontPrintSemicolon
\SetAlgoLined
\KwResult{Generación de una cadena de Markov continua mediante Monte Carlo}
\SetKwInOut{Input}{Input}
\SetKwInOut{Output}{Output}
\Input{El número de trayectoria $N$, la longitud de la cadena $n$, la función $p(x)$, la función de densidad de probabilidad $g(x)$ en $[0, 1]$}
\Output{$x_0^{(s)} \rightarrow x_1^{(s)} \rightarrow \dots \rightarrow x_n^{(s)}, \qquad s = 1(1)/N$}
\BlankLine
\For{$s = 1$ \KwTo $N$}{
  \emph{Generar una trayectoria}\;
  Generar $x_0^{(s)}$ desde la densidad $p(x)$\;

  \For{$m = 0$ \KwTo $n-1$}{
    Generar $\xi \sim \mathcal{U}(0, 1)$\;

    \eIf{$p(x_m^{(s)}) > \xi$} {
      $x_{m+1}^{(s)} = x_m^{(s)}$\;
    }{
      Generar $x_{m+1}^{(s)}$ desde la función de densidad de probabilidad $g$\;
    }
  }
}
\caption{Generación de una cadena de Markov continua mediante Monte Carlo}
\end{algorithm}
```

```{=html}
<pre><code>Algoritmo 1
input: el número de trayectoria N, la longitud de la cadena n, la función p(x), la función de densidad de probabilidad g(x) en [0, 1]
output: la cadena de Markov x_0^{(s)} -&gt; x_1^{(s)} -&gt; ... -&gt; x_n^{(s)}

for s = 1 to N do
  // Generar una trayectoria
  generar x_0^{(s)} mediante la función de densidad p(x)

  for m = 1 to n-1 do
    generar un número aleatorio u uniformemente distribuido en (0, 1)

    if p(x_m^{(s)}) &gt; u then
      calcular x_{m+1}^{(s)} = x_m^{(s)}
    else
      generar x_{m+1}^{(s)} desde la función de densidad de prob. g
    fi
  end
end</code></pre>
```

[^3]: En su defecto, si tenemos una función de densidad $p_X$, podemos hallar la función de distribución haciendo $F_X(x) = P[X < x] = \int_{x_{min}}^{x}{p_X(t)dt}$.
[^4]: No tiene por qué ser en tiempo, pero generalmente se considera este tipo de variable.