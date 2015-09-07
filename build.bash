#!/bin/bash
set -e
cd src
make opt
echo "To run, do:"
cd ../bin
./gametmp
