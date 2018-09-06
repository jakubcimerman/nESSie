all:
	mkdir -p results
	g++ -c sorter.cpp
	g++ -c rotator.cpp
	g++ -c checker.cpp
	g++ *.o -o main.exe -mcmodel=large main.cpp
clean:
	rm -f *.o