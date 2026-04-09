#include <string>
#include <iostream>
#include <vector>
#include "../include/bitvector.h"
#include <math.h>


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

/*

void create_tables(size_t block_size)
{

}
*/

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

    return 0;
}
