CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = word_counter
SOURCES = main.cpp word_counter.cpp
HEADERS = word_counter.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean