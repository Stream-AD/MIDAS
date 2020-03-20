# RejectMIDAS

C++ implementation of the RejectMIDAS algorithm.

## Demo

Due the file size limitation, you need to download the dataset.

1. 

If you use Window:
1. Download `final_dataset.csv` from the link below
2. Move `final_dataset.csv` to `RejectMIDAS/data`
3. `""`
4. Open a Visual Studio developer command prompt
5. `cd` to the project root `RejectMIDAS`
6. `cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" -S . -B build/release`
7. `cmake --build build/release --target CPU_Demo`
8. `"build/release/CPU/CPU_Demo.exe"`

The demo runs on the `final_dataset_processed.csv`, which has 12M records, with the relational core.

The scores will be exported to `RejectMIDAS/temp/score.txt`, higher means more anomalous.

`cmake` must be called at the project root to "hardcode" file paths. So the demo can be run from anywhere.

The last step must be within quotes, so as to be regarded as a path.

## Environment

Below is my development environment.

- CMake: 3.16.3
- MSVC: v142, x64, c++20
- Windows 10 SDK: 10.0.183622.0
- Python: 3.7.6
	- Optional, only for evaluating scores

## Dataset

- `darpa_original.csv`: <https://www.comp.nus.edu.sg/~sbhatia/assets/datasets/darpa_original.csv>
- `final_dataset.csv`: <https://www.kaggle.com/devendra416/ddos-datasets>, the balanced one.

## Customization

### Switch Cores

Cores are instantiated at `Demo.cpp:49-50`, uncomment the chosen one.

### Other Provided Datasets

Dataset paths are declared in `Demo.cpp:9-17`.

Each dataset has a `pathMeta` and a `pathData`, uncomment your chosen group.

### Custom Datasets + `Demo.cpp`

You need to prepare two files:
- A text file only consists an integer, the number of records in the dataset (that you want to use).
- A csv file (without headers) consists of four columns of integers, which stand for sources, destinations, timestamps and labels, respectively. 

Then you can change the `pathMeta` and `pathData` to your file paths, and run the demo.

### Custom Datasets + Custom Runner

Include the header `CPU/NormalCore.hpp` or `CPU/RelationalCore.hpp`, the implementations are all in-place.

Then, instantiate the core and use `operator()` to process one record and obtain the score.

## Contact

If you have any questions, please consider opening a issue instead of directly sending emails to me.