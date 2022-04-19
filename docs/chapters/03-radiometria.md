# Transporte de luz

En este capítulo estudiaremos las bases de la radiometría. Esta área de la óptica nos proporcionará una serie de herramientas con las cuales podremos responder a la pregunta *cuánta luz existe en un punto*.

## Unidades radiométricas básicas

> **Nota**: cuando usemos un paréntesis tras una ecuación, dentro denotaremos sus unidades de medida.

Antes de comenzar a trabajar, necesitamos conocer *qué entendemos* por luz. Aunque hay muchas formas de trabajar con ella (a fin de cuentas, todavía seguimos discutiendo sobre *qué es* exactamente la luz [^2]), nosotros nos quedaremos con algunas pinceladas de la cuántica. Nos será suficiente quedarnos con la concepción de fotón. Una fuente de iluminación emite una serie de fotones. Estos fotones tienen<sup>[@ShirleyRRT]</sup> una posición, una dirección de propagación y una longitud de onda $\lambda$. Un fotón también tiene asociado una velocidad $c$ que depende del índice de refracción del medio, $n$.

La unidad de medida de $\lambda$ es el nanómetro ($\text{nm}$). También nos vendrá bien definir una frecuencia, $f$. Su utilidad viene del hecho de que, cuando la luz cambia de medio al propagarse, la frecuencia se mantiene constante.

$$
f = \frac{c}{\lambda}
$$

Un fotón tiene asociada una carga de energía, denotada por $Q$:

$$
Q = hf = \frac{hc}{\lambda} (\text{J})
$$

donde $h = 6.62607004 \times 10^{-34} \text{J} \cdot \text{s}$ es la constante de Plank y $c = 299 792 458 \text{m/s}$ la velocidad de la luz.

En realidad, **todas estas cantidades deberían tener un subíndice $\lambda$**, puesto que dependen de la longitud de onda. La energía de un fotón $Q$, por ejemplo, debería denotarse $Q_\lambda$. Sin embargo, en la literatura de informática gráfica, **se ha optado por omitirla**. ¡Tenlo en cuenta a partir de aquí!

### Potencia

A partir de la energía anterior, podemos estimar *la tasa de producción de energía*. A esta tasa la llamaremos<sup>[@PBRT3e]</sup> **potencia**, o **flujo radiante** $\Phi$. Esta medida nos resultará más útil que la energía total, puesto que nos permite estimar la energía en un instante:

$$
\Phi = \lim_{\Delta t \to 0}{\frac{\Delta Q}{\Delta t}} = \frac{dQ}{dt} (J/s)
$$

Su unidad es julios por segundo, comúnmente denotado vatio (*watts*, $\text{W}$). También se utiliza el lumen.
Podemos encontrar la energía total en un periodo de tiempo $[t_0, t_1]$ integrando el flujo radiante:

$$
Q = \int_{t_0}^{t_1}{\Phi(t)dt}
$$

### Irradiancia

La **irradiancia** o **radiancia emitida** es el flujo radiante que recibe una superficie. Dada un área $A$, se define como

$$
E = \frac{\Phi}{A} (\text{W/m}^2)
$$


![La irradiancia es la potencia por metro cuadrado incidente en una superficie. Es proporcional al coseno del ángulo entre la dirección de la luz y la normal a la superficie.](./img/03/Irradiancia.png)

Ahora que tenemos la potencia emitida en una cierta área, nos surge una pregunta: *¿y en un cierto punto $p$?*. Tomando límites en la expresión anterior, encontramos la respuesta:

$$
E(p) = \lim_{\Delta A \to 0}{\frac{\Delta \Phi}{\Delta A}} = \frac{d\Phi}{dA} (\text{W/m}^2)
$$

De la misma manera que con la potencia, integrando $E(p)$ podemos obtener el flujo radiante:

$$
\Phi = \int_{A}{E(p)dp}
$$

El principal problema de la irradiancia es que *no nos dice nada sobre las direcciones* desde las que ha llegado la luz.

### Ángulos sólidos

Con estas tres unidades básicas, nos surge una pregunta muy natural: *¿cómo mido cuánta luz llega a una superficie?*

