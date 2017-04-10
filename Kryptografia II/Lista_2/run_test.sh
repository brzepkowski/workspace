#!/bin/bash

echo "Zaczynam..."
dieharder -a -g 201 -f "RC4(256, 256).in" | tee "RC4(256, 256)-result.txt"
echo "------------1 - skonczony ---------------"
dieharder -a -g 201 -f "RC4-RS(256, 4096).in" | tee "RC4-RS(256, 4096)-result.txt"
echo "------------2 - skonczony ---------------"
dieharder -a -g 201 -f "RC4-SST(256).in" | tee "RC4-SST(256)-result.txt"
echo "------------3 - skonczony ---------------"

