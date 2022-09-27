#!/usr/bin/env bash
cd $(dirname $0)
cd ..
workdir=$(pwd)
echo "Workdir : $(pwd)"

mkdir build-release
mkdir build

cd ${workdir}/build
cmake ..
if [ $? -ne 0 ] ; then
  exit 1;
fi

make uvm
if [ $? -ne 0 ] ; then
  exit 1;
fi

cd ${workdir}/build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0 ] ; then
  exit 1;
fi

make uvm
if [ $? -ne 0 ] ; then
  exit 1;
fi

make all_tests
if [ $? -ne 0 ] ; then
  exit 1;
fi

cd ${workdir};
echo "Finish all building in uvm.";

UVM_EXEC="./build/uvm"
TRACE_OPT=""
# ${UVM_EXEC} ${TRACE_OPT} file %s func %x sreg %x %x

