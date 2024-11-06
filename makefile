CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
LIBRAYFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LDFLAGS = -L/usr/local/lib

SRCDIR = src
ENGINEDIR = $(SRCDIR)/engine
INTERFACEDIR = $(SRCDIR)/interface
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(ENGINEDIR)/*.cpp) $(wildcard $(INTERFACEDIR)/*.cpp)
OBJECTS = $(patsubst $(ENGINEDIR)/%.cpp, $(OBJDIR)/engine/%.o, $(wildcard $(ENGINEDIR)/*.cpp)) \
          $(patsubst $(INTERFACEDIR)/%.cpp, $(OBJDIR)/interface/%.o, $(wildcard $(INTERFACEDIR)/*.cpp))
TARGET = $(BINDIR)/chess

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LIBRAYFLAGS) $(LDFLAGS) \
	-lraylib $(OBJECTS) -o $(TARGET)
	
#$(TARGET): main.o
#	$(CXX) $(CXXFLAGS) $(OBJECTS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o chess

# Pattern rules for objects in nested directories
$(OBJDIR)/engine/%.o: $(ENGINEDIR)/%.cpp | $(OBJDIR)/engine
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/interface/%.o: $(INTERFACEDIR)/%.cpp | $(OBJDIR)/interface
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/engine:
	@mkdir -p $(OBJDIR)/engine

$(OBJDIR)/interface:
	@mkdir -p $(OBJDIR)/interface

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