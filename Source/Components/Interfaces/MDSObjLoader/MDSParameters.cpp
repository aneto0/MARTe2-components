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
#include "../MDSObjLoader/MDSParameters.h"

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
    
    // Modify path based upon options like Dim or StartIdx
    if ( targetDim != 0u && startIdx == 0u && stopIdx == 0u ) {
        StreamString indices;
        indices.Printf("[0:%u]", targetDim);
        MDSPath += indices;
        printf("startstop: %s\n", MDSPath.Buffer());
    }
    else if ( targetDim == 0u && startIdx != 0u && stopIdx != 0u ) {
        StreamString originalPath = MDSPath;
        MDSPath = "[";
        for (uint32 currIdx = startIdx; currIdx <= stopIdx; currIdx++) {
            //MDSPath.Printf(originalPath.Buffer(), currIdx);  // The MARTe::Printf function does not work as printf, thus the following is required:
            char stemp[CSTRINGMAXLEN];
            snprintf(stemp, sizeof(stemp), originalPath.Buffer(), currIdx);
            MDSPath += stemp;
            if (currIdx != stopIdx) {
                MDSPath += ", ";
            }
        }
        MDSPath += "]";
        printf("startstop: %s\n", MDSPath.Buffer());
    }
    else if ( targetDim != 0u && startIdx != 0u && stopIdx != 0u ) {
        // error
    }
    else {
        // do nothing, path is ok as it is
    }
    
    // If the path is empty, this parameter shall be skipped (unlinked parameter).
    if (this->IsStaticDeclared()) {
    
        try {
            
            #ifdef MDSDISTCLIENT
                MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
                MDSplus::Data *nodeData = node->getData();
            #endif
            #ifdef MDSTHINCLIENT
                MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
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

MDSParScalar::MDSParScalar() :
        MDSParameter() {
    //printf("MDSParScalar constructor called\n");
}

MDSParScalarInt::MDSParScalarInt() :
        MDSParameter() {
    //printf("MDSParScalarInt constructor called\n");
}

MDSParScalarBool::MDSParScalarBool() :
        MDSParameter() {
    //printf("MDSParScalarInt constructor called\n");
}

MDSParVector::MDSParVector() :
        MDSParameter() {
    //printf("MDSParVector constructor called\n");
}

MDSParVectorBool::MDSParVectorBool() :
        MDSParameter() {
    //printf("MDSParVector constructor called\n");
}

MDSParFixDimVector::MDSParFixDimVector() :
        MDSParameter() {
    //printf("MDSParFixDimVector constructor called\n");
    targetdim=0;
}

MDSParFixDimVectorInt::MDSParFixDimVectorInt() :
        MDSParameter() {
    //printf("MDSParFixDimVectorInt constructor called\n");
    targetdim=0;
}

MDSParFixDimVectorIdx::MDSParFixDimVectorIdx() :
        MDSParameter() {
    //printf("MDSParFixDimVectorIdx constructor called\n");
    startidx=1;
    stopidx=0;
    numelems=0;
}

MDSParMatrix::MDSParMatrix() :
        MDSParameter() {
    //printf("MDSParMatrix constructor called\n");
}

MDSParMatrixBool::MDSParMatrixBool() :
        MDSParameter() {
}

MDSPar3DMatrix::MDSPar3DMatrix() :
        MDSParameter() {
    //printf("MDSPar3DMatrix constructor called\n");
}

MDSParBusChecker::MDSParBusChecker() :
        MDSParameter() {
}

bool MDSParScalar::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);
    return ret;
}

bool MDSParScalarInt::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);
    return ret;
}

bool MDSParScalarBool::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);
    return ret;
}

bool MDSParVector::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);
    return ret;
}

bool MDSParVectorBool::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);
    return ret;
}

bool MDSParFixDimVector::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);

    if(ret) ret=data.Read("Dim", targetdim);
    if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParFixDimVector %s, Dim not set", this->GetName());

    return ret;
}

bool MDSParFixDimVectorInt::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);

    if(ret) ret=data.Read("Dim", targetdim);
    if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParFixDimVectorInt %s, Dim not set", this->GetName());

    return ret;
}

