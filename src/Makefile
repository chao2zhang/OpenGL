OBJECTS = dataManager.o gl.o polygon.o main.o
TARGET = proj1
proj1: $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -lglut
main.o : main.cpp gl.h polygon.h dataManager.h
	g++ -c $<
gl.o : gl.cpp gl.h
	g++ -c $<
polygon.o : polygon.cpp polygon.h gl.h
	g++ -c $<
dataManager.o : dataManager.cpp dataManager.h gl.h
	g++ -c $<
clean : 
	rm $(TARGET) $(OBJECTS)
