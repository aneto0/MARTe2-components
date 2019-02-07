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

SPB=IOGAM.x\
	ConstantGAM.x\
	ConversionGAM.x\
	CRCGAM.x\
	FilterGAM.x\
	HistogramGAM.x\
	Interleaved2FlatGAM.x\
	MuxGAM.x\
	PIDGAM.x\
	SSMGAM.x\
	StatisticsGAM.x\
	TimeCorrectionGAM.x\
	TriggerOnChangeGAM.x\
	WaveformGAM.x

ifdef $(EFDA_MARTe_DIR)
SPB+=BaseLib2GAM.x
endif

PACKAGE=Components
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)


LIBRARIES_STATIC=$(BUILD_DIR)/IOGAM/IOGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/ConstantGAM/ConstantGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/ConversionGAM/ConversionGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/CRCGAM/CRCGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/FilterGAM/FilterGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/HistogramGAM/HistogramGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/Interleaved2FlatGAM/Interleaved2FlatGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/MuxGAM/MuxGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/PIDGAM/PIDGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/SSMGAM/SSMGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/StatisticsGAM/StatisticsGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/TimeCorrectionGAM/TimeCorrectionGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/TriggerOnChangeGAM/TriggerOnChangeGAM$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/WaveformGAM/WaveformGAM$(LIBEXT)

ifdef $(EFDA_MARTe_DIR)
LIBRARIES_STATIC+=$(BUILD_DIR)/BaseLib2GAM/BaseLib2GAM$(LIBEXT)
endif

all: $(OBJS)  $(SUBPROJ)  \
    $(BUILD_DIR)/GAMs$(LIBEXT) \
    $(BUILD_DIR)/GAMs$(DLLEXT)
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)


