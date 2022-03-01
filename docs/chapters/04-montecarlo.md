# Integración de Monte Carlo

> TODO: este capítulo seguramente debería ir más tarde. De esa forma, puedo introducir otros conceptos antes. De momento, se queda aquí.

La parte más importante de nuestro ray tracer es saber calcular la luz en un punto. Para ello, necesitaríamos hallar la radianza en dicho punto mediante la *rendering equation*. Sin embargo, es *muy* difícil resolverla. Tanto computacionalmente como analíticamente. Por ello, debemos atacar el problema desde otro punto de vista.

Las técnicas de Monte Carlo nos permitirán aproximar el valor que toman mediante una estimación. Utilizando muestreo aleatorio para evaluar integrales, seremos capaces de obtener un resultado suficientemente bueno.

Una de las propiedades más improtantes que tienen es la **independencia del ratio de convergencia y la dimensionalidad del integrando**. Sin embargo, dadas $n$ nuestras, la convergencia a la solución correcta tiene un orden de $\mathcal{O}\left(n^{-1/2}\right)$. Es decir, para reducir el error a la mitad, necesitaríamos 4 veces más muestras.

## Repaso de probabilidad

Necesitaremos unas cuantas nociones de variable aleatoria para poder entender la integración de Monte Carlo, así que vamos a hacer un breve repaso.

Una **variable aleatoria**

<hr>

- https://www.wikiwand.com/en/Rendering_equation
- https://www.pbr-book.org/3ed-2018/contents
- RTT Shirley.