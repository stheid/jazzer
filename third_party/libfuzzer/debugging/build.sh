#!/bin/sh
LIBFUZZER_SRC_DIR=$(dirname $0)/..
CXX="${CXX:-clang}"
for f in $LIBFUZZER_SRC_DIR/*.cpp; do
  $CXX -g -O2 -fno-omit-frame-pointer -std=c++11 $f -c &
done
wait
rm -f libFuzzer.a
ar ru libFuzzer.a *.o
rm -f *.o
$CXX -fsanitize=fuzzer -g -O2 target.cc -c
$CXX++ libFuzzer.a target.o -lpthread -o target