bool MDSParFixDimVectorIdx::Initialise(StructuredDataI &data) {
    bool ret=MDSParameter::Initialise(data);

    if(ret)
    {
        ret=data.Read("StartIdx", startidx);
        if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParFixDimVectorIdx %s, StartIdx not set", this->GetName());
    }

    if(ret)
    {
        ret=data.Read("StopIdx", stopidx);
        if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParFixDimVectorIdx %s, StopIdx not set", this->GetName());
    }

    if(ret)
    {
        ret = startidx<=stopidx;
        if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParFixDimVectorIdx %s, StartIdx must be <= that StopIdx", this->GetName());
    }

    return ret;
}

bool MDSParMatrix::Initialise(StructuredDataI &data) {
    bool ret = MDSParameter::Initialise(data);
    return ret;
}

bool MDSParMatrixBool::Initialise(StructuredDataI &data) {
    bool ret = MDSParameter::Initialise(data);
    return ret;
}

bool MDSPar3DMatrix::Initialise(StructuredDataI &data) {
    bool ret = MDSParameter::Initialise(data);
    return ret;
}

bool MDSParBusChecker::Initialise(StructuredDataI &data) {
    bool ret = MDSParameter::Initialise(data);

    if(ret)
    {
        ret=data.Read("Against", against);
        if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParBusChecker %s, Against not set", this->GetName());
    }

    if(ret)
    {
        ret=data.Read("NElems", nelems);
        if(!ret) REPORT_ERROR(ErrorManagement::ParametersError, "MDSParBusChecker %s, NElems not set", this->GetName());
    }

    return ret;
}


bool MDSParScalar::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    float retValue;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    bool mdsok = true;
    char stemp[CSTRINGMAXLEN];

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif
        
        retValue = nodeData->getFloat();
        
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }
    try {
        if(mdsok)
        {
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, value %f", me->GetName(), MDSPath.Buffer(), MARTePath.Buffer(), retValue);
            REPORT_ERROR(ErrorManagement::Information, stemp);

            // Additional checks
            datatypesize=sizeof(float);
            numdims=2;
            type="float";
            size=datatypesize;
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=1;
            dims[1]=1;
            orientation=rtwCAPI_SCALAR;
            databuffer=(void *)malloc(sizeof(float));
            //memcpy(databuffer,(void *)&retValue,sizeof(float));
            *(float *)databuffer=retValue;
            // parameter valid
            valid=true;
        }
    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }

    return ret;
}

bool MDSParScalarInt::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int retValue;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    bool mdsok = true;
    char stemp[CSTRINGMAXLEN];

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
       MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
       MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
       MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        retValue = nodeData->getInt();
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try{
        if(mdsok) {
        snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, value %d", me->GetName(), MDSPath.Buffer(), MARTePath.Buffer(), retValue);
        REPORT_ERROR(ErrorManagement::Information, stemp);

        // Additional checks
        datatypesize=sizeof(int);
        numdims=2;
        type="int";
        size=datatypesize;
        dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
        dims[0]=1;
        dims[1]=1;
        orientation=rtwCAPI_SCALAR;
        databuffer=(void *)malloc(sizeof(int));
        //memcpy(databuffer,(void *)&retValue,sizeof(float));
        *(int *)databuffer=retValue;
        // parameter valid
        valid=true;
        }
    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }

    return ret;
}

bool MDSParScalarBool::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    char retValue;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    bool mdsok = true;
    char stemp[CSTRINGMAXLEN];

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
       MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
       MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
       MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        retValue = nodeData->getByte();
    }

    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try{
        if(mdsok) {

        bool boolValue;
        StreamString printvalue;

        if(retValue != 0u && retValue != 1u)
        {
            std::string err = "Boolean can only have 0 or 1 value";
            throw err;
        }
        else
        {
            boolValue = retValue == 1u;
            if(boolValue)
                printvalue="TRUE";
            else
                printvalue="FALSE";
        }
        snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, value %s", me->GetName(), MDSPath.Buffer(), MARTePath.Buffer(), printvalue.Buffer());
        REPORT_ERROR(ErrorManagement::Information, stemp);

        // Additional checks
        datatypesize=sizeof(char);
        numdims=2;
        type="unsigned char"; // Booleans (or Logicals) are encoded as unsigned char by Simulink coder and by MDS+ as well
        size=datatypesize;
        dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
        dims[0]=1;
        dims[1]=1;
        orientation=rtwCAPI_MATRIX_COL_MAJOR;
        databuffer=(void *)malloc(sizeof(char));
        //memcpy(databuffer,(void *)&retValue,sizeof(float));
        *(char *)databuffer=retValue;
        // parameter valid
        valid=true;
        }
    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }

    return ret;
}

