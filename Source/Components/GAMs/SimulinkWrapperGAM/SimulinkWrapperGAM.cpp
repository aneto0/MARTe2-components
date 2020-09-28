/**
 * @file SimulinkWrapperGAM.cpp
 * @brief Source file for class SimulinkWrapperGAM
 * @date 14/03/2019
 * @author Bertrand Bauvir, Galperti Cristian
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
#include "StructuredDataI.h"           // CRIADD
#include "SimulinkWrapperGAM.h"
#include "TypeDescriptor.h"

// testing
#include "IntrospectionT.h"
#include "IntrospectionEntry.h"
#include "IntrospectionStructure.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#ifndef UNIT_VERSION_HR
#define UNIT_VERSION_HR "1.0"
#endif

/*
static void PrintIntrospection(const MARTe::char8 * const structOrClassToSearch) {
    using namespace MARTe;

    ClassRegistryDatabase *crdSingleton = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classRegistryItem = crdSingleton->Find(structOrClassToSearch);
    if (classRegistryItem != NULL) {
        //Get the object builder (which knows how to build classes of this type).
        const Introspection *introspection = classRegistryItem->GetIntrospection();
        //Print all the available information.
        if (introspection != NULL) {
            uint32 numberOfMembers = introspection->GetNumberOfMembers();
            uint32 n;
            REPORT_ERROR_STATIC(ErrorManagement::Information, "[%s] number of members: %d:", structOrClassToSearch, numberOfMembers);
            for (n = 0u; n < numberOfMembers; n++) {
                const IntrospectionEntry entry = introspection->operator [](n);
                const char8 * const memberName = entry.GetMemberName();
                const char8 * const memberType = entry.GetMemberTypeName();
                const char8 * const memberModifiers = entry.GetMemberModifiers();
                const char8 * const memberAttributes = entry.GetMemberAttributes();
                REPORT_ERROR_STATIC(ErrorManagement::Information, "[%d] [name]: %s [type]: %-7s [mods]: %s [attrs]: %s", n, memberName, memberType, memberModifiers, memberAttributes);
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "No introspection available for struct or class "
                    "with name: %s", structOrClassToSearch);
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not find struct or class "
                "with name: %s", structOrClassToSearch);
    }
}
*/
void rtwCAPI_GetOrientationName(rtwCAPI_Orientation  &ELEorientation, MARTe::StreamString * name)
{
    switch(ELEorientation)
    {
    case rtwCAPI_SCALAR:
        *name = "scalar";
        break;
    case rtwCAPI_VECTOR:
        *name = "vector";
        break;
    case rtwCAPI_MATRIX_ROW_MAJOR:
        *name = "matrix row major";
        break;
    case rtwCAPI_MATRIX_COL_MAJOR:
        *name = "matrix col major";
        break;
    case rtwCAPI_MATRIX_COL_MAJOR_ND:
        *name = "matrix col major nd";
        break;
    default:
        *name = "N/A";
        break;
    }
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
    
    {"unsigned char" , "uint8"  },
    {"signed char"   , "int8"  },
    {"char"          , "int8"   },
    {"unsigned short", "uint16" },
    {"short"         , "int16"  },
    {"unsigned int"  , "uint32" },
    {"int"           , "int32"  },
    {"float"         , "float32"},
    {"double"        , "float64"},
    {NULL            ,NULL      }
    
};

