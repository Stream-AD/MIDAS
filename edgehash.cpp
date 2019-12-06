#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#include <iostream>
#include "edgehash.hpp"

Edgehash::Edgehash(int r, int b, int m0)
{
    num_rows = r;
    num_buckets = b;
    m = m0;
    hash_a.reserve(r);
    hash_b.reserve(r);
    for (int i = 0; i < r; i++) {
        // a is in [1, p-1]; b is in [0, p-1]
        hash_a[i] = rand() % (num_buckets - 1) + 1;
        hash_b[i] = rand() % num_buckets;
    }
    this->clear();
}

Edgehash::~Edgehash()
{
}

int Edgehash::hash(int a, int b, int i)
{
    int resid = ((a + m * b) * hash_a[i] + hash_b[i]) % num_buckets;
    return resid + (resid < 0 ? num_buckets : 0);
}

void Edgehash::insert(int a, int b, double weight)
{
    for (int i = 0; i < num_rows; i++) {
        int bucket = hash(a, b, i);
        count[i][bucket] += weight;
    }
}

double Edgehash::get_count(int a, int b)
{
    double min_count = numeric_limits<double>::max();
    int bucket;
    for (int i = 0; i < num_rows; i++) {
        bucket = hash(a, b, i);
        min_count = MIN(min_count, count[i][bucket]);
    }
    return min_count;
}

void Edgehash::clear()
{
    count = vector<vector<double> >(num_rows, vector<double>(num_buckets, 0.0));
}

void Edgehash::lower(double factor)
{
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_buckets; j++) {
            count[i][j] = count[i][j] * factor;
        }
    }
}