
src = test/test.cpp 

all: tests

tests: $(src)
	g++ -std=c++17 $(src) -o tests
	./tests
	rm -f tests

.PHONY : clean
clean:
	rm -f *.out
	rm -f tests
