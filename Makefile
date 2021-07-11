compiler = g++ -g -Wall -std=c++11

all: bin/mdToHTML.exe bin/mdToHTMLGUI.exe

bin/mdToHTML.exe: main.cpp
	${compiler} -o $@ $^

bin/mdToHTMLGUI.exe: mdToHTMLGUI.py uploadFTP.py
	pyinstaller -w -F $<
	move "dist\\mdToHTMLGUI.exe" bin\\
	del *.spec
	rd /s /q build dist

clean:
	del bin/*.exe bin/*.o
