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

#include "SimulinkClasses.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe
{

/*---------------------------------------------------------------------------*/
/*                              SimulinkDataI                                */
/*---------------------------------------------------------------------------*/
    
SimulinkDataI::SimulinkDataI() {
    
    fullName = "";
    dataClass = "Data";
    
    numberOfDimensions = 0u;
    for (uint16 dimIdx = 0u; dimIdx < maxNumOfDims; dimIdx++) {
        numberOfElements[dimIdx] = 1u;
    }
    totalNumberOfElements = 0u;
    orientation = rtwCAPI_SCALAR;
    
    byteSize     = 0u;
    dataTypeSize = 0u;
    offset       = 0u;
    
    cTypeName     = "void";
    MARTeTypeName = "void";
    type = InvalidType;
    
    address = NULL_PTR(void*);
    
    verbosity = 0u;
}

SimulinkDataI::~SimulinkDataI() {
    address = NULL_PTR(void*);
}

void SimulinkDataI::PrintData(const uint64 maxNameLength /* = 0u */, StreamString additionalText /* = "" */) {
    
    // Adds spaces at the end of the name until it reaches maxNameLength
    StreamString nameWithSpacesAtTheEnd = fullName;
    while (nameWithSpacesAtTheEnd.Size() < maxNameLength) {
        nameWithSpacesAtTheEnd += " ";
    }
    
    REPORT_ERROR_STATIC(ErrorManagement::Information,
        "%s %s │ dims (%-3u %-3u %-3u) │ elems %-5u │ type %-7s │ bytesize %-6u │ %s @%p",
        dataClass.Buffer(),
        nameWithSpacesAtTheEnd.Buffer(),
        numberOfElements[0u],
        numberOfElements[1u],
        numberOfElements[2u],
        totalNumberOfElements,
        MARTeTypeName.Buffer(),
        byteSize,
        additionalText.Buffer(),
        address
        );
}

/*---------------------------------------------------------------------------*/
/*                            SimulinkParameter                              */
/*---------------------------------------------------------------------------*/

bool SimulinkParameter::Actualise(const AnyType& sourceParameter) {
    
    bool ok;
    
    // Type coherence check
    TypeDescriptor slkType = TypeDescriptor::GetTypeDescriptorFromTypeName(MARTeTypeName.Buffer());
    TypeDescriptor extType = sourceParameter.GetTypeDescriptor();
    
    ok = (extType == slkType);
    
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Warning,
            "Parameter %s data type not matching (parameter source: %s, model: %s)",
            fullName.Buffer(),
            TypeDescriptor::GetTypeNameFromTypeDescriptor(extType),
            MARTeTypeName.Buffer());
    }
    
    // Type size coherence check
    if (ok) {
        
        uint32 slkTypeSize = dataTypeSize;
        uint32 extTypeSize = sourceParameter.GetByteSize();
        
        ok = (extTypeSize == slkTypeSize);
        
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning,
                "Parameter %s data type size not matching (parameter source: %d, model: %d)",
                fullName.Buffer(), extTypeSize, slkTypeSize);
        }
    }
    
    // Number of dimensions check (scalar, vector or matrix)
    if(ok) {
        
        ok = (sourceParameter.GetNumberOfDimensions() == numberOfDimensions);
        
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning,
                "Parameter %s number of dimensions not matching (parameter source: %d, model: %d)",
                fullName.Buffer(), sourceParameter.GetNumberOfDimensions(), numberOfDimensions);
        }
    }
    
    // Dimensionality check
    if(ok)
    {
        uint32 extDim1 = sourceParameter.GetNumberOfElements(0u);
        uint32 extDim2 = sourceParameter.GetNumberOfElements(1u);
        uint32 extDim3 = sourceParameter.GetNumberOfElements(2u);
        
        uint32 slkDim1 = numberOfElements[0u];
        uint32 slkDim2 = numberOfElements[1u];
        uint32 slkDim3 = numberOfElements[2u];
        
        // On the model side, scalar are [1,1], vectors are [1,N] or [N,1],
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
                    fullName.Buffer());
                break;
            
        }
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                "Parameter %s: dimensions not matching (parameter source: [%u, %u, %u], model: [%u, %u, %u]).",
                fullName.Buffer(),
                extDim1, extDim2, extDim3,
                slkDim1, slkDim2, slkDim3);
        }
    }

    // Total data size check
    if (ok) {
        
        ok = (sourceParameter.GetDataSize() == byteSize);
        
        if(!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning,
                "Parameter %s: total data size not matching (parameter source: %d, model: %d)",
                fullName.Buffer(), sourceParameter.GetDataSize(), byteSize);
        }
    }
    
    // Orientation check is not necessary: an AnyType is passed by, so
    // whatever generated this AnyType is in charge for granting that
    // the AnyType data buffer is in standard C++ row-major orientation.
    
    // Checks passed, data buffer can be copied from the input AnyType to the model memory
    if (ok) {
        
        if (numberOfDimensions <= 1u) {
            
            // Scalars and vectors have no orientation and can be copied as they are.
            ok = MemoryOperationsHelper::Copy(address, sourceParameter.GetDataPointer(), sourceParameter.GetDataSize());
            
        }
        else if (numberOfDimensions == 2u) {
            
            // For 2D matrices we handle the case in which model has column-major parameters.
            if (orientation == rtwCAPI_MATRIX_ROW_MAJOR) {
                ok = MemoryOperationsHelper::Copy(address, sourceParameter.GetDataPointer(), sourceParameter.GetDataSize());
            }
            else {
                ok = TransposeAndCopy(address, sourceParameter.GetDataPointer());
            }
        }
        else {
            
            // Also 3D matrices are memcopied, since handling all possible combinations of cases is not feasible
            ok = MemoryOperationsHelper::Copy(address, sourceParameter.GetDataPointer(), sourceParameter.GetDataSize());
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "3D matrix used. The GAM does not check data orientation of 3D matrices, carefully check results.");
        }
        
    }
    
    return ok;
}

