CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
LIBRAYFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LDFLAGS = -L/usr/local/lib

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/chess

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LIBRAYFLAGS) $(LDFLAGS) \
	-lraylib $(OBJDIR)/*.o -o $(TARGET)
	
#$(TARGET): main.o
#	$(CXX) $(CXXFLAGS) $(OBJS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o chess

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINDIR): 
	@mkdir -p $(BINDIR)

$(OBJDIR): 
	@mkdir -p $(OBJDIR)

run: all
	./$(TARGET)

crun: clean all
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)