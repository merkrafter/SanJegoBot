![Platform independency](https://github.com/merkrafter/SanJegoBot/workflows/Platform%20independency/badge.svg)

# SanJego
Algorithms for playing the Game of San Jego.

# Getting started
This is a CMake project and adheres to its conventions.

## Building the library
This project supports both in- and out-of-sources builds, hence it does not matter where the `build` folder is created.
```bash
$ cmake -E make_directory build
$ cmake -DCMAKE_BUILD_TYPE=release -B build -S path/to/this/directory
$ cmake --build build --target sanjego
```

## Building and running the tests

The option for building tests is called `BUILD_TESTING`.
It is turned on by default, but you can deactivate it by passing `-DBUILD_TESTING=OFF` to the initialization command.

```bash
$ cmake -E make_directory build
$ cmake -B build -S path/to/this/directory
$ cmake --build build
$ cd build
$ ctest
```
