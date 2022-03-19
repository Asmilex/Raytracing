# Radiometría

En este capítulo estudiaremos las bases de la radiometría. Esta área de la óptica nos proporcionará una serie de herramientas con las cuales podremos responder a la pregunta *cuánta luz existe en un punto*.

## Unidades básicas

> **Nota**: cuando usemos un paréntesis tras una ecuación, dentro denotaremos sus unidades de medida.

Antes de comenzar a trabajar, necesitamos conocer *qué entendemos* por luz. Aunque hay muchas formas de trabajar con ella (a fin de cuentas, todavía seguimos discutiendo sobre *qué es* exactamente la luz[^2]), nosotros nos quedaremos con algunas pinceladas de la cuántica. Nos será suficiente quedarnos con la concepción de fotón. Una fuente de iluminación emite una serie de fotones. Estos fotones tienen<sup>[@ShirleyRRT]</sup> una posición, una dirección de propagación y una longitud de onda $\lambda$. Un fotón también tiene asociado una velocidad $c$ que depende del índice de refracción del medio, $n$.

La unidad de medida de $\lambda$ es el nanómetro ($\text{nm}$). También nos vendrá bien definir una frecuencia, $f$. Su utilidad viene del hecho de que, cuando la luz cambia de medio al propagarse, la frecuencia se mantiene constante.

$$
f = \frac{c}{\lambda}
$$

Un fotón tiene asociada una carga de energía, denotada por $Q$:

$$
Q = hf = \frac{hc}{\lambda} (\text{J})
$$

donde $h = 6.62607004 \times 10^{-34} \text{J} \cdot \text{s}$ es la constante de Planck y $c = 299 792 458 \text{m/s}$ la velocidad de la luz.

En realiad, **todas estas cantidades deberían tener un subíndice $\lambda$**, puesto que dependen de la longitud de onda. La energía de un fotón $Q$, por ejemplo, debería denotarse $Q_\lambda$. Sin embargo, en la literatura de informática gráfica, **se ha optado por omitirla**. ¡Tenlo en cuenta a partir de aquí!

### Potencia

A partir de la energía anterior, podemos estimar *la tasa de producción de energía*. A esta tasa la llamaremos<sup>[@PBRT3e]</sup> **potencia**, o **flujo radiante** $\Phi$. Esta medida nos resultará más útil que la energía total, puesto que nos permite estimar la energía en un instante:

$$
\Phi = \lim_{\Delta t \to 0}{\frac{\Delta Q}{\Delta t}} = \frac{dQ}{dt} (J/s)
$$

Su unidad es julios por segundo, comunmente denotado vatio (*watts*, $\text{W}$).
Podemos encontrar la energía total en un periodo de tiempo $[t_0, t_1]$ integrando el flujo randiante:

$$
Q = \int_{t_0}^{t_1}{\Phi(t)dt}
$$

### Irradiancia

La **irradiancia** o **radiancia emitida** es el fujo radiante que recibe una superficie. Dada un área $A$, se define como

$$
E = \frac{\Phi}{A} (W/m^2)
$$

Ahora que tenemos la potencia emitida en un cierto área, nos surge una pregunta: *¿y en un cierto punto $p$?*. Tomando límites en la expresión anterior, encontramos la respuesta:

$$
E(p) = \lim_{\Delta A \to 0}{\frac{\Delta \Phi}{\Delta A}} = \frac{d\Phi}{dA} (W/m^2)
$$

De la misma manera que con la potencia, integrando $E(p)$ podemos obtener el flujo radiante:

$$
\Phi = \int_{A}{E(p)dp}
$$

El principal problema de la irradianza es que *no nos dice nada sobre las direcciones* desde las que ha llegado la luz.

### Ángulos sólidos

Con estas tres unidades básicas, nos surge una pregunta muy natural: *¿cómo mido cuánta luz llega a una superficie?*

Para responder a esta pregunta, usaremos los **ángulos sólidos**.

Un ángulo sólido es la extensión del concepto de **ángulo planar** (en dos dimensiones). Para ilustrar el sentido de estos ángulos, imaginemos que tenemos un cierto objeto en dos dimensiones delante nuestra. Dependiendo de cómo de lejos se encuentre, nos parecerá más grande o más pequeño. Si trazamos un par de líneas desde nuestra posición a las partes más alejadas de este objeto, y las cortamos con una circunferencia de radio $r$, obtendremos un par de puntos en dicha circunferencia. Al arco que encapsulan ambos puntos le corresponde un cierto ángulo: el ángulo planar.

![La idea intuitiva de un ángulo planar](./img/Ángulo%20planar.png)

Llevando esta idea a las tres dimensiones es como conseguimos el concepto de **ángulo sólido**. Su unidad de medida es el estereorradián ($\text{sr}$). Corresponde a la superficie generada por las rectas proyectantes que van desde un objeto hasta un punto, cortando con una esfera de radio $r$. Se define como

