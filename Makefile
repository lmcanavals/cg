CC  := clang
CXX := clang++
CP  := cp
RM  := rm -rf
MD  := mkdir -p

CXXFLAGS += -std=c++20 -pedantic -Wall -O3
LDFLAGS  += -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp

CPPFLAGS += -Iinclude

src/%.: src/%/*.cpp dirs build/glad.o
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o build/main.o $<
	$(CXX) -o bin/prog.out build/*.o $(LDFLAGS)
	$(CP) $(shell dirname $<)/*.vert bin/ 2>/dev/null || echo ''
	$(CP) $(shell dirname $<)/*.frag bin/ 2>/dev/null || echo ''

build/glad.o:
	$(CC) -c $(CPPFLAGS) -o build/glad.o src/glad.c
	$(CC) -c $(CPPFLAGS) -o build/stb_image.o src/stb_image.c

dirs: build bin

build:
	@$(MD) build

bin:
	@$(MD) bin

clean:
	@echo Removing build files
	$(RM) build bin

