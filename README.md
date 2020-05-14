# MIDAS

C++ implementation of the MIDAS algorithm.

## Demo

Due to the file size limitation, you need to download the dataset manually:

1. Download `final_dataset.csv` using the link below
2. Move `final_dataset.csv` to `MIDAS/data/`
3. Run `MIDAS/util/PreprocessData.py`, the script can detect the correct file path

To run the demo, if you have a CMake-based IDE, you can build the target `CPU_Demo` and enjoy.

Otherwise, if you use Windows:

1. Open a Visual Studio developer command prompt, we want their toolchain
2. `cd` to the project root `MIDAS/`
3. `cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" -S . -B build/release`
4. `cmake --build build/release --target CPU_Demo`
5. `cd` to `MIDAS/build/release/CPU/`
6. `.\CPU_Demo`

The code should be compatible with Linux, but I only tested with WSL.

The demo runs on the `MIDAS/data/final_dataset_processed.csv`, which has 12M records, with the relational core.

The scores will be exported to `MIDAS/temp/score.txt`, higher means more anomalous.

All file paths are absolute and "hardcoded", but if you double-click to run, you may miss the ROC-AUC metric.

## Environment

Below is my Windows environment.

- CMake: 3.16.3
- MSVC: v142, x64, c++20
- Windows 10 SDK: 10.0.183622.0
- Python: 3.7.6
  - Optional, only for evaluating scores

## Dataset

- `darpa_original.csv`: <https://www.comp.nus.edu.sg/~sbhatia/assets/datasets/darpa_original.csv>
- `final_dataset.csv`: <https://www.kaggle.com/devendra416/ddos-datasets>, the balanced one

## Customization

### Switch Cores

Cores are instantiated at `MIDAS/example/Demo.cpp:53-54`, uncomment the chosen one.

### Other Provided Datasets

1. Download the dataset from links above, the file name should be exactly the same
1. Move it to `MIDAS/data/`
1. Uncomment corresponding function in `MIDAS/util/PreprocessData.py`
1. Run the script
1. Uncomment the corresponding group of `pathMeta` and `pathData` within `MIDAS/example/Demo.cpp:12-18`
1. Compile and run

### Custom Datasets + `Demo.cpp`

You need to prepare two files:
- A text file only consists an integer, the number of records in the dataset
- A csv file (without headers) consists of four columns of integers, which stand for sources, destinations, timestamps and labels, respectively

Then you can change the `pathMeta` and `pathData` to your file paths, and run the demo.

### Custom Datasets + Custom Runner

Include the header `MIDAS/CPU/NormalCore.hpp` or `MIDAS/CPU/FilteringCore.hpp`, the implementations are all in-place.

Then, instantiate the core and use `operator()` to process one record and obtain the score.

## Contact

If you have any questions, please consider opening an issue instead of directly sending emails to me.