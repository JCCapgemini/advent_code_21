PQ_PATH = ../binary_heap
PQ_DEPS = $(PQ_PATH)/pq.c
SRC=main.c $(PQ_DEPS)
OBJ = $(SRC:.c=.o)
OBJD = $(SRC:.c=.d.o)

GLIB_FLAGS=`pkg-config --libs glib-2.0`
CFLAGS=-Wall -std=c11 `pkg-config --cflags glib-2.0` -O2 -lgmp -I$(PQ_PATH)

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
