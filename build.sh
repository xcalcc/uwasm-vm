#!/usr/bin/env bash

BUILD_TYPE=Debug
BUILD_TARGET=""
BUILD_DIR=build
BUILD_OPT=""
CMAKE_STR="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
usage()
{
  echo "Usage: $0 [-d|-r|-m32|-h]"
  echo "  -d: build debug version [default]"
  echo "  -r: build release version"
  echo "  -m32: build 32 bit"
  echo "  -acc-mem: vm direct access memory"
  echo "  -h: print usage"
}

parse_option()
{
  while [ $# -gt 0 ]
  do
    key="$1"
    case $key in
      -d)
        BUILD_TYPE=Debug
        CMAKE_STR="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
      ;;
      -r)
        BUILD_TYPE=Release
        BUILD_DIR=build-release
        CMAKE_STR="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
      ;;
      -m32)
        BUILD_TARGET=uvm-m32
      ;;
      -acc-mem)
        BUILD_OPT="-DDIRECT_ACC_MEM $BUILD_OPT"
      ;;
      -h)
        usage
        exit 0
      ;;
      *)
        usage
        exit -1
      ;;
    esac
    shift
  done

  if [ "${BUILD_TARGET}" = "uvm-m32" ]
  then
    if [ "$BUILD_TYPE" = "Release" ]
    then
      BUILD_DIR=build-m32-release
    elif [ "$BUILD_TYPE" = "Debug" ]
    then
      BUILD_DIR=build-m32
    fi
    CMAKE_STR="-DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_TARGET=$BUILD_TARGET"
  fi 
  CMAKE_STR="$CMAKE_STR -DCMAKE_CXX_FLAGS=$BUILD_OPT"
}

parse_option $@

cd $(dirname $0);
echo "Workdir : $(pwd)";


mkdir -p $BUILD_DIR
cd $BUILD_DIR
echo "Build dir : $(pwd)"
echo ">> cmake .. ${CMAKE_STR}"
cmake .. ${CMAKE_STR}
echo ">> make "
make uvm
RET=$?
if [ $RET -ne 0 ]; then
  exit -1
fi
make uvm_encoder
RET=$?
if [ $RET -ne 0 ]; then
  exit -1
fi
make u_sld
RET=$?
if [ $RET -ne 0 ]; then
  exit -1
fi
