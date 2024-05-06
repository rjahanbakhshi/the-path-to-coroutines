# The Path to Coroutines

<!--toc:start-->
- [The Path to Coroutines](#the-path-to-coroutines)
  - [Configure the build](#configure-the-build)
  - [Build all](#build-all)
  - [Build a specific target (coroutines target in this example)](#build-a-specific-target-coroutines-target-in-this-example)
  - [Running a specific target (coroutines target in this example)](#running-a-specific-target-coroutines-target-in-this-example)
  - [Sending commands using netcat](#sending-commands-using-netcat)
<!--toc:end-->

## Configure the build

cmake -B build -S . -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/conan_provider.cmake -DCMAKE_BUILD_TYPE=Debug

## Build all

cmake --build build -j

## Build a specific target (coroutines target in this example)

cmake --build build -j --target coroutines

## Running a specific target (coroutines target in this example)

build/coroutines 8090

## Sending commands using netcat

netcat 127.0.0.1 8090
