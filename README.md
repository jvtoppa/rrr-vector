# RRR15-vector

RRR is a two-layered indexing algorithm for calculating succint rank/select in amortized O(1) operations. Some day I'll sit down and fix this readme, but for now that is all. If you're interested, read: https://dl.acm.org/doi/10.5555/545381.545411, https://www.alexbowe.com/rrr/, etc.

# Creating an RRR object

To create an RRR structure, use RRR15(string).

Operations:

Let $i \in [0, size(bitvector(string))]$ 

- rank0( $i$ )
- rank1( $i$ )
- access( $i$ ) / operator[ $i$ ]

# Some stats:

Rank Operation Performance

| Queries | Bitvector Size (bits) | Total Time (ms) | Time/Operation (ns) |
| :--- | :--- | :--- | :--- |
| 4 | 80 | 0.000249 | 62.2500 |
| 40 | 800 | 0.001048 | 26.2000 |
| 400 | 8,000 | 0.008678 | 21.6950 |
| 4,000 | 80,000 | 0.080650 | 20.1625 |
| 40,000 | 800,000 | 0.906303 | 22.6576 |
| 400,000 | 8,000,000 | 13.461600 | 33.6539 |
| 4,000,000 | 80,000,000 | 261.037000 | 65.2593 |
| 40,000,000 | 800,000,000 | 3801.100000 | 95.0276 |



# How to run

Simply run python3 run.py on the terminal. The results of the tests will be written to a .txt document called "results.txt". Happy coding :)
