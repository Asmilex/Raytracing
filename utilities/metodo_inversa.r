F <- function(xi, lambda) {
  -log(xi) / lambda
}

N <- 1000
xi <- runif(N)
lambda <- 1.5
x <- F(xi, lambda)

png(file = "./metodo_inversa.png", width = 900, height = 700)
hist(x, freq = FALSE, breaks = 'FD', main = 'Método de la inversa para la exponencial', ylim = c(0, 1.5))
lines(density(x), col = 'blue')
curve(dexp(x, rate = lambda), add = TRUE, col = 2)
dev.off()
