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

OBJSX +=  ProfinetDataStructureTest.x \
    ProfinetDataSourceAdapterTest.x \
    ProfinetToMARTeLogAdapterTest.x \
	ProfinetDataSourceTest.x
        
PACKAGE=Components/DataSources
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(ROOT_DIR)/Source/Components/DataSources/ProfinetDataSource/Makefile.detect

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I$(MDSPLUS_DIR)/include/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4StateMachine
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L3Streams

INCLUDES += -I$(PROFINET_INCLUDE)

INCLUDES += -I$(ROOT_DIR)/Source/Components/DataSources/ProfinetDataSource
INCLUDES += -I$(ROOT_DIR)/Source/Components/GAMs/IOGAM
INCLUDES += -I$(MARTe2_DIR)/Lib/gtest-1.7.0/include

LIBRARIES +=  -L$(PROFINET_LIBRARY) -losal -lprofinet

all: $(OBJS) \
	$(BUILD_DIR)/ProfinetDataSourceTest$(LIBEXT) 
	echo $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

