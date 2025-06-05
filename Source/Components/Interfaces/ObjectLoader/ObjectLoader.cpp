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

ErrorManagement::ErrorType ObjectLoader::GetStatus() {
    return status;
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

            data.MoveToAncestor(1u);
        }
    }

    status.initialisationError = !ReferenceContainer::Initialise(data);
    if (status.initialisationError) {
        REPORT_ERROR(status, "[%s] - Failed Initialise().", GetName());
    }

    if (status.ErrorsCleared()) {
        status = UpdateObjects(false);
        if (!status.ErrorsCleared()) {
            REPORT_ERROR(status, "[%s] - failed UpdateObjects()", GetName());
        }
    }

    return status.ErrorsCleared();
}


ErrorManagement::ErrorType ObjectLoader::UpdateObjects(bool overwriteParams /* = true */) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;
    for (uint32 connectionIdx = 0u; (connectionIdx < Size()) && ret; connectionIdx++) {

        ReferenceT<ObjectConnectionI> connection = Get(connectionIdx);
        if (connection.IsValid()) {
            for (uint32 paramIdx = 0u; (paramIdx < connection->GetSize()) && ret; paramIdx++) {

                AnyType* anyTypeParam = connection->operator[](paramIdx);
                StreamString paramName = (connection->GetParameterName(paramIdx)).Buffer();

                ret.exception = (anyTypeParam->GetTypeDescriptor() == InvalidType);
                if (ret.exception) {
                    REPORT_ERROR(ret, "[%s] - parameter %s: invalid type", GetName(), paramName.Buffer());
                }

                if (ret.ErrorsCleared()) {

                    ReferenceT<AnyObject> paramObject = Find(paramName.Buffer());
                    if ( (paramObject.IsValid()) && overwriteParams ) {
                        // parameter is already there, cleanup for updating
                        paramObject->CleanUp();
                        Delete(paramObject);
                    } else {
                        // create new parameter
                        paramObject = ReferenceT<AnyObject>("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    }

                    if (paramObject.IsValid()) {
                        ret.internalSetupError = !(paramObject->Serialise(*anyTypeParam));
                        if (ret.internalSetupError) {
                            REPORT_ERROR(ret, "[%s] - parameter %s: failed Serialise()", GetName(), paramName.Buffer());
                        }
                    }

                    if (ret.ErrorsCleared()) {
                        paramObject->SetName((connection->GetParameterName(paramIdx)).Buffer());
                        ret.illegalOperation = !Insert(paramName.Buffer(), paramObject);
                        if (ret.illegalOperation) {
                            REPORT_ERROR(ret, "[%s] - parameter %s: failed Insert()", GetName(), paramName.Buffer());
                        }
                    }
                }
            }
        }
    }

    return ret;
}


CLASS_REGISTER(ObjectLoader, "1.1")
} /* namespace MARTe */
