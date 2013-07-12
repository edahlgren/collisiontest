#!/usr/bin/Rscript

printf <- function(...) invisible(print(sprintf(...)))

args <- commandArgs(trailingOnly = TRUE)
file <- args[1]

df <- read.csv(file=file, header=FALSE, sep=",");
vec <- as.numeric(as.vector(df[1,]))
printf("mean %f", mean(vec))
printf("median %f", median(vec))
printf("variance %f", var(vec))
printf("deviation %f", sd(vec))
printf("max %f", max(vec))
