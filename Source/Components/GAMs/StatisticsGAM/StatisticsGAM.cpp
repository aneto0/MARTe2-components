/**
 * @file StatisticsGAM.cpp
 * @brief Source file for class StatisticsGAM
 * @date 08/03/2017
 * @author Bertrand Bauvir
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class StatisticsGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "StatisticsHelperT.h"
#include "StatisticsGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#ifndef UNIT_VERSION_HR
#define UNIT_VERSION_HR "1.0"
#endif

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StatisticsGAM::StatisticsGAM() :
        GAM() {

    signalType = InvalidType;
    stats = NULL_PTR(void *);
    windowSize = 1024u;
}

StatisticsGAM::~StatisticsGAM() {

   bool ok = (stats != NULL_PTR(void *));

    /* Delete StatisticsHelperT class */

    if (ok) {

        if (signalType == SignedInteger8Bit) {
	    StatisticsHelperT<int8> * ref = (StatisticsHelperT<int8> *) stats;
	    delete ref;
	}

	if (signalType == SignedInteger16Bit) {
	    StatisticsHelperT<int16> * ref = (StatisticsHelperT<int16> *) stats;
	    delete ref;
	}

	if (signalType == SignedInteger32Bit) {
	    StatisticsHelperT<int32> * ref = (StatisticsHelperT<int32> *) stats;
	    delete ref;
	}

	if (signalType == SignedInteger64Bit) {
	    StatisticsHelperT<int64> * ref = (StatisticsHelperT<int64> *) stats;
	    delete ref;
	}

	if (signalType == UnsignedInteger8Bit) {
	    StatisticsHelperT<uint8> * ref = (StatisticsHelperT<uint8> *) stats;
	    delete ref;
	}

	if (signalType == UnsignedInteger16Bit) {
	    StatisticsHelperT<uint16> * ref = (StatisticsHelperT<uint16> *) stats;
	    delete ref;
	}

	if (signalType == UnsignedInteger32Bit) {
	    StatisticsHelperT<uint32> * ref = (StatisticsHelperT<uint32> *) stats;
	    delete ref;
	}

	if (signalType == UnsignedInteger64Bit) {
	    StatisticsHelperT<uint64> * ref = (StatisticsHelperT<uint64> *) stats;
	    delete ref;
	}

	if (signalType == Float32Bit) {
	    StatisticsHelperT<float32> * ref = (StatisticsHelperT<float32> *) stats;
	    delete ref;
	}

	if (signalType == Float64Bit) {
	    StatisticsHelperT<float64> * ref = (StatisticsHelperT<float64> *) stats;
	    delete ref;
	}

    }

}

bool StatisticsGAM::Initialise(StructuredDataI & data) {

    bool ret = true;

    /* Rerieve window size attribute */

    if (ret) {
        ret = data.Read("WindowSize", windowSize);
    }

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to retrieve WindowSize parameter");
    } else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Retrieve '%u' WindowSize parameter", windowSize);
    }

    return ret;
}

