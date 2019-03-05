#!/bin/bash
OUTPUT_DIR=Build

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core

#Build with coverage enabled
make -f Makefile.cov clean
make -f Makefile.cov

#Run baseline coverage
lcov --capture --initial --directory . --no-external --output-file $OUTPUT_DIR/MARTe2-components.coverage.info.initial

#Execute the tests
Test/GTest/cov/MainGTest.ex --gtest_filter=*

#Create test coverage data file
lcov --capture --directory . --no-external --output-file $OUTPUT_DIR/MARTe2-components.coverage.info.tests

#Combine baseline and test coverage data
lcov --add-tracefile $OUTPUT_DIR/MARTe2-components.coverage.info.initial --add-tracefile $OUTPUT_DIR/MARTe2-components.coverage.info.tests --output-file $OUTPUT_DIR/MARTe2-components.coverage.info.1

#Remove false positives
lcov --remove $OUTPUT_DIR/MARTe2-components.coverage.info.1 "/Test*" --output-file $OUTPUT_DIR/MARTe2-components.coverage.info.2
lcov --remove $OUTPUT_DIR/MARTe2-components.coverage.info.2 "*gtest*" --output-file $OUTPUT_DIR/MARTe2-components.coverage.info

#Generate the html
genhtml $OUTPUT_DIR/MARTe2-components.coverage.info --output-directory $OUTPUT_DIR/cov_html

make -f Makefile.cov clean_gen
