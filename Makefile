# Proje derleme ve calistirma Makefile dosya altyapisi

all: compile run

compile:
	gcc main.c -o test.exe -I"C:\SFML\include" -L"C:\SFML\lib" -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -mconsole

run:
	.\test.exe

clean:
	del test.exe