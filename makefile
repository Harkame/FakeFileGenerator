GPP = g++
OPTIONS = -std=c++17 -O3 -W -Wall -Weffc++ -Werror -pedantic

#-mwindows

all:
	${GPP} ${OPTIONS} src/ffg.cpp -o ffg.exe
