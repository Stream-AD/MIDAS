#ifndef edgehash_hpp
#define edgehash_hpp

#include <vector>
using namespace std;

class Edgehash {
public:
    Edgehash(int r, int b, int m0);
    ~Edgehash();
    void insert(int a, int b, double weight);
    double get_count(int a, int b);
    void clear();
    void lower(double factor);
    int num_rows;
    int num_buckets;
    int m;

private:
    vector<int> hash_a, hash_b;
    vector<std::vector<double> > count;
    int hash(int a, int b, int i);
};

#endif /* edgehash_hpp */
