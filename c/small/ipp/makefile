CC       = gcc
CPPFLAGS =
CFLAGS   = -Wall -Wextra -std=c11 -O2
LDFLAGS  =

.PHONY: all clean

all: similar_lines
run:
	gcc -Wall -Wextra -std=c11 -O2 -o similar_lines main.c String_vector.c Wiersz.c Wiersz_vector.c LL_vector.c LD_vector.c Auxilary_functions.c
	./similar_lines

val:
	gcc -Wall -Wextra -std=c11 -O2 -o similar_lines main.c String_vector.c Wiersz.c Wiersz_vector.c LL_vector.c LD_vector.c Auxilary_functions.c
	valgrind --leak-check=full -s --track-origins=yes ./similar_lines
similar_lines: Wiersz.o String_vector.o Wiersz_vector.o main.o LL_vector.o LD_vector.o Auxilary_functions.o
	gcc -Wall -Wextra -std=c11 -O2 -o similar_lines main.c String_vector.c Wiersz.c Wiersz_vector.c LL_vector.c LD_vector.c Auxilary_functions.c
Wiersz.h: String_vector.h LD_vector.h LL_vector.h
Wiersz_vector.o: Wiersz_vector.h Wiersz_vector.c
Wiersz.o: Wiersz.h Wiersz.c
String_vector.o: String_vector.c String_vector.h
LL_vector.o: LL_vector.c LL_vector.h
LD_vector.o: LD_vector.c LD_vector.h
Auxilary_functions.o: Auxilary_functions.c Auxilary_functions.h
main.o: main.c Wiersz.h String_vector.h Wiersz_vector.h LL_vector.h LD_vector.h Auxilary_functions.h

clean:
	rm -f *.o similar_lines temp.out temp.err
