# Radiometría

En este capítulo estudiaremos las bases de la radiometría. Esta área de la óptica nos proporcionará una serie de herramientas con las cuales podremos responder a la pregunta *cuánta luz existe en un punto*.

## Unidades básicas

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

> TODO: dibujo como el de https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf, p.28

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

Para responder a esta pregunta, usaremos los **ángulos sólidos**.

Un ángulo sólido es la extensión del concepto de **ángulo planar** (en dos dimensiones). Para ilustrar el sentido de estos ángulos, imaginemos que tenemos un cierto objeto en dos dimensiones delante de nosotros. Dependiendo de cómo de lejos se encuentre, nos parecerá más grande o más pequeño. Si trazamos un par de líneas desde nuestra posición a las partes más alejadas de este objeto, y las cortamos con una circunferencia de radio $r$, obtendremos un par de puntos en dicha circunferencia. Al arco que encapsulan ambos puntos le corresponde un cierto ángulo: el ángulo planar.

![La idea intuitiva de un ángulo planar](./img/Ángulo%20planar.png)

Llevando esta idea a las tres dimensiones es como conseguimos el concepto de **ángulo sólido**. Su unidad de medida es el estereorradián ($\text{sr}$). Corresponde a la superficie generada por las rectas proyectantes que van desde un objeto hasta un punto, cortando con una esfera de radio $r$. Se define como

$$
\omega = \frac{A}{r^2} \text{(sr)}
$$

siendo $A$ la superficie cubierta por el objeto. Por tanto, un esteorradián corresponde una superficie con área $r^2$: $1 \text{sr} = \frac{r^2}{r^2}$.

> TODO: foto de un ángulo sólido.

Si $2 \pi$ radianes corresponden a la circunferencia completa, para la esfera se tiene que $4 \pi$ esteorradianes cubren toda la superficie de esta. Además, $2\pi$ sr cubren un hemisferio.

Denotaremos a los ángulos sólidos por $\omega$. Como hemos visto, $\omega \in [0, 4\pi]$.

De vez en cuando, usaremos $\omega$ **un vector dirección unitario en la esfera**.

> TODO: y si meto un xkcd https://xkcd.com/1276/

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
$$

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

Otra forma de solucionarlo (y preferible, puesto que simplifica entender lo que ocurre) es distinguir entre la radiancia que llega a un punto --la incidente--, y la que sale.

La primera se llamará $L_i(p, \omega)$, mientras que la segunda será $L_o(p, \omega)$. Es importante destacar que $\omega$ apunta *hacia fuera* de la superficie. Quizás es contraintuitivo en $L_i$, puesto que $-\omega$ apunta *hacia* la superficie. Depende del autor se utiliza una concepción u otra.

Utilizando esta notación y usando [{@eq:L_limit}], podemos escribir $L_i$ y $L_o$ como

$$
\begin{aligned}
    L_i(p, \omega) & =
        \begin{cases}
            L^+(p, -\omega) & \text{si }  \omega \cdot \mathbf{n_p} > 0  \\
            L^-(p, -\omega) & \text{si }  \omega \cdot \mathbf{n_p} < 0
        \end{cases} \\
    L_o(p, \omega) & =
        \begin{cases}
            L^+(p, \omega)  & \text{si }  \omega \cdot \mathbf{n_p} > 0  \\
            L^-(p, \omega)  & \text{si }  \omega \cdot \mathbf{n_p} < 0
        \end{cases}
\end{aligned}
$$

> TODO: https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf, p.36

Una propiedad a tener en cuenta es que, si cogemos un punto $p$ del espacio donde no existe ninguna superifcie, $L_o(p, \omega) = L_i(p, -\omega) = L(p, \omega)$

La importancia de la radiancia se debe a un par de propiedades:

La primera de ellas es que, dado $L$, podemos calcular cualquier otra unidad básica mediante integración. Además, su valor se mantiene constante en rayos que viajan en el vacío. Parece natural usarla en un ray tracer.

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

## Integrando sobre área

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

En este capítulo vamos a modelar la primera. Estudiaremos qué es lo que ocurre cuando los fotones alcanzan una superficie, en qué dirección se reflejan, y cómo cambia el comportamiento dependiendo de las propiedades del material.

### La función de distribución de reflectancia bidireccional (BRDF)

La **función de distribución de reflectancia bidireccional** (en inglés, *bidirectional reflectance distribution function*, BRDF) describe cómo la luz se refleja en una superficie opaca. Se encarga de informarnos sobre cuánta radiancia sale en dirección $\omega_o$ debido a la radiancia incidente desde la dirección $\omega_i$, partiendo de un punto $p$ en una superficie con normal $\mathbf{n}$.

