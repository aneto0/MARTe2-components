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

OBJSX=EPICSPVAChannelWrapper.x EPICSPVAInput.x EPICSPVAOutput.x

PACKAGE=Components/DataSources

ROOT_DIR=../../../../

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
INCLUDES += -I$(EPICS_BASE)/include/
INCLUDES += -I$(EPICS_BASE)/include/os/Linux/
INCLUDES += -I$(EPICS_BASE)/include/compiler/gcc/
INCLUDES += ../../Interfaces/EPICSPVA/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs

OPTIM = -O2
LIBRARIES += -L$(EPICS_BASE)/lib/$(EPICS_HOST_ARCH)/ -lpvAccess -lpvData -lpvDatabase -lnt -lpvAccessIOC -lpvAccessCA -ldbRecStd -ldbCore -lca -lCom -lpvaClient

#Must be named EPICSPVADataSource as otherwise will clash with EPICSPVA.so from the Interfaces!
all: $(OBJS)    \
    $(BUILD_DIR)/EPICSPVADataSource$(LIBEXT) \
    $(BUILD_DIR)/EPICSPVADataSource$(DLLEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

