#ifndef nodehash_hpp
#define nodehash_hpp

#include <vector>
using namespace std;

class Nodehash {
public:
    Nodehash(int r, int b);
    ~Nodehash();
    void insert(int a, double weight);
    double get_count(int a);
    void clear();
    void lower(double factor);
    int num_rows;
    int num_buckets;
    int m;

private:
    vector<int> hash_a, hash_b;
    vector<vector<double> > count;
    int hash(int a, int i);
};

#endif /* nodehash_hpp */
