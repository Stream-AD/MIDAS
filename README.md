# MIDAS

<p>
  <a href="https://aaai.org/Conferences/AAAI-20/">
    <img src="http://img.shields.io/badge/AAAI-2020-red.svg">
  </a>
  <a href="https://arxiv.org/pdf/1911.04464.pdf"><img src="http://img.shields.io/badge/Paper-PDF-brightgreen.svg"></a>
  <a href="https://www.comp.nus.edu.sg/~sbhatia/assets/pdf/midasslides.pdf">
      <img src="http://img.shields.io/badge/Slides-PDF-ff9e18.svg">
  </a>
  <a href="https://youtu.be/Bd4PyLCHrto">
    <img src="http://img.shields.io/badge/Talk-Youtube-ff69b4.svg">
  </a>
  <a href="https://www.youtube.com/watch?v=DPmN-uPW8qU"> 
    <img src="https://img.shields.io/badge/Overview-Youtube-orange.svg">
  </a>
  <a href="https://github.com/bhatiasiddharth/MIDAS/blob/master/LICENSE">
    <img src="https://img.shields.io/badge/License-Apache%202.0-blue.svg">
  </a>
</p>

C++ implementation of

- Real-time Streaming Anomaly Detection in Dynamic Graphs. *Siddharth Bhatia, Rui Liu, Bryan Hooi, Minji Yoon, Kijung Shin, Christos Faloutsos*. (Under Review)
- [MIDAS: Microcluster-Based Detector of Anomalies in Edge Streams](https://arxiv.org/pdf/1911.04464.pdf). *Siddharth Bhatia, Bryan Hooi, Minji Yoon, Kijung Shin, Christos Faloutsos*. AAAI 2020.

The old implementation is in another branch `OldImplementation`, it should be considered as being archived and will hardly receive feature updates.

![](asset/Intro.png)

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Customization](#customization)
- [Online Articles](#online-articles)
- [MIDAS in other Languages](#midas-in-other-languages)
- [Citation](#citation)

## Features

- Finds Anomalies in Dynamic/Time-Evolving Graph: (Intrusion Detection, Fake Ratings, Financial Fraud)
- Detects Microcluster Anomalies (suddenly arriving groups of suspiciously similar edges e.g. DoS attack)
- Theoretical Guarantees on False Positive Probability
- Constant Memory (independent of graph size)
- Constant Update Time (real-time anomaly detection to minimize harm)
- Up to 55% more accurate and 929 times faster than the state of the art approaches
- Experiments are performed using the following datasets: 
  - [DARPA](https://www.ll.mit.edu/r-d/datasets/1998-darpa-intrusion-detection-evaluation-dataset)
  - [TwitterWorldCup2014](http://odds.cs.stonybrook.edu/twitterworldcup2014-dataset)
  - [TwitterSecurity](http://odds.cs.stonybrook.edu/twittersecurity-dataset)

## Demo

If you use Windows:

1. Open a Visual Studio developer command prompt, we want their toolchain
1. `cd` to the project root `MIDAS/`
1. `cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" -S . -B build/release`
1. `cmake --build build/release --target Demo`
1. `cd` to `MIDAS/build/release/src`
1. `.\Demo.exe`

If you use Linux/macOS systems:

1. Open a terminal
1. `cd` to the project root `MIDAS/`
1. `cmake -DCMAKE_BUILD_TYPE=Release -S . -B build/release`
1. `cmake --build build/release --target Demo`
1. `cd` to `MIDAS/build/release/src`
1. `./Demo`

The demo runs on `MIDAS/data/DARPA/darpa_processed.csv`, which has 4.5M records, with the filtering core.

The scores will be exported to `MIDAS/temp/Score.txt`, higher means more anomalous.

All file paths are absolute and "hardcoded" by CMake, but it's suggested NOT to run by double-click on the executable file.

## Customization

### Switch Cores

Cores are instantiated at `MIDAS/example/Demo.cpp:64-66`, uncomment the chosen one.

### Custom Dataset + `Demo.cpp`

You need to prepare three files:

- Meta file
  - Only includes an integer `N`, the number of records in the dataset
  - Use its path for `pathMeta`
  - E.g. `MIDAS/data/DARPA/darpa_shape.txt`
- Data file
  - A header-less csv format file of shape `[N,3]`
  - Columns are sources, destinations, timestamps
  - Use its path for `pathData`
  - E.g. `MIDAS/data/DARPA/darpa_processed.csv`
- Label file
  - A header-less csv format file of shape `[N,1]`
  - The corresponding label for data records
    - 0 means normal record
    - 1 means anomalous record
  - Use its path for `pathGroundTruth`
  - E.g. `MIDAS/data/DARPA/darpa_ground_truth.csv`

### Custom Dataset + Custom Runner

1. Include the header `MIDAS/CPU/NormalCore.hpp`, `MIDAS/CPU/RelationalCore.hpp` or `MIDAS/CPU/FilteringCore.hpp`
1. Instantiate cores with required parameters
1. Call `operator()` on individual data records, it returns the anomaly score for the input record

## Online Coverage

1. [ACM TechNews](https://technews.acm.org/archives.cfm?fo=2020-05-may/may-06-2020.html)
2. [AIhub](https://aihub.org/2020/05/01/interview-with-siddharth-bhatia-a-new-approach-for-anomaly-detection/)
3. [Hacker News](https://news.ycombinator.com/item?id=22802604)
4. [KDnuggets](https://www.kdnuggets.com/2020/04/midas-new-baseline-anomaly-detection-graphs.html)
5. [Microsoft](https://techcommunity.microsoft.com/t5/azure-sentinel/announcing-the-azure-sentinel-hackathon-winners/ba-p/1548240)
6. [Towards Data Science](https://towardsdatascience.com/controlling-fake-news-using-graphs-and-statistics-31ed116a986f)

## MIDAS in Other Languages

1. [Golang](https://github.com/steve0hh/midas) by [Steve Tan](https://github.com/steve0hh)
2. [Ruby](https://github.com/ankane/midas) by [Andrew Kane](https://github.com/ankane)
3. [Rust](https://github.com/scooter-dangle/midas_rs) by [Scott Steele](https://github.com/scooter-dangle)
4. [R](https://github.com/pteridin/MIDASwrappeR) by [Tobias Heidler](https://github.com/pteridin)
5. [Python](https://github.com/ritesh99rakesh/pyMIDAS) by [Ritesh Kumar](https://github.com/ritesh99rakesh)
6. [Java](https://github.com/jotok/MIDAS-Java) by [Joshua Tokle](https://github.com/jotok)
7. [Julia](https://github.com/ashryaagr/MIDAS.jl) by [Ashrya Agrawal](https://github.com/ashryaagr)

## Citation

If you use this code for your research, please consider citing our paper.

```
@inproceedings{bhatia2020midas,
    title="MIDAS: Microcluster-Based Detector of Anomalies in Edge Streams",
    author="Siddharth {Bhatia} and Bryan {Hooi} and Minji {Yoon} and Kijung {Shin} and Christos {Faloutsos}",
    booktitle="AAAI 2020 : The Thirty-Fourth AAAI Conference on Artificial Intelligence",
    year="2020"
}
```