Para responder a esta pregunta, necesitaremos los **ángulos sólidos**. Son la extensión de los **ángulos planares**, en dos dimensiones.

Ilustremos el sentido de estos ángulos: imaginemos que tenemos un cierto objeto en dos dimensiones delante de nosotros, a una distancia desconocida. ¿Sabríamos cuál es su tamaño, solo con esta información? Es más, si entrara otro objeto en la escena, ¿podríamos distinguir cuál de ellos es más grande?

Parece difícil responder a estas preguntas. Sin embargo, sí que podemos determinar *cómo de grandes nos parecen* desde nuestro punto de vista. Para ello, describimos una circunferencia de radio $r$ alrededor nuestra. Si trazamos un par de líneas desde nuestra posición a las partes más alejadas de este objeto, y las cortamos con nuestra circunferencia, obtendremos un par de puntos inscritos en ella. Pues bien, al arco que encapsulan dichos puntos le vamos a hacer corresponder un cierto ángulo: el ángulo planar.

![La idea intuitiva de un ángulo planar](./img/03/Ángulo%20planar.png)

Llevando esta idea a las tres dimensiones es como conseguimos el concepto de **ángulo sólido**. Si en dos dimensiones teníamos una circunferencia, aquí tendremos una esfera. Cuando generemos las rectas proyectantes hacia el volumen, a diferencia de los ángulos planares, se inscribirá un área en la esfera. La razón entre dicha área $A$ y el cuadrado del radio $r$ nos dará un ángulo sólido:

$$
\omega = \frac{A}{r^2} \text{(sr)}
$$

![Un ángulo sólido es la razón entre el área proyectada y el cuadrado del radio](./img/03/Ángulo%20sólido.png)

Los denotaremos por $\omega$. En física se suele usar $\Omega$, pero aquí optaremos por la minúscula. Su unidad de medida es el estereorradián ($\text{sr}$).  Se tiene que $\omega \in [0, 4\pi]$. Si $2 \pi$ radianes corresponden a la circunferencia completa, para la esfera se tiene que $4 \pi$ esteorradianes cubren toda la superficie de esta. Se tiene también que $2\pi \text{sr}$ cubren un hemisferio. Además, un esteorradián corresponde a una superficie con área $r^2$: $1 \text{sr} = \frac{r^2}{r^2}$.


De vez en cuando, usaremos $\omega$ **un vector dirección unitario en la esfera**.

