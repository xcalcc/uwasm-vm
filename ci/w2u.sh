running_dir=$(pwd)
cd $(realpath "$(dirname $(realpath $0))")
script_dir=$(pwd)
cd ..
cu_dir=$(realpath .)
uwasm_dir=${UWASM_VM_DIR:-${cu_dir}}


cd $running_dir
echo ">> Working dir : $running_dir, Uwasm dir: $uwasm_dir"
echo ">> $uwasm_dir/cmake-build-debug/wasm_to_uwasm -t 5 -o ${uwasm_dir}/w2u.uwm $*"
$uwasm_dir/cmake-build-debug/wasm_to_uwasm -t 5 -o ${uwasm_dir}/w2u.uwm $*
ret=$?
exit $ret