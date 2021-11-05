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

OBJSX=DANAPI.x DANSource.x DANStream.x3

PACKAGE=Components/DataSources
ROOT_DIR=../../../../
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs
INCLUDES += -I/usr/include/libxml2

ifdef CODAC_ROOT
INCLUDES += -I$(CODAC_ROOT)/include
LIBRARIES += -L$(CODAC_ROOT)/lib -lccs-core -ldan_api -ldan_client_api -ldan_stream -ltcn -llog -lxml2
# Override CPPFLAGS due to scoped enums. Allow narrowing. TODO file bug report.
#CPPFLAGS += -std=c++0x -Wno-error=narrowing

## Compatibility issue with dan_initLibrary_icprog pre CCSv6.0
CCS_LT_60 = $(shell if [ `codac-version -v | sed 's/\(.*\)\.\(.*\)/\1\2/g'` -lt 60 ]; then echo 1; else echo 0; fi)
ifeq "$(CCS_LT_60)" "1"
CPPFLAGS += -DCCS_LT_60
endif

endif

all: $(OBJS)    \
    $(BUILD_DIR)/DAN$(LIBEXT) \
    $(BUILD_DIR)/DAN$(DLLEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