> TODO: esquema como el de pbr fig 5.18.

Si consideramos $\omega_i$ como un cono diferencial de direcciones, la irradiancia diferencial en $p$ viene dada por

$$
dE(p, \omega_i) = L_i(p, \omega_i) \cos\theta_i\ d\omega_i
$$

Debido a esta irradiancia, una pequeña parte de radiancia saldrá en dirección $\omega_o$, proporcional a la irradiancia:

$$
dL_o(p, \omega_o) \propto dE(p, \omega_i)
$$

Si lo ponemos en forma de cociente, sabremos exactamente cuál es la proporción de luz. A este cociente lo llamaremos $f_r(p, \omega_o, \omega_i)$; la función de distribución de reflectancia bidireccional:

$$
f_r(p, \omega_o, \omega_i) = \frac{dL_o(p, \omega_o)}{dE(p, \omega_i)} = \frac{dL_o(p, \omega_o)}{L_i(p, \omega_i) \cos\theta_i\ d\omega_i} \text{(1/sr)}
$$

Las BRDFs físicamente realistas tienen un par de propiedades importantes:

1. **Reciprocidad**: para cualquier par de direcciones $\omega_i$, $\omega_o$, se tiene que $f_r(p, \omega_i, \omega_o)=\ $ $f_r(p, \omega_o, \omega_i)$.
2. **Conservación de la energía**: La energía reflejada tiene que ser menor o igual que la incidente:

$$
\int_{H^2(\mathbf{n})}{f_r(p, \omega_o, \omega_i) \cos\theta_i\ d\omega_i} \leq 1
$$

### La función de distribución de transmitancia bidireccional (BTDF)

Si la BRDF describe cómo se refleja la luz, la *bidirectional transmittance distribution function* (abreviada BTDF) nos informará sobre la transmitancia; es decir, cómo se comporta la luz cuando entra en un medio. Son caras de la misma moneda: cuando la luz impacta en una superficie, y parte de ella, se reflejará, y otra parte se transmitirá.

Denotaremos a la BTDF por

$$
f_t(p, \omega_o, \omega_i)
$$

Al contrario que en la BRDF, $\omega_o$ y $\omega_i$ se encuentran en hemisferios diferentes.

### Juntando la BRDF y la BTDF

Convenientemente, podemos unir la BRDF y la BTDF en una sola expresión, llamada **la función de distribución de dispersión bidireccional** (*bidirectional scattering distribution function*, BSDF). A la BSDF la llamaremos

$$
f(p, \omega_o, \omega_i)
$$

Usando esta definición, podemos obtener

$$
dL_o(p, \omega_o) = f(p, \omega_o, \omega_i) L_i(p, \omega_i) \abs{\cos\theta_i} d\omega_i
$$


> NOTE: En wikipedia integran con respecto a $\omega_o$, y no con la incidente. ¿Quizás afecte en algo? Además, el término $\cos\theta_i$ aparece en valor absoluto porque las normales no siempre están orientadas hacia fuera. ¿Podríamos omitirlo?

Esto nos deja a punto de caramelo una nueva expresión de la randiancia en términos de la randiancia incidente en un punto $p$. Integrando la expresión anterior, obtenemos

$$
L_o(p, \omega_o) = \int_{\mathbb{S}^2}{f(p, \omega_o, \omega_i)L_i(p, \omega_i)\abs{\cos\theta_i} d\omega_i}
$$

siendo $\mathbb{S}^2$ la esfera.

Esta forma de expresar la radiancia es muy importante. Generalmente se le suele llamar la *ecuación de dispersión* (*scattering equation*, en inglés). Dado que es una integral muy importante, seguramente tengamos que evaluarla repetidamente. ¡Los métodos de Monte Carlo nos vendrán de perlas!

### *Subsurface scattering*

> NOTE: quizás no merezca la pena hablar de esto. De momento, se queda para recordar su existencia. No es tampoco difícil, pero si no lo vamos a usar...

[^2]: No entraremos en detalle sobre la naturaleza de la luz. Sin embargo, si te pica la curiosidad, hay muchos divulgadores [como QuantumFracture](https://www.youtube.com/watch?v=DkcEAz09Buo) que han tratado el tema con suficiente profundidad.
[^3]: Recuerda que estamos omitiendo la longitud de onda $\lambda$.

<hr>

@wikipedia-contributors-2021D, @studysession-2021

- https://www.wikiwand.com/es/Funci%C3%B3n_de_distribuci%C3%B3n_de_reflectancia_bidireccional
- https://www.wikiwand.com/en/Transmittance
- https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-11-radiometry-and-photometry/lec-11-radiometry-and-photometry.pdf