/**
 * @brief To implement a lookup table between C type names and MARTe type names.
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
    
    {"unsigned char" , 1u   },
    {"signed char"   , 1u   },
    {"char"          , 1u   },
    {"unsigned short", 2u   },
    {"short"         , 2u   },
    {"unsigned int"  , 4u   },
    {"int"           , 4u   },
    {"float"         , 4u   },
    {"double"        , 8u   },
    {NULL            , 0u   }
    
};


static inline const char8* GetMARTeTypeNameFromCTypeName(const char8* cTypeNameIn) {
    
    uint32 lookupIdx = 0u;
    const char8* MARTeTypeNameOut = NULL;
    
    while (typeLookUpTable[lookupIdx].cTypeName != NULL) {
        if ( StringHelper::Compare(typeLookUpTable[lookupIdx].cTypeName, cTypeNameIn) == 0u ) {
            break;
        }
        lookupIdx++;
    }
    
    MARTeTypeNameOut = typeLookUpTable[lookupIdx].MARTeTypeName;
    
    return MARTeTypeNameOut;
}


static inline const char8* GetCTypeNameFromMARTeTypeName(const char8* MARTeTypeNameIn) {
    
    uint32 lookupIdx = 0u;
    const char8* CTypeNameOut = NULL;
    
    while (typeLookUpTable[lookupIdx].MARTeTypeName != NULL) {
        if ( StringHelper::Compare(typeLookUpTable[lookupIdx].MARTeTypeName, MARTeTypeNameIn) == 0u ) {
            break;
        }
        lookupIdx++;
    }
    
    CTypeNameOut = typeLookUpTable[lookupIdx].cTypeName;
    
    return CTypeNameOut;
}


static inline uint16 GetTypeSizeFromCTypeName(const char8* cTypeNameIn) {
    
    uint32 lookupIdx   = 0u;
    uint16 typeSizeOut = 0u;
    
    while (sizeLookUpTable[lookupIdx].cTypeName != NULL) {
        if ( StringHelper::Compare(sizeLookUpTable[lookupIdx].cTypeName, cTypeNameIn) == 0u ) {
            break;
        }
        lookupIdx++;
    }
    
    typeSizeOut = sizeLookUpTable[lookupIdx].size;
    
    return typeSizeOut;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SimulinkWrapperGAM::SimulinkWrapperGAM() :
        GAM() {

    libraryHandle = static_cast<LoadableLibrary*>(NULL);
    
    instFunction  = static_cast<void*(*)(void)>(NULL);
    initFunction  = static_cast<void(*)(void*)>(NULL);
    stepFunction  = static_cast<void(*)(void*)>(NULL);
    
    getMmiFunction = static_cast<void*(*)(void*)>(NULL);

    states               = NULL;
    paramSeparator      = "-";
    signalSeparator     = ".";
    verbosityLevel       = 0u;
    modelNumOfInputs     = 0u;
    modelNumOfOutputs    = 0u;
    modelNumOfParameters = 0u;
    
    skipInvalidTunableParams = true;
    paramsHaveStructArrays   = false;

    return;
}

SimulinkWrapperGAM::~SimulinkWrapperGAM() {

    if (states != NULL) {
        free(states); 
        states = NULL;
    }

    if (libraryHandle != NULL) {
        libraryHandle->Close();
        delete libraryHandle;
        libraryHandle = static_cast<LoadableLibrary*>(NULL);
    }

    getMmiFunction       = static_cast<void*(*)(void*)>(NULL);
    getAlgoInfoFunction  = static_cast<void(*)(void*)>(NULL);

    instFunction = static_cast<void*(*)(void)>(NULL);
    initFunction = static_cast<void(*)(void*)>(NULL);
    stepFunction = static_cast<void(*)(void*)>(NULL);
    
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
    
    return;
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
    
    /// 2. Opening of model code shared object library.
    
    if (status) {
        status = (static_cast<LoadableLibrary*>(NULL) == libraryHandle);
    }

    if (status) { // Load libray
        libraryHandle = new LoadableLibrary ();
        status = (static_cast<LoadableLibrary*>(NULL) != libraryHandle);
    }

    if (status) { // Load libray
        status = libraryHandle->Open(libraryName.Buffer());
        if (status) {
            REPORT_ERROR(ErrorManagement::Information, "Library %s succesfully loaded.", libraryName.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Could'nt open library: %s", libraryName.Buffer());
        }
    }
    
    /// 3. Retrieval of model code symbols.
    
    char symbol [64u];
    
    // instFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
    }

    if (status) { // Find symbol
        instFunction = reinterpret_cast<void*(*)(void)>(libraryHandle->Function(symbol));
        status = (static_cast<void*(*)(void)>(NULL) != instFunction);
        if (!status) {
            REPORT_ERROR(ErrorManagement::Information, "Couldn't find %s symbol in model library (instFunction == NULL).", symbol);
        }
    }

    // getMmiFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
        if (status) {
            status = StringHelper::ConcatenateN(symbol, "_GetCAPImmi", 64u);
        }
    }

    if (status) { // Find symbol
        getMmiFunction = reinterpret_cast<void*(*)(void*)>(libraryHandle->Function(symbol));
        status = (static_cast<void*(*)(void*)>(NULL) != getMmiFunction);
        if (!status) {
            REPORT_ERROR(ErrorManagement::Warning, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
        }
    }

    // initFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
        if (status) {
            status = StringHelper::ConcatenateN(symbol, "_initialize", 64u);
        }
    }

    if (status) { // Find symbol
        initFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(symbol));
        status = (static_cast<void(*)(void*)>(NULL) != initFunction);
        if (!status) {
            REPORT_ERROR(ErrorManagement::Information, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
        }
    }
    
    // stepFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
        if (status) {
            status = StringHelper::ConcatenateN(symbol, "_step", 64u);
        }
    }

    if (status) { // Find symbol
        stepFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(symbol));
        status = (static_cast<void(*)(void*)>(NULL) != stepFunction);
        if (!status) {
            REPORT_ERROR(ErrorManagement::Warning, "Couldn't find %s symbol in model library (%s == NULL).", symbol, symbol);
        }
    }

    // getAlgoInfoFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
        if (status) {
            status = StringHelper::ConcatenateN(symbol, "_GetAlgoInfo", 64u);
        }
    }
    
    if (status) { // Find symbol
        getAlgoInfoFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(symbol));
        status = (static_cast<void(*)(void*)>(NULL) != getAlgoInfoFunction);
        if (!status) {
            REPORT_ERROR(ErrorManagement::Information, "Algorithm information not found in the Simulink .so");
            status = true;
        }
    }

    /// 4. Build a reference container containing parameter values
    ///    retrieved in the configuration file (under the `Parameters` node).
    
    bool hasParameterLeaf = false;
    hasParameterLeaf = data.MoveRelative("Parameters");
    
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
                cfgParameterDatabase.Write(cfgParameterReference->GetName(), absolutePath);
            }
        }
        
        if (!status) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to create parameter database from Parameters node in configuration file.");
        }
        
        data.MoveToAncestor(1u);
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
        REPORT_ERROR(ErrorManagement::Information, "Setup done, now initing the Simulink model");
    }
    else {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "SetupSimulink() failed.");
    }
    
    if (ok) {
        (*initFunction)(states);
    }
    
    // Send simulink ready message
    if (ok) {
        ReferenceT<Message> simulinkReadyMessage = Get(0);
        if (simulinkReadyMessage.IsValid()) {
            REPORT_ERROR(ErrorManagement::Information, "Sending simulink ready message 1.");
            SendMessage(simulinkReadyMessage, this);
        }
    }
    
    return ok;
}

bool SimulinkWrapperGAM::SetupSimulink() {

    REPORT_ERROR(ErrorManagement::Information, "Allocating Simulink model dynamic memory...");

    // Simulink instFunction call, dynamic allocation of model data structures
    states = (*instFunction)();
    bool status = (NULL != states);

    // Simulink initFunction call, init of the Simulink model
    if (!status) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Simulink model allocation function returned a NULL data pointer");
        return false;
    }

    
    // Get the Model Mapping Information (mmi) data structure from the Simulink shared object
    void *mmiTemp = ((*getMmiFunction)(states));
    rtwCAPI_ModelMappingInfo* mmi = reinterpret_cast<rtwCAPI_ModelMappingInfo*>(mmiTemp);
    
    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);

    modelNumOfInputs     = rtwCAPI_GetNumRootInputs(mmi);
    modelNumOfOutputs    = rtwCAPI_GetNumRootOutputs(mmi);
    modelNumOfParameters = rtwCAPI_GetNumModelParameters(mmi);
    
    RTWCAPIV2LOG(ErrorManagement::Information,
        "Simulink C API version number: %d",
        mmi->versionNum);
    
    uint32 numberOfGAMInputSignals  = GetNumberOfInputSignals();
    uint32 numberOfGAMOutputSignals = GetNumberOfOutputSignals();

    // Check number of declared main ports
    if (numberOfGAMInputSignals != modelNumOfInputs) {
        REPORT_ERROR(ErrorManagement::ParametersError,
            "Number of input signals mismatch (GAM: %u, model %u)",
            numberOfGAMInputSignals,  modelNumOfInputs);
        return false;
    }

    if (numberOfGAMOutputSignals != modelNumOfOutputs) {
        REPORT_ERROR(ErrorManagement::ParametersError,
            "Number of output signals mismatch (GAM: %u, model %u)",
            numberOfGAMOutputSignals,  modelNumOfOutputs);
        return false;
    }
    
    ///-------------------------------------------------------------------------
    /// 1. Populate modelParameters and print informations
    ///-------------------------------------------------------------------------
    
    // Scan tunable parameters, print them if vervosity level is enough and
    // build the vector of tunable parameters objects
    REPORT_ERROR(ErrorManagement::Information, "%s, number of main tunable parameters: %d", libraryName.Buffer(), modelNumOfParameters);
    ScanTunableParameters(mmi);

    if (paramsHaveStructArrays) {
        REPORT_ERROR(ErrorManagement::InitialisationError,
            "Arrays of structures detected in the parameters. Feature is not yet supported, execution stopped.");
        return false;
    }
    
    // Max length of parameter names for this model is computed.
    uint64 maxNameLength = 0u;
    StreamString currentName;
    uint32       currentNameSize;
    for(uint32 paramIdx = 0u; paramIdx < modelParameters.GetSize(); paramIdx++) {
        
        currentName     = modelParameters[paramIdx]->fullName;
        currentNameSize = currentName.Size();
        
        if (maxNameLength < currentNameSize) {
            maxNameLength = currentNameSize;
        }
    }
    RTWCAPIV1LOG(ErrorManagement::Information, "%s, configured tunable parameters:", libraryName.Buffer());
    for(uint32 paramIdx = 0u; paramIdx < modelParameters.GetSize(); paramIdx++) {
        modelParameters[paramIdx]->PrintData(maxNameLength);
    }
    
    ///-------------------------------------------------------------------------
    /// 2. Populate modelPorts/modelSignals and print informations
    ///-------------------------------------------------------------------------
    
    // Scan root input/output ports, print them if verbosity level is enough and
    // build the vectors of port objects
    REPORT_ERROR(ErrorManagement::Information, "%s, number of root inputs: %d", libraryName.Buffer(), modelNumOfInputs);
    ScanRootIO(mmi, InputSignals);

    REPORT_ERROR(ErrorManagement::Information, "%s, number of root outputs: %d", libraryName.Buffer(), modelNumOfOutputs);
    ScanRootIO(mmi, OutputSignals);

    maxNameLength = 0u;
    for (uint32 portIdx = 0u; portIdx < modelPorts.GetSize(); portIdx++) {
        
        currentName     = modelPorts[portIdx]->fullName;
        currentNameSize = currentName.Size();
        
        if (maxNameLength < currentNameSize) {
            maxNameLength = currentNameSize;
        }
    }
    RTWCAPIV1LOG(ErrorManagement::Information, "%s, configured input/output ports:", libraryName.Buffer());
    for (uint32 portIdx = 0u; portIdx < modelPorts.GetSize(); portIdx++) {
        modelPorts[portIdx]->PrintPort(maxNameLength);
    }
    
    ///-------------------------------------------------------------------------
    /// 3. Check general coherence between GAM and model
    ///-------------------------------------------------------------------------
    
    // Now check for mismatch in post sizes
    for (uint32 portIdx = 0u; portIdx < modelPorts.GetSize(); portIdx++) {
        
        if (!modelPorts[portIdx]->isContiguous) {
            
            REPORT_ERROR(ErrorManagement::Warning,
                "Port '%s', size from type based scan vs. CAPI size mismatch, possibly uncontiguous structures, check signals offsets carefully",
                (modelPorts[portIdx]->fullName).Buffer());
        }
        
        if (modelPorts[portIdx]->offsetBasedSize != modelPorts[portIdx]->CAPISize) {
            
            REPORT_ERROR(ErrorManagement::InitialisationError,
                "Port '%s', size from offset based scan vs. CAPI size mismatch, port cannot be mapped",
                (modelPorts[portIdx]->fullName).Buffer());
            return false;
        }
        
    }

    ///-------------------------------------------------------------------------
    /// 4. Check input port/signal coherence between GAM and model
    ///    and set copy address
    ///-------------------------------------------------------------------------
    
    if (status) {
        status = MapPorts(InputSignals);
    }
    
    ///-------------------------------------------------------------------------
    /// 5. Check output port/signal coherence between GAM and model
    ///    and set copy address
    ///-------------------------------------------------------------------------
    
    if (status) {
        status = MapPorts(OutputSignals);
    }
    
    ///-------------------------------------------------------------------------
    /// 6. Verify that the external parameter source (if any)
    ///    is compatible with the GAM
    ///-------------------------------------------------------------------------
    
    // The external parameter source must be a ReferenceContainer
    // populated by references to AnyObject.
    
    if (status) {
        
        ReferenceT<ReferenceContainer> mdsPar = ObjectRegistryDatabase::Instance()->Find(tunableParamExternalSource.Buffer());
        
        if (!mdsPar.IsValid()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Tunable parameter loader %s is not valid (maybe not a ReferenceContainer?).", tunableParamExternalSource.Buffer());
        }
        else {
            
            if (status) {
                
                // Loop over all references in the MDSObjLoader container
                for (uint32 objectIdx = 0u; (objectIdx < mdsPar->Size()); objectIdx++) {
                    
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
                        externalParameterDatabase.Write(paramObject->GetName(), parameterAbsolutePath.Buffer());
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
        
        // Based upon the actualisation outcome, execution is continued or stopped.
        
        // Cases in which execution can continue
        if (isLoaded && isActualised) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s correctly actualized from %s.",
                currentParamName, parameterSourceName.Buffer());
        }
        else if (isLoaded && !isActualised && isUnlinked) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s unlinked, using compile time value",
                currentParamName);
        }
        else if (isLoaded && !isActualised && !isUnlinked && skipInvalidTunableParams) {
            REPORT_ERROR(ErrorManagement::Warning,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s cannot be actualized, using compile time value",
                currentParamName);
        }
        else if (!isLoaded && !isActualised && skipInvalidTunableParams) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s not found, using compile time value",
                currentParamName);
        }
        
        // Cases in which execution should be stopped
        else if (!isLoaded && !isActualised && !skipInvalidTunableParams) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s not found, failing",
                currentParamName);
            status = false;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "SkipInvalidTunableParams is false and parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s cannot be actualized, failing",
                currentParamName);
            status = false;
        }

    }
    
    ///-------------------------------------------------------------------------
    /// 8. Print port/signal details
    ///-------------------------------------------------------------------------
    
    // Print ports signals details
    for(uint32 portIdx = 0u; (portIdx < modelNumOfInputs) && status; portIdx++) {
        
        uint32 signalsInThisPort = (modelPorts[portIdx]->carriedSignals).GetSize();
        
        RTWCAPIV2LOGST(ErrorManagement::Information,
            "IN port %s, # signals %d, signal content:",
            (modelPorts[portIdx]->fullName).Buffer(), signalsInThisPort);
        
        for(uint32 signalIdx = 0u; signalIdx < signalsInThisPort; signalIdx++) {
            (modelPorts[portIdx]->carriedSignals[signalIdx])->PrintSignal(40u);
        }
    
    }
    
    for(uint32 portIdx = modelNumOfInputs; (portIdx < modelNumOfInputs + modelNumOfOutputs) && status; portIdx++) {
        
        uint32 signalsInThisPort = (modelPorts[portIdx]->carriedSignals).GetSize();
        
        RTWCAPIV2LOGST(ErrorManagement::Information,
            "OUT port %s, # signals %d, signal content:",
            (modelPorts[portIdx]->fullName).Buffer(), signalsInThisPort);
        
        for(uint32 signalIdx = 0u; signalIdx < signalsInThisPort; signalIdx++) {
            (modelPorts[portIdx]->carriedSignals[signalIdx])->PrintSignal(40u);
        }
    }

    return status;

}


bool SimulinkWrapperGAM::Execute() {
    
    uint32 portIdx;

    // stepFunction must be defined at this point
    bool status = ((NULL != states) && (static_cast<void(*)(void*)>(NULL) != stepFunction));

    // Inputs update
    for (portIdx = 0u; portIdx < modelNumOfInputs; portIdx++) {
        modelPorts[portIdx]->CopyData();
    }
    
    // Model step
    (*stepFunction)(states);

    // Ouputs update
    for (portIdx = modelNumOfInputs; portIdx < modelNumOfInputs + modelNumOfOutputs; portIdx++) {
        modelPorts[portIdx]->CopyData();
    }
    
    return status;

}

bool SimulinkWrapperGAM::ScanTunableParameters(rtwCAPI_ModelMappingInfo* mmi)
{
    bool ok = false;
    
    uint32        nOfParams;
    const char8* paramName;
    uint16      dataTypeIdx;
    uint8       slDataID;
    uint16      numElements;
    uint16      dataTypeSize;
    uint32        addrIdx;
    void*         paramAddress;
    uint16      SUBdimIdx;
    uint8       SUBnumDims;
    uint32        SUBdimArrayIdx;

    nOfParams = rtwCAPI_GetNumModelParameters(mmi);
    if (nOfParams == 0u) {
        RTWCAPIV1LOG(ErrorManagement::Information, "No tunable parameters found.");
    }

    // Populating C API data structure pointers of the class from mmi
    modelParams = rtwCAPI_GetModelParameters(mmi);
    ok = (modelParams != NULL);
    
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

    for(uint32 paramIdx = 0u; (paramIdx < nOfParams) && ok; paramIdx++) {
        
        dataTypeIdx  = rtwCAPI_GetModelParameterDataTypeIdx(modelParams, paramIdx); // Index into the data type in rtwCAPI_DataTypeMap
        paramName    = rtwCAPI_GetModelParameterName(modelParams, paramIdx);        // Name of the parameter
        slDataID     = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);           // Simulink type from data type map
        numElements  = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);     // Number of elements of the strucutre data type
        dataTypeSize = rtwCAPI_GetDataTypeSize(dataTypeMap,dataTypeIdx);            // Size of the datatype in bytes, WARNING: 16 bits maximum !!
        
        // clear lastAddressVector
        while (lastAddressVector.GetSize() != 0u) {
            lastAddressVector.Remove(0u);
        }
        paramlastaddress = NULL;
        
        
        if(slDataID != SS_STRUCT) {
            
            // Not structured parameter, directly print it from main params structure
            ok = ScanParameter(paramIdx, "", ParamFromParameters, NULL, "", 0u, 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameter for parameter %s.", paramName);
            }
        }
        else {
            
            // Structured parameters, descend the tree
            addrIdx         = rtwCAPI_GetModelParameterAddrIdx(modelParams,paramIdx);
            paramAddress    = (void *) rtwCAPI_GetDataAddress(dataAddrMap,addrIdx);
            SUBdimIdx       = rtwCAPI_GetModelParameterDimensionIdx(modelParams, paramIdx);
            SUBnumDims      = rtwCAPI_GetNumDims(dimMap, SUBdimIdx);
            SUBdimArrayIdx  = rtwCAPI_GetDimArrayIndex(dimMap, SUBdimIdx);

            StreamString diminfo = "[";
            bool structarray = false;
            
            for(uint32 idx2 = 0u; idx2 < SUBnumDims; idx2++)
            {
                
                if(dimArray[SUBdimArrayIdx + idx2] > 1u) {
                    structarray = true;
                }
                
                diminfo.Printf("%u", dimArray[SUBdimArrayIdx + idx2]);
                if (idx2 != SUBnumDims - 1u) {
                    diminfo += ",";
                }
            }
            
            diminfo += "]";

            uint64 absDeltaAddress;
            currentParamBaseAddress = paramAddress;
            absDeltaAddress = reinterpret_cast<uint64>(paramAddress) - reinterpret_cast<uint64>(currentParamBaseAddress);

            RTWCAPIV2LOG(ErrorManagement::Information,
                    "%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, struct with %d elems, size(u16!): %d bytes, base addr: %p, dims: %s",
                   paramName, numElements, dataTypeSize, paramAddress, diminfo.Buffer());
            
            StreamString paramNameAndSeparator = StreamString(paramName);
            paramNameAndSeparator += paramSeparator;
            
            ok = ScanParametersStruct(dataTypeIdx, 1u, (uint8*) paramAddress, paramNameAndSeparator, absDeltaAddress, "");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameterStruct for parameter %s.", paramName);
            }
            
            if (structarray) {
                RTWCAPIV2LOG(ErrorManagement::Warning, "Array of structure detected (%s), not yet supported! Please flatten the previous params structure.", paramName);
                paramsHaveStructArrays = true;
            }
        }
    }
    
    return ok;
}

bool SimulinkWrapperGAM::ScanParametersStruct(uint32 dataTypeIdx, uint32 depth, void *startaddr, StreamString basename, uint32 baseoffset, StreamString spacer)
{
    bool ok = true;
    
    const char8* elementName;
    uint16       numElements;
    uint16       elemMapIdx;
    uint16       SUBdataTypeIndex;
    uint8        SUBslDataID;
    uint16       SUBnumElements;
    uint32       SUBdataTypeOffset;
    void*        byteptr = startaddr;
    void*        runningbyteptr = startaddr;
    StreamString tempstr;
    uint16       SUBdimIdx;
    uint8        SUBnumDims;
    uint32       SUBdimArrayIdx;
    uint16       SUBdataTypeSize;

    elemMapIdx  = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap,dataTypeIdx);
    numElements = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);

    StreamString specificSpacer; // spacer of the current parameter
    StreamString passoverSpacer; // spacer that will be passed to a recursive call

    for(uint32 elemIdx = 0u; (elemIdx < numElements) && ok; elemIdx++) {
        
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
        if (elemIdx == numElements - 1u) {
            specificSpacer += "└ ";
        } 
        else {
            specificSpacer += "├ ";
        }
        
        StreamString diminfo = "[";
        bool structarray = false;
        for (uint32 elemIdx2 = 0u; elemIdx2 < SUBnumDims; elemIdx2++)
        {
            if (dimArray[SUBdimArrayIdx + elemIdx2] > 1u) {
                structarray = true;
            }
            diminfo.Printf("%d", dimArray[SUBdimArrayIdx + elemIdx2]);
            if (elemIdx2 != SUBnumDims - 1u) {
                diminfo += ",";
            }
        }
        diminfo += "]";

        if (SUBslDataID != SS_STRUCT) {
            
            ok = ScanParameter(elemMapIdx+elemIdx, specificSpacer, ParamFromElementMap, byteptr, basename, baseoffset, depth);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanParameter for parameter %s.", elementName);
            }
        }
        else {

            runningbyteptr = reinterpret_cast<void *>(static_cast<uint8*>(startaddr) + SUBdataTypeOffset);

            // Calculating same level delta address
            uint64 deltaaddr;
            if (lastAddressVector.GetSize() < depth)
            {
                lastAddressVector.Add(runningbyteptr);
                deltaaddr = 0u;
            }
            else if (lastAddressVector.GetSize() > depth)
            {
                while (lastAddressVector.GetSize() == depth) {
                    lastAddressVector.Remove(lastAddressVector.GetSize() - 1u);
                }
                deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
            }
            else
            {
                if(elemIdx == 0u)
                {
                    lastAddressVector.Set(depth - 1u, runningbyteptr);
                    deltaaddr = 0u;
                }
                else
                {
                    deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
                }
            }

            // Calculating absolute delta address
            uint64 absDeltaAddress;
            absDeltaAddress = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(currentParamBaseAddress);

            RTWCAPIV2LOG(ErrorManagement::Information,
                    "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, nested struct (idx %d) with %d elems, size(u16!): %d, offset: %d, base addr: %p, same lvl delta: %d, abs delta: %d, dims: %s",
                    specificSpacer.Buffer(), elementName, elemIdx, SUBnumElements, SUBdataTypeSize, SUBdataTypeOffset, runningbyteptr, deltaaddr, absDeltaAddress, diminfo.Buffer());
            tempstr=elementName;
            if(structarray)
            {
                RTWCAPIV2LOG(ErrorManagement::Warning, "Array of structure detected, not yet supported! Please flatten the previous params structure!");
                paramsHaveStructArrays = true;
            }
            
            StreamString nameAndSeparators = basename;
            nameAndSeparators += tempstr;
            nameAndSeparators += paramSeparator;
            
            if (elemIdx == numElements - 1u) { 
                passoverSpacer += "  "; 
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

bool SimulinkWrapperGAM::ScanParameter(uint32 paridx, StreamString spacer, ParameterMode mode, void* startaddr, StreamString basename, uint32 baseoffset, uint32 depth)
{
    bool ok = true;
    
    const char8*        ELEelementName;
    uint32              ELEelementOffset;
    uint16              ELEdataTypeIndex;
    uint16              ELEdimIndex;
    uint8               ELEnumDims;
    uint32              ELEdimArrayIdx;
    rtwCAPI_Orientation ELEorientation;

    const char8*        ELEctypename;
    uint32              ELEaddrIdx;
    uint8*              ELEparamAddress;
    uint16              ELEdataTypeSize;
    uint32              ELEsize;
    uint32              ELEelements;
    StreamString        fullpathname;
    StreamString        ELEtypename;
    uint32              ELEMARTeNumDims = 0u;   // number of dimensions according to MARTe standard
    
    switch(mode) {
        
        case ParamFromElementMap: {
            
            // Parameters data is retrieved from the elements data structure
            // (this case applies to all other cases w.r.t. the case below)
            ELEdataTypeIndex     = rtwCAPI_GetElementDataTypeIdx(elementMap, paridx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize(dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetElementName(elementMap, paridx);
            ELEelementOffset     = rtwCAPI_GetElementOffset(elementMap, paridx);
            ELEdimIndex          = rtwCAPI_GetElementDimensionIdx(elementMap, paridx);
            ELEparamAddress      = static_cast<uint8*>(startaddr)+ELEelementOffset;
            
            break;
        }
        
        case ParamFromParameters: {
            
            // Parameter data is retrieved from main CAPI parameters data structure
            // (this case applies to not structured parameters at root level of the tree)
            ELEdataTypeIndex     = rtwCAPI_GetModelParameterDataTypeIdx(modelParams, paridx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize(dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetModelParameterName(modelParams, paridx);
            ELEelementOffset     = 0u; // root level parameters have their address directly specified in the dataAddrMap structure
            ELEdimIndex          = rtwCAPI_GetModelParameterDimensionIdx(modelParams, paridx);
            
            ELEaddrIdx           = rtwCAPI_GetModelParameterAddrIdx(modelParams,paridx);
            ELEparamAddress      = (uint8 *) rtwCAPI_GetDataAddress(dataAddrMap,ELEaddrIdx);
            
            break;
        }
        
        default:
            REPORT_ERROR(ErrorManagement::FatalError, "Wrong parameter mode in SimulinkWrapperGAM::ScanParameter()");
            ok = false;
            
    }

    if (ok) {
        ELEctypename         = rtwCAPI_GetDataTypeCName(dataTypeMap, ELEdataTypeIndex);
        ELEnumDims           = rtwCAPI_GetNumDims      (dimMap,      ELEdimIndex);      // not number of dimensions in MARTe2 sense, but number of slots occupied in the dimension array
        ELEdimArrayIdx       = rtwCAPI_GetDimArrayIndex(dimMap,      ELEdimIndex);
        ELEorientation       = rtwCAPI_GetOrientation  (dimMap,      ELEdimIndex);

        Vector<uint32> ELEactualDimensions(ELEnumDims);
        
        ELEsize     = 1u;
        ELEelements = 1u;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++)
        {
            ELEactualDimensions[dimIdx] = dimArray[ELEdimArrayIdx + dimIdx];
            ELEelements *= ELEactualDimensions[dimIdx];
        }
        
        // Calculate number of dimensions in MARTe2 sense
        ELEMARTeNumDims = 0u;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            if (ELEactualDimensions[dimIdx] > 1u) {
                ELEMARTeNumDims++;
            }
        }
        
        ELEsize=ELEelements*ELEdataTypeSize;

        fullpathname  = basename.Buffer();
        fullpathname += ELEelementName;
        
        // Tree view
        StreamString paramInfoString = "";
        StreamString orientationName = "";
        paramInfoString.Printf(
            "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, offset %d, type %-6s (%d bytes), ndims %d, dims [",
            spacer.Buffer(), ELEelementName, ELEelementOffset, ELEctypename, ELEdataTypeSize, ELEMARTeNumDims);
        
        paramInfoString.Printf("%d", ELEactualDimensions[0u]);
        
        for (uint32 dimIdx = 1u; dimIdx < ELEnumDims; dimIdx++) {
            paramInfoString.Printf(",%d", ELEactualDimensions[dimIdx]);
        }
        uint32 deltaAddress;
        if(paramlastaddress != NULL)
            deltaAddress = reinterpret_cast<uint64>(ELEparamAddress) - reinterpret_cast<uint64>(paramlastaddress);
        else
            deltaAddress = 0;
        paramlastaddress = ELEparamAddress;
        
        // TODO printf version used to erase the leading zeros in front of the pointer
        paramInfoString.Printf("], addr: %p, pr par delta: %d, orient: ",ELEparamAddress, deltaAddress);
        rtwCAPI_GetOrientationName(ELEorientation, &orientationName);
        
        paramInfoString += orientationName;
        RTWCAPIV2LOG(ErrorManagement::Information, paramInfoString.Buffer());
        
        ELEtypename = ELEctypename;
        
        // Parameter that is currently being updated.
        SimulinkParameter* currentParameter = new SimulinkParameter();
        
        currentParameter->fullName      = fullpathname;
        
        currentParameter->numberOfDimensions = ELEMARTeNumDims;
        currentParameter->totalNumberOfElements = ELEelements;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            currentParameter->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
        }
        currentParameter->orientation   = ELEorientation;
        
        currentParameter->byteSize      = ELEsize;
        currentParameter->dataTypeSize  = ELEdataTypeSize;
        
        currentParameter->cTypeName     = ELEctypename;
        currentParameter->MARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEtypename.Buffer());
        currentParameter->type          = TypeDescriptor::GetTypeDescriptorFromTypeName((currentParameter->MARTeTypeName).Buffer());
        
        currentParameter->address       = ELEparamAddress;
        
        ok = modelParameters.Add(currentParameter);
    }
    
    return ok;
}

bool SimulinkWrapperGAM::ScanRootIO(rtwCAPI_ModelMappingInfo* mmi, SignalDirection mode)
{
    bool ok = false;
    
    uint32       nOfSignals;
    const char8* sigName;
    uint16       dataTypeIdx;
    uint8        slDataID;
    uint16       numElements;
    uint16       dataTypeSize;
    uint32       addrIdx;
    void*        sigAddress;
    StreamString stemp;

    // Populating C API data structure pointers of the class from mmi
    rootInputs = rtwCAPI_GetRootInputs(mmi);
    ok = (rootInputs != NULL);
    
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
                RTWCAPIV1LOG(ErrorManagement::Information, "No root inputs found");
            }
            sigGroup = rootInputs;
            break;
            
        case OutputSignals:
            nOfSignals = rtwCAPI_GetNumRootOutputs(mmi);
            if (nOfSignals == 0u) {
                RTWCAPIV1LOG(ErrorManagement::Information, "No root outputs found");
            }
            sigGroup = rootOutputs;
            break;
            
        default:
            REPORT_ERROR(ErrorManagement::FatalError, "Wrong signal direction in SimulinkWrapperGAM::ScanRootIO()");
            ok = false;
        }
    }
    
    for (uint32 sigIdx = 0u; (sigIdx < nOfSignals) && ok ; sigIdx++) {
        
        dataTypeIdx  = rtwCAPI_GetSignalDataTypeIdx(sigGroup, sigIdx);              // Index into the data type in rtwCAPI_DataTypeMap
        sigName      = rtwCAPI_GetSignalName(sigGroup, sigIdx);                     // Name of the parameter
        slDataID     = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);           // Simulink type from data type map
        numElements  = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);     // Number of elements of the strucutre data type
        dataTypeSize = rtwCAPI_GetDataTypeSize(dataTypeMap,dataTypeIdx);            // Size of the datatype in bytes, WARNING: 16 bits maximum !!

        // clear lastAddressVector
        while (lastAddressVector.GetSize() != 0u) {
            lastAddressVector.Remove(0u);
        }
        
        switch(mode)
        {
            case InputSignals:
                currentPort = static_cast<SimulinkPort*>(new SimulinkInputPort());
                break;
                
            case OutputSignals:
                currentPort = static_cast<SimulinkPort*>(new SimulinkOutputPort());
                break;
                
            default:
                REPORT_ERROR(ErrorManagement::FatalError, "Wrong mode in SimulinkWrapperGAM::ScanRootIO()");
                ok = false;
        }
        
        if (ok) {
            stemp = sigName;
            currentPort->fullName = stemp;
            currentPort->verbosity = verbosityLevel;

            if (slDataID != SS_STRUCT) {
                // Not structured parameter, directly print it from main params structure
                
                ok = ScanSignal(sigIdx, "", SignalFromSignals, NULL, "", 0u, 1u); // TODO: check if baseoffset 0 is correct
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

                RTWCAPIV2LOG(ErrorManagement::Information, "%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, struct with %d elems, size: %d bytes, base addr: %p, abs delta: %d",
                       sigName, numElements, dataTypeSize, sigAddress, absDeltaAddress);
                
                currentPort->CAPISize = dataTypeSize;
                currentPort->byteSize = dataTypeSize;
                currentPort->address  = sigAddress;
                // TODO: verify if base offset 0 is correct for a root port structure
                StreamString nameAndSeparators = sigName;
                nameAndSeparators += signalSeparator;
                
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

bool SimulinkWrapperGAM::ScanSignalsStruct(uint32 dataTypeIdx, uint32 depth,  void *startaddr, StreamString basename, uint32 baseoffset, StreamString spacer){
    
    bool ok = true;
    
    const char8* elementName;
    uint8        SUBslDataID;
    uint16       numElements;
    uint16       elemMapIdx;
    uint16       SUBdataTypeIndex;
    uint16       SUBnumElements;
    uint32       SUBdataTypeOffset;
    void*        byteptr = startaddr;
    void*        runningbyteptr = startaddr;
    StreamString tempstr;

    elemMapIdx  = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap,dataTypeIdx);
    numElements = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);

    StreamString specificSpacer; // spacer of the current signal
    StreamString passoverSpacer; // spacer that will be passed to a recursive call
    
    for(uint32 elemIdx = 0u; (elemIdx < numElements) && ok; elemIdx++) {
        
        elementName         = rtwCAPI_GetElementName(elementMap, elemIdx+elemMapIdx);
        SUBdataTypeIndex    = rtwCAPI_GetElementDataTypeIdx(elementMap, elemIdx+elemMapIdx);
        SUBdataTypeOffset   = rtwCAPI_GetElementOffset(elementMap, elemIdx+elemMapIdx);
        SUBslDataID         = rtwCAPI_GetDataTypeSLId(dataTypeMap, SUBdataTypeIndex);
        SUBnumElements      = rtwCAPI_GetDataTypeNumElements(dataTypeMap,SUBdataTypeIndex);
        
        
        specificSpacer = spacer; // reset the spacer
        passoverSpacer = spacer;
        if (elemIdx == numElements - 1u) {
            specificSpacer += "└ ";
        } 
        else {
            specificSpacer += "├ ";
        }
        
        // Scan the parameter or structure
        if(SUBslDataID != SS_STRUCT) {
            
            ok = ScanSignal(elemMapIdx+elemIdx, specificSpacer, SignalFromElementMap, byteptr, basename, baseoffset, depth);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanSignal for signal %s.", elementName);
            }
        }
        else {

            runningbyteptr=reinterpret_cast<void *>(static_cast<uint8*>(startaddr)+SUBdataTypeOffset);

            // Calculating same level delta address
            uint64 deltaaddr;
            if (lastAddressVector.GetSize() < depth)
            {
                lastAddressVector.Add(runningbyteptr);
                deltaaddr = 0;
            }
            else if (lastAddressVector.GetSize() > depth)
            {
                // resize lastAddressVector
                while (lastAddressVector.GetSize() == depth) {
                    lastAddressVector.Remove(lastAddressVector.GetSize() - 1u);
                }
                deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
            }
            else
            {
                if (elemIdx == 0u)
                {
                    lastAddressVector.Set(depth - 1u, runningbyteptr);
                    deltaaddr = 0;
                }
                else
                {
                    deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
                }
            }

            // Calculating absolute delta address
            uint64 absDeltaAddress;
            absDeltaAddress = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(currentPort->baseAddress);
            
            RTWCAPIV2LOG(ErrorManagement::Information, "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, nested struct (idx %d) with %d elems, offset: %d, base addr: %p, same level delta: %d, abs delta: %d",
                    specificSpacer.Buffer(), elementName, elemIdx, SUBnumElements, SUBdataTypeOffset, runningbyteptr, deltaaddr, absDeltaAddress);
            tempstr=elementName;
            
            StreamString nameAndSeparators = basename;
            nameAndSeparators += tempstr;
            nameAndSeparators += signalSeparator;
            
            if (elemIdx == numElements - 1u) { 
                passoverSpacer += "  "; 
            } else {
                passoverSpacer += "┆ ";
            }
            
            ok = ScanSignalsStruct(SUBdataTypeIndex, depth+1, runningbyteptr, nameAndSeparators, absDeltaAddress, passoverSpacer);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed ScanSignalStruct for signal %s.", elementName);
            }
            
        }
    }
    
    return ok;
}

bool SimulinkWrapperGAM::ScanSignal(uint32 sigidx, StreamString spacer, SignalMode mode, void *startaddr, StreamString basename, uint32 baseoffset, uint32 depth)
{
    bool ok = true;
    
    const char8*        ELEelementName;
    uint32              ELEelementOffset;
    uint16              ELEdataTypeIndex;
    uint16              ELEdimIndex;
    uint8               ELEnumDims;
    uint32              ELEdimArrayIdx;
    rtwCAPI_Orientation ELEorientation;
    
    const char8*        ELEctypename;
    uint32              ELEaddrIdx;
    uint8*              ELEparamAddress;
    uint16              ELEdataTypeSize;
    uint32              ELEsize;
    StreamString        fullpathname;
    uint32              ELEMARTeNumDims = 0u;   // number of dimensions according to MARTe standard
    
    SimulinkSignal*     currentSignal;

    switch (mode) {
        
        case SignalFromElementMap:
            // Signal data is retrieved from the signal data structure
            // (this case applies to all other cases w.r.t. the one below)
            
            ELEdataTypeIndex     = rtwCAPI_GetElementDataTypeIdx(elementMap, sigidx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize(dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetElementName(elementMap, sigidx);
            ELEdimIndex          = rtwCAPI_GetElementDimensionIdx(elementMap, sigidx);
            ELEelementOffset     = rtwCAPI_GetElementOffset(elementMap, sigidx);
            
            ELEparamAddress      = static_cast<uint8*>(startaddr) + ELEelementOffset;
            
            break;
            
        case SignalFromSignals:
            // Signal data is retrieved from main CAPI signal data structure
            // (this case applies to not structured parameters at root level of the tree)
            
            ELEdataTypeIndex     = rtwCAPI_GetSignalDataTypeIdx (sigGroup,    sigidx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize      (dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetSignalName        (sigGroup,    sigidx);
            ELEdimIndex          = rtwCAPI_GetSignalDimensionIdx(sigGroup,    sigidx);
            ELEelementOffset     = 0u;           // root level parameters have their address directly specified in the dataAddrMap structure
            
            ELEaddrIdx           = rtwCAPI_GetSignalAddrIdx(sigGroup, sigidx);
            ELEparamAddress      = (uint8 *) rtwCAPI_GetDataAddress(dataAddrMap, ELEaddrIdx);
            
            currentPort->address     = ELEparamAddress;
            currentPort->baseAddress = ELEparamAddress;
            
            break;
            
        default:
            REPORT_ERROR(ErrorManagement::FatalError, "Wrong signal mode in SimulinkWrapperGAM::ScanSignal()");
            ok = false;
    }
    
    if (ok) {
        ELEctypename         = rtwCAPI_GetDataTypeCName(dataTypeMap,ELEdataTypeIndex);
        ELEnumDims           = rtwCAPI_GetNumDims(dimMap, ELEdimIndex);
        ELEdimArrayIdx       = rtwCAPI_GetDimArrayIndex(dimMap, ELEdimIndex);
        ELEorientation       = rtwCAPI_GetOrientation(dimMap, ELEdimIndex);
        
        Vector<uint32> ELEactualDimensions(ELEnumDims); 

        ELEsize=1;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            
            ELEactualDimensions[dimIdx] = dimArray[ELEdimArrayIdx + dimIdx];
            ELEsize *= ELEactualDimensions[dimIdx];
        }

        // Calculate number of dimensions in MARTe2 sense
        ELEMARTeNumDims = 0;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            if (ELEactualDimensions[dimIdx] > 1) {
                ELEMARTeNumDims++;
            }
        }

        if(mode == SignalFromSignals) {
            currentPort->CAPISize = ELEsize*ELEdataTypeSize;
            currentPort->byteSize = ELEsize*ELEdataTypeSize; 
        }


        fullpathname =  basename.Buffer();
        fullpathname += ELEelementName;

        StreamString paramInfoString = "";
        StreamString orientationName = "";
        paramInfoString.Printf(
            "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, offset %d, type %s (%d bytes), ndims %d, dims [",
            spacer.Buffer(), ELEelementName, ELEelementOffset, ELEctypename, ELEdataTypeSize, ELEMARTeNumDims);
        
        paramInfoString.Printf("%d", ELEactualDimensions[0]);
        
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
        
            paramInfoString.Printf(",%d", ELEactualDimensions[dimIdx]);
        }
        uint32 deltaAddress;
        if (currentPort->lastSignalAddress != NULL) {
            deltaAddress = reinterpret_cast<uint64>(ELEparamAddress) - reinterpret_cast<uint64>(currentPort->lastSignalAddress);
        }
        else {
            deltaAddress = 0u;
        }
        currentPort->lastSignalAddress = ELEparamAddress;
        
        // TODO printf version used to erase the leading zeros in front of the pointer
        paramInfoString.Printf("], addr: %p, pr sig delta: %d, orient: ",ELEparamAddress, deltaAddress);
        rtwCAPI_GetOrientationName(ELEorientation, &orientationName);
        
        paramInfoString += orientationName;
        RTWCAPIV2LOG(ErrorManagement::Information, paramInfoString.Buffer());
        
        
        currentPort->typeBasedSize += ELEsize*ELEdataTypeSize;
        currentPort->totalNumberOfElements += ELEsize;
        currentPort->numberOfDimensions = ELEMARTeNumDims;
        
        StreamString sstemp;
        sstemp = ELEctypename;
        
        if (!currentPort->isTyped) {
            // this is first signal encountered in this port, set type
            currentPort->cTypeName     = sstemp;
            currentPort->MARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEctypename);
            currentPort->type          = TypeDescriptor::GetTypeDescriptorFromTypeName(GetMARTeTypeNameFromCTypeName(ELEctypename));
            currentPort->dataTypeSize  = ELEdataTypeSize;
            currentPort->orientation   = ELEorientation;
            for(uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
                currentPort->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
            }
            
            
            currentPort->isTyped = true;
        }
        else {
            // not the first signal, check coherence with previous ones
            if (sstemp != currentPort->cTypeName) {
                currentPort->cTypeName     = "unsigned char";
                currentPort->MARTeTypeName = "uint8";
                currentPort->type          = TypeDescriptor::GetTypeDescriptorFromTypeName("uint8");
                
                currentPort->hasHomogeneousType = false;
            }
            
            if (ELEorientation != currentPort->orientation) {
                currentPort->hasHomogeneousOrientation = false;
            }
            
            for(uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
                currentPort->numberOfElements[dimIdx] = 1u;
            }
        }
        
        currentSignal = new SimulinkSignal();
        
        currentSignal->fullName              = fullpathname;
        
        currentSignal->numberOfDimensions    = ELEMARTeNumDims;
        currentSignal->totalNumberOfElements = ELEsize;
        for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
            currentSignal->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
        }
        
        currentSignal->cTypeName     = ELEctypename; 
        currentSignal->MARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEctypename);
        currentSignal->type          = TypeDescriptor::GetTypeDescriptorFromTypeName((currentSignal->MARTeTypeName).Buffer());

        currentSignal->dataTypeSize  = ELEdataTypeSize;
        
        currentSignal->offset        = baseoffset + ELEelementOffset;
        
        ok = currentPort->AddSignal(currentSignal);
    }
    
    return ok;
}

bool SimulinkWrapperGAM::CheckrtwCAPITypeAgainstSize(StreamString rtwCAPItype, uint16 checksize)
{
    return GetTypeSizeFromCTypeName(rtwCAPItype.Buffer()) == checksize;
}

void SimulinkWrapperGAM::PrintAlgoInfo() {
    
    SimulinkAlgoInfo info;

    if ( (static_cast<void(*)(void*)>(NULL) == getAlgoInfoFunction) ) {
        info.expCode = 0u;
        StringHelper::Copy(info.gitHash, "00000000\0");
        StringHelper::Copy(info.gitLog,  "\0");
    }
    else {
        (*getAlgoInfoFunction)((void*) &info);
    }
    
    REPORT_ERROR(ErrorManagement::Information,"Algorithm expcode : %d", info.expCode);
    REPORT_ERROR(ErrorManagement::Information,"Algorithm git hash: %s", info.gitHash);
    REPORT_ERROR(ErrorManagement::Information,"Algorithm git log : %s", info.gitLog);


}

bool SimulinkWrapperGAM::MapPorts(SignalDirection direction) {
    
    bool ok    = false;
    bool found = false;
    
    uint32 portIdx  = 0u;
    uint32 startIdx = 0u;
    uint32 endIdx   = 0u;
    uint32 numberOfSignals = 0u;
    
    StreamString directionName = "";
    
    StreamString   GAMSignalName;
    uint32         GAMNumberOfElements;
    uint32         GAMNumberOfDimensions;
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
    
    // Check and map input ports
    for(uint32 signalIdx = 0u; (signalIdx < numberOfSignals) && ok ; signalIdx++) {
        
        GAMSignalName = "";
        GetSignalName(direction, signalIdx, GAMSignalName);
        found = false;
        
        for( portIdx = startIdx; portIdx < endIdx; portIdx++ ) {
            
            if (GAMSignalName == (modelPorts[portIdx]->fullName))
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "GAM %s signal %s not found in Simulink model",
                directionName.Buffer(), GAMSignalName.Buffer());
            return false;
        }
        
        if(modelPorts[portIdx]->hasHomogeneousType)
        {
            // Homogeneus port checks (an array). In this case we check datatype, number of dimensions and number of elements.
            
            if(!modelPorts[portIdx]->isContiguous)
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "Simulink port '%s' is homogeneous but not contiguous, mapping not supported.",
                    (modelPorts[portIdx]->fullName).Buffer());
                return false;
            }
            
            GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
            if (GAMNumberOfDimensions != (modelPorts[portIdx]->numberOfDimensions))
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "%s signal %s number of dimensions mismatch (GAM: %d, model: %u)",
                    directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfDimensions, modelPorts[portIdx]->numberOfDimensions);
                return false;
            }
            
            GetSignalNumberOfElements(direction, signalIdx, GAMNumberOfElements);
            if (GAMNumberOfElements != (modelPorts[portIdx]->totalNumberOfElements))
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "%s signal %s number of elements mismatch (GAM: %d, model: %u)",
                    directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfElements, modelPorts[portIdx]->totalNumberOfElements);
                return false;
            }
            
            GAMSignalType = GetSignalType(direction, signalIdx);
            StreamString inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(GAMSignalType);
            if ( modelPorts[portIdx]->MARTeTypeName != inputSignalTypeStr )
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "%s signal %s type mismatch (GAM: %s, model: %s)",
                    directionName.Buffer(), GAMSignalName.Buffer(), inputSignalTypeStr.Buffer(), (modelPorts[portIdx]->MARTeTypeName).Buffer());
                return false;
            }
            
            if(!CheckrtwCAPITypeAgainstSize(modelPorts[portIdx]->cTypeName, modelPorts[portIdx]->dataTypeSize))
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "Simulink %s port %s has data type size not maching with the one configured in the GAM",
                    directionName.Buffer(), (modelPorts[portIdx]->fullName).Buffer());
                return false;
            }
            
            // Matrix signals in column major orientation requires additional workload.
            if (GAMNumberOfDimensions > 1u)
            {
                if (modelPorts[portIdx]->orientation != rtwCAPI_MATRIX_ROW_MAJOR) {
                    REPORT_ERROR(ErrorManagement::Warning,
                        "%s signal %s orientation is column-major. Supported, but requires real-time transposition and may result in performance loss.",
                        directionName.Buffer(), GAMSignalName.Buffer());
                        modelPorts[portIdx]->requiresTransposition = true;
                }
            }
            
        }
        else
        {
            // Non-homogeneus port checks (structured signal). In this case we check only the size and the GAM datatype must be uint8
            // (i.e. we treat the port as a continuous array of bytes)
            
            GetSignalNumberOfElements(direction, signalIdx, GAMNumberOfElements);
            if (GAMNumberOfElements != (modelPorts[portIdx]->CAPISize))
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "GAM %s signal %s doesn't have the same size (%d) of the corresponding (mixed signals) Simulink port (%d)",
                    directionName.Buffer(), GAMSignalName.Buffer(), GAMNumberOfElements, modelPorts[portIdx]->CAPISize);
                return false;
            }
            
            GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
            if (GAMNumberOfDimensions != 1u)
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "%s signal %s dimension mismatch: structured signal must have NumberOfDimensions = 1",
                    directionName.Buffer(), GAMSignalName.Buffer());
                return false;
            }
            
            GAMSignalType = GetSignalType(direction, signalIdx);
            StreamString inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(GAMSignalType);
            if ( GAMSignalType != UnsignedInteger8Bit )
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "GAM %s signal %s has data type (%s), mixed signals ports must be declared as uint8",
                    directionName.Buffer(), GAMSignalName.Buffer(), inputSignalTypeStr.Buffer());
                return false;
            }
            
        }
        
        // Ok, here we can map memory inputs
        if (direction == InputSignals) {
            modelPorts[portIdx]->MARTeAddress = GetInputSignalMemory(signalIdx);
        }
        else if (direction == OutputSignals) {
            modelPorts[portIdx]->MARTeAddress = GetOutputSignalMemory(signalIdx);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Unsupported signal direction in MapPorts()");
        }
    }
    
    return ok;
}

CLASS_REGISTER(SimulinkWrapperGAM, UNIT_VERSION_HR)

} /* namespace MARTe */

