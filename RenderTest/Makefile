TARGET := RenderTest

SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,obj/%.o,$(SOURCES))
DEBUGOBJS := $(patsubst %.cpp,obj/debug/%.o,$(SOURCES))

IRRLICHT := ../../irrlicht
RENDERER := ..

CPPFLAGS := -I$(RENDERER)/include -Iinclude -Wall -O2 -ffast-math -MMD -fPIC -I$(IRRLICHT)/include -I/usr/X11R6/include
DEBUGFLAGS = -g

LDFLAGS := -L$(IRRLICHT)/lib/Linux -lIrrlicht -lirrRenderer -lXxf86vm -lX11 -lGL -lEGL -lXext -lXcursor


all: release

debug: $(DEBUGOBJS)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) $^ -o ../bin/$(TARGET) $(LDFLAGS) -L$(RENDERER)/lib/linux/debug

release: $(OBJS)
	g++ $(CPPFLAGS) $^ -o ../bin/$(TARGET) $(LDFLAGS) -L$(RENDERER)/lib/linux/release

obj/%.o: %.cpp
	mkdir -p $$(dirname $@)
	g++ $(CPPFLAGS) -c -o $@ $<

obj/debug/%.o: %.cpp
	mkdir -p $$(dirname $@)
	g++ $(CPPFLAGS) $(DEBUGFLAGS) -c -o $@ $<

-include $(OBJS:.o=.d)

clean:
	rm -rf obj


.PHONY: all debug release clean
