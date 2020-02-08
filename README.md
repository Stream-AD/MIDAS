# MIDAS
[![Conference](http://img.shields.io/badge/AAAI-2020-4b44ce.svg)](https://aaai.org/Conferences/AAAI-20/)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://github.com/bhatiasiddharth/MIDAS/blob/master/LICENSE)

Anomaly detection in graphs is a critical problem for finding suspicious behavior in innumerable systems, such as intrusion detection, fake ratings, and financial fraud. This has been a well-researched problem with majority of the proposed approaches focusing on static graphs. However, many real-world graphs are dynamic in nature, and methods based on static connections may miss temporal characteristics of the graphs and anomalies.

Among the methods focusing on dynamic graphs, most of them have edges aggregated into graph snapshots. However, to minimize the effect of malicious activities and start recovery as soon as possible, we need to detect anomalies in real-time or near real-time i.e. to identify whether an incoming edge is anomalous or not, as soon as we receive it. In addition, since the number of vertices can increase as we process the stream of edges, we need an algorithm which uses constant memory in graph size. Moreover, fraudulent or anomalous events in many applications occur in microclusters or suddenly arriving groups of suspiciously similar edges e.g. denial of service attacks in network traffic data and lockstep behavior.

In this work, we propose MIDAS, short for Microcluster-Based Detector of Anomalies in Edge Streams, which detects microcluster anomalies, or suddenly arriving groups of suspiciously similar edges, in edge streams, using constant time and memory. In addition, by using a principled hypothesis testing framework, MIDAS provides theoretical bounds on the false positive probability, which earlier methods do not provide. Also, we are up to 48% more accurate while being up to 644 times faster than state of the art approaches.

For more details, please read the paper - [MIDAS:Microcluster-Based Detector of Anomalies in Edge Streams](https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midas.pdf). *Siddharth Bhatia, Bryan Hooi, Minji Yoon, Kijung Shin, Christos Faloutsos*. AAAI 2020.


## Getting started
1. Run `make` to compile code and create the binary.
2. Run `./midas -i ` followed by the input file path and name.
3. Run `make clean` to clean binaries.


## Demo
1. Run `./demo.sh` to compile the code and run it on example dataset.


## Command line options
  * `-h --help`: produce help message
  * `-i --input`: input file name
  * `-o --output`: output file name (default: scores.txt)  
  * `-r --rows`: Number of Hash Functions (default: 2)  
  * `-b --buckets`: Number of Buckets (default: 769)  
  * `-a --alpha`: Temporal Decay Factor (default: 0.6)  
  * `--norelations` : Run MIDAS instead of MIDAS-R
  * `--undirected` : Treat graph as undirected instead of directed


## Input file format
MIDAS expects the input edge stream to be stored in a single file containing the following three columns in order:
1. `source (int)`: source ID of the edge
2. `destination (int)`: destination ID of the edge
3. `time (int)`: time stamp of the edge

Thus, each line represents an edge. Edges should be sorted in non-decreasing order of their time stamps and the column delimiter should be `,`


## Datasets
1. [DARPA](https://www.ll.mit.edu/r-d/datasets/1998-darpa-intrusion-detection-evaluation-dataset)
2. [TwitterWorldCup2014](http://odds.cs.stonybrook.edu/twitterworldcup2014-dataset)
3. [TwitterSecurity](http://odds.cs.stonybrook.edu/twittersecurity-dataset)

## Citation
If you use this code for your research, please consider citing our paper.

```
@article{bhatia2019midas,
  title={MIDAS: Microcluster-Based Detector of Anomalies in Edge Streams},
  author={Bhatia, Siddharth and Hooi, Bryan and Yoon, Minji and Shin, Kijung and Faloutsos, Christos},
  journal={arXiv preprint arXiv:1911.04464},
  year={2019}
}
```
