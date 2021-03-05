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

static MARTe::StreamString GetOrientationName(const rtwCAPI_Orientation  &ELEorientation)
{
    MARTe::StreamString name;
    
    switch(ELEorientation)
    {
    case rtwCAPI_SCALAR:
        name = "scalar";
        break;
    case rtwCAPI_VECTOR:
        name = "vector";
        break;
    case rtwCAPI_MATRIX_ROW_MAJOR:
        name = "matrix row major";
        break;
    case rtwCAPI_MATRIX_COL_MAJOR:
        name = "matrix col major";
        break;
    case rtwCAPI_MATRIX_COL_MAJOR_ND:
        name = "matrix col major nd";
        break;
#ifdef ROW_MAJOR_ND_FEATURE
    case rtwCAPI_MATRIX_ROW_MAJOR_ND:
        name = "matrix row major nd";
        break;
#endif
    default:
        name = "N/A";
        break;
    }
    
    return name;
}

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
    sigGroup    = NULL_PTR(rtwCAPI_Signals*);
    dataTypeMap = NULL_PTR(rtwCAPI_DataTypeMap*);
    elementMap  = NULL_PTR(rtwCAPI_ElementMap*);
    dimMap      = NULL_PTR(rtwCAPI_DimensionMap*);
    dimArray    = NULL_PTR(uint32*);
    dataAddrMap = NULL_PTR(void**);
    
    states = NULL_PTR(void*);
    
    paramlastaddress        = NULL_PTR(void*);
    currentParamBaseAddress = NULL_PTR(void*);
    
    currentPort = NULL_PTR(SimulinkPort*);
    
    paramSeparator      = "-";
    signalSeparator     = ".";
    verbosityLevel       = 0u;
    modelNumOfInputs     = 0u;
    modelNumOfOutputs    = 0u;
    modelNumOfParameters = 0u;
    
    skipInvalidTunableParams = true;
    paramsHaveStructArrays   = false;

    nonVirtualBusMode             = ByteArrayBusMode;
    enforceModelSignalCoverage    = false;
}

/*lint -e{1551} memory must be freed and functions called in the destructor are expected not to throw exceptions */
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
    sigGroup    = NULL_PTR(rtwCAPI_Signals*);
    dataTypeMap = NULL_PTR(rtwCAPI_DataTypeMap*);
    elementMap  = NULL_PTR(rtwCAPI_ElementMap*);
    dimMap      = NULL_PTR(rtwCAPI_DimensionMap*);
    dimArray    = NULL_PTR(uint32*);
    dataAddrMap = NULL_PTR(void**);
    
    states = NULL_PTR(void*);
    
    paramlastaddress        = NULL_PTR(void*);
    currentParamBaseAddress = NULL_PTR(void*);
    
    currentPort = NULL_PTR(SimulinkPort*);
    
    // Deallocate all SimulinkClasses objects
    uint32 parameterSize = modelParameters.GetSize();
    for (uint32 paramIdx = 0U; paramIdx < parameterSize; paramIdx++) {
        SimulinkParameter* toDelete;
        if(modelParameters.Extract(0U, toDelete)) {
            delete toDelete;
        }
    }
    
    uint32 portSize = modelPorts.GetSize();
    for (uint32 portIdx = 0U; portIdx < portSize; portIdx++) {
        SimulinkPort* toDelete;
        if(modelPorts.Extract(0U, toDelete)) {
            delete toDelete;
        }
    }
}

