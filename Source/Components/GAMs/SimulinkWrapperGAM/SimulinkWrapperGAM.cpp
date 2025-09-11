/**
 * @file SimulinkWrapperGAM.cpp
 * @brief Source file for class SimulinkWrapperGAM
 * @date 14/03/2019
 * @author Bertrand Bauvir
 * @author Cristian Galperti 
 * @author Nicolo Ferron 
 * @author Giuseppe Avon
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
 * the class SimulinkWrapperGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "LoadableLibrary.h"
#include "StructuredDataI.h"
#include "SimulinkWrapperGAM.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

// /*lint -e{788} lint is confused with the enum types*/
// static MARTe::StreamString GetOrientationName(const rtwCAPI_Orientation  &ELEorientation)
// {
//     MARTe::StreamString name;
//
//     switch(ELEorientation)
//     {
//     case rtwCAPI_SCALAR:
//         name = "scalar";
//         break;
//     case rtwCAPI_VECTOR:
//         name = "vector";
//         break;
//     case rtwCAPI_MATRIX_ROW_MAJOR:
//         name = "matrix row major";
//         break;
//     case rtwCAPI_MATRIX_COL_MAJOR:
//         name = "matrix col major";
//         break;
//     case rtwCAPI_MATRIX_COL_MAJOR_ND:
//         name = "matrix col major nd";
//         break;
// #ifdef ROW_MAJOR_ND_FEATURE
//     case rtwCAPI_MATRIX_ROW_MAJOR_ND:
//         name = "matrix row major nd";
//         break;
// #endif
//     default:
//         name = "N/A";
//         break;
//     }
//
//     return name;
// }



namespace MARTe {

/**
 * @brief To implement a lookup table between C type names and MARTe type names.
 */
struct TypeMap {
    /**
     * the character in the printf format
     */
    const char8* cTypeName;

    /**
     * the set of flags
     */
    const char8* MARTeTypeName;
};

/**
 * @brief Lookup table between C type names and MARTe type names.
 */
static const TypeMap typeLookUpTable[] = {

    {"unsigned char"  , "uint8"          },
    {"signed char"    , "int8"           },
    {"char"           , "int8"           },
    {"unsigned short" , "uint16"         },
    {"short"          , "int16"          },
    {"unsigned int"   , "uint32"         },
    {"int"            , "int32"          },
    {"float"          , "float32"        },
    {"double"         , "float64"        },
    {NULL_PTR(char8*) , NULL_PTR(char8*) }

};

/**
 * @brief To implement a lookup table between C type names and type sizes.
 */
struct SizeMap {
    /**
     * the character in the printf format
     */
    const char8* cTypeName;

    /**
     * the set of flags
     */
    uint16 size;
};

/**
 * @brief Lookup table between C type names and type sizes.
 */
static const SizeMap sizeLookUpTable[] = {
    
    {"unsigned char"  , 1u   },
    {"signed char"    , 1u   },
    {"char"           , 1u   },
    {"unsigned short" , 2u   },
    {"short"          , 2u   },
    {"unsigned int"   , 4u   },
    {"int"            , 4u   },
    {"float"          , 4u   },
    {"double"         , 8u   },
    {NULL_PTR(char8*) , 0u   }
    
};

static inline const char8* GetMARTeTypeNameFromCTypeName(const char8* const cTypeNameIn) {

    uint32 lookupIdx = 0u;

    while (typeLookUpTable[lookupIdx].cTypeName != NULL) {
        if ( StringHelper::Compare(typeLookUpTable[lookupIdx].cTypeName, cTypeNameIn) == 0 ) {
            break;
        }
        lookupIdx++;
    }

    const char8* MARTeTypeNameOut = typeLookUpTable[lookupIdx].MARTeTypeName;

    return MARTeTypeNameOut;
}


static inline const char8* GetCTypeNameFromMARTeTypeName(const char8* const MARTeTypeNameIn) {

    uint32 lookupIdx = 0u;

    while (typeLookUpTable[lookupIdx].MARTeTypeName != NULL) {
        if ( StringHelper::Compare(typeLookUpTable[lookupIdx].MARTeTypeName, MARTeTypeNameIn) == 0 ) {
            break;
        }
        lookupIdx++;
    }

    const char8* CTypeNameOut = typeLookUpTable[lookupIdx].cTypeName;

    return CTypeNameOut;
}


static inline uint16 GetTypeSizeFromCTypeName(const char8* const cTypeNameIn) {

    uint32 lookupIdx   = 0u;

    while (sizeLookUpTable[lookupIdx].cTypeName != NULL) {
        if ( StringHelper::Compare(sizeLookUpTable[lookupIdx].cTypeName, cTypeNameIn) == 0 ) {
            break;
        }
        lookupIdx++;
    }

    uint16 typeSizeOut = sizeLookUpTable[lookupIdx].size;

    return typeSizeOut;
}

/*lint -e{40} -e{9129} -e{30} -e{142} lint is confused with the enum types*/
static inline StreamString GetMARTeTypeNameFromEnumeratedTypes(const uint8 slEnumTypeIn) {
    
    MARTe::StreamString MARTeTypeNameOut = "void";
    
    switch(slEnumTypeIn)
    {
    case SS_DOUBLE:
        MARTeTypeNameOut = "float64";
        break;
    case SS_SINGLE:
        MARTeTypeNameOut = "float32";
        break;
    case SS_INT8:
        MARTeTypeNameOut = "int8";
        break;
    case SS_UINT8:
        MARTeTypeNameOut = "uint8";
        break;
    case SS_INT16:
        MARTeTypeNameOut = "int16";
        break;
    case SS_UINT16:
        MARTeTypeNameOut = "uint16";
        break;
    case SS_INT32:
        MARTeTypeNameOut = "int32";
        break;
    case SS_UINT32:
        MARTeTypeNameOut = "uint32";
        break;
    default:
        MARTeTypeNameOut = "void";
        break;
    }
    
    return MARTeTypeNameOut;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SimulinkWrapperGAM::SimulinkWrapperGAM()
        : GAM()
        , MessageI() {

    libraryHandle = NULL_PTR(LoadableLibrary*);

    instFunction  = static_cast<void*(*)(void)>(NULL);
    initFunction  = static_cast<void(*)(void*)>(NULL);
    stepFunction  = static_cast<void(*)(void*)>(NULL);
    termFunction  = static_cast<void(*)(void*)>(NULL);

    getMmiFunction = static_cast<void*(*)(void*)>(NULL);
    getAlgoInfoFunction  = static_cast<void(*)(void*)>(NULL);

    modelParams = NULL_PTR(rtwCAPI_ModelParameters*);
    rootInputs  = NULL_PTR(rtwCAPI_Signals*);
    rootOutputs = NULL_PTR(rtwCAPI_Signals*);
    logSignals  = NULL_PTR(rtwCAPI_Signals*);
    dataTypeMap = NULL_PTR(rtwCAPI_DataTypeMap*);
    elementMap  = NULL_PTR(rtwCAPI_ElementMap*);
    dimMap      = NULL_PTR(rtwCAPI_DimensionMap*);
    dimArray    = NULL_PTR(uint32*);
    dataAddrMap = NULL_PTR(void**);

    states = NULL_PTR(void*);

    verbosityLevel       = 0u;
    modelNumOfInputs     = 0u;
    modelNumOfOutputs    = 0u;
    modelNumOfSignals    = 0u;
    modelNumOfParameters = 0u;

    skipInvalidTunableParams = true;
    paramsHaveStructArrays   = false;
    isInputMapped.SetSize(0u);
    isOutputMapped.SetSize(0u);

    nonVirtualBusMode             = ByteArrayBusMode;
    enforceModelSignalCoverage    = false;

    inputs  = NULL_PTR(SimulinkRootInterface*);
    outputs = NULL_PTR(SimulinkRootInterface*);
    params  = NULL_PTR(SimulinkRootInterface*);
    signals = NULL_PTR(SimulinkRootInterface*);

    status = ErrorManagement::NoError;
}

/*lint -e{1551, 1559} Justification: no exceptions thrown */
SimulinkWrapperGAM::~SimulinkWrapperGAM() {

    if (libraryHandle != NULL) {
        libraryHandle->Close();
        delete libraryHandle;
        libraryHandle = static_cast<LoadableLibrary*>(NULL);
    }

    instFunction = static_cast<void*(*)(void)>(NULL);
    initFunction = static_cast<void(*)(void*)>(NULL);
    stepFunction = static_cast<void(*)(void*)>(NULL);
    termFunction  = static_cast<void(*)(void*)>(NULL);
    
    getMmiFunction       = static_cast<void*(*)(void*)>(NULL);
    getAlgoInfoFunction  = static_cast<void(*)(void*)>(NULL);
    
    modelParams = NULL_PTR(rtwCAPI_ModelParameters*);
    rootInputs  = NULL_PTR(rtwCAPI_Signals*);
    rootOutputs = NULL_PTR(rtwCAPI_Signals*);
    logSignals  = NULL_PTR(rtwCAPI_Signals*);
    dataTypeMap = NULL_PTR(rtwCAPI_DataTypeMap*);
    elementMap  = NULL_PTR(rtwCAPI_ElementMap*);
    dimMap      = NULL_PTR(rtwCAPI_DimensionMap*);
    dimArray    = NULL_PTR(uint32*);
    dataAddrMap = NULL_PTR(void**);
    
    states = NULL_PTR(void*);
    
    isInputMapped.SetSize(0u);
    isOutputMapped.SetSize(0u);

    if (inputs != NULL_PTR(SimulinkRootInterface*)) {
        delete[] inputs;
        inputs = NULL_PTR(SimulinkRootInterface*);
    }
    if (outputs != NULL_PTR(SimulinkRootInterface*)) {
        delete[] outputs;
        outputs = NULL_PTR(SimulinkRootInterface*);
    }
    if (params != NULL_PTR(SimulinkRootInterface*)) {
        delete[] params;
        params = NULL_PTR(SimulinkRootInterface*);
    }
    if (signals != NULL_PTR(SimulinkRootInterface*)) {
        delete[] signals;
        signals = NULL_PTR(SimulinkRootInterface*);
    }
}

bool SimulinkWrapperGAM::Initialise(StructuredDataI &data) {
 
    bool ok = GAM::Initialise(data);

    /// The method performs the following actions:
    
    /// 1. Retrieval of mandatory configurations (library name, symbol prefix...)
    ///    from configuration file.
    
    // Library name parameter
    if (ok) {
        ok = data.Read("Library", libraryName);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as Library parameter.", libraryName.Buffer());
        }
        else {
            status.parametersError = true;
            REPORT_ERROR(status, "Error getting Library parameter.");
        }
    }

    // SymbolPrefix parameter
    if (ok) {
        ok = data.Read("SymbolPrefix", symbolPrefix);
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as SymbolPrefix parameter.", symbolPrefix.Buffer());
    }
        else
        {
            status.parametersError = true;
            REPORT_ERROR(status, "Error getting SymbolPrefix parameter.");
        }
    }

