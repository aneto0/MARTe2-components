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

ConfigurationDatabaseConnection::~ConfigurationDatabaseConnection() {
}

bool ConfigurationDatabaseConnection::Initialise(StructuredDataI & data) {

    bool ok = ObjectConnectionI::Initialise(data);

    if (ok) {

        ok = data.MoveRelative("Parameters");
        if (!ok) {
            status.parametersError = true;
            REPORT_ERROR(status, "[%s] - 'Parameters' node not found", GetName());
        }

        // traverse and flatten the `Parameters` node (iteratively to avoid recursion)
        if (ok) {

            ok = data.Copy(parametersCdb);
            StaticList<StreamString*> nodeStack;

            // add root node to the stack
            StreamString* currentNodePtr = new StreamString("");
            ok &= nodeStack.Add(currentNodePtr);

            while ((nodeStack.GetSize() > 0u) && ok) {

                StreamString stackNodePath = "";
                StreamString separator = "";

                // pop element from the stack
                stackNodePath = *(nodeStack[nodeStack.GetSize() - 1u]);
                delete nodeStack[nodeStack.GetSize() - 1u];
                ok = nodeStack.Remove(nodeStack.GetSize() - 1u);

                if ((stackNodePath.Size() > 0u) && ok) { // not on the root node
                    ok = parametersCdb.MoveAbsolute(stackNodePath.Buffer());
                    separator = ".";
                    if (!ok) {
                        status.illegalOperation = true;
                        REPORT_ERROR(status, "[%s] - failed MoveAbsolute()", GetName());
                    }
                }

                for (uint32 elemIdx = 0u; (elemIdx < parametersCdb.GetNumberOfChildren()) && ok; elemIdx++) {

                    StreamString currentNodePath = stackNodePath;
                        currentNodePath += separator;
                        currentNodePath += parametersCdb.GetChildName(elemIdx);

                    // has subnodes: add this node's subelements to the stack
                    if (parametersCdb.MoveToChild(elemIdx)) {
                        if(parametersCdb.MoveToAncestor(1u)) {}

                        currentNodePtr = new StreamString(currentNodePath);
                        ok = nodeStack.Add(currentNodePtr);
                    }
                    // is leaf: add this node to this connection
                    else {
                            // substitute any dash `-` with dots `.` for retrocompatibility
                        while (currentNodePath.Locate("-") != -1) {
                            int32 dashIdx = currentNodePath.Locate("-");
                            (currentNodePath.BufferReference())[dashIdx] = '.';
                        }

                        AnyType* anyTypeParam = new AnyType(parametersCdb.GetType(parametersCdb.GetChildName(elemIdx)));
                        Add(anyTypeParam);
                        paramNames.Add(new StreamString(currentNodePath));
                    }
                }

                if ((stackNodePath.Size() > 0u) && ok) { // not on the root node
                    ok = parametersCdb.MoveToAncestor(1u);
                }
            }

            if (data.MoveToAncestor(1u)) {}
        }
    }

    return ok;
}


CLASS_REGISTER(ConfigurationDatabaseConnection, "1.1")
} /* namespace MARTe */
