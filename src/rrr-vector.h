#ifndef RRR_VECTOR_H
#define RRR_VECTOR_H

#include <string>
#include <vector>
#include "../include/bitvector.h"

using namespace std;

inline constexpr uint16_t mask_table[] = {
    0x8000, 0xC000, 0xE000, 0xF000, 
    0xF800, 0xFC00, 0xFE00, 0xFF00, 
    0xFF80, 0xFFC0, 0xFFE0, 0xFFF0, 
    0xFFF8, 0xFFFC, 0xFFFE, 0xFFFF
};

class RRR15 {
private:
    size_t n;
    size_t t;
    size_t factor;
    bool verbose;

    vector<size_t> k_vector;
    vector<size_t> r_vector;
    vector<size_t> sb_vector;
    
    uint16_t pattern_to_offset[32768];
    uint8_t  pattern_to_class[32768];
    vector<vector<uint16_t>> lookup_table;
public:
    RRR15(const string& s, bool verbose = false);

    ~RRR15();
    
    template <typename container> bitVector B(const container& c)
    {
        return B(c.begin(), c.end());
    }

    template <typename it> bitVector B(const it begin, const it end)
    {
        using T = typename std::iterator_traits<it>::value_type;
        static_assert(is_integral_v<T>, "Elements must be integral");
        auto s_of = sizeof(T) * 8;
        auto n = distance(begin, end);
        bitVector bv = bitVector(n * s_of, s_of);

        for (auto iter = begin; iter != end; iter++)
        {
            const T& c = *iter;
            
            for (int i = s_of - 1; i >= 0; i--)
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


    inline size_t rank0(const size_t i) const
    {
        return i - rank1(i);
    }

        inline size_t access(const size_t i) const
        {
            size_t block_index = i / t;
            size_t bit_offset = i % t;

            uint16_t pattern = lookup_table[k_vector[block_index]][r_vector[block_index]];
            return (pattern >> (t - 1 - bit_offset)) & 1;
        }

    inline size_t operator[](const size_t i) const
    {
        return access(i);
    }

    void create_tables(size_t t);

    void build(const bitVector& bv); 

    vector<size_t> superblock(const vector<size_t>& K, size_t factor, 
                                  size_t t, bool verbose);

    size_t rank1(size_t i) const;
};

#endif