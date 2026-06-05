# Deploying the open62541 library

The OPCUA DataSource relies on the [open62541 library](https://open62541.org/).

The DataSource Makefiles assume static linking against the amalgamated version of the open62541.

## Instructions

These instructions also apply to version 1.3. of the library. Filenames and paths need to be updated accordingly.

### Amalgamation (deprecated)

Older versions of the MARTe2 OPC UA DataSources required building open62541 with amalgamation enabled. This approach is now deprecated, and users should follow the instructions below for static or dynamic linking instead.

If you still need to use amalgamation, follow the instructions below:


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

### Static linking

When building with static linking, the FLTO option must be disabled (`CMAKE_INTERPROCEDURAL_OPTIMIZATION`) to allow the MARTe2 gtest to compile correctly.

1. `mkdir build`
2. `cd build`
3. `cmake3 .. -DCMAKE_INSTALL_PREFIX=/opt/open62541/open62541-1.3.17-build -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=OFF`
4. `make`

### Dynamic linking

1. `mkdir build`
2. `cd build`
3. `cmake3 .. -DCMAKE_INSTALL_PREFIX=/opt/open62541/open62541-1.3.17-build -DBUILD_SHARED_LIBS=ON`
4. `make`

## Export environment variables

Make sure that the following environment variables are exported before compiling (and using the DataSources):
```
export OPEN62541_DIR=/opt/open62541-1.0
export OPEN62541_INCLUDE=$OPEN62541_DIR/build
export OPEN62541_LIB=$OPEN62541_DIR/build/bin
```

If amalgamation is disabled:
```
export OPEN62541_INCLUDE=$OPEN62541_DIR/include
export OPEN62541_LIB=$OPEN62541_DIR/lib64
export OPEN62541_NO_ALMAGAMATION=1
```

If dynamic linking is enabled:
```
export OPEN62541_INCLUDE=$OPEN62541_DIR/include
export OPEN62541_LIB=$OPEN62541_DIR/lib64
export OPEN62541_NO_ALMAGAMATION=1
export OPEN62541_DYNAMIC_LINK=1
```

