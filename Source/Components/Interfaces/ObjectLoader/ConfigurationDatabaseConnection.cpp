/**
 * @file ConfigurationDatabaseConnection.cpp
 * @brief Source file for class ConfigurationDatabaseConnection
 * @date 16/09/2024
 * @author Cristian Galperti
 * @author Nicolo Ferron
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ConfigurationDatabaseConnection (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabaseConnection.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ConfigurationDatabaseConnection::ConfigurationDatabaseConnection() :
        ObjectConnectionI() {
}

/*lint -e{1551} Justification: no exceptions thrown */
ConfigurationDatabaseConnection::~ConfigurationDatabaseConnection() {
    ErrorManagement::ErrorType ret = ConfigurationDatabaseConnection::CleanUp();
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ret, "[%s] - Failed freeing memory in destructor.", GetName());
    }
}

/*lint -e{423, 429} Justification: the while loop condition ensures that all the allocated objects are freed */
ErrorManagement::ErrorType ConfigurationDatabaseConnection::UpdateParameters() {

    ErrorManagement::ErrorType ret = CleanUp();

    // traverse and flatten the `Parameters` node (iteratively to avoid recursion)
    StaticList<StreamString*> nodeStack;

    // add root node to the stack
    StreamString* currentNodePtr = new StreamString("");
    ret.exception = !nodeStack.Add(currentNodePtr);

    bool noErrors = ret.ErrorsCleared();
    while ((nodeStack.GetSize() > 0u) && noErrors) {

        StreamString stackNodePath = "";
        StreamString separator = "";

        // pop element from the stack
        stackNodePath = *(nodeStack[nodeStack.GetSize() - 1u]);
        delete nodeStack[nodeStack.GetSize() - 1u];
        ret.exception = !nodeStack.Remove(nodeStack.GetSize() - 1u);

        noErrors = ret.ErrorsCleared();
        if ((stackNodePath.Size() > 0u) && noErrors) { // not on the root node
            ret.illegalOperation = !parametersCdb.MoveAbsolute(stackNodePath.Buffer());
            separator = ".";
            if (bool(ret.illegalOperation)) {
                REPORT_ERROR(status, "[%s] - failed MoveAbsolute()", GetName());
            }
        }

        noErrors = ret.ErrorsCleared();
        for (uint32 elemIdx = 0u; (elemIdx < parametersCdb.GetNumberOfChildren()) && noErrors; elemIdx++) {

            StreamString currentNodePath = stackNodePath;
                currentNodePath += separator;
                currentNodePath += parametersCdb.GetChildName(elemIdx);

            // has subnodes: add this node's subelements to the stack
            if (parametersCdb.MoveToChild(elemIdx)) {
                if(parametersCdb.MoveToAncestor(1u)) {}

                currentNodePtr = new StreamString(currentNodePath);
                ret.exception = !nodeStack.Add(currentNodePtr);
            }
            // is leaf: add this node to this connection
            else {
                    // substitute any dash `-` with dots `.` for retrocompatibility
                while (currentNodePath.Locate("-") != -1) {
                    int32 dashIdx = currentNodePath.Locate("-");
                    (currentNodePath.BufferReference())[dashIdx] = '.';
                }

                AnyType* anyTypeParam = new AnyType(parametersCdb.GetType(parametersCdb.GetChildName(elemIdx)));
                ret.exception = !Add(anyTypeParam);
                if (ret.ErrorsCleared()) {
                    ret.exception = !paramNames.Add(new StreamString(currentNodePath));
                }
            }
        }

        noErrors = ret.ErrorsCleared();
        if ( (stackNodePath.Size() > 0u) && noErrors) { // not on the root node
            ret.illegalOperation = !parametersCdb.MoveToAncestor(1u);
        }
    }

    return ret;
}

ErrorManagement::ErrorType ConfigurationDatabaseConnection::CleanUp() {

    ErrorManagement::ErrorType ret = ObjectConnectionI::CleanUp();

    return ret;
}

bool ConfigurationDatabaseConnection::Initialise(StructuredDataI & data) {

    status.initialisationError = !ObjectConnectionI::Initialise(data);

    if (status.ErrorsCleared()) {

        status.parametersError = !data.MoveRelative("Parameters");
        if (bool(status.parametersError)) {
            REPORT_ERROR(status, "[%s] - 'Parameters' node not found", GetName());
        }

        if (status.ErrorsCleared()) {
            status.exception = !data.Copy(parametersCdb);
            if (data.MoveToAncestor(1u)) {}
        }

        if (status.ErrorsCleared()) {
            status = UpdateParameters();
            if (!status) {
                REPORT_ERROR(status, "[%s] - Failed 'UpdateParameters' in Initialise.", GetName());
            }
        }
    }

    return status;
}


CLASS_REGISTER(ConfigurationDatabaseConnection, "2.0")
} /* namespace MARTe */
