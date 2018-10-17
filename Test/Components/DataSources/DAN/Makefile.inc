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

OBJSX +=  DANSourceTest.x DANStreamTest.x
		
PACKAGE=Components/DataSources
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

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

INCLUDES += -I/usr/include/libxml2
INCLUDES += -I../../../../Source/Components/DataSources/DAN

ifdef CODAC_ROOT
INCLUDES += -I$(CODAC_ROOT)/include
endif

ifdef CODAC_ROOT
## Compatibility issue with HDF5 post CCSv6.0
HDF5_SET_CHANNEL = $(shell if [ `codac-version -v | sed 's/\(.*\)\.\(.*\)/\1\2/g'` -le 60 ]; then echo 1; else echo 0; fi)
ifeq "$(HDF5_SET_CHANNEL)" "0"
CPPFLAGS += -DNO_HDF5_SET_CHANNEL
endif
## DAN client API dependencies
LIBRARIES += -L$(CODAC_ROOT)/lib -lccs-core -ldan_api -ldan_client_api -ldan_stream -ltcn -llog -lxml2
## DAN HDF5 reader dependencies (test code)
LIBRARIES += -ldan_base -ldan_archive -ldan_wrapper_srv -ldan_uda_reader_base -ldan_stream_reader_cpp
endif

#TODO Temporary to fix in hpn.h
#CPPFLAGS += -Wno-error=narrowing

all: $(OBJS) \
	$(BUILD_DIR)/DANTest$(LIBEXT)
	echo  $(OBJS)


include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