bool MDSParVector::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    float *retArray;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok = true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        
        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = nodeData->getFloatArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }


    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="]";

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=1)
            {
                errormsg="Number of dimensions is not 1";
                throw(errormsg);
            }

            datatypesize=sizeof(float);
            numdims=2;
            type="float";
            size=datatypesize*mdsdims[0];
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=mdsdims[0];
            dims[1]=1;
            orientation=rtwCAPI_MATRIX_COL_MAJOR;
            databuffer=(void *)malloc(size);

            // MDS+ stores matrices as row major oriented data
            // (see: http://www.mdsplus.org/index.php/Documentation:Tutorial:MdsObjects)
            // Simulink normally works opposite: i.e. col major (not all blocks
            // are compatible with row major code generation, notably matlab function blocks)
            // So we have to convert
            // No, that's not true, it seems that matrices stored on MDS from Matlab
            // are already in the good Simulink shape

            memcpy(databuffer,(void *)retArray,size);
            valid=true;
        }

    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }

    return ret;
}

bool MDSParVectorBool::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    unsigned char *retArray;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok = true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = (unsigned char *)nodeData->getByteArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }


    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="]";

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=1)
            {
                errormsg="Number of dimensions is not 1";
                throw(errormsg);
            }

            for(int ii=0; ii<numElements; ii++)
            {
                if(retArray[ii] != 0u && retArray[ii] != 1u)
                {
                    std::string err = "Boolean can only have 0 or 1 value";
                    throw err;
                }
            }

            datatypesize=sizeof(unsigned char);
            numdims=2;
            type="unsigned char";
            size=datatypesize*mdsdims[0];
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=mdsdims[0];
            dims[1]=1;
            orientation=rtwCAPI_MATRIX_COL_MAJOR;
            databuffer=(void *)malloc(size);

            // MDS+ stores matrices as row major oriented data
            // (see: http://www.mdsplus.org/index.php/Documentation:Tutorial:MdsObjects)
            // Simulink normally works opposite: i.e. col major (not all blocks
            // are compatible with row major code generation, notably matlab function blocks)
            // So we have to convert
            // No, that's not true, it seems that matrices stored on MDS from Matlab
            // are already in the good Simulink shape

            memcpy(databuffer,(void *)retArray,size);
            valid=true;
        }

    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }

    return ret;
}

bool MDSParFixDimVector::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    float *retArray;
    float *tmp1;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok = true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = nodeData->getFloatArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="], targetdim: ";
            snprintf(stemp, sizeof(stemp), "%d", targetdim);
            sctemp+=stemp;

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=1)
            {
                errormsg="Number of dimensions is not 1";
                throw(errormsg);
            }

            datatypesize=sizeof(float);
            numdims=2;
            type="float";
            size=datatypesize*targetdim;
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=targetdim;
            dims[1]=1;
            orientation=rtwCAPI_MATRIX_COL_MAJOR;

            databuffer=(void *)malloc(size);
            tmp1=(float *)databuffer;
            for(int ii=0; ii<targetdim; ii++) *tmp1++=0.0;

            if(numElements>targetdim)
                memcpy(databuffer,(void *)retArray,size);
            else
                memcpy(databuffer,(void *)retArray,numElements*sizeof(float));

            valid=true;
        }

    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }


    return ret;
}

bool MDSParFixDimVectorInt::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    int *retArray;
    int *tmp1;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok = true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = nodeData->getIntArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="], targetdim: ";
            snprintf(stemp, sizeof(stemp), "%d", targetdim);
            sctemp+=stemp;

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=1)
            {
                errormsg="Number of dimensions is not 1";
                throw(errormsg);
            }

            datatypesize=sizeof(int);
            numdims=2;
            type="int";
            size=datatypesize*targetdim;
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=targetdim;
            dims[1]=1;
            orientation=rtwCAPI_MATRIX_COL_MAJOR;

            databuffer=(void *)malloc(size);
            tmp1=(int *)databuffer;
            for(int ii=0; ii<targetdim; ii++) *tmp1++=0.0;

            if(numElements>targetdim)
                memcpy(databuffer,(void *)retArray,size);
            else
                memcpy(databuffer,(void *)retArray,numElements*sizeof(int));

            valid=true;
        }
    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }


    return ret;
}


