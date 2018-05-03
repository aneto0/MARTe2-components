# MARTe2-components

The MARTe2-components repository offers a set of components that are deemed useful to the community. 

These include general purpose GAMs, support to off-the-shelf hardware and interfaces to known supervisory control and data acquisition systems.

The current components are currently available:

## GAMs

| Component | Documentation |
| --------- | ------------- |
| [Source-code](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/) | [Encapsulate and execute GAMs from BaseLib2 in MARTe2](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1BaseLib2GAM.html)|

## DataSources

## Interfaces


# Other information

More information about the MARTe software framework can be found in: 

- [User documentation](https://vcis-jenkins.f4e.europa.eu/job/MARTe2/docs)
- [API documentation](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-docs-master/doxygen/annotated.html)
- [API documentation (components)](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-components-docs-master/doxygen/annotated.html)

- [Source-code](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2)
- [Integration](https://vcis-jenkins.f4e.europa.eu/job/MARTe2)


## How to build components and run tests

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
