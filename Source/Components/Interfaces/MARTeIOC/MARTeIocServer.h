/**
 * @file MARTeIocServer.h
 * @brief Header file for class MARTeIocServer
 * @date 01/07/2019
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

 * @details This header file contains the declaration of the class MARTeIocServer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MARTEIOCSERVER_H_
#define MARTEIOCSERVER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>
#include <casdef.h>
#include <epicsStdlib.h>
#include <epicsGetopt.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "StreamString.h"
#include "Threads.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"
#include "gddAppTable.h"
#include "tsMinMax.h"
#include "alarm.h"
#include "dbMapper.h"
#include "db_access.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {


/**
 * @brief MARTe implementation of EPICS IOC server.
 *
 * @details This interface allows MARTe to publish PVs to EPICS. It is a container of MARTePv objects.
 *
 * @details Follows an example of configuration:
 * <pre>
 * MARTeIoc = {
 *    Class = MARTeIocServer"
 *    RunOnCPU = 1"
 *    +jane = {"
 *        Class = MARTeCommonPv"
 *        TYPE = aitEnumFloat64"
 *        LEN = 1"
 *    }"
 * }
 * </pre>
 */
class MARTeIocServer: public caServer, public ReferenceContainer, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MARTeIocServer();

    /**
     * @brief Destructor
     */
    virtual ~MARTeIocServer();

    /**
     * @brief Initialises the component.
     * @details The user can define the following parameters:\n
     *   RunOnCPU = the CPU where to execute the internal thread.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Replies to the EPICS request of PV exist.
     * @details If the PV is contained it returns a pointer to the PV, otherwise it
     * returns S_casApp_pvNotFound.
     * @param[in] ctx the ca context (not used)
     * @param[in] clientAddress the client address (not used)
     * @param[in] pPVAliasName the PV name
     */
    /*lint -e{1411} -e{114}*/
    virtual pvExistReturn pvExistTest(const casCtx &ctx,
                                      const caNetAddr &clientAddress,
                                      const char8 * pPVAliasName);

    /**
     * @brief Returns the pointer to the PV named as \a pPVAliasName
     * @param[in] ctx the ca context (not used)
     * @param[in] pPVAliasName the PV name
     */
    /*lint -e{1411} -e{114}*/
    virtual pvAttachReturn pvAttach(const casCtx &ctx,
                                    const char8 * pPVAliasName);

    /**
     * @brief Executes the internal thread.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Publishes a PV with the value in input.
     * @param[in] pvNameIn the name of the PV to be written.
     * @param[in] value the value to be written in the PV.
     */
    caStatus IocWrite(const char8 * const pvNameIn,
                      const AnyType &value);

    /**
     * @brief Reads the value of the PV.
     * @param[in] pvNameIn the name of the PV to be read.
     * @param[in] value the read value.
     */
    caStatus IocRead(const char8 * const pvNameIn,
                     const AnyType &value);

    /**
     * @brief Adds a PV to the IOC.
     * @param[in] newPvIn the Reference (must be a ReferenceT<MARTePv>) to the PV to be added.
     * @return true if the PV is added successfully, false otherwise.
     */
    bool IocAddPv(Reference newPvIn);

    /**
     * @brief Removes a PV from the IOC.
     * @param[in] newPvIn the name of the PV to be removed.
     * @return true if the PV is successfully removed, false otherwise.
     */
    bool IocRemovePv(const char8 * const pvName);

    /**
     * @brief Retrieves the number of PVs that belongs to this IOC.
     * @return the number of PVs.
     */
    uint32 GetNumberOfPVs() const;
private:

    /**
     * The cpu mask where the internal thread shall be executed
     */
    uint32 cpuMask;

    /**
     * The number of PVs belonging to this IOC
     */
    uint32 numberOfPVs;

    /**
     * The internal thread executor
     */
    SingleThreadService executor;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



}

#endif /* MARTEIOCSERVER_H_ */