    // Tunable parameters source name
    if (ok) {
        bool isExternalSpecified = data.Read("TunableParamExternalSource", tunableParamExternalSource);
        if (isExternalSpecified) {
            REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as TunableParamExternalSource parameter.", tunableParamExternalSource.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "No TunableParamExternalSource declared.");
        }
    }
    
    // Verbosity parameter
    if (ok) {
        // Simulink CAPI verbosity level
        if(data.Read("Verbosity", verbosityLevel)) {
            REPORT_ERROR(ErrorManagement::Information, "GAM verbosity set to %d.", verbosityLevel);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "GAM verbosity not set, by default it is set to 0.");
        }
        
        uint16 itemp;
        if(data.Read("SkipInvalidTunableParams", itemp)) {
            skipInvalidTunableParams = (itemp != 0u);
            if (verbosityLevel > 1u) {
                REPORT_ERROR(ErrorManagement::Information, "SkipInvalidTunableParams set to %d.", itemp);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "SkipInvalidTunableParams not set, by default it is set to true.");
        }
    }
    
    if (ok) {
        StreamString copyModeString = "";
        if ( data.Read("NonVirtualBusMode", copyModeString) ) {
            if (copyModeString == "Structured") {
                nonVirtualBusMode = StructuredBusMode;
            }
            else if (copyModeString == "ByteArray") {
                nonVirtualBusMode = ByteArrayBusMode;
            }
            else {
                ok = false;
                status.parametersError = true;
                REPORT_ERROR(status, "Invalid NonVirtualBusMode: %s (can be ByteArray or Structured).", copyModeString.Buffer());
            }
            
            if (ok) {
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ErrorManagement::Information, "NonVirtualBusMode mode set to %s", copyModeString.Buffer());
                }
            }
        }
        else {
            if (verbosityLevel > 1u) {
                REPORT_ERROR(ErrorManagement::Information, "NonVirtualBusMode mode not set, by default it is set to ByteArray");
            }
        }
        
    }
    
    //Check if Simulink signals must be completely mapped on the GAM
    if(ok) {
        uint32 tempSigCoverage = 0u;
        if(data.Read("EnforceModelSignalCoverage", tempSigCoverage)) {
            if(tempSigCoverage > 0u) {
                enforceModelSignalCoverage = true;
            }
            else {
                enforceModelSignalCoverage = false;
            }
        }
        else {
            enforceModelSignalCoverage = false;
        }
        if (verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "GAM I/O %s cover Simulink I/O",
                        enforceModelSignalCoverage ? "must" : "has not to");
        }
    }

    /// 2. Opening of model code shared object library.
    
    if (ok) {
        ok = (static_cast<LoadableLibrary*>(NULL) == libraryHandle);
    }

    if (ok) { // Load library
        libraryHandle = new LoadableLibrary();
    }

    if ((libraryHandle != NULL) && ok) { // Load library
        ok = libraryHandle->Open(libraryName.Buffer());
        if (ok) {
            if (verbosityLevel > 0u) {
                REPORT_ERROR(ErrorManagement::Information, "Library %s successfully loaded.", libraryName.Buffer());
            }
        }
        else {
            status.initialisationError = true;
            REPORT_ERROR(status, "Couldn't open library: %s", libraryName.Buffer());
        }
    }
    
    /// 3. Retrieval of model code symbols.
    
    char8 symbol[64u];
    
    /*lint -e{611} the pointer returned by LoadableLibrary::Function() can be safely casted to a pointer to function */
    if ((libraryHandle != NULL) && ok) {
        // instFunction
        ok = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u); // Compose symbol

        if (ok) { // Find symbol
            instFunction = reinterpret_cast<void*(*)(void)>(libraryHandle->Function(&symbol[0u]));
            ok = (static_cast<void*(*)(void)>(NULL) != instFunction);
            if (!ok) {
                status.initialisationError = true;
                REPORT_ERROR(status, "Couldn't find %s symbol in model library (instFunction == NULL).", symbol);
            }
        }

        // getMmiFunction
        if (ok) { // Compose symbol
            ok = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (ok) {
                ok = StringHelper::ConcatenateN(&symbol[0u], "_GetCAPImmi", 64u);
            }
        }

        if (ok) { // Find symbol
            getMmiFunction = reinterpret_cast<void*(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            ok = (static_cast<void*(*)(void*)>(NULL) != getMmiFunction);
            if (!ok) {
                status.initialisationError = true;
                REPORT_ERROR(status, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
            }
        }

        // initFunction
        if (ok) { // Compose symbol
            ok = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (ok) {
                ok = StringHelper::ConcatenateN(&symbol[0u], "_initialize", 64u);
            }
        }

        if (ok) { // Find symbol
            initFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            ok = (static_cast<void(*)(void*)>(NULL) != initFunction);
            if (!ok) {
                status.initialisationError = true;
                REPORT_ERROR(status, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
            }
        }
        
        // stepFunction
        if (ok) { // Compose symbol
            ok = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (ok) {
                ok = StringHelper::ConcatenateN(&symbol[0u], "_step", 64u);
            }
        }

        if (ok) { // Find symbol
            stepFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            ok = (static_cast<void(*)(void*)>(NULL) != stepFunction);
            if (!ok) {
                status.initialisationError = true;
                REPORT_ERROR(status, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
            }
        }

        // getAlgoInfoFunction
        if (ok) { // Compose symbol
            ok = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (ok) {
                ok = StringHelper::ConcatenateN(&symbol[0u], "_GetAlgoInfo", 64u);
            }
        }
        
        if (ok) { // Find symbol
            getAlgoInfoFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            ok = (static_cast<void(*)(void*)>(NULL) != getAlgoInfoFunction);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::Information, "Algorithm information not found in the Simulink .so");
                ok = true;
            }
        }
    }

    /// 4. Build a reference container containing parameter values
    ///    retrieved in the configuration file (under the `Parameters` node).
    
    if (ok) {
        ReferenceT<ReferenceContainer> cfgParameterContainerTemp("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cfgParameterContainer = cfgParameterContainerTemp;
        cfgParameterContainer->SetName("CfgParameterContainer");

        ok = cfgParameterContainer.IsValid();

        if (ok) {
            ok = ObjectRegistryDatabase::Instance()->Insert(cfgParameterContainer);
        }

        bool hasParametersNode = data.MoveRelative("Parameters");

        // traverse the `Parameters` node iteratively to avoid recursion
        /*lint -e{423, 429} Justification: the while loop condition ensures that all the allocated objects are freed */
        if (hasParametersNode && ok) {

            ConfigurationDatabase parametersCdb;
            if (data.Copy(parametersCdb)) {}

            StaticList<StreamString*> nodeStack;

            // add root node to the stack
            StreamString* currentNodePtr = new StreamString("");
            if (nodeStack.Add(currentNodePtr)) {}

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
                    // is leaf: add this node to cfgParameterContainer
                    else {
                            // substitute any dash `-` with dots `.` for retrocompatibility
                        while (currentNodePath.Locate("-") != -1) {
                            int32 dashIdx = currentNodePath.Locate("-");
                            (currentNodePath.BufferReference())[dashIdx] = '.';
                        }

                        ReferenceT<AnyObject> cfgParameterReference("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                        AnyType cfgParameter = parametersCdb.GetType(parametersCdb.GetChildName(elemIdx));
                        cfgParameterReference->SetName(currentNodePath.Buffer());
                        ok = cfgParameterReference->Serialise(cfgParameter);
                        if(ok) {
                            ok = cfgParameterContainer->Insert(currentNodePath.Buffer(), cfgParameterReference);
                        }
                    }
                }
            }
        }

        if (hasParametersNode) {
            ok = data.MoveToAncestor(1u);
        }

        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "[%s] - Failed to create parameter database from Parameters node in configuration file.", GetName());
        }
    }

    return ok;
}

