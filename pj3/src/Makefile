OBJECTS = dataManager.o console.o color.o point.o object.o utils.o matrix.o main.o
TARGET = proj3
proj3: $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) -lglut -lGLU
main.o : main.cpp gl.h console.h dataManager.h hint.h object.h
	g++ -c $<
point.o : point.cpp point.h utils.h
	g++ -c $<
color.o : color.cpp color.h utils.h
	g++ -c $<
object.o : object.cpp object.h utils.h matrix.h
	g++ -c $<
dataManager.o : dataManager.cpp dataManager.h gl.h
	g++ -c $<
console.o : console.cpp console.h hint.h
	g++ -c $<
matrix.o : matrix.cpp matrix.h
	g++ -c $<
utils.o : utils.cpp utils.h point.h
	g++ -c $<

clean : 
	rm $(TARGET) $(OBJECTS)
