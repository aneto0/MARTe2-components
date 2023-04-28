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
OBJSX=  UEIDataSourceI.x \
		UEIReader.x		\
		UEIWritter.x

PACKAGE=Components/DataSources

ROOT_DIR=../../../../

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
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
INCLUDES += -I ../../Interfaces/UEIDAQ

ifdef UEIDAQ_MOCKUP
INCLUDES += -I ../../../../Test/Components/Interfaces/UEIDAQ/UEIDAQMockup
$(info Compiling UEIDataSource with mockup enabled)
else
INCLUDES += -I $(PDNA_INCLUDE)
endif

all: $(OBJS)    \
	$(BUILD_DIR)/UEIDataSource$(LIBEXT) \
	$(BUILD_DIR)/UEIDataSource$(DLLEXT)
	echo  $(OBJS)


include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

