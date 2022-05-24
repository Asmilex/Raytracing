a <- 2
b <- 6
resultado <- optimize(
    f = function(x) { dbeta(x, shape1 = a, shape2 = b) },
    maximum = TRUE,
    interval = c(0, 1)
)

M <- resultado$objective

png(file = "./metodo_rechazo_grafica.png", width = 900, height = 700)
curve(dbeta(x, shape1 = a, shape2 = b), 0, 1)
curve(M * dunif(x), 0, 1, add = TRUE, col = 2, lty = 2)
legend('right',
    legend = c('p_X(x): Beta(a = 2, b = 6)', 'p_Y * M'),
    col = c(1, 2), lty = c(1, 2), bty = 'n'
)
dev.off()

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

valores_generados

png(file = "./metodo_rechazo.png", width = 900, height = 700)
hist(x, freq = FALSE, breaks = 'FD', main = 'Método del rechazo para la distribución Beta(a = 2, b = 6)')
lines(density(x), col = 'blue')
curve(dbeta(x, shape1 = a, shape2 = b), add = TRUE, col = 2)
dev.off()
