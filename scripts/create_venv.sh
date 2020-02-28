#!/usr/bin/env bash
if [ ! -d "venv" ]; then
    echo "Creating a fresh virtualenv..."
    pip install -U pip setuptools wheel
    python -m venv venv
else
    echo "Virtualenv already created."
fi
