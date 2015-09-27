#!/bin/bash
set -e
cd src
make opt
cd ../bin
./game
