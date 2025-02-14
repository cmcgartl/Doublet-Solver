compiler = g++
flags = -g -Wall -std=c++17
compile = $(compiler) $(flags)

doublet: doublet.cpp 
	$(compile) doublet.cpp -o doublet

.PHONY: clean
clean:
	rm -rf *.o doublet
