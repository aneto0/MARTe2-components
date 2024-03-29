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

SPB=  IOGAM.x\
    ConstantGAM.x\
    ConversionGAM.x\
    CRCGAM.x\
    DoubleHandshakeGAM.x\
    FilterGAM.x\
    FlattenedStructIOGAM.x\
    HistogramGAM.x\
    Interleaved2FlatGAM.x\
    MathExpressionGAM.x\
    MessageGAM.x\
    MuxGAM.x\
    PIDGAM.x\
    SSMGAM.x\
    StatisticsGAM.x\
    TimeCorrectionGAM.x\
    TriggeredIOGAM.x\
    WaveformGAM.x

ifdef EFDA_MARTe_DIR
SPB += BaseLib2GAM.x
endif

ifdef MATLAB_DIR
SPB += SimulinkWrapperGAM.x
endif

PACKAGE=Components
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

all: $(OBJS) $(SUBPROJ) 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
