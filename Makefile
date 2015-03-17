TARGET := irrRenderer

SOURCES := $(wildcard source/*.cpp)
OBJS := $(patsubst source/%.cpp,obj/%.o,$(SOURCES))
DEBUGOBJS := $(patsubst source/%.cpp,obj/debug/%.o,$(SOURCES))

IRRLICHT := ../irrlicht

CPPFLAGS := -Iinclude/ -Wall -O2 -ffast-math -MMD -fPIC -I$(IRRLICHT)/include -I/usr/X11R6/include
DEBUGFLAGS = -g


all: release

debug: $(DEBUGOBJS)
	mkdir -p lib/linux/debug
	ar -rs lib/linux/debug/lib$(TARGET).a $^

release: $(OBJS)
	mkdir -p lib/linux/release
	ar -rs lib/linux/release/lib$(TARGET).a $^

obj/%.o: source/%.cpp
	mkdir -p $$(dirname $@)
	g++ $(CPPFLAGS) -c -o $@ $<

obj/debug/%.o: source/%.cpp
	mkdir -p $$(dirname $@)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -c -o $@ $<

-include $(OBJS:.o=.d)

clean:
	rm -rf obj


.PHONY: all debug release clean
