graphit: graphit.o
	g++ test.cpp graphit.o -o test -lncursesw
clean:
	rm *.o
