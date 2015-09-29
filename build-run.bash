#!/bin/bash
if ! $(command -v uuidgen >/dev/null 2>&1); then
	echo "error: unix command 'uuidgen' dependency required"
	exit 1
fi

if ! $(command -v hostname >/dev/null 2>&1) ; then
	echo "error: unix command 'hostname' dependency required"
	exit 1
fi

export TARGET_NAME_OPT=$(uuidgen)
game=$TARGET_NAME_OPT

set -e
cd src
make opt
cd ../bin
./$game $game $(hostname -f)
rm $game
