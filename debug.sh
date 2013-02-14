#!/bin/bash
#./stop.sh
if [ -z "$DEBUG" ]; then
    echo "set \$DEBUG env var"
    exit
fi
pushd bin > /dev/null
$DEBUG gridtop $*
#read
popd > /dev/null
#./stop.sh