bool SimulinkWrapperGAM::Initialise(StructuredDataI &data) {
 
    bool status = GAM::Initialise(data);
    
    /// The method performs the following actions:
    
    /// 1. Retrieval of mandatory configurations (library name, symbol prefix...)
    ///    from configuration file.
    
    // Library name parameter
    if (status) {
        status = data.Read("Library", libraryName);
        if (status) {
            REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as Library parameter.", libraryName.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting Library parameter.");
        }
    }

    // SymbolPrefix parameter
    if (status) {
        status = data.Read("SymbolPrefix", symbolPrefix);
    if (status) {
        REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as SymbolPrefix parameter.", symbolPrefix.Buffer());
    }
        else
        {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting SymbolPrefix parameter.");
        }
    }

    // Tunable parameters source name
    if (status) {
        bool isExternalSpecified = data.Read("TunableParamExternalSource", tunableParamExternalSource);
        if (isExternalSpecified) {
            REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as TunableParamExternalSource parameter.", tunableParamExternalSource.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "No TunableParamExternalSource declared.");
        }
    }
    
    // Verbosity parameter
    if (status) {
        // Simulink CAPI verbosity level
        if(data.Read("Verbosity", verbosityLevel)) {
            REPORT_ERROR(ErrorManagement::Information, "GAM verbosity set to %d.", verbosityLevel);
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "GAM verbosity not set, by default it is set to 0.");
        }
        
        uint16 itemp;
        if(data.Read("SkipInvalidTunableParams", itemp)) {
            skipInvalidTunableParams = (itemp != 0u);
            REPORT_ERROR(ErrorManagement::Information, "SkipInvalidTunableParams set to %d.", itemp);
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "SkipInvalidTunableParams not set, by default it is set to true.");
        }
    }
    
    if (status) {
        StreamString copyModeString = "";
        if ( data.Read("NonVirtualBusMode", copyModeString) ) {
            if (copyModeString == "Structured") {
                nonVirtualBusMode = StructuredBusMode;
            }
            else if (copyModeString == "ByteArray") {
                nonVirtualBusMode = ByteArrayBusMode;
            }
            else {
                status = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid NonVirtualBusMode: %s (can be ByteArray or Structured).", copyModeString.Buffer());
            }
            
            if (status) {
                REPORT_ERROR(ErrorManagement::Information, "NonVirtualBusMode mode set to %s", copyModeString.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "NonVirtualBusMode mode not set, by default it is set to ByteArray");
        }
        
    }
    
    //Check if Simulink signals must be completely mapped on the GAM
    if(status) {
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

        REPORT_ERROR(ErrorManagement::Information, "GAM I/O %s cover Simulink I/O",
                     enforceModelSignalCoverage?"must":"has not to");
    }

    /// 2. Opening of model code shared object library.
    
    if (status) {
        status = (static_cast<LoadableLibrary*>(NULL) == libraryHandle);
    }

    if (status) { // Load library
        libraryHandle = new LoadableLibrary();
    }

    if ((libraryHandle != NULL) && status) { // Load library
        status = libraryHandle->Open(libraryName.Buffer());
        if (status) {
            REPORT_ERROR(ErrorManagement::Information, "Library %s successfully loaded.", libraryName.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Couldn't open library: %s", libraryName.Buffer());
        }
    }
    
    /// 3. Retrieval of model code symbols.
    
    char8 symbol[64u];
    
    /*lint -e{611} the pointer returned by LoadableLibrary::Function() can be safely casted to a pointer to function */
    if ((libraryHandle != NULL) && status) {
        // instFunction
        status = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u); // Compose symbol

        if (status) { // Find symbol
            instFunction = reinterpret_cast<void*(*)(void)>(libraryHandle->Function(&symbol[0u]));
            status = (static_cast<void*(*)(void)>(NULL) != instFunction);
            if (!status) {
                REPORT_ERROR(ErrorManagement::Information, "Couldn't find %s symbol in model library (instFunction == NULL).", symbol);
            }
        }

        // getMmiFunction
        if (status) { // Compose symbol
            status = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (status) {
                status = StringHelper::ConcatenateN(&symbol[0u], "_GetCAPImmi", 64u);
            }
        }

        if (status) { // Find symbol
            getMmiFunction = reinterpret_cast<void*(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            status = (static_cast<void*(*)(void*)>(NULL) != getMmiFunction);
            if (!status) {
                REPORT_ERROR(ErrorManagement::Warning, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
            }
        }

        // initFunction
        if (status) { // Compose symbol
            status = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (status) {
                status = StringHelper::ConcatenateN(&symbol[0u], "_initialize", 64u);
            }
        }

        if (status) { // Find symbol
            initFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            status = (static_cast<void(*)(void*)>(NULL) != initFunction);
            if (!status) {
                REPORT_ERROR(ErrorManagement::Information, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
            }
        }
        
        // stepFunction
        if (status) { // Compose symbol
            status = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (status) {
                status = StringHelper::ConcatenateN(&symbol[0u], "_step", 64u);
            }
        }

        if (status) { // Find symbol
            stepFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            status = (static_cast<void(*)(void*)>(NULL) != stepFunction);
            if (!status) {
                REPORT_ERROR(ErrorManagement::Warning, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
            }
        }

        // getAlgoInfoFunction
        if (status) { // Compose symbol
            status = StringHelper::CopyN(&symbol[0u], symbolPrefix.Buffer(), 64u);
            if (status) {
                status = StringHelper::ConcatenateN(&symbol[0u], "_GetAlgoInfo", 64u);
            }
        }
        
        if (status) { // Find symbol
            getAlgoInfoFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(&symbol[0u]));
            status = (static_cast<void(*)(void*)>(NULL) != getAlgoInfoFunction);
            if (!status) {
                REPORT_ERROR(ErrorManagement::Information, "Algorithm information not found in the Simulink .so");
                status = true;
            }
        }
    }
    
    /// 4. Build a reference container containing parameter values
    ///    retrieved in the configuration file (under the `Parameters` node).
    
    bool hasParameterLeaf = data.MoveRelative("Parameters");
    
    if (hasParameterLeaf && status) {
        
        ReferenceT<ReferenceContainer> cfgParameterContainerTemp("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cfgParameterContainer = cfgParameterContainerTemp;
        cfgParameterContainer->SetName("CfgParameterContainer");
        
        status = cfgParameterContainer.IsValid();
        
        if (status) {
            status = ObjectRegistryDatabase::Instance()->Insert(cfgParameterContainer);
        }
        
        StreamString absolutePath;
        
        for (uint32 paramIdx = 0u; (paramIdx < data.GetNumberOfChildren()) && status; paramIdx++) {
            
            // AnyType does not manage its own memory, so a reference to AnyObject is required.
            ReferenceT<AnyObject> cfgParameterReference("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
            AnyType cfgParameter = data.GetType(data.GetChildName(paramIdx));
            
            cfgParameterReference->SetName(data.GetChildName(paramIdx));
            
            status = cfgParameterReference->Serialise(cfgParameter);
            
            if (status) {
                status = cfgParameterContainer->Insert(cfgParameterReference);
                
                absolutePath  = cfgParameterContainer->GetName();
                absolutePath += ".";
                absolutePath += cfgParameterReference->GetName();
                
                // Now write the name-path pair in a database.
                if (status) {
                    status = cfgParameterDatabase.Write(cfgParameterReference->GetName(), absolutePath);
                }
            }
        }
        
        if (status) {
            status = data.MoveToAncestor(1u);
        }
            
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to create parameter database from Parameters node in configuration file.");
        }
    }
    
    return status;
}

bool SimulinkWrapperGAM::Setup() {
    
    bool ok = SetupSimulink();
    
    // Call init method
    // This has to be called after tunable parameters actualization
    // to correctly handle tunable parameters dependent inits
    // (see f4e example)

    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Setup done, now init-ing the Simulink model");
    }
    else {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "SetupSimulink() failed.");
    }
    
    //Check if there are Simulink mapped signals which are not MARTe mapped when working in single signal mode
    if(enforceModelSignalCoverage && (nonVirtualBusMode == StructuredBusMode) ) {

        uint32 portIdx;
        uint32 signalInPortIdx = 0u;

        uint32 modelPortsCount = modelPorts.GetSize();

        if(verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "Scanning for orphaned Simulink signals on %d ports", modelPortsCount);
        }

        bool foundDisconnected = false;

        for(portIdx = 0u; (portIdx < modelPortsCount) && (!foundDisconnected); portIdx++) {
            uint32 portIdxLoop = portIdx;
            uint32 signalsInPortCount = modelPorts[portIdxLoop]->carriedSignals.GetSize();

            for(signalInPortIdx = 0u; (signalInPortIdx < signalsInPortCount) && (!foundDisconnected); signalInPortIdx++) {
		uint32 loopIndex = signalInPortIdx;
                if(modelPorts[portIdxLoop]->carriedSignals[loopIndex]->MARTeAddress == NULL) {
                    foundDisconnected = true;

                    REPORT_ERROR(ErrorManagement::ParametersError, "Found disconnected [%s] signal in portId %d - signalId %d",
                                 modelPorts[portIdxLoop]->carriedSignals[loopIndex]->fullName.Buffer(), portIdxLoop, loopIndex);
                }
            }
        }

        ok = !foundDisconnected;
    }

    // Simulink initFunction call, init of the Simulink model
    if (ok) {
        (*initFunction)(states);
    }
    
    // Send simulink ready message
    if (ok) {
        ReferenceT<Message> simulinkReadyMessage = Get(0u);
        if (simulinkReadyMessage.IsValid()) {
            REPORT_ERROR(ErrorManagement::Information, "Sending Simulink ready message 1.");
            if(!SendMessage(simulinkReadyMessage, this)) {
                REPORT_ERROR(ErrorManagement::Warning, "Failed to send ready message 1.");
            }
        }
    }
    
    return ok;
}

/*lint -e{613} NULL pointers are checked beforehand.*/
bool SimulinkWrapperGAM::SetupSimulink() {
    
    bool status;
    
    REPORT_ERROR(ErrorManagement::Information, "Allocating Simulink model dynamic memory...");

    // Simulink instFunction call, dynamic allocation of model data structures
    if (instFunction != NULL) {
        states = (*instFunction)();
    }
    
    status = (states != NULL);
    if (!status) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Simulink model allocation function returned a NULL data pointer");
    }

    // Get the Model Mapping Information (mmi) data structure from the Simulink shared object
    rtwCAPI_ModelMappingInfo* mmi = NULL_PTR(rtwCAPI_ModelMappingInfo*);
    if ( (getMmiFunction != NULL) && status) {
        void *mmiTemp = ((*getMmiFunction)(states));
        mmi = reinterpret_cast<rtwCAPI_ModelMappingInfo*>(mmiTemp);
    }
    
    status = (mmi != NULL);
    if (!status) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetMmiPtr function returned a NULL data pointer");
    }
    
    if (status) {
        
        dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);
        status = (dataTypeMap != NULL);

        modelNumOfInputs     = rtwCAPI_GetNumRootInputs(mmi);
        modelNumOfOutputs    = rtwCAPI_GetNumRootOutputs(mmi);
        modelNumOfParameters = rtwCAPI_GetNumModelParameters(mmi);
        
        if (verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "Simulink C API version number: %d", mmi->versionNum);
        }
        
        //Ensures the number of I/O Ports must be the same as the Simulink Model when in plain mode
        if (nonVirtualBusMode == ByteArrayBusMode) {
            uint32 numberOfGAMInputSignals  = GetNumberOfInputSignals();
            uint32 numberOfGAMOutputSignals = GetNumberOfOutputSignals();

            // Check number of declared main ports

            if (status) {
                status = (numberOfGAMInputSignals == modelNumOfInputs);
                if (!status) {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                        "Number of input signals mismatch (GAM: %u, model %u)",
                        numberOfGAMInputSignals,  modelNumOfInputs);
                }
            }

            if (status) {
                status = (numberOfGAMOutputSignals == modelNumOfOutputs);
                if (!status) {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                        "Number of output signals mismatch (GAM: %u, model %u)",
                        numberOfGAMOutputSignals,  modelNumOfOutputs);
                }
            }
        }
    }
    
    ///-------------------------------------------------------------------------
    /// 1. Populate modelParameters and print information
    ///-------------------------------------------------------------------------
    
    // Scan tunable parameters, print them if verbosity level is enough and
    // build the vector of tunable parameters objects
    if (status) {
        if (verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "%s, number of main tunable parameters: %d", libraryName.Buffer(), modelNumOfParameters);
        }
        
        status = ScanTunableParameters(mmi);
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed ScanTunableParameters().");
        }
    }
    
    if (status) {
        status = !paramsHaveStructArrays;
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                "Arrays of structures detected in the parameters. Feature is not yet supported, execution stopped.");
        }
    }
    
    // Max length of parameter names for this model is computed.
    uint64       maxNameLength;
    StreamString currentName;
    uint64       currentNameSize;
    
    if (status) {
        maxNameLength = 0u;
        for(uint32 paramIdx = 0u; paramIdx < modelParameters.GetSize(); paramIdx++) {
            
            currentName     = modelParameters[paramIdx]->fullName;
            currentNameSize = currentName.Size();
            
            if (maxNameLength < currentNameSize) {
                maxNameLength = currentNameSize;
            }
        }
        if (verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "%s, configured tunable parameters:", libraryName.Buffer());
            for(uint32 paramIdx = 0u; paramIdx < modelParameters.GetSize(); paramIdx++) {
                modelParameters[paramIdx]->PrintData(maxNameLength);
            }
        }
    }
    
    ///-------------------------------------------------------------------------
    /// 2. Populate modelPorts/modelSignals and print information
    ///-------------------------------------------------------------------------
    
    // Scan root input/output ports, print them if verbosity level is enough and
    // build the vectors of port objects
    if (status) {
        REPORT_ERROR(ErrorManagement::Information, "%s, number of root inputs: %d", libraryName.Buffer(), modelNumOfInputs);
        
        status = ScanRootIO(mmi, InputSignals);
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed ScanRootIO() for input signals.");
        }
    }
    
    if (status) {
        REPORT_ERROR(ErrorManagement::Information, "%s, number of root outputs: %d", libraryName.Buffer(), modelNumOfOutputs);
        
        status = ScanRootIO(mmi, OutputSignals);
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed ScanRootIO() for output signals.");
        }
    }
    
    if (status) {
        maxNameLength = 0u;
        for (uint32 portIdx = 0u; portIdx < modelPorts.GetSize(); portIdx++) {
            
            currentName     = modelPorts[portIdx]->fullName;
            currentNameSize = currentName.Size();
            
            if (maxNameLength < currentNameSize) {
                maxNameLength = currentNameSize;
            }
        }
        if (verbosityLevel > 1u) {
            REPORT_ERROR(ErrorManagement::Information, "%s, configured input/output ports:", libraryName.Buffer());
            for (uint32 portIdx = 0u; portIdx < modelPorts.GetSize(); portIdx++) {
                modelPorts[portIdx]->PrintPort(maxNameLength);
            }
        }
    }
    
    ///-------------------------------------------------------------------------
    /// 3. Check general coherence between GAM and model
    ///-------------------------------------------------------------------------
    
    // Now check for mismatch in post sizes
    for (uint32 portIdx = 0u; (portIdx < modelPorts.GetSize()) && status; portIdx++) {
        
        if (!modelPorts[portIdx]->isContiguous) {
            
            REPORT_ERROR(ErrorManagement::Warning,
                "Port '%s', size from type based scan vs. CAPI size mismatch, possibly uncontiguous structures, check signals offsets carefully",
                (modelPorts[portIdx]->fullName).Buffer());
        }
        
        if (modelPorts[portIdx]->offsetBasedSize != modelPorts[portIdx]->CAPISize) {
            
            REPORT_ERROR(ErrorManagement::InitialisationError,
                "Port '%s', size from offset based scan vs. CAPI size mismatch, port cannot be mapped",
                (modelPorts[portIdx]->fullName).Buffer());
            status = false;
        }
        
    }

    ///-------------------------------------------------------------------------
    /// 4. Check input port/signal coherence between GAM and model
    ///    and set copy address
    ///-------------------------------------------------------------------------
    
    if (status) {
        status = MapPorts(InputSignals);
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed MapPorts() for input signals.");
        }
    }
    
    ///-------------------------------------------------------------------------
    /// 5. Check output port/signal coherence between GAM and model
    ///    and set copy address
    ///-------------------------------------------------------------------------
    
    if (status) {
        status = MapPorts(OutputSignals);
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed MapPorts() for output signals.");
        }
    }
    
    ///-------------------------------------------------------------------------
    /// 6. Verify that the external parameter source (if any)
    ///    is compatible with the GAM
    ///-------------------------------------------------------------------------
    
    // The external parameter source must be a ReferenceContainer
    // populated by references to AnyObject.
    
    if (status) {
        
        if (tunableParamExternalSource.Size() > 0u) {
            ReferenceT<ReferenceContainer> mdsPar = ObjectRegistryDatabase::Instance()->Find(tunableParamExternalSource.Buffer());
            
            if (!mdsPar.IsValid()) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Tunable parameter loader %s is not valid (maybe not a ReferenceContainer?).", tunableParamExternalSource.Buffer());
            }
            else {
                    
                // Loop over all references in the MDSObjLoader container
                for (uint32 objectIdx = 0u; (objectIdx < mdsPar->Size()) && status; objectIdx++) {
                    
                    ReferenceT<AnyObject> paramObject = mdsPar->Get(objectIdx);
                    bool isAnyObject = paramObject.IsValid();
                    
                    // Ignore references that do not point to AnyObject
                    if (isAnyObject) {
                        
                        // Compose absolute path:
                        StreamString parameterAbsolutePath = "";
                        parameterAbsolutePath += mdsPar->GetName();
                        parameterAbsolutePath += ".";
                        parameterAbsolutePath += paramObject->GetName();
                        
                        // the string is used to make a name-path cdb
                        status = externalParameterDatabase.Write(paramObject->GetName(), parameterAbsolutePath.Buffer());
                        if (!status) {
                            REPORT_ERROR(ErrorManagement::InitialisationError,
                                "Failed Write() on database.");
                        }
                    }
                    
                }
            }
        }
    }
    
    ///-------------------------------------------------------------------------
    /// 7. Retrieve a value for parameter actualisation,
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
    
    for(uint32 paramIdx = 0u; (paramIdx < modelParameters.GetSize()) && status; paramIdx++) {
        
        isLoaded     = false;
        isActualised = false;
        isUnlinked   = false;
        
        // Retrieve the ReferenceT<AnyType> of the source parameter from which to actualise
        StreamString parameterPathInObjDatabase;
        const char8* currentParamName = (modelParameters[paramIdx]->fullName).Buffer();
        
        // 1. Parameters from configuration file (highest priority).
        if (cfgParameterDatabase.Read(currentParamName, parameterPathInObjDatabase)) {
            
            parameterSourceName = "configuration file";
            isLoaded = true;
        }
        
        // 2. Parameters from loader class (2nd-highest priority)
        else if (externalParameterDatabase.Read(currentParamName, parameterPathInObjDatabase)) {
            
            parameterSourceName = "loader class";
            isLoaded = true;
        }
        
        // 3. Parameter not found (if skipInvalidTunableParams, then use compile-time value)
        else {
            isLoaded = false;
        }
        
        // Data is copied from the source parameter to the model
        if (isLoaded) {
            
            ReferenceT<AnyObject> sourceParameterPtr;
            sourceParameterPtr = ObjectRegistryDatabase::Instance()->Find(parameterPathInObjDatabase.Buffer());
            
            isLoaded = sourceParameterPtr.IsValid();
            if (!isLoaded) {
                REPORT_ERROR(ErrorManagement::CommunicationError,
                    "Parameter %s: invalid reference from %s.",
                    currentParamName,
                    parameterSourceName
                );
            }
            
            if (isLoaded) {
                sourceParameter = sourceParameterPtr->GetType();
                
                if (sourceParameter.IsStaticDeclared()) {
                    isActualised = modelParameters[paramIdx]->Actualise(sourceParameter);
                }
                else {
                    isUnlinked = true;
                }
            }
        }
        
        // Add spaces to align the output
        StreamString alignedParamName = currentParamName;
        while ( alignedParamName.Size() < maxVariableNameLentgh ) {
            alignedParamName += " ";
        }
        
        // Based upon the actualisation outcome, execution is continued or stopped.
        
        // Cases in which execution can continue
        if ( isLoaded && isActualised ) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %s correctly actualized from %s.",
                alignedParamName.Buffer(), parameterSourceName.Buffer());
        }
        else if ( isLoaded && (!isActualised) && isUnlinked ) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %s unlinked, using compile time value",
                alignedParamName.Buffer());
        }
        else if ( isLoaded && (!isActualised) && (!isUnlinked) && skipInvalidTunableParams ) {
            REPORT_ERROR(ErrorManagement::Warning,
                "Parameter %s cannot be actualized, using compile time value",
                alignedParamName.Buffer());
        }
        else if ( (!isLoaded) && (!isActualised) && skipInvalidTunableParams ) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %s not found, using compile time value",
                alignedParamName.Buffer());
        }
        
        // Cases in which execution should be stopped
        else if ( (!isLoaded) && (!isActualised) && (!skipInvalidTunableParams) ) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "Parameter %s not found, failing",
                alignedParamName.Buffer());
            status = false;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "SkipInvalidTunableParams is false and parameter %s cannot be actualized, failing",
                alignedParamName.Buffer());
            status = false;
        }
        
    }
    
    ///-------------------------------------------------------------------------
    /// 8. Print port/signal details
    ///-------------------------------------------------------------------------
    
    // Print ports signals details
    if (verbosityLevel > 1u) {
        
        for(uint32 portIdx = 0u; (portIdx < modelNumOfInputs) && status; portIdx++) {
            
            uint32 signalsInThisPort = (modelPorts[portIdx]->carriedSignals).GetSize();
            
            REPORT_ERROR(ErrorManagement::Information,
                "IN port %s, # signals %d, signal content:",
                (modelPorts[portIdx]->fullName).Buffer(), signalsInThisPort);
            
            for(uint32 signalIdx = 0u; signalIdx < signalsInThisPort; signalIdx++) {
                (modelPorts[portIdx]->carriedSignals[signalIdx])->PrintSignal(40ul);
            }
        
        }
        
        for(uint32 portIdx = modelNumOfInputs; ( portIdx < (modelNumOfInputs + modelNumOfOutputs) ) && status; portIdx++) {
            
            uint32 signalsInThisPort = (modelPorts[portIdx]->carriedSignals).GetSize();
            
            REPORT_ERROR(ErrorManagement::Information,
                "OUT port %s, # signals %d, signal content:",
                (modelPorts[portIdx]->fullName).Buffer(), signalsInThisPort);
            
            for(uint32 signalIdx = 0u; signalIdx < signalsInThisPort; signalIdx++) {
                (modelPorts[portIdx]->carriedSignals[signalIdx])->PrintSignal(40ul);
            }
        }
    }
    
    return status;

}


