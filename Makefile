compiler = g++ -g -Wall -std=c++11

bin/mdToHTML.exe: bin/mdToHTML.o bin/blockFilter.o bin/lineFilter.o bin/wordFilter.o bin/other.o
	${compiler} -o $@ $^

bin/%.o: src/%.cpp src/filter.h
	${compiler} -o $@ -c $<
