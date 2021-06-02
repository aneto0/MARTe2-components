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
OBJSX=SimulinkWrapperGAM.x SimulinkClasses.x

PACKAGE=Components/GAMs

ROOT_DIR=../../../../
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

CPPFLAGS += -O1

# Flags required in order to retrieve type definitions from tmwtypes.h
# instead of rtwtypes.h (which is generated for each model and not available in MATLAB root directory)
CPPFLAGS += -DHOST_CAPI_BUILD -DMATLAB_MEX_FILE
CFLAGS += -DHOST_CAPI_BUILD -DMATLAB_MEX_FILE

# Add row-major support for 3D matrices only if available (from version 2018b onwards)
ROW_MAJOR_ND_FEATURE = $(shell if grep -qr rtwCAPI_MATRIX_ROW_MAJOR_ND $$MATLAB_DIR/rtw/c/src; then echo 0; else echo 1; fi)
ifeq "$(ROW_MAJOR_ND_FEATURE)" "0"
CFLAGS += -DROW_MAJOR_ND_FEATURE
CPPFLAGS += -DROW_MAJOR_ND_FEATURE
endif

# Add enum support only if available (from version 2019a onwards)
ENUM_FEATURE = $(shell if grep -qr rtwCAPI_GetDataEnumStorageType $$MATLAB_DIR/rtw/c/src; then echo 0; else echo 1; fi)
ifeq "$(ENUM_FEATURE)" "0"
CFLAGS += -DENUM_FEATURE
CPPFLAGS += -DENUM_FEATURE
endif

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

INCLUDES += -I$(MATLAB_DIR)/extern/include
INCLUDES += -I$(MATLAB_DIR)/rtw/c/src/
INCLUDES += -I$(MATLAB_DIR)/simulink/include/

# Remember that no linking shall be performed against the matlab libraries, as this would prevent deploying it in an environment where no matlab is installed.


all: $(OBJS) $(SUBPROJ) \
	$(BUILD_DIR)/SimulinkWrapperGAM$(LIBEXT) \
	$(BUILD_DIR)/SimulinkWrapperGAM$(DLLEXT)
	    echo  $(OBJS) $(ENUM_SUPPORT)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