bool SimulinkWrapperGAM::Execute() {
    
    uint32 portIdx;

    bool status = (states != NULL);

    // Inputs update
    for (portIdx = 0u; (portIdx < modelNumOfInputs) && status; portIdx++) {
        status = modelPorts[portIdx]->CopyData(nonVirtualBusMode);
    }
    
    // Model step
    if ( (stepFunction != NULL) && status) {
        (*stepFunction)(states);
    }

    // Ouputs update
    for (portIdx = modelNumOfInputs; ( portIdx < (modelNumOfInputs + modelNumOfOutputs) ) && status; portIdx++) {
        status = modelPorts[portIdx]->CopyData(nonVirtualBusMode);
    }
    
    return status;
}

/*lint -e{613} NULL pointers are checked beforehand.*/
bool SimulinkWrapperGAM::ScanTunableParameters(const rtwCAPI_ModelMappingInfo* const mmi)
{
    /*lint --e{ 923, 9016, 9091 } pointer arithmetic in this method looks safe. However, that part should probably be refactored */
    
    uint32        nOfParams = 0u;
    
    const char8* paramName;
    uint32       addrIdx;
    uint16       dataTypeIdx;
    uint16       numElements;
    uint16       dataTypeSize;
    uint8        slDataID;
    void*        paramAddress;
    
    uint32       SUBdimArrayIdx;
    uint16       SUBdimIdx;
    uint8        SUBnumDims;
    
    bool ok (mmi != NULL);

    // Populating C API data structure pointers of the class from mmi
    modelParams = rtwCAPI_GetModelParameters(mmi);
    ok = ( (modelParams != NULL) && ok);
    
    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);
    ok = ( (dataTypeMap != NULL) && ok);
    
    elementMap = rtwCAPI_GetElementMap(mmi);
    ok = ( (elementMap != NULL) && ok);
    
    dimMap   = rtwCAPI_GetDimensionMap(mmi);
    ok = ( (dimMap != NULL) && ok);
    
    dimArray = rtwCAPI_GetDimensionArray(mmi);
    ok = ( (dimArray != NULL) && ok);
    
    dataAddrMap = rtwCAPI_GetDataAddressMap(mmi);
    ok = ( (dataAddrMap != NULL) && ok);
    
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "Pointer to one of the model maps is NULL.");
    }
    
    if (ok) {
        nOfParams = rtwCAPI_GetNumModelParameters(mmi);
        if (nOfParams == 0u) {
            if (verbosityLevel > 0u) { REPORT_ERROR(ErrorManagement::Information, "No tunable parameters found."); }
        }
    }
    
    for(uint16 paramIdx = 0u; (paramIdx < nOfParams) && ok; paramIdx++) {
        
        dataTypeIdx  = rtwCAPI_GetModelParameterDataTypeIdx(modelParams, paramIdx); // Index into the data type in rtwCAPI_DataTypeMap
        paramName    = rtwCAPI_GetModelParameterName(modelParams, paramIdx);        // Name of the parameter
        slDataID     = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);           // Simulink type from data type map
        numElements  = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);     // Number of elements of the strucutre data type
        dataTypeSize = rtwCAPI_GetDataTypeSize(dataTypeMap,dataTypeIdx);            // Size of the datatype in bytes, WARNING: 16 bits maximum !!
        
        // clear lastAddressVector
        while ( (lastAddressVector.GetSize() != 0u) && ok) {
            ok = lastAddressVector.Remove(0u);
        }
        paramlastaddress = NULL_PTR(void*);
        
        /*lint -e{1924} SS_STRUCT is defined as (uint8_T)(255U) in the C APIs, so the C-style cast cannot be removed */
        if (slDataID != SS_STRUCT) {
            
            // Not structured parameter, directly print it from main params structure
            ok = ScanParameter(paramIdx, 1u, ParamFromParameters, NULL_PTR(void*), "", 0ul, "");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameter for parameter %s.", paramName);
            }
        }
        else {
            
            // Structured parameters, descend the tree
            addrIdx         = rtwCAPI_GetModelParameterAddrIdx(modelParams,paramIdx);
            paramAddress    = static_cast<void*>(rtwCAPI_GetDataAddress(dataAddrMap,addrIdx));
            SUBdimIdx       = rtwCAPI_GetModelParameterDimensionIdx(modelParams, paramIdx);
            SUBnumDims      = rtwCAPI_GetNumDims(dimMap, SUBdimIdx);
            SUBdimArrayIdx  = rtwCAPI_GetDimArrayIndex(dimMap, SUBdimIdx);

            StreamString diminfo = "[";
            bool structarray = false;
            
            for(uint32 idx2 = 0u; (idx2 < SUBnumDims) && ok; idx2++) {
                
                /*lint -e{679} uint32 used as index of uint32[] is ok */
                if(dimArray[SUBdimArrayIdx + idx2] > 1u) {
                    structarray = true;
                }
                /*lint -e{679} uint32 used as index of uint32[] is ok */
                ok = diminfo.Printf("%u", dimArray[SUBdimArrayIdx + idx2]);
                if ( idx2 != ( static_cast<uint32>(SUBnumDims) - 1u ) ) {
                    diminfo += ",";
                }
            }
            
            diminfo += "]";

            uint64 absDeltaAddress;
            currentParamBaseAddress = paramAddress;
            absDeltaAddress = reinterpret_cast<uint64>(paramAddress) - reinterpret_cast<uint64>(currentParamBaseAddress);
            
            // Print info about the scanned parameter
            if (verbosityLevel > 1u) {
                
                // add spaces to align names
                StreamString alignedName = paramName;
                while (alignedName.Size() < maxVariableNameLentgh) {
                    alignedName += " ";
                }
                
                REPORT_ERROR(ErrorManagement::Information, "%s, struct with %d elems, size(u16!): %d bytes, base addr: %p, dims: %s",
                    alignedName.Buffer(), numElements, dataTypeSize, paramAddress, diminfo.Buffer());
                    
                if (structarray) {
                    REPORT_ERROR(ErrorManagement::Warning, "Array of structure detected (%s), not yet supported! Please flatten the previous params structure.", paramName);
                    paramsHaveStructArrays = true;
                }
            }
            
            StreamString paramNameAndSeparator = StreamString(paramName);
            paramNameAndSeparator += paramSeparator;
            
            ok = ScanParametersStruct(dataTypeIdx, 1u, static_cast<uint8*>(paramAddress), paramNameAndSeparator, absDeltaAddress, "");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameterStruct for parameter %s.", paramName);
            }
            
        }
    }
    
    return ok;
}

