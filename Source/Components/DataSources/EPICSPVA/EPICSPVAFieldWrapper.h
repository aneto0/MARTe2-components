/**
 * @file EPICSPVAFieldWrapper.h
 * @brief Header file for class EPICSPVAFieldWrapper
 * @date 07/06/2021
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class EPICSPVAFieldWrapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVAFIELDWRAPPER_H_
#define EPICSPVA_EPICSPVAFIELDWRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <pv/pvaClient.h>
#include <pva/client.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EPICSPVAFieldWrapperI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Helper class which encapsulates a PVA field and allows to put/monitor.
 * @tparam T the type for the PVA field. 
 */
namespace MARTe {
template<typename T>
class EPICSPVAFieldWrapper : public EPICSPVAFieldWrapperI {
public:
    /**
     * @brief Constructor. NOOP.
     */
    EPICSPVAFieldWrapper();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVAFieldWrapper();

    /**
     * @brief Updates the PVA value from the current memory value.
     * @pre
     *  SetMemory
     *  SetPVAField
     */
    virtual void Put();

    /**
     * @brief Updates the memory from the current PVA value.
     * @return true if the value can be successfully read.
     * @pre
     *  SetMemory
     *  SetPVAField
     */
    virtual bool Get();
};
}

namespace MARTe {

template<typename T>
EPICSPVAFieldWrapper<T>::EPICSPVAFieldWrapper() : EPICSPVAFieldWrapperI() {
}

template<typename T>
EPICSPVAFieldWrapper<T>::~EPICSPVAFieldWrapper() {
}

template<typename T>
void EPICSPVAFieldWrapper<T>::Put() {
    if (numberOfElements == 1u) {
        epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast<epics::pvData::PVScalar>(pvField);
        if (scalarFieldPtr ? true : false) {
            scalarFieldPtr->putFrom<T>(*static_cast<T *>(memory));
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Signal %s has an invalid pv field", qualifiedName);
        }
    }
    else {
        epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::dynamic_pointer_cast<epics::pvData::PVScalarArray>(pvField);
        if (scalarArrayPtr ? true : false) {
            epics::pvData::shared_vector<T> out;
            out.resize(numberOfElements);
            (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(out.data()), memory, numberOfElements * sizeof(T));
            epics::pvData::shared_vector<const T> outF = freeze(out);
            scalarArrayPtr->putFrom<T>(outF);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Signal %s has an invalid pv field", qualifiedName);
        }
    }
}

template<>
inline void EPICSPVAFieldWrapper<bool>::Put() {
    if (numberOfElements == 1u) {
        epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast<epics::pvData::PVScalar>(pvField);
        if (scalarFieldPtr ? true : false) {
            scalarFieldPtr->putFrom<epics::pvData::boolean>(*static_cast<epics::pvData::boolean *>(memory));
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Signal %s has an invalid pv field", qualifiedName);
        }
    }
    else {
        epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::dynamic_pointer_cast<epics::pvData::PVScalarArray>(pvField);
        if (scalarArrayPtr ? true : false) {
            epics::pvData::shared_vector<epics::pvData::boolean> out;
            out.resize(numberOfElements);
            (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(out.data()), memory, numberOfElements * sizeof(bool));
            epics::pvData::shared_vector<const epics::pvData::boolean> outF = freeze(out);
            scalarArrayPtr->putFrom<epics::pvData::boolean>(outF);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Signal %s has an invalid pv field", qualifiedName);
        }
    }
}

template<>
inline void EPICSPVAFieldWrapper<char8>::Put() {
    epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast<epics::pvData::PVScalar>(pvField);
    if (scalarFieldPtr ? true : false) {
        std::string value = reinterpret_cast<char8 *>(memory);
        scalarFieldPtr->putFrom<std::string>(value);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "Signal %s has an invalid pv field. Note that arrays of strings are not supported in the data-source yet",
                            qualifiedName.Buffer());
    }
}

template<typename T>
bool EPICSPVAFieldWrapper<T>::Get() {
    bool ok = true;
    if (numberOfElements == 1u) {
        epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast<epics::pvData::PVScalar>(pvField);
        ok = (scalarFieldPtr ? true : false);
        if (ok) {
            *reinterpret_cast<T *>(memory) = scalarFieldPtr->getAs<T>();
        }
    }
    else {
        epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::dynamic_pointer_cast<epics::pvData::PVScalarArray>(pvField);
        ok = (scalarArrayPtr ? true : false);
        if (ok) {
            epics::pvData::shared_vector<const T> out;
            scalarArrayPtr->getAs<T>(out);
            uint32 i;
            Vector<T> readVec(reinterpret_cast<T *>(memory), numberOfElements);
            Vector<T> srcVec(const_cast<T *>(reinterpret_cast<const T *>(out.data())), numberOfElements);
            for (i = 0u; i < numberOfElements; i++) {
                readVec[i] = srcVec[i];
            }
        }
    }
    return ok;
}

template<>
inline bool EPICSPVAFieldWrapper<bool>::Get() {
    bool ok = true;
    if (numberOfElements == 1u) {
        epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast<epics::pvData::PVScalar>(pvField);
        ok = (scalarFieldPtr ? true : false);
        if (ok) {
            *reinterpret_cast<bool *>(memory) = scalarFieldPtr->getAs<epics::pvData::boolean>();
        }
    }
    else {
        epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::dynamic_pointer_cast<epics::pvData::PVScalarArray>(pvField);
        ok = (scalarArrayPtr ? true : false);
        if (ok) {
            epics::pvData::shared_vector<const epics::pvData::boolean> out;
            scalarArrayPtr->getAs<epics::pvData::boolean>(out);
            uint32 i;
            Vector<bool> readVec(reinterpret_cast<bool *>(memory), numberOfElements);
            Vector<bool> srcVec(const_cast<bool *>(reinterpret_cast<const bool *>(out.data())), numberOfElements);
            for (i = 0u; i < numberOfElements; i++) {
                readVec[i] = srcVec[i];
            }
        }
    }
    return ok;
}



template<>
inline bool EPICSPVAFieldWrapper<char8>::Get() {
    bool ok = true;
    epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast<epics::pvData::PVScalar>(pvField);
    ok = (scalarFieldPtr ? true : false);
    if (ok) {
        std::string value = scalarFieldPtr->getAs<std::string>();
        uint32 maxSize = value.size();
        if (maxSize > numberOfElements) {
            maxSize = numberOfElements;
        }
        StringHelper::CopyN(reinterpret_cast<char8 *>(memory), value.c_str(), maxSize);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "Signal %s has an invalid pv field. Note that arrays of strings are not supported in the data-source yet",
                            qualifiedName.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "For strings use Type = char8; for bytes use Type = uint8");
    }
    return ok;
}

}

#endif

