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

OBJSX += SDNPublisherTest.x SDNSubscriberTest.x
		
PACKAGE=Components/DataSources
ROOT_DIR=../../../..
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

ifdef CODAC_ROOT
INCLUDES += -I$(CODAC_ROOT)/include
endif

ifdef SDN_CORE_INCLUDE_DIR
INCLUDES += -I$(SDN_CORE_INCLUDE_DIR)
endif

INCLUDES += -I../../../../Source/Components/DataSources/SDN

## SDN core from v1.2 onwards (only check in CCS machines, otherwise codac-version will not exist for sure).
ifdef CODAC_ROOT
FEATURE_10840 = $(shell if [ -z ${SDN_CORE_VERSION} ] && [ `codac-version -f | grep 'PATCHED (10840)' | wc -l` == 0 ]; then echo 0; else echo 1; fi)
ifeq "$(FEATURE_10840)" "0"
# Override CPPFLAGS due to scoped enums
CPPFLAGS += -std=c++0x
else
CPPFLAGS += -DFEATURE_10840 -std=c++0x
endif
else
CPPFLAGS += -std=c++0x
endif

all: $(OBJS) \
    $(BUILD_DIR)/SDNTest$(LIBEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

