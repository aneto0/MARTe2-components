#******************************************************************************
#
#      $Log$
#      Revision 1.1  2011/10/16 16:33:20  aneto
#      First working version. The header is automatically populated by the producer.
#
#
#******************************************************************************

OBJSX=UDPSender.x

PACKAGE=Components/DataSources
ROOT_DIR=../../../../
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

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
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L3Streams


all: $(OBJS) $(SUBPROJ) \
    $(BUILD_DIR)/UDP$(LIBEXT) \
    $(BUILD_DIR)/UDP$(DLLEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
