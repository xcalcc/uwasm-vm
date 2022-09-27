DBAR="----------------------------------------------"
echo $DBAR
echo "------------- Starting Scirpt   ------------"
echo $DBAR
PREVDIR=$(pwd)
echo "Current : $(pwd)"
RET=0
RUN_TEST_MODE="NORMAL"
./ci/setup-env.sh

if test -f ./BASE_DIR
then
    LASTTIME_DIR=$(cat ./BASE_DIR)
    echo "FOUND BASE_DIR : $LASTTIME_DIR"
else
    echo $DBAR
    echo " [ERROR] cannot locate BASE_DIR file. "
    exit 3
fi

if test -d $LASTTIME_DIR
then
    if test -f $LASTTIME_DIR/BUILD_FINISHED
    then   
    cd $LASTTIME_DIR
    echo "Inside $(pwd) "
    else
	echo "$LASTTIME_DIR/BUILD_FINISHED not present! "
	exit 3
    fi
else
    echo "$LASTTIME_DIR not found!"
    exit 3
fi


# Copying scripts
echo $DBAR
echo " --------- Checking Environment ----------"
echo $DBAR
export TMCI_SANITY_PATH=$PATH:$LASTTIME_DIR/install/bin:$LASTTIME_DIR/install
printenv > ./TEST_PRINTENV
echo " ------------------------------------------"
echo " ----   Real Test Scripts Starting --------"
echo "$DBAR"
echo " ----   Test Suite Sanity Starting --------"
echo "$DBAR"
mkdir testresult

echo "${DBAR}"
echo "Prepare to perform XcalScan check."
./ci/sanity.sh || exit 4
echo $DBAR
./ci/cti.sh || exit 4
echo $DBAR

echo "--------- Copying to Gitlab --------------"
cp -rf ./testresult $PREVDIR/
echo "TEST FINISHED SUCCESS" > ./STATUS
echo "TEST FINISHED SUCCESS" > $PREVDIR/STATUS
echo "--------- cleaning          --------------"
rm -rf $LASTTIME_DIR/build
if test -f $LASTTIME_DIR/NIGHTLY
then
    echo " NIGHTLY_PRESERVE_INSTALl=OFF ... Not Preserving Install Dir"
    rm -rf $LASTTIME_DIR/install
    rm -rf $LASTTIME_DIR/occ
else
    rm -rf $LASTTIME_DIR/install
    rm -rf $LASTTIME_DIR/occ
fi
echo "--------- All Tests Completed ------------"

if [ ${RET} -ne 0 ]; then
    echo "------ Failed due to RET = ${RET} ; -------"
    exit 2;
fi