bool SimulinkWrapperGAM::Setup() {
    

    isInputMapped.SetSize(GetNumberOfInputSignals());
    isOutputMapped.SetSize(GetNumberOfOutputSignals());
    for (uint32 sigIdx = 0u; sigIdx < GetNumberOfInputSignals(); sigIdx++) {
        isInputMapped[sigIdx] = false;
    }
    for (uint32 sigIdx = 0u; sigIdx < GetNumberOfOutputSignals(); sigIdx++) {
        isOutputMapped[sigIdx] = false;
    }

    status = SetupSimulink();
    // Call init method
    // This has to be called after tunable parameters actualization
    // to correctly handle tunable parameters dependent inits
    // (see f4e example)

    if (status.ErrorsCleared()) {
        if (verbosityLevel > 0u) {
            REPORT_ERROR(ErrorManagement::Information, "[%s] - Setup done, now init-ing the Simulink model", GetName());
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "[%s] - SetupSimulink() failed.", GetName());
    }

    // Simulink initFunction call, init of the Simulink model
    if (status.ErrorsCleared()) {
        (*initFunction)(states);
    }
    
    // Send simulink ready message
    if (status.ErrorsCleared()) {
        ReferenceT<Message> simulinkReadyMessage = Get(0u);
        if (simulinkReadyMessage.IsValid()) {
            if (verbosityLevel > 1u) {
                REPORT_ERROR(ErrorManagement::Information, "[%s] - Sending Simulink ready message 1.", GetName());
            }
            if(!SendMessage(simulinkReadyMessage, this)) {
                REPORT_ERROR(ErrorManagement::Warning, "[%s] - Failed to send ready message 1.", GetName());
            }
        }
    }
    return status.ErrorsCleared();
}

