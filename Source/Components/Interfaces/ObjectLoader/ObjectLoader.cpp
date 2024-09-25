/**
 * @file ObjectLoader.h
 * @brief Source file for class ObjectLoader
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
 * the class ObjectLoader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ObjectLoader.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ObjectLoader::ObjectLoader() :
        ReferenceContainer(), MessageI() {
}

ObjectLoader::~ObjectLoader() {
}

bool ObjectLoader::Initialise(StructuredDataI & data) {

    // copy additional parameters from leaves
    ConfigurationDatabase additionalNodes;
    for (uint32 nodeIdx = 0u; nodeIdx < data.GetNumberOfChildren(); nodeIdx++) {
        StreamString nodeName = data.GetChildName(nodeIdx);
        if (nodeName[0u] != '+' && nodeName != "Class") {
            additionalNodes.Write(nodeName.Buffer(), data.GetType(nodeName.Buffer()));
        }
    }
    // paste additional parameters to subnodes
    for (uint32 nodeIdx = 0u; nodeIdx < data.GetNumberOfChildren(); nodeIdx++) {
        if (data.MoveToChild(nodeIdx)) {

            for (uint32 addNodeIdx = 0u; addNodeIdx < additionalNodes.GetNumberOfChildren(); addNodeIdx++) {
                StreamString currentParam = additionalNodes.GetChildName(addNodeIdx);
                StreamString temp;
                // copy parameter only if it`s not already there
                if (!data.Read(currentParam.Buffer(), temp)) {
                    data.Write(currentParam.Buffer(), additionalNodes.GetType(currentParam.Buffer()));
                }
            }

            //additionalNodes.Copy(data);
            data.MoveToAncestor(1u);
        }
    }

    bool ok = ReferenceContainer::Initialise(data);
    if (!ok) {
        status.initialisationError = true;
        REPORT_ERROR(status, "[%s] - Failed Initialise().", GetName());
    }

    for (uint32 connectionIdx = 0u; (connectionIdx < Size()) && ok; connectionIdx++) {

        ReferenceT<ObjectConnectionI> connection = Get(connectionIdx);
        if (connection.IsValid()) {
            for (uint32 paramIdx = 0u; (paramIdx < connection->GetSize()) && ok; paramIdx++) {
                AnyType* anyTypeParam = connection->operator[](paramIdx);

                ok = (anyTypeParam->GetTypeDescriptor() != InvalidType);
                if (!ok) {
                    status.exception = true;
                    REPORT_ERROR(status, "[%s] - invalid type", GetName());
                }

                if (ok) {

                    ReferenceT<AnyObject> paramObject("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    if (paramObject.IsValid()) {
                        ok = paramObject->Serialise(*anyTypeParam);
                    }
                    if (!ok) {
                        status.internalSetupError = true;
                        REPORT_ERROR(status, "[%s] - failed Serialise()", GetName());
                    }

                    if (ok) {
                        paramObject->SetName((connection->GetParameterName(paramIdx)).Buffer());
                        ok = Insert((connection->GetParameterName(paramIdx)).Buffer(), paramObject);
                        if (!ok) {
                            status.illegalOperation = true;
                            REPORT_ERROR(status, "[%s] - failed Insert()", GetName());
                        }
                    }
                }
            }
        }
    }

    return ok;
}


CLASS_REGISTER(ObjectLoader, "1.0")
} /* namespace MARTe */
