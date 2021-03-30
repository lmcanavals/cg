CC  := gcc
CXX := g++
CP  := cp
RM  := rm -rf
MD  := mkdir -p

CXXFLAGS += -std=c++17 -pedantic -Wall -O3
LDFLAGS += -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

CPPFLAGS += -Iinclude

all: dirs bin/program

%.: src/%/*.cpp dirs
	$(CC) -c $(CPPFLAGS) -o build/glad.o src/glad.c
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o build/main.o $<
	$(CXX) -o bin/program build/*.o $(LDFLAGS)
	$(CP) $(shell dirname $<)/*.vert .
	$(CP) $(shell dirname $<)/*.frag .

dirs:
	@echo Creating folders
	@$(MD) build
	@$(MD) bin

clean:
	@echo Removing build files
	$(RM) build bin *.vert *.frag

