compiler = g++ -g -Wall -std=c++11

all: ../bin/mdToHTML.exe ../bin/mdToHTMLGUI.exe

../bin/mdToHTML.exe: ../bin/mdToHTML.o ../bin/blockFilter.o ../bin/lineFilter.o ../bin/wordFilter.o ../bin/other.o ../bin/replaceSplit.o
	${compiler} -o $@ $^

../bin/%.o: %.cpp filter.h
	${compiler} -o $@ -c $<

../bin/mdToHTMLGUI.exe: ../mdToHTMLGUI.py ../uploadFTP.py
	pyinstaller -w -F $<
	move "dist\\mdToHTMLGUI.exe" ..\\bin\\
	del *.spec
	rd /s /q build dist

clean:
	del bin/*.exe bin/*.o
