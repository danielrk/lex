CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall

HWK4= /c/cs323/Hwk4

Lex: $(HWK4)/mainLex.o history.o Histlist.o lex.o
	${CC} ${CFLAGS} -o Lex $(HWK4)/mainLex.o history.o Histlist.o lex.o

history.o : history.c Histlist.h

Histlist.o : Histlist.c Histlist.h $(HWK4)/lex.h

lex.o : lex.c $(HWK4)/lex.h

$(HWK4)/mainLex.o : $(HWK4)/mainLex.c $(HWK4)/lex.h $(HWK4)/history.h $(HWK4)/getLine.h
