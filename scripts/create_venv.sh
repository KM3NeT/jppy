#!/usr/bin/env bash
if [ ! -d "venv" ]; then
    echo "Creating a fresh virtualenv..."
    python3 -m pip install -U pip setuptools wheel pybind11
    python3 -m venv venv
else
    echo "Virtualenv already created."
fi
