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

    for(size_t c : s)
    {
        for(int i = 7; i >= 0; i--)
        {
            if((c >> i) & 1)
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
    
    for (size_t x = 0; x < total; x++)
    {
        bitVector block = bitVector(t, t);
        string pattern = "";
        // inserts x inside of a bitvector
        
        for (size_t j = 0; j < t; j++)
        {
            size_t bit = (x >> j) & 1;
            if (bit == 1)
            {

                block.set1(t - 1 - j);
                pattern += "1";
            }
            else
            {   

                block.set0(t - 1 - j);
                pattern += "0";
            }
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



vector<size_t> K(bitVector bv, size_t t)
{   
    size_t n = bv.getLength();
    vector<size_t> v; 
    size_t counter = 0;

    for (size_t i = 0; i < n; i++)
    {
        auto bit = bv[i];
        
        if(bit == 1)
        counter++;
        
        if((i + 1)% t == 0 || i == n - 1)
        {
            v.push_back(counter);
            counter = 0;
        }

        
    }
    
    for (auto item : v)
    {
        cout << item << " ";
    }
    

    return v;

}
vector<size_t> superblock(const vector<size_t>& K, size_t factor, size_t t)
{
    size_t n = K.size();
    vector<size_t> superblock;

    for (size_t i = 0; i < n; i += factor * t)
    {
        size_t global_rank = 0;
        
        for (size_t j = i; j < min(i + factor * t, n); j++)
        {
            global_rank += K[j];
        }
        
        superblock.push_back(global_rank);
    }
    
    return superblock;
}



vector<size_t> R(bitVector bv, unordered_map<string, size_t>& map, size_t t)
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
    cout << "R: ";
    for (size_t i = 0; i < R.size(); i++)
    {
        cout << R[i] << " ";
    }
    
    return R;
}


void rank(size_t i, vector<size_t> superblock, vector<size_t> K, size_t t, size_t factor)
{
    size_t block_index = ceil(i / t);
    size_t superblock_index = ceil(block_index / factor);
    size_t sum = superblock[superblock_index];
    size_t n = K.size();
    for (size_t i = superblock_index; i < min(superblock_index + factor, n); i++)
    {
        sum += K[i];
    }
    
    //Repeat previous step until we reach . We then add (from , not the global rank)
    // to our result, where , and is the position we are querying local to.
    // Our final answer is the result.
}



int main(void)
{
    string input = "banana";
    bitVector bv = B(input);
    bv.print();
    pair<vector<vector<bitVector>>, unordered_map<string, size_t>> maps = create_tables(4);

    vector<size_t> Ka = K(bv, 4);
    vector<size_t> Ra = R(bv, maps.second, 4);

    cout << "maps: ";
    maps.first[Ra[0]][Ka[0]].print(); 
    return 0;
}
