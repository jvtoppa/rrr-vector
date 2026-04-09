#include <string>
#include <iostream>
#include <vector>
#include "../include/bitvector.h"
#include <math.h>
#include <unordered_map>

using namespace std;

bitVector create_bv(string s)
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

void create_tables(size_t t)
{
    vector<vector<bitVector>> um(t + 1);
    
    size_t total = 1 << t;
    
    for (size_t x = 0; x < total; x++)
    {
        bitVector block = bitVector(t, t);
        
        // Cria permutacoes bitvectors
        
        for (size_t j = 0; j < t; j++)
        {
            size_t bit = (x >> j) & 1;
            if (bit == 1)
                block.set1(t - 1 - j);
            else
                block.set0(t - 1 - j);
        }
        

        // Conta quantos elementos no bitvector para definir a
        // classe
        
        size_t clss = 0;
        for (size_t j = 0; j < t; j++) 
        {
            if (block[j] == 1)
                clss++;
        }
        
        um[clss].push_back(block);
    }        

    // Acesso: um[class][block] 
}


/*

void B(bitVector bv)
{

}
*/

/*

void R(bitVector bv)
{

}
*/




int main(void)
{
    string input = "banana";
    bitVector bv = create_bv(input);
    bv.print();


    create_tables(4);
    return 0;
}
