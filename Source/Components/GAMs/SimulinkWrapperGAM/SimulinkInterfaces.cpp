/**
 * @file SimulinkClasses.cpp
 * @brief Source file for class SimulinkClasses
 * @date 27/05/2019
 * @author Bertrand Bauvir
 * @author Cristian Galperti
 * @author Nicolo Ferron
 * @author Giuseppe Avon
 *
 * @copyright Copyright 2019 Swiss Plasma Center, EPFL Lausanne Switzerland
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
 * the class SimulinkWrapperGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SimulinkInterfaces.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe
{

/*---------------------------------------------------------------------------*/
/*                            SimulinkInterface                              */
/*---------------------------------------------------------------------------*/

SimulinkInterface::SimulinkInterface() {

    interfaceName = "";
    structPath    = "";
    fullPath      = "";

    interfaceType = InvalidInterface;

    CTypeName     = "";
    className     = "";
    MARTeTypeName = "";
    typeDesc      = InvalidType;

    numElements   = 0u;
    numDimensions = 0u;
    dimensions.SetSize(0u);
    orientation = rtwCAPI_SCALAR;

    dataAddr     = NULL_PTR(void*);
    MARTeAddress = NULL_PTR(void*);
    byteSize     = 0u;
    dataTypeSize = 0u;

    destPtr   = NULL_PTR(void*);
    sourcePtr = NULL_PTR(void*);

    transpose    = true;
    isStructured = false;

}

SimulinkInterface::~SimulinkInterface() {
    if (dimensions.GetNumberOfElements() != 0u) {
        dimensions.SetSize(0u);
    }
}

