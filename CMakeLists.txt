cmake_minimum_required(VERSION 3.0)
project(UWASM_VM)

# please always set build type
# Options
set(ENABLE_WASM_TO_UWASM ON CACHE BOOL "" FORCE)

# Definitions
set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR ${PROJ_DIR}/src)
set(INC_DIR ${PROJ_DIR}/include)
set(LIBS_DIR ${PROJ_DIR}/libs)
set(TEST_DIR ${PROJ_DIR}/tests)
set(COM_INC_DIR ${SRC_DIR}/utilities ${SRC_DIR}/trace ${SRC_DIR}/encoder)
set(LIBFFI_DIR ${LIBS_DIR}/libffi CACHE STRING "The folder of libffi install dir")

# Set by-default cxx flags when user does not specify whether it's release or debug.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Disabled by-default
option(ENABLE_WASM_RUNTIME "Whether to enable wasm-targeted runtime(register index is different)" OFF)
if(ENABLE_WASM_RUNTIME)
set(WASM_COMPAT_FLAG "-DUWASM_VM_RUNTIME_WASM")
endif()

option(ENABLE_MEM_CHECK "Whether to enable memory OOB check at running" OFF)
if(ENABLE_MEM_CHECK)
  set(WASM_COMPAT_FLAG "${WASM_COMPAT_FLAG} -DUWASM_MEM_CHECK")
endif()


option(ENABLE_PERF "Whether to enable performance and debugging tools, slows the VM down." OFF)
if(ENABLE_PERF)
  set(WASM_COMPAT_FLAG "${WASM_COMPAT_FLAG} -DUWASM_VM_RELEASE_PERF")
endif()


# please modify debug and release option, don't change the c and cxx flags directly
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g3 ${WASM_COMPAT_FLAG}")
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -g3 ${WASM_COMPAT_FLAG}")

set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Ofast -DUWASM_VM_RELEASE -g ${WASM_COMPAT_FLAG}")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -Ofast -DUWASM_VM_RELEASE -g ${WASM_COMPAT_FLAG}")

if (DEFINED BUILD_TARGET)
  include(cmake/${BUILD_TARGET}.cmake)
endif()


# set src and include as project include directory
include_directories(${SRC_DIR} ${INC_DIR})

file(GLOB INTERPRETER_SRC ${SRC_DIR}/interpreter/*.cxx)
file(GLOB TRACE_SRC ${SRC_DIR}/trace/*.cxx)
file(GLOB RUNTIME_SRC ${SRC_DIR}/runtime/*.cxx)
file(GLOB UTILITIES_SRC ${SRC_DIR}/utilities/*.cxx)
file(GLOB ENCODER_SRC ${SRC_DIR}/encoder/*.cxx)
file(GLOB UBH_SRC ${SRC_DIR}/ubh/*.cxx)
file(GLOB STATIC_LINKER_SRC ${SRC_DIR}/static_linker/*.cxx)

add_library(uvm_encoder STATIC ${ENCODER_SRC} ${TRACE_SRC} ${UBH_SRC} ${UTILITIES_SRC})
set_property(TARGET uvm_encoder PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(uvm_common STATIC ${INTERPRETER_SRC} ${RUNTIME_SRC} ${TRACE_SRC} ${UTILITIES_SRC} ${UBH_SRC})

# Standalone UVM Execution
add_executable(uvm ${SRC_DIR}/main.cxx)
target_link_libraries(uvm uvm_common ${LIBFFI_DIR}/lib/libffi.a dl pthread)
target_include_directories(uvm_common PRIVATE ${LIBFFI_DIR}/include)
target_include_directories(uvm PRIVATE ${LIBFFI_DIR}/include)

set(LIBELFIO_DIR ${LIBS_DIR}/elfio)
add_executable(u_sld ${STATIC_LINKER_SRC})
target_include_directories(u_sld PRIVATE ${LIBELFIO_DIR}/include)
target_link_libraries(u_sld uvm_common)

# WASM 2 UWASM
if (ENABLE_WASM_TO_UWASM)
set(WASM_WABT_DIR ${SRC_DIR}/thirdparty/wasm)
aux_source_directory(${SRC_DIR}/wasm WASM_CONVERTER_SRCS)
aux_source_directory(${WASM_WABT_DIR}/src WASM_WABT_SRCS) # There are .c files.
add_executable(wasm_to_uwasm
               ${WASM_CONVERTER_SRCS}
               ${WASM_WABT_SRCS})

target_include_directories(wasm_to_uwasm PUBLIC
        ${SRC_DIR}
        ${COM_INC_DIR}
        ${SRC_DIR}/wasm
        ${WASM_WABT_DIR}
        ${WASM_WABT_DIR}/src)

target_compile_options(wasm_to_uwasm PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)
set(UWASM_VM_TARGETS ${UWASM_VM_TARGETS} wasm_to_uwasm)
set(UWASM_VM_EXEC_TARGETS ${UWASM_VM_EXEC_TARGETS} wasm_to_uwasm)
target_link_libraries(wasm_to_uwasm PRIVATE uvm_encoder)
endif()

# Install allow install using uvm.
install(TARGETS uvm DESTINATION bin)
install(TARGETS uvm_encoder DESTINATION lib)
install(TARGETS u_sld DESTINATION bin)
install(DIRECTORY include DESTINATION .)

file(GLOB UNIT_SRC ${TEST_DIR}/unit/*.cxx)
set(GTEST_DIR ${LIBS_DIR}/gtest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(all_tests ${UNIT_SRC})
target_include_directories(all_tests PRIVATE ${GTEST_DIR}/include)
target_link_libraries(all_tests uvm_encoder uvm_common
    ${GTEST_DIR}/lib/libgtest_main.a ${GTEST_DIR}/lib/libgtest.a pthread ${LIBFFI_DIR}/lib/libffi.a dl)
add_test(NAME all_tests COMMAND all_tests)
