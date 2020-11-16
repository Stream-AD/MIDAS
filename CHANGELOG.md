## v1.1.2 (2020.11.16)

- Change C++ standard 20 -> 11
- \- unused `#include` in `Demo` and `Reproducible`
- Convert `const char*` to `const auto` in `Demo`

## v1.1.1 (2020.09.29)

- \- unused `#include <algorithm>` in `RelationalCore` and `FilteringCore`

## v1.1.0 (2020.09.20)

- Partially vectorize MIDAS-F's conditional merge
    - Reduce running time by ~10%
- \+ a reproducible (no-random) demo
    - To test implementations in other languages
- \+ official python implementation
    - See `README.md`
- Merge `EdgeHash.hpp` and `NodeHash.hpp` -> `CountMinSketch.hpp`
- Change the method signature of `MIDAS::CountMinSketch::Hash()`
    - `indexOut` is the first, same as other methods
    - `b` has a default value `0`
- Merge `src/CMakeLists.txt` into `CMakeLists.txt`
- Rename variable `MIDAS::*Core::timestampCurrent` -> `MIDAS::*Core::timestamp`
    - Use `this->` to differentiate
- Rename macro `ParallelProvider_*` -> `ParallelizationProvider_*`
    - Only used in `example/Experiment.cpp`

## v1.0.2 (2020.07.23)

- Rename parameter name `to` -> `with` of `Assign()` in `EdgeHaash.hpp` and `NodeHash.hpp`

## v1.0.1 (2020.06.25)

- Remove useless script file `MIDAS/util/PlotAnomalousEvent.py`
- Unify import style in `MIDAS/util/PreprocessData.py`
- Add tbb support for `MIDAS/example/Experiment.cpp`

## v1.0.0 (2020.06.16)

- New implementation.
