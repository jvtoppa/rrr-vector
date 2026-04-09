CXX = g++
CXXFLAGS = -O3 -w

SRC = ./src/rrr-vector.cpp ./include/bitvector.cpp
OUT = build/rrr-vector

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)