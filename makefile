CXX = g++
CXXFLAGS = -O3 -w -I./include

# Sources
SRC = ./src/rrr-vector.cpp ./include/bitvector.cpp
TEST_SRC = ./unit-tests/unit.cpp

# Outputs
#OUT = build/rrr-vector
TEST_OUT = build/unit-test

.PHONY: all clean

all: $(OUT) $(TEST_OUT)

# Build the main executable
#$(OUT): $(SRC)
#	mkdir -p build
#	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Build the unit test executable
$(TEST_OUT): $(TEST_SRC) $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $(TEST_OUT) $(TEST_SRC) $(SRC)

clean:
	rm -rf build