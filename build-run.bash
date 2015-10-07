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
cp ScoreChanges.csv $game.ScoreChanges.csv
cp Settings.json $game.Settings.json
cp .qtable $game.qtable

# make sure MySQL mode 'STRICT_TRANS_TABLES' is enabled
# SELECT @@GLOBAL.sql_mode;
# SET GLOBAL sql_mode = 'STRICT_TRANS_TABLES';

alterUUID="CALL SetInstanceID('$game');"
query="INSERT INTO ScoreChangeCount (GameNumber, Score) VALUES"
revertUUID=";CALL UnSetInstanceID();"
insertStateVars=""

for key in $(jq '.StateSettings.Variables | keys[]' $game.Settings.json); do
	# remove string quotes from key
	key=$(echo $key | sed "s/^\([\"']\)\(.*\)\1\$/\2/g")
	insertStateVars=$insertStateVars"INSERT INTO Instance_StateVariables (InstanceID,StateVariableID) VALUES ('$game',$key); "
done

# insert record into database
mysql --host=localhost --user=snappyAgent --password=TLQPdTfsGqm2utb4 snappy << EOF
$insertStateVars
$alterUUID
$query
$(cat $game.ScoreChanges.csv)
$revertUUID
EOF
