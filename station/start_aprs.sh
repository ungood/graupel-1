#!/bin/sh

BASEDIR=$(dirname "$0")
CONFIG_FILE="$BASEDIR/sdr.conf"

echo "Supported gain values (14): -1.0 1.5 4.0 6.5 9.0 11.5 14.0 16.5 19.0 21.5 24.0 29.0 34.0 42.0"
GAIN=$1
echo "Starting rtf_fm -f 144.39M -g $GAIN"

rtl_fm -f 144.39M -g $GAIN | direwolf -c $CONFIG_FILE -r 24000 -D 1 -