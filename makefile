CXX=g++
CXXLINK=$(CXX)
CXXFLAGS=-std=c++17 -Wall -O2
LIBRAYFLAGS= -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
OBJS = game.o main.o graphics.o
RM= rm -rf *.o chess

#chess.exe: main.o
#	$(CXX) $(CXXFLAGS) $(LIBRAYFLAGS) -L/usr/local/lib -lraylib $(OBJS) -o chess.exe 
chess: main.o
	$(CXX) $(CXXFLAGS) $(OBJS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o chess

main.o: game.o graphics.o main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

graphics.o: game.o graphics.hpp graphics.cpp
	$(CXX) $(CXXFLAGS) -c graphics.cpp

game.o: game.hpp game.cpp
	$(CXX) $(CXXFLAGS) -c game.cpp

clean:
	$(RM)