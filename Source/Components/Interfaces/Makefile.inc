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

SPB=MemoryGate.x \
	SysLogger.x 

ifdef OPEN62541_LIB
SPB+=OPCUA.x
endif

ifdef CODAC_ROOT
SPB+=NI9157Device.x
endif

ifdef EFDA_MARTe_DIR
SPB+=BaseLib2Wrapper.x
endif

ifdef EPICS_BASE
SPB+=EPICS.x
endif

ifdef MDSPLUS_DIR
SPB+= MDSStructuredDataI.x
endif

ifdef CODAC_ROOT
## Only force EPICSPVA if CCS version is gt v6.1
CCSGT61 = $(shell if [ `codac-version -v | sed 's/\(.*\)\.\(.*\)/\1\2/g'` -ge 61 ]; then echo 1; else echo 0; fi)
ifeq "$(CCSGT61)" "1"
EPICSPVA = "1"
endif
endif

ifdef CODAC_ROOT
SPB+=TCNTimeProvider.x
endif

ifdef EPICSPVA
SPB+=EPICSPVA.x
endif

PACKAGE=Components
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES_STATIC=$(BUILD_DIR)/MemoryGate/MemoryGate$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/SysLogger/SysLogger$(LIBEXT)

ifdef CODAC_ROOT
LIBRARIES_STATIC+=$(BUILD_DIR)/NI9157Device/NI9157Device$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/TCNTimeProvider/TCNTimeProvider$(LIBEXT)
endif

ifdef EFDA_MARTe_DIR
LIBRARIES_STATIC+=$(BUILD_DIR)/BaseLib2Wrapper/BaseLib2Wrapper$(LIBEXT)
endif
ifdef EPICS_BASE
LIBRARIES_STATIC+=$(BUILD_DIR)/EPICS/EPICS$(LIBEXT)
endif
ifdef EPICSPVA
LIBRARIES_STATIC+=$(BUILD_DIR)/EPICSPVA/EPICSPVA$(LIBEXT)
endif
ifdef OPEN62541_LIB
LIBRARIES_STATIC+=$(BUILD_DIR)/OPCUA/OPCUA$(LIBEXT)
endif

ifdef MDSPLUS_DIR
LIBRARIES_STATIC+=$(BUILD_DIR)/MDSStructuredDataI/MDSStructuredDataI$(LIBEXT)
endif


all: $(OBJS)  $(SUBPROJ)  \
    $(BUILD_DIR)/Interfaces$(LIBEXT) \
    $(BUILD_DIR)/Interfaces$(DLLEXT)
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)