bool SimulinkInterface::Actualise(const AnyType& sourceParameter) {

    bool ok;

    // Type coherence check
    TypeDescriptor slkType = typeDesc;
    TypeDescriptor extType = sourceParameter.GetTypeDescriptor();

    ok = (extType == slkType);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
            "Parameter %s data type not matching (parameter source: %s, model: %s)",
            fullPath.Buffer(),
            TypeDescriptor::GetTypeNameFromTypeDescriptor(extType),
            MARTeTypeName.Buffer());
    }

    // In case of an enum parameter ("numeric") a warning is issued
    // if actualisation of the enum type is based on an integer type
    if (ok) {
        /*lint -e{9007} no side-effects on the right of the && operator */
        if ( (typeDesc.IsNumericType()) && (StreamString(CTypeName) == "numeric") ) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning,
                "Parameter %s is of enum type %s but is being actualised using %s. No check is performed on input data range.",
                fullPath.Buffer(), className, MARTeTypeName.Buffer());
        }
    }

    // Type size coherence check
    if (ok) {

        uint32 slkTypeSize = dataTypeSize;
        uint32 extTypeSize = sourceParameter.GetByteSize();

        ok = (extTypeSize == slkTypeSize);

        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                "Parameter %s data type size not matching (parameter source: %d, model: %d)",
                fullPath.Buffer(), extTypeSize, slkTypeSize);
        }
    }

    // Number of dimensions check (scalar, vector or matrix)
    if(ok) {

        ok = (sourceParameter.GetNumberOfDimensions() == numDimensions);

        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                "Parameter %s number of dimensions not matching (parameter source: %d, model: %d)",
                fullPath.Buffer(), sourceParameter.GetNumberOfDimensions(), numDimensions);
        }
    }

    // Dimensionality check
    if(ok)
    {
        uint32 extDim1 = sourceParameter.GetNumberOfElements(1u);  // AnyType has rows on position 1
        uint32 extDim2 = sourceParameter.GetNumberOfElements(0u);
        uint32 extDim3 = sourceParameter.GetNumberOfElements(2u);

        uint32 slkDim1 = dimensions[0u];
        uint32 slkDim2 = dimensions[1u];
        uint32 slkDim3 = dimensions.GetNumberOfElements() > 2u ? dimensions[2u] : 1u;

        // On the model side, scalars are [1,1], vectors are [1,N] or [N,1],
        // matrices are [N,M]. AnyType instead can be respectively [1], [N], [N,M].
        switch (sourceParameter.GetNumberOfDimensions()) {

            case 0u: { // Scalar

                ok = (extDim1 == 1u);
                break;
            }

            case 1u: { // Vector

                // Model vectors can be [1,N] or [N,1], both are ok
                bool orientOk1 = ( (extDim1 == slkDim1) && (extDim2 == 1u)      );
                bool orientOk2 = ( (extDim1 == 1u)      && (extDim2 == slkDim1) );
                bool orientOk3 = ( (extDim1 == slkDim2) && (extDim2 == 1u)      );

                ok = ( orientOk1 || orientOk2 || orientOk3 );
                break;
            }

            case 2u: { // Matrix

                ok = ( (extDim1 == slkDim1) && (extDim2 == slkDim2) && (extDim3 == 1u) );
                break;
            }

            case 3u: { // 3D matrix

                ok = ( (extDim1 == slkDim1) && (extDim2 == slkDim2) && (extDim3 == slkDim3) );
                break;
            }

            default:

                REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                    "Parameter %s: number of dimensions > 3.",
                    fullPath.Buffer());
                break;

        }
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                "Parameter %s: dimensions not matching (parameter source: [%u, %u, %u], model: [%u, %u, %u]).",
                fullPath.Buffer(),
                extDim1, extDim2, extDim3,
                slkDim1, slkDim2, slkDim3);
        }
    }

    // Total data size check
    if (ok) {

        ok = (sourceParameter.GetDataSize() == byteSize);

        if(!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                "Parameter %s: total data size not matching (parameter source: %d, model: %d)",
                fullPath.Buffer(), sourceParameter.GetDataSize(), byteSize);
        }
    }

    // Orientation check is not necessary: an AnyType is passed by, so
    // whatever generated this AnyType is in charge for granting that
    // the AnyType data buffer is in standard C++ row-major orientation.

    // Checks passed, data buffer can be copied from the input AnyType to the model memory
    if (ok) {

        if (numDimensions <= 1u) {

            // Scalars and vectors have no orientation and can be copied as they are.
            ok = MemoryOperationsHelper::Copy(dataAddr, sourceParameter.GetDataPointer(), sourceParameter.GetDataSize());

        }
        else if (numDimensions == 2u) {

            // For 2D matrices we handle the case in which model has column-major parameters.
            if (orientation == rtwCAPI_MATRIX_ROW_MAJOR) {
                ok = MemoryOperationsHelper::Copy(dataAddr, sourceParameter.GetDataPointer(), sourceParameter.GetDataSize());
            }
            else {
                ok = TransposeAndCopy(dataAddr, sourceParameter.GetDataPointer(), Parameter);
            }
        }
        else {

            // Also 3D matrices are memcopied, since handling all possible combinations of cases is not feasible
            ok = MemoryOperationsHelper::Copy(dataAddr, sourceParameter.GetDataPointer(), sourceParameter.GetDataSize());
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "3D matrix used. The GAM does not check data orientation of 3D matrices, carefully check results.");
        }

    }

    return ok;
}

bool SimulinkInterface::TransposeAndCopy(void *const destination, const void *const source, const InterfaceType mode) {

    bool ok = false;

    if (typeDesc==UnsignedInteger8Bit) {

        ok = TransposeAndCopyT<uint8>(destination, source, mode);

    } else if (typeDesc==UnsignedInteger16Bit) {

        ok = TransposeAndCopyT<uint16>(destination, source, mode);

    } else if (typeDesc==UnsignedInteger32Bit) {

        ok = TransposeAndCopyT<uint32>(destination, source, mode);

    } else if (typeDesc==UnsignedInteger64Bit) {

        ok = TransposeAndCopyT<uint64>(destination, source, mode);

    } else if (typeDesc==SignedInteger8Bit) {

        ok = TransposeAndCopyT<int8>(destination, source, mode);

    } else if (typeDesc==SignedInteger16Bit) {

        ok = TransposeAndCopyT<int16>(destination, source, mode);

    } else if (typeDesc==SignedInteger32Bit) {

        ok = TransposeAndCopyT<int32>(destination, source, mode);

    } else if (typeDesc==SignedInteger64Bit) {

        ok = TransposeAndCopyT<int64>(destination, source, mode);

    } else if (typeDesc==Float32Bit) {

        ok = TransposeAndCopyT<float32>(destination, source, mode);

    } else if (typeDesc==Float64Bit) {

        ok = TransposeAndCopyT<float64>(destination, source, mode);

    } else {

        REPORT_ERROR_STATIC(ErrorManagement::Exception,
                     "Unsupported type %s", TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDesc));
    }

    return ok;
}

