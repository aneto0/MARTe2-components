/**
 * @file HistogramGAM.cpp
 * @brief Source file for class HistogramGAM
 * @date 28/08/2018
 * @author Giuseppe Ferro
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
 * the class HistogramGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "AnyType.h"
#include "HistogramGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Helper class that creates all the possible histogram types.
 */
class HistogramComparatorCreator {
public:
    /**
     * @brief Constructor.
     */
    HistogramComparatorCreator();
    /**
     * @brief Destructor.
     */
    virtual ~HistogramComparatorCreator();
    /**
     * @return a new instance of an HistogramComparator.
     */
    virtual HistogramComparator *Create() const=0;

    /**
     * @brief Checks if the instance is compatible with the \a tdIn.
     * @param[in] tdIn the type to check.
     * @return true if the instance is compatible with the \a tdIn.
     */
    virtual bool IsMe(const TypeDescriptor &tdIn) const=0;
};

HistogramComparatorCreator::HistogramComparatorCreator() {

}

HistogramComparatorCreator::~HistogramComparatorCreator() {

}

/**
 * @brief See HistogramComparatorCreator.
 */
template<typename T>
class HistogramComparatorCreatorT: public HistogramComparatorCreator {
public:
    /**
     * @brief Constructor.
     */
    HistogramComparatorCreatorT();
    /**
     * @brief See HistogramComparatorCreator.
     * @param[in] typeNameIn the type of histogram to create.
     */
    HistogramComparatorCreatorT(const char8 * const typeNameIn);
    /**
     * @brief Destructor.
     */
    virtual ~HistogramComparatorCreatorT();
    /**
     * @brief See HistogramComparatorCreator.
     */
    virtual HistogramComparator *Create() const;
    /**
     * @brief See HistogramComparatorCreator.
     */
    virtual bool IsMe(const TypeDescriptor &tdIn) const;

private:
    const char8* typeName;
};

template<typename T>
HistogramComparatorCreatorT<T>::HistogramComparatorCreatorT() :
        HistogramComparatorCreator() {
    typeName = NULL_PTR(char8 *);
}

template<typename T>
HistogramComparatorCreatorT<T>::HistogramComparatorCreatorT(const char8* const typeNameIn) :
        HistogramComparatorCreator() {
    typeName = typeNameIn;
}

template<typename T>
HistogramComparatorCreatorT<T>::~HistogramComparatorCreatorT() {
    typeName = NULL_PTR(char8 *);
}

template<typename T>
HistogramComparator *HistogramComparatorCreatorT<T>::Create() const {
    HistogramComparator *ret = new HistogramComparatorT<T>;
    return ret;
}

template<typename T>
bool HistogramComparatorCreatorT<T>::IsMe(const TypeDescriptor &tdIn) const {
    return (tdIn == TypeDescriptor::GetTypeDescriptorFromTypeName(typeName));
}

/**
 * HistogramComparator int8 instance.
 */
HistogramComparatorCreatorT<int8> compCreateI8("int8");
/**
 * HistogramComparator uint8 instance.
 */
HistogramComparatorCreatorT<uint8> compCreateU8("uint8");
/**
 * HistogramComparator int16 instance.
 */
HistogramComparatorCreatorT<int16> compCreateI16("int16");
/**
 * HistogramComparator uint16 instance.
 */
HistogramComparatorCreatorT<uint16> compCreateU16("uint16");
/**
 * HistogramComparator int32 instance.
 */
HistogramComparatorCreatorT<int32> compCreateI32("int32");
/**
 * HistogramComparator uint32 instance.
 */
HistogramComparatorCreatorT<uint32> compCreateU32("uint32");
/**
 * HistogramComparator int64 instance.
 */
HistogramComparatorCreatorT<int64> compCreateI64("int64");
/**
 * HistogramComparator uint64 instance.
 */
HistogramComparatorCreatorT<uint64> compCreateU64("uint64");
/**
 * HistogramComparator float32 instance.
 */
HistogramComparatorCreatorT<float32> compCreateF32("float32");
/**
 * HistogramComparator float64 instance.
 */
HistogramComparatorCreatorT<float64> compCreateF64("float64");


static HistogramComparatorCreator *compCreator[] = { &compCreateI8, &compCreateU8, &compCreateI16, &compCreateU16, &compCreateI32, &compCreateU32,
        &compCreateI64, &compCreateU64, &compCreateF32, &compCreateF64 };
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HistogramGAM::HistogramGAM() :
        GAM() {
    comps = NULL_PTR(HistogramComparator **);

    beginCycle = 0u;
    cycleCounter = 0u;
}

