#!/usr/bin/env Rscript

result <- system(paste("./test-needlet 2.50 10 needlet-2.50",
                       "./test-needlet 2.73 10 needlet-2.73", sep = " && "),
                 wait = TRUE, intern = FALSE)
if (result != 0) {
  cat("Unable to create the needlet files\n")
  q()
}

data2.50 <- matrix(nrow=1000, ncol=10)
data2.73 <- matrix(nrow=1000, ncol=10)

for (i in 1:10) {
  d <- read.table(paste("~/work/needlet_decomp/needlet-2.50-",
			sprintf("%02d", i),
			".txt", sep = ""),
		  col.names = c("l", "w"))
  data2.50[,i] <- d$w

  d <- read.table(paste("~/work/needlet_decomp/needlet-2.73-",
			sprintf("%02d", i),
			".txt", sep = ""),
		  col.names = c("l", "w"))
  data2.73[,i] <- d$w
}

pdf("window-function-plot.pdf", width = 7, height = 9)
j.to.plot <- 1:6

par(mfrow = c(length(j.to.plot), 1),
    mar = c(2, 4, 1, 2))

for (j in j.to.plot) {
  last <- (j == j.to.plot[length(j.to.plot)])
  plot(data2.50[,j],
       type = "l",
       xlab = if (last) "l" else "",
       ylab = "Window function",
       mar = c(if (last) 3 else 2, 4, 0, 2))
  lines(data2.73[,j], lty = 2)
  text(1000, 1, paste("j =", j), adj = c(1, 1))
}

r <- dev.off()