bool MDSParFixDimVectorIdx::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    float retValue;
    float *tmpptr;
    std::string errormsg;
    std::string mdschannel;
    uint16 channelidx;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::size_t strfound;
    std::string sctemp;
    std::string backslash = "\\";
    bool mdsok = true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {

        numelems = stopidx-startidx+1;
        if(numelems<=0)
        {
            errormsg="Wrong number of elements";
            throw(errormsg);
        }

        databuffer=(void *)malloc((stopidx-startidx+1)*sizeof(float));
        if(databuffer==NULL)
        {
            errormsg="Could not allocate local memory for the parameter";
            throw(errormsg);
        }

        tmpptr=(float *)databuffer;

        for(channelidx=startidx; channelidx<=stopidx; channelidx++)
        {
            //mdschannel = MDS_string_format(csMDSPath, channelidx);

            size_t strsize = snprintf( nullptr, 0,  MDSPath.Buffer(), channelidx ) + 1; // Extra space for '\0', this calculate the size
            char * csMDSPath = (char *)new char[ strsize ];
            snprintf( csMDSPath, strsize, MDSPath.Buffer(), channelidx );
            //char * csMDSPath2 = (char *)new char[ strsize ];
            //snprintf( csMDSPath2, strsize, "%s", csMDSPath);

            sctemp = csMDSPath;
            strfound = sctemp.find("\\\\", 0);
            while(strfound!=std::string::npos)
            {
                sctemp.replace(strfound,2,backslash);
                strfound = sctemp.find("\\\\", 0);
            }
printf("path with idxes: %s\n", csMDSPath);
#ifdef MDSDISTCLIENT
            MDSplus::TreeNode *node = treeName->getNode(csMDSPath);
            MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
            MDSplus::Data *nodeData = conn->get(sctemp.c_str());
#endif

            retValue = nodeData->getFloat();

            *tmpptr = retValue;
            tmpptr++;
        }
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), 1, numelems);
            sctemp=stemp;
            sctemp+="]";

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());


            datatypesize=sizeof(float);
            numdims=2;
            type="float";
            size=datatypesize*numelems;
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=numelems;
            dims[1]=1;
            orientation=rtwCAPI_MATRIX_COL_MAJOR;
            //databuffer=(void *)malloc(size);
            //memcpy(databuffer,(void *)retArray,size);
            valid=true;
        }

    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }


    return ret;
}

bool MDSParMatrix::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    float *retArray;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok=true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }


    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = nodeData->getFloatArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="]";

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=2)
            {
                errormsg="Number of dimensions is not 2";
                throw(errormsg);
            }

            datatypesize=sizeof(float);
            numdims=mdsnumDim;
            type="float";
            size=datatypesize*mdsdims[0]*mdsdims[1];
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=mdsdims[0];
            dims[1]=mdsdims[1];
            orientation=rtwCAPI_MATRIX_COL_MAJOR;
            databuffer=(void *)malloc(size);

            // MDS+ stores matrices as row major oriented data
            // (see: http://www.mdsplus.org/index.php/Documentation:Tutorial:MdsObjects)
            // Simulink normally works opposite: i.e. col major (not all blocks
            // are compatible with row major code generation, notably matlab function blocks)
            // So we have to convert
            // No, that's not true, it seems that matrices stored on MDS from Matlab
            // are already in the good Simulink shape

            memcpy(databuffer,(void *)retArray,size);
            valid=true;
            /*
        targetArray=(float *)databuffer;
        for(mdscol=0; mdscol<mdsdims[0]; mdscol++)
            for(mdsrow=0; mdscol<mdsdims[1]; mdsrow++)
            {

            }
             */
        }

    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }


    return ret;
}

