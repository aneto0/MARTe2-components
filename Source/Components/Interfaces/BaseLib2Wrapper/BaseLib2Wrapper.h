/**
 * @file BaseLib2Wrapper.h
 * @brief Header file for class BaseLib2Wrapper
 * @date 02/12/2016
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

 * @details This header file contains the declaration of the class BaseLib2Wrapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASELIB2WRAPPER_H_
#define BASELIB2WRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdapterMessageListener.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "QueuedMessageI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Load BaseLib2 objects into a BaseLib2 GlobalObjectDatabase.
 * @details This class allows to instantiate BaseLib2 objects and to send and receive Messages from these objects.
 * The configuration syntax is (names are only given as an example):
 * +BL2Wrapper = {
 *     Class = BaseLib2Wrapper
 *     BaseLib2Config = "
 *         +SomeBaseLib2Obj1 = {
 *             Class = \"SomeBaseLib2Class\"
 *             ...
 *         }
 *         +AnotherBaseLib2Obj1 = {
 *             Class = \"AnotherBaseLib2Class\"
 *             ...
 *         }
 *     }"
 * }
 *
 */
class BaseLib2Wrapper : public Object, public QueuedMessageI, public BaseLib2::AdapterMessageListener {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     */
    BaseLib2Wrapper();

    /**
     * @brief Destructor. Stops the QueuedMessageI.
     */
    virtual ~BaseLib2Wrapper();

    /**
     * @brief Loads the BaseLib2Config objects into the BaseLib2::Adapter.
     * @param[in] data configuration in the form described in the class description above.
     * The parameters BaseLib2Config is compulsory.
     * @return true iff BaseLib2Config is set and if BaseLib2::Adapter is capable of loading the configuration file.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Propages the Message received from BaseLib2 into MARTe2.
     * @param[in] destination the name of the Object to receive the message (see Message).
     * @param[in] content the Function of the message (see Message).
     * @param[in] code not currently used.
     * @return true if MessageI::SendMessage returns ErrorManagement::NoError.
     */
    virtual bool HandleBaseLib2Message(const char8 *destination, const char8 *content, uint32 code);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASELIB2WRAPPER_H_ */

