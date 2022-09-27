SET(CMAKE_C_FLAGS             "${CMAKE_C_FLAGS} -m32 -DTARG_32BIT" CACHE STRING "C compiler flags"   FORCE)
SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -m32 -DTARG_32BIT" CACHE STRING "C++ compiler flags" FORCE)
set(LIBFFI_DIR ${LIBS_DIR}/libffi32 CACHE STRING "The folder of libffi install dir" FORCE)
