all: logging swap
	g++ main.cpp Obj/Logging.o Obj/Swap.o -o Exe\Start.exe 

create_dirs:
	mkdir Exe
	mkdir Obj
	mkdir GraphicDumps

logging:
	g++ -c Libs/Logging/Logging.cpp -o Obj/Logging.o

swap:
	g++ -c Libs/Swap/Swap.cpp -o Obj/Swap.o