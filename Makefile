MAKEFLAGS+=--no-builtin-rules --no-builtin-variables

CXXFLAGS=-O0 -g -Wall -MMD -Wno-unused-function
CXXFLAGS+=-Iengine/include -Idemo/include -Iengine/include/core -Iengine/include/core/maths\
		  -Iengine/include/resources -Iengine/include/renderer -Iengine/include/physics -Iengine/include/physics/shapes\
		  -Iengine/include/core/debug -Iengine/include/core/dataStructure -Iengine/include/primitives\
		  -Iengine/include/entities -Iengine/include/controllers -Iengine/include/save -Iengine/include/imgui

CFLAGS=$(CXXFLAGS)
CC=gcc
CXX=g++

LDLIBS= -Llib -lglfw -lGL -ldl -msse2
SRC=$(wildcard demo/src/*.cpp)\
	$(wildcard engine/src/*.cpp)\
	$(wildcard engine/src/core/*.cpp)\
	$(wildcard engine/src/core/maths/*.cpp)\
	$(wildcard engine/src/core/debug/*.cpp)\
	$(wildcard engine/src/resources/*.cpp)\
	$(wildcard engine/src/renderer/*.cpp)\
	$(wildcard engine/src/physics/*.cpp)\
	$(wildcard engine/src/core/dataStructure/*.cpp)\
	$(wildcard engine/src/primitives/*.cpp)\
	$(wildcard engine/src/entities/*.cpp)\
	$(wildcard engine/include/imgui/*.cpp)\
	$(wildcard engine/src/save/*.cpp)
SRC_C=
OBJS=$(patsubst %.cpp, %.o, $(SRC)) $(patsubst %.c, %.o, $(SRC_C)) engine/src/glad.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: bin/exe

-include $(DEPS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

bin/exe: $(OBJS) 
	mkdir -p bin
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -f $(DEPS) $(OBJS) gmon.out perf bin/exe

gmon: 
	gprof bin/exe gmon.out > perf

threading:
	make -j

run: bin/exe
	./bin/exe