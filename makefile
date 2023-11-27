CXX=g++
CXXLINK=$(CXX)
CXXFLAGS=-std=c++17 -Wall
LIBRAYFLAGS= -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
OBJS = game.o main.o
RM= rm -rf *.o chess.exe

chess.exe: main.o
	$(CXX) $(CXXFLAGS) $(LIBRAYFLAGS) -L/usr/local/lib -lraylib $(OBJS) -o chess.exe 

main.o: game.o main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

game.o: game.hpp game.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

clean:
	$(RM)