/*lint -e{1551} destructor does not throw any exception */
HistogramGAM::~HistogramGAM() {
    if (comps != NULL_PTR(HistogramComparator **)) {
        for (uint32 i = 0u; i < numberOfInputSignals; i++) {
            if (comps[i] != NULL_PTR(HistogramComparator *)) {
                delete comps[i];
                comps[i] = NULL_PTR(HistogramComparator *);
            }
        }
        delete[] comps;
        comps = NULL_PTR(HistogramComparator **);
    }
}

bool HistogramGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        if (!data.Read("BeginCycleNumber", beginCycle)) {
            beginCycle = 0u;
        }
    }
    return ret;
}

bool HistogramGAM::Setup() {
    //check that number of input and output signals are equal
    bool ret = (numberOfInputSignals == numberOfOutputSignals);

    if (ret) {
        comps = new HistogramComparator *[numberOfInputSignals];

        for (uint32 i = 0u; (i < numberOfInputSignals); i++) {
            /*lint -e{613} the NULL pointer is checked before*/
            comps[i] = NULL_PTR(HistogramComparator *);
        }
        //check that the number of dimensions of the input signals is one
        /*lint -e{850} the variable i does not change in the loop */
        for (uint32 i = 0u; (i < numberOfInputSignals) && (ret); i++) {
            uint32 numberOfElements;
            ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
            if (ret) {
                ret = (numberOfElements == 1u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "The input signal %d must have NumberOfElements=1", i);
                }
            }
            if (ret) {
                uint32 numberOfSamples;
                ret = GetSignalNumberOfSamples(InputSignals, i, numberOfSamples);
                if (ret) {
                    ret = (numberOfSamples == 1u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::FatalError, "The input signal %d must have NumberOfSamples=1", i);
                    }
                }
            }
            if (ret) {
                TypeDescriptor td = GetSignalType(InputSignals, i);
                ret = false;
                for (uint32 j = 0u; (j < 10u) && (!ret); j++) {
                    if (compCreator[j]->IsMe(td)) {
                        /*lint -e{613} the NULL pointer is checked before*/
                        comps[i] = compCreator[j]->Create();
                        ret = true;
                    }
                }
                if (ret) {
                    ret = signalsDatabase.MoveAbsolute("InputSignals");
                    if (ret) {
                        ret = signalsDatabase.MoveToChild(i);
                    }
                    if (ret) {
                        uint32 tsize = static_cast<uint32>(td.numberOfBits);
                        tsize /= 8u;
                        uint8* ptr = new uint8[tsize];
                        AnyType converted(td, 0u, ptr);

                        ret = signalsDatabase.Read("MinLim", converted);
                        if (ret) {
                            /*lint -e{613} the NULL pointer is checked before*/
                            comps[i]->SetMin(ptr);

                            ret = signalsDatabase.Read("MaxLim", converted);
                            if (ret) {
                                /*lint -e{613} the NULL pointer is checked before*/
                                comps[i]->SetMax(ptr);
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::InitialisationError, "Please Define MaxLim in input signal %d", i);
                            }

                        }
                        else {
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Please Define MinLim in input signal %d", i);
                        }
                        delete[] ptr;
                    }

                }
            }
        }
        (void) signalsDatabase.MoveToRoot();
        //check that the number of elements of output signals is at least 3
        //and the type is uint32
        /*lint -e{850} the variable i does not change in the loop */
        for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
            uint32 numberOfElements;
            ret = GetSignalNumberOfElements(OutputSignals, i, numberOfElements);
            if (ret) {
                ret = (numberOfElements >= 3u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "The output signal %d must have at least NumberOfElements=3", i);
                }
                else {
                    /*lint -e{613} the NULL pointer is checked before*/
                    comps[i]->SetNumberOfBins(numberOfElements);
                }
            }
            if (ret) {
                TypeDescriptor td = GetSignalType(OutputSignals, i);
                ret = (td == UnsignedInteger32Bit);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "The type of the output signal %d must be uint32", i);
                }
            }
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "The number of input signals and output signals must be the same");
    }

    return ret;

}

bool HistogramGAM::Execute() {
    if (cycleCounter >= beginCycle) {
        for (uint32 i = 0u; i < numberOfInputSignals; i++) {
            uint32 *outputSignal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(i));
            /*lint -e{613} the NULL pointer is checked before*/
            uint32 index = comps[i]->InRange(GetInputSignalMemory(i));
            outputSignal[index]++;
        }
    }
    else {
        cycleCounter++;
    }
    return true;
}
CLASS_REGISTER(HistogramGAM, "1.0")
}
