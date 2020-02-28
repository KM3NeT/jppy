#!/usr/bin/env bash
if [ ! -d 'jpp' ]; then
    echo "Cloning Jpp..."
    git clone --depth 1 https://$KM3NET_USERNAME:$KM3NET_PASSWORD@git.km3net.de/common/jpp.git
else
    echo "Fetching latest master"
    cd jpp
    git pull
fi
