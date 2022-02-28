# Integración de Monte Carlo

> TODO: este capítulo seguramente debería ir más tarde. De esa forma, puedo introducir otros conceptos antes. De momento, se queda aquí.

La parte más importante de nuestro ray tracer es saber calcular la luz en un punto. Para ello, necesitaríamos hallar la radianza en un punto mediante la *rendering equation*. Sin embargo, es *muy* difícil resolverla. Tanto computacionalmente como analíticamente. Por ello, debemos atacar el problema desde otro punto de vista.

Las técnicas de Monte Carlo nos permitirán aproximar el valor que toman mediante una estimación. Utilizan muestreo aleatorias para evaluar integrales. Una de las propiedades más improtantes que tienen es la **independencia del ratio de convergencia y la dimensionalidad del integrando**.

<hr>

- https://www.wikiwand.com/en/Rendering_equation
- https://www.pbr-book.org/3ed-2018/contents
- RTT Shirley.