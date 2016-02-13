CFLAGS=-ggdb

all: diskSim diskSimFinal

filesystemsim.o: filesystemsim.cpp diskSim.h
	g++ -c filesystemsim.cpp

ldisk.o: ldisk.cpp diskSim.h
	g++ -c ldisk.cpp

main.o: main.cpp diskSim.h
	g++ -c main.cpp

diskSimFinal: main.cpp ldisk.o main.o filesystemsim.o
	g++ $(CFLAGS) main.o ldisk.o filesystemsim.o -DDEBUG_BUILD=0 -o diskSimFinal
	
diskSim: main.cpp ldisk.o main.o filesystemsim.o
	g++ $(CFLAGS) main.o ldisk.o filesystemsim.o -DDEBUG_BUILD=1 -o diskSim

clean:
	rm *.o diskSim diskSimFinal