template<typename T>
bool SimulinkInterface::TransposeAndCopyT(void *const destination, const void *const source, const InterfaceType mode) {

    uint32 numberOfRows    = dimensions[0u];
    uint32 numberOfColumns = dimensions[1u];
    uint32 numberOfPages   = dimensions.GetNumberOfElements() <= 2u ? 1u : dimensions[2u];

    if (numberOfPages > 1u) {
        // 3D matrix
        for (uint32 rowIdx = 0u; rowIdx < numberOfRows; rowIdx++) {
            for (uint32 colIdx = 0u; colIdx < numberOfColumns; colIdx++) {
                for (uint32 pagIdx = 0u; pagIdx < numberOfPages; pagIdx++) {
                    if (mode == InputPort || mode == Parameter) {   // row-major to col-major
                        *((T *)destination + rowIdx + numberOfRows  * colIdx + numberOfColumns * numberOfRows  * pagIdx)
                        = *((T *)source    + pagIdx + numberOfPages * colIdx + numberOfColumns * numberOfPages * rowIdx);
                    }
                    else {   // col-major to row-major
                        *((T *)destination + pagIdx + numberOfPages * colIdx + numberOfColumns * numberOfPages * rowIdx)
                        = *((T *)source    + rowIdx + numberOfRows  * colIdx + numberOfColumns * numberOfRows  * pagIdx);
                    }
                }
            }
        }
    }
    else {
        // 2D matrix
        for (uint32 rowIdx = 0u; rowIdx < numberOfRows; rowIdx++) {
            for (uint32 colIdx = 0u; colIdx < numberOfColumns; colIdx++) {
                if (mode == InputPort || mode == Parameter) {
                    *( (T*) destination + rowIdx + numberOfRows    * colIdx )
                    = *( (T*) source    + colIdx + numberOfColumns * rowIdx);
                }
                else {
                    *((T *)destination + colIdx + numberOfColumns * rowIdx)
                    = *((T *)source    + rowIdx + numberOfRows    * colIdx);
                }
            }
        }
    }

    return true;
}



/*---------------------------------------------------------------------------*/
/*                          SimulinkRootInterface                            */
/*---------------------------------------------------------------------------*/

SimulinkRootInterface::SimulinkRootInterface()
    : SimulinkInterface() {}

SimulinkRootInterface::~SimulinkRootInterface() {
    while (GetSize() > 0u) {
        SimulinkInterface* toDelete;
        if(Extract(0U, toDelete)) {
            delete toDelete;
        }
    }
}

