#!/usr/bin/env bash

export URL="https://sftp.km3net.de/data/v17.0.0/"
if [ ! -d "pdfs" ]; then
    echo "Retrieving PDFs..."
    mkdir pdfs
    cd pdfs
    for i in {1..6}; do
        wget "$URL/J${i}p.dat"
    done
    for i in {13..14}; do
        wget "$URL/J${i}p.dat"
    done
else
    echo "PDFs already downloaded."
fi
