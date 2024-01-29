GLIB_FLAGS=`pkg-config --libs glib-2.0`
CFLAGS=-Wall -std=c11 `pkg-config --cflags glib-2.0` -O2 --static
OBJ=main.o

program: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(GLIB_FLAGS) -o program

main.o: main.c
	$(CC) $(CFLAGS) $(GLIB_FLAGS) -c main.c -o main.o

test: program
	./program < test.txt | diff --strip-trailing-cr -w testr.txt -

actual: program
	./program < actual.txt
