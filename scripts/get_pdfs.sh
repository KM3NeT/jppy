#!/usr/bin/env bash
if [ ! -d "pdfs" ]; then
    echo "Retrieving PDFs..."
    mkdir pdfs
    cd pdfs
    for i in {1..6}; do
        wget "http://pi1139.physik.uni-erlangen.de/data/latest/J${i}p.dat"
    done
else
    echo "PDFs already downloaded."
fi
