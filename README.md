<h1 align="center">
  MIDAS
</h1>

<h4 align="center">Microcluster-Based Detector of Anomalies in Edge Streams</h4>

<p align="center">
  <a href="https://aaai.org/Conferences/AAAI-20/">
    <img src="http://img.shields.io/badge/AAAI-2020-red.svg">
  </a>
  <a href="https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midas.pdf"><img src="http://img.shields.io/badge/Paper-pdf-brightgreen.svg"></a>
  <a href="https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midasslides.pdf">
      <img src="http://img.shields.io/badge/Slides-pdf-ff9e18.svg">
  </a>
<!--   <a href="https://lionbridge.ai/articles/introducing-midas-a-new-baseline-for-anomaly-detection-in-graphs/">
    <img src="http://img.shields.io/badge/Press-article-ff69b4.svg">
  </a> -->
<!--   <a href="#midas-in-other-languages"> -->
<!--     <img src="https://img.shields.io/badge/contributions-welcome-orange.svg"> -->
<!--   </a> -->
  <a href="https://github.com/bhatiasiddharth/MIDAS/blob/master/LICENSE">
    <img src="https://img.shields.io/badge/License-Apache%202.0-blue.svg">
  </a>
  
</p>


![GIF demo](https://www.comp.nus.edu.sg/~sbhatia/assets/img/midas_gif.gif) 
<img align="center" src="https://www.comp.nus.edu.sg/~sbhatia/assets/img/midasstream.jpg" alt="...">

## Table of Contents


- [Features](#features)
- [Use Cases](#use-cases)
- [Getting Started](#getting-started)
- [Demo](#demo)
- [Command-Line Options](#command-line-options)
- [Input File Format](#input-file-format)
- [Datasets](#datasets)
- [MIDAS in other Languages](#midas-in-other-languages)
- [Online Articles](#online-articles)
- [Citation](#citation)

<!-- - [See Also](#see-also) -->


## Features
- Finds Anomalies in Dynamic/Time-Evolving Graphs
- Detects Microcluster Anomalies (suddenly arriving groups of suspiciously similar edges e.g. DoS attack)
- Theoretical Guarantees on False Positive Probability
- Constant Memory (independent of graph size)
- Constant Update Time (real-time anomaly detection to minimize harm)
- Up to 48% more accurate and 644 times faster than the state of the art approaches

For more details, please read the paper - [MIDAS: Microcluster-Based Detector of Anomalies in Edge Streams](https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midas.pdf). *Siddharth Bhatia, Bryan Hooi, Minji Yoon, Kijung Shin, Christos Faloutsos*. AAAI 2020.

## Use Cases
1. Intrusion Detection
2. Fake Ratings
3. Financial Fraud


## Getting Started
1. Run `make` to compile code and create the executable.
2. Run `./midas -i ` followed by the input file path and name.


## Demo
1. Run `./demo.sh` to compile the code and run it on an example dataset.


## Command-Line Options
  * `-h --help`: produce help message
  * `-i --input`: input file name
  * `-o --output`: output file name (default: scores.txt)  
  * `-r --rows`: Number of Hash Functions (default: 2)  
  * `-b --buckets`: Number of Buckets (default: 769)
  * `-a --alpha`: Temporal Decay Factor (default: 0.6)  
  * `--norelations` : Run MIDAS instead of MIDAS-R
  * `--undirected` : Treat graph as undirected instead of directed


## Input File Format
MIDAS expects the input edge stream to be stored in a single file containing the following three columns in order:
1. `source (int)`: source ID of the edge
2. `destination (int)`: destination ID of the edge
3. `time (int)`: timestamp of the edge

Thus, each line represents an edge. Edges should be sorted in non-decreasing order of their timestamps and the column delimiter should be `,`


## Datasets
1. [DARPA](https://www.ll.mit.edu/r-d/datasets/1998-darpa-intrusion-detection-evaluation-dataset): [Original Format](https://www.comp.nus.edu.sg/~sbhatia/assets/datasets/darpa_original.csv), [MIDAS format](https://www.comp.nus.edu.sg/~sbhatia/assets/datasets/darpa_midas.csv)
2. [TwitterWorldCup2014](http://odds.cs.stonybrook.edu/twitterworldcup2014-dataset)
3. [TwitterSecurity](http://odds.cs.stonybrook.edu/twittersecurity-dataset)

## MIDAS in other Languages
1. [Rust](https://github.com/scooter-dangle/midas_rs) and [Python](https://github.com/scooter-dangle/midas_rs/tree/master/python) by [Scott Steele](https://github.com/scooter-dangle)
2. [Ruby](https://github.com/ankane/midas) by [Andrew Kane](https://github.com/ankane)

## Online Articles
1. KDnuggets: [Introducing MIDAS: A New Baseline for Anomaly Detection in Graphs](https://www.kdnuggets.com/2020/04/midas-new-baseline-anomaly-detection-graphs.html)
1. Towards Data Science: [Controlling Fake News using Graphs and Statistics](https://towardsdatascience.com/controlling-fake-news-using-graphs-and-statistics-31ed116a986f)
2. Towards Data Science: [Anomaly detection in dynamic graphs using MIDAS](https://towardsdatascience.com/anomaly-detection-in-dynamic-graphs-using-midas-e4f8d0b1db45)
3. Towards AI: [Anomaly Detection with MIDAS](https://medium.com/towards-artificial-intelligence/anomaly-detection-with-midas-2735a2e6dce8)

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


------------------------------------------------------------------------

<a href="https://twitter.com/intent/tweet?text=Wow:&url=https%3A%2F%2Fgithub.com%2Fbhatiasiddharth%2FMIDAS%2F">
    <img src="https://img.shields.io/twitter/url?style=social&url=https%3A%2F%2Fgithub.com%2Fbhatiasiddharth%2FMIDAS%2F">
</a>
<br>

> Webpage [https://www.comp.nus.edu.sg/~sbhatia/](https://www.comp.nus.edu.sg/~sbhatia/) &nbsp;&middot;&nbsp;
> Email [siddharth@comp.nus.edu.sg](mailto:siddharth@comp.nus.edu.sg?subject=[GitHub]%20MIDAS) &nbsp;&middot;&nbsp;
> Twitter [@siddharthb_](https://twitter.com/siddharthb_)
