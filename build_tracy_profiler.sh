#!/bin/bash

pushd build/_deps/tracy-src/profiler/build/unix
make -j 8
popd

