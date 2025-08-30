
CXX = g++  -O3 
PROJECT = planet
CPPLIBS = -lstdc++ -L/usr/X11R6/lib -lXt -lX11
OBJECTS = $(PROJECT).o Xgraphics.o output.o euler.o initial.o calcenergy.o

$(PROJECT) : $(OBJECTS)
	@echo Link project $(@)
	$(CXX) $(CPPLIBS) -o $(@) $(OBJECTS)

.cpp.o : 
	$(CXX) $(CPPFLAGS) -c $?

.c.o : 
	$(CXX) -c $?

clean :
	rm -f $(OBJECTS)