bool SimulinkDataI::TransposeAndCopy(void *const destination, const void *const source) {
    
    bool ok = false;
    
    if (type==UnsignedInteger8Bit) {
        
        ok = TransposeAndCopyT<uint8>(destination, source);
        
    } else if (type==UnsignedInteger16Bit) {
        
        ok = TransposeAndCopyT<uint16>(destination, source);
        
    } else if (type==UnsignedInteger32Bit) {
        
        ok = TransposeAndCopyT<uint32>(destination, source);
        
    } else if (type==UnsignedInteger64Bit) {
        
        ok = TransposeAndCopyT<uint64>(destination, source);
        
    } else if (type==SignedInteger8Bit) {
        
        ok = TransposeAndCopyT<int8>(destination, source);
        
    } else if (type==SignedInteger16Bit) {
        
        ok = TransposeAndCopyT<int16>(destination, source);
        
    } else if (type==SignedInteger32Bit) {
        
        ok = TransposeAndCopyT<int32>(destination, source);
        
    } else if (type==SignedInteger64Bit) {
        
        ok = TransposeAndCopyT<int64>(destination, source);
        
    } else if (type==Float32Bit) {
        
        ok = TransposeAndCopyT<float32>(destination, source);
        
    } else if (type==Float64Bit) {
        
        ok = TransposeAndCopyT<float64>(destination, source);
        
    } else {
        
        REPORT_ERROR_STATIC(ErrorManagement::Exception,
                     "Unsupported type %s", TypeDescriptor::GetTypeNameFromTypeDescriptor(type));
    }
    
    return ok;
}

template<typename T>
bool SimulinkDataI::TransposeAndCopyT(void *const destination, const void *const source) {
    
    uint32 numberOfRows    = numberOfElements[0u];
    uint32 numberOfColumns = numberOfElements[1u];
    
    for (uint32 rowIdx = 0u; rowIdx < numberOfRows; rowIdx++) {
        
        for (uint32 colIdx = 0u; colIdx < numberOfColumns; colIdx++) {
            
            *( (T*) destination + colIdx + numberOfColumns*rowIdx ) = *( (T*) source + rowIdx + numberOfRows*colIdx );
            
        }
    }
    
    return true;
}

/*---------------------------------------------------------------------------*/
/*                              SimulinkSignal                               */
/*---------------------------------------------------------------------------*/

SimulinkSignal::SimulinkSignal() : SimulinkDataI() {
    
    dataClass = "Signal";
    
    MARTeAddress = NULL_PTR(void*);
    offset       = 0u;
    requiresTransposition = false;
}

void SimulinkSignal::PrintSignal(const uint64 maxNameLength /* = 0u */ ) {
    
    SimulinkDataI::PrintData(maxNameLength);
}

/*---------------------------------------------------------------------------*/
/*                               SimulinkPort                                */
/*---------------------------------------------------------------------------*/

SimulinkPort::SimulinkPort() : SimulinkSignal() {
    
    isValid      = true;
    isTyped      = false;
    isContiguous = true;
    hasHomogeneousType = true;
    hasHomogeneousOrientation = true;
    
    runningOffset = 0u;
    typeBasedSize = 0u;
    CAPISize      = 0u;
    offsetBasedSize = 0u;
    baseAddress       = NULL_PTR(void*);
    lastSignalAddress = NULL_PTR(void*);
    
    mode = InputSignals;
    
    requiresTransposition = false;
    
    dataClass = "Port";
    isStructured = false;
}

/*lint -e{1551} memory must be freed and functions called in the destructor are expected not to throw exceptions */
SimulinkPort::~SimulinkPort() {
    
    baseAddress       = NULL_PTR(void*);
    lastSignalAddress = NULL_PTR(void*);
    
    uint32 numberOfSignalsInThisPort = carriedSignals.GetSize();
    for (uint32 signalIdx = 0U; signalIdx < numberOfSignalsInThisPort; signalIdx++) {
        SimulinkSignal* toDelete;
        if(carriedSignals.Extract(0U, toDelete)) {
            delete toDelete;
        }
    }
}

