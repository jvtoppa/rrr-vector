CXX = g++
CXXFLAGS = -O3 -w -I./include

CORE_SRC = ./src/rrr-vector.cpp ./include/bitvector.cpp

TEST_SRC = ./unit-tests/unit.cpp
RANK0_SRC = ./benchmarks/rank0.cpp
RANK1_SRC = ./benchmarks/rank1.cpp
ACCESS_SRC = ./benchmarks/access.cpp


BIN_DIR = build
TEST_OUT = $(BIN_DIR)/unit-test
RANK0_OUT = $(BIN_DIR)/run-r0
RANK1_OUT = $(BIN_DIR)/run-r1
ACCESS_OUT = $(BIN_DIR)/run-access

.PHONY: all clean


all: $(BIN_DIR) $(TEST_OUT) $(RANK0_OUT) $(RANK1_OUT) $(ACCESS_OUT)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_OUT): $(TEST_SRC) $(CORE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(RANK0_OUT): $(RANK0_SRC) $(CORE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(RANK1_OUT): $(RANK1_SRC) $(CORE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(ACCESS_OUT): $(ACCESS_SRC) $(CORE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(BIN_DIR)