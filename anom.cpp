#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include "anom.hpp"
#include "edgehash.hpp"
#include "nodehash.hpp"

vector<double>* midas(vector<int>& src, vector<int>& dst, vector<int>& times, int num_rows, int num_buckets)
{
    int m = *max_element(src.begin(), src.end());
    Edgehash cur_count(num_rows, num_buckets, m);
    Edgehash total_count(num_rows, num_buckets, m);
    vector<double>* anom_score = new vector<double>(src.size());
    int cur_t = 1, size = src.size(), cur_src, cur_dst;
    double cur_mean, sqerr, cur_score;
    for (int i = 0; i < size; i++) {

        if (i == 0 || times[i] > cur_t) {
            cur_count.clear();
            cur_t = times[i];
        }

        cur_src = src[i];
        cur_dst = dst[i];
        cur_count.insert(cur_src, cur_dst, 1);
        total_count.insert(cur_src, cur_dst, 1);
        cur_mean = total_count.get_count(cur_src, cur_dst) / cur_t;
        sqerr = pow(cur_count.get_count(cur_src, cur_dst) - cur_mean, 2);
        if (cur_t == 1) cur_score = 0;
        else cur_score = sqerr / cur_mean + sqerr / (cur_mean * (cur_t - 1));
        (*anom_score)[i] = cur_score;
    }

    return anom_score;
}

double counts_to_anom(double tot, double cur, int cur_t)
{
    double cur_mean = tot / cur_t;
    double sqerr = pow(MAX(0, cur - cur_mean), 2);
    return sqerr / cur_mean + sqerr / (cur_mean * MAX(1, cur_t - 1));
}

vector<double>* midasR(vector<int>& src, vector<int>& dst, vector<int>& times, int num_rows, int num_buckets, double factor)
{
    int m = *max_element(src.begin(), src.end());
    Edgehash cur_count(num_rows, num_buckets, m);
    Edgehash total_count(num_rows, num_buckets, m);
    Nodehash src_score(num_rows, num_buckets);
    Nodehash dst_score(num_rows, num_buckets);
    Nodehash src_total(num_rows, num_buckets);
    Nodehash dst_total(num_rows, num_buckets);
    vector<double>* anom_score = new vector<double>(src.size());
    int cur_t = 1, size = src.size(), cur_src, cur_dst;
    double cur_score, cur_score_src, cur_score_dst, combined_score;

    for (int i = 0; i < size; i++) {
    	
        if (i == 0 || times[i] > cur_t) {
            cur_count.lower(factor);
            src_score.lower(factor);
            dst_score.lower(factor);
            cur_t = times[i];
        }

        cur_src = src[i];
        cur_dst = dst[i];
        cur_count.insert(cur_src, cur_dst, 1);
        total_count.insert(cur_src, cur_dst, 1);
        src_score.insert(cur_src, 1);
        dst_score.insert(cur_dst, 1);
        src_total.insert(cur_src, 1);
        dst_total.insert(cur_dst, 1);
        cur_score = counts_to_anom(total_count.get_count(cur_src, cur_dst), cur_count.get_count(cur_src, cur_dst), cur_t);
        cur_score_src = counts_to_anom(src_total.get_count(cur_src), src_score.get_count(cur_src), cur_t);
        cur_score_dst = counts_to_anom(dst_total.get_count(cur_dst), dst_score.get_count(cur_dst), cur_t);
        //combined_score = MAX(cur_score_src, cur_score_dst) + cur_score;
        //combined_score = cur_score_src + cur_score_dst + cur_score;
        combined_score = MAX(MAX(cur_score_src, cur_score_dst), cur_score);
        (*anom_score)[i] = log(1 + combined_score);
    }

    return anom_score;
}