ErrorManagement::ErrorType SimulinkWrapperGAM::SetupSimulink() {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;
    
    if (verbosityLevel > 1u) {
        REPORT_ERROR(ErrorManagement::Information, "[%s] - Allocating Simulink model dynamic memory...", GetName());
    }

    // Simulink instFunction call, dynamic allocation of model data structures
    if (instFunction != NULL) {
        states = (*instFunction)();
    }
    
    ret.exception = (states == NULL);
    if (bool(ret.exception)) {
        REPORT_ERROR(ret, "[%s] - Simulink model allocation function returned a NULL data pointer", GetName());
    }

    // Get the Model Mapping Information (mmi) data structure from the Simulink shared object
    rtwCAPI_ModelMappingInfo* mmi = NULL_PTR(rtwCAPI_ModelMappingInfo*);
    if (ret.ErrorsCleared()) {
        if ( getMmiFunction != NULL ) {
            void *mmiTemp = ((*getMmiFunction)(states));
            mmi = reinterpret_cast<rtwCAPI_ModelMappingInfo*>(mmiTemp);
        }

        ret.exception = (mmi == NULL);
        if (bool(ret.exception)) {
            REPORT_ERROR(ret, "[%s] - GetMmiPtr function returned a NULL data pointer", GetName());
        }
    }
    
    if (ret.ErrorsCleared() && (mmi != NULL)) {

        modelNumOfInputs     = rtwCAPI_GetNumRootInputs(mmi);
        modelNumOfOutputs    = rtwCAPI_GetNumRootOutputs(mmi);
        modelNumOfParameters = rtwCAPI_GetNumModelParameters(mmi);
        modelNumOfSignals    = rtwCAPI_GetNumSignals(mmi);
        
        if (verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "[%s] - Simulink C API version number: %d", GetName(), mmi->versionNum);
        }
    }


    ///-------------------------------------------------------------------------
    /// 1. Retrieve model inputs/outputs/params
    ///-------------------------------------------------------------------------

    // Populating C API data structure pointers of the class from mmi
    if (ret.ErrorsCleared() && (mmi != NULL)) {
        rootInputs  = rtwCAPI_GetRootInputs(mmi);       ret.exception = ( ret.exception && (rootInputs  == NULL) );
        rootOutputs = rtwCAPI_GetRootOutputs(mmi);      ret.exception = ( ret.exception && (rootOutputs == NULL) );
        modelParams = rtwCAPI_GetModelParameters(mmi);  ret.exception = ( ret.exception && (modelParams == NULL) );
        logSignals  = rtwCAPI_GetSignals(mmi);          ret.exception = ( ret.exception && (logSignals  == NULL) );

        dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);      ret.exception = ( ret.exception && (dataTypeMap == NULL) );
        elementMap  = rtwCAPI_GetElementMap(mmi);       ret.exception = ( ret.exception && (elementMap  == NULL) );
        dimMap      = rtwCAPI_GetDimensionMap(mmi);     ret.exception = ( ret.exception && (dimMap      == NULL) );
        dimArray    = rtwCAPI_GetDimensionArray(mmi);   ret.exception = ( ret.exception && (dimArray    == NULL) );
        dataAddrMap = rtwCAPI_GetDataAddressMap(mmi);   ret.exception = ( ret.exception && (dataAddrMap == NULL) );

        if (bool(ret.exception)) {
            REPORT_ERROR(ret, "[%s] - Pointer to one of the model maps is NULL.", GetName());
        }
    }

    // populate inputs/outputs/params lists
    /*lint -e{613} Justification: no null pointers here*/
    if (ret.ErrorsCleared()) {
        inputs  = new SimulinkRootInterface[modelNumOfInputs];
        outputs = new SimulinkRootInterface[modelNumOfOutputs];
        params  = new SimulinkRootInterface[modelNumOfParameters];
        signals = new SimulinkRootInterface[modelNumOfSignals];

        ret = ScanInterfaces(inputs,  rootInputs,  modelNumOfInputs,     InputPort);
        if (!ret) {
            REPORT_ERROR(ret, "[%s] - Failed ScanInterfaces for input signals", GetName());
        }
        if (ret.ErrorsCleared()) {
            ret = ScanInterfaces(outputs, rootOutputs, modelNumOfOutputs,    OutputPort);
            if (!ret) {
                REPORT_ERROR(ret, "[%s] - Failed ScanInterfaces for output signals", GetName());
            }
        }
        if (ret.ErrorsCleared()) {
            ret = ScanInterfaces(params,  modelParams, modelNumOfParameters, Parameter);
            if (!ret) {
                REPORT_ERROR(ret, "[%s] - Failed ScanInterfaces for parameters", GetName());
            }
        }
        if (ret.ErrorsCleared()) {
            ret = ScanInterfaces(signals,  logSignals, modelNumOfSignals, Signal);
            if (!ret) {
                REPORT_ERROR(ret, "[%s] - Failed ScanInterfaces for logging signals", GetName());
            }
        }

        ret.exception = (inputs == NULL) || (outputs == NULL) || (signals == NULL) || (params == NULL);
    }


    ///-------------------------------------------------------------------------
    /// 2. Print the retrieved info
    ///-------------------------------------------------------------------------

    // Max length of parameter names for this model is computed.
    uint64       maxNameLength = 25u;
    StreamString currentName;
    uint64       currentNameSize;

    uint32 numOfFlattenedInputs  = 0u;
    uint32 numOfFlattenedOutputs = 0u;
    uint32 numOfFlattenedParams  = 0u;
    uint32 numOfFlattenedSignals = 0u;

    if (ret.ErrorsCleared() && (inputs != NULL) && (outputs != NULL) && (signals != NULL) && (params != NULL) ) {
        for(uint32 paramIdx = 0u; paramIdx < modelNumOfParameters; paramIdx++) {
            for (uint32 elemIdx = 0u; elemIdx < params[paramIdx].GetSize(); elemIdx++) {
                currentName     = params[paramIdx][elemIdx]->fullPath;
                currentNameSize = currentName.Size();
                if (maxNameLength < currentNameSize) {
                    maxNameLength = currentNameSize;
                }
                numOfFlattenedParams++;
            }
        }
        for(uint32 inputIdx = 0u; inputIdx < modelNumOfInputs; inputIdx++) {
            for (uint32 elemIdx = 0u; elemIdx < inputs[inputIdx].GetSize(); elemIdx++) {
                currentName     = inputs[inputIdx][elemIdx]->fullPath;
                currentNameSize = currentName.Size();
                if (maxNameLength < currentNameSize) {
                    maxNameLength = currentNameSize;
                }
                numOfFlattenedInputs++;
            }
        }
        for(uint32 outputIdx = 0u; outputIdx < modelNumOfOutputs; outputIdx++) {
            for (uint32 elemIdx = 0u; elemIdx < outputs[outputIdx].GetSize(); elemIdx++) {
                currentName     = outputs[outputIdx][elemIdx]->fullPath;
                currentNameSize = currentName.Size();
                if (maxNameLength < currentNameSize) {
                    maxNameLength = currentNameSize;
                }
                numOfFlattenedOutputs++;
            }
        }
        for(uint32 signalIdx = 0u; signalIdx < modelNumOfSignals; signalIdx++) {
            for (uint32 elemIdx = 0u; elemIdx < signals[signalIdx].GetSize(); elemIdx++) {
                currentName     = signals[signalIdx][elemIdx]->fullPath;
                currentNameSize = currentName.Size();
                if (maxNameLength < currentNameSize) {
                    maxNameLength = currentNameSize;
                }
                numOfFlattenedSignals++;
            }
        }
        maxNameLength = maxNameLength + 4u;
    }


    if (ret.ErrorsCleared() && (verbosityLevel > 0u) && (inputs != NULL) && (outputs != NULL) && (signals != NULL) && (params != NULL)) {

        StreamString fillerWhiteSpaces    = "";
        StreamString fillerEqualSign      = "";
        StreamString fillerEqualSignSmall = "";
        StreamString fillerMinusSignSmall = "";

        while (fillerWhiteSpaces.Size() < maxNameLength) {
            fillerWhiteSpaces += " ";
            fillerEqualSign   += "=";
        }
        StreamString currentElemName = GetName();
        while (fillerEqualSignSmall.Size() < currentElemName.Size()) {
            fillerEqualSignSmall += "=";
            fillerMinusSignSmall += "-";
        }

        StreamString header = "";
        StreamString horizontalLine = "";
        StreamString sectionTitle = "";

        ret.exception = !header.Printf( "%s| type    | dims | elems | shape               | model address      | bytesize", fillerWhiteSpaces.Buffer());
        horizontalLine = "+---------+------+-------+---------------------+--------------------+------------------";

        REPORT_ERROR(ErrorManagement::Information, "========================================= [%s] - MODEL INTERFACES =========================%s", GetName(), fillerEqualSign.Buffer());
        REPORT_ERROR(ErrorManagement::Information, "%s", header.Buffer());

        // print root inputs
        if (ret.ErrorsCleared() && (modelNumOfInputs > 0u)) {
            ret.exception = !sectionTitle.Printf("--- ROOT INPUTS (%d) ----", modelNumOfInputs);
            while (sectionTitle.Size() < maxNameLength) {
                sectionTitle += "-";
            }
            REPORT_ERROR(ErrorManagement::Information, "%s%s%s", sectionTitle.Buffer(), horizontalLine.Buffer(), fillerMinusSignSmall.Buffer());
            for (uint32 rootParIdx = 0u; rootParIdx < modelNumOfInputs; rootParIdx++) {
                inputs[rootParIdx].Print(maxNameLength);
            }
            sectionTitle = "";
        }

        // print root outputs
        if (ret.ErrorsCleared() && (modelNumOfOutputs > 0u)) {
            ret.exception = !sectionTitle.Printf("--- ROOT OUTPUTS (%d) ---", modelNumOfOutputs);
            while (sectionTitle.Size() < maxNameLength) {
                sectionTitle += "-";
            }
            REPORT_ERROR(ErrorManagement::Information, "%s%s%s", sectionTitle.Buffer(), horizontalLine.Buffer(), fillerMinusSignSmall.Buffer());
            for (uint32 rootParIdx = 0u; rootParIdx < modelNumOfOutputs; rootParIdx++) {
                outputs[rootParIdx].Print(maxNameLength);
            }
            sectionTitle = "";
        }

        // print logging signals
        if (ret.ErrorsCleared() && (modelNumOfSignals > 0u)) {
            ret.exception = !sectionTitle.Printf("--- LOGGING SIGNALS (%d) ", modelNumOfSignals);
            while (sectionTitle.Size() < maxNameLength) {
                sectionTitle += "-";
            }
            REPORT_ERROR(ErrorManagement::Information, "%s%s%s", sectionTitle.Buffer(), horizontalLine.Buffer(), fillerMinusSignSmall.Buffer());
            for (uint32 rootParIdx = 0u; rootParIdx < modelNumOfSignals; rootParIdx++) {
                signals[rootParIdx].Print(maxNameLength);
            }
            sectionTitle = "";
        }

        // print parameters
        if (ret.ErrorsCleared() && (modelNumOfParameters > 0u)) {
            ret.exception = !sectionTitle.Printf("--- ROOT PARAMETERS (%d) ", modelNumOfParameters);
            while (sectionTitle.Size() < maxNameLength) {
                sectionTitle += "-";
            }
            REPORT_ERROR(ErrorManagement::Information, "%s%s%s", sectionTitle.Buffer(), horizontalLine.Buffer(), fillerMinusSignSmall.Buffer());
            for (uint32 rootParIdx = 0u; rootParIdx < modelNumOfParameters; rootParIdx++) {
                params[rootParIdx].Print(maxNameLength);
            }
            sectionTitle = "";
        }

        if (verbosityLevel < 2u) {
            REPORT_ERROR(ErrorManagement::Information, "=======================================================================================%s%s", fillerEqualSignSmall.Buffer(), fillerEqualSign.Buffer());
        }
        else {
            sectionTitle = "";

            REPORT_ERROR(ErrorManagement::Information, "==================================== [%s] - FLATTENED MODEL INTERFACES ====================%s", GetName(), fillerEqualSign.Buffer());

            // printf flattened inputs
            if (ret.ErrorsCleared() && (modelNumOfInputs > 0u)) {
                ret.exception = !sectionTitle.Printf("--- FLATTENED INPUTS (%d) ----", numOfFlattenedInputs);
                while (sectionTitle.Size() < (87u + fillerEqualSign.Size() + fillerEqualSignSmall.Size())) {
                    sectionTitle += "-";
                }
                REPORT_ERROR(ErrorManagement::Information, "%s", sectionTitle.Buffer());
                for (uint32 rootIntIdx = 0u; rootIntIdx < modelNumOfInputs; rootIntIdx++) {
                    if (inputs[rootIntIdx].isStructured) {
                        for (uint32 intIdx = 0u; intIdx < inputs[rootIntIdx].GetSize(); intIdx++) {
                            REPORT_ERROR(ErrorManagement::Information, "▪ %s", inputs[rootIntIdx][intIdx]->fullPath.Buffer());
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::Information, "▪ %s", inputs[rootIntIdx].fullPath.Buffer());
                    }
                }
                sectionTitle = "";
            }

            // print flattened outputs
            if (ret.ErrorsCleared() && (modelNumOfOutputs > 0u)) {
                ret.exception = !sectionTitle.Printf("--- FLATTENED OUTPUTS (%d) ---", numOfFlattenedOutputs);
                while (sectionTitle.Size() < (87u + fillerEqualSign.Size() + fillerEqualSignSmall.Size())) {
                    sectionTitle += "-";
                }
                REPORT_ERROR(ErrorManagement::Information, "%s", sectionTitle.Buffer());
                for (uint32 rootIntIdx = 0u; rootIntIdx < modelNumOfOutputs; rootIntIdx++) {
                    if (outputs[rootIntIdx].isStructured) {
                        for (uint32 intIdx = 0u; intIdx < outputs[rootIntIdx].GetSize(); intIdx++) {
                            REPORT_ERROR(ErrorManagement::Information, "▪ %s", outputs[rootIntIdx][intIdx]->fullPath.Buffer());
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::Information, "▪ %s", outputs[rootIntIdx].fullPath.Buffer());
                    }
                }
                sectionTitle = "";
            }

            // print flattened logging signals
            if (ret.ErrorsCleared() && (modelNumOfSignals > 0u)) {
                ret.exception = !sectionTitle.Printf("--- FLATTENED SIGNALS (%d) ---", numOfFlattenedSignals);
                while (sectionTitle.Size() < (87u + fillerEqualSign.Size() + fillerEqualSignSmall.Size())) {
                    sectionTitle += "-";
                }
                REPORT_ERROR(ErrorManagement::Information, "%s", sectionTitle.Buffer());
                for (uint32 rootIntIdx = 0u; rootIntIdx < modelNumOfSignals; rootIntIdx++) {
                    if (signals[rootIntIdx].isStructured) {
                        for (uint32 intIdx = 0u; intIdx < signals[rootIntIdx].GetSize(); intIdx++) {
                            REPORT_ERROR(ErrorManagement::Information, "▪ %s", signals[rootIntIdx][intIdx]->fullPath.Buffer());
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::Information, "▪ %s", signals[rootIntIdx].fullPath.Buffer());
                    }
                }
                sectionTitle = "";
            }

            // print flattened parameters
            if (ret.ErrorsCleared() && (modelNumOfParameters > 0u)) {
                ret.exception = !sectionTitle.Printf("--- FLATTENED PARAMETERS (%d) ", numOfFlattenedParams);
                while (sectionTitle.Size() < (87u + fillerEqualSign.Size() + fillerEqualSignSmall.Size())) {
                    sectionTitle += "-";
                }
                REPORT_ERROR(ErrorManagement::Information, "%s", sectionTitle.Buffer());
                for (uint32 rootIntIdx = 0u; rootIntIdx < modelNumOfParameters; rootIntIdx++) {
                    if (params[rootIntIdx].isStructured) {
                        for (uint32 intIdx = 0u; intIdx < params[rootIntIdx].GetSize(); intIdx++) {
                            REPORT_ERROR(ErrorManagement::Information, "▪ %s", params[rootIntIdx][intIdx]->fullPath.Buffer());
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::Information, "▪ %s", params[rootIntIdx].fullPath.Buffer());
                    }
                }
                sectionTitle = "";
            }

            REPORT_ERROR(ErrorManagement::Information, "=======================================================================================%s%s", fillerEqualSignSmall.Buffer(), fillerEqualSign.Buffer());
        }

    }

    ///-------------------------------------------------------------------------
    /// 3. Check root IO coherence between GAM and model
    ///    and map memory
    ///-------------------------------------------------------------------------


    if (ret.ErrorsCleared()) {
        //Ensures the number of I/O Ports must be the same as the Simulink Model when in plain mode
        if (nonVirtualBusMode == ByteArrayBusMode) {
            uint32 numberOfGAMInputSignals  = GetNumberOfInputSignals();
            uint32 numberOfGAMOutputSignals = GetNumberOfOutputSignals();

            // Check number of declared main ports

            ret.illegalOperation = (numberOfGAMInputSignals != modelNumOfInputs);
            if (bool(ret.illegalOperation)) {
                REPORT_ERROR(ret, "[%s] - Number of input signals mismatch (GAM: %u, model %u)",
                    GetName(), numberOfGAMInputSignals,  modelNumOfInputs);
            }

            if (ret.ErrorsCleared()) {
                ret.illegalOperation = ( (numberOfGAMOutputSignals < modelNumOfOutputs) || (numberOfGAMOutputSignals > (modelNumOfOutputs + modelNumOfSignals)) );
                if (bool(ret.illegalOperation)) {
                    REPORT_ERROR(ret, "[%s] - Number of output signals mismatch (GAM: %u, model %u)",
                        GetName(), numberOfGAMOutputSignals,  modelNumOfOutputs + modelNumOfSignals);
                }
            }
        }
    }


    if (ret.ErrorsCleared()) {
        ret = MapPorts(InputPort);
        if (!ret) {
            REPORT_ERROR(ret, "[%s] - Failed MapPorts() for input signals.", GetName());
        }
    }

    if (ret.ErrorsCleared()) {
        ret = MapPorts(OutputPort);
        if (!ret) {
            REPORT_ERROR(ret, "[%s] - Failed MapPorts() for output signals.", GetName());
        }
    }

    if (ret.ErrorsCleared()) {
        ret = MapPorts(Signal);
        if (!ret) {
            REPORT_ERROR(ret, "[%s] - Failed MapPorts() for logging signals.", GetName());
        }
    }

    // Check that every GAM signal is mapped to a model port or signal
    if (ret.ErrorsCleared()) {
        for (uint32 inputIdx = 0u; ret && (inputIdx < numberOfInputSignals); inputIdx++) {
            if (!isInputMapped[inputIdx]) {
                StreamString signalName = "";
                ret.internalSetupError = true;
                ret.exception = !GetSignalName(InputSignals, inputIdx, signalName);
                REPORT_ERROR(ret, "[%s] - GAM input signal %s not found in Simulink model",
                    GetName(), signalName.Buffer());
            }
        }
        for (uint32 outputIdx = 0u; ret.ErrorsCleared() && (outputIdx < numberOfOutputSignals); outputIdx++) {
            if (!isOutputMapped[outputIdx]) {
                StreamString signalName = "";
                ret.internalSetupError = true;
                ret.exception = !GetSignalName(OutputSignals, outputIdx, signalName);
                REPORT_ERROR(ret, "[%s] - GAM output signal %s not found in Simulink model",
                    GetName(), signalName.Buffer());
            }
        }
    }

    // Check that every Simulink port and signal is mapped to a GAM signal (optional, if EnforceModelSignalCoverage == true)
    if( (ret.ErrorsCleared()) && enforceModelSignalCoverage && (nonVirtualBusMode == StructuredBusMode) ) {

        if(verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "[%s] - Scanning for orphaned Simulink signals on %d ports ...", GetName(), modelNumOfInputs + modelNumOfOutputs);
        }

        StreamString disconnectedSignalName = "";

        for(uint32 portIdx = 0u; ret.ErrorsCleared() && (inputs != NULL) && (portIdx < modelNumOfInputs); portIdx++) {
            if (inputs[portIdx].isStructured) {
                uint32 portSize = inputs[portIdx].GetSize();
                for(uint32 elemIdx = 0u; ret.ErrorsCleared() && (elemIdx < portSize); elemIdx++) {
                    if(inputs[portIdx][elemIdx]->destPtr == NULL) {
                        ret.internalSetupError = true;
                        disconnectedSignalName = inputs[portIdx][elemIdx]->fullPath;
                    }
                }
            }
            else {
                if(inputs[portIdx].destPtr == NULL) {
                    ret.internalSetupError = true;
                    disconnectedSignalName = inputs[portIdx].fullPath;
                }
            }
        }

        for(uint32 portIdx = 0u; ret.ErrorsCleared() && (outputs != NULL) && (portIdx < modelNumOfOutputs); portIdx++) {
            if (outputs[portIdx].isStructured) {
                uint32 portSize = outputs[portIdx].GetSize();
                for(uint32 elemIdx = 0u; ret.ErrorsCleared() && (elemIdx < portSize); elemIdx++) {
                    if(outputs[portIdx][elemIdx]->destPtr == NULL) {
                        ret.internalSetupError = true;
                        disconnectedSignalName = outputs[portIdx][elemIdx]->fullPath;
                    }
                }
            }
            else {
                if(outputs[portIdx].destPtr == NULL) {
                    ret.internalSetupError = true;
                    disconnectedSignalName = outputs[portIdx].fullPath;
                }
            }
        }

        if (!ret) {
            REPORT_ERROR(ret, "[%s] - signal `%s` is disconnected (no matching MARTe2 signal). Not allowed when EnforceModelSignalCoverage is set to true",
                        GetName(), disconnectedSignalName.Buffer());
        }
    }


    ///-------------------------------------------------------------------------
    /// 4. Verify that the external parameter source (if any)
    ///    is compatible with the GAM
    ///-------------------------------------------------------------------------

    // The external parameter source must be a ReferenceContainer
    // populated by references to AnyObject.

    if (ret.ErrorsCleared()) {
        if (tunableParamExternalSource.Size() > 0u) {
            Reference mdsPar = ObjectRegistryDatabase::Instance()->Find(tunableParamExternalSource.Buffer());
            if (!mdsPar.IsValid()) {
                REPORT_ERROR(ErrorManagement::Warning, "[%s] - TunableParamExternalSource `%s` is not valid (wrong name, not declared in configuration file or not a ReferenceContainer).", GetName(), tunableParamExternalSource.Buffer());
            }
        }
    }

    ///-------------------------------------------------------------------------
    /// 5. Retrieve a value for parameter actualisation,
    ///    check parameter coherence between GAM and model
    ///    and Actualise()
    ///-------------------------------------------------------------------------

    // Loop over all tunable parameters found in the model and update their
    // value with what is stored in the AnyType that is passed to Actualise()
    
    AnyType sourceParameter;    // the source of data from which to actualise
    
    bool isLoaded;              // whether the parameter is available (i.e. was correctly loaded by the loader mechanism)
    bool isActualised;          // whether the parameter has been correctly actualized
    bool isUnlinked;            // special condition for a parameter from MDSplus whose path is empty. It shall be skipped even if skipInvalidTunableParams == 0
    
    StreamString parameterSourceName;

    for(uint32 paramIdx = 0u; ret.ErrorsCleared() && (paramIdx < modelNumOfParameters) && (params != NULL); paramIdx++) {

        uint32 parameterSize = params[paramIdx].GetSize();
        for (uint32 subParamIdx = 0u; ret.ErrorsCleared() && (subParamIdx < parameterSize); subParamIdx++) {
        
            isLoaded     = false;
            isActualised = false;
            isUnlinked   = false;

            // Retrieve the ReferenceT<AnyType> of the source parameter from which to actualise
            StreamString parameterPathInObjDatabase;

            const char8* currentParamName = (params[paramIdx][subParamIdx]->fullPath).Buffer();

            ReferenceT<AnyObject> sourceParameterPtr;

            // 1. Parameters from configuration file (highest priority).
            parameterPathInObjDatabase  = cfgParameterContainer->GetName();
            parameterPathInObjDatabase += ".";
            parameterPathInObjDatabase += currentParamName;
            sourceParameterPtr = ObjectRegistryDatabase::Instance()->Find(parameterPathInObjDatabase.Buffer());
            if (sourceParameterPtr.IsValid()) {
                parameterSourceName = "configuration file";
                isLoaded = true;
            }

            // 2. Parameters from loader class (2nd-highest priority)
            else {
                parameterPathInObjDatabase  = tunableParamExternalSource;
                parameterPathInObjDatabase += ".";
                parameterPathInObjDatabase += currentParamName;
                sourceParameterPtr = ObjectRegistryDatabase::Instance()->Find(parameterPathInObjDatabase.Buffer());
                if (sourceParameterPtr.IsValid()) {
                    parameterSourceName = "loader class";
                    isLoaded = true;
                }
            // 3. Parameter not found (if skipInvalidTunableParams, then use compile-time value)
                else {
                    isLoaded = false;
                }
            }


            // Data is copied from the source parameter to the model
            if (isLoaded) {

                isLoaded = sourceParameterPtr.IsValid();
                if (!isLoaded) {
                    REPORT_ERROR(ErrorManagement::CommunicationError,
                        "[%s] - Parameter %s: invalid reference from %s.", GetName(), currentParamName, parameterSourceName.Buffer()
                    );
                }

                if (isLoaded) {
                    sourceParameter = sourceParameterPtr->GetType();

                    if (sourceParameter.IsStaticDeclared()) {
                        isActualised = params[paramIdx][subParamIdx]->Actualise(sourceParameter);
                    }
                    else {
                        isUnlinked = true;
                    }
                }
            }

            // Add spaces to align the output
            StreamString alignedParamName = currentParamName;
            while ( alignedParamName.Size() < maxNameLength ) {
                alignedParamName += " ";
            }

            // Based upon the actualisation outcome, execution is continued or stopped.

            // Cases in which execution can continue
            if ( isLoaded && isActualised ) {
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ErrorManagement::Information,
                        "[%s] - Parameter %s correctly actualized from %s.",
                        GetName(), alignedParamName.Buffer(), parameterSourceName.Buffer());
                }
            }
            else if ( isLoaded && (!isActualised) && isUnlinked ) {
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ErrorManagement::Information,
                        "[%s] - Parameter %s unlinked, using compile time value",
                        GetName(), alignedParamName.Buffer());
                }
            }
            else if ( isLoaded && (!isActualised) && (!isUnlinked) && skipInvalidTunableParams ) {
                //ret.warning = true;
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ErrorManagement::Information,
                        "[%s] - Parameter %s cannot be actualized, using compile time value",
                        GetName(), alignedParamName.Buffer());
                }
            }
            else if ( (!isLoaded) && (!isActualised) && skipInvalidTunableParams ) {
                //ret.warning = true;
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ErrorManagement::Information,
                        "[%s] - Parameter %s not found, using compile time value",
                        GetName(), alignedParamName.Buffer());
                }
            }
            // Cases in which execution should be stopped
            else if ( (!isLoaded) && (!isActualised) && (!skipInvalidTunableParams) ) {
                ret.internalSetupError = true;
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ret,
                        "[%s] - Parameter %s not found, failing",
                        GetName(), alignedParamName.Buffer());
                }
            }
            else {
                ret.internalSetupError = true;
                if (verbosityLevel > 1u) {
                    REPORT_ERROR(ret,
                        "SkipInvalidTunableParams is false and parameter %s cannot be actualized, failing",
                        currentParamName);
                }
            }

        }
        
    }

    return ret;

}


