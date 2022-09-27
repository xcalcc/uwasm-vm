#!/bin/sh
set -x;
script_dir=$(dirname $0)
#$1 is the local compiler location
CTI_HOME=/proj/test/opencti-src
CTI_SRC=/proj/test/opencti-src
CTI_SERVER_ADDR=127.0.0.1
CTI_SERVER="ubuntu@""$CTI_SERVER_ADDR"
cmplr=$(cat ./BASE_DIR)/install
CORE_NIGHTLY_PFX="ubuntu"
CORE_PUSH_PFX="ubuntu"
PLUS_NIGHTLY_PFX="ubuntu"
PLUS_PUSH_PFX="ubuntu"
COMMON_PRESET="bash -x $CTI_HOME/istoire/setup_env.sh alter ; cd ~/CTI/compiler; tar jxf -; "
IDENTITY_FILE=${script_dir}/identity.pem

# get env settings
echo "----------------------------------------------------"
echo "        Uploading to CTI ... "
echo "----------------------------------------------------"
echo " --  cmplr : $cmplr "

if [ -z "$cmplr" ]; then
  echo "Usage: $0 <cmplr_install_prefix>"
  echo "  cmplr_install_prefix: the prefix passed to mvsa configure"
  echo "  e.g. $0 /home/user/install"
  echo "       where occ is in /home/user/install/bin"
  exit 1
fi

# echo "When password is prompted, please input your password on cti server."

PLUS_nightly() {
    echo "-- Running PLUS nightly specifics "
    cmd="$COMMON_PRESET $CTI_HOME/istoire/run_remote_nightly.sh"
    CTI_SERVER="$PLUS_NIGHTLY_PFX""@""$CTI_SERVER_ADDR" #gitlab-PLUS-nightly
    return
}

PLUS_plugin() {
    echo "-- Running PLUS plugin nightly specifics "
    cmd="$COMMON_PRESET $CTI_HOME/istoire/run_remote_nightly.sh"
    CTI_SERVER="$PLUS_NIGHTLY_PFX""@""$CTI_SERVER_ADDR" #gitlab-PLUS-nightly
    return
}

core_nightly() {
    echo "-- Running core nightly specifics () "
    cmd="$COMMON_PRESET $CTI_HOME/istoire/run_remote_nightly.sh"
    CTI_SERVER="$CORE_NIGHTLY_PFX""@""$CTI_SERVER_ADDR" # gitlab-core-nightly@cti.xc5.io
    return
}
    

core_push() {
    echo "-- Running core push specifics"
    cmd="$COMMON_PRESET $CTI_HOME/istoire/run_remote_push.sh"
    CTI_SERVER="$CORE_PUSH_PFX""@""$CTI_SERVER_ADDR" # gitlab-runner@cti.xc5.io
    return;
}

PLUS_push() {
    echo "-- Running PLUS push specifics"
    cmd="$COMMON_PRESET $CTI_HOME/istoire/run_remote_push_PLUS.sh"
    CTI_SERVER="$PLUS_PUSH_PFX""@""$CTI_SERVER_ADDR" # gitlab-PLUS@cti.xc5.io
    return;
}

protect_sanity() {
    echo "-- Running protect sanity --"
    cmd="$COMMON_PRESET $CTI_HOME/istoire/run_remote_pro_sanity.sh"
    CTI_SERVER="$PLUS_NIGHTLY_PFX""@""$CTI_SERVER_ADDR" #gitlab-PLUS-nightly
    return;

}

prepare_args() {
    if [ -f ./NIGHTLY ] && [ -f ./PLUS ] ; then
	PLUS_nightly;
	return;
    elif [ -f ./NIGHTLY ] ; then
	core_nightly;
	return;
    elif [ -f ./PLUSPLUGIN ] ; then
	PLUS_plugin;
	return;
    elif [ -f ./PLUSPUSH ] ; then
    PLUS_push;
	return;
    else
	core_push;
	return;
    fi
}
   
prepare_args;
echo "[CTI.sh] SERVER : ${CTI_SERVER} "
echo "[CTI.sh] [CMD] ${cmd}"

echo "SKIPPING CTI TESTS  .... ";
exit 0;


my_pwd=`pwd`
cd $cmplr
tar jhcf - bin \
  | ssh -i $IDENTITY_FILE $CTI_SERVER "$cmd"
cd $my_pwd
