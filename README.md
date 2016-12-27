How to build components and run tests
-------------------------------------
**Commands:**
```
$ export MARTe2_DIR=<marte2_dir>
$ export LD_LIBRARY_PATH=$MARTe2_DIR/Build/linux/Core/
$ make -f Makefile.linux
$ Build/linux/GTest/MainGTest.ex
```	
**Paths:**
+ marte2_dir is the root directory of MARTe2 project's source code.

**Note:** The directory marte2_dir can be a snapshot of the project or a clone of the repository itself, so it can be switched to any branch or commit.