/*lint -e{613} NULL pointers are checked in the caller method.*/
bool SimulinkWrapperGAM::ScanParametersStruct(const uint32 dataTypeIdx, const uint32 depth, void* const startAddress, StreamString baseName, const uint64 baseOffset, StreamString spacer) {
    
    /*lint --e{ 923, 9016, 9091 } pointer arithmetic in this method looks safe. However, that part should probably be refactored */
    
    bool ok = true;
    
    const char8* elementName;
    uint16       numElements;
    uint16       elemMapIdx;
    uint16       SUBdataTypeIndex;
    uint8        SUBslDataID;
    uint16       SUBnumElements;
    uint32       SUBdataTypeOffset;
    void*        byteptr = startAddress;
    void*        runningbyteptr = startAddress;
    uint16       SUBdimIdx;
    uint8        SUBnumDims;
    uint32       SUBdimArrayIdx;
    uint16       SUBdataTypeSize;

    elemMapIdx  = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap,dataTypeIdx);
    numElements = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);

    StreamString specificSpacer; // spacer of the current parameter
    StreamString passoverSpacer; // spacer that will be passed to a recursive call
    
    /*lint -e{850} elemIdx is NOT modified in the body of the loop! */
    for(uint16 elemIdx = 0u; (elemIdx < numElements) && ok; elemIdx++) {
        
        elementName         = rtwCAPI_GetElementName        (elementMap, elemIdx + elemMapIdx);
        SUBdimIdx           = rtwCAPI_GetElementDimensionIdx(elementMap, elemIdx + elemMapIdx);
        SUBnumDims          = rtwCAPI_GetNumDims            (dimMap,     SUBdimIdx);
        SUBdimArrayIdx      = rtwCAPI_GetDimArrayIndex      (dimMap,     SUBdimIdx);
        SUBdataTypeIndex    = rtwCAPI_GetElementDataTypeIdx (elementMap, elemIdx + elemMapIdx);
        SUBdataTypeOffset   = rtwCAPI_GetElementOffset      (elementMap, elemIdx + elemMapIdx);
        SUBslDataID         = rtwCAPI_GetDataTypeSLId       (dataTypeMap, SUBdataTypeIndex);
        SUBnumElements      = rtwCAPI_GetDataTypeNumElements(dataTypeMap, SUBdataTypeIndex);
        SUBdataTypeSize     = rtwCAPI_GetDataTypeSize       (dataTypeMap, SUBdataTypeIndex);
        
        specificSpacer = spacer; // reset the spacer
        passoverSpacer = spacer;
        if (elemIdx == ( static_cast<uint32>(numElements) - 1u ) ) {
            specificSpacer += "└ ";
        } 
        else {
            specificSpacer += "├ ";
        }
        
        StreamString diminfo = "[";
        bool structarray = false;
        for (uint32 subIdx = 0u; (subIdx < SUBnumDims) && ok; subIdx++)
        {
            /*lint -e{679} uint32 used as index of uint32[] is ok */
            if (dimArray[SUBdimArrayIdx + subIdx] > 1u) {
                structarray = true;
            }
            /*lint -e{679} uint32 used as index of uint32[] is ok */
            ok = diminfo.Printf("%d", dimArray[SUBdimArrayIdx + subIdx]);
            if (subIdx != ( static_cast<uint32>(SUBnumDims) - 1u ) ) {
                diminfo += ",";
            }
        }
        diminfo += "]";
        
        /*lint -e{1924} SS_STRUCT is defined as (uint8_T)(255U) in the C APIs, so the C-style cast cannot be removed */
        if ( SUBslDataID != SS_STRUCT ) {
            
            ok = ScanParameter(elemMapIdx + elemIdx, depth, ParamFromElementMap, byteptr, baseName, baseOffset, specificSpacer);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameter for parameter %s.", elementName);
            }
        }
        else {

            runningbyteptr = reinterpret_cast<void *>(static_cast<uint8*>(startAddress) + SUBdataTypeOffset);

            // Calculating same level delta address
            uint64 deltaaddr;
            if (lastAddressVector.GetSize() < depth) {
                
                if(lastAddressVector.Add(runningbyteptr)) {
                    deltaaddr = 0u;
                }
            }
            else if (lastAddressVector.GetSize() > depth) {
                
                while ( (lastAddressVector.GetSize() == depth) && ok) {
                    ok = lastAddressVector.Remove(lastAddressVector.GetSize() - 1u);
                }
                deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
            }
            else {
                
                if(elemIdx == 0u) {
                    
                    if (lastAddressVector.Set(depth - 1u, runningbyteptr)) {
                        deltaaddr = 0u;
                    }
                }
                else {
                    
                    deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
                }
            }

            // Calculating absolute delta address
            uint64 absDeltaAddress;
            absDeltaAddress = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(currentParamBaseAddress);
            
            // Print info about the scanned parameter
            if (verbosityLevel > 1u) {
                
                // add spaces to align names
                StreamString alignedName = specificSpacer;
                alignedName += elementName;
                while (alignedName.Size() < (maxVariableNameLentgh + (specificSpacer.Size()/2u) )) {
                    alignedName += " ";
                }
                
                REPORT_ERROR(ErrorManagement::Information, "%s, nested struct (idx %d) with %d elems, size(u16!): %d, offset: %d, base addr: %p, same lvl delta: %d, abs delta: %d, dims: %s",
                    alignedName.Buffer(), elemIdx, SUBnumElements, SUBdataTypeSize, SUBdataTypeOffset, runningbyteptr, deltaaddr, absDeltaAddress, diminfo.Buffer());
                    
                if (structarray) {
                    REPORT_ERROR(ErrorManagement::Warning, "Array of structure detected, not yet supported! Please flatten the previous params structure!");
                    paramsHaveStructArrays = true;
                }
            }
            
            StreamString nameAndSeparators = baseName;
            nameAndSeparators += elementName;
            nameAndSeparators += paramSeparator;
            
            if (elemIdx == (static_cast<uint32>(numElements) - 1u) ) { 
                passoverSpacer += "  ";  // first space is a non-breaking space for size symmetry with ┆
            } else {
                passoverSpacer += "┆ ";
            }
            
            ok = ScanParametersStruct(SUBdataTypeIndex, depth + 1u, runningbyteptr, nameAndSeparators, absDeltaAddress, passoverSpacer);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameterStruct for parameter %s.", elementName);
            }
        }
    }
    
    return ok;
}

