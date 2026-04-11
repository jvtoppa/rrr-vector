#ifndef RRR_VECTOR_H
#define RRR_VECTOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../include/bitvector.h"

class RRR15 {
private:
    size_t n;
    size_t t;
    size_t factor;
    bool verbose;

    std::vector<size_t> k_vector;
    std::vector<size_t> r_vector;
    std::vector<size_t> sb_vector;
    
    std::vector<std::vector<bitVector>> lookup_table;
    std::unordered_map<std::string, size_t> pattern_to_offset;

public:
    RRR15(const std::string& s, bool verbose = false);

    ~RRR15();

    bitVector B(const std::string& s);
    
    std::pair<std::vector<std::vector<bitVector>>, std::unordered_map<std::string, size_t>> 
    create_tables(size_t t);

    std::vector<size_t> K(const bitVector& bv, size_t t, bool verbose);

    std::vector<size_t> R(bitVector& bv, std::unordered_map<std::string, size_t>& map, 
                         size_t t, bool verbose);

    std::vector<size_t> superblock(const std::vector<size_t>& K, size_t factor, 
                                  size_t t, bool verbose);

    size_t rank1(size_t i) const;
};

#endif