bool SimulinkWrapperGAM::Execute() {

    uint32 portIdx;

    bool ok = (states != NULL) && (inputs != NULL) && (outputs != NULL) && (signals != NULL);

    // Inputs update
    for (portIdx = 0u; (portIdx < modelNumOfInputs) && (inputs != NULL) && ok; portIdx++) {
        ok = inputs[portIdx].CopyData(nonVirtualBusMode);
    }

    // Model step
    if ( (stepFunction != NULL) && ok) {
        (*stepFunction)(states);
    }

    // Outputs update
    for (portIdx = 0u; (portIdx < modelNumOfOutputs) && (outputs != NULL) && ok; portIdx++) {
        ok = outputs[portIdx].CopyData(nonVirtualBusMode);
    }
    
    for (portIdx = 0u; (portIdx < modelNumOfSignals) && (signals != NULL) && ok; portIdx++) {
        ok = signals[portIdx].CopyData(nonVirtualBusMode);
    }

    return ok;
}


/*lint -e{429} Justification: currentInterface is freed in the destructor */
ErrorManagement::ErrorType SimulinkWrapperGAM::ScanInterface(SimulinkRootInterface& interfaceArray, const void* const interfaceStruct, const uint32 sigIdx, const InterfaceType mode, void* const parentAddr /* = NULL*/, const StreamString parentName /* = "" */) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;
    ret.exception = (dataTypeMap == NULL) || (dimArray == NULL) || (dimMap == NULL) || (dataAddrMap == NULL);

    StreamString interfaceName  = "";
    StreamString fullPath       = "";

    uint16       typeIdx = 0u;
    uint16  dimensionIdx = 0u;
    uint32       addrIdx = 0u;
    uint32   dimArrayIdx = 0u;

    const char8* CTypeName = NULL_PTR(char8*);
    const char8* className = NULL_PTR(char8*);
    StreamString MARTeTypeName = "";
    uint8 SLIdType = 0u;
    uint8 enumSLId = 127u; // Invalid datatype
    uint8 dimNum = 0u;
    uint32 dataTypeSize = 0u;
    uint32 byteSize = 0u;
    uint32 numElements = 1u;
    uint32 numDimensions = 0u;
    Vector<uint32> dimensions;
    rtwCAPI_Orientation orientation = rtwCAPI_SCALAR;
    bool transpose = false;
    bool isStructured = false;
    void* dataAddr = NULL_PTR(void*);

    if (ret.ErrorsCleared() && (dataAddrMap != NULL)) {
        if ( (mode == InputPort) || (mode == OutputPort) || (mode == Signal) ) {
            const rtwCAPI_Signals* signalStruct = static_cast<const rtwCAPI_Signals*>(interfaceStruct);

            interfaceName = rtwCAPI_GetSignalName        (signalStruct, sigIdx);
            typeIdx       = rtwCAPI_GetSignalDataTypeIdx (signalStruct, sigIdx);
            dimensionIdx  = rtwCAPI_GetSignalDimensionIdx(signalStruct, sigIdx);
            addrIdx       = rtwCAPI_GetSignalAddrIdx     (signalStruct, sigIdx);
            dataAddr      = rtwCAPI_GetDataAddress       (dataAddrMap, addrIdx);

            fullPath = interfaceName;
        }
        else if (mode == Parameter) {
            const rtwCAPI_ModelParameters* paramStruct = static_cast<const rtwCAPI_ModelParameters*>(interfaceStruct);

            interfaceName = rtwCAPI_GetModelParameterName        (paramStruct, sigIdx);
            typeIdx       = rtwCAPI_GetModelParameterDataTypeIdx (paramStruct, sigIdx);
            dimensionIdx  = rtwCAPI_GetModelParameterDimensionIdx(paramStruct, sigIdx);
            addrIdx       = rtwCAPI_GetModelParameterAddrIdx     (paramStruct, sigIdx);
            dataAddr      = rtwCAPI_GetDataAddress               (dataAddrMap, addrIdx);

            fullPath = interfaceName;
        }
        else if (mode == Element) {
            const rtwCAPI_ElementMap* elementStruct = elementMap;
            ret.exception = (elementStruct == NULL) || (parentAddr == NULL);

            if ( (!ret.exception) && (elementStruct != NULL) && (parentAddr != NULL) ) {
                interfaceName = rtwCAPI_GetElementName(elementStruct, sigIdx);
                typeIdx       = rtwCAPI_GetElementDataTypeIdx (elementStruct, sigIdx);
                dimensionIdx  = rtwCAPI_GetElementDimensionIdx(elementStruct, sigIdx);
                dataAddr      = static_cast<uint8*>(parentAddr) + rtwCAPI_GetElementOffset(elementStruct, sigIdx); //lint !e9016 Justification: pointer arithmetic is required by the Simulink C-APIs

                fullPath += parentName;
                fullPath += ".";
                fullPath += interfaceName;
            }
        }
        else {
            ret.unsupportedFeature = true;
            REPORT_ERROR(ret, "[%s] - Invalid mode", GetName());
        }
    }

    if (ret.ErrorsCleared()) {
        ret.illegalOperation = (interfaceName == "");
        if (!ret) {
            REPORT_ERROR(ret, "[%s] - In %s: Interface name is an empty string. Not allowed since interface mapping is name-based.",
                         GetName(), (fullPath.Size() == 0u) ? "root" : fullPath.Buffer());
        }
    }

    if ( ret.ErrorsCleared() && (dataTypeMap != NULL) && (dimMap != NULL) ) {
        CTypeName    = rtwCAPI_GetDataTypeCName (dataTypeMap, typeIdx);
        className    = rtwCAPI_GetDataTypeMWName(dataTypeMap, typeIdx);
        SLIdType     = rtwCAPI_GetDataTypeSLId  (dataTypeMap, typeIdx);
        dataTypeSize = rtwCAPI_GetDataTypeSize  (dataTypeMap, typeIdx);
        dimNum       = rtwCAPI_GetNumDims       (dimMap, dimensionIdx);
        dimArrayIdx  = rtwCAPI_GetDimArrayIndex (dimMap, dimensionIdx);
        orientation  = rtwCAPI_GetOrientation   (dimMap, dimensionIdx);
#ifdef ENUM_FEATURE
        enumSLId     = rtwCAPI_GetDataEnumStorageType(dataTypeMap, typeIdx); // Add enum support only if available (from version 2019a onwards)
#endif


#ifdef ENUM_FEATURE
        /*lint -e{1924, 9117} SS_ENUM_TYPE is defined as (uint8_T)(255U - 1) in the C APIs, C-style cast and signedness change cannot be removed */
        if (SLIdType == SS_ENUM_TYPE) {
            MARTeTypeName = GetMARTeTypeNameFromEnumeratedTypes(enumSLId);
        } else
#endif
        /*lint -e{1924} SS_STRUCT is defined as (uint8_T)(255U) in the C APIs, C-style cast cannot be removed */
        if (SLIdType == SS_STRUCT) {
            MARTeTypeName = className;
            isStructured  = true;
        }
        else {  // numeric
            MARTeTypeName = GetMARTeTypeNameFromEnumeratedTypes(SLIdType);
        }

        // Calculate dimensions
        dimensions.SetSize(dimNum);

        for (uint32 dimIdx = 0u; (dimIdx < dimNum) && (dimArray != NULL); dimIdx++) {
            /*lint -e{679} uint32 used as index of uint32[] is ok */
            dimensions[dimIdx] = dimArray[dimArrayIdx + dimIdx];
            numElements *= dimensions[dimIdx];
        }

        // Calculate number of dimensions in MARTe2 sense
        for (uint32 dimIdx = 0u; dimIdx < dimNum; dimIdx++) {
            if (dimensions[dimIdx] > 1u) {
                numDimensions++;
            }
        }

        transpose = (orientation == rtwCAPI_MATRIX_COL_MAJOR) || (orientation == rtwCAPI_MATRIX_COL_MAJOR_ND);

        byteSize = dataTypeSize*numElements;

        if ( mode != Element ) { // root level interface

            interfaceArray.interfaceName = interfaceName;
            interfaceArray.structPath    = "";
            interfaceArray.fullPath      = fullPath;
            interfaceArray.interfaceType = mode;

            interfaceArray.CTypeName     = CTypeName;
            interfaceArray.MARTeTypeName = MARTeTypeName;
            interfaceArray.typeDesc      = TypeDescriptor::GetTypeDescriptorFromTypeName(MARTeTypeName.Buffer());
            interfaceArray.numDimensions = numDimensions;
            interfaceArray.numElements   = numElements;
            interfaceArray.dimensions    = dimensions;
            interfaceArray.orientation   = orientation;
            interfaceArray.transpose     = transpose;
            interfaceArray.isStructured  = isStructured;

            interfaceArray.dataTypeSize  = dataTypeSize;
            interfaceArray.byteSize      = byteSize;
            interfaceArray.dataAddr      = dataAddr;
        }

        if (isStructured) {
            // structured: traverse all its elements
            uint16 dataTypeNumElements   = rtwCAPI_GetDataTypeNumElements (dataTypeMap, typeIdx);
            uint16 elemOffsetIdx = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap, typeIdx);

            ret.exception = !( (interfaceArray.rootStructure).CreateRelative(interfaceName.Buffer()) );

            // cycle over the array of structure (if numElements > 1)
            //lint -e{850} Justification: elemIdx is not modified within this loop
            for (uint32 elemIdx = 0u; ret.ErrorsCleared() && (elemIdx < numElements); elemIdx++) {

                // append struct array indices to the name (MARTe2 flattened signal syntax)
                StreamString parentStructName = fullPath;
                if (numDimensions == 0u) {
                    // not a struct array, name is ok as it is
                }
                if ( (numDimensions == 1u) || ((numDimensions != 0u) && (mode != Element) && (mode != Parameter)) ) {
                    // struct vector OR generic root-level struct array signal
                    // (the latter must be treated as vector since GAM signal shapes cannot be specified)
                    ret.exception = !parentStructName.Printf("[%d]", elemIdx);
                }
                else {
                    // generic n-D
                    Vector<uint32> subscripts = LinearIndexToSubscripts(elemIdx, dimensions);
                    for (uint32 dimIdx = 0u; dimIdx < numDimensions; dimIdx++) {
                        if ( parentStructName.Printf("[%d]", subscripts[dimIdx]) ) {}
                    }
                }

                // recursively add struct fields
                for (uint32 fieldIdx = 0u; ret.ErrorsCleared() && (fieldIdx < dataTypeNumElements); fieldIdx++) {
                    ret = ScanInterface(interfaceArray, interfaceStruct, elemOffsetIdx + fieldIdx, Element, dataAddr, parentStructName);
                }

                // pointer to the next element of the struct array
                if (dataAddr != NULL) {
                    dataAddr = static_cast<uint8*>(dataAddr) + dataTypeSize; //lint !e9016 Justification: pointer arithmetic is required by the Simulink C-APIs
                }
            }

//             // detect padding
//             if (ret.ErrorsCleared()) {
//                 if (mode != Element) {
//                     uint32 numOfSignalElements = interfaceArray.GetSize();
//                     for (uint32 elemIdx = 0u; ret.ErrorsCleared() && (elemIdx < (numOfSignalElements - 1u)); elemIdx++) {
//                         uint8* endOfElemMemory   = static_cast<uint8*>(interfaceArray[elemIdx]->dataAddr) + interfaceArray[elemIdx]->byteSize; //lint !e9016 Justification: pointer arithmetic is required by the Simulink C-APIs
//                         uint8* startOfNextMemory = static_cast<uint8*>(interfaceArray[elemIdx + 1u]->dataAddr);
//
//                         if (endOfElemMemory != startOfNextMemory) {
//                             bool isSubStruct = (interfaceArray.rootStructure).MoveRelative(interfaceArray[elemIdx]->structPath.Buffer());
//                             bool okWrite     = false;
//                             bool okReturn    = false;
//                             okWrite = (interfaceArray.rootStructure).Write("_padding_", static_cast<uint32>(startOfNextMemory - endOfElemMemory));
//                             if ( isSubStruct ) {
//                                 okReturn = (interfaceArray.rootStructure).MoveToAncestor(1u);
//                             } else {
//                                 okReturn = true;
//                             }
//                             ret.exception = !( okWrite && okReturn );
//                         }
//                     }
//                 }
//             }

            if (ret.ErrorsCleared()) {
                if (mode != Element) {
                    uint32 numOfSignalElements = interfaceArray.GetSize();
                    for (uint32 elemIdx = 0u; ret.ErrorsCleared() && (elemIdx < (numOfSignalElements - 1u)); elemIdx++) {
                        uint64 endOfElemMemory   = reinterpret_cast<uint64>(interfaceArray[elemIdx]->dataAddr) + interfaceArray[elemIdx]->byteSize; //lint !e923 !e9091 Justification: actually compliant to rule 5-2-7, while rule 5-2-9 is only advisory
                        uint64 startOfNextMemory = reinterpret_cast<uint64>(interfaceArray[elemIdx + 1u]->dataAddr);                                //lint !e923 !e9091 Justification: actually compliant to rule 5-2-7, while rule 5-2-9 is only advisory
                        if (endOfElemMemory != startOfNextMemory) {
                            bool isSubStruct = (interfaceArray.rootStructure).MoveRelative(interfaceArray[elemIdx]->structPath.Buffer());
                            bool okWrite     = (interfaceArray.rootStructure).Write("_padding_", startOfNextMemory - endOfElemMemory);
                            bool okReturn    = false;
                            if ( isSubStruct ) {
                                okReturn = (interfaceArray.rootStructure).MoveToAncestor(1u);
                            } else {
                                okReturn = true;
                            }
                            ret.exception = !( okWrite && okReturn );
                        }
                    }
                }
            }

            // struct info are written in the associated ConfigurationDatabase
            if (ret.ErrorsCleared()) {
                ret.exception = !( (interfaceArray.rootStructure).Write("__Dimensions__", dimensions) );
                if (!ret.exception) {
                    ret.exception = !( (interfaceArray.rootStructure).MoveToAncestor(1u) );
                }
            }
        }
        else {
            // numeric: add interface to the list
            SimulinkInterface* currentInterface = new SimulinkInterface();

            currentInterface->interfaceName = interfaceName;
            currentInterface->structPath    = (interfaceArray.rootStructure).GetName();
            currentInterface->fullPath      = fullPath;
            currentInterface->interfaceType = mode;

            currentInterface->CTypeName     = CTypeName;
            currentInterface->MARTeTypeName = MARTeTypeName;
            currentInterface->typeDesc      = TypeDescriptor::GetTypeDescriptorFromTypeName(MARTeTypeName.Buffer());
            currentInterface->numDimensions = numDimensions;
            currentInterface->numElements   = numElements;
            currentInterface->dimensions    = dimensions;
            currentInterface->orientation   = orientation;
            currentInterface->transpose     = transpose;

            currentInterface->dataTypeSize  = dataTypeSize;
            currentInterface->byteSize      = byteSize;
            currentInterface->dataAddr      = dataAddr;

            ret.exception = !(interfaceArray.Add(currentInterface));

            interfaceArray.transpose = (transpose || interfaceArray.transpose);

            if ( (interfaceArray.rootStructure).Write(interfaceName.Buffer(), interfaceArray.GetSize() - 1u) ) {} // store the index of the signal in the associated list
        }
    }

    return ret;
}

