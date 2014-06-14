SRCS = $(wildcard *.cpp)
HEADERS = Definition.h LogDebug.h
OBJECTS = $(SRCS:.cpp=.o)
TARGETS = yaush
CXX = g++
#CXXFLAGS = -Wall -DDEBUG -g3
CXXFLAGS = -Wall
LFLAGS = -lreadline

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) $(LFLAGS) -o $@

$(OBJECTS): %.o: %.cpp %.h $(HEADERS)
	$(CXX) $< $(CXXFLAGS) -c

clean:
	-rm -f $(TARGETS) $(OBJECTS)
