# C++ Config 
[![Linux Build](https://github.com/voldien/cxx-config/actions/workflows/linux-build.yml/badge.svg)](https://github.com/voldien/cxx-config/actions/workflows/linux-build.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/voldien/cxx-config.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/voldien/cxx-config/context:cpp)
[![GitHub release](https://img.shields.io/github/release/voldien/cxx-config.svg)](https://github.com/voldien/cxx-config/releases)

A Simple library for adding support for hierarchical configuration as a C++ object.

## Motivation

Create a config library that is compatible with Xml, Json, YAML and INI, with support for generic template support.

## Installation

First, download the repository, with the following command. It will download the repository along with all the dependent git submodules it uses.

```bash
git clone --recurse-submodules https://github.com/voldien/cxx-config.git
```

The software can be easily installed by invoking the following command.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## License

This project is licensed under the GPL+3 License - see the [LICENSE](LICENSE) file for details
