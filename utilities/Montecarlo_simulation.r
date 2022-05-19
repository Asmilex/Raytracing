f <- function(x) {
    2 * x^4 * (x > 0 & x < 1)
}

curve(f, 0, 1)

N <- 1000
x <- runif(N) # x1, ...., xn
f_x <- sapply(x, f) # f(x1), ..., f(xn)
mean(f_x)

# Calcular la media y su error
estimacion <- cumsum(f_x) / (1:N)
error <- sqrt(cumsum((f_x - estimacion)^2)) / (1:N)

# Gráfico
png(file = "./error.png", width = 900, height = 700)
plot(1:N, estimacion,
    type = "l",
    ylab = "Aproximación y límites del error (1 - alpha = 0.975)",
    xlab = "Número de simulaciones",
)
z <- qnorm(0.025, lower.tail = FALSE)
lines(estimacion - z * error, col = "blue", lwd = 2, lty = 3)
lines(estimacion + z * error, col = "blue", lwd = 2, lty = 3)
abline(h = 0.4, col = 2)
dev.off()
