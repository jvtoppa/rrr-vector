#include <string>
#include <iostream>
#include <vector>
#include "../include/bitvector.h"
#include "rrr-vector.h"
#include <cmath>
#include <bit>
#include <cstring>


using namespace std;

RRR15::~RRR15() {}

// There are 2^t possible bitvectors. Each of them must be saved to a class based
// on no. of zeros.
void RRR15::create_tables(size_t t) {
    
    size_t total_patterns = 1 << t; //2^t
    
    memset(pattern_to_class, 0, sizeof(pattern_to_class));
    memset(pattern_to_offset, 0, sizeof(pattern_to_offset));

    lookup_table.assign(t + 1, vector<uint16_t>());
    
    vector<size_t> class_counters(t + 1, 0);

    for (uint32_t x = 0; x < total_patterns; x++) {
        
        uint8_t clss = __builtin_popcountll(x);
        
        uint16_t offset = class_counters[clss]++;
        
        pattern_to_class[x] = clss;
        pattern_to_offset[x] = offset;

        lookup_table[clss].push_back((uint16_t)x);
    }
}

void RRR15::build(const bitVector& bv) {
    size_t num_blocks = (bv.getLength() + t - 1) / t;
    k_vector.reserve(num_blocks);
    r_vector.reserve(num_blocks);

    for (size_t i = 0; i < bv.getLength(); i += t) {

        uint16_t block_val = bv.get_block_as_int(i, t);
        
        uint8_t clss = pattern_to_class[block_val];
        uint16_t offset = pattern_to_offset[block_val];
        
        k_vector.push_back(clss);
        r_vector.push_back(offset);

    }
}

vector<size_t> RRR15::superblock(const vector<size_t> &K, size_t factor, size_t t, bool verbose) //TODO: add tuple with marker for variable R. Should receive R as part of the function
{
    size_t n = K.size();
    vector<size_t> superblock;

    size_t global_rank = 0;
    for (size_t i = 0; i < n; i += factor)
    {

        superblock.push_back(global_rank);
        for (size_t j = i; j < min(i + factor, n); j++)
        {
            global_rank += K[j];
        }

    }
    if(verbose)
    {
        cout << "Superblocks: "; 
        for(int i = 0; i < superblock.size(); i++)
        {
            cout << superblock[i] << " ";
        }
        cout << "\n";
    }
    return superblock;
}

size_t RRR15::rank1(size_t i) const
{

    if (i >= n)
    {
        i = n - 1; 
    }

    size_t block_index = i / t;
    size_t superblock_index = block_index / factor;

    size_t sum = sb_vector[superblock_index];

    size_t start_block = superblock_index * factor;
    for (size_t j = start_block; j < block_index; j++) {
        sum += k_vector[j];
    }

    const uint16_t block = lookup_table[k_vector[block_index]][r_vector[block_index]];

    size_t bit_offset = i % t;
        
    uint16_t aligned_block = block << (16 - t);

    sum += __builtin_popcount(aligned_block & mask_table[bit_offset]);
    
    return sum;
}


RRR15::RRR15(const string& s, bool verbose) : verbose(verbose)
{
    bitVector bv = B(s); 
    this->n = bv.getLength();

    size_t calc_t = (size_t)max(1.0, ceil(log2(n) / 2.0));
    this->t = (calc_t < 15) ? calc_t : 15;
    this->factor = max(1.0, ceil(log2(n)));

    if (verbose)
    {
        cout << "Initializing RRR with t=" << t << " and factor=" << factor << endl;
    }

    create_tables(this->t);
    build(bv);
    
    this->sb_vector = superblock(this->k_vector, this->factor, this->t, verbose);

}