OBJECTS = dataManager.o console.o point.o line.o polygon.o utils.o matrix.o main.o
TARGET = proj2
proj2: $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -lglut -lGLU
main.o : main.cpp gl.h console.h dataManager.h
	g++ -c $<
point.o : point.cpp point.h
	g++ -c $<
line.o : line.cpp line.h point.h
	g++ -c $<
polygon.o : polygon.cpp polygon.h line.h matrix.h utils.h
	g++ -c $<
dataManager.o : dataManager.cpp dataManager.h gl.h
	g++ -c $<
console.o : console.cpp console.h gl.h hint.h
	g++ -c $<
matrix.o : matrix.cpp matrix.h
	g++ -c $<
utils.o : utils.cpp utils.h point.h
	g++ -c $<

clean : 
	rm $(TARGET) $(OBJECTS)
