# *Importance sampling*

Con la llegada de ray tracing en tiempo real surge una obligación por optimizar los pocos rayos que se pueden trazar. Una de las preguntas que nos debemos hacer es *hacia dónde* generamos el rayo.

En este capítulo daremos respuesta a este dilema. Estudiaremos cómo las fuentes de luz afectan a la calidad de la imagen final. Veremos técnicas de reducción del error, las cuales nos permitirán acelerar enormemente el cómputo de la escena.



<hr>

## Referencias {.unlisted .unnumbered}

- *(berkeley-cs184)* https://cs184.eecs.berkeley.edu/public/sp22/lectures/lec-12-monte-carlo-integration/lec-12-monte-carlo-integration.pdf
- Gems I, p.284.