bool MDSParMatrixBool::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    unsigned char *retArray;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok=true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = (unsigned char *)nodeData->getByteArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="]";

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=2)
            {
                errormsg="Number of dimensions is not 2";
                throw(errormsg);
            }

            for(int ii=0; ii<numElements; ii++)
            {
                if(retArray[ii] != 0u && retArray[ii] != 1u)
                {
                    std::string err = "Boolean can only have 0 or 1 value";
                    throw err;
                }
            }

            datatypesize=sizeof(unsigned char);
            numdims=mdsnumDim;
            type="unsigned char";
            size=datatypesize*mdsdims[0]*mdsdims[1];
            dims=(uint32 *)malloc(2*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=mdsdims[0];
            dims[1]=mdsdims[1];
            orientation=rtwCAPI_MATRIX_COL_MAJOR;
            databuffer=(void *)malloc(size);

            // MDS+ stores matrices as row major oriented data
            // (see: http://www.mdsplus.org/index.php/Documentation:Tutorial:MdsObjects)
            // Simulink normally works opposite: i.e. col major (not all blocks
            // are compatible with row major code generation, notably matlab function blocks)
            // So we have to convert
            // No, that's not true, it seems that matrices stored on MDS from Matlab
            // are already in the good Simulink shape

            memcpy(databuffer,(void *)retArray,size);
            valid=true;
            /*
        targetArray=(float *)databuffer;
        for(mdscol=0; mdscol<mdsdims[0]; mdscol++)
            for(mdsrow=0; mdscol<mdsdims[1]; mdsrow++)
            {

            }
             */
        }

    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }


    return ret;
}


bool MDSPar3DMatrix::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);

    int numElements;
    int* mdsdims;
    int  mdsnumDim;
    float *retArray;
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    bool mdsok=true;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }

    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT
        MDSplus::Data *nodeData = conn->get(MDSPath.Buffer());
#endif

        mdsdims=nodeData->getShape(&mdsnumDim);
        retArray = nodeData->getFloatArray(&numElements);
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
        }
    }

    try {
        if(mdsok) {
            sctemp.empty();
            snprintf(stemp, sizeof(stemp), "%s actualize: %s -> %s, numDims: %d, [%d", me->GetName(),MDSPath.Buffer(), MARTePath.Buffer(), mdsnumDim, mdsdims[0]);
            sctemp+=stemp;
            for(int ii=1; ii<mdsnumDim; ii++)
            {
                snprintf(stemp, sizeof(stemp), ",%d", mdsdims[ii]);
                sctemp+=stemp;
            }
            sctemp+="]";

            REPORT_ERROR(ErrorManagement::Information, sctemp.c_str());

            if(mdsnumDim!=3)
            {
                errormsg="Number of dimensions is not 3";
                throw(errormsg);
            }

            datatypesize=sizeof(float);
            numdims=mdsnumDim;
            type="float";
            size=datatypesize*mdsdims[0]*mdsdims[1]*mdsdims[2];
            dims=(uint32 *)malloc(3*sizeof(uint32)); // Simulink allocates 2 dimensions with 1 element each for a scalar
            dims[0]=mdsdims[0];
            dims[1]=mdsdims[1];
            dims[2]=mdsdims[2];
            orientation=rtwCAPI_MATRIX_COL_MAJOR_ND;
            databuffer=(void *)malloc(size);

            // MDS+ stores matrices as row major oriented data
            // (see: http://www.mdsplus.org/index.php/Documentation:Tutorial:MdsObjects)
            // Simulink normally works opposite: i.e. col major (not all blocks
            // are compatible with row major code generation, notably matlab function blocks)
            // So we have to convert
            // No, that's not true, it seems that matrices stored on MDS from Matlab
            // are already in the good Simulink shape

            memcpy(databuffer,(void *)retArray,size);
            valid=true;
            /*
        targetArray=(float *)databuffer;
        for(mdscol=0; mdscol<mdsdims[0]; mdscol++)
            for(mdsrow=0; mdscol<mdsdims[1]; mdsrow++)
            {

            }
             */
        }
    }

    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
    }
    return ret;
}


