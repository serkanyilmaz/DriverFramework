############################################################################
# Cross-compilation Toolchain File (CMAKE_TOOLCHAIN_FILE)
# for Raspberry Pi 2
#     requires a proper toolchain setup:
#     https://github.com/pixhawk/rp_toolchain
#
# Author: Bo Liu (bo-rc@acm.org)
#
############################################################################
add_definitions(
	-D__DF_OPI
	-D__DF_LINUX
)

######### test DriverFramework for opi ###
# used for debug
#add_definitions(-DDF_DEBUG)

if ("${OPI_TOOLCHAIN_DIR}" STREQUAL "")
	set(OPI_TOOLCHAIN_DIR /home/vagrant/armbian/cache/toolchains)
endif()

# this one is important
set(CMAKE_SYSTEM_NAME Linux)

# this one not so much
set(CMAKE_SYSTEM_VERSION 1)

# set cross compilers
set(CMAKE_C_COMPILER "${OPI_TOOLCHAIN_DIR}/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${OPI_TOOLCHAIN_DIR}/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++")
set(CMAKE_C_FLAGS "")
set(LINKER_FLAGS "-Wl,-gc-sections")
set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