/*lint -e{613} NULL pointers are checked in the caller method.*/
bool SimulinkWrapperGAM::ScanParameter(const uint16 parIdx, const uint32 depth, const ParameterMode mode, void* const startAddress, StreamString baseName, const uint64 baseOffset, StreamString spacer)
{
    /*lint --e{ 923, 9016, 9091 } pointer arithmetic in this method looks safe. However, that part should probably be refactored */
    
    bool ok = true;
    
    const char8*        ELEelementName   = NULL_PTR(char8*);
    uint16              ELEdataTypeIndex = 0u;
    uint16              ELEdimIndex      = 0u;
    uint32              ELEelementOffset = 0u;
    uint32              ELEaddrIdx       = 0u;
    uint16              ELEdataTypeSize  = 0u;
    uint8               ELEslDataID      = 0u;
    uint8               ELEenumType      = 0u;  // data type for enum parameters
    uint8*              ELEparamAddress  = NULL_PTR(uint8*);
    
    const char8*        ELEclassName;
    const char8*        ELEcTypeName;
    StreamString        ELEMARTeTypeName;
    uint8               ELEnumDims;
    uint32              ELEdimArrayIdx;
    rtwCAPI_Orientation ELEorientation;

    uint32              ELEsize         = 1u;
    uint32              ELEelements     = 1u;
    uint32              ELEMARTeNumDims = 0u;   // number of dimensions according to MARTe standard
    
    StreamString        fullPathName;
    TypeDescriptor      ELEtype;
    
    switch(mode) {
        
        case ParamFromElementMap: {
            
            // Parameters data is retrieved from the elements data structure
            // (this case applies to all other cases w.r.t. the case below)
            ELEelementName       = rtwCAPI_GetElementName        (elementMap,  parIdx);
            ELEdataTypeIndex     = rtwCAPI_GetElementDataTypeIdx (elementMap,  parIdx);
            ELEdimIndex          = rtwCAPI_GetElementDimensionIdx(elementMap,  parIdx);
            ELEelementOffset     = rtwCAPI_GetElementOffset      (elementMap,  parIdx);
            ELEslDataID          = rtwCAPI_GetDataTypeSLId       (dataTypeMap, ELEdataTypeIndex);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize       (dataTypeMap, ELEdataTypeIndex);
            
            ELEparamAddress      = static_cast<uint8*>(startAddress) + ELEelementOffset;
            
            break;
        }
        
        case ParamFromParameters: {
            
            // Parameter data is retrieved from main CAPI parameters data structure
            // (this case applies to not structured parameters at root level of the tree)
            ELEelementName       = rtwCAPI_GetModelParameterName        (modelParams, parIdx);
            ELEdataTypeIndex     = rtwCAPI_GetModelParameterDataTypeIdx (modelParams, parIdx); 
            ELEdimIndex          = rtwCAPI_GetModelParameterDimensionIdx(modelParams, parIdx);
            ELEelementOffset     = 0u;                                                          // root level parameters have their address directly specified in the dataAddrMap structure
            ELEslDataID          = rtwCAPI_GetDataTypeSLId              (dataTypeMap, ELEdataTypeIndex);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize              (dataTypeMap, ELEdataTypeIndex);
            
            ELEaddrIdx           = rtwCAPI_GetModelParameterAddrIdx     (modelParams, parIdx);
            ELEparamAddress      = static_cast<uint8*>(rtwCAPI_GetDataAddress(dataAddrMap, ELEaddrIdx));
            
            break;
        }
        
        default:
            REPORT_ERROR(ErrorManagement::FatalError, "Wrong parameter mode in SimulinkWrapperGAM::ScanParameter(), depth %u", depth);
            ok = false;
            
    }
    
    /*lint -e{429} memory allocated for currentParameter is freed by class destructor */
    if (ok) {
        
        // Type
        ELEcTypeName   = rtwCAPI_GetDataTypeCName (dataTypeMap, ELEdataTypeIndex);
        ELEclassName   = rtwCAPI_GetDataTypeMWName(dataTypeMap, ELEdataTypeIndex);
#ifdef ENUM_FEATURE
        ELEenumType    = rtwCAPI_GetDataEnumStorageType(dataTypeMap, ELEdataTypeIndex); // Add enum support only if available (from version 2019a onwards)
#else
        ELEenumType    = 127u; // Invalid datatype
#endif
        
        /*lint -e{1924, 9117} SS_ENUM_TYPE is defined as (uint8_T)(255U - 1) in the C APIs, C-style cast and signedness change cannot be removed */
        if (ELEslDataID == SS_ENUM_TYPE) {
            // if the parameter is an enum the typename is "numeric" and
            // the underlying datatype is stored in enumStorageType
            ELEMARTeTypeName = GetMARTeTypeNameFromEnumeratedTypes(ELEenumType);
            ELEtype          = TypeDescriptor::GetTypeDescriptorFromTypeName(ELEMARTeTypeName.Buffer());
#ifndef ENUM_FEATURE
            REPORT_ERROR(ErrorManagement::Warning, "Parameter %s: usage of enumeration type requires a higher version.", fullPathName.Buffer());
#endif
        }
        else {
            ELEMARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEcTypeName);
            ELEtype          = TypeDescriptor::GetTypeDescriptorFromTypeName(GetMARTeTypeNameFromCTypeName(ELEcTypeName));
        }
        
        // Dimensions
        ELEnumDims           = rtwCAPI_GetNumDims      (dimMap,      ELEdimIndex);      // not number of dimensions in MARTe2 sense, but number of slots occupied in the dimension array
        ELEdimArrayIdx       = rtwCAPI_GetDimArrayIndex(dimMap,      ELEdimIndex);
        ELEorientation       = rtwCAPI_GetOrientation  (dimMap,      ELEdimIndex);

        Vector<uint32> ELEactualDimensions(ELEnumDims);
        
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++)
        {
            /*lint -e{679} uint32 used as index of uint32[] is ok */
            ELEactualDimensions[dimIdx] = dimArray[ELEdimArrayIdx + dimIdx];
            ELEelements *= ELEactualDimensions[dimIdx];
        }
        
        // Calculate number of dimensions in MARTe2 sense
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            if (ELEactualDimensions[dimIdx] > 1u) {
                ELEMARTeNumDims++;
            }
        }
        
        ELEsize = ELEelements*ELEdataTypeSize;

        fullPathName  = baseName.Buffer();
        fullPathName += ELEelementName;
        
        uint64 deltaAddress;
        if (paramlastaddress != NULL) {
            deltaAddress = reinterpret_cast<uint64>(ELEparamAddress) - reinterpret_cast<uint64>(paramlastaddress);
        }
        else {
            deltaAddress = 0ul;
        }
        paramlastaddress = ELEparamAddress;
        
        // Print info about the scanned parameter
        if (verbosityLevel > 1u) {
            
            // add spaces to align names
            StreamString alignedName = spacer;
            alignedName += ELEelementName;
            while ( alignedName.Size() < (maxVariableNameLentgh + (spacer.Size()/2u) ) ) {
                alignedName += " ";
            }
            
            StreamString paramInfoString = "";
            ok = paramInfoString.Printf("%s, offset %d, type %-7s (%d bytes), ndims %d, dims [",
                alignedName.Buffer(), ELEelementOffset, ELEcTypeName, ELEdataTypeSize, ELEMARTeNumDims);
            
            if (ok) {
                ok = paramInfoString.Printf("%d", ELEactualDimensions[0u]);
            
                for (uint32 dimIdx = 1u; (dimIdx < ELEnumDims) && ok; dimIdx++) {
                    ok = paramInfoString.Printf(",%d", ELEactualDimensions[dimIdx]);
                }
                
                // TODO printf version used to erase the leading zeros in front of the pointer
                ok = paramInfoString.Printf("], addr: %p, pr par delta: %d, orient: ", ELEparamAddress, deltaAddress);
                paramInfoString += GetOrientationName(ELEorientation);
            }
            
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, paramInfoString.Buffer());
            }
        }
        
        // Parameter that is currently being updated.
        SimulinkParameter* currentParameter = new SimulinkParameter();
        
        currentParameter->fullName      = fullPathName;
        
        currentParameter->orientation           = ELEorientation;
        currentParameter->numberOfDimensions    = ELEMARTeNumDims;
        currentParameter->totalNumberOfElements = ELEelements;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            currentParameter->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
        }
        
        currentParameter->address       = ELEparamAddress;
        currentParameter->byteSize      = ELEsize;
        currentParameter->offset        = baseOffset;
        
        currentParameter->className     = ELEclassName;
        currentParameter->cTypeName     = ELEcTypeName;
        currentParameter->MARTeTypeName = ELEMARTeTypeName;
        currentParameter->type          = ELEtype;
        currentParameter->dataTypeSize  = ELEdataTypeSize;
        
        currentParameter->verbosity     = verbosityLevel;
        
        ok = modelParameters.Add(currentParameter);
    }
    
    return ok;
}

