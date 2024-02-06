GLIB_FLAGS=`pkg-config --libs glib-2.0`
CFLAGS=-Wall -std=c11 `pkg-config --cflags glib-2.0` -O2
DEB_CFLAGS=-ggdb3
OBJ=main.o
OBJD=maind.o

program: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(GLIB_FLAGS) -o program

main.o: main.c
	$(CC) $(CFLAGS) $(GLIB_FLAGS) -c main.c -o main.o

programd: $(OBJD)
	$(CC) $(OBJD) $(DEB_CFLAGS) $(CFLAGS) $(GLIB_FLAGS) -o programd

maind.o: main.c
	$(CC) $(DEB_CFLAGS) $(CFLAGS) $(GLIB_FLAGS) -c main.c -o maind.o

test: program
	./program < test.txt | tail -1 | diff --strip-trailing-cr -w testr.txt -

testd: program
	./program < test.txt

debug: programd
	gdb programd

actual: program
	./program < actual.txt

.PHONY = clean
clean:
	rm -f $(OBJ) $(OBJD) program programd
