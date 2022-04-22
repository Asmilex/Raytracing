# Integración de Monte Carlo

> TODO: este capítulo seguramente debería ir más tarde. De esa forma, puedo introducir otros conceptos antes. De momento, se queda aquí.

Una de las partes más importante de nuestro ray tracer es saber calcular la cantidad de luz en un punto de la escena. Para ello, necesitaríamos hallar la radianza en dicha posición mediante la *rendering equation*. Sin embargo, es *muy* difícil resolverla; tanto computacional como analíticamente. Por ello, debemos atacar el problema desde otro punto de vista.

Las técnicas de Monte Carlo nos permitirán aproximar el valor que toman las integrales mediante una estimación. Utilizando muestreo aleatorio para evaluar puntos de una función, seremos capaces de obtener un resultado suficientemente bueno.

Una de las propiedades que hacen interesantes a este tipo de métodos es la **independencia del ratio de convergencia y la dimensionalidad del integrando**. Sin embargo, conseguir un mejor rendimiento tiene un precio a pagar. Dadas $n$ muestras, la convergencia a la solución correcta tiene un orden de $\mathcal{O}\left(n^{-1/2}\right) = \mathcal{O}\left(\frac{1}{\sqrt{n}}\right)$. Es decir, para reducir el error a la mitad, necesitaríamos 4 veces más muestras.

En este capítulo veremos el fundamento de la integración de Monte Carlo, cómo muestrear distribuciones específicas y cómo afinar el resultado final.

## Repaso de probabilidad

Necesitaremos unas cuantas nociones de variable aleatoria para poder entender la integración de Monte Carlo, así que vamos a hacer un breve repaso.

Una **variable aleatoria** $X$ (v.a.) es, esencialmente, una regla que asigna un valor numérico a cada posibilidad de proceso de azar. Formalmente, es una función definida en un espacio de probabilidad $(\Omega, \mathcal{A}, P)$ asociado a un exprimento aleatorio:

$$
X: \Omega \rightarrow \mathbb{R}
$$

A $\Omega$ lo conocemos como espacio muestral (conjunto de todas las posibilidades), $\mathcal{A}$ es una $\sigma$-álgebra de subconjuntos de $\Omega$ que refleja todas las posibilidades de eventos aleatorios, y $P$ es una función probabilidad, que asigna a cada evento una probabilidad.

> NOTE: no sé hasta qué punto debería meterme en la definición formal de variable aleatoria. Es una movida tremenda para poca cosa que necesitamos. De momento, voy con lo más interesante.

Una variable aleatoria $X$ puede clasificarse en discreta o continua, dependiendo de cómo sea su rango $R_X = \left\{ x \in \mathbb{R} \,\middle|\, \exists \omega \in \Omega : X(\omega) = x \right\}$:

### Variables aleatorias discretas

Las v.a. discretas son aquellas cuyo rango es un conjunto discreto.

Para comprender mejor cómo funcionan, pongamos un ejemplo:
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

La v.a. $X$ denotará la suma de los valores obtenidos en cada uno. Así, por ejemplo, si al lanzar los dados hemos obtenido $(1, 3)$, $X$ tomará el valor $4$. En total, $X$ puede tomar todos los valores comprendidos entre $2$ y $12$. Este sería el **espacio muestral**. Cada pareja no está asociada a un único valor de $X$. Por ejemplo, $(1, 2)$ suma lo mismo que $(2, 1)$. Esto nos lleva a preguntarnos... ¿Cuál es la probabilidad de que $X$ adquiera un cierto valor?

La **función masa de probabilidad** nos permite conocer la probabilidad de que $X$ tome un cierto valor $x$. Se denota por $P(X = x)$.

En este ejemplo, la probabilidad de que $X$ tome el valor $4$ es

$$
\begin{aligned}
P(X = 4) & = \sum{\small{\text{nº parejas que suman 4}} \cdot \small{\text{probabilidad de que salga la pareja}}} \\
         & = 3 \cdot \frac{1}{36} = \frac{1}{12}
\end{aligned}
$$

Las parejas serían $(1, 3), (2, 2)$ y $(3, 1)$.

Por definición, si el espacio muestral de $X$ es $\Omega = \{x_1, \dots, x_n\}$, la función masa de probabilidad debe cumplir que

