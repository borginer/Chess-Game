CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -I/opt/homebrew/Cellar/catch2/3.7.1/include/
LIBRAYFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LDFLAGS = -L/usr/local/lib

SRCDIR = src
ENGINEDIR = $(SRCDIR)/engine
INTERFACEDIR = $(SRCDIR)/interface
OBJDIR = obj
BINDIR = bin
TARGET = $(BINDIR)/chess

SRC_FILES = $(wildcard $(ENGINEDIR)/*.cpp) $(wildcard $(INTERFACEDIR)/*.cpp)
OBJ_FILES = $(patsubst $(ENGINEDIR)/%.cpp,$(OBJDIR)/engine/%.o,$(wildcard $(ENGINEDIR)/*.cpp)) \
            $(patsubst $(INTERFACEDIR)/%.cpp,$(OBJDIR)/interface/%.o,$(wildcard $(INTERFACEDIR)/*.cpp))

all: chess_game

chess_game: $(OBJ_FILES) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LIBRAYFLAGS) $(LDFLAGS) \
	-lraylib $(OBJ_FILES) $(SRCDIR)/main.cpp -o $(TARGET)

tests: $(OBJ_FILES) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LIBRAYFLAGS) $(LDFLAGS) \
	-lraylib $(OBJ_FILES) $(SRCDIR)/tests.cpp -o $(BINDIR)/tests
	./$(BINDIR)/tests

$(OBJDIR)/engine/%.o: $(ENGINEDIR)/%.cpp | $(OBJDIR)/engine
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/interface/%.o: $(INTERFACEDIR)/%.cpp | $(OBJDIR)/interface
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR)/engine:
	@mkdir -p $(OBJDIR)/engine

$(OBJDIR)/interface:
	@mkdir -p $(OBJDIR)/interface

run: all
	./$(TARGET)

crun: clean all
	./$(TARGET)

ctest: clean tests

clean:
	rm -rf $(OBJDIR) $(BINDIR) *.o