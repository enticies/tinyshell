tinyshell: src/main.o src/util.o
	g++ src/main.o src/util.o -o tinyshell

main.o: main.cpp
	g++ -c src/main.cpp

util.o: util.cpp
	g++ -c src/util.cpp

clean:
	rm *.o