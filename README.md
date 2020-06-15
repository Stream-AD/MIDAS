# MIDAS

C++ implementation of MIDAS algorithms.

## Demo

If you have a CMake-based IDE, you can build the target `Demo` and enjoy.

Otherwise, if you use Windows:

1. Open a Visual Studio developer command prompt, we want their toolchain
1. `cd` to the project root `MIDAS/`
1. `cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" -S . -B build/release`
1. `cmake --build build/release --target Demo`
1. `cd` to `MIDAS/build/release/src`
1. `.\Demo.exe`

If you use Unix-like systems:

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
- Data file
  - A header-less csv format file of shape `[N,3]`
  - Columns are sources, destinations, timestamps
  - Use its path for `pathData`
- Label file
  - A header-less csv format file of shape `[N,1]`
  - The corresponding label for data records
    - 0 means normal record
    - 1 means anomalous record
  - Use its path for `pathGroundTruth` 

### Custom Dataset + Custom Runner

1. Include the header `MIDAS/CPU/NormalCore.hpp`, `MIDAS/CPU/RelationalCore.hpp` or `MIDAS/CPU/FilteringCore.hpp`
1. Instantiate cores with required parameters
1. Call `operator()` on individual data records, it returns the anomaly score for the input record.

## Contact

If you have any questions, please consider opening an issue instead of directly sending emails to us.