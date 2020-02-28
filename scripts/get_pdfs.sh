#!/bin/bash
if [ ! -d "pdfs" ]; then
    mkdir pdfs
    cd pdfs
    for i in {1..6}; do
        wget "http://pi1139.physik.uni-erlangen.de/data/latest/J${i}p.dat"
    done
    cd ..
fi
