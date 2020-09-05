

TARGET=husky
SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCES))

CXX:=g++
OPTIMIZATION?=-O2 -DNDEBUG
WARNINGS=-Wall
REAL_CXXFLAGS=$(OPTIMIZATION) $(CXXFLAGS) $(WARNINGS)
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