![Como de costumbre, hay un xkcd relevante. [(Fuente)](https://xkcd.com/1276/)](./img/03/xkcd_1276.png)

Usualmente emplearemos coordenadas esféricas cuando trabajemos con ellos, dado que resulta más cómodo.

$$
\begin{aligned}
    \begin{cases}
        x = \sin\theta\cos\theta \\
        y = \sin\theta\sin\theta \\
        z = \cos\theta
    \end{cases}
\end{aligned}
$$

A $\theta$ se le denomina ángulo polar, mientras que a $\phi$ se le llama acimut. Imaginémonos un punto en la esfera de radio $r$ ubicado en una posición $(r, \theta, \phi)$. Queremos calcular un área chiquitita $dA_h$, de forma que el ángulo sólido asociado a dicha área debe ser $d\omega$. Así, $d\omega = \frac{dA_h}{r^2}$. Si proyectamos el área, obtenemos $d\theta$ y $d\phi$: pequeños cambios en los ángulos que nos generan nuestra pequeña área.

$dA_h$ debe tener dos lados $lado_1$ y $lado_2$. Podemos hallar $lado_1$ si lo trasladamos al eje $z$ de nuevo. Así, $lado_1 = r \sin d\theta$. De la misma manera, $lado_2 = r d\theta$.

> TODO: foto que explique todo esto, porque si no, no hay quien se entere. Quizás me sirva la de https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf, p.16 siempre que adapte $\phi$.

Poniendo estos valores en $d\omega$:

$$
\begin{aligned}
d\omega & = \frac{dA_h}{r^2} = \frac{lado_1 lado_2}{r^2} = \\
        & = \frac{r \sin\theta\ d\phi\ r\ d\theta}{r^2} = \\
        & = \sin\theta\ d\theta\ d\phi
\end{aligned}
$${#eq:d_omega}

¡Genial! Acabamos de añadir un recurso muy potente a nuestro inventario. Esta expresión nos permitirá convertir integrales sobre ángulos sólidos en integrales sobre ángulos esféricos.

### Intensidad radiante

Los ángulos sólidos nos proporcionan una variedad de herramientas nuevas considerable. Gracias a ellos, podemos desarrollar algunos conceptos nuevos. Uno de ellos es la **intensidad radiante**.

Imaginémonos un pequeñito punto de luz encerrado en una esfera, el cual emite fotones en todas direcciones. Nos gustaría medir cuánta energía pasa por la esfera. Podríamos entonces definir

$$
I = \frac{\Phi}{4\pi} \text{(W/sr)}
$$

Otra unidad de medida es el lumen por esterorradián, $\text{(lm/sr)}$. La anterior definición mide cuántos fotones pasan por toda la esfera. ¿Qué ocurre si *cerramos* el ángulo, restringiéndonos así a un área muy pequeña de la esfera?

$$
I = \lim_{\Delta\omega \to 0}{\frac{\Delta\Phi}{\Delta\omega}} = \frac{d\Phi}{d\omega}
$$

De la misma manera que con los conceptos anteriores, podemos volver a la potencia integrando sobre un conjunto de direcciones:

$$
\Phi = \int_{\Omega}{I(\omega)d\omega}
$$

### Radiancia

Finalmente, llegamos al concepto más importante. La **radiancia espectral** (o radiancia a secas[^3]) es una extensión de la radiancia emitida teniendo en cuenta la dirección:

$$
L(p, \omega) = \lim_{\Delta\omega \to 0}{\frac{\Delta E_\omega(p)}{\Delta\omega}} = \frac{dE_\omega(p)}{d\omega}
$$

siendo $E_\omega(p)$ la radiancia emitida a la superficie perpendicular a $\omega$.

> TODO: foto como la de https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf, página 10.

Podemos dar otra expresión de la radiancia en términos del flujo:

$$
L(p, \omega) = \frac{d^2\Phi(p, \omega)}{d\omega\ dA^\bot} = \frac{d^2\Phi(p, \omega)}{d\omega\ dA\ \cos\theta}
$${#eq:radiancia_flujo}

donde $dA^\bot$ es el área proyectada por $dA$ en una hipotética superficie perpendicular a $\omega$:

> TODO: figura similar a pbr figura 5.10 https://www.pbr-book.org/3ed-2018/Color_and_Radiometry/Radiometry

Cuando un rayo impacta en una superficie, $L$ puede tomar valores muy diferentes en un lado y otro de dicha superficie. Por ejemplo, si nos imaginamos un espejo, el valor un poco por encima y un poco por debajo de un punto del espejo es muy diferente. Para solucionarlo, podemos tomar límites para distinguir a ambos lados:

$$
\begin{aligned}
L^+(p, \omega) = \lim_{t \to 0^+}{L(p + t\mathbf{n_p}, \omega)} \\
L^-(p, \omega) = \lim_{t \to 0^-}{L(p + t\mathbf{n_p}, \omega)}
\end{aligned}
$${#eq:L_limit}

donde $\mathbf{n_p}$ es la normal en el punto $p$.

Otra forma de solucionarlo (y preferible, puesto que simplifica entender lo que ocurre) es distinguir entre la radiancia que llega a un punto --la incidente--, y la saliente.

La primera se llamará $L_i(p, \omega)$, mientras que la segunda será $L_o(p, \omega)$. Es importante destacar que $\omega$ apunta *hacia fuera* de la superficie. Quizás es contraintuitivo en $L_i$, puesto que $-\omega$ apunta *hacia* la superficie. Depende del autor se utiliza una concepción u otra.

> **Nota**(ción): a $L_o$ también se le conoce como la radiancia reflejada. Por eso, algunas veces aparece como $L_r$ en algunas fuentes.

Utilizando esta notación y usando [{@eq:L_limit}], podemos escribir $L_i$ y $L_o$ como

$$
\begin{aligned}
    L_i(p, \omega) & =
        \begin{cases}
            L^+(p, -\omega) & \text{si } \omega \cdot \mathbf{n_p} > 0 \\
            L^-(p, -\omega) & \text{si } \omega \cdot \mathbf{n_p} < 0
        \end{cases} \\
    L_o(p, \omega) & =
        \begin{cases}
            L^+(p, \omega) & \text{si } \omega \cdot \mathbf{n_p} > 0 \\
            L^-(p, \omega) & \text{si } \omega \cdot \mathbf{n_p} < 0
        \end{cases}
\end{aligned}
$$

Hacemos esta distinción porque, a fin de cuentas, necesitamos distinguir entre los fotones que llegan a la superficie y los que salen.

> TODO: https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf, p.36

Una propiedad a tener en cuenta es que, si cogemos un punto $p$ del espacio donde no existe ninguna superifcie, $L_o(p, \omega) = L_i(p, -\omega) = L(p, \omega)$

La importancia de la radiancia se debe a un par de propiedades:

La primera de ellas es que, dado $L$, podemos calcular cualquier otra unidad básica mediante integración. Además, **su valor se mantiene constante en rayos que viajan en el vacío en línea recta** [@Pellacini-Marschner-2017]. Esto último hace que resulte muy natural usarla en un ray tracer.

Veamos por qué ocurre esto:

> TODO: https://pellacini.di.uniroma1.it/teaching/graphics17b/lectures/12_pathtracing.pdf, página 18.

Consideremos dos superficies ortogonales entre sí, $S_1$ y $S_2$ separadas una distancia $r$. Debido a la conservación de la energía, cualquier fotón que salga de una superficie y se encuentre bajo el ángulo sólido de la otra debe llegar impactar en dicha superficie opuesta.

Por tanto:

$$
d^2\Phi_1 = d^2\Phi_2
$$

Sustituyendo en la expresión de la radiancia [@eq:radiancia_flujo], y teniendo en cuenta que son ortogonales (lo que nos dice que $\cos\theta = 1$):

$$
L_1 d\omega_1 dA_1 = L_2 d\omega_2 dA_2
$$

Por construcción, podemos cambiar los ángulos sólidos:

$$
L_1 \frac{dA_2}{r^2} dA_1 = L_2 \frac{dA_1}{r^2} dA_2
$$

Lo que finalmente nos dice que $L_1 = L_2$, como queríamos ver.

## Integrales radiométricas

En esta sección, vamos a explorar las nuevas herramientas que nos proporciona la radiancia. Veremos también cómo integrar ángulos sólidos, y cómo simplificar dichas integrales.

### Una nueva expresión de la irradiancia y el flujo

Como dijimos al final de [la sección de la irradiancia](#irradiancia), esta medida no tiene en cuenta las direcciones desde las que llegaba la luz. A diferencia de esta, la radiancia sí que las utiliza. Dado que una de las ventajas de la radiancia es que nos permite obtener el resto de medidas radiométricas, ¿por qué no desarrollamos una nueva expresión de la irradiancia?

Para obtener cuánta luz llega a un punto, debemos acumular la radiancia incidente que nos llega desde cualquier dirección.

> TODO: dibujo como el de la libreta roja. Me lo mandé por Telegram, por si no lo encuentro

Dado un punto $p$ que se encuentra en una superficie con normal $\mathbf{n}$ en dicho punto, la irradiancia se puede expresar como

$$
E(p, \mathbf{n}) = \int_{\Omega}{L_i(p, \omega) \abs{cos\theta} d\omega}
$${#eq:E_abs_cos}

El término $\cos\theta$ aparece en la integral debido a la derivada del área proyectada, $dA^\bot$. $\theta$ es el ángulo entre la dirección $\omega$ y la normal $\mathbf{n}$.

Generalmente, la irradiancia se calcula únicamente en el hemisferio de direcciones asociado a la normal en el punto, $H^2(\mathbf{n})$.

Podemos eliminar el $\cos\theta$ de la integral mediante una pequeña transformación: proyectando el ángulo sólido sobre el disco alrededor del punto $p$ con normal $\mathbf{n}$, obtenemos una expresión más sencilla: como $d\omega^\bot = \abs{\cos\theta} d\omega$, entonces

$$
\begin{aligned}
    E(p, \mathbf{n}) = \int_{H^2(\mathbf{n})}{L_i(p, \omega) d\omega^\bot}
\end{aligned}
$$

Usando lo que aprendimos sobre la derivada de los ángulos sólidos [{@eq:d_omega}], se puede reescribir la ecuación anterior como

$$
E(p, \mathbf{n}) = \int_{0}^{2\pi}\int_{0}^{\pi/2}{L_i(p, \theta, \phi) \cos\theta\ \sin\theta\ d\theta\ d\phi}
$$

Haciendo el mismo juego con el flujo emitido de un cierto objeto al hemisferio que encapsula la normal, conseguimos:

$$
\begin{aligned}
    \Phi & = \int_{A}\int_{H^2(\mathbf{n})}{L_o(p, \omega) \cos\theta\ d\omega dA} = \\
         & = \int_{A}\int_{H^2(\mathbf{n})}{L_o(p, \omega) d\omega^\bot dA}
\end{aligned}
$$

> TODO: a lo mejor merece la pena hacer un ejemplo sobre los diferentes tipos de luz, como en https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf p.41? O a lo mejor un capítulo para hablar de luces en general.

### Integrando sobre área

Una herramienta más que nos vendrá bien será la capacidad de convertir integrales sobre direcciones en integrales sobre área. Hemos hecho algo similar en las secciones anteriores, así que no perdemos nada por generalizarlo.

Considera un punto $p$ sobre una superficie con normal en dicho punto $\mathbf{n}$. Supongamos que tenemos una pequeña área $dA$ con normal $\mathbf{n_{dA}}$. Sea $\theta$ el ángulo entre $\mathbf{n}$ y $\mathbf{n_{dA}}$, y $r$ la distancia entre $p$ y $dA$.

Entonces, la relación entre la diferencial de un ángulo sólido y la de un área es

$$
d\omega = \frac{dA\cos\theta}{r^2}
$$

> TODO: figura como la de pbr book 5.16.

Esto nos permite, por ejemplo, expandir algunas expresiones como la de la irradiancia [{@eq:E_abs_cos}] si partimos de un cuadrilátero $dA$:

$$
\begin{aligned}
    E(p, \mathbf{n}) & = \int_{\Omega}{L_i(p, \omega) \abs{\cos\theta} d\omega} = \\
                     & = \int_{A}{L\cos\theta\ \frac{\cos\theta_o}{r^2}dA}
\end{aligned}
$$

siendo $\theta_o$ el ángulo de la radiancia de salida de la superficie del cuadrilátero.

## Dispersión de luz: las familias de funciones de distribución bidireccionales

Cuando una fuente de luz emite fotones hacia una superficie impactando en ella, ocurren un par de sucesos: parte de la luz se refleja en ella, saliendo disparada hacia alguna dirección; mientras que otra parte se absorbe.

En informática gráfica se consideran tres tipos principales de dispersión de luz: **dispersión en superficie** (*surface scattering*), **dispersión volumétrica** (*volumetric scattering*) y **dispersión bajo superficie** (*subsurface scattering*)

En este capítulo vamos a modelar la primera. Estudiaremos qué es lo que ocurre cuando los fotones alcanzan una superficie, en qué dirección se reflejan, y cómo cambia el comportamiento dependiendo de las propiedades del material.

### La función de distribución de reflectancia bidireccional (BRDF)

La **función de distribución de reflectancia bidireccional** (en inglés, *bidirectional reflectance distribution function*, BRDF) describe cómo la luz se refleja en una superficie opaca. Se encarga de informarnos sobre cuánta radiancia sale en dirección $\omega_o$ debido a la radiancia incidente desde la dirección $\omega_i$, partiendo de un punto $p$ en una superficie con normal $\mathbf{n}$. Depende de la longitud de onda $\lambda$, pero, como de costumbre, la omitiremos.

> **Intuición**: *¿cuál es la probabilidad de que, habiéndome llegado un fotón desde $\omega_i$, me salga disparado hacia $\omega_o$?*

> TODO: esquema como el de pbr fig. 5.18, o como https://pellacini.di.uniroma1.it/teaching/graphics17b/lectures/12_pathtracing.pdf p.20

Si consideramos $\omega_i$ como un cono diferencial de direcciones, la irradiancia diferencial en $p$ viene dada por

$$
dE(p, \omega_i) = L_i(p, \omega_i) \cos\theta_i\ d\omega_i
$$

Debido a esta irradiancia, una pequeña parte de radiancia saldrá en dirección $\omega_o$, proporcional a la irradiancia:

$$
dL_o(p, \omega_o) \propto dE(p, \omega_i)
$$

Si lo ponemos en forma de cociente, sabremos exactamente cuál es la proporción de luz. A este cociente lo llamaremos $f_r(p, \omega_o \leftarrow \omega_i)$; la función de distribución de reflectancia bidireccional:

$$
f_r(p, \omega_o \leftarrow \omega_i) = \frac{dL_o(p, \omega_o)}{dE(p, \omega_i)} = \frac{dL_o(p, \omega_o)}{L_i(p, \omega_i) \cos\theta_i\ d\omega_i} \text{(1/sr)}
$$

> **Nota**(ción): dependiendo de la fuente que estés leyendo, es posible que te encuentres una integral algo diferente. Por ejemplo, en tanto en Wikipedia como en [@ShirleyRRT] se integra con respecto a los ángulos de salida $\omega_o$, en vez de los incidentes.
>
> Aquí, usaremos la notación de integrar con respecto a los incidentes, como se hace en [@PBRT3e].



Las BRDF físicamente realistas tienen un par de propiedades importantes:

1. **Reciprocidad**: para cualquier par de direcciones $\omega_i$, $\omega_o$, se tiene que $f_r(p, \omega_i, \omega_o)=\ $ $f_r(p, \omega_o \leftarrow \omega_i)$.
2. **Conservación de la energía**: La energía reflejada tiene que ser menor o igual que la incidente:

$$
\int_{H^2(\mathbf{n})}{f_r(p, \omega_o \leftarrow \omega_i) \cos\theta_i\ d\omega_i} \leq 1
$$

### La función de distribución de transmitancia bidireccional (BTDF)

Si la BRDF describe cómo se refleja la luz, la *bidirectional transmittance distribution function* (abreviada BTDF) nos informará sobre la transmitancia; es decir, cómo se comporta la luz cuando *entra* en un medio. Generalmente serán dos caras de la misma moneda: cuando la luz impacta en una superficie, parte de ella, se reflejará, y otra parte se transmitirá.

Puedes imaginarte la BTDF como una función de reflectancia del hemisferio opuesto a donde se encuentra la normal de la superficie.

Denotaremos a la BTDF por

$$
f_t(p, \omega_o \leftarrow \omega_i)
$$

Al contrario que en la BRDF, $\omega_o$ y $\omega_i$ se encuentran en hemisferios diferentes.

### Juntando la BRDF y la BTDF en La función de distribución de dispersión bidireccional

Convenientemente, podemos unir la BRDF y la BTDF en una sola expresión, llamada **la función de distribución de dispersión bidireccional** (*bidirectional scattering distribution function*, BSDF). A la BSDF la denotaremos por

$$
f(p, \omega_o \leftarrow \omega_i)
$$

> **Intuición:** *la BSDF son todas las posibles direcciones en las que puede salir disparada la luz.*

Usando esta definición, podemos obtener

$$
dL_o(p, \omega_o) = f(p, \omega_o \leftarrow \omega_i) L_i(p, \omega_i) \abs{\cos\theta_i} d\omega_i
$$

Esto nos deja a punto de caramelo una nueva expresión de la randiancia en términos de la randiancia incidente en un punto $p$. Integrando la expresión anterior, obtenemos

$$
L_o(p, \omega_o) = \int_{\mathbb{S}^2}{f(p, \omega_o \leftarrow \omega_i)L_i(p, \omega_i)\abs{\cos\theta_i} d\omega_i}
$${#eq:scattering_equation}

siendo $\mathbb{S}^2$ la esfera.

Esta forma de expresar la radiancia es muy importante. Generalmente se le suele llamar la *ecuación de dispersión* (*scattering equation*, en inglés). Dado que es una integral muy importante, seguramente tengamos que evaluarla repetidamente. ¡Los métodos de Monte Carlo nos vendrán de perlas! Más adelante hablaremos de ella.

Las BSDFs tienen unas propiedades interesantes:

- **Positividad**: como los fotones no se pueden reflejar "negativamente", $f(p, \omega_o \leftarrow \omega_i) \ge 0$.
- **Reciprocidad de Helmotz:** se puede invertir un rayo de luz: $f(p, \omega_o \leftarrow \omega_i) = f(p, \omega_i \leftarrow \omega_o)$.
- **Conservación de la energía**: todos los fotones que llegan a la superficie deben ser reflejados o absorbidos. Es decir, no se emite ningún fotón nuevo:

$$
\int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) \cos\theta_i\ d\omega_i} \le 1\ \forall \omega_o
$$

### Reflectancia hemisférica

Puede ser útil tomar el comportamiento agregado de las BRDFs y las BTDFs y reducirlo un cierto valor que describa su comportamiento general de dispersión. Sería Algo así como un resumen de su distribución. Para conseguirlo, vamos a introducir dos nuevas funciones:

La **reflectancia hemisférica-direccional** (*hemispherical-directional reflectance*) describe la reflexión total sobre un hemisferio debida a una fuente de luz que proviene desde la dirección $\omega_o$:

$$
\rho_{hd}(\omega_o) = \int_{H^2(n)}{f_r(p, \omega_o \leftarrow \omega_i) \abs{\cos\theta_i}\ d\omega_i}
$$


Por otra parte, la **reflectancia hemisférica-hemisférica** (*hemispherical-hemispherical reflectance*) es un valor espectral que nos proporciona el ratio de luz incidente reflejada por una superficie, suponiendo que llega la misma luz desde todas direcciones:

$$
\rho_{hh} = \frac{1}{\pi} \int_{H^2(n)} \int_{H^2(n)}{f_r(p, \omega_o \leftarrow \omega_i) \abs{\cos\theta_o\ \cos\theta_i}\ d\omega_o\ d\omega_i}
$$

### Reflejos

Una vez hemos definido las funciones de distribución bidireccionales, debemos encargarnos de modelar el comportamiento explícitamente. Para ello, veamos cómo los materiales modifican las distribuciones.

En esencia, los reflejos se pueden clasificar en cuatro grandes tipos:

- **Difusos** (*Diffuse*): esparcen la luz en todas direcciones casi equiprobablemente. Por ejemplo, la tela y el papel son materiales difusos.
- **Especulares brillantes** (*Glossy specular*): la distribución de luz se asemeja a un cono. La chapa de un coche es un material especular brillante.
- **Especulares perfectos** (*Perfect specular*): en esencia, son espejos. El ángulo de salida de la luz es muy pequeño, por lo que reflejan casi a la perfección lo que les llega.
- **Retrorreflectores** (*Retro reflective*): la luz se refleja en dirección contraria a la de llegada. Esto es lo que sucede a la luna.

Ten en cuenta que es muy difícil encontrar objetos físicos que imiten a la perfección un cierto modelo. Suelen recaer en un híbrido entre dos o más modelos.

Fijado un cierto modelo, la función de distribución de reflectancia, BRDF, puede ser **isotrópica** o **anisotrópica**. Los materiales isotrópicos mantienen las propiedades de reflectancia invariantes ante rotaciones; es decir, la distribución de luz es la misma en todas direcciones. Por el contrario, los anisotrópicos reflejan diferentes cantidades de luz dependiendo desde dónde los miremos. Los ejemplos más habituales de materiales anisotrópicos son las rocas y la madera.

## La rendering equation

Y, finalmente, tras esta introducción de los principales conceptos radiométricos, llegamos a la ecuación más importante de todo este trabajo: la **rendering equation**; también llamada la **ecuación del transporte de luz**.

> **Nota**(ción): esta vez no traduciré el concepto. Es cierto que afea un poco la escritura teniendo en cuenta que esto es un texto en castellano. Sin embargo, la otra opción es inventarme una traducción que nadie usa.

Antes de comenzar, volvamos a plantear de nuevo la situación: nos encontramos observando desde nuestra pantalla una escena virtual mediante la cámara. Queremos saber qué color tomará un pixel específico. Para conseguirlo, dispararemos rayos desde nuestro punto de vista hacia el entorno, haciendo que reboten en los objetos. Cuando un rayo impacte en una superficie, adquirirá parte de las propiedades del material del objeto. Además, de este rayo surgirán otros nuevos (un rayo dispersado y otro refractado), que a su vez repetirán el proceso. La información que se obtiene a partir de estos caminos de rayos nos permitirá darle color al píxel.

La *rendering equation* se va a encargar de describir analíticamente cómo ocurre esto.

Un último concepto más: denotemos por $L_e(p, \omega_o)$ a **la radiancia producida por los materiales emisivos**. Por ejemplo, una luz emite radiancia por sí misma.

Bien, partamos de la ecuación de para la radiancia reflejada:

$$
L_o(p, \omega_o) = \int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) L_i(p, \omega_i) \cos\theta_i\ d\omega_i}
$$

Vamos a buscar expresar la radiancia incidente en términos de la radiancia reflejada. Para ello, usamos la propiedad de que la radiancia a lo largo de un rayo no cambia.

Si a una superficie le llega un fotón desde alguna parte, debe ser porque *"alguien"* ha tenido que emitirlo. El fotón necesariamente ha llegado a partir de un rayo. La propiedad nos dice que la radiancia no ha podido cambiar en el camino.

Pues bien, consideremos una función $r: \mathbb{R}^3 \times \Omega \to \mathbb{R}^3$ tal que, dado un punto $p$ y una dirección $\omega$, devuelve el siguiente punto de impacto en una superficie. En esencia, es una función de *ray casting*.

> TODO: foto como la de https://pellacini.di.uniroma1.it/teaching/graphics17b/lectures/12_pathtracing.pdf, p.29

Esta función nos permite expresar el punto anterior de la siguiente forma:

$$
L_i(p, \omega) = L_o(r(p, \omega), -\omega)
$$

Esto nos permite cambiar la expresión de $L_i$ en la integral anterior:

$$
L_o(p, \omega_o) = \int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) L_o(r(p, \omega_i), -\omega_i) \cos\theta_i\ d\omega_i}
$$

Finalmente, la radiancia total vendrá dada por la suma de la radiancia emitida y la reflejada:

$$
L(p, \omega_o) = L_e(p, \omega_o) + \int_{H^2(\mathbf{n})}{f(p, \omega_o \leftarrow \omega_i) L_o(r(p, \omega_i), -\omega_i) \cos\theta_i\ d\omega_i}
$${#eq:rendering_equation}

Y con esto, ¡hemos obtenido la *rendering equation*!

Si quieres ver gráficamente cómo funciona, te recomiendo pasarte por [@arneback-2019]. Es un vídeo muy intuitivo.

<iframe width="784" height="441" src="https://www.youtube-nocookie.com/embed/eo_MTI-d28s" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

Si nos paramos a pensar, la ecuación de reflexión es muy similar a la de renderizado. Sin embargo, hay un par de matices que las hacen muy diferentes:

- La ecuación de reflexión describe cómo se comporta la luz reflejada en un cierto punto. Es decir, tiene un ámbito local. Además, para calcular la radiancia reflejada, se necesita conocer la radiancia incidente.
- La *rendering equation* calcula las condiciones globales de la luz. Además, no se conocen las radiancias de salida.

Este último matiz es importante. Para renderizar una imagen, se necesita calcular la radiancia de salida para aquellos puntos visibles desde nuestra cámara.

## Materiales



<hr>

## Referencias {.unlisted .unnumbered}

[@PBRT3e], [@wikipedia-contributors-2021D], [@studysession-2021], [@berkeley-cs184, Radiometry & Photometry], [@wikipedia-funcion-de-distribucion-de-reflectancia-bidireccional-2022], [@wikipedia-transmittance-2021]

- https://matmatch.com/learn/property/isotropy-anisotropy
- https://pellacini.di.uniroma1.it/teaching/graphics17b/lectures/12_pathtracing.pdf

[^2]: No entraremos en detalle sobre la naturaleza de la luz. Sin embargo, si te pica la curiosidad, hay muchos divulgadores como [@quantumfracture-2021] que han tratado el tema con suficiente profundidad.
[^3]: Recuerda que estamos omitiendo la longitud de onda $\lambda$.
