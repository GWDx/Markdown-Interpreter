compiler = g++ -g -Wall -std=c++11

all: bin/mdToHTML.exe bin/mdToHTMLGUI.exe

bin/mdToHTML.exe: bin/mdToHTML.o bin/blockFilter.o bin/lineFilter.o bin/wordFilter.o bin/other.o
	${compiler} -o $@ $^

bin/%.o: src/%.cpp src/filter.h
	${compiler} -o $@ -c $<

bin/mdToHTMLGUI.exe: src/mdToHTMLGUI.py
	pyinstaller -w -F $^
	move "dist\\mdToHTMLGUI.exe" bin\\
	del *.spec
	rd /s /q build dist

clean:
	del bin//*.exe bin//*.o