$$
\omega = \frac{A}{r^2} \text{(sr)}
$$

siendo $A$ la superficie cubierta por el objeto. Por tanto, un esterorradián corresponde una superficie con área $r^2$.

> TODO: foto de un ángulo sólido.

Si $2 \pi$ radianes corresponden a la cirfunferencia completa, para la esfera se tiene que $4 \pi$ esterorradianes cubren toda la superficie de ésta. Además, $2\pi$ sr cubren un hemisferio.

Denotaremos a los ángulos sólidos por $\omega$. Como hemos visto, $\omega \in [0, 4\pi]$.

### Intensidad

Los ángulos sólidos nos proporcionan una variedad de herramientas nuevas considerable. Gracias a ellos, podemos desarrollar algunos conceptos nuevos. Uno de ellos es la **intensidad**.

Imaginémonos un pequeñito punto de luz encerrado en una esfera, el cual emite fotones en todas direcciones. Nos gustaría medir cuánta energía pasa por la esfera. Podríamos entonces definir

$$
I = \frac{\Phi}{4\pi} \text{(W/sr)}
$$

La anterior definición mide cuántos fotones pasan por toda la esfera. ¿Qué ocurre si *cerramos* el ángulo, restringiéndonos así a un área muy pequeña de la esfera?

$$
I = \lim_{\Delta\omega \to 0}{\frac{\Delta\Phi}{\Delta\omega}} = \frac{d\Phi}{d\omega}
$$

De la misma manera que con los conceptos anteriores, podemos volver a la potencia integrando sobre un conjunto de direcciones:

$$
\Phi = \int_{\Omega}{I(\omega)d\omega}
$$

### Randianza

Finalmente, llegamos al concepto más importante. La **radianza espectral** (o radianza a secas[^3]) es una extensión de la radianza emitida teniendo en cuenta la dirección:

$$
L(p, \omega) = \lim_{\Delta\omega \to 0}{\frac{\Delta E_\omega(p)}{\Delta\omega}} = \frac{dE_\omega(p)}{d\omega}
$$

siendo $E_\omega(p)$ la radianza emitida a la superficie perpendicular a $\omega$.

Podemos dar otra expresión de la radianza en términos del flujo:

$$
L = \frac{d\Phi}{d\omega dA^\bot}
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

Otra forma de solucionarlo (y preferible, puesto que simplifica entender lo que ocurre) es distinguir entre la radianza que llega a un punto --la incidente--, y la que sale.

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
            L^+(p, \omega)  & \text{si }  \omega \cdot \mathbf{n_p} > 0 & \\
            L^-(p, \omega)  & \text{si }  \omega \cdot \mathbf{n_p} < 0 &
        \end{cases}
\end{aligned}
$$

Una propiedad a tener en cuenta es que, si cogemos un punto $p$ del espacio donde no existe ninguna superifcie, $L_o(p, \omega) = L_i(p, -\omega) = L(p, \omega)$

La importancia de la radianza se debe a un par de propiedades:

La primera de ellas es que, dado $L$, podemos calcular cualquier otra unidad básica mediante integración. Además, su valor se mantiene constante en rayos que viajan en el vacío. Parece natural usarla en un ray tracer.

## Integrales radiométricas

En esta sección, vamos a explorar las nuevas herramientas que nos proporciona la radianza. Veremos también cómo integrar ángulos sólidos, y cómo simplificar dichas integrales.

### Una nueva expresión de la irradianza

Como dijimos al final de [la sección de la irradianza](#irradiancia), esta medida no tiene en cuenta las direcciones desde las que llegaba la luz.

Una de las ventajas de la radianza es que nos permite obtener el resto de medidas radiométricas. Pues bien, empecemos con la irradianza.

Para obtener cuánta luz llega a un punto, debemos acumular la radianza incidente que nos llega desde cualquier dirección.

> TODO: dibujo como el de la libreta roja. Me lo mandé por Telegram, por si no lo encuentro

Dado un punto $p$ que se encuentra en una superficie con normal $n$ en dicho punto, la irradianza se puede expresar como

$$
\int_{\Omega}{L_i(p, \omega) \mid cos\theta \mid d\omega}
$$

El término $cos\theta$ aparece en la integral debido a la derivada del área proyectada, $dA^\bot$. $\theta$ es el ángulo entre la dirección $\omega$ y la normal $\mathbf{n}$.

Generalmente, la irradianza se calcula únicamente en el hemisferio de direcciones asociado a la normal en el punto, $H^2(\mathbf{n})$.

[^2]: No entraremos en detalle sobre la naturaleza de la luz. Sin embargo, si te pica la curiosidad, hay muchos divulgadores [como QuantumFracture](https://www.youtube.com/watch?v=DkcEAz09Buo) que han tratado el tema con suficiente profundidad.
[^3]: Recuerda que estamos omitiendo la longitud de onda $\lambda$.

<br>

- https://www.wikiwand.com/en/Radiometry