bool StatisticsGAM::Setup() {

    bool ret = ((GetNumberOfInputSignals() != 0u) && (GetNumberOfOutputSignals() != 0u));

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "(GetNumberOfInputSignals() == 0u) || (GetNumberOfOutputSignals() == 0u)");
    }

    if (ret) {
      ret = (GetSignalType(InputSignals, 0u) == GetSignalType(OutputSignals, 0u));
    }

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalType(InputSignals, 0u) != GetSignalType(OutputSignals, 0u)");
    }

    if (ret) {
      signalType = GetSignalType(InputSignals, 0u);
    }

    uint32 signalNumberOfDimensions = 0u;

    if (ret) {
        ret = GetSignalNumberOfDimensions(InputSignals, 0u, signalNumberOfDimensions);
    }

    if (ret) {
        ret = (signalNumberOfDimensions == 0u);
    }  

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfDimensions(InputSignals, 0u) != 0u");
    }

    uint32 signalNumberOfElements = 0u;

    if (ret) {
        ret = GetSignalNumberOfElements(InputSignals, 0u, signalNumberOfElements);
    }

    if (ret) {
        ret = (signalNumberOfElements == 1u);
    }  

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements(InputSignals, 0u) != 1u");
    }

    uint32 signalIndex;

    for (signalIndex = 0u; (signalIndex < GetNumberOfOutputSignals()) && (ret); signalIndex++) {

	if (ret) {
	    ret = (signalType == GetSignalType(OutputSignals, signalIndex));
	}

	if (!ret) {
	  REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GetSignalType(OutputSignals, %u) != signalType", signalIndex);
	}

	if (ret) {
	    ret = GetSignalNumberOfDimensions(OutputSignals, signalIndex, signalNumberOfDimensions);
	}

	if (ret) {
            ret = (signalNumberOfDimensions == 0u);
	}  

	if (!ret) {
	    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GetSignalNumberOfDimensions(OutputSignals, %u) != 0u", signalIndex);
	}

	if (ret) {
            ret = GetSignalNumberOfElements(OutputSignals, signalIndex, signalNumberOfElements);
	}

	if (ret) {
	    ret = (signalNumberOfElements == 1u);
	}  

	if (!ret) {
	    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GetSignalNumberOfElements(OutputSignals, %u) != 1u", signalIndex);
	}

    }

    /* Instantiate Statistics class */

    if (ret) {

        if (signalType == SignedInteger8Bit) {
	    stats = (void *) new StatisticsHelperT<int8>(windowSize);
	}

	if (signalType == SignedInteger16Bit) {
	    stats = (void *) new StatisticsHelperT<int16>(windowSize);
	}

	if (signalType == SignedInteger32Bit) {
	    stats = (void *) new StatisticsHelperT<int32>(windowSize);
	}

	if (signalType == SignedInteger64Bit) {
	    stats = (void *) new StatisticsHelperT<int64>(windowSize);
	}

	if (signalType == UnsignedInteger8Bit) {
	    stats = (void *) new StatisticsHelperT<uint8>(windowSize);
	}

	if (signalType == UnsignedInteger16Bit) {
	    stats = (void *) new StatisticsHelperT<uint16>(windowSize);
	}

	if (signalType == UnsignedInteger32Bit) {
	    stats = (void *) new StatisticsHelperT<uint32>(windowSize);
	}

	if (signalType == UnsignedInteger64Bit) {
	    stats = (void *) new StatisticsHelperT<uint64>(windowSize);
	}

	if (signalType == Float32Bit) {
	    stats = (void *) new StatisticsHelperT<float32>(windowSize);
	}

	if (signalType == Float64Bit) {
	    stats = (void *) new StatisticsHelperT<float64>(windowSize);
	}

	ret = (stats != NULL_PTR(void *));

    }

    return ret;
}

bool StatisticsGAM::Execute() {

    bool ret = true;

    if (signalType == SignedInteger8Bit) {
        ret = this->StatisticsGAM::Execute<int8>();
    }

    if (signalType == SignedInteger16Bit) {
        ret = this->StatisticsGAM::Execute<int16>();
    }

    if (signalType == SignedInteger32Bit) {
        ret = this->StatisticsGAM::Execute<int32>();
    }

    if (signalType == SignedInteger64Bit) {
        ret = this->StatisticsGAM::Execute<int64>();
    }

    if (signalType == UnsignedInteger8Bit) {
        ret = this->StatisticsGAM::Execute<uint8>();
    }

    if (signalType == UnsignedInteger16Bit) {
        ret = this->StatisticsGAM::Execute<uint16>();
    }

    if (signalType == UnsignedInteger32Bit) {
        ret = this->StatisticsGAM::Execute<uint32>();
    }

    if (signalType == UnsignedInteger64Bit) {
        ret = this->StatisticsGAM::Execute<uint64>();
    }

    if (signalType == Float32Bit) {
        ret = this->StatisticsGAM::Execute<float32>();
    }

    if (signalType == Float64Bit) {
        ret = this->StatisticsGAM::Execute<float64>();
    }

    return ret;
}