bool SimulinkRootInterface::CopyData(const SimulinkNonVirtualBusMode copyMode) {

    bool ok = true;
    // Copy signal content, telling apart the two modes (struct or byte array)
    // If address==NULL, this signal or port has no corresponding MARTe signal and thus is not mapped
    if (!transpose) {
        if( (copyMode == StructuredBusMode) && (isStructured) ) {
            for(uint32 carriedSignalIdx = 0u; (carriedSignalIdx < GetSize()) && ok; carriedSignalIdx++) {
                if((*this)[carriedSignalIdx]->destPtr != NULL) {
                    ok = MemoryOperationsHelper::Copy((*this)[carriedSignalIdx]->destPtr, (*this)[carriedSignalIdx]->sourcePtr, (*this)[carriedSignalIdx]->byteSize);
                }
            }
        }
        else {
            if(destPtr != NULL) {
                ok = MemoryOperationsHelper::Copy(destPtr, sourcePtr, byteSize);
            }
        }
    }
    else {
        if( (copyMode == StructuredBusMode) && (isStructured ) ) {
            for(uint32 carriedSignalIdx = 0u; (carriedSignalIdx < GetSize()) && ok; carriedSignalIdx++) {
                if((*this)[carriedSignalIdx]->destPtr != NULL) {
                    ok = (*this)[carriedSignalIdx]->TransposeAndCopy((*this)[carriedSignalIdx]->destPtr, (*this)[carriedSignalIdx]->sourcePtr, interfaceType);
                }
            }
        }
        else if ( (copyMode == ByteArrayBusMode) && (isStructured ) ) {
            // Always plain copy in this case as the port is always a 1D data buffer
            if(destPtr != NULL) {
                ok = MemoryOperationsHelper::Copy(destPtr, sourcePtr, byteSize);
            }
        }
        else {
            if(destPtr != NULL) {
                ok = TransposeAndCopy(destPtr, sourcePtr, interfaceType);
            }
        }
    }

    return ok;
}

void SimulinkRootInterface::Print(const uint64 paddingLength /*= 50u*/, StreamString parentSpacer /*= ""*/) {

    uint32 currNumOfChildren = rootStructure.GetNumberOfChildren();

    for (uint32 parIdx = 0u; parIdx < currNumOfChildren; parIdx++) {

        StreamString spacer = "";
        StreamString printName = "";

        if (StreamString(rootStructure.GetName()) == StreamString("")) {  // root node
            spacer = "▪ ";
        }
        else {
            if (rootStructure.GetNumberOfChildren() == parIdx + 1u) {
                spacer = "└ ";
            }
            else {
                spacer = "├ ";
            }
        }

        printName = parentSpacer;
        printName += spacer;

        // substructure
        if (rootStructure.MoveToChild(parIdx)) {

            // format the structure name
            printName += rootStructure.GetName();
            while (printName.Size() < ( paddingLength + (parentSpacer.Size() + spacer.Size())/2u )) {
                printName += " ";
            }

            // get the structure base address (address of the first element)
            StreamString firstElementName = rootStructure.GetChildName(0u);
            uint32 firstElementIdx = 0u;
            rootStructure.Read(firstElementName.Buffer(), firstElementIdx);
            SimulinkInterface* currentInterface = this->operator[](firstElementIdx);
            void* structAddr = currentInterface->dataAddr;

            // get the structure shape (not trivial for arrays of struct)
            StreamString sizeFiller = "     ";
            uint32 numOfDims = 0u;
            uint32 MARTeNumOfDims = 0u;
            uint32 numOfElements = 1u;
            Vector<uint32> dimensions(2u);
            dimensions[0u] = 1u; dimensions[1u] = 1u;
            AnyType arrayDescription = rootStructure.GetType("__Dimensions__");
            if (arrayDescription.GetDataPointer() != NULL_PTR(void *)) {
                numOfDims = arrayDescription.GetNumberOfElements(0u);
                uint32* dimPtr = new uint32[numOfDims];  //lint !e429 Justification: memory pointed by dimPtr is freed using dimensions.SetSize(0u) in the destructor
                dimensions = Vector<uint32>(dimPtr, numOfDims);
                if (rootStructure.Read("__Dimensions__", dimensions)) {
                    if (numOfDims >= 3u) {
                        MARTeNumOfDims = numOfDims - 1u;
                        sizeFiller = "";
                    }
                    else {
                        if (dimensions[0u] != 1u && dimensions[1u] != 1u) {
                            MARTeNumOfDims = 2u;
                        }
                        else {
                            MARTeNumOfDims = 1u;
                        }
                    }

                    for (uint32 dimIdx = 0u; dimIdx < numOfDims; dimIdx++) {
                        numOfElements *= dimensions[dimIdx];
                    }
                }

                if (rootStructure.Delete("__Dimensions__")) {}
            }

            // calculate the substructure bytesize
            uint32 structBytesize = GetInterfaceBytesize(rootStructure);

            REPORT_ERROR_STATIC(ErrorManagement::Information, "%s| struct  | % 4d | % 5d | % 4!%s | %p | %d [struct]", printName.Buffer(),
                                MARTeNumOfDims, numOfElements, dimensions, sizeFiller.Buffer(), structAddr, numOfElements*structBytesize);

            StreamString passedSpacer = parentSpacer;
            if (currNumOfChildren != parIdx + 1u) {
                passedSpacer += "┆ ";
            }
            else {
                passedSpacer += "  ";  // first space is a non-breaking space for size symmetry with ┆
            }

            Print(paddingLength, passedSpacer);
            rootStructure.MoveToAncestor(1u);
        }
        // numeric
        else {

            StreamString interfaceName = rootStructure.GetChildName(parIdx);

            // format the numeric name
            printName += interfaceName;
            while (printName.Size() < ( paddingLength + ((parentSpacer.Size() + spacer.Size())/2u) )) {
                printName += " ";
            }

            if (interfaceName == "__Dimensions__") {
                // nothing to do
            } else if (interfaceName == "_padding_") {
                uint32 paddingSize = 0u;
                if (rootStructure.Read(interfaceName.Buffer(), paddingSize)) {}
                REPORT_ERROR_STATIC(ErrorManagement::Information, "%s| void    | -    | -     | -                   | -                  | %d [pad]", printName.Buffer(), paddingSize);
            } else {
                uint32 interfaceIdx = 0u;
                if (rootStructure.Read(interfaceName.Buffer(), interfaceIdx)) {}
                SimulinkInterface* currentInterface = this->operator[](interfaceIdx);

                StreamString sizeFiller = "";
                if ((currentInterface->dimensions).GetNumberOfElements() < 3u) {
                    sizeFiller = "     ";
                }

                REPORT_ERROR_STATIC(ErrorManagement::Information, "%s| % -7s | % 4d | % 5d | % 4!%s | %p | %d", printName.Buffer(),
                    currentInterface->MARTeTypeName.Buffer(), currentInterface->numDimensions,
                    currentInterface->numElements, currentInterface->dimensions, sizeFiller.Buffer(), currentInterface->dataAddr, currentInterface->byteSize
                );
            }
        }
    }
}

