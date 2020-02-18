GPP = g++
OPTIONS = -std=c++17

#-mwindows

all:
	${GPP} ${OPTIONS} fake_file_generator.cpp -o prog.exe
