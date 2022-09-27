DBAR="-------------------------------------------"
echo $DBAR
echo "--------- Starting Analyzes ----------"
PREVDIR=$(pwd)
if test -f ./BASE_DIR
then
    LASTTIME_DIR=$(cat BASE_DIR)
    echo "FOUND BASE_DIR : $LASTTIME_DIR"
else
    exit 2
fi
cd $LASTTIME_DIR

echo $DBAR
echo "-- Inside : $(pwd)"

set -x;

echo "========== Cleanup Starting ==========="
rm -rf ./testing
rm -rf ./baseline
rm -rf ./install
rm -rf ./testresult/.xcalibyte
tail -n 100 ./logs/make.err.log > ./make.err.log.tail
rm -rf ./logs
# rm ./logs/make.err.log
echo "========== Cleanup Finished ==========="
echo "TEST_ANALYZE FINISHED" > ./STATUS
echo "TEST_ANALYZE FINISHED" > $PREVDIR/STATUS
echo " To see the testresults, goto :           "
echo " cd $LASTTIME_DIR/testresult            "
echo " find                                   "
echo "----------------------------------------"
