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
OBJSX=  UEIDevice.x 			\
		UEIMasterObject.x 		\
		UEIMapContainer.x 		\
		UEICircularBuffer.x		\
		UEIBufferPointer.x		\
		UEIRtDMap.x				\
		UEIRtVMap.x				\
		UEIAI217_803.x 			

PACKAGE=Components/Interfaces

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

ifdef UEIDAQ_MOCKUP
INCLUDES += -I ../../../../Test/Components/Interfaces/UEIDAQ/UEIDAQMockup
$(info Compiling UEIDAQ interface with mockup enabled)
else
INCLUDES += -I $(PDNA_INCLUDE)
ifeq ($(TARGET),ppc-linux)
$(info Compiling UEIDAQ interface for TARGET ppc architecture)
LIBRARIES_STATIC += $(PDNA_LIB_PPC)/libpowerdna$(LIBEXT)
LIBRARIES_STATIC += $(PDNA_LIB_PPC)/libUeiPal$(LIBEXT)
endif
ifeq ($(TARGET),x86-linux)
$(info Compiling UEIDAQ interface for TARGET x86 architecture)
LIBRARIES_STATIC += $(PDNA_LIB)/libpowerdna$(LIBEXT)
LIBRARIES_STATIC += $(PDNA_LIB)/libUeiPal$(LIBEXT)
endif
endif

all: $(OBJS)    \
	$(BUILD_DIR)/UEIInterface$(LIBEXT) 		\
	$(BUILD_DIR)/UEIInterface$(DLLEXT) 
	echo  $(OBJS)


include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

