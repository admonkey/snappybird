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

# make sure MySQL mode 'STRICT_TRANS_TABLES' is enabled
# SELECT @@GLOBAL.sql_mode;
# SET GLOBAL sql_mode = 'STRICT_TRANS_TABLES';

alterUUID="ALTER TABLE ScoreChangeCount MODIFY InstanceID char(36) Default '$(uuidgen)';"
query="INSERT INTO ScoreChangeCount (GameNumber, Score) VALUES"
revert=";ALTER TABLE ScoreChangeCount MODIFY InstanceID char(36) NOT NULL;"

# insert record into database
mysql --host=localhost --user=snappyAgent --password=TLQPdTfsGqm2utb4 snappy << EOF
$alterUUID
$query
$(cat ScoreChanges.csv)
$revert
EOF