$$
\sum_{i = 1}^{n}{P(X = x_i)} = 1
$$

Muchas veces nos interesará conocer la probabilidad de que $X$ se quede por debajo de un cierto valor $x$ (de hecho, podemos caracterizar distribuciones aleatorias gracias a esto). Para ello, usamos la **función de distribución**:

$$
F_X(x) = P(X \le x) = \sum_{\substack{k \in \Omega \\ k \le x}}{P(X = k)}
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

1. $f_X$ es no negativa; esto es, $f_X(x) \ge 0\ \forall x \in \Omega$
2. $f_X$ integra uno en todo el espacio muestral:

$$
\int_{\Omega}{f_X(x)} = 1
$$

Intuitivamente, podemos ver esta última propiedad como *si acumulamos todos los valores que puede tomar la variable aleatoria, la probabilidad de que te encuentres en el conjunto debe ser 1*. Si tratamos con un conjunto de números reales, podemos escribir la integral como $\int_{-\infty}^{\infty}{f_X(x)} = 1$.

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
E[X] = \sum_{x_i \in \Omega}{x_i p_i}
$$

donde $x_i$ son los posibles valores que puede tomar la v.a., y $p_i$ la probabilidad asociada a cada uno de ellos; es decir, $p_i = P[X = x_i]$

Para una variable aleatoria continua real, la esperanza viene dada por

$$
E[X] = \int_{-\infty}^{\infty}{x f_X(x) dx}
$$

aunque, generalizando a una v.a. con espacio muestral $\Omega$, la esperanza se puede generalizar como

$$
E[X] = \int_{\Omega}{x f_X(x) dx}
$$

