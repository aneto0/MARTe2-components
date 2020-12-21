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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StatisticsGAM::StatisticsGAM() :
        GAM() {

    signalType = InvalidType;
    stats = NULL_PTR(void *);
    windowSize = 1024u;
    startCycleNumber = 0u;
    cycleCounter = 0u;
    infiniteMaxMin = false;
}

/*lint -e{1551} no exception thrown deleting the StatisticsHelperT<> instance*/
StatisticsGAM::~StatisticsGAM() {

    bool ok = (stats != NULL_PTR(void *));

    /* Delete StatisticsHelperT class */

    if (ok) {

        if (signalType == SignedInteger8Bit) {
            StatisticsHelperT<int8> * ref = static_cast<StatisticsHelperT<int8> *>(stats);
            delete ref;
        }

        if (signalType == SignedInteger16Bit) {
            StatisticsHelperT<int16> * ref = static_cast<StatisticsHelperT<int16> *>(stats);
            delete ref;
        }

        if (signalType == SignedInteger32Bit) {
            StatisticsHelperT<int32> * ref = static_cast<StatisticsHelperT<int32> *>(stats);
            delete ref;
        }

        if (signalType == SignedInteger64Bit) {
            StatisticsHelperT<int64> * ref = static_cast<StatisticsHelperT<int64> *>(stats);
            delete ref;
        }

        if (signalType == UnsignedInteger8Bit) {
            StatisticsHelperT<uint8> * ref = static_cast<StatisticsHelperT<uint8> *>(stats);
            delete ref;
        }

        if (signalType == UnsignedInteger16Bit) {
            StatisticsHelperT<uint16> * ref = static_cast<StatisticsHelperT<uint16> *>(stats);
            delete ref;
        }

        if (signalType == UnsignedInteger32Bit) {
            StatisticsHelperT<uint32> * ref = static_cast<StatisticsHelperT<uint32> *>(stats);
            delete ref;
        }

        if (signalType == UnsignedInteger64Bit) {
            StatisticsHelperT<uint64> * ref = static_cast<StatisticsHelperT<uint64> *>(stats);
            delete ref;
        }

        if (signalType == Float32Bit) {
            StatisticsHelperT<float32> * ref = static_cast<StatisticsHelperT<float32> *>(stats);
            delete ref;
        }

        if (signalType == Float64Bit) {
            StatisticsHelperT<float64> * ref = static_cast<StatisticsHelperT<float64> *>(stats);
            delete ref;
        }

    }

    stats = NULL_PTR(void *);

}

bool StatisticsGAM::Initialise(StructuredDataI & data) {

    bool ret = GAM::Initialise(data);

    if (ret) {
        /* Rerieve optional window size attribute */

        if (!data.Read("WindowSize", windowSize)) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to retrieve WindowSize parameter");
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Retrieve '%u' WindowSize parameter", windowSize);
        }

        if (!data.Read("StartCycleNumber", startCycleNumber)) {
            startCycleNumber = 0u;
        }

        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Starting from %u cycle number", startCycleNumber);

        uint8 infiniteMaxMinTemp = 0u;
        if (!data.Read("InfiniteMaxMin", infiniteMaxMinTemp)) {
            infiniteMaxMinTemp = 0u;
        }
        infiniteMaxMin = (infiniteMaxMinTemp > 0u);
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Max and Min are %s", infiniteMaxMin?"absolute":"windowed");
    }

    return ret;
}

bool StatisticsGAM::Setup() {

    /*lint -e{9007} side effect of the '&&' operator is acceptable since the evaluation of the second part
     of the expression is only necessary when the first is verified to be true*/
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

    /*lint -e{850} no modification of the loop index inside the body of the loop (constness issue with the variadic macro ?)*/
    for (signalIndex = 0u; (signalIndex < GetNumberOfOutputSignals()) && (ret); signalIndex++) {

        ret = (signalType == GetSignalType(OutputSignals, signalIndex));

        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalType(OutputSignals, %u) != signalType", signalIndex);
        }

        if (ret) {
            ret = GetSignalNumberOfDimensions(OutputSignals, signalIndex, signalNumberOfDimensions);
        }

        if (ret) {
            ret = (signalNumberOfDimensions == 0u);
        }

        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfDimensions(OutputSignals, %u) != 0u", signalIndex);
        }

        if (ret) {
            ret = GetSignalNumberOfElements(OutputSignals, signalIndex, signalNumberOfElements);
        }

        if (ret) {
            ret = (signalNumberOfElements == 1u);
        }

        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements(OutputSignals, %u) != 1u", signalIndex);
        }

    }

    /* Instantiate Statistics class */

    if (ret) {
        ret = (stats == NULL_PTR(void *));
    }

    /*lint -e{423} no leak as assignment of stats is exclusively done*/
    if (ret) {
        if (signalType == SignedInteger8Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<int8>(windowSize));
        }
        else if (signalType == SignedInteger16Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<int16>(windowSize));
        }
        else if (signalType == SignedInteger32Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<int32>(windowSize));
        }
        else if (signalType == SignedInteger64Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<int64>(windowSize));
        }
        else if (signalType == UnsignedInteger8Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<uint8>(windowSize));
        }
        else if (signalType == UnsignedInteger16Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<uint16>(windowSize));
        }
        else if (signalType == UnsignedInteger32Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<uint32>(windowSize));
        }
        else if (signalType == UnsignedInteger64Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<uint64>(windowSize));
        }
        else if (signalType == Float32Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<float32>(windowSize));
        }
        else if (signalType == Float64Bit) {
            stats = static_cast<void *>(new StatisticsHelperT<float64>(windowSize));
        }
        else { //NOOP
        }
        ret = (stats != NULL_PTR(void *));

        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unsupported type");
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Instantiate StatisticsHelperT<> class");
        }

    }

    return ret;
}

