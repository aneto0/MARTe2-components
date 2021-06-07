/**
 * @file EPICSPVAFieldWrapperI.h
 * @brief Header file for class EPICSPVAFieldWrapperI
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

 * @details This header file contains the declaration of the class EPICSPVAFieldWrapperI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVAFIELDIWRAPPER_H_
#define EPICSPVA_EPICSPVAFIELDIWRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <pv/pvaClient.h>
#include <pva/client.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Interface to abstract the use of the EPICSPVAFieldWrapperI template.
 */
class EPICSPVAFieldWrapperI {
public:
    /**
     * @brief Constructor. NOOP.
     */
    EPICSPVAFieldWrapperI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVAFieldWrapperI();

    /**
     * @brief Sets the signal memory parameters.
     * @param[in] numberOfElementsIn the field number of elements.
     * @param[in] qualifiedNameIn the signal fully qualified name.
     * @param[in] memoryIn the memory where to copy/read the values from.
     */
    void SetMemory(uint32 numberOfElementsIn, const char8 *qualifiedNameIn, void *memoryIn);

    /**
     * @brief Sets the PVA field.
     * @param[in] pvFieldIn the pvField to set.
     */
    void SetPVAField(epics::pvData::PVFieldPtr pvFieldIn);

    /**
     * @brief Updates the PVA value from the current memory value.
     */
    virtual void Put() = 0;

    /**
     * @brief Updates the memory from the current PVA value.
     * @return true if the value can be successfully read.
     */
    virtual bool Get() = 0;

protected:
    /**
     * Number of elements in the record signal.
     */
    uint32 numberOfElements;

    /**
     * Full qualified name of the signal inside the record (dot separated).
     */
    StreamString qualifiedName;

    /**
     * Allocate memory with sufficient space to hold a copy of the signal.
     */
    void *memory;

    /**
     * The pv field
     */
    epics::pvData::PVFieldPtr pvField;
};
}

#endif