Pongamos un par de ejemplos del cálculo de la esperanza. En el [ejemplo de las variables discretas](#variables-aleatorias-discretas), la esperanza venía dada por

$$
E[X] = \sum_{i = 2}^{12}{i \cdot P[X = i]} = 2 \cdot \frac{1}{36} + 3 \cdot \frac{2}{36} + \dots + 12 \cdot \frac{1}{36} = 7
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
  - Estas propiedades no necesitan que las variables aleatorias sean independientes. Este hecho será clave para las técnicas de Monte Carlo.
- La **Ley del estadístico insconciente** (*Law of the unconscious statistician*, o LOTUS): dada una variable aleatoria $X$ y una función medible $g$, la esperanza de $g(X)$ se puede calcular como

$$
E[g(X)] = \int_{\Omega}{g(x) f_X(x) dx}
$$

**Esta propiedad será clave en nuestro desarrollo**.

Será habitual encontrarnos con el problema de que no conocemos la distribución de una variable aleatoria $Y$. Sin embargo, si encontramos una transformación medible de una variable aleatoria $X$ de forma que obtengamos $Y$ (esto es, $\exists g$ función medible tal que $g(X) = Y$), entonces podemos calcular la esperanza de $Y$ fácilmente. Esta propiedad hará que las variables aleatorias con distribución uniforme adquieran muchísima importancia. Generar números aleatorios en $[0, 1)$ es muy fácil, así [que obtendremos otras v.a.s a partir de $\xi$](#método-de-la-transformada-inversa).

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

## El estimador de Monte Carlo

Tras este breve repaso de probabilidad, estamos en condiciones de definir el estimador de Monte Carlo. Primero, vamos con su versión más sencilla.

Los estimadores de Monte Carlo nos permiten hallar la esperanza de una variable aleatoria, digamos, $Y$, sin necesidad de calcular explícitamente su valor. Para ello, tomamos unas cuantas muestras $Y_1, \dots, Y_N$ que sigan la misma distribución que $Y$ con media $\mu$. Entonces, consideramos el estimador de $\mu$ [@mcbook]:

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
\hat{F}_N = \frac{1}{N} \sum_{i = 1}^{N}{\frac{f(X_i)}{p_X(X_i)}}
$${#eq:mc_integral}

Es importante mencionar que $p_X(x)$ debe ser distinto de 0 cuando $f$ también lo sea.

Podemos particularizar el caso en el que nuestras muestras $X_i$ sigan una distribución uniforme en $[a, b]$. Si eso ocurre, su función de densidad es $p_X(x) = \frac{1}{b - a}$, así que podemos simplificar un poco [@eq:mc_integral]:

$$
\hat{F}_N = \frac{b - a}{N} \sum_{i = 1}^{N}{f(X_i)}
$$

Elegir correctamente la función de densidad $p_X$ será clave. Si conseguimos escogerla debidamente, reduciremos mucho el error que genera el estimador. Esto es lo que se conoce como *importance sampling*.

> TODO: añadir enlace al capítulo de importance sampling.

Podemos calcular el error cuadrático medio de la estimación si volvemos al estimador de la media $\hat\mu_N$ [@eq:mc_simple]. Para ello, necesitamos la varianza: como $\hat\mu_N$ es insesgado, tenemos que

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

Pongamos un ejemplo de estimador de Monte Carlo para una caja de dimensiones $\small{[x_0, x_1] \times [y_0, y_1] \times [z_0, z_1]}$. Si queremos estimar la integral de la función $f: \mathbb{R}^3 \rightarrow \mathbb{R}$

$$
\int_{x_0}^{x_1} \int_{y_0}^{y_1} \int_{z_0}^{z_1}{f(x, y, z)dx dy dz}
$$

mediante una variable aleatoria $X \sim U(\small{[x_0, x_1] \times [y_0, y_1] \times [z_0, z_1]})$ con función de densidad $p(x, y, z) = \frac{1}{x_1 - x_0} \frac{1}{y_1 - y_0} \frac{1}{z_1 - z_0}$, tomamos el estimador

$$
\hat{F}_N = \frac{1}{(x_1 - x_0) \cdot (y_1 - y_0) \cdot (z_1 - z_0)} \sum_{i = 1}^{N}{f(X_i)}
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

Bien, consideremos que una circunferencia de radio $r$ se encuentra inscrita en un cuadrado. El área de la circunferencia es $\pi r^2$, mientras que la del cuadrado es $(2r)^2 = 4r^2$. Por tanto, la probabilida de obtener un punto dentro de la circunferencia es $\frac{\pi r^2}{4r^2} = \frac{\pi}{4}$. Podemos tomar $p(x, y) = \frac{1}{4}$, de forma que

$$
\pi \approx \frac{4}{N} \sum_{i = 1}^{N}{f(x_i, y_i)}, \text{  con } (x_i, y_i) \sim U(\small{[-1, 1] \times [-1, 1]})
$$

## Escogiendo puntos aleatorios

Una de las partes clave del estimador de Monte Carlo [@eq:mc_integral] es saber escoger la función de densidad $p_X$ correctamente. En esta sección, veremos algunos métodos para conseguir distribuciones específicas partiendo de funciones de densidad sencillas.

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
\xi & = F_X(x)  = \frac{x^3}{8} \iff \\
x & = \sqrt[3]{8 \xi}
\end{aligned}
$$

Sacando un número aleatorio $\xi$, y pasándolo por la función obtenida, conseguimos un elemento con distribución $f(x)$.

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

## *Importance sampling*

Con la llegada de ray tracing en tiempo real surge una obligación por optimizar los pocos rayos que se pueden trazar. Una de las preguntas que nos debemos hacer es *hacia dónde* generamos el rayo.

En esta sección daremos respuesta a este dilema. Estudiaremos cómo las fuentes de luz afectan a la calidad de la imagen final. Veremos técnicas de reducción del error, las cuales nos permitirán acelerar enormemente el cómputo de la escena.



[^1]: En su defecto, si tenemos una función de densidad $f_X$, podemos hallar la función de distribución haciendo $F_X(x) = P[X < x] = \int_{x_{min}}^{x}{f_X(t)dt}$


<hr>

## Referencias {.unlisted .unnumbered}

[@wikipedia-contributors-2021B], [@wikipedia-contributors-2021C], [@wikipedia-contributors-2022H], [@wikipedia-contributors-2022I], [@wikipedia-contributors-2022J], [@galvin-no-date], [@wikipedia-contributors-2022K], [@wikipedia-contributors-2022L], [@wikipedia-contributors-2022M], [@wikipedia-contributors-2022N], [@ShirleyRRT], [@PBRT3e], [@mcbook], [@berkeley-cs184, Monte Carlo Integration]


- *(berkeley-cs184)* https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-12-monte-carlo-integration/lec-12-monte-carlo-integration.pdf
- Gems I, p.284.