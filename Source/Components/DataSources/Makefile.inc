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

OBJSX= 

SPB    = LinuxTimer.x LoggerDataSource.x
ifdef CODAC_ROOT
SPB    += NI6259.x
SPB    += NI6368.x
endif

PACKAGE=Components
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES_STATIC=$(BUILD_DIR)/LinuxTimer/LinuxTimer$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/LoggerDataSource/LoggerDataSource$(LIBEXT)

ifdef CODAC_ROOT
LIBRARIES_STATIC+=$(BUILD_DIR)/NI6259/NI6259$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/NI6368/NI6368$(LIBEXT)
endif

all: $(OBJS)  $(SUBPROJ)  \
    $(BUILD_DIR)/DataSources$(LIBEXT) \
    $(BUILD_DIR)/DataSources$(DLLEXT)
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)


