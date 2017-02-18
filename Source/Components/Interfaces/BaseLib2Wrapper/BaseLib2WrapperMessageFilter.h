/**
 * @file BaseLib2WrapperMessageFilter.h
 * @brief Header file for class BaseLib2WrapperMessageFilter
 * @date 04/12/2016
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

 * @details This header file contains the declaration of the class BaseLib2WrapperMessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASELIB2WRAPPERMESSAGEFILTER_H_
#define BASELIB2WRAPPERMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BaseLib2Wrapper.h"
#include "MessageFilter.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Forwards received messages into BaseLib2 objects
 */
class BaseLib2WrapperMessageFilter : public MessageFilter, public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. Initialises the MessageFilter(false).
     * @post
     *   IsPermanentFilter() == true
     */
    BaseLib2WrapperMessageFilter();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~BaseLib2WrapperMessageFilter();


    /**
     * @brief Forwards the received message into BaseLib2 objects.
     * @param[in] messageToTest The message to be forwarded. The function argument shall have the format
     * DESTINATION::CONTENT, where DESTINATION is the name object to be called in BaseLib2 and CONTENT the content
     * to be set on the message.
     * @return ErrorManagement::NoError if the message can be successfully sent with BaseLib2::Adapter::SendMessageToBaseLib2.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASELIB2WRAPPERMESSAGEFILTER_H_ */

