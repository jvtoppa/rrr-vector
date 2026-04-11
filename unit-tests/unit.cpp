#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include "../include/bitvector.h"
#include <chrono>
#include "../src/rrr-vector.h"

using namespace std;


size_t naive_rank1(bitVector& bv, size_t i)
{
    size_t count = 0;
    for (size_t j = 0; j <= i; j++)
    {
        if (bv[j] == 1) count++;
    }
    return count;
}


void test_superblock_logic()
{
    // Mocking a K vector (number of 1s in each block of size t)
    // Let's assume t=3, and our blocks have these many 1s:
    vector<size_t> mock_K = {2, 1, 3, 0, 2, 1, 3}; 
    size_t factor = 3; 

    vector<size_t> sb = superblock(mock_K, factor, 3, false);

    // Expected logic:
    // Block chunks: [2, 1, 3], [0, 2, 1], [3]
    // Superblock 0 (starts at block 0): initial sum = 0.
    // Superblock 1 (starts at block 3): sum of previous = 2+1+3 = 6.
    // Superblock 2 (starts at block 6): sum of previous = 6 + 0+2+1 = 9.

    assert(sb.size() == 3 && "Should have 3 superblocks");
    assert(sb[0] == 0 && "First superblock must always start with rank 0");
    assert(sb[1] == 6 && "Second superblock rank incorrect");
    assert(sb[2] == 9 && "Third superblock rank incorrect");

    cout << "[OK] test_superblock_logic passed.\n";
}

void test_out_of_bounds()
{
    string input = "abc"; // 24 bits
    bitVector bv = B(input);
    size_t n = bv.getLength();
    
    size_t t = max(1.0, ceil(log2(n) / 2.0));
    size_t factor = max(1.0, ceil(log2(n)));

    auto maps = create_tables(t);
    vector<size_t> k_vec = K(bv, t);
    vector<size_t> r_vec = R(bv, maps.second, t);
    vector<size_t> sb = superblock(k_vec, factor, t, false);

    // Get the total number of 1s in the whole vector
    size_t total_ones = naive_rank1(bv, n - 1);

    // TEST: Index exactly N
    size_t out_rank = rank1(n, sb, k_vec, t, factor, r_vec, maps.first, n);
    assert(out_rank == total_ones && "Rank(N) should equal total number of 1s");

    // TEST: Very large index
    size_t huge_rank = rank1(99999, sb, k_vec, t, factor, r_vec, maps.first, n);
    assert(huge_rank == total_ones && "Rank(99999) should equal total number of 1s");

    cout << "[OK] test_out_of_bounds passed.\n";
}

void test_rank1_correctness(string test_name, string input_string)
{
    bitVector bv = B(input_string);
    size_t n = bv.getLength();
    
    size_t t = max(1.0, ceil(log2(n) / 2.0));
    size_t factor = max(1.0, ceil(log2(n)));

    auto maps = create_tables(t);
    vector<size_t> k_vec = K(bv, t);
    vector<size_t> r_vec = R(bv, maps.second, t);
    vector<size_t> sb = superblock(k_vec, factor, t, false);
    for (size_t i = 0; i < n; i++)
    {
        size_t expected = naive_rank1(bv, i);
        size_t actual = rank1(i, sb, k_vec, t, factor, r_vec, maps.first, n);

        if (expected != actual) {
            cout << "\n[FAILED] " << test_name << " at index " << i << "\n";
            cout << "Expected rank1(" << i << ") = " << expected << ", but got " << actual << "\n";
            assert(false); 
        }
    }
    cout << "[OK] " << test_name << " passed (tested all " << n << " bits).\n";
}

void benchmark_rank1()
{
    cout << "\n--- Benchmarking RRR vs Naive ---\n";

    size_t string_size = 500000000 / 4;
    cout << "Bitstring size: " << string_size * 8 << "\n";
    string large_string(string_size, 'a');
    for (size_t i = 0; i < string_size; i++) {
        large_string[i] = (char)(rand() % 256);
    }

    bitVector bv = B(large_string);
    size_t n = bv.getLength();

    size_t t = max(1.0, ceil(log2(n) / 2.0));
    size_t factor = max(1.0, ceil(log2(n)));
    cout << "Building structures...\n";
    auto maps = create_tables(t);
    vector<size_t> k_vec = K(bv, t);
    vector<size_t> r_vec = R(bv, maps.second, t);
    vector<size_t> sb = superblock(k_vec, factor, t, false);

    size_t num_queries = 40000000;
    vector<size_t> queries(num_queries);
    for(size_t i = 0; i < num_queries; ++i) {
        queries[i] = rand() % n;
    }
    cout << "Starting doing rank operations...\n";
    cout << "No. of rank queries: " << num_queries << "\n";
    
    auto start_rrr = chrono::high_resolution_clock::now();
    for(size_t q : queries) {
        rank1(q, sb, k_vec, t, factor, r_vec, maps.first, n);
    }
    
    auto end_rrr = chrono::high_resolution_clock::now();
    
    chrono::duration<double, std::milli> rrr_time = end_rrr - start_rrr;

    cout << "Tested " << num_queries << " queries on a bitvector of size " << n << " bits.\n";
    cout << "RRR Time:   " << rrr_time.count() << " ms\n";
}


int main()
{
    cout << "--- Running RRR Unit Tests ---\n\n";

    // 1. Test isolated logic
    test_superblock_logic();

    // 2. Test full end-to-end rank calculation with varying strings
    // Testing a string of identical characters (edge case for classes)
    test_rank1_correctness("All identical chars", "aaaaaaaaaaaaaaaaaaa");

    // Testing an alternating pattern
    test_rank1_correctness("Alternating chars", "abababababababab");

    // Testing a complex/random string
    test_rank1_correctness("Random words", "the quick brown fox jumps over the lazy dog");

    // Testing a very short string
    test_rank1_correctness("Short string", "hi");
    test_out_of_bounds();
    benchmark_rank1();
    cout << "\n--- All tests passed successfully! ---\n";
    return 0;
}