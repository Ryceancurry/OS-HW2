CFLAGS=-ggdb

all: diskSim diskSimFinal

filesystemsim.o: filesystemsim.cpp diskSim.h
	g++ -c filesystemsim.cpp

filesystemutil.o: filesystemutil.cpp diskSim.h
	g++ -c filesystemutil.cpp

ldisk.o: ldisk.cpp diskSim.h
	g++ -c ldisk.cpp

main.o: main.cpp diskSim.h
	g++ -c main.cpp

diskSimFinal: main.cpp ldisk.o main.o filesystemsim.o filesystemutil.o
	g++ $(CFLAGS) main.o ldisk.o filesystemsim.o filesystemutil.o -o diskSimFinal
	
diskSim: main.cpp ldisk.o main.o filesystemsim.o filesystemutil.o
	g++ $(CFLAGS) main.o ldisk.o filesystemsim.o filesystemutil.o -D DEBUG_BUILD -o diskSim

clean:
	rm *.o diskSim diskSimFinal
