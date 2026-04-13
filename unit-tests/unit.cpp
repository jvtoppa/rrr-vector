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


bitVector Bi(const string& s)
{
    size_t n = s.size();
    bitVector bv = bitVector(n * 8, 8);

    for (size_t c : s)
    {
        for (int i = 7; i >= 0; i--)
        {
            if ((c >> i) & 1)
            {
                bv.append1();
            }
            else
            {
                bv.append0();
            }
        }
    }
    return bv;
}

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
    RRR15 rrr(input_string);
    bitVector bv = Bi(input_string);
    for (size_t i = 0; i < input_string.size(); i++)
    {
        size_t expected = naive_rank1(bv, i);
        size_t actual = rrr.rank1(i);

        if (expected != actual) {
            cout << "\n[FAILED] " << test_name << " at index " << i << "\n";
            cout << "Expected rank1(" << i << ") = " << expected << ", but got " << actual << "\n";
            assert(false); 
        }
    }
    cout << "[OK] " << test_name << " passed (tested all " << input_string.size()*8 << " bits).\n";
}

void test_rrr_access_rank0() {

    std::string input = "AB"; 
    RRR15 rrr(input);

    std::cout << "Testing RRR15 with ASCII-to-Binary encoding..." << std::endl;

    assert(rrr.access(0) == 0); // 2^0
    assert(rrr.access(1) == 1); // 2^1
    assert(rrr.access(2) == 0);
    assert(rrr.access(3) == 0);
    assert(rrr.access(4) == 0);
    assert(rrr.access(5) == 0);
    assert(rrr.access(6) == 0); // 2^6
    assert(rrr.access(7) == 1); // 2^7
    std::cout << "[OK] Access verified for ASCII bit-patterns." << std::endl;

    assert(rrr.rank0(8) == 6);

    assert(rrr.rank0(16) == 12);

    assert(rrr.rank0(10) == 7);

    std::cout << "[OK] Rank0 verified for ASCII bit-patterns." << std::endl;
}



int main()
{
    cout << "Running RRR Unit Tests\n\n";
    
    test_rank1_correctness("All identical chars", "aaaaaaaaaaaaaaaaaaa");
    
    test_rank1_correctness("Alternating chars", "abababababababab");
    
    test_rank1_correctness("Random words", "the quick brown fox jumps over the lazy dog");
    
    test_rank1_correctness("Short string", "hi");
    test_rrr_access_rank0();
    
    cout << "\n--- All tests passed successfully! ---\n";
    
    return 0;
}