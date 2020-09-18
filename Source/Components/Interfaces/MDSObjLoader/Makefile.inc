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

OBJSX=ParObjLoader.x \
    ObjParameters.x \
    MDSObjLoader.x \
    MDSObjConnection.x \
    MDSParameters.x \


PACKAGE=Components/Interfaces

ROOT_DIR=../../../../
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

# These flags are required in order to retrieve type definitions from tmwtypes.h
# instead of rtwtypes.h (which is generated for each model and not available in MATLAB root directory)
CPPFLAGS += -DHOST_CAPI_BUILD -DMATLAB_MEX_FILE
CFLAGS += -DHOST_CAPI_BUILD -DMATLAB_MEX_FILE

INCLUDES += -I.
INCLUDES += -I../../GAMs/SimulinkWrapperGAM/include
INCLUDES += -I$(MDSPLUS_DIR)/include/
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

INCLUDES += -I$(MATLAB_DIR)/extern/include
INCLUDES += -I$(MATLAB_DIR)/rtw/c/src/
INCLUDES += -I$(MATLAB_DIR)/simulink/include/

all: $(OBJS)    \
    $(BUILD_DIR)/MDSObjLoader$(LIBEXT) \
    $(BUILD_DIR)/MDSObjLoader$(DLLEXT)
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

