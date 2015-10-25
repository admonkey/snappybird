#!/bin/bash
set -e
cd java
javac Game.java Model.java View.java Controller.java
java Game
