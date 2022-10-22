all: logging
	g++ main.cpp Obj/logging.o -o Exe\Start.exe 

create_dirs:
	mkdir Exe
	mkdir Obj

logging:
	g++ -c Libs/Logging/Logging.cpp -o Obj/Logging.o