#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
#  and the Development of Fusion Energy ('Fusion for Energy')
# 
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent  
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#  
# http://ec.europa.eu/idabc/eupl
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
# $Id: Makefile.inc 3 2012-01-15 16:26:07Z aneto $
#
#############################################################

SPB    = EpicsDataSource.x \
        FileDataSource.x \
        LinuxTimer.x \
        LoggerDataSource.x \
        RealTimeThreadSynchronisation.x

ifdef CODAC_ROOT
SPB+=NI6259.x NI6368.x DAN.x
endif

ifdef MDSPLUS_DIR
SPB+=MDSWriter.x
SPB+=MDSReader.x
endif

ifdef EPICS_BASE
SPB+=EPICSCA.x
endif

# Compilation of SDNPublisher DataSource is contingent to availability of SDN core library, etc.
# Current assumption is prior installation of CODAC Core System on the build node
ifneq "$(or $(CODAC_VERSION),$(SDN_CORE_LIBRARY_DIR))" ""
SPB   += SDN.x
endif
	
PACKAGE=Components
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES_STATIC=$(BUILD_DIR)/EpicsDataSource/EpicsDataSourceTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/FileDataSource/FileDataSourceTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/LinuxTimer/LinuxTimerTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/LoggerDataSource/LoggerDataSourceTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/RealTimeThreadSynchronisation/RealTimeThreadSynchronisationTest$(LIBEXT)

ifdef CODAC_ROOT
LIBRARIES_STATIC+=$(BUILD_DIR)/DAN/DANTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/NI6259/NI6259Test$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/NI6368/NI6368Test$(LIBEXT)
endif

ifdef MDSPLUS_DIR
LIBRARIES_STATIC+=$(BUILD_DIR)/MDSWriter/MDSWriterTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/MDSReader/MDSReaderTest$(LIBEXT)
endif

# Inclusion of SDNPublisher DataSource is contingent to availability of SDN core library, etc.
ifneq "$(or $(CODAC_VERSION),$(SDN_CORE_LIBRARY_DIR))" ""
LIBRARIES_STATIC+=$(BUILD_DIR)/SDN/SDNTest$(LIBEXT)
endif

ifdef EPICS_BASE
LIBRARIES_STATIC+=$(BUILD_DIR)/EPICSCA/EPICSCATest$(LIBEXT)
endif

all: $(OBJS) $(SUBPROJ) \
    $(BUILD_DIR)/DataSourcesTests$(LIBEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
