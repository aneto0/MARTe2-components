#!/bin/sh


unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BIN_LIST=none

export M2_RPM_LIB_LIST='libFileDataSource.so \
libLinkDataSource.so \
libLinuxTimer.so \
libLoggerDataSource.so \
libRealTimeThreadSynchronisation.so \
libRealTimeThreadAsyncBridge.so \
libConstantGAM.so \
libConversionGAM.so \
libCRCGAM.so \
libDoubleHandshakeGAM.so \
libFilterGAM.so \
libHistogramGAM.so \
libInterleaved2FlatGAM.so \
libIOGAM.so \
libMuxGAM.so \
libMathExpressionGAM.so \
libPIDGAM.so \
libSSMGAM.so \
libStatisticsGAM.so \
libTimeCorrectionGAM.so \
libWaveform.so \
libMessageGAM.so \
libMemoryGate.so \
libSysLogger.so'

export M2_RPM_NAME=MARTe2_Components
export M2_RPM_ID=marte2-components
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES=marte2-core
export M2_RPM_TOP_DIR='/opt/codac/marte2'

export FOLDERSX='Source.x Makefile.x86-linux.x Makefile.inc.x'
echo "begin"
make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild


# DAN ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/DataSources/DAN.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libDAN.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_DAN
export M2_RPM_ID=marte2-components-dan
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)


export FOLDERSX='Source/Components/DataSources/DAN.x Makefile.x86-linux.x Makefile.inc.x'
echo "begin"
#make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild


# EPICS ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/DataSources/EPICSCA.x Source/Components/DataSources/EPICSPVA.x Source/Components/Interfaces/EPICS.x Source/Components/Interfaces/EPICSPVA.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libEPICSCA.so libEPICSPVADataSource.so libEPICS.so libEPICSPVA.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_EPICS
export M2_RPM_ID=marte2-components-epics
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source/Components/DataSources/EPICSCA.x Source/Components/DataSources/EPICSPVA.x Source/Components/Interfaces/EPICS.x Source/Components/Interfaces/EPICSPVA.x Makefile.x86-linux.x Makefile.inc.x'

echo "begin"
#make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild


# MATLAB ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/GAMs/SimulinkWrapperGAM.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libSimulinkWrapperGAM.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_MATLAB
export M2_RPM_ID=marte2-components-matlab
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source/Components/GAMs/SimulinkWrapperGAM.x Makefile.x86-linux.x Makefile.inc.x'

echo "begin"

#not possible to compile

#make -f Makefile.rpmbuild clean
#make -f Makefile.rpmbuild


# OPC-UA ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/DataSources/OPCUADataSource.x Source/Components/Interfaces/OPCUA.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libOPCUADataSource.so libOPCUA.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_OPCUA
export M2_RPM_ID=marte2-components-opcua
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source/Components/DataSources/OPCUADataSource.x Source/Components/Interfaces/OPCUA.x Makefile.x86-linux.x Makefile.inc.x'

echo "begin"
#make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild


# PXI ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/DataSources/NI1588.x Source/Components/DataSources/NI6259.x Source/Components/DataSources/NI6368.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libNI1588.so libNI6259.so libNI6368.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_PXI
export M2_RPM_ID=marte2-components-pxi
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source/Components/DataSources/NI1588.x Source/Components/DataSources/NI6259.x Source/Components/DataSources/NI6368.x Makefile.x86-linux.x Makefile.inc.x'

echo "begin"
#make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild



# RIO ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/DataSources/NI9157.x Source/Components/Interfaces/NI9157Device.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libNI9157.so libNI9157Device.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_RIO
export M2_RPM_ID=marte2-components-rio
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source/Components/DataSources/NI9157.x Source/Components/Interfaces/NI9157Device.x Makefile.x86-linux.x Makefile.inc.x'

echo "begin"
#make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild


# SDN ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BUILD_EXTRA_ARGS="SPBM='Source/Components/DataSources/SDN.x' SPBMT="
export M2_RPM_BIN_LIST=none
export M2_RPM_LIB_LIST='libSDN.so'
export M2_RPM_TOP_DIR=\/opt\/marte-components
export M2_RPM_NAME=MARTe2_Components_SDN
export M2_RPM_ID=marte2-components-sdn
export M2_RPM_BUILD_OPTIONS=core
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source/Components/DataSources/SDN.x Makefile.x86-linux.x Makefile.inc.x'
echo "begin"
#make -f Makefile.rpmbuild clean
make -f Makefile.rpmbuild



# TEST ---------------------------------------------------------------------------------------------------------------------------------

unset M2_RPM_BUILD_EXTRA_ARGS
unset M2_RPM_BIN_LIST
unset M2_RPM_LIB_LIST
unset M2_RPM_NAME
unset M2_RPM_ID
unset M2_RPM_BUILD_OPTIONS
unset M2_RPM_REQUIRES
unset FOLDERSX
unset SPBM
unset SPBT

export M2_RPM_BIN_LIST=MainGTest.ex
export M2_RPM_LIB_LIST=none
export M2_RPM_NAME=MARTe2_Components_TEST
export M2_RPM_ID=marte2-components-test
export M2_RPM_BUILD_OPTIONS=all
export M2_RPM_REQUIRES='marte2-core marte2-components'
export M2_RPM_TOP_DIR='/opt/codac/marte2'
export M2_RPM_CUSTOM_DIST=ccs$(codac-version)

export FOLDERSX='Source.x Test.x Makefile.x86-linux.x Makefile.inc.x'
#echo "begin"
#make -f Makefile.rpmbuild clean
#make -f Makefile.rpmbuild


# INSTALL ---------------------------------------------------------------------------------------------------------------------------------


if [ $1 == "install" ]
then
  cd Build/rpmbuild/RPMS/x86_64/
  sudo yum localinstall *
fi


