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
#Subprojects w.r.t. the main directory only (important to allow setting SPBM as an export variable).
#If SPB is directly exported as an environment variable it will also be evaluated as part of the subprojects SPB, thus
#potentially overriding its value
#Main target subprojects. May be overridden by shell definition.
SPBM?=Source/Components/DataSources.x \
	Source/Components/GAMs.x \
	Source/Components/Interfaces.x

#Testing of the main target subprojects. May be overridden by shell definition.
SPBMT?=Test/Components/DataSources.x \
    Test/Components/GAMs.x \
    Test/Components/Interfaces.x \
	Test/GTest.x

#This really has to be defined locally.
SUBPROJMAIN=$(SPBM:%.x=%.spb)
SUBPROJMAINTEST=$(SPBMT:%.x=%.spb)
SUBPROJMAINCLEAN=$(SPBM:%.x=%.spc)
SUBPROJMAINTESTCLEAN=$(SPBMT:%.x=%.spc)

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

ROOT_DIR=.
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

all: $(OBJS) core test
	echo  $(OBJS)

core: $(SUBPROJMAIN) check-env
	echo  $(SUBPROJMAIN)

test: $(SUBPROJMAINTEST) check-env
	echo  $(SUBPROJMAINTEST)

clean:: $(SUBPROJMAINCLEAN) $(SUBPROJMAINTESTCLEAN) clean_wipe_old

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif

