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

void benchmark_rank1(size_t string_size, size_t num_queries)
{
    cout << "\n--- Benchmarking RRR vs Naive ---\n";

    string_size /= 4;
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
    cout << "Rank Operation Time (total):   " << rrr_time.count() << " ms\n";
    cout << "Rank Operation Time (per operation) " << (rrr_time.count() / num_queries)*1e6 << "ns \n";
}


int main()
{
    cout << "--- Running RRR Unit Tests ---\n\n";

    test_rank1_correctness("All identical chars", "aaaaaaaaaaaaaaaaaaa");

    test_rank1_correctness("Alternating chars", "abababababababab");

    test_rank1_correctness("Random words", "the quick brown fox jumps over the lazy dog");

    test_rank1_correctness("Short string", "hi");
    for (size_t i = 40; i < 1e9; i *= 10)
    {
        benchmark_rank1(i,i / 10);
        
    }
    
    
    cout << "\n--- All tests passed successfully! ---\n";

    return 0;
}