## UWASM-VM

UWASM-VM repository include a few parts:

1. UWASM VM: an interpreter, can load and run uwm format file
2. encoder: a library, create uwm file, and write content
3. u_sld: a uwm file editor, merge uwm file and so file
4. wasm_to_uwasm: a convertor, can convert wasm file to uwm file

All source code under `src` direcroty.

Documents are available in `doc` dir.

## Supported environment

| OS | bit length |
| :---: | :--: | :--: |
| Linux | 32 bit |
| Linux | 64 bit |
| OSX | 64 bit |

## Build the project

1. Common build phase

``` bash
cd uwasm-vm
mkdir build
cd build
cmake ..
make
make install
```

2. Specific build type

```bash
# build debug version
cmake .. -DCMAKE_BUILD_TYPE=Debug
# build release version
cmake .. -DCMAKE_BUILD_TYPE=Release
```

3. Specific install prefix

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
```

4. Specific extra cflags or cxxflags

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TARGET=uvm-m32
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TARGET=uvm-m32
```

5. Build part of components

```bash
# uvm: intepreter
make uvm
# uvm_encoder: encoder
make encoder
# u_sld: uwm editor
make u_sld
# wasm_to_uwasm: wasm to uwasm convertor
make wasm_to_uwasm
```

## Build third party libraries

1. build `libffi`

```bash
git clone https://github.com/libffi/libffi.git
cd libffi
mkdir build install
./autogen.sh
cd build
../configure --prefix=/abs/path/libffi/install
make
make install
# build debug version
../configure --prefix=/abs/path/libffi/install --enable-debug
# build 32 bit version
../configure --prefix=/abs/path/libffi/install CFLAGS="-m32" CXXFLAGS="-m32"
```

2. build `gtest`

```bash
git clone https://github.com/google/googletest.git
cd googletest
mkdir build install
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Release
make
make install
#build debug version
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Debug
# build 32 bit version
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-m32" -DCMAKE_CXX_FLAGS="-m32"
```

3. build `elfio`

```bash
git clone https://github.com/serge1/ELFIO.git
cd ELFIO
# just copy the elfio folder to include directory
```
