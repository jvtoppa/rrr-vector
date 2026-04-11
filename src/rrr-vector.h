#ifndef RRR
#define RRR

#include <string>
#include <iostream>
#include <vector>
#include "../include/bitvector.h"
#include <cmath>
#include <unordered_map>

using namespace std;

bitVector B(const string& s);

pair<vector<vector<bitVector>>, unordered_map<string, size_t>> create_tables(size_t t);

vector<size_t> K(const bitVector& bv, size_t t, bool verbose = false);

vector<size_t> superblock(const vector<size_t> &K, size_t factor, size_t t, bool verbose = false);

vector<size_t> R(bitVector& bv, unordered_map<string, size_t> &map, size_t t, bool verbose = false);

size_t rank1(size_t i, const vector<size_t>& sb, const vector<size_t>& K, size_t t, size_t factor, const vector<size_t>& R, const vector<vector<bitVector>>& mp, size_t n);


#endif