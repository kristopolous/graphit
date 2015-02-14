graphit: graphit.o
	g++ test.cpp graphit.o -o test -lncursesw
test: winborder.c
	cc -ggdb -gstabs+ winborder.c -lncurses
clean:
	rm *.o
