CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall

HWK4= /c/cs323/Hwk4

Lex: $(HWK4)/mainLex.o history.o Histlist.o lex.o
	${CC} ${CFLAGS} -o Lex $(HWK4)/mainLex.o history.o Histlist.o lex.o

history.o : history.c Histlist.h

lex.o : lex.c $(HWK4)/lex.h

