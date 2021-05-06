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

OBJSX +=  CreateNI9157DeviceOperatorTest.x \
    CreateNI9157DeviceOperatorGTest.x \
    CreateNI9157DeviceOperatorITest.x \
    CreateNI9157DeviceOperatorIGTest.x \
    NI9157DeviceTest.x \
    NI9157DeviceGTest.x \
    NI9157DeviceOperatorDatabaseTest.x \
    NI9157DeviceOperatorDatabaseGTest.x \
    NI9157DeviceOperatorTTest.x \
    NI9157DeviceOperatorTGTest.x \
    NI9157DeviceOperatorTITest.x \
    NI9157DeviceOperatorTIGTest.x

PACKAGE=Components/Interfaces
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

ARCHIVEPATH = TestLvFw
ARCHIVE = TestLvFw.tar.gz
FILE_PREIX = NiFpga_NI915

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Logger
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4StateMachine
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4LoggerService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(CODAC_ROOT)/include
INCLUDES += -I../../../../Source/Components/Interfaces/NI9157Device

all: $(OBJS) \
	$(BUILD_DIR)/NI9157DeviceTest$(LIBEXT)
	echo  $(OBJS)
	-tar -xzvf $(ARCHIVEPATH)/$(ARCHIVE) -C $(ARCHIVEPATH)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

clean::
	-@rm -fv $(ARCHIVEPATH)/$(FILE_PREIX)*
