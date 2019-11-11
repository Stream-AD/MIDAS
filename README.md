# MIDAS
MIDAS, short for Microcluster-Based Detector of Anomalies in Edge Streams, detects microcluster anomalies from an edge stream in constant time and memory, while providing theoretical guarantees about its false positive probability. We output an anomaly score for each edge.

This implementation is based on the following paper - [MIDAS:Microcluster-Based Detector of Anomalies in Edge Streams](https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midas.pdf). *Siddharth Bhatia, Bryan Hooi, Minji Yoon, Kijung Shin, Christos Faloutsos*. AAAI 2020.

## Getting started
1. Run `make` to compile code and create the binary.
2. Run `./midas -i ` followed by the input file path and name.
3. Run `make clean` to clean binaries.

Note: The Makefile has currently only been tested on MacOS and may require some changes for the other operating systems.


## Demo
1. Run `./demo.sh` to compile the code and run it on example dataset.


## Command line options
  * `-h --help`: produce help message
  * `-i --input`: input file name
  * `-o --output`: output file name (default: scores.txt)  
  * `-r --rows`: Number of Hash Functions (default: 2)  
  * `-b --buckets`: Number of Buckets (default: 769)  
  * `-a --alpha`: Temporal Decay Factor (default: 0.6)  
  * `--midas` : Run MIDAS instead of MIDAS-R
  * `--undirected` : Treat graph as undirected instead of directed


## Input file format
MIDAS expects the input edge stream to be stored in a single file containing the following three columns in order:
1. `source (int)`: source ID of the edge
2. `destination (int)`: destination ID of the edge
3. `time (int)`: time stamp of the edge

Thus, each line represents an edge. Edges should be sorted in non-decreasing order of their time stamps and the column delimiter should be ,
