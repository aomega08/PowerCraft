SRC := $(shell find . -name "*.cpp")
OBJS := $(SRC:.cpp=.o)

all: stdafx.h.pch $(OBJS)
	g++ $(OBJS) -o PowerCraft

stdafx.h.pch: stdafx.h
	g++ -emit-pch stdafx.h -o $@

.cpp.o: 
	g++ -include-pch stdafx.h.pch $< -o $@ -c

clean:
	rm -f stdafx.h.pch $(OBJS) PowerCraft

.PHONY: stdafx.h.pch