template <class Type> bool StatisticsGAM::Execute() {

    bool ret = true;

    StatisticsHelperT<Type> * ref = NULL_PTR(StatisticsHelperT<Type> *);
    Type input = (Type) 0;
    Type output = (Type) 0;

    ret = MemoryOperationsHelper::Copy(&input, GetInputSignalMemory(0u), sizeof(Type));

    if (ret) {
        ret = (stats != NULL_PTR(void *));
    }

    if (ret) {
        ref = (StatisticsHelperT<Type> *) stats; ref->PushSample(input);
    }

    if (ret) {
        output = ref->GetAvg();
        ret = MemoryOperationsHelper::Copy(GetOutputSignalMemory(0u), &output, sizeof(Type));
    }

    if ((ret) && (GetNumberOfOutputSignals() > 1u)) {
        output = ref->GetStd();
        ret = MemoryOperationsHelper::Copy(GetOutputSignalMemory(1u), &output, sizeof(Type));
    }

    if ((ret) && (GetNumberOfOutputSignals() > 2u)) {
        output = ref->GetMin();
        ret = MemoryOperationsHelper::Copy(GetOutputSignalMemory(2u), &output, sizeof(Type));
    }

    if ((ret) && (GetNumberOfOutputSignals() > 3u)) {
        output = ref->GetMax();
        ret = MemoryOperationsHelper::Copy(GetOutputSignalMemory(3u), &output, sizeof(Type));
    }

    return ret;

}

bool StatisticsGAM::PrepareNextState(const char8 * const currentStateName,
				     const char8 * const nextStateName) {

   bool ret = (stats != NULL_PTR(void *));

    if (ret) {

        if (signalType == SignedInteger8Bit) {
	    StatisticsHelperT<int8> * ref = (StatisticsHelperT<int8> *) stats;
	    ref->Reset();
	}

	if (signalType == SignedInteger16Bit) {
	    StatisticsHelperT<int16> * ref = (StatisticsHelperT<int16> *) stats;
	    ref->Reset();
	}

	if (signalType == SignedInteger32Bit) {
	    StatisticsHelperT<int32> * ref = (StatisticsHelperT<int32> *) stats;
	    ref->Reset();
	}

	if (signalType == SignedInteger64Bit) {
	    StatisticsHelperT<int64> * ref = (StatisticsHelperT<int64> *) stats;
	    ref->Reset();
	}

	if (signalType == UnsignedInteger8Bit) {
	    StatisticsHelperT<uint8> * ref = (StatisticsHelperT<uint8> *) stats;
	    ref->Reset();
	}

	if (signalType == UnsignedInteger16Bit) {
	    StatisticsHelperT<uint16> * ref = (StatisticsHelperT<uint16> *) stats;
	    ref->Reset();
	}

	if (signalType == UnsignedInteger32Bit) {
	    StatisticsHelperT<uint32> * ref = (StatisticsHelperT<uint32> *) stats;
	    ref->Reset();
	}

	if (signalType == UnsignedInteger64Bit) {
	    StatisticsHelperT<uint64> * ref = (StatisticsHelperT<uint64> *) stats;
	    ref->Reset();
	}

	if (signalType == Float32Bit) {
	    StatisticsHelperT<float32> * ref = (StatisticsHelperT<float32> *) stats;
	    ref->Reset();
	}

	if (signalType == Float64Bit) {
	    StatisticsHelperT<float64> * ref = (StatisticsHelperT<float64> *) stats;
	    ref->Reset();
	}

    }

    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Reset StatisticsHelperT<> instance");
    }

    return ret;
}

CLASS_REGISTER(StatisticsGAM, UNIT_VERSION_HR)

} /* namespace MARTe */

