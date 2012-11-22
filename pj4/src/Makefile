OBJECTS = main.o curve.o point.o utils.o
TARGET = proj4
proj4: $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -lglut -lGLU
main.o : main.cpp point.h curve.h utils.h
	g++ -c $<
point.o : point.cpp point.h
	g++ -c $<
curve.o : curve.cpp point.h curve.h
	g++ -c $<
utils.o : utils.cpp utils.h point.h curve.h
	g++ -c $<
clean : 
	rm $(TARGET) $(OBJECTS)
