/**
 * @file BaseLib2Wrapper.cpp
 * @brief Source file for class BaseLib2Wrapper
 * @date Dec 2, 2016 TODO Verify the value and format of the date
 * @author aneto TODO Verify the name and format of the author
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
 * the class BaseLib2Wrapper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "Adapter.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BaseLib2Wrapper.h"
#include "BaseLib2WrapperMessageFilter.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BaseLib2Wrapper::BaseLib2Wrapper() :
        Object(),
        QueuedMessageI() {
    BaseLib2::Adapter::Instance()->SetAdapterMessageListener(this);
}

/*lint -e{1551} the destructor must guarantee that the QueuedMessageI SingleThreadService is stopped.*/
BaseLib2Wrapper::~BaseLib2Wrapper() {
    ErrorManagement::ErrorType err = Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Stop the BaseLib2Wrapper. Retrying...");
        err = Stop();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not Stop the BaseLib2Wrapper.");
        }
    }
    BaseLib2::Adapter::Instance()->UnloadObjects();
}

bool BaseLib2Wrapper::Initialise(StructuredDataI & data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ReferenceT<BaseLib2WrapperMessageFilter> filter(new BaseLib2WrapperMessageFilter());
        ok = (InstallMessageFilter(filter) == ErrorManagement::NoError);
    }
    StreamString baseLib2Config;
    if (ok) {
        ok = data.Read("BaseLib2Config", baseLib2Config);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The BaseLib2Config parameter shall be specified.");
        }
    }
    if (ok) {
        ok = baseLib2Config.Seek(0LLU);
    }
    if (ok) {
        ok = BaseLib2::Adapter::Instance()->LoadObjects(baseLib2Config.Buffer());
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to LoadObjects in BaseLib2::Adapter");
        }
    }
    if (ok) {
        ok = (Start() == ErrorManagement::NoError);
    }
    return ok;
}

bool BaseLib2Wrapper::HandleBaseLib2Message(const char8 * const destination,
                                            const char8 * const content,
                                            uint32 code) {
    REPORT_ERROR(ErrorManagement::Information, "Received message to %s with content %s and code %d\n", destination, content, code);
    ReferenceT<Message> msg(new Message());
    ConfigurationDatabase cdb;
    bool ok = cdb.Write("Destination", destination);
    if (ok) {
        ok = cdb.Write("Function", content);
    }
    if (ok) {
        ok = msg->Initialise(cdb);
    }
    if (ok) {
        ok = (MessageI::SendMessage(msg, this) == ErrorManagement::NoError);
    }
    return ok;
}

CLASS_REGISTER(BaseLib2Wrapper, "1.0")
}
