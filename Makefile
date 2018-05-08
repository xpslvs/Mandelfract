
# Makefile Mandelfract

CXX      := g++
CXXFLAGS := -std=c++17 -Ofast -Wall -pedantic -march=native -flto -fno-math-errno -fassociative-math -freciprocal-math -fno-signed-zeros -fno-trapping-math -fcx-fortran-rules -frename-registers -funroll-loops -ftracer
LDFLAGS  := -lpthread -lSDL2 -lSDL2_ttf $(CXXFLAGS)
ELF      := mandelfract
EXE      := mandelfract.exe
SOURCES  := $(wildcard src/*.cc)
OBJECTS  := $(patsubst src/%.cc, obj/%.cc.o, $(SOURCES))

.PHONY: linux windows clean rebuild 

linux: $(ELF)

windows: $(EXE)

clean:
	$(RM) obj/*.o

rebuild: clean
	make

$(ELF): $(OBJECTS) Makefile
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

$(EXE): $(OBJECTS) resources/bin/mandelfract.res Makefile
	$(CXX) -o $@ $(OBJECTS) resources/bin/mandel.res $(LDFLAGS) -mwindows

resources/bin/mandelfract.res: resources/mandelfract.rc mandelfract.ico Makefile
	windres $< -O coff $@

obj/%.cc.o: src/%.cc Makefile
	$(CXX) -o $@ $< $(CXXFLAGS) -c 

