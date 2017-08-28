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

SPB=  IOGAM.x ConversionGAM.x FilterGAM.x PIDGAM.x StatisticsGAM.x WaveformGAM.x

ifdef EFDA_MARTe_DIR
SPB += BaseLib2GAM.x
endif

PACKAGE=Components
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES_STATIC+=$(BUILD_DIR)/IOGAM/IOGAMTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/ConversionGAM/ConversionGAMTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/FilterGAM/FilterGAMTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/PIDGAM/PIDGAMTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/StatisticsGAM/StatisticsGAMTest$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/WaveformGAM/WaveformGAMTest$(LIBEXT)
ifdef EFDA_MARTe_DIR
LIBRARIES_STATIC+=$(BUILD_DIR)/BaseLib2GAM/BaseLib2GAMTest$(LIBEXT)
endif

all: $(OBJS) $(SUBPROJ) \
                $(BUILD_DIR)/GAMsTests$(LIBEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
