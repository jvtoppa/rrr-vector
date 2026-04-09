#include <string>
#include <iostream>
#include <vector>
#include "../include/bitvector.h"
#include <cmath>
#include <unordered_map>

using namespace std;

bitVector B(string s)
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
pair<vector<vector<bitVector>>, unordered_map<string, size_t>> create_tables(size_t t) // access: table[class][block]
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

        for (size_t j = t; j > 0; j--)
        {
            size_t bit = (x >> (j - 1)) & 1;
            if (bit == 1)
            {
                pattern += "1";
                block.set1(t - j);
            }

            else
                pattern += "0";
        }

        // Counts how many ones in bitvector
        // to choose the class

        size_t clss = 0;
        for (size_t j = 0; j < t; j++)
        {
            if (block[j] == 1)
                clss++;
        }
        size_t offset = table[clss].size();
        table[clss].push_back(block);
        pattern_to_offset[pattern] = offset;
    }

    return {table, pattern_to_offset};
}

vector<size_t> K(bitVector bv, size_t t, bool verbose = false)
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

vector<size_t> superblock(const vector<size_t> &K, size_t factor, size_t t, bool verbose = false) //TODO: add tuple with marker for variable R. Should receive R as part of the function
{
    size_t n = K.size();
    vector<size_t> superblock;

    size_t global_rank = 0;
    for (size_t i = 0; i < n; i += factor)
    {

        for (size_t j = i; j < min(i + factor, n); j++)
        {
            global_rank += K[j];
        }

        superblock.push_back(global_rank);
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


vector<size_t> R(bitVector bv, unordered_map<string, size_t> &map, size_t t, bool verbose = false)
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

void rank(size_t i, vector<size_t> superblock, vector<size_t> K, size_t t, size_t factor, vector<size_t> R)
{
    size_t block_index = ceil(i / t);
    size_t superblock_index = ceil(block_index / factor);
    size_t sum = superblock[superblock_index];
    size_t n = K.size();
    for (size_t i = superblock_index; i < min(superblock_index + factor, n); i++)
    {
        sum += K[i];
    }



    // Repeat previous step until we reach . We then add (from , not the global rank)
    //  to our result, where , and is the position we are querying local to.
    //  Our final answer is the result.
}

int main(void)
{
    string input = "bananaana";
    bitVector bv = B(input);
    bv.print();
    size_t t = max(1.0, ceil(log2(bv.getLength()) / 2));
    cout << t << "\n";
    pair<vector<vector<bitVector>>, unordered_map<string, size_t>> maps = create_tables(t);

    vector<size_t> k = K(bv, t); // Offset vector;
    vector<size_t> r = R(bv, maps.second, t); // Class vector
    superblock(k, 3, t, true);
    cout << "k size: " <<k.size() << "\n";
    //cout << "maps: " << r[0] << k[0] << "\n";
    //    maps.first[r[2]][k[2]].print();

    return 0;
}