/*lint -e{613} NULL pointers are checked beforehand.*/
bool SimulinkWrapperGAM::ScanRootIO(const rtwCAPI_ModelMappingInfo* const mmi, const SignalDirection mode) {
    /*lint --e{ 923, 9016, 9091 } pointer arithmetic in this method looks safe. However, that part should probably be refactored */
    
    uint32       nOfSignals = 0u;
    const char8* sigName;
    uint16       dataTypeIdx;
    uint8        slDataID;
    uint16       numElements;
    uint16       dataTypeSize;
    uint32       addrIdx;
    void*        sigAddress;
    
    bool ok = (mmi != NULL);
    
    // Populating C API data structure pointers of the class from mmi
    rootInputs = rtwCAPI_GetRootInputs(mmi);
    ok = ( (rootInputs != NULL) && ok );
    
    rootOutputs = rtwCAPI_GetRootOutputs(mmi);
    ok = ( (rootOutputs != NULL) && ok );
    
    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);
    ok = ( (dataTypeMap != NULL) && ok );
    
    elementMap = rtwCAPI_GetElementMap(mmi);
    ok = ( (elementMap != NULL) && ok );
    
    dimMap = rtwCAPI_GetDimensionMap(mmi);
    ok = ( (dimMap != NULL) && ok );
    
    dimArray = rtwCAPI_GetDimensionArray(mmi);
    ok = ( (dimArray != NULL) && ok );
    
    dataAddrMap = rtwCAPI_GetDataAddressMap(mmi);
    ok = ( (dataAddrMap != NULL) && ok );
    
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "Pointer to one of the model maps is NULL.");
    }
    
    if (ok) {
        switch(mode) {
            
        case InputSignals:
            nOfSignals = rtwCAPI_GetNumRootInputs(mmi);
            if (nOfSignals == 0u) {
                if (verbosityLevel > 0u) { REPORT_ERROR(ErrorManagement::Information, "No root inputs found"); }
            }
            sigGroup = rootInputs;
            break;
            
        case OutputSignals:
            nOfSignals = rtwCAPI_GetNumRootOutputs(mmi);
            if (nOfSignals == 0u) {
                if (verbosityLevel > 0u) { REPORT_ERROR(ErrorManagement::Information, "No root outputs found"); }
            }
            sigGroup = rootOutputs;
            break;
        
        case None:
        default:
            sigGroup = NULL_PTR(rtwCAPI_Signals*);
            REPORT_ERROR(ErrorManagement::FatalError, "Wrong signal direction in SimulinkWrapperGAM::ScanRootIO()");
            break;
        }
        
        ok = (sigGroup != NULL);
    }
    
    for (uint16 sigIdx = 0u; (sigIdx < nOfSignals) && ok ; sigIdx++) {
        
        dataTypeIdx  = rtwCAPI_GetSignalDataTypeIdx(sigGroup, sigIdx);              // Index into the data type in rtwCAPI_DataTypeMap
        sigName      = rtwCAPI_GetSignalName(sigGroup, sigIdx);                     // Name of the parameter
        slDataID     = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);           // Simulink type from data type map
        numElements  = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);     // Number of elements of the strucutre data type
        dataTypeSize = rtwCAPI_GetDataTypeSize(dataTypeMap,dataTypeIdx);            // Size of the datatype in bytes, WARNING: 16 bits maximum !!

        // clear lastAddressVector
        while ( (lastAddressVector.GetSize() != 0u) && ok) {
            ok = lastAddressVector.Remove(0u);
        }
        
        switch(mode)
        {
            case InputSignals:
                currentPort = static_cast<SimulinkPort*>(new SimulinkInputPort());
                break;
                
            case OutputSignals:
                currentPort = static_cast<SimulinkPort*>(new SimulinkOutputPort());
                break;
            
            case None:
            default:
            currentPort = NULL_PTR(SimulinkPort*);
                REPORT_ERROR(ErrorManagement::FatalError, "Wrong mode in SimulinkWrapperGAM::ScanRootIO()");
                ok = false;
        }
        
        if ((currentPort != NULL) && ok) {
            
            currentPort->fullName = sigName;
            currentPort->verbosity = verbosityLevel;
            
            /*lint -e{1924} SS_STRUCT is defined as (uint8_T)(255U) in the C APIs, so the C-style cast cannot be removed */
            if (slDataID != SS_STRUCT) {
                // Not structured parameter, directly print it from main params structure
                ok = ScanSignal(sigIdx, 1u, SignalFromSignals, NULL_PTR(void*), "", 0ul, ""); // TODO: check if baseoffset 0 is correct
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanSignal for signal %s.", sigName);
                }
            }
            else {
                // Structured signal, descend the tree
                addrIdx    = rtwCAPI_GetSignalAddrIdx(sigGroup,sigIdx);
                sigAddress = (void *) rtwCAPI_GetDataAddress(dataAddrMap,addrIdx);

                uint64 absDeltaAddress;
                currentPort->baseAddress = sigAddress;
                absDeltaAddress = reinterpret_cast<uint64>(sigAddress) - reinterpret_cast<uint64>(currentPort->baseAddress);
                
                // Print info about the scanned parameter
                if (verbosityLevel > 1u) {
                    
                    // add spaces to align names
                    StreamString alignedName = sigName;
                    while (alignedName.Size() < maxVariableNameLentgh) {
                        alignedName += " ";
                    }
                    
                    REPORT_ERROR(ErrorManagement::Information, "%s, struct with %d elems, size: %d bytes, base addr: %p, abs delta: %d",
                       alignedName.Buffer(), numElements, dataTypeSize, sigAddress, absDeltaAddress);
                        
                }
                
                currentPort->CAPISize = dataTypeSize;
                currentPort->byteSize = dataTypeSize;
                currentPort->address  = sigAddress;
                // TODO: verify if base offset 0 is correct for a root port structure
                StreamString nameAndSeparators = sigName;
                nameAndSeparators += signalSeparator;
                
                //Signal classification
                currentPort->isStructured = true;

                ok = ScanSignalsStruct(dataTypeIdx, 1u, sigAddress, nameAndSeparators, absDeltaAddress, "");
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanSignalStruct for signal %s.", sigName);
                }
            }
            
            ok = modelPorts.Add(currentPort);
        }
    }
    
    return ok;

}

/*lint -e{613} NULL pointers are checked in the caller method.*/
bool SimulinkWrapperGAM::ScanSignalsStruct(const uint32 dataTypeIdx, const uint32 depth, void* const startAddress, StreamString baseName, const uint64 baseOffset, StreamString spacer){
    
    /*lint --e{ 923, 9016, 9091 } pointer arithmetic in this method looks safe. However, that part should probably be refactored */
    
    bool ok = true;
    
    const char8* elementName;
    uint8        SUBslDataID;
    uint32       numElements;
    uint16       elemMapIdx;
    uint16       SUBdataTypeIndex;
    uint16       SUBnumElements;
    uint32       SUBdataTypeOffset;
    void*        byteptr = startAddress;
    void*        runningbyteptr = startAddress;

    elemMapIdx  = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap,dataTypeIdx);
    numElements = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);

    StreamString specificSpacer; // spacer of the current signal
    StreamString passoverSpacer; // spacer that will be passed to a recursive call
    
    /*lint -e{850} elemIdx is NOT modified in the body of the loop! */
    for(uint16 elemIdx = 0u; (elemIdx < numElements) && ok; elemIdx++) {
        
        elementName         = rtwCAPI_GetElementName        (elementMap,  elemIdx + elemMapIdx);
        SUBdataTypeIndex    = rtwCAPI_GetElementDataTypeIdx (elementMap,  elemIdx + elemMapIdx);
        SUBdataTypeOffset   = rtwCAPI_GetElementOffset      (elementMap,  elemIdx + elemMapIdx);
        SUBslDataID         = rtwCAPI_GetDataTypeSLId       (dataTypeMap, SUBdataTypeIndex);
        SUBnumElements      = rtwCAPI_GetDataTypeNumElements(dataTypeMap, SUBdataTypeIndex);
        
        
        specificSpacer = spacer; // reset the spacer
        passoverSpacer = spacer;
        if ( elemIdx == (numElements - 1u) ) {
            specificSpacer += "└ ";
        } 
        else {
            specificSpacer += "├ ";
        }
        
        /*lint -e{1924} SS_STRUCT is defined as (uint8_T)(255U) in the C APIs, so the C-style cast cannot be removed */
        if (SUBslDataID != SS_STRUCT) {
            
            ok = ScanSignal(elemMapIdx + elemIdx, depth, SignalFromElementMap, byteptr, baseName, baseOffset, specificSpacer);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanSignal for signal %s.", elementName);
            }
        }
        else {

            runningbyteptr = reinterpret_cast<void *>(static_cast<uint8*>(startAddress) + SUBdataTypeOffset);

            // Calculating same level delta address
            uint64 deltaaddr;
            if (lastAddressVector.GetSize() < depth) {
                
                if (lastAddressVector.Add(runningbyteptr)) {
                    deltaaddr = 0u;
                }
            }
            else if (lastAddressVector.GetSize() > depth) {
                
                // resize lastAddressVector
                while ( (lastAddressVector.GetSize() == depth) && ok) {
                    ok = lastAddressVector.Remove(lastAddressVector.GetSize() - 1u);
                }
                deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
            }
            else {
                
                if (elemIdx == 0u) {
                    
                    if (lastAddressVector.Set(depth - 1u, runningbyteptr)) {
                        deltaaddr = 0u;
                    }
                }
                else {
                    
                    deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
                }
            }

            // Calculating absolute delta address
            uint64 absDeltaAddress;
            absDeltaAddress = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(currentPort->baseAddress);
            
            // Print info about the scanned parameter
            if (verbosityLevel > 1u) {
                
                // add spaces to align names
                StreamString alignedName = specificSpacer;
                alignedName += elementName;
                while (alignedName.Size() < (maxVariableNameLentgh + (specificSpacer.Size()/2u) )) {
                    alignedName += " ";
                }
                
                REPORT_ERROR(ErrorManagement::Information, "%s, nested struct (idx %d) with %d elems, offset: %d, base addr: %p, same level delta: %d, abs delta: %d",
                    alignedName.Buffer(), elemIdx, SUBnumElements, SUBdataTypeOffset, runningbyteptr, deltaaddr, absDeltaAddress);
                    
            }
            
            StreamString nameAndSeparators = baseName;
            nameAndSeparators += elementName;
            nameAndSeparators += signalSeparator;
            
            if ( elemIdx == (numElements - 1u) ) { 
                passoverSpacer += "  ";  // first space is a non-breaking space for size symmetry with ┆
            } else {
                passoverSpacer += "┆ ";
            }
            
            ok = ScanSignalsStruct(SUBdataTypeIndex, depth + 1u, runningbyteptr, nameAndSeparators, absDeltaAddress, passoverSpacer);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanSignalStruct for signal %s.", elementName);
            }
            
        }
    }
    
    return ok;
}

