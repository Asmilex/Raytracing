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

$J/s = W$

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

### Ángulos sólidos

Con estas tres unidades básicas, nos surge una pregunta muy natural: *¿cómo mido cuánta luz llega a una superficie?*

Para responder a esta pregunta, usaremos los **ángulos sólidos**.

Un ángulo sólido es la extensión del concepto de ángulo planar (en dos dimensiones). Para ilustrar el sentido de estos ángulos, imaginemos que tenemos un cierto objeto en dos dimensiones delante nuestra. Dependiendo de cómo de lejos se encuentre, nos parecerá más grande o más pequeño. Si trazamos un par de líneas desde nuestra posición a las partes más alejadas de este objeto, y las cortamos con una circunferencia de radio $r$, tendremos un par de puntos en nuestra circunferencia. Al arco que encapsulan ambos puntos le corresponde un cierto ángulo.

![La idea intuitiva de un ángulo planar](./img/Ángulo%20planar.png)


[^2]: No entraremos en detalle sobre la naturaleza de la luz. Sin embargo, si te pica la curiosidad, hay muchos divulgadores [como QuantumFracture](https://www.youtube.com/watch?v=DkcEAz09Buo) que han tratado el tema con suficiente profundidad.

<br>

- https://www.wikiwand.com/en/Radiometry