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
#############################################################

PACKAGE=
ROOT_DIR=../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(ROOT_DIR)/Source/Components/DataSources/ProfinetDataSource/Makefile.detect

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams

# Detect project related files
# Compilation of SDNPublisher DataSource is contingent to availability of SDN core library, etc.
# Current assumption is prior installation of CODAC Core System on the build node
ifdef CODAC_ROOT
## DAN client API dependencies
LIBRARIES += -L$(CODAC_ROOT)/lib -lccs-core -ldan_api -ldan_client_api -ldan_stream -ltcn -llog -lxml2
## DAN HDF5 reader dependencies (test code)
LIBRARIES += -ldan_base -ldan_archive -ldan_wrapper_srv -ldan_uda_reader_base -ldan_stream_reader_cpp
CCS_LT_63 = $(shell if [ `codac-version -v | sed 's/\(.*\)\.\(.*\)/\1\2/g'` -le 62 ]; then echo 1; else echo 0; fi)
ifeq "$(CCS_LT_63)" "0"
LIBRARIES += -ldan_data_srv
endif
endif

ifdef SDN_CORE_LIBRARY_DIR
LIBRARIES += -L$(SDN_CORE_LIBRARY_DIR) -lsdn-core -lxml2
endif

ifdef PROFINET_COMPILE
LIBRARIES += -L$(PROFINET_LIBRARY) -losal -lprofinet
endif

ifdef CODAC_ROOT
LIBRARIES += -L$(CODAC_ROOT)/lib/
LIBRARIES += -lnisync
LIBRARIES += -lnixseries
LIBRARIES += -lpxi6259
LIBRARIES += -lNiFpga

## Only force EPICSPVA if CCS version is gt v6.1
CCSGT61 = $(shell if [ `codac-version -v | sed 's/\(.*\)\.\(.*\)/\1\2/g'` -ge 61 ]; then echo 1; else echo 0; fi)
ifeq "$(CCSGT61)" "1"
EPICSPVA = "1"
endif
endif

ifdef EFDA_MARTe_DIR
LIBRARIES   += -L$(EFDA_MARTe_DIR)/Interfaces/BaseLib2Adapter/linux -lBaseLib2Adapter -L$(EFDA_MARTe_DIR)/BaseLib2/linux -lBaseLib2
endif

ifdef MDSPLUS_DIR
LIBRARIES += -L$(MDSPLUS_DIR)/lib -lMdsObjectsCppShr
endif

ifdef EPICS_BASE
LIBRARIES += -L$(EPICS_BASE)/lib/$(EPICS_HOST_ARCH)/ -lca
endif

ifdef EPICSPVA
LIBRARIES += -lpvAccess -lpvData -lpvDatabase -lnt -lpvAccessIOC -lpvAccessCA -ldbRecStd -ldbCore -lca -lCom -lpvaClient 
endif 

ifdef MATLAB_DIR
LIBRARIES += -Wl,-rpath-link,$(MATLAB_DIR)/bin/glnxa64 "$(MATLAB_DIR)/sys/os/glnxa64/libstdc++.so.6" -L$(MATLAB_DIR)/extern/bin/glnxa64/ -L$(MATLAB_DIR)/bin/glnxa64/ -lMatlabDataArray -lMatlabEngine -lmx -lmex -lmat -lm -lstdc++ -leng
endif

all: $(BUILD_DIR)/MainGTest$(EXEEXT)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

