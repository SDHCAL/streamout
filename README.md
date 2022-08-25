# SDHCAL v0.1.0 #

Standalone SDHCAL raw data processor

[![macOS](https://github.com/SDHCAL/streamout/workflows/macOS/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/macOS.yml)
[![Linux GCC](https://github.com/SDHCAL/streamout/workflows/Linux%20GCC/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/Linux-gcc.yml)
[![Linux Clang](https://github.com/SDHCAL/streamout/workflows/Linux%20Clang/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/Linux-clang.yml)
[![Windows](https://github.com/SDHCAL/streamout/workflows/Windows/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/Windows.yml)
[![CMake ilcsoft](https://github.com/SDHCAL/streamout/actions/workflows/CMake-ilcsoft.yml/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/CMake-ilcsoft.yml)

[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/SDHCAL/streamout/main.svg)](https://results.pre-commit.ci/latest/github/SDHCAL/streamout/main)

## Available interfaces ##

|                Description                |    Type    |                                                                                           Build                                                                                            |
|:-----------------------------------------:|:----------:|:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
| Reading the rawdata from binary file .dat | **Input**  | [![Rawdata interface](https://github.com/SDHCAL/streamout/actions/workflows/Rawdata-interface.yml/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/Rawdata-interface.yml) |
|            Write to ROOT file             | **Output** |     [![ROOT interface](https://github.com/SDHCAL/streamout/actions/workflows/ROOT-interface.yml/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/ROOT-interface.yml)      |
|            Write to LCIO file             | **Output** |     [![LCIO interface](https://github.com/SDHCAL/streamout/actions/workflows/LCIO-interface.yml/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/LCIO-interface.yml)      |

## Documentation [![docs](https://github.com/SDHCAL/streamout/actions/workflows/Docs.yml/badge.svg)](https://github.com/SDHCAL/streamout/actions/workflows/Docs.yml) ##

<h1 align="center">
  <a href="https://sdhcal.github.io/assets/projects/streamout/">

  ```markdown
  🌍 Online
  ```

  </a>
  <a href="https://sdhcal.github.io/assets/projects/streamout/streamout_Manual.pdf">

  ```markdown
  📖 PDF
  ```

  </a>
</h1>

## How to compile ##

### CMake options ###

**BUILD_ROOT_INTERFACE** : Allow to write **ROOT** files.</br>
**BUILD_LCIO_INTERFACE** : Allow to write **LCIO** files (not implemented).</br>
**BUILD_RAWDATA_INTERFACE** : Allow to read RAWDATA from **.dat** files.</br>
**BUILD_TESTS** : Build the tests.</br>
**BUILD_APPS** : Build the apps.</br>
**BUILD_DOCS** : Build the documentation.</br>

### Use cmake (tested with version 3.15.3 ... 3.24) ###

1) `git clone https://github.com/SDHCAL/streamout.git`
2) `cmake -DBUILD_ROOT_INTERFACE=ON -DBUILD_LCIO_INTERFACE=ON -DBUILD_RAWDATA_INTERFACE=ON -B ./build -S ./streamout -DCMAKE_INSTALL_PREFIX=./install`
3) `cmake --build ./build --parallel 10`
4) `cmake --install build`
5) `ctest -VV`

## How to use the software ##

⚠️ It still under testing so they are in `tests` folder

1) You can ``export STREAMOUT_OUTPUT_PATH=mypath`` to select where the apps will write the output file.
2) Run `./install/bin/tests/RawdataROOT.test --help` to list the parameters.
3) For example `./install/bin/tests/RawdataROOT.test -f ./SMM_210616_040234_732873.dat -s 20 -e 10`.  Will read the **10** first event skipping the first **20** bits of the header and write **ROOT** file `SMM_210616_040234_732873.root`.
4) You can cross-check the output using `../install/bin/tests/ROOTchecker.test -f SMM_210616_040234_732873.root`.
