graphit: graphit.o
	g++ test.cpp graphit.o -o test
test: winborder.c
	cc -ggdb -gstabs+ winborder.c -lncurses
clean:
	rm *.o
