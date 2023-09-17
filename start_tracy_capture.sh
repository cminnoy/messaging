#!/bin/bash

build/_deps/tracy-src/capture/build/unix/capture-release -f -o messaging.tracy -a 127.0.0.1 &> tracy.log &

