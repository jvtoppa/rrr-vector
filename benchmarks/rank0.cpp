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


void benchmark_rank0(size_t string_size, size_t num_queries)
{  
    string large_string(string_size, 'a');
   
    for (size_t i = 0; i < string_size; i++) {
        large_string[i] = (char)(rand() % 256);
    }
    
    auto start_structure_rrr = chrono::high_resolution_clock::now();

    RRR15 rrr(large_string, false);
    
    auto end_structure_rrr = chrono::high_resolution_clock::now();
    
    chrono::duration<double, std::milli> rrr_construction_time = end_structure_rrr - start_structure_rrr;

    cout << "Construction time: " << rrr_construction_time.count() << " ms\n";


    vector<size_t> queries(num_queries);
    for(size_t i = 0; i < num_queries; ++i)
    {
        queries[i] = rand() % (string_size * 8);
    }

    auto start_rrr = chrono::high_resolution_clock::now();
    for(size_t q : queries)
    {
        rrr.rank0(q);
    }
    
    auto end_rrr = chrono::high_resolution_clock::now();
    
    chrono::duration<double, std::milli> rrr_time = end_rrr - start_rrr;

    cout << "Operation Time: " << (rrr_time.count() / num_queries)*1e6 << "ns \n";
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    char *end;

    double a_tmp = strtod(argv[1], &end);
    if (*end != '\0') {
        printf("Invalid input for a\n");
        return 1;
    }

    double b_tmp = strtod(argv[2], &end);
    if (*end != '\0') {
        printf("Invalid input for b\n");
        return 1;
    }

    size_t a = (size_t)a_tmp;
    size_t b = (size_t)b_tmp;

    benchmark_rank0(a, b);

    return 0;
}