/*lint -e{613} NULL pointers are checked in the caller method.*/
bool SimulinkWrapperGAM::ScanSignal(const uint16 sigIdx, const uint32 depth, const SignalMode mode, void* const startAddress, StreamString baseName, const uint64 baseOffset, StreamString spacer) {
    /*lint --e{ 923, 9016, 9091 } pointer arithmetic in this method looks safe. However, that part should probably be refactored */
    
    bool ok = true;
    
    const char8*        ELEelementName   = NULL_PTR(char8*);
    uint16              ELEdataTypeIndex = 0u;
    uint16              ELEdimIndex      = 0u;
    uint32              ELEelementOffset = 0u;
    uint32              ELEaddrIdx       = 0u;
    uint16              ELEdataTypeSize  = 0u;
    uint8               ELEslDataID      = 0u;
    uint8               ELEenumType      = 0u;  // data type for enum signals
    uint8*              ELEparamAddress  = NULL_PTR(uint8*);
    
    const char8*        ELEclassName;
    const char8*        ELEcTypeName;
    StreamString        ELEMARTeTypeName;
    uint8               ELEnumDims;
    uint32              ELEdimArrayIdx;
    rtwCAPI_Orientation ELEorientation;
    
    uint32              ELEsize         = 1u;
    uint32              ELEMARTeNumDims = 0u;   // number of dimensions according to MARTe standard
    
    StreamString        fullPathName;
    TypeDescriptor      ELEtype;
    
    SimulinkSignal*     currentSignal;

    switch (mode) {
        
        case SignalFromElementMap:
            // Signal data is retrieved from the signal data structure
            // (this case applies to all other cases w.r.t. the one below)
            
            ELEdataTypeIndex     = rtwCAPI_GetElementDataTypeIdx (elementMap,  sigIdx);
            ELEelementName       = rtwCAPI_GetElementName        (elementMap,  sigIdx);
            ELEdimIndex          = rtwCAPI_GetElementDimensionIdx(elementMap,  sigIdx);
            ELEelementOffset     = rtwCAPI_GetElementOffset      (elementMap,  sigIdx);
            ELEslDataID          = rtwCAPI_GetDataTypeSLId       (dataTypeMap, ELEdataTypeIndex);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize       (dataTypeMap, ELEdataTypeIndex);
            
            ELEparamAddress      = static_cast<uint8*>(startAddress) + ELEelementOffset;
            
            break;
            
        case SignalFromSignals:
            // Signal data is retrieved from main CAPI signal data structure
            // (this case applies to not structured parameters at root level of the tree)
            
            ELEdataTypeIndex     = rtwCAPI_GetSignalDataTypeIdx (sigGroup,    sigIdx);
            ELEelementName       = rtwCAPI_GetSignalName        (sigGroup,    sigIdx);
            ELEdimIndex          = rtwCAPI_GetSignalDimensionIdx(sigGroup,    sigIdx);
            ELEaddrIdx           = rtwCAPI_GetSignalAddrIdx     (sigGroup,    sigIdx);
            ELEslDataID          = rtwCAPI_GetDataTypeSLId       (dataTypeMap, ELEdataTypeIndex);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize      (dataTypeMap, ELEdataTypeIndex);
            
            ELEparamAddress      = static_cast<uint8*>( rtwCAPI_GetDataAddress(dataAddrMap, ELEaddrIdx) );
            
            // ELEelementOffset remains 0 since root level parameters have their address directly specified in the dataAddrMap structure
            
            currentPort->address     = ELEparamAddress;
            currentPort->baseAddress = ELEparamAddress;
            
            break;
            
        default:
            REPORT_ERROR(ErrorManagement::FatalError, "Wrong signal mode in SimulinkWrapperGAM::ScanSignal(), depth %u", depth);
            ok = false;
    }
    
    if (ok) {
        
        // Type
        ELEcTypeName    = rtwCAPI_GetDataTypeCName (dataTypeMap,ELEdataTypeIndex);
        ELEclassName    = rtwCAPI_GetDataTypeMWName(dataTypeMap, ELEdataTypeIndex);
#ifdef ENUM_FEATURE
        ELEenumType     = rtwCAPI_GetDataEnumStorageType(dataTypeMap, ELEdataTypeIndex); // Add enum support only if available (from version 2019a onwards)
#else
        ELEenumType    = 127u; // Invalid datatype
#endif
        
        /*lint -e{1924, 9117} SS_ENUM_TYPE is defined as (uint8_T)(255U - 1) in the C APIs, C-style cast and signedness change cannot be removed */
        if (ELEslDataID == SS_ENUM_TYPE) {
            // if the signal is an enum the typename is "numeric" and
            // the underlying datatype is stored in enumStorageType
            ELEMARTeTypeName = GetMARTeTypeNameFromEnumeratedTypes(ELEenumType);
            ELEtype          = TypeDescriptor::GetTypeDescriptorFromTypeName(ELEMARTeTypeName.Buffer());
#ifndef ENUM_FEATURE
            REPORT_ERROR(ErrorManagement::Warning, "Signal %s: usage of enumeration type requires a higher version.", fullPathName.Buffer());
#endif
        }
        else {
            ELEMARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEcTypeName);
            ELEtype          = TypeDescriptor::GetTypeDescriptorFromTypeName(ELEMARTeTypeName.Buffer());
        }
        
        // Dimensions
        ELEnumDims           = rtwCAPI_GetNumDims(dimMap, ELEdimIndex);
        ELEdimArrayIdx       = rtwCAPI_GetDimArrayIndex(dimMap, ELEdimIndex);
        ELEorientation       = rtwCAPI_GetOrientation(dimMap, ELEdimIndex);
        
        Vector<uint32> ELEactualDimensions(ELEnumDims); 

        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            
            /*lint -e{679} uint32 used as index of uint32[] is ok */
            ELEactualDimensions[dimIdx] = dimArray[ELEdimArrayIdx + dimIdx];
            ELEsize *= ELEactualDimensions[dimIdx];
        }

        // Calculate number of dimensions in MARTe2 sense
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            if (ELEactualDimensions[dimIdx] > 1u) {
                ELEMARTeNumDims++;
            }
        }

        if (mode == SignalFromSignals) {
            currentPort->CAPISize = ELEsize*ELEdataTypeSize;
            currentPort->byteSize = static_cast<uint32>(ELEsize)*ELEdataTypeSize; 
        }

        fullPathName =  baseName.Buffer();
        fullPathName += ELEelementName;
        
        uint64 deltaAddress;
        if (currentPort->lastSignalAddress != NULL) {
            deltaAddress = reinterpret_cast<uint64>(ELEparamAddress) - reinterpret_cast<uint64>(currentPort->lastSignalAddress);
        }
        else {
            deltaAddress = 0ul;
        }
        currentPort->lastSignalAddress = ELEparamAddress;

        // Tree view
        // Print info about the scanned parameter
        if (verbosityLevel > 1u) {
            
            // add spaces to align names
            StreamString alignedName = spacer;
            alignedName += ELEelementName;
            while ( alignedName.Size() < (maxVariableNameLentgh + (spacer.Size()/2u) ) ) {
                alignedName += " ";
            }
            
            StreamString signalInfoString = "";
            ok = signalInfoString.Printf("%s, offset %d, type %-7s (%d bytes), ndims %d, dims [",
                alignedName.Buffer(), ELEelementOffset, ELEcTypeName, ELEdataTypeSize, ELEMARTeNumDims);
            
            if (ok) {
                ok = signalInfoString.Printf("%d", ELEactualDimensions[0u]);
            
                for (uint32 dimIdx = 1u; (dimIdx < ELEnumDims) && ok; dimIdx++) {
                    ok = signalInfoString.Printf(",%d", ELEactualDimensions[dimIdx]);
                }
                
                // TODO printf version used to erase the leading zeros in front of the pointer
                ok = signalInfoString.Printf("], addr: %p, pr sig delta: %d, orient: ", ELEparamAddress, deltaAddress);
                signalInfoString += GetOrientationName(ELEorientation);
            }
            
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, signalInfoString.Buffer());
            }
        }
        
        currentPort->typeBasedSize += static_cast<uint64>(ELEsize)*ELEdataTypeSize;
        currentPort->totalNumberOfElements += ELEsize;
        currentPort->numberOfDimensions = ELEMARTeNumDims;
        
        if (!currentPort->isTyped) {
            // this is first signal encountered in this port, set type
            currentPort->className     = ELEclassName;
            currentPort->cTypeName     = ELEcTypeName;
            currentPort->MARTeTypeName = ELEMARTeTypeName;
            currentPort->type          = ELEtype;
            currentPort->dataTypeSize  = ELEdataTypeSize;
            currentPort->orientation   = ELEorientation;
            for(uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
                currentPort->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
            }
            
            currentPort->isTyped = true;
        }
        else {
            // not the first signal, check coherence with previous ones
            
            if ( StreamString(ELEcTypeName) != currentPort->cTypeName ) {
                currentPort->className     = "struct";
                currentPort->cTypeName     = "unsigned char";
                currentPort->MARTeTypeName = "uint8";
                currentPort->type          = TypeDescriptor::GetTypeDescriptorFromTypeName("uint8");

                currentPort->hasHomogeneousType = false;
            }
            
            if (nonVirtualBusMode == ByteArrayBusMode) {


                if (ELEorientation != currentPort->orientation) {
                    currentPort->hasHomogeneousOrientation = false;
                }

                for(uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {

                    currentPort->numberOfElements[dimIdx] = 1u;
                }
            }

        }
        
        currentSignal = new SimulinkSignal();
        
        currentSignal->fullName              = fullPathName;
        
        currentSignal->orientation           = ELEorientation;
        currentSignal->numberOfDimensions    = ELEMARTeNumDims;
        currentSignal->totalNumberOfElements = ELEsize;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            currentSignal->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
        }
        
        currentSignal->className     = ELEclassName;
        currentSignal->cTypeName     = ELEcTypeName;
        currentSignal->MARTeTypeName = ELEMARTeTypeName;
        currentSignal->type          = ELEtype;
        currentSignal->dataTypeSize  = ELEdataTypeSize;
        
        //Address and size propagated to the signal
        currentSignal->address  = ELEparamAddress;
        currentSignal->byteSize = ELEsize * ELEdataTypeSize;
        currentSignal->offset   = baseOffset + ELEelementOffset;
        
        currentSignal->verbosity   = verbosityLevel;

        ok = currentPort->AddSignal(currentSignal);
    }
    
    return ok;
}

