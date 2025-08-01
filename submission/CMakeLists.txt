cmake_minimum_required(VERSION 3.14)
project(add_openfhe LANGUAGES CXX)

# --------------------------------------------------------------------
# 1.  Compiler / flags
# --------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# add_compile_options(-O3 -Wall -Wextra -Wno-unused-parameter)
option( BUILD_STATIC "Set to ON to include static versions of the library" OFF)

# --------------------------------------------------------------------
# 2.  Find the OpenFHE *package* that the install script put into
#     third_party/openfhe
# --------------------------------------------------------------------
#   scripts/get-openfhe.sh installs:
#     third_party/openfhe/lib/cmake/OpenFHE/OpenFHEConfig.cmake

find_package(OpenFHE CONFIG REQUIRED)
if (OpenFHE_FOUND)
    message(STATUS "FOUND PACKAGE OpenFHE")
    message(STATUS "OpenFHE Version: ${BASE_OPENFHE_VERSION}")
    message(STATUS "OpenFHE installed as shared libraries: ${OpenFHE_SHARED}")
    message(STATUS "OpenFHE include files location: ${OpenFHE_INCLUDE}")
    message(STATUS "OpenFHE lib files location: ${OpenFHE_LIBDIR}")
    message(STATUS "OpenFHE Native Backend size: ${OpenFHE_NATIVE_SIZE}")
else()
    message(FATAL_ERROR "PACKAGE OpenFHE NOT FOUND")
endif ()

set( CMAKE_CXX_FLAGS "${OpenFHE_CXX_FLAGS} -Werror")


# --------------------------------------------------------------------
# 3.  Link libraries
# --------------------------------------------------------------------

include_directories( ${OPENMP_INCLUDES} )
include_directories( ${OpenFHE_INCLUDE} )
include_directories( ${OpenFHE_INCLUDE}/third-party/include )
include_directories( ${OpenFHE_INCLUDE}/core )
include_directories( ${OpenFHE_INCLUDE}/pke )
include_directories( ${OpenFHE_INCLUDE}/binfhe )
### add directories for other OpenFHE modules as needed for your project

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories( ${OpenFHE_LIBDIR} )
link_directories( ${OPENMP_LIBRARIES} )
if(BUILD_STATIC)
    set( CMAKE_EXE_LINKER_FLAGS "${OpenFHE_EXE_LINKER_FLAGS} -static")
    link_libraries( ${OpenFHE_STATIC_LIBRARIES} )
    message(STATUS "Build with static libs")
else()
    set( CMAKE_EXE_LINKER_FLAGS ${OpenFHE_EXE_LINKER_FLAGS} )
    link_libraries( ${OpenFHE_SHARED_LIBRARIES} )
endif()

# --------------------------------------------------------------------
# 4.  Each *.cpp file becomes its own executable.
#     The eight stage names are hard-wired by the benchmark contract.
# --------------------------------------------------------------------
 
add_executable( client_key_generation src/client_key_generation.cpp )

add_executable( client_preprocess_dataset src/client_preprocess_dataset.cpp )

add_executable( client_preprocess_query src/client_preprocess_query.cpp )

add_executable( client_encode_encrypt_db src/client_encode_encrypt_db.cpp )

add_executable( client_encode_encrypt_query src/client_encode_encrypt_query.cpp )

add_executable( client_decrypt_decode src/client_decrypt_decode.cpp )

add_executable( client_postprocess src/client_postprocess.cpp )

add_executable( server_preprocess_dataset src/server_preprocess_dataset.cpp )

add_executable( server_encrypted_compute src/server_encrypted_compute.cpp )
