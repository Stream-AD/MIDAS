#include <iostream>
#include <vector>
#include "anom.hpp"
#include "argparse.hpp"
using namespace std;

void load_data(vector<int>& src, vector<int>& dst, vector<int>& times, string input_file, bool undirected)
{
    FILE* infile = fopen(input_file.c_str(), "r");
    if (infile == NULL) {
        cerr << "Could not read file " << input_file << "\n";
        exit(0);
    }

    int s, d, t;

    if (undirected == false) {
        while (fscanf(infile, "%d,%d,%d", &s, &d, &t) == 3) {
            src.push_back(s);
            dst.push_back(d);
            times.push_back(t);
        }
        return;
    }
    else {
        while (fscanf(infile, "%d:%d:%d", &s, &d, &t) == 3) {
            src.push_back(s);
            dst.push_back(d);
            times.push_back(t);
            src.push_back(d);
            dst.push_back(s);
            times.push_back(t);
        }
        return;
    }
}

int main(int argc, const char* argv[])
{
    argparse::ArgumentParser program("midas");
    program.add_argument("-i", "--input")
        .required()
        .help("Input File ");
    program.add_argument("-o", "--output")
        .default_value(string("scores.txt"))
        .help("Output File. Default is scores.txt");
    program.add_argument("-r", "--rows")
        .default_value(2)
        .action([](const std::string& value) { return std::stoi(value); })
        .help("Number of rows/hash functions. Default is 2");
    program.add_argument("-b", "--buckets")
        .default_value(769)
        .action([](const std::string& value) { return std::stoi(value); })
        .help("Number of buckets. Default is 769");
    program.add_argument("-a", "--alpha")
        .default_value(0.6)
        .action([](const std::string& value) { return std::stod(value); })
        .help("Alpha: Temporal Decay Factor. Default is 0.6");
    program.add_argument("--norelations")
        .default_value(false)
        .implicit_value(true)
        .help("To run Midas instead of Midas-R.");
    program.add_argument("--undirected")
        .default_value(false)
        .implicit_value(true)
        .help("If graph is undirected.");
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        program.print_help();
        exit(0);
    }

    string input_file = program.get<string>("-i");
    string output_file = program.get<string>("-o");
    int rows = program.get<int>("-r");
    int buckets = program.get<int>("-b");
    double alpha = program.get<double>("-a");

    if (rows < 1) {
        cerr << "Number of hash functions should be positive." << endl;
        exit(0);
    }

    if (buckets < 2) {
        cerr << "Number of buckets should be atleast 2" << endl;
        exit(0);
    }

    if (alpha <= 0 || alpha >= 1) {
        cerr << "Alpha: Temporal Decay Factor must be between 0 and 1." << endl;
        exit(0);
    }

    vector<int> src, dst, times;
    if (program["--undirected"] == true)
        load_data(src, dst, times, input_file, true);
    else
        load_data(src, dst, times, input_file, false);
    cout << "Finished Loading Data from " << input_file << endl;

    if (program["--norelations"] == true) {
        clock_t start_time1 = clock();
        vector<double>* scores = midas(src, dst, times, rows, buckets);
        cout << "Time taken: " << ((double)(clock() - start_time1)) / CLOCKS_PER_SEC << " s" << endl;

        cout << "Writing Anomaly Scores to " << output_file << endl;
        FILE* outfile = fopen(output_file.c_str(), "w");
        for (int i = 0; i < scores->size(); i++) {
            fprintf(outfile, "%f\n", scores->at(i));
        }
    }
    else {
        clock_t start_time2 = clock();
        vector<double>* scores2 = midasR(src, dst, times, rows, buckets, alpha);
        cout << "Time taken: " << ((double)(clock() - start_time2)) / CLOCKS_PER_SEC << " s" << endl;
        cout << "Writing Anomaly Scores to " << output_file << endl;
        FILE* outfile2 = fopen(output_file.c_str(), "w");
        for (int i = 0; i < scores2->size(); i++) {
            fprintf(outfile2, "%f\n", scores2->at(i));
        }
    }

    return 0;
}
