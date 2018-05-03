# MARTe2-components

The MARTe2-components repository offers a set of components that are deemed useful to the community. 

These include general purpose GAMs, support to off-the-shelf hardware and interfaces to known supervisory control and data acquisition systems.

The current components are currently available:

## GAMs

| Component | Documentation |
| --------- | ------------- |
| [BaseLib2GAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/BaseLib2GAM) | [Encapsulate and execute GAMs from BaseLib2 in MARTe2](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1BaseLib2GAM.html)|
| [ConversionGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/ConversionGAM) | [GAM which allows to convert between different signal types](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1ConversionGAM.html)|
| [FilterGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/FilterGAM) | [GAM which allows to implement FIR & IIR filter with float32 type](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1FilterGAM.html)|
| [IOGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/IOGAM) | [GAM which copies its inputs to its outputs. Allows to plug different DataSources (e.g. driver with a DDB).](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1IOGAM.html)|
| [MuxGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/MuxGAM) | [Multiplexer GAM that allows multiplex different signals.](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1MuxGAM.html)|
| [PIDGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/PIDGAM) | [A generic PID with saturation and anti-windup.](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1PIDGAM.html)|
| [SSMGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/SSMGAM) | [A generic State Space model with constant matrices and float64.](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1SSMGAM.html)|
| [StatisticsGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/StatisticsGAM) | [GAM which provides average, standard deviation, minimum and maximum of its input signal over a moving time window.](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1StatisticsGAM.html)|
| [WaveformGAM](https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components/tree/master/Source/Components/GAMs/WaveformGAM) | [GAM which provides average, standard deviation, minimum and maximum of its input signal over a moving time window.](https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1WaveformGAM.html)|

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

# License

Copyright 2015 F4E | European Joint Undertaking for ITER and the Development of Fusion Energy ('Fusion for Energy').
Licensed under the EUPL, Version 1.1 or - as soon they will be approved by the European Commission - subsequent versions of the EUPL (the "Licence"). You may not use this work except in compliance with the Licence. You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl

Unless required by applicable law or agreed to in writing, software distributed under the Licence is distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the Licence permissions and limitations under the Licence.