uint32 SimulinkRootInterface::GetInterfaceBytesize(ConfigurationDatabase structureIn) {

    uint32 structureBytesize = 0u;

    for (uint32 parIdx = 0u; parIdx < structureIn.GetNumberOfChildren(); parIdx++) {

        // node
        if (structureIn.MoveToChild(parIdx)) {
            uint32 currentStructureBytesize = GetInterfaceBytesize(structureIn);
            uint32 numOfElements = 1u;
            AnyType arrayDescription = structureIn.GetType("__Dimensions__");
            if (arrayDescription.GetDataPointer() != NULL_PTR(void *)) {
                uint32  numOfDims = arrayDescription.GetNumberOfElements(0u);
                uint32* dimPtr = new uint32[numOfDims];
                dimensions = Vector<uint32>(dimPtr, numOfDims);
                if (structureIn.Read("__Dimensions__", dimensions)) {
                    for (uint32 dimIdx = 0u; dimIdx < numOfDims; dimIdx++) {
                        numOfElements *= dimensions[dimIdx];
                    }
                }
            }
            structureBytesize += numOfElements*currentStructureBytesize;
            if (structureIn.MoveToAncestor(1u)) {}
        }
        // leaf
        else {
            StreamString leafName = structureIn.GetChildName(parIdx);
            if (leafName == "__Dimensions__") {
                // nothing to do
            } else if (leafName == "_padding_") {
                uint32 paddingSize = 0u;
                if (structureIn.Read(leafName.Buffer(), paddingSize)) {}
                structureBytesize += paddingSize;
            } else {
                uint32 interfaceIdx = 0u;
                if (structureIn.Read(leafName.Buffer(), interfaceIdx)) {}

                SimulinkInterface* currentInterface = this->operator[](interfaceIdx);
                structureBytesize += currentInterface->byteSize;
            }
        }
    }

    return structureBytesize;
}



} /* namespace MARTe */
