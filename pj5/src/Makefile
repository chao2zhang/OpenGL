OBJECTS = main.o base.o color.o object.o
TARGET = proj5
proj5: $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -lglut -lGLU
main.o : main.cpp base.h color.h object.h
	g++ -c $<
base.o : base.cpp
	g++ -c $<
color.o : color.cpp base.h
	g++ -c $<
object.o : object.cpp base.h color.h
	g++ -c $<
clean : 
	rm $(TARGET) $(OBJECTS)