bool SimulinkWrapperGAM::CheckrtwCAPITypeAgainstSize(StreamString cTypeName, const uint16 checkSize) const
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

bool SimulinkWrapperGAM::MapPorts(const SignalDirection direction) {
    
    bool ok    = false;
    bool found = false;
    
    uint32 portIdx  = 0u;
    uint32 signalInPortIdx = 0u;
    uint32 startIdx = 0u;
    uint32 endIdx   = 0u;
    uint32 numberOfSignals = 0u;
    
    StreamString directionName = "";
    
    StreamString   GAMSignalName;
    uint32         GAMNumberOfElements   = 0u;
    uint32         GAMNumberOfDimensions = 0u;
    TypeDescriptor GAMSignalType;
    
    if (direction == InputSignals) {
        numberOfSignals = numberOfInputSignals;
        startIdx        = 0u;
        endIdx          = modelNumOfInputs;
        directionName   = "Input";
        ok              = true;
    }
    else if (direction  == OutputSignals) {
        numberOfSignals = numberOfOutputSignals;
        startIdx        = modelNumOfInputs;
        endIdx          = modelNumOfInputs + modelNumOfOutputs;
        directionName   = "Output";
        ok              = true;
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Unsupported signal direction in MapPorts()");
        ok = false;
    }
    
    // Check and map input/output ports
    for(uint32 signalIdxLoop = 0u; (signalIdxLoop < numberOfSignals) && ok ; signalIdxLoop++) {
    uint32 signalIdx = signalIdxLoop;

        found = false;
        
        GAMSignalName = "";
        ok = GetSignalName(direction, signalIdx, GAMSignalName);

        //Signal mapping, either 1:1 or port (byte array) based
        portIdx = startIdx;
        while((!found) && (portIdx < endIdx)) {
            uint32 portCarriedSignalsCount = modelPorts[portIdx]->carriedSignals.GetSize();

            if(modelPorts[portIdx]->isStructured && (nonVirtualBusMode == StructuredBusMode)) {

                signalInPortIdx = 0u;
                while((!found) && (signalInPortIdx < portCarriedSignalsCount)) {
                    if (GAMSignalName == (modelPorts[portIdx]->carriedSignals[signalInPortIdx]->fullName)) {

                       found = true;
                    }
                    else {
                        signalInPortIdx++;
                    }
                }
            }
            else {
                if (GAMSignalName == (modelPorts[portIdx]->fullName)) {

                    found = true;
                }
            }
            if(!found) {
                portIdx++;
            }
        }
        

        if (!found) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "GAM %s signal %s not found in Simulink model",
                directionName.Buffer(), GAMSignalName.Buffer());
            ok = false;
        }
        
        if (ok) {
            
            // Array signal or structured signal in StructuredBusMode. In this case we check datatype, number of dimensions and number of elements.
            if(modelPorts[portIdx]->hasHomogeneousType || (nonVirtualBusMode == StructuredBusMode)) {
                
                if(!modelPorts[portIdx]->isContiguous)
                {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                        "Simulink port '%s' is homogeneous but not contiguous, mapping not supported.",
                        (modelPorts[portIdx]->fullName).Buffer());
                    ok = false;
                }
                
                if (ok) {
                    ok = GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
                    if ( (GAMNumberOfDimensions != (modelPorts[portIdx]->carriedSignals[signalInPortIdx]->numberOfDimensions)) && ok ) {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                            "%s signal %s number of dimensions mismatch (GAM: %d, model: %u)",
                            directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfDimensions, modelPorts[portIdx]->carriedSignals[signalInPortIdx]->numberOfDimensions);
                        ok = false;
                    }
                }

                if (ok) {
                    ok = GetSignalNumberOfElements(direction, signalIdx, GAMNumberOfElements);
                    if ( (GAMNumberOfElements != (modelPorts[portIdx]->carriedSignals[signalInPortIdx]->totalNumberOfElements)) && ok )
                    {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                                     "%s signal %s number of elements mismatch (GAM: %d, model: %u)",
                                     directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfElements, modelPorts[portIdx]->carriedSignals[signalInPortIdx]->totalNumberOfElements);
                        ok = false;
                    }
                }

                if (ok) {
                    GAMSignalType = GetSignalType(direction, signalIdx);
                    StreamString inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(GAMSignalType);
                    if ( modelPorts[portIdx]->carriedSignals[signalInPortIdx]->MARTeTypeName != inputSignalTypeStr )
                    {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                            "%s signal %s type mismatch (GAM: %s, model: %s)",
                            directionName.Buffer(), GAMSignalName.Buffer(), inputSignalTypeStr.Buffer(), (modelPorts[portIdx]->carriedSignals[signalInPortIdx]->MARTeTypeName).Buffer());
                        ok = false;
                    }
                }

                if (ok) {
                    StreamString actualCTypeName = GetCTypeNameFromMARTeTypeName((modelPorts[portIdx]->carriedSignals[signalInPortIdx]->MARTeTypeName).Buffer());
                    if(!CheckrtwCAPITypeAgainstSize(actualCTypeName.Buffer(), modelPorts[portIdx]->carriedSignals[signalInPortIdx]->dataTypeSize))
                    {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                            "Simulink %s port %s: expected data type size for %s not maching with the actual type size (%u).",
                            directionName.Buffer(), (modelPorts[portIdx]->fullName).Buffer(),
                            modelPorts[portIdx]->carriedSignals[signalInPortIdx]->cTypeName,
                            modelPorts[portIdx]->carriedSignals[signalInPortIdx]->dataTypeSize);
                        ok = false;
                    }
                }

                if (ok) {
                    // Matrix signals in column major orientation requires additional workload.
                    if (GAMNumberOfDimensions > 1u)
                    {
                        if (modelPorts[portIdx]->carriedSignals[signalInPortIdx]->orientation != rtwCAPI_MATRIX_ROW_MAJOR) {
                            
                            modelPorts[portIdx]->requiresTransposition = true;
                            if (verbosityLevel > 0u) {
                                REPORT_ERROR(ErrorManagement::Warning,
                                    "%s signal %s orientation is column-major. Supported, but requires real-time transposition and may result in performance loss.",
                                    directionName.Buffer(), GAMSignalName.Buffer());
                            }
                        }
                    }
                }
            }
            // Structured signal in byte array mode
            else {
                ok = GetSignalNumberOfElements(direction, signalIdx, GAMNumberOfElements);
                if ( (GAMNumberOfElements != (modelPorts[portIdx]->CAPISize)) && ok )
                {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                        "GAM %s signal %s doesn't have the same size (%d) of the corresponding (mixed signals) Simulink port (%d)",
                        directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfElements, modelPorts[portIdx]->CAPISize);
                    ok = false;
                }

                if (ok) {
                    ok = GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
                    if ( (GAMNumberOfDimensions != 1u) && ok )
                    {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                            "%s signal %s dimension mismatch: structured signals in byte array mode must have NumberOfDimensions = 1",
                            directionName.Buffer(), GAMSignalName.Buffer());
                        ok = false;
                    }
                }

                if (ok) {
                    GAMSignalType = GetSignalType(direction, signalIdx);
                    StreamString inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(GAMSignalType);
                    if ( GAMSignalType != UnsignedInteger8Bit )
                    {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                            "GAM %s signal %s has data type (%s), structured signals in byte array mode must be declared as uint8",
                            directionName.Buffer(), GAMSignalName.Buffer(), inputSignalTypeStr.Buffer());
                        ok = false;
                    }
                }
            }

        }
        
        // Ok, here we can map memory inputs
        if (ok) {

            if(modelPorts[portIdx]->isStructured && (nonVirtualBusMode == StructuredBusMode)) {

                if (direction == InputSignals) {

                    modelPorts[portIdx]->carriedSignals[signalInPortIdx]->MARTeAddress = GetInputSignalMemory(signalIdx);
                }
                else if (direction == OutputSignals) {
                    
                    modelPorts[portIdx]->carriedSignals[signalInPortIdx]->MARTeAddress = GetOutputSignalMemory(signalIdx);
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported signal direction in MapPorts()");
                }
            }
            else {
                
                if (direction == InputSignals) {
                    
                    modelPorts[portIdx]->MARTeAddress = GetInputSignalMemory(signalIdx);
                }
                else if (direction == OutputSignals) {
                    
                    modelPorts[portIdx]->MARTeAddress = GetOutputSignalMemory(signalIdx);
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported signal direction in MapPorts()");
                }
            }
        }
    }

    return ok;
}

CLASS_REGISTER(SimulinkWrapperGAM, "1.0")

} /* namespace MARTe */

