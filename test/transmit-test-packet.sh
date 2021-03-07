#!/bin/bash

BASEDIR=$(dirname "$0")

TMP_DIR=$(mktemp -d)
mkdir -p $TMP_DIR

kissutil -f $TMP_DIR &

while true
do
    time=$(date +%s)
    cp "$BASEDIR/test-aprs-packet.txt" $TMP_DIR
    sleep 2
done