bool MDSParBusChecker::Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *treeName, StreamString &baseClassName)
{
    bool ret = MDSParameter::Actualize(targetcdb, conn, treeName, baseClassName);
    std::string errormsg;
    const class ClassProperties* me;
    me=this->GetClassProperties();
    //char stemp[CSTRINGMAXLEN];
    std::string sctemp;
    char *mdsstring;
    bool mdsok=true;
    StreamString finalpath;

    if(unlinked)
    {
        REPORT_ERROR(ErrorManagement::Information, "%s actualize: UNLINKED -> %s, action ignored.", me->GetName(), MARTePath.Buffer());
        valid=true;
        return true;
    }


    try {
#ifdef MDSDISTCLIENT
        MDSplus::TreeNode *node = treeName->getNode(MDSPath.Buffer());
        MDSplus::Data *nodeData = node->getData();
#endif
#ifdef MDSTHINCLIENT


        finalpath="consolidate(";
        finalpath+=MDSPath;
        finalpath+=")";

        //printf("finalpath is%s\n", finalpath.Buffer());

        MDSplus::Data *nodeData = conn->get(finalpath.Buffer());
#endif

        //printf("Going to exec getShape\n");
        //mdsdims=nodeData->getShape(&mdsnumDim);


        mdsstring = nodeData->getString();
    }
    catch(MDSplus::MdsException &ex)
    {
        if(__MDSEXCHECK__)
        {
            REPORT_ERROR(ErrorManagement::Warning, "%s MDS+ error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), ex.what());
            mdsok=false;
            ret=false;
        }
    }

    try {
        if(mdsok){
            //printf("mdsstring is %s\n", mdsstring);
            //retArray = nodeData->getFloatArray(&numElements);
            //printf("MDSParBusChecker checking '%s' against cfg file entry", finalpath.Buffer());

            if(!strcmp(mdsstring, against.Buffer()))
            {
                REPORT_ERROR(ErrorManagement::Information, "MDSParBusChecker checking '%s' against cfg file PASSED.", finalpath.Buffer());
                //printf("passed\n");
            }
            else
            {
                REPORT_ERROR(ErrorManagement::ParametersError, "MDSParBusChecker checking '%s' against cfg file NOT PASSED!", finalpath.Buffer());
                ret=false;
                //printf("not passed\n");
            }



            //printf("numDims: %d, [%d", mdsnumDim, mdsdims[0]);
            //for(int ii=1; ii<mdsnumDim; ii++) printf(",%d", mdsdims[ii]);
            //printf("]\n");


            /*
        printf("matrix %s values:",MDSPath.Buffer());
        for(int ii=0; ii<numElements; ii++) printf(" %f",retArray[ii]);
        printf("\n");
             */
        }
    }
    catch(std::string &localex)
    {
        REPORT_ERROR(ErrorManagement::Warning, "%s local error getting node %s: %s, parameter invalidated.", me->GetName(), MDSPath.Buffer(), localex.c_str());
        valid=false;
        ret=false;
    }


    //exit(0);
    return ret;

}

MDSParameter::~MDSParameter() {
}

MDSParScalar::~MDSParScalar() {
}

MDSParScalarInt::~MDSParScalarInt() {
}

MDSParScalarBool::~MDSParScalarBool() {
}

MDSParVector::~MDSParVector() {
}

MDSParVectorBool::~MDSParVectorBool() {
}

MDSParFixDimVector::~MDSParFixDimVector() {
}

MDSParFixDimVectorInt::~MDSParFixDimVectorInt() {
}

MDSParFixDimVectorIdx::~MDSParFixDimVectorIdx() {
}

MDSParMatrix::~MDSParMatrix() {
}

MDSParMatrixBool::~MDSParMatrixBool() {
}

MDSPar3DMatrix::~MDSPar3DMatrix() {
}

MDSParBusChecker::~MDSParBusChecker() {
}

CLASS_REGISTER(MDSParameter, "1.0")

CLASS_REGISTER(MDSParScalar, "1.0")
CLASS_REGISTER(MDSParScalarInt, "1.0")
CLASS_REGISTER(MDSParScalarBool, "1.0")
CLASS_REGISTER(MDSParVector, "1.0")
CLASS_REGISTER(MDSParVectorBool, "1.0")
CLASS_REGISTER(MDSParFixDimVector, "1.0")
CLASS_REGISTER(MDSParFixDimVectorInt, "1.0")
CLASS_REGISTER(MDSParFixDimVectorIdx, "1.0")
CLASS_REGISTER(MDSParMatrix, "1.0")
CLASS_REGISTER(MDSParMatrixBool, "1.0")
CLASS_REGISTER(MDSPar3DMatrix, "1.0")
CLASS_REGISTER(MDSParBusChecker, "1.0")



}






