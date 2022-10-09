#!/bin/bash

CURR_DIR=$(pwd)
SCRIPT_DIR=$(dirname "$0")

cd $SCRIPT_DIR
RUN_SIM=$(realpath ../build/X86/gem5.opt)
CONFIG=$(realpath ../configs/example/se.py)

cd $CURR_DIR
$RUN_SIM $CONFIG --cpu-type=DerivO3CPU --caches -c $@