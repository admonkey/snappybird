#!/bin/bash
keepInstanceFiles=true
updateDatabase=true

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

if ( $keepInstanceFiles ); then
	cp ScoreChanges.csv $game.ScoreChanges.csv
	cp Settings.json $game.Settings.json
	cp .qtable $game.qtable
else
	echo "deleting files..."
	rm ScoreChanges.csv
	rm Settings.json
	rm .qtable

	# exit without updating database
	exit 0
fi

if !( $updateDatabase ); then
	exit 0
fi

if ! $(command -v jq >/dev/null 2>&1) ; then
	echo "Error: unix command 'jq' not installed. Can't parse JSON to create SQL statements."
	exit 1
fi

# make sure MySQL mode 'STRICT_TRANS_TABLES' is enabled
# SELECT @@GLOBAL.sql_mode;
# SET GLOBAL sql_mode = 'STRICT_TRANS_TABLES';

alterUUID="CALL SetInstanceID('$game');"
query="INSERT INTO ScoreChangeCount (GameNumber, Score) VALUES"
revertUUID="; CALL UnSetInstanceID();"
insertStateVars=""

for key in $(jq '.StateSettings.Variables | keys[]' $game.Settings.json); do
	# remove string quotes from key
	key=$(echo $key | sed "s/^\([\"']\)\(.*\)\1\$/\2/g")
	insertStateVars=$insertStateVars"INSERT INTO Instance_StateVariables (InstanceID,StateVariableID) VALUES ('$game',$key); "
done

insertInstanceSettingsKeys="InstanceID"
insertInstanceSettingsVals="'$game'"

# get previous instanceID if exists
previousInstanceID=$(jq .PreviousInstanceID $game.Settings.json | tr \" \')
if [[ ! -z "$previousInstanceID" ]]; then
	insertInstanceSettingsKeys=$insertInstanceSettingsKeys",PreviousInstanceID"
	insertInstanceSettingsVals=$insertInstanceSettingsVals",$previousInstanceID"
fi

# hostname
insertInstanceSettingsKeys=$insertInstanceSettingsKeys",Hostname"
val=$(jq .Hostname $game.Settings.json | tr \" \')
insertInstanceSettingsVals=$insertInstanceSettingsVals",$val"

# agent settings
for key in $(jq '.AgentSettings | keys[]' $game.Settings.json); do
	# remove string quotes from key
	key=$(echo $key | sed "s/^\([\"']\)\(.*\)\1\$/\2/g")
	insertInstanceSettingsKeys=$insertInstanceSettingsKeys",$key"
	val=$(jq .AgentSettings.$key $game.Settings.json)
	insertInstanceSettingsVals=$insertInstanceSettingsVals",$val"
done

# State.Discretizer
insertInstanceSettingsKeys=$insertInstanceSettingsKeys",Discretizer"
insertInstanceSettingsVals=$insertInstanceSettingsVals",$(jq .StateSettings.Discretizer $game.Settings.json)"

# instance scores
for key in $(jq '.InstanceScores | keys[]' $game.Settings.json); do
	# remove string quotes from key
	key=$(echo $key | sed "s/^\([\"']\)\(.*\)\1\$/\2/g")
	insertInstanceSettingsKeys=$insertInstanceSettingsKeys",$key"
	val=$(jq .InstanceScores.$key $game.Settings.json)
	insertInstanceSettingsVals=$insertInstanceSettingsVals",$val"
done

# total scores
for key in $(jq '.TotalScores | keys[]' $game.Settings.json); do
	# remove string quotes from key
	key=$(echo $key | sed "s/^\([\"']\)\(.*\)\1\$/\2/g")
	insertInstanceSettingsKeys=$insertInstanceSettingsKeys",$key"
	val=$(jq .TotalScores.$key $game.Settings.json)
	insertInstanceSettingsVals=$insertInstanceSettingsVals",$val"
done

# times
insertInstanceSettingsKeys=$insertInstanceSettingsKeys",StartTime"
insertInstanceSettingsVals=$insertInstanceSettingsVals",$(jq .Time.StartTime $game.Settings.json | tr \" \')"
insertInstanceSettingsKeys=$insertInstanceSettingsKeys",EndTime"
insertInstanceSettingsVals=$insertInstanceSettingsVals",$(jq .Time.EndTime $game.Settings.json | tr \" \')"

insertInstanceSettings="INSERT INTO InstanceSettings ($insertInstanceSettingsKeys) VALUES ($insertInstanceSettingsVals);"

# insert record into database
if $(command -v mysql >/dev/null 2>&1); then
mysql --host=localhost --user=snappyAgent --password=TLQPdTfsGqm2utb4 snappy << EOF
$insertInstanceSettings
$insertStateVars
$alterUUID
$query
$(cat $game.ScoreChanges.csv)
$revertUUID
EOF
else
	echo "no MySQL client installed."
	echo "writing sql statements to bin/$game.sql"
	echo $insertInstanceSettings > $game.sql
	echo $insertStateVars >> $game.sql
	echo $alterUUID >> $game.sql
	echo $query >> $game.sql
	echo " -- copy/paste $game.ScoreChanges.csv values here." >> $game.sql
	echo $revertUUID >> $game.sql
fi
