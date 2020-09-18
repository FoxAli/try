
ver=debug

TARGET=husky
SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCES))

CXX:=g++

ifeq ($(ver) , debug)
$(info "debug mode.")
REAL_CXXFLAGS = -O0 -g -Wall
else
$(info "release mode")
 # -DNDEBUG  , compiler will igore ‘assert’
REAL_CXXFLAGS = -O2 -DNDEBUG -Wall
endif

REAL_LDFLAGS=$(LDFLAGS) $(PROF) -rdynamic -lpthread -levent -lcrypt -ldl

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ $(REAL_LDFLAGS)

.cpp.o:
	$(CXX) -c $(REAL_CXXFLAGS) $<

clean:
	-rm -rf $(TARGET) $(OBJS)
	-rm -rf ../$(TARGET)
	-rm *~


