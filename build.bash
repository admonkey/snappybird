#!/bin/bash
set -e
cd java
javac Game.java Model.java View.java Controller.java
echo "Build complete. To run, do:"
echo "  cd java && java Game"
echo "and start tapping keys or your mouse button."
