CFLAGS=-g3 -O2
CC = gcc

all: chaining linear quadratic double robinhood

chaining: chaining.c harness.c lookup3.c
	${CC} ${CFLAGS} -o chaining chaining.c harness.c lookup3.c

linear: linear.c harness.c lookup3.c
	${CC} ${CFLAGS} -o linear linear.c harness.c lookup3.c

quadratic: quadratic.c harness.c lookup3.c
	${CC} ${CFLAGS} -o quadratic quadratic.c harness.c lookup3.c

double: double.c harness.c lookup3.c
	${CC} ${CFLAGS} -o double double.c harness.c lookup3.c

robinhood: robinhood.c harness.c lookup3.c
	${CC} ${CFLAGS} -o robinhood robinhood.c harness.c lookup3.c

clean:
	rm chaining linear quadratic double robinhood