bool StatisticsGAM::Execute() {

    bool ret = (signalType != InvalidType);

    if (ret) {
        if (cycleCounter >= startCycleNumber) {
            if (signalType == SignedInteger8Bit) {
                ret = this->StatisticsGAM::ExecuteT<int8>();
            }
            else if (signalType == SignedInteger16Bit) {
                ret = this->StatisticsGAM::ExecuteT<int16>();
            }
            else if (signalType == SignedInteger32Bit) {
                ret = this->StatisticsGAM::ExecuteT<int32>();
            }
            else if (signalType == SignedInteger64Bit) {
                ret = this->StatisticsGAM::ExecuteT<int64>();
            }
            else if (signalType == UnsignedInteger8Bit) {
                ret = this->StatisticsGAM::ExecuteT<uint8>();
            }
            else if (signalType == UnsignedInteger16Bit) {
                ret = this->StatisticsGAM::ExecuteT<uint16>();
            }
            else if (signalType == UnsignedInteger32Bit) {
                ret = this->StatisticsGAM::ExecuteT<uint32>();
            }
            else if (signalType == UnsignedInteger64Bit) {
                ret = this->StatisticsGAM::ExecuteT<uint64>();
            }
            else if (signalType == Float32Bit) {
                ret = this->StatisticsGAM::ExecuteT<float32>();
            }
            else if (signalType == Float64Bit) {
                ret = this->StatisticsGAM::ExecuteT<float64>();
            }
            else {//NOOP
            }
        }
        else {
            cycleCounter++;
        }
    }

    return ret;
}

template<class Type> bool StatisticsGAM::ExecuteT() {

    /*lint -e{665} [MISRA C++ Rule 16-0-6] templated type passed as argument to MACRO*/
    StatisticsHelperT<Type> * ref = NULL_PTR(StatisticsHelperT<Type> *);
    Type input = (Type) 0;
    Type output = (Type) 0;

    bool ret = MemoryOperationsHelper::Copy(&input, GetInputSignalMemory(0u), sizeof(Type));

    if (ret) {
        ret = (stats != NULL_PTR(void *));
    }

    if (ret) {
        ref = static_cast<StatisticsHelperT<Type> *>(stats);
        ret = ref->PushSample(input, infiniteMaxMin);
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

/*lint -e{613} Reset() method called only when stats is not NULL*/
/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns irrespectively of the input parameters.*/
bool StatisticsGAM::PrepareNextState(const char8 * const currentStateName,
                                     const char8 * const nextStateName) {

    bool ret = (stats != NULL_PTR(void *));

    if (ret) {

        if (signalType == SignedInteger8Bit) {
            StatisticsHelperT<int8> * ref = static_cast<StatisticsHelperT<int8> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == SignedInteger16Bit) {
            StatisticsHelperT<int16> * ref = static_cast<StatisticsHelperT<int16> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == SignedInteger32Bit) {
            StatisticsHelperT<int32> * ref = static_cast<StatisticsHelperT<int32> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == SignedInteger64Bit) {
            StatisticsHelperT<int64> * ref = static_cast<StatisticsHelperT<int64> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == UnsignedInteger8Bit) {
            StatisticsHelperT<uint8> * ref = static_cast<StatisticsHelperT<uint8> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == UnsignedInteger16Bit) {
            StatisticsHelperT<uint16> * ref = static_cast<StatisticsHelperT<uint16> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == UnsignedInteger32Bit) {
            StatisticsHelperT<uint32> * ref = static_cast<StatisticsHelperT<uint32> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == UnsignedInteger64Bit) {
            StatisticsHelperT<uint64> * ref = static_cast<StatisticsHelperT<uint64> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == Float32Bit) {
            StatisticsHelperT<float32> * ref = static_cast<StatisticsHelperT<float32> *>(stats);
            ret = ref->Reset();
        }

        if (signalType == Float64Bit) {
            StatisticsHelperT<float64> * ref = static_cast<StatisticsHelperT<float64> *>(stats);
            ret = ref->Reset();
        }

    }

    if (ret) {
        REPORT_ERROR(ErrorManagement::Information, "Reset StatisticsHelperT<> instance");
    }

    return ret;
}

CLASS_REGISTER(StatisticsGAM, "1.0")

} /* namespace MARTe */

