#!/bin/bash

pushd build/_deps/tracy-src/capture/build/unix
make -j 8
popd

