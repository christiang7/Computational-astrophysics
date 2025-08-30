#CXX = g++ -O3
#CPFLAGS = -Wall
LIBRARYS = -lX11

OBJECTS = Xgraphics.c
PROJECT = Xgraphicsdemo
CPP = programm.cpp


$(PROJECT) : $(OBJECTS)
	g++ $(CPP) -o $@ $(OBJECTS) ${LIBRARYS} #${CXX} $(CPFLAGS)

.o :#Xgraphics
	g++ -c $(OBJECTS)  # ${CPFLAGS} $?

clean :
	rm -f $(OBJECTS)
