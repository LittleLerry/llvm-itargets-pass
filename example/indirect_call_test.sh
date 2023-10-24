#!/bin/bash

set -euo pipefail
cd indirect_call_test

export CC=$AFLGO/instrument/aflgo-clang; export CXX=$AFLGO/instrument/aflgo-clang++
export CFLAGS="-fpass-plugin=../../instrument/aflgo-pass.so"

make clean; make
