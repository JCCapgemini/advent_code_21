GLIB_FLAGS=`pkg-config --libs glib-2.0`
CFLAGS=-Wall -std=c11 `pkg-config --cflags glib-2.0` -O2 -lgmp
SRC=main.c pq.c
OBJ = $(SRC:.c=.o)
OBJD = $(SRC:.c=.d.o)

program: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(GLIB_FLAGS) -o program

programd: $(OBJD)
	$(CC) $(OBJD) -g $(CFLAGS) $(GLIB_FLAGS) -o programd

%.o: %.c
	$(CC) $(CFLAGS) $(GLIB_FLAGS) -c $< -o $@ 

%.d.o: %.c
	$(CC) -g $(CFLAGS) $(GLIB_FLAGS) -c $< -o $@ 

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