bool SimulinkPort::AddSignal(SimulinkSignal* const signalIn) {
    
    bool ok = carriedSignals.Add(signalIn);
    
    if (ok) {
        uint64 totalNumOfElems = 1u;
        for (uint32 elemIdx = 0u; elemIdx < maxNumOfDims; elemIdx++) {
            totalNumOfElems *= signalIn->numberOfElements[elemIdx];
        }
        offsetBasedSize = (signalIn->offset) + ( (signalIn->dataTypeSize) * totalNumOfElems );
    }
    
    return ok;
}


void SimulinkPort::PrintPort(const uint64 maxNameLength) {
    
    StreamString typeStr = "";
    if (hasHomogeneousType) {
        typeStr = "homogeneous type";
    }
    else {
        typeStr = "mixed types     ";
    }
    
    StreamString additionalText;
    bool ok = additionalText.Printf(
        "size by type: %-6u │ size by offset %-6u │ %-16s │",
        typeBasedSize, offsetBasedSize, typeStr.Buffer()
        );
    if (!ok) {
        additionalText = "";
    }
    
    SimulinkDataI::PrintData(maxNameLength, additionalText);
    
}

SimulinkInputPort::SimulinkInputPort() : SimulinkPort() {
    
    dataClass = "IN  port";
}

SimulinkOutputPort::SimulinkOutputPort() : SimulinkPort() {
    
    dataClass = "OUT port";
}

SimulinkInputPort::~SimulinkInputPort() {

}

SimulinkOutputPort::~SimulinkOutputPort() {

}

bool SimulinkInputPort::CopyData(const SimulinkNonVirtualBusMode copyMode) {
    
    bool ok = true;
    
    // Copy signal content, telling apart the two modes (struct or byte array)
    // If address==NULL, this signal or port has no corresponding MARTe signal and thus is not mapped
    if (!requiresTransposition) {
        if( (copyMode == StructuredBusMode) && (isStructured) ) {
            
            for(uint32 carriedSignalIdx = 0u; (carriedSignalIdx < carriedSignals.GetSize()) && ok; carriedSignalIdx++) {
                if(carriedSignals[carriedSignalIdx]->MARTeAddress != NULL) {
                    ok = MemoryOperationsHelper::Copy(carriedSignals[carriedSignalIdx]->address, carriedSignals[carriedSignalIdx]->MARTeAddress, carriedSignals[carriedSignalIdx]->byteSize);
                }
            }
        }
        else {
            if(MARTeAddress != NULL) {
                ok = MemoryOperationsHelper::Copy(address, MARTeAddress, CAPISize);
            }
        }
    }
    else {
        if( (copyMode == StructuredBusMode) && (isStructured) ) {
            
            for(uint32 carriedSignalIdx = 0u; (carriedSignalIdx < carriedSignals.GetSize()) && ok; carriedSignalIdx++) {
                if(carriedSignals[carriedSignalIdx]->MARTeAddress != NULL) {
                    ok = TransposeAndCopy(carriedSignals[carriedSignalIdx]->address, carriedSignals[carriedSignalIdx]->MARTeAddress);
                }
            }
        }
        else {
            if(MARTeAddress != NULL) {
                ok = TransposeAndCopy(address, MARTeAddress);
            }
        }
    }
    
    return ok;
}

bool SimulinkOutputPort::CopyData(const SimulinkNonVirtualBusMode copyMode) {
    
    bool ok = true;

    // Copy signal content, telling apart the two modes (struct or byte array)
    // If address==NULL, this signal or port has no corresponding MARTe signal and thus is not mapped
    if (!requiresTransposition) {
        if( (copyMode == StructuredBusMode) && (isStructured) ) {
            
            for(uint32 carriedSignalIdx = 0u; (carriedSignalIdx < carriedSignals.GetSize()) && ok; carriedSignalIdx++) {
                
                if(carriedSignals[carriedSignalIdx]->MARTeAddress != NULL) {
                    ok = MemoryOperationsHelper::Copy(carriedSignals[carriedSignalIdx]->MARTeAddress, carriedSignals[carriedSignalIdx]->address, carriedSignals[carriedSignalIdx]->byteSize);
                }
            }
        }
        else {
            if(MARTeAddress != NULL) {
                ok = MemoryOperationsHelper::Copy(MARTeAddress, address, CAPISize);
            }
        }
    }
    else {
        if( (copyMode == StructuredBusMode) && (isStructured) ) {
            
            for(uint32 carriedSignalIdx = 0u; (carriedSignalIdx < carriedSignals.GetSize()) && ok; carriedSignalIdx++) {
                if(carriedSignals[carriedSignalIdx]->MARTeAddress != NULL) {
                    ok = TransposeAndCopy(carriedSignals[carriedSignalIdx]->MARTeAddress, carriedSignals[carriedSignalIdx]->address);
                }
            }
        }
        else {
            if(MARTeAddress != NULL) {
                ok = TransposeAndCopy(MARTeAddress, address);
            }
        }
    }
        
    return ok;
}


} /* namespace MARTe */
