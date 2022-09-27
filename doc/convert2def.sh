#!/usr/bin/env bash
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
cd $SCRIPT_DIR

python3 convert2def.py
