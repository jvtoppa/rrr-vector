#include <string>
#include <iostream>
#include <vector>
#include "../include/bitvector.h"
#include "rrr-vector.h"
#include <cmath>
#include <unordered_map>
#include <bit>
using namespace std;


RRR15::~RRR15() {}

bitVector RRR15::B(const string& s)
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

// There are 2^t possible bitvectors. Each of them must be saved to a class based
// on no. of zeros.
pair<vector<vector<bitVector>>, unordered_map<string, size_t>> RRR15::create_tables(size_t t) // access: table[class][block]
{
    vector<vector<bitVector>> table(t + 1);
    unordered_map<string, size_t> pattern_to_offset;

    size_t total = 1 << t;
    bitVector block = bitVector(t, t);
    for (size_t i = 0; i < t; i++)
    {
        block.append0();
    }

    for (size_t x = 0; x < total; x++)
    {
        string pattern = "";
        for (size_t i = 0; i < t; i++)
        {
            block.set0(i);
        }
        size_t clss = 0;
        for (size_t j = t; j > 0; j--)
        {
            size_t bit = (x >> (j - 1)) & 1;
            if (bit == 1)
            {
                pattern += "1";
                block.set1(t - j);
                clss++;
            }

            else
                pattern += "0";
        }

        size_t offset = table[clss].size();
        table[clss].push_back(block);
        pattern_to_offset[pattern] = offset;
    }

    return {table, pattern_to_offset};
}

vector<size_t> RRR15::K(const bitVector& bv, size_t t, bool verbose) //should be done with popcount
{
    size_t n = bv.getLength();
    vector<size_t> v;
    size_t counter = 0;

    for (size_t i = 0; i < n; i++)
    {
        auto bit = bv[i];

        if (bit == 1)
            counter++;

        if ((i + 1) % t == 0 || i == n - 1)
        {
            v.push_back(counter);
            counter = 0;
        }
    }
    if(verbose)
    {
        cout << "K: ";
        for (auto item : v)
        {
            cout << item << " ";
        }
        cout << "\n";
    }

    return v;
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


vector<size_t> RRR15::R(bitVector& bv, unordered_map<string, size_t> &map, size_t t, bool verbose)
{
    size_t n = bv.getLength();
    string block = "";
    vector<size_t> R;

    for (size_t i = 0; i < n; i++)
    {
        if (bv[i] == 1)
            block += "1";
        else
            block += "0";

        if ((i + 1) % t == 0)
        {
            R.push_back(map[block]);
            block = "";
        }
    }
    if (!block.empty())
    {
        while (block.length() < t)
        {
            block += "0";
        }
        R.push_back(map[block]);
    }
    if(verbose)
    {
        cout << "R: ";
        for (size_t i = 0; i < R.size(); i++)
        {
            cout << R[i] << " ";
        }
        cout << "\n";
    }

    return R;
}

size_t RRR15::rank1(size_t i) const {

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

    const bitVector& block = lookup_table[k_vector[block_index]][r_vector[block_index]];

    size_t bit_offset = i % t;
    
    sum += block.popcount(bit_offset);

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

    auto tables = create_tables(this->t);
    this->lookup_table = tables.first;
    this->pattern_to_offset = tables.second;

    this->k_vector = K(bv, this->t, verbose);
    this->r_vector = R(bv, this->pattern_to_offset, this->t, verbose);
    
    this->sb_vector = superblock(this->k_vector, this->factor, this->t, verbose);

}