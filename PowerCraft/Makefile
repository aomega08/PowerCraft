SRC := $(shell find . -name "*.cpp")
OBJS := $(SRC:.cpp=.o)

TOOLCHAIN := $(HOME)/.clibs
INCLUDES := $(TOOLCHAIN)/includes
LIBS := $(TOOLCHAIN)/lib

CXXFLAGS := -include-pch stdafx.h.pch -I$(INCLUDES) -Wall -c
LINKFLAGS := -L$(LIBS) -lglfw3 -lGLEW -lSOIL

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  LINKFLAGS += -framework cocoa -framework iokit -framework corevideo -framework opengl
endif

all: stdafx.h.pch $(OBJS)
	g++ $(OBJS) -o PowerCraft $(LINKFLAGS)

stdafx.h.pch: stdafx.h
	g++ -emit-pch stdafx.h -o $@ -I$(INCLUDES)

.cpp.o: 
	g++ $(CXXFLAGS) $< -o $@

clean:
	rm -f stdafx.h.pch $(OBJS) PowerCraft


