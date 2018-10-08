# owlapi

This library reimplements owlapi (https://github.com/owlcs/owlapi) functionality
for C++ projects.
It does not (yet) support the full feature set of the original JAVA-based owlapi
implementation, but a significant subset.

## Installation

### Dependencies

This library depends on the following packages:

| Name       | Description |
|--------|-----|
| [boost](https://www.boost.org/users/license.html) | Boost: protable C++ source libraries            |
| [base/cmake](http://github.com/rock-core/base-cmake.git) | Essential CMake Macros for simplication of the build management            |
| [base/logging](https://github.com/rock-core/base-logging) | C++ Logging Library |
| [knowledge_reasoning/fact++](https://bitbucket.org/dtsarkov/factplusplus)  | Description Logic (DL) Reasoner |
| [uriparser](https://uriparser.github.io/) | Library for RFC 3986 compliant URI parsing |
| [soprano](http://soprano.sourceforge.net) | A Qt-based framwork for RDF data |
| [redland](http://librdf.org/LICENSE.html) | Resource Description Framework (RDF) handling, including (de)serialization and inmemory representation |
| [raptor2](http://librdf.org/LICENSE.html) | RDF handling as part of the redland libraries |
| [utilmm](https://github.com/orocos-toolchain/utilmm) | C++-helper libraries, here use a C++-Wrapper for pkg-config |


### Installation

Either install all dependencies by hand, then:
```
    mkdir build
    cd build
    cmake ..
    make
```

or use the existing Rock infrastructure:

```
    sudo apt install ruby wget
    mkdir your_workspace_dir
    cd your_workspace_dir
    wget http://www.rock-robotics.org/master/autoproj_bootstrap
    ruby autoproj_bootstrap
```

and add the following package to autoproj/manifest
```
    layout:
        - knowledge_reasoning/owlapi
```

```
    $amake knowledge_reasoning/owlapi
```

## Documentation

### API Documentation
To see the documentation for this project please do the following after
checkout to generate the doxygen-based documentation:

```
    mkdir build
    cd build
    make doc
```

Open the doxygen documentation with a browser: build/doc/index.html
The doxygen documentation contains all information on how to start
using the library along with the general API documentation.

## Tests
To run the tests you have to enable the testing suite first.
Either by explicitly setting ROCK_TEST_ENABLE, or by
calling
```
    autoproj test enable knowledge_reasoning/owlapi
    autoproj rebuild knowledge_reasoning/owlapi
```

Tests are based on Boost Testing.
They can be run with the following command with the project folder, after
building the project.

```
export BOOST_TEST_CATCH_SYSTEM_ERRORS="no"
./build/test/owlapi-test

```



## COPYRIGHT
Copyright (c) 2013-2018 Thomas M Roehr, DFKI GmbH Robotics Innovation Center

## LICENSE
This project is licensed under [LGPL v2.1 or later](https://www.gnu.org/licenses/lgpl-2.1.txt)

