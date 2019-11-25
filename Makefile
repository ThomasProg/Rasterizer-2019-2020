MAKEFLAGS+=--no-builtin-rules --no-builtin-variables

CXXFLAGS=-O0 -Wall -MMD -Wno-unused-function
CXXFLAGS+=-Iinclude -ItestMains -Iinclude/maths -Iinclude/render
CFLAGS=$(CXXFLAGS)
CC=gcc
CXX=g++

LDLIBS=-Llib -lglfw -lSDL2 -lSDL2main -lGL -lGLU -ldl
SRC=$(wildcard src/*.cpp) $(wildcard src/maths/*.cpp) $(wildcard src/render/*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: exe

-include $(DEPS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

exe: $(OBJS)
	$(CXX) -o $@ $(LDFLAGS) -pg $^ $(LDLIBS)

clean:
	rm -f $(DEPS) $(OBJS)  exe