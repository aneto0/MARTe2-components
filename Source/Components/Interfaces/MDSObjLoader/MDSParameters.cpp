/**
 * @file MDSParameters.cpp
 * @brief Source file for class MDSParameters
 * @date 29/05/2019
 * @author Galperti Cristian
 *
 * @copyright Copyright 2019 EPFL SPC Lausanne
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
 * the class MDSParameters (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MDSParameters.h"

#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSParameter::MDSParameter() :
        ObjParameter() {
    sourceParamOrientation = None;
    targetDim           = 0u;
}

MDSParameter::~MDSParameter() {
}

bool MDSParameter::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName) {
    
    StreamString parameterName = this->GetName();
    MARTePath  = baseClassName;
    MARTePath += ".";
    MARTePath += parameterName;

    bool ok = targetcdb.Write(parameterName.Buffer(), MARTePath.Buffer());
    
    // Introspection informations from MDSplus will be stored here
    char8 MDSDataClass;        // Scalar (0) or array (4), see mdsdescrip.h
    char8 MDSDataType;
    char8 MDSNumOfDims;
    int16 MDSDataByteSize;
    int*  MDSDimArray;
    void* MDSDataPtr;
    
    StreamString expandedMDSPath;
    
    // Modify path if Dim option is specified
    if ( targetDim != 0u && startIdx == 0u && stopIdx == 0u ) {
        
        // Addes indices to the path in the form "[startIdx:stopIdx]"
        StreamString nodeWithIndices;
        StreamString indices;
        
        indices.Printf("[0:%u]", targetDim);
        nodeWithIndices  = MDSPath;
        nodeWithIndices += indices;
        
        // Now use TDI syntax to append zeroes if indices exceed array dimension
        StreamString tdiExpr = ""
            "_swgTargetDim = %u;"                                       // store targetDim in MDSplus
            "_swgVec       = %s;"                                       // store the current vector with indices
            "_swgVecSize   = shape(_swgVec, 0);"                        // calculate actual vector dimensions
            "if(_swgTargetDim > _swgVecSize)"                           // if vector size is less than required...
            "    for(_i = 0; _i < _swgTargetDim - _swgVecSize; _i++)"
            "        _swgVec = [_swgVec, 0];"                           // ... fill up with zeroes
            "_swgVec";                                                  // return the updated vector as result of this expression
        
        expandedMDSPath.Printf(tdiExpr.Buffer(), targetDim, nodeWithIndices.Buffer());
    }
    // Modify path if StartIdx and StopIdx options are specified
    else if ( targetDim == 0u && (startIdx != 0u || stopIdx != 0u) ) {
        
        // Concatenate scalar values in an array in the form "[\DATA001, \DATA002, \DATA003, ...]"
        expandedMDSPath = "[";
        for (uint32 currIdx = startIdx; currIdx <= stopIdx; currIdx++) {
            StreamString currNode;
            currNode.Printf(MDSPath.Buffer(), currIdx);
            expandedMDSPath += currNode;
            if (currIdx != stopIdx) {
                expandedMDSPath += ", ";
            }
        }
        expandedMDSPath += "]";
        printf("dim: %s\n", expandedMDSPath.Buffer());
    }
    // Error
    else if ( targetDim != 0u && startIdx != 0u && stopIdx != 0u ) {
        REPORT_ERROR(ErrorManagement::ParametersError, "MDSParameter %s: both Dim and StartIdx/StopIdx used, unsupported.", this->GetName());
        ok = false;
    }
    // Path is ok as it is
    else {
        expandedMDSPath = MDSPath;
    }
    
    // If the path is empty, this parameter shall be skipped (unlinked parameter).
    if (this->IsStaticDeclared()) {
    
        try {
            
            #ifdef MDSDISTCLIENT
                MDSplus::TreeNode *node = treeName->getNode(expandedMDSPath.Buffer());
                MDSplus::Data *nodeData = node->getData();
            #endif
            #ifdef MDSTHINCLIENT
                MDSplus::Data *nodeData = conn->get(expandedMDSPath.Buffer());
            #endif
            
            // MDSplus C++ API are used to retrieve informations about the parameter
            nodeData->getInfo(&MDSDataClass, &MDSDataType, &MDSDataByteSize, &MDSNumOfDims, &MDSDimArray, &MDSDataPtr);
            
            ok = true;
            
        }
        catch (MDSplus::MdsException &ex) {
            
            if (__MDSEXCHECK__) {
                
                REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", this->GetClassProperties()->GetName(), MDSPath.Buffer(), ex.what());
            }
            ok = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Warning, "Parameter %s unlinked, value not stored.", parameterName.Buffer());
    }
    
    // More than 3 dimensions are not supported.
    if (this->IsStaticDeclared() && ok) {
        
        ok = (MDSNumOfDims <= 3);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "%s not stored: number of dimensions > 3. Failing.", parameterName.Buffer());
        }
    }
    
    // Informations are stored using AnyType APIs.
    StreamString MARTeType;
    StreamString ModelType;
    
    if (this->IsStaticDeclared() && ok) {
        
        this->SetNumberOfDimensions(MDSNumOfDims);
        for (uint16 dimIdx = 0u; dimIdx < MDSNumOfDims; dimIdx++) {
            this->SetNumberOfElements(dimIdx, MDSDimArray[dimIdx]);
        }
        
        if (MDSDataType == 2u) {
            MARTeType = "uint8";
            ModelType = "unsigned char";
        }
        else if (MDSDataType == 3u) {
            MARTeType = "uin16";
            ModelType = "unsigned short";
        }
        else if (MDSDataType == 4u) {
            MARTeType = "uint32";
            ModelType = "unsigned int";
        }
        else if (MDSDataType == 5u) {
            MARTeType = "uint64";
            ModelType = "unsigned int";
        }
        else if (MDSDataType == 6u) {
            MARTeType = "int8";
            ModelType = "char";
        }
        else if (MDSDataType == 7u) {
            MARTeType = "int16";
            ModelType = "short";
        }
        else if (MDSDataType == 8u) {
            MARTeType = "int32";
            ModelType = "int";
        }
        else if (MDSDataType == 9u) {
            MARTeType = "int64";
            ModelType = "int";
        }
        else if (MDSDataType == 10u || MDSDataType == 52u) {
            MARTeType = "float32";
            ModelType = "float";
        }
        else if (MDSDataType == 11u || MDSDataType == 53u) {
            MARTeType = "float64";
            ModelType = "double";
        }
        else if (MDSDataType == 14u) {
            MARTeType = "string";
            ModelType = "string";
        }
        
        this->SetTypeDescriptor(TypeDescriptor::GetTypeDescriptorFromTypeName(MARTeType.Buffer()));
    }
    
    StreamString additionalInfo;
    
    // If the source parameter is a matrix, orientation must be specified.
    if (this->IsStaticDeclared() && ok) {
        
        if ( (this->GetNumberOfDimensions() == 2u) && (sourceParamOrientation == None) ) {
            MDSOrientation = "ColumnMajor";
            sourceParamOrientation = ColumnMajor;
            additionalInfo += " [WARNING: No data orientation specified, using ColumnMajor]";
        }
        
    }
    
    // Allocate AnyType data buffer and copy data from MDSplus
    if (this->IsStaticDeclared() && ok) {
        
        dataBuffer = HeapManager::Malloc(this->GetDataSize());
        ok = (dataBuffer != NULL_PTR(void*));
        
        if (ok) {
            
            if (this->GetNumberOfDimensions() <= 1u) {
                
                // Scalars and vectors have no orientation and can be copied as they are.
                ok = MemoryOperationsHelper::Copy(dataBuffer, MDSDataPtr, this->GetDataSize());
                
            }
            else if (this->GetNumberOfDimensions() == 2u) {
                
                // For 2D matrices we handle the case in which model has column-major parameters.
                if (sourceParamOrientation == RowMajor) {
                    ok = MemoryOperationsHelper::Copy(dataBuffer, MDSDataPtr, this->GetDataSize());
                }
                else {
                    ok = TransposeAndCopy(this->GetTypeDescriptor(), dataBuffer, MDSDataPtr);
                }
            }
            else {
                
                // Also 3D matrices are memcopied, since handling all possible combinations of cases is not feasible
                ok = MemoryOperationsHelper::Copy(dataBuffer, MDSDataPtr, this->GetDataSize());
                additionalInfo += " [WARNING: 3D matrix, data orientation not checked].";
            }
            
        }
        
        if (ok) {
            this->SetDataPointer(dataBuffer);   
        }
        
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "%s stored: %s → %s%s", this->GetClassProperties()->GetName(), MDSPath.Buffer(), MARTePath.Buffer(), additionalInfo.Buffer());
        }
    }
    
    return ok; // TODO failing to find the parameter should not result in Initialise failure
}

bool MDSParameter::Initialise(StructuredDataI &data) {


    bool ret = ObjParameter::Initialise(data);
    
    
    if (ret) {
        ret = data.Read("Path", MDSPath);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MDSParameter %s: Path not set.", this->GetName());
        }
    }
    
    /// 1. Read the path
    // An empty source path means an unlinked parameter, so the dataBuffer pointer
    // won't point to a valid memory location. If requested, the parameter actualization will be
    // ignored without errors and the compile-time parameter value will be used.
    if (ret) {
        if(MDSPath.Size() == 0u) {
            this->SetStaticDeclared(false);
            
            //TODO redundand
            unlinked=true;
        }
        else {
            this->SetStaticDeclared(true);
            
            //TODO redundant
            unlinked=false;
        }
    }
    
    /// 2. Read the orientation specification
    // (user must know the orientation of the data coming from MDSplus)
    if (ret) {
        bool orientationDeclared = data.Read("DataOrientation", MDSOrientation);
        if (orientationDeclared) {
            if (MDSOrientation == "RowMajor") {
                sourceParamOrientation = RowMajor;
            }
            else if (MDSOrientation  == "ColumnMajor") {
                sourceParamOrientation = ColumnMajor;
            }
            else {
                REPORT_ERROR(ErrorManagement::Warning, "MDSParameter %s: %s is not a valid orientation.", this->GetName(), MDSOrientation.Buffer());
                sourceParamOrientation = None;
            }
        }
        
    }
    
    /// 3. Read `Dim` if declared
    if (ret) {
        bool targetDimDeclared = data.Read("Dim", targetDim);
        if (targetDimDeclared) {
            printf("DIM: %u\n", targetDim);
        }
        
    }
    
    /// 4. Read `StartIdx` and `StopIdx` if declared
    if (ret) {
        bool startIdxDeclared = data.Read("StartIdx", startIdx);
        if (startIdxDeclared) {
            if (data.Read("StopIdx", stopIdx)) {
                printf("STARTIDX: %u, STOPIDX: %u\n", startIdx, stopIdx);
            }
            REPORT_ERROR(ErrorManagement::Warning, "MDSParameter %s: declared StartIdx but not StopIdx, option ignored.", this->GetName());
        }
        
    }
    
    return ret;
}

bool MDSParameter::TransposeAndCopy(TypeDescriptor type, void *const destination, const void *const source) {
    
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
        
        REPORT_ERROR(ErrorManagement::Exception,
                     "Unsupported type.");
        
    }
    
    return ok;
}

template<typename T>
bool MDSParameter::TransposeAndCopyT(void *const destination, const void *const source) {
    
    uint32 numberOfRows    = GetNumberOfElements(0u);
    uint32 numberOfColumns = GetNumberOfElements(1u);
    
    for (uint32 rowIdx = 0u; rowIdx < numberOfRows; rowIdx++) {
        
        for (uint32 colIdx = 0u; colIdx < numberOfColumns; colIdx++) {
            
            *( (T*) destination + colIdx + numberOfColumns*rowIdx ) = *( (T*) source + rowIdx + numberOfRows*colIdx );
            
        }
    }
    
    return true;
}

CLASS_REGISTER(MDSParameter, "1.0")

} /* namespace MARTe */

