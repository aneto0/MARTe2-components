/**
 * @file EPICSPVARecord.h
 * @brief Header file for class EPICSPVARecord
 * @date 12/06/2018
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

 * @details This header file contains the declaration of the class EPICSPVARecord
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVARECORD_H_
#define EPICSPVA_EPICSPVARECORD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/pvDatabase.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "Object.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Serve a MARTe StructureDataI as an epics::pvDatabase::PVRecord.
 * @details It shall be used with EPICSPVADatabase in order to register the PVRecord in a epics::pvDatabase::PVDatabase.
 *
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +Record1 = { //If the Alias field is not set, the Object name is the record name
 *   Class = EPICSPVA::EPICSPVARecord
 *   Alias = "f4e::falcon::Fast::Record1" //Optional. If set this will be the record name.
 *   Structure = {
 *     ElementsA = {
 *       Element1 = {
 *         Type = uint32
 *         NumberOfElements = 10
 *       }
 *       Element2 = {
 *         Type = float32
 *       }
 *       ElementsB = {
 *         ElementB1 = {
 *           Type = uint8
 *           NumberOfElements = 100
 *         }
 *         ElementB2 = {
 *           Type = float32
 *           NumberOfElements = 5
 *         }
 *       }
 *     }
 *   }
 * }
 * </pre>
 */
namespace MARTe {
class EPICSPVARecord: public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
EPICSPVARecord    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVARecord();

    /**
     * @brief Called by the EPICSPVADatabase in order to trigger the creation of the record from the Structure defined in the object configuration (see Initialise).
     * @param[out] pvRecordPtr the PVRecord created from the structured data.
     * @return true if the record was successfully created.
     */
    epics::pvDatabase::PVRecordPtr CreatePVRecord();

    /**
     * @brief Verifies if a node named Structure exits and if so makes a local copy of the configuration \a data.
     * @return true if a node named Structure exists and if the copy is successful, false otherwise.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Gets the record name.
     * @param[out] recName the record name to return.
     */
    void GetRecordName(StreamString &recName);

private:
    /**
     * @brief TODO
     */
    bool InitEPICSStructure(epics::pvData::PVStructurePtr pvStructure);

    /**
     * @brief Recursively constructs the PVRecord from the configuration described in the "Structure" node of the configuration input (see Initialise).
     * @param[in] fieldBuilder the epics::pvData::FieldBuilde that is used to build the epics::pvDatabase::PVRecord.
     * @return true if all the fields are successfully (recursively) added to the epics::pvDatabase::PVRecord.
     */
    bool GetEPICSStructure(epics::pvData::FieldBuilderPtr & fieldBuilder);

    /**
     * @brief Helper method which computes the total number of elements in a given array.
     * TODO
     */
    bool GetArrayNumberOfElements(TypeDescriptor &typeDesc, StructuredDataI &cdb, uint32 &totalElements, uint8 &numberOfDimensions);

    /**
     * Local copy of the ConfigurationDatabase.
     */
    ConfigurationDatabase cdb;

    /**
     * The record name.
     */
    StreamString recordName;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVARECORD_H_ */
