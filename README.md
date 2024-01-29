# Advent of code 2021 in C

My small and very merciless attempt to complete as much as I can of the *advent
of code 2021* by using pure C. I will allow myself to use common C libraries in
the extent possible, like GLIB.

## How to

For the project to work one needs to have installed all required libraries.
For execution of a test:
```
make test
```
This will compile and execute the test. Will give error if test is incorrect.
For more output you can execute **testd** where it will be interpreted as
test debug and give all output.

For processing the actual input, download the input from the page and save it
as **actual.txt** in the folder. Then for execution:
```
make actual
```

If one wants to debug with **GDB** is already setted up for that. Just execute
```
make debug
```
and it will run the debug compilation and give you the gdb for execution. The
way to start **GDB** will be similar to:
```
(gdb) run < test.txt
```
and will launch the opeartion with the test file.

## Libs used
GLIB

## References
C
- https://en.cppreference.com/w/c
GLIB
- https://developer-old.gnome.org/glib/unstable/
- https://docs.gtk.org/glib/index.html
