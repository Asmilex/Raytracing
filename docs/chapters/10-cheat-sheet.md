# Anexo: Glosario de términos y expresiones

> *It's dangerous to go alone, take this.*

Tener en mente *todos* los conceptos y las expresiones que pueden tomar es prácticamente imposible. Tampoco hay necesidad de ello, realmente. Por eso, aquí tienes recopilada una lista con las fórmulas y enlace a las secciones correspondientes.

## Notación

| **Concepto**                                        | **Notación**                                                                                                                          |
|:----------------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------------|
| **Puntos**                                          | Letras mayúsculas: $P, Q, \dots$                                                                                                      |
| **Escalares**                                       | Letras minúsculas: $a, b, c, k, \dots$                                                                                                |
| **Vectores**                                        | Letras minúsculas en negrita: $\mathbf{v, w, n}, \dots$. Si están normalizados, se les pone gorrito (por ejemplo, $\hat{\mathbf{n}}$) |
| **Matrices**                                        | Letras mayúsculas en negrita: $\mathbf{M}$. Por columnas.                                                                             |
| **Producto escalar**                                | $\mathbf{v} \cdot \mathbf{w}$. Si es el producto escalar de un vector consigo mismo, a veces pondremos $\mathbf{v}^2$                 |
| **Producto vectorial**                              | $\mathbf{v} \times \mathbf{w}$                                                                                                        |
| [**Variables aleatorias**](#repaso-de-probabilidad) | $X, Y$. $\xi$ representa una variable aleatoria con distribución uniforme en $[0, 1)$.                                                |

## [Radiometría](#radiometría)

| **Concepto**                                                    | **Expresiones**                                                                                                                                                                                                                                                                      |
|:----------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [**Ángulo sólido**](#ángulos-sólidos), derivada [{@eq:d_omega}] | $\begin{aligned} &\omega = \frac{A}{r^2} \\ & d\omega = \sin\theta\ d\theta\ d\phi\end{aligned}$                                                                                                                                                                                     |
| **Hemisferio de direcciones alrededor de un vector**            | $H^2(\mathbf{n})$                                                                                                                                                                                                                                                                      |
| [**Carga de energía**](#unidades-básicas)                       | $Q = hf = \frac{hc}{\lambda}$                                                                                                                                                                                                                                                        |
| **[Flujo radiante, potencia](#potencia)**                       | $\Phi = \frac{dQ}{dt}$                                                                                                                                                                                                                                                               |
| **[Irradiancia, radiancia emitida](#irradiancia)**              | $\begin{aligned} & E = \frac{\Phi}{A} \\ & E(p) = \frac{d\Phi}{dA} \\ & E(p, \mathbf{n}) = \int_{\Omega}{L_i(p, \omega) \abs{cos\theta} d\omega} \\ & E(p, \mathbf{n}) = \int_{0}^{2\pi}\int_{0}^{\pi/2}{L_i(p, \theta, \phi) \cos\theta\ \sin\theta\ d\theta\ d\phi} \end{aligned}$ |
| **[Intensidad radiante](#intensidad_radiante)**                 | $I = \frac{d\Phi}{d\omega}$                                                                                                                                                                                                                                                          |
| **[Radiancia](#radiancia)**                                     | $\begin{aligned} & L(p, \omega) = \frac{dE_\omega(p)}{d\omega} \\ & L = \frac{d\Phi}{d\omega dA^\bot} \\ & L^+(p, \omega) = \lim_{t \to 0^+}{L(p + t\mathbf{n_p}, \omega)} \\ & L^-(p, \omega)  = \lim_{t \to 0^-}{L(p + t\mathbf{n_p}, \omega)} \end{aligned}$                      |
| **[Radiancia incidente](#radiancia)**                           | $\begin{aligned} L_i(p, \omega) = \begin{cases} L^+(p, -\omega) & \text{si }  \omega \cdot \mathbf{n_p} > 0  \\ L^-(p, -\omega) & \text{si }  \omega \cdot \mathbf{n_p} < 0 \end{cases}\end{aligned}$                                                                                |
| **[Radiancia de salida](#radiancia)**                           | $\begin{aligned} L_o(p, \omega) & = \begin{cases} L^+(p, \omega)  & \text{si }  \omega \cdot \mathbf{n_p} > 0 \\ L^-(p, \omega)  & \text{si }  \omega \cdot \mathbf{n_p} < 0 \end{cases} \end{aligned}$                                                                              |