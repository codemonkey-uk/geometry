
src = test/test.cpp 

all: tests

tests: $(src)
	g++ -std=c++17 $(src) -o test/test.out
	./test/test.out
	rm -f test/test.out

.PHONY : clean
clean:
	rm -f *.out
	rm -f testest/test.out