/*lint -e{429} Justification: interfaceArray is freed in the destructor */
ErrorManagement::ErrorType SimulinkWrapperGAM::ScanInterfaces(SimulinkRootInterface* const interfaceArray, const void* const interfaceStruct, const uint32 numOfInterfaces, const InterfaceType mode) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    for (uint32 portIdx = 0u; ret.ErrorsCleared() && (portIdx < numOfInterfaces); portIdx++) {
        ret = ScanInterface(interfaceArray[portIdx], interfaceStruct, portIdx, mode);
    }

    return ret;
}

ErrorManagement::ErrorType SimulinkWrapperGAM::MapPorts(const InterfaceType interfaceType) {



    ErrorManagement::ErrorType ret = ErrorManagement::NoError;
    bool found = false;

    uint32 portIdx  = 0u;
    uint32 signalInPortIdx = 0u;
    uint32 GAMNumberOfSignals = 0u;
    uint32 modelNumberOfInterfaces = 0u;

    StreamString directionName = "";

    StreamString    GAMSignalName;
    uint32          GAMNumberOfElements   = 0u;
    uint32          GAMNumberOfDimensions = 0u;
    TypeDescriptor  GAMSignalType;
    SignalDirection direction = None;
    Vector<bool>*   isMapped = static_cast< Vector<bool>* >(0);

    SimulinkRootInterface* signalList = NULL_PTR(SimulinkRootInterface*);

    if (interfaceType == InputPort) {
        signalList              = inputs;
        GAMNumberOfSignals      = numberOfInputSignals;
        modelNumberOfInterfaces = modelNumOfInputs;
        direction               = InputSignals;
        directionName           = "Input ";
        isMapped                = &isInputMapped;
    }
    else if (interfaceType  == OutputPort) {
        signalList              = outputs;
        GAMNumberOfSignals      = numberOfOutputSignals;
        modelNumberOfInterfaces = modelNumOfOutputs;
        direction               = OutputSignals;
        directionName           = "Output";
        isMapped                = &isOutputMapped;
    }
    else if (interfaceType  == Signal) {
        signalList              = signals;
        GAMNumberOfSignals      = numberOfOutputSignals;
        modelNumberOfInterfaces = modelNumOfSignals;
        direction               = OutputSignals;
        directionName           = "Logging";
        isMapped                = &isOutputMapped;
    }
    else {
        ret.unsupportedFeature = true;
        REPORT_ERROR(ret, "[%s] - Unsupported signal direction in MapPorts()", GetName());
    }

    // cycle over GAM signals (to map each to a model signal)
    for(uint32 signalIdxLoop = 0u; ret.ErrorsCleared() && (signalIdxLoop < GAMNumberOfSignals); signalIdxLoop++) {

        uint32 signalIdx = signalIdxLoop;
        found = false;

        GAMSignalName = "";
        ret.exception = !GetSignalName(direction, signalIdx, GAMSignalName);

        // search the GAM signal among the model ports, either 1:1 or port (byte array) based
        if (ret.ErrorsCleared() && (signalList != NULL)) {
            portIdx = 0u;
            while((!found) && (portIdx < modelNumberOfInterfaces)) {

                uint32 portCarriedSignalsCount = signalList[portIdx].GetSize();

                if((signalList[portIdx].isStructured) && (nonVirtualBusMode == StructuredBusMode)) {

                    signalInPortIdx = 0u;
                    while((!found) && (signalInPortIdx < portCarriedSignalsCount)) {
                        if (GAMSignalName == (signalList[portIdx][signalInPortIdx]->fullPath)) {
                            found = true;
                        }
                        else {
                            signalInPortIdx++;
                        }
                    }
                }
                else {
                    if (GAMSignalName == (signalList[portIdx].fullPath)) {
                        found = true;
                    }
                }
                if(!found) {
                    portIdx++;
                }
            }

            if (found && (isMapped != NULL)) {
                (*isMapped)[signalIdx] = true;
            }
        }

        if (ret && found && (signalList != NULL) ) {

            // Array signal or structured signal in StructuredBusMode. In this case we check datatype, number of dimensions and number of elements.
            if( (!signalList[portIdx].isStructured) || (nonVirtualBusMode == StructuredBusMode) ) {

                ret.exception = !GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
                bool noErrors = ret.ErrorsCleared();
                if ( (GAMNumberOfDimensions != (signalList[portIdx][signalInPortIdx]->numDimensions)) && noErrors ) {
                    ret.internalSetupError = true;
                    REPORT_ERROR(ret, "[%s] - %s signal `%s`: number of dimensions mismatch (GAM: %d, model: %u)",
                        GetName(), directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfDimensions, signalList[portIdx][signalInPortIdx]->numDimensions);
                }

                if (ret.ErrorsCleared()) {
                    ret.exception = !GetSignalNumberOfElements(direction, signalIdx, GAMNumberOfElements);
                    noErrors = ret.ErrorsCleared();
                    if ( (GAMNumberOfElements != (signalList[portIdx][signalInPortIdx]->numElements)) && noErrors )
                    {
                        ret.internalSetupError = true;
                        REPORT_ERROR(ret, "[%s] - %s signal `%s`: number of elements mismatch (GAM: %d, model: %u)",
                            GetName(), directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfElements, signalList[portIdx][signalInPortIdx]->numElements);
                    }
                }

                if (ret.ErrorsCleared()) {
                    GAMSignalType = GetSignalType(direction, signalIdx);
                    StreamString inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(GAMSignalType);
                    if ( signalList[portIdx][signalInPortIdx]->MARTeTypeName != inputSignalTypeStr )
                    {
                        ret.internalSetupError = true;
                        REPORT_ERROR(ret, "[%s] - %s signal `%s`: type mismatch (GAM: %s, model: %s)",
                            GetName(), directionName.Buffer(), GAMSignalName.Buffer(), inputSignalTypeStr.Buffer(), (signalList[portIdx][signalInPortIdx]->MARTeTypeName).Buffer());
                    }
                }

                if (ret.ErrorsCleared()) {
                    StreamString actualCTypeName = GetCTypeNameFromMARTeTypeName((signalList[portIdx][signalInPortIdx]->MARTeTypeName).Buffer());
                    if(!CheckrtwCAPITypeAgainstSize(actualCTypeName.Buffer(), signalList[portIdx][signalInPortIdx]->dataTypeSize))
                    {
                        ret.internalSetupError = true;
                        REPORT_ERROR(ret, "[%s] - %s signal `%s`: datatype size mismatch (GAM: %s (%d bytes), model (%u bytes)).",
                            GetName(), directionName.Buffer(), (signalList[portIdx][signalInPortIdx]->fullPath).Buffer(),
                            signalList[portIdx][signalInPortIdx]->CTypeName,
                            GetTypeSizeFromCTypeName(actualCTypeName.Buffer()),
                            signalList[portIdx][signalInPortIdx]->dataTypeSize);
                    }
                }

                if (ret.ErrorsCleared()) {
                    // Matrix signals in column major orientation requires additional workload.
                    if (GAMNumberOfDimensions > 1u) {
                        if (signalList[portIdx][signalInPortIdx]->transpose) {

                            if (verbosityLevel > 0u) {
                                REPORT_ERROR(ErrorManagement::Warning,
                                    "[%s] - %s signal `%s`: orientation is column-major. Supported, but requires real-time transposition and may result in performance loss.",
                                    GetName(), directionName.Buffer(), GAMSignalName.Buffer());
                            }
                        }
                    }
                }
            }
            // Structured signal in byte array mode
            else {
                ret.exception = !GetSignalNumberOfElements(direction, signalIdx, GAMNumberOfElements);
                if ( ret.ErrorsCleared() && (GAMNumberOfElements != (signalList[portIdx].byteSize)) ) {
                    ret.internalSetupError = true;
                    REPORT_ERROR(ret, "[%s] - %s signal `%s`: size mismatch (GAM: %d, model %d)",
                        GetName(), directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfElements, signalList[portIdx].byteSize);
                }

                if (ret.ErrorsCleared()) {
                    ret.exception = !GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
                    if ( ret.ErrorsCleared() && (GAMNumberOfDimensions != 1u) ) {
                        ret.internalSetupError = true;
                        REPORT_ERROR(ret, "[%s] - %s signal `%s`: dimension mismatch (structured signals in byte array mode must have NumberOfDimensions = 1)",
                            GetName(), directionName.Buffer(), GAMSignalName.Buffer());
                    }
                }

                if (ret.ErrorsCleared()) {
                    GAMSignalType = GetSignalType(direction, signalIdx);
                    StreamString inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(GAMSignalType);
                    if ( GAMSignalType != UnsignedInteger8Bit ) {
                        ret.internalSetupError = true;
                        REPORT_ERROR(ret, "[%s] - %s signal `%s`: type mismatch (declared %s, structured signals in byte array mode must be declared as uint8)",
                            GetName(), directionName.Buffer(), GAMSignalName.Buffer(), inputSignalTypeStr.Buffer());
                    }
                }
            }

        }

        // Ok, here we can map memory inputs
        if ( ret.ErrorsCleared() && found && (signalList != NULL) ) {

            if((signalList[portIdx].isStructured) && (nonVirtualBusMode == StructuredBusMode)) {

                if ( (interfaceType == InputPort) || (interfaceType == Parameter) ) {

                    signalList[portIdx][signalInPortIdx]->MARTeAddress = GetInputSignalMemory(signalIdx);
                    signalList[portIdx][signalInPortIdx]->sourcePtr    = signalList[portIdx][signalInPortIdx]->MARTeAddress;
                    signalList[portIdx][signalInPortIdx]->destPtr      = signalList[portIdx][signalInPortIdx]->dataAddr;

                    if (signalInPortIdx == 0u) {
                        signalList[portIdx].MARTeAddress = GetInputSignalMemory(signalIdx);
                        signalList[portIdx].sourcePtr    = signalList[portIdx].MARTeAddress;
                        signalList[portIdx].destPtr      = signalList[portIdx].dataAddr;
                    }
                }
                else if ( (interfaceType == OutputPort) || (interfaceType == Signal) ) {

                    signalList[portIdx][signalInPortIdx]->MARTeAddress = GetOutputSignalMemory(signalIdx);
                    signalList[portIdx][signalInPortIdx]->sourcePtr    = signalList[portIdx][signalInPortIdx]->dataAddr;
                    signalList[portIdx][signalInPortIdx]->destPtr      = signalList[portIdx][signalInPortIdx]->MARTeAddress;

                    if (signalInPortIdx == 0u) {
                        signalList[portIdx].MARTeAddress = GetOutputSignalMemory(signalIdx);
                        signalList[portIdx].sourcePtr    = signalList[portIdx].dataAddr;
                        signalList[portIdx].destPtr      = signalList[portIdx].MARTeAddress;
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "[%s] - Invalid InterfaceType in MapPorts()", GetName());
                }
            }
            else {
                if ( (interfaceType == InputPort) || (interfaceType == Parameter) ) {

                    signalList[portIdx].MARTeAddress = GetInputSignalMemory(signalIdx);

                    signalList[portIdx].sourcePtr = signalList[portIdx].MARTeAddress;
                    signalList[portIdx].destPtr   = signalList[portIdx].dataAddr;
                }
                else if ( (interfaceType == OutputPort) || (interfaceType == Signal) ) {

                    signalList[portIdx].MARTeAddress = GetOutputSignalMemory(signalIdx);

                    signalList[portIdx].sourcePtr = signalList[portIdx].dataAddr;
                    signalList[portIdx].destPtr   = signalList[portIdx].MARTeAddress;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "[%s] - Invalid InterfaceType in MapPorts()", GetName());
                }
            }
        }
    }

    return ret;
}

bool SimulinkWrapperGAM::CheckrtwCAPITypeAgainstSize(StreamString cTypeName, const uint32 checkSize) const
{
    return ( GetTypeSizeFromCTypeName(cTypeName.Buffer()) == checkSize );
}

void SimulinkWrapperGAM::PrintAlgoInfo() const {
    
    SimulinkAlgoInfo info;
    bool ok;
    
    if ( (static_cast<void(*)(void*)>(NULL) == getAlgoInfoFunction) ) {
        info.expCode = 0u;
        ok = StringHelper::Copy(&info.gitHash[0u], "00000000");
        if (ok) {
            ok = StringHelper::Copy(&info.gitLog[0u],  "");
        }
    }
    else {
        (*getAlgoInfoFunction)(static_cast<void*>(&info));
        ok = true;
    }
    
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information,"Algorithm expcode : %d", info.expCode);
        REPORT_ERROR(ErrorManagement::Information,"Algorithm git hash: %s", info.gitHash);
        REPORT_ERROR(ErrorManagement::Information,"Algorithm git log : %s", info.gitLog);
    }
}

CLASS_REGISTER(SimulinkWrapperGAM, "3.0")

} /* namespace MARTe */

