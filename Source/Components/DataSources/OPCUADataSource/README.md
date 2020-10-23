# Deploying the open62541 library

The OPCUA DataSource relies on the [open62541 library](https://open62541.org/).

The DataSource Makefiles assume static linking against the amalgamated version of the open62541.

## Instructions

1. Download version 1.0 of the library [here](https://api.github.com/repos/open62541/open62541/tarball/v1.0)
2. Copy to your projects folder, e.g. /opt: `cp open62541-open62541-v1.0-0-gce5209d.tar.gz /opt/`
3. `cd /opt/`
4. `tar zxf open62541-open62541-v1.0-0-gce5209d.tar.gz`
5. `mv open62541-open62541-ce5209d open62541-1.0`
6. `cd open62541-1.0`
7. Open the `CMakeLists.txt` file and comment the lines marked with # below
```
if("${CMAKE_VERSION}" VERSION_GREATER 3.9)
    cmake_policy(SET CMP0069 NEW) # needed as long as required cmake < 3.9
    #include(CheckIPOSupported)
    #check_ipo_supported(RESULT CC_HAS_IPO) # Inter Procedural Optimization / Link Time Optimization (should be same as -flto)
    #if(CC_HAS_IPO)
    #    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    #endif()
endif()
```
8. Save and close the `CMakeLists.txt` files and compile.
9. `mkdir build`
10. `cd build`
11. `cmake3 -DUA_ENABLE_AMALGAMATION=ON ..`
12. `make`

## Export environment variables

Make sure that the following environment variables are exported before compiling (and using the DataSources):
```
export OPEN62541_DIR=/opt/open62541-1.0
export OPEN62541_INCLUDE=$OPEN62541_DIR/build
export OPEN62541_LIB=$OPEN62541_DIR/build/bin
```



