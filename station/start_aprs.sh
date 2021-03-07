#!/bin/sh

BASEDIR=$(dirname "$0")
CONFIG_FILE="$BASEDIR/sdr.conf"

rtl_fm -f 144.39M - | direwolf -c $CONFIG_FILE -r 24000 -D 1 -