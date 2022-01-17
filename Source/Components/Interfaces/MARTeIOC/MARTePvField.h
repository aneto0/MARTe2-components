/**
 * @file MARTePvField.h
 * @brief Header file for class MARTePvField
 * @date 04/09/2019
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

 * @details This header file contains the declaration of the class MARTePvField
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVFIELD_H_
#define SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVFIELD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MARTePv.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A field of a PV
 *
 * @details In order to allow the EPICS channel access to read, write and monitor a PV field, these have been treated here as
 * special PVs. The PV that holds a field needs to create and insert in itself (being a ReferenceContainer) a Reference to this
 * object and, using the \a SetPvFather function, set the field characteristics (field identifier and type).
 *
 * @details Once the PV holding this field and the application field id are set, a read/write operation to this object will be mapped
 * to the PV read/write function, passing the id to read the specified field (see MARTeCommonPV::read and MARTeCommonPv::write)
 */
class MARTePvField: public MARTePv {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MARTePvField();

    /**
     * @brief Destructor
     */
    virtual ~MARTePvField();

    /**
     * @brief Reads the PV field. The field is identified by the id (\a applType) set using \a SetPvFather.
     * @param[in] ctx the context.
     * @param[out] prototype the read field value.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    virtual caStatus read(const casCtx &ctx,
                          gdd & prototype);

    /**
     * @brief Writes the PV field. The field is identified by the id (\a applType) set using \a SetPvFather.
     * @param[in] ctx the context.
     * @param[in] value the field value to be written.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    virtual caStatus write(const casCtx &ctx,
                           const gdd & value);

    /**
     * @brief Writes the PV field from inside the IOC. The field is identified by the id (\a applType) set using \a SetPvFather.
     * @param[in] valueIn the field value to be written.
     * @param[in] applType not used. The field id has to be set using \a SetPvFather.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    /*lint -e{1735} default parameter*/
    virtual caStatus WriteDirect(const AnyType& valueIn,
                                 const uint32 applType = 16u);

    /**
     * @brief Writes the PV field from inside the IOC. The field is identified by the id (\a applType) set using \a SetPvFather.
     * @param[in] valueOut the read field value.
     * @param[in] applType not used. The field id has to be set using \a SetPvFather.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    /*lint -e{1735} default parameter*/
    virtual caStatus ReadDirect(const AnyType& valueOut,
                                const uint32 applType = 16u);

    /**
     * @see MARTePv::scan
     * @details Refresh the PV field timestamp and if this field is set as \a interest an event is posted.
     */
    virtual void scan();

    /**
     * @see write
     */
    virtual caStatus writeNotify(const casCtx & ctx,
                                 const gdd & value);

    /**
     * @brief Sets the PV field characteristics.
     * @param[in] fatherIn reference to the PV that holds this field.
     * @param[in] applType the application field identifier.
     * @param[in] pvType the PV field type
     * @param[in] useLocalTsIn if use the local clock to timestamp the PV field or not.
     */
    void SetPvFather(MARTePv &fatherIn,
                     const uint32 applType,
                     const aitEnum pvType,
                     const bool useLocalTsIn);

protected:

    /**
     * Pointer to the PV holding this field
     */
    MARTePv *fatherPv;

    /**
     * The application field identifier
     */
    uint32 applicationId;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVFIELD_H_ */

