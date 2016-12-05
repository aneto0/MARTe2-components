/**
 * @file BaseLib2WrapperMessageFilter.cpp
 * @brief Source file for class BaseLib2WrapperMessageFilter
 * @date 4/12/2016
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

 * @details This source file contains the definition of all the methods for
 * the class BaseLib2WrapperMessageFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
//Otherwise lint may stop here when the target system does not have this module
#ifdef EFDA_MARTe_DIR
#include "Adapter.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BaseLib2WrapperMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
BaseLib2WrapperMessageFilter::BaseLib2WrapperMessageFilter() :
        MessageFilter(false),
        Object() {

}

BaseLib2WrapperMessageFilter::~BaseLib2WrapperMessageFilter() {

}

ErrorManagement::ErrorType BaseLib2WrapperMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest) {
    ErrorManagement::ErrorType err;
    BaseLib2::Adapter *adapter = BaseLib2::Adapter::Instance();
    StreamString function = messageToTest->GetFunction();
    StreamString destination;
    StreamString content;
    char8 terminator;
    bool ok = function.Seek(0ULL);
    if (ok) {
        ok = function.GetToken(destination, "::", terminator);
    }
    if (ok) {
        ok = function.GetToken(content, "::", terminator);
    }
    if (ok) {
        ok = adapter->SendMessageToBaseLib2(destination.Buffer(), content.Buffer(), 0u);
    }
    err = !ok;
    return err;
}

CLASS_REGISTER(BaseLib2WrapperMessageFilter, "1.0")
}
#endif
