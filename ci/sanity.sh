#!/usr/bin/env bash
script_dir=$(dirname $0)
work_dir=$(pwd)
echo "----------------------------------------"
echo "------- Cloning   into  sanity  --------"
echo "----------------------------------------"
echo "workdir : $work_dir"
echo "script_dir : $script_dir"
RUNTIME_DIR=$(pwd)
export PATH=$TMCI_SANITY_PATH # Binary dirs, multiple dirs
mkdir testing
cd testing
TESTING_DIR=$(pwd)

echo "===== Running test.sh ====="
python3 ./vmsrc/tests/run_test.py -n
echo "================================="
cd ..
echo " running copying from $(pwd)"
mkdir testresult/sanity
cp -r ./testing/open64-sanity/test/*.log ./testresult/sanity/
echo "FINISHED" > ./testresult/SANITY_FINISHED
echo "================================"
echo "--- Sanity Test Finished ------"
echo "================================"
