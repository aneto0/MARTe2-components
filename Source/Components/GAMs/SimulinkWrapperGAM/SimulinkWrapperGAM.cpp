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
    
    getstaticmapFunction = static_cast<void*(*)(void)>(NULL);
    getmmiFunction = static_cast<void*(*)(void*)>(NULL);

    states               = NULL;
    paramSeparator      = "-";
    signalSeparator     = ".";
    verbosityLevel       = 0u;
    modelNumOfInputs     = 0u;
    modelNumOfOutputs    = 0u;
    modelNumOfParameters = 0u;
    
    skipUnlinkedTunableParams = true;
    paramsHaveStructArrays    = false;

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

    getmmiFunction       = static_cast<void*(*)(void*)>(NULL);
    getalgoinfoFunction  = static_cast<void(*)(void*)>(NULL);
    getstaticmapFunction = static_cast<void*(*)(void)>(NULL);

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
 
// This section shows how to create a structure that can then be used as a signal type by the GAM:
/*
ConfigurationDatabase cfgdb;
cfgdb.CreateRelative("Field1");
cfgdb.MoveRelative("Field1");
cfgdb.Write("Type", "uint32");
cfgdb.Write("NumberOfElements", 2);
cfgdb.MoveToAncestor(1u);
cfgdb.CreateRelative("Field2");
cfgdb.MoveRelative("Field2");
cfgdb.Write("Type", "float64");
cfgdb.Write("NumberOfElements", 4);
cfgdb.MoveToAncestor(1u);
    
IntrospectionStructure intr;
intr.SetName("Intr");
intr.Initialise(cfgdb);

for (uint32 nodeIdx = 0u; nodeIdx < cfgdb.GetNumberOfChildren(); nodeIdx++) {
    
    printf("struct: %s\n", intr.GetName());
    
    cfgdb.MoveToChild(nodeIdx);
    printf("- member: %s\n", cfgdb.GetName());
        
    StreamString typeLeaf  = "";
    uint32       nEltsLeaf = 0u;;
    cfgdb.Read("Type", typeLeaf);
    cfgdb.Read("NumberOfElements", nEltsLeaf);
    
    printf("-- type: %s\n", typeLeaf.Buffer());
    printf("-- elts: %u\n", nEltsLeaf);
    
    printf("member alt: %s\n", cfgdb.GetName());
    
    AnyType type  = cfgdb.GetType("Type");
    AnyType nelts = cfgdb.GetType("NumberOfElements");
    
    REPORT_ERROR(ErrorManagement::Debug, "-- type : %!", type);
    REPORT_ERROR(ErrorManagement::Debug, "-- nelts: %!", nelts);
    
    cfgdb.MoveToAncestor(1u);

}
PrintIntrospection("Intr");
*/


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
        status = data.Read("TunableParamExternalSource", tunableParamExternalSource);
        if (status) {
            REPORT_ERROR(ErrorManagement::Information, "Retrieved '%s' as TunableParamExternalSource parameter.", tunableParamExternalSource.Buffer());
        }
        else
        {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting TunableParamExternalSource parameter.");
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
        if(data.Read("SkipUnlinkedTunableParams", itemp)) {
            skipUnlinkedTunableParams = (itemp != 0u);
            REPORT_ERROR(ErrorManagement::Information, "SkipUnlinkedTunableParams set to %d.", itemp);
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "SkipUnlinkedTunableParams not set, by default it is set to true.");
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

    // getmmiFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
        if (status) {
            status = StringHelper::ConcatenateN(symbol, "_GetCAPImmi", 64u);
        }
    }

    if (status) { // Find symbol
        getmmiFunction = reinterpret_cast<void*(*)(void*)>(libraryHandle->Function(symbol));
        status = (static_cast<void*(*)(void*)>(NULL) != getmmiFunction);
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

    // getalgoinfoFunction
    if (status) { // Compose symbol
        status = StringHelper::CopyN(symbol, symbolPrefix.Buffer(), 64u);
        if (status) {
            status = StringHelper::ConcatenateN(symbol, "_GetAlgoInfo", 64u);
        }
    }
    
    if (status) { // Find symbol
        getalgoinfoFunction = reinterpret_cast<void(*)(void*)>(libraryHandle->Function(symbol));
        status = (static_cast<void(*)(void*)>(NULL) != getalgoinfoFunction);
        if (!status) {
            REPORT_ERROR(ErrorManagement::Information, "Algorithm information not found in the Simulink .so");
            status = true;
        }
    }

    /// 4. Building of a reference container containing parameter values
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
    void *mmiTemp = ((*getmmiFunction)(states));
    rtwCAPI_ModelMappingInfo* mmi = reinterpret_cast<rtwCAPI_ModelMappingInfo*>(mmiTemp);
    
    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);

    modelNumOfInputs     = rtwCAPI_GetNumRootInputs(mmi);
    modelNumOfOutputs    = rtwCAPI_GetNumRootOutputs(mmi);
    modelNumOfParameters = rtwCAPI_GetNumModelParameters(mmi);
    
    RTWCAPIV2LOG(ErrorManagement::Information,
        "Simulink C API version number: %d",
        mmi->versionNum);
    
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
    // build the vectos of ports objects
    REPORT_ERROR(ErrorManagement::Information, "%s, number of root inputs: %d", libraryName.Buffer(), modelNumOfInputs);
    ScanRootIO(mmi, ROOTSIG_INPUTS);

    REPORT_ERROR(ErrorManagement::Information, "%s, number of root outputs: %d", libraryName.Buffer(), modelNumOfOutputs);
    ScanRootIO(mmi, ROOTSIG_OUTPUTS);

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
    
    // Now match the Simulink input and ouput ports to the GAM ones
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
    /// 6. Check parameter coherence between GAM and model
    ///    and Actualise()
    ///-------------------------------------------------------------------------
    
    if (status) {
        
        ReferenceT<ReferenceContainer> mdsPar = ObjectRegistryDatabase::Instance()->Find(tunableParamExternalSource.Buffer());
        
        if (!mdsPar.IsValid()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Tunable parameter loader %s is not valid", tunableParamExternalSource.Buffer());
        }
        else {
            if (status) {
                
                // mdsPar must be of type MDSObjLoader
                status = (StringHelper::Compare("MDSObjLoader", (mdsPar->GetClassProperties())->GetName()) == 0u);
                
                if (!status) {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                        "Tunable parameter loader %s must be an instance of MDSObjLoader class.",
                        tunableParamExternalSource.Buffer());
                }
            }
            
            // Loop over all connections in the MDSObjLoader container
            for (uint32 connectionIdx = 0u; (connectionIdx < mdsPar->Size()) && (status); connectionIdx++) {
                
                ReferenceT<ReferenceContainer> connection = mdsPar->Get(connectionIdx);
                status = connection.IsValid();
                
                if (status) {
                    status = (StringHelper::Compare("MDSObjConnection", (connection->GetClassProperties())->GetName()) == 0u);
                    if (!status) {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                        "Children of parameter loader %s must be instances of MDSObjConnection class.",
                        tunableParamExternalSource.Buffer());
                    }
                }
                
                // Loop over all parameters in the MDSObjConnection container
                for (uint32 paramIdx = 0u; (paramIdx < connection->Size()) && (status); paramIdx++) {
                    
                    ReferenceT<ReferenceContainer> parameter = connection->Get(paramIdx);
                    status = parameter.IsValid();
                    
                    if (status) {
                        
                        // Compose absolute path:
                        StreamString parameterAbsolutePath = "";
                        parameterAbsolutePath += mdsPar->GetName();
                        parameterAbsolutePath += ".";
                        parameterAbsolutePath += connection->GetName();
                        parameterAbsolutePath += ".";
                        parameterAbsolutePath += parameter->GetName();
                        
                        // the string is used to make a name-path cdb that can be passed to params instead of mdsParcdb
                        externalParameterDatabase.Write(parameter->GetName(), parameterAbsolutePath.Buffer());
                        
                    }
                    else {
                        
                        REPORT_ERROR(ErrorManagement::Exception, "Reference to parameter %s is invalid.", parameter->GetName());
                    }
                    
                }
                
            }
        }
    }

    // Loop over all parameters retrieved from the model and update their
    // value with what is stored in the AnyType that is passed to Actualise()
    
    AnyType sourceParameter;        // the source of data from which to actualise
    
    bool isLoaded;                  // whether the parameter was correctly loaded by the loader mechanism
    bool isActualised;              // whether the parameter has been correctly actualized
    
    StreamString parameterSourceName;
    
    for(uint32 paramIdx = 0u; (paramIdx < modelParameters.GetSize()) && status; paramIdx++) {
        
        isLoaded     = false;
        isActualised = false;
        
        // Retrieve the ReferenceT<AnyType> of the source parameter from which to actualise
        StreamString parameterPathInObjDatabase;
        const char8* currentParamName = (modelParameters[paramIdx]->fullName).Buffer();
        
        // 1. Parameters from configuration file (highest priority).
        if (cfgParameterDatabase.Read(currentParamName, parameterPathInObjDatabase)) {
            
            parameterSourceName = "configuration file";
            
            ReferenceT<AnyObject> sourceParameterPtr;
            sourceParameterPtr = ObjectRegistryDatabase::Instance()->Find(parameterPathInObjDatabase.Buffer());
            
            isLoaded = sourceParameterPtr.IsValid();
            if (!status) {
                REPORT_ERROR(ErrorManagement::CommunicationError,
                    "Parameter %s: invalid reference from configuration file.",
                    currentParamName
                );
            }
            
            if (isLoaded) {
                sourceParameter = sourceParameterPtr->GetType();
            }
            
        }
        // 2. Parameters from loader class (2nd-highest priority)
        else if (externalParameterDatabase.Read(currentParamName, parameterPathInObjDatabase)) {
            
            parameterSourceName = "loader class";
            
            ReferenceT<AnyType> sourceParameterPtr;
            sourceParameterPtr = ObjectRegistryDatabase::Instance()->Find(parameterPathInObjDatabase.Buffer());
            
            isLoaded = sourceParameterPtr.IsValid();
            if (!status) {
                REPORT_ERROR(ErrorManagement::CommunicationError,
                    "Parameter %s: invalid reference from loader class.",
                    currentParamName
                );
            }
            
            if (isLoaded) {
                // Check reference->IsStaticDeclared(): if false, the paramater was skipped by the loader class
                isLoaded = sourceParameterPtr->IsStaticDeclared();
                
                if (isLoaded) {
                    sourceParameter = *(sourceParameterPtr.operator ->());
                }
                
            }
            
        }
        // 3. Parameter not found (if skipUnlinkedTunableParams, then use compile-time value)
        else {
            isLoaded = false;
        }
            
        // Data is copied from the source parameter to the model
        if (isLoaded) {
            isActualised = modelParameters[paramIdx]->Actualise(sourceParameter);
        }
        
        if (isLoaded && isActualised) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s correctly actualized from %s.",
                currentParamName, parameterSourceName.Buffer());
        }
        else if (!isLoaded && !isActualised && skipUnlinkedTunableParams) {
            REPORT_ERROR(ErrorManagement::Information,
                "Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s unlinked, using compile time value",
                currentParamName);
        }
        else if (isLoaded && !isActualised && skipUnlinkedTunableParams) {
            REPORT_ERROR(ErrorManagement::Warning,"Parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s cannot be actualized, using compile time value",
                currentParamName);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "skipUnlinkedTunableParams is false and parameter %-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s cannot be actualized, failing",
                currentParamName);
            status = false;   // this is the only case execution should be stopped
        }

    }
    
    ///-------------------------------------------------------------------------
    /// 7. Print port/signal details
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

void SimulinkWrapperGAM::ScanTunableParameters(rtwCAPI_ModelMappingInfo* mmi)
{
    uint_T        nparams;
    const char_T* paramName;
    uint16_T      dataTypeIdx;
    uint8_T       slDataID;
    uint16_T      numElements;
    uint16_T      dataTypeSize;
    uint_T        addrIdx;
    void*         paramAddress;
    uint16_T      SUBdimIdx;
    uint8_T       SUBnumDims;
    uint_T        SUBdimArrayIdx;

    nparams = rtwCAPI_GetNumModelParameters(mmi);
    if(nparams==0)
    {
        RTWCAPIV1LOG(ErrorManagement::Information, "No tunable parameters found.");
        return;
    }
    //RTWCAPIV1LOG(ErrorManagement::Information, "%s, number of main tunable parameters: %d\n", libraryName.Buffer(), nparams);

    // Populating C API data structure pointers of the class from mmi
    modelParams = rtwCAPI_GetModelParameters(mmi);
    if (modelParams == NULL) return;
    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);
    if (dataTypeMap == NULL) return;
    elementMap = rtwCAPI_GetElementMap(mmi);
    if (elementMap == NULL) return;
    dimMap   = rtwCAPI_GetDimensionMap(mmi);
    if(dimMap ==NULL) return;
    dimArray = rtwCAPI_GetDimensionArray(mmi);
    if(dimArray == NULL) return;
    dataAddrMap = rtwCAPI_GetDataAddressMap(mmi);
    if(dataAddrMap == NULL) return;


    for(uint_T paramIdx = 0; paramIdx<nparams; paramIdx++)
    {
        dataTypeIdx  = rtwCAPI_GetModelParameterDataTypeIdx(modelParams, paramIdx); // Index into the data type in rtwCAPI_DataTypeMap
        paramName    = rtwCAPI_GetModelParameterName(modelParams, paramIdx);        // Name of the parameter
        slDataID     = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);           // Simulink type from data type map
        numElements  = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);     // Number of elements of the strucutre data type
        dataTypeSize = rtwCAPI_GetDataTypeSize(dataTypeMap,dataTypeIdx);            // Size of the datatype in bytes, WARNING: 16 bits maximum !!
        
        // clear lastAddressVector
        while (lastAddressVector.GetSize() != 0u) {
            lastAddressVector.Remove(0u);
        }
        paramlastaddress=NULL;
        
        
        if(slDataID!=SS_STRUCT) {
            // Not structured parameter, directly print it from main params structure
            ScanParameter(paramIdx, "", PAR_FROM_PARAMS, NULL, "", 0, 1);
        }
        else
        {
            // Structured parameters, descend the tree
            addrIdx         = rtwCAPI_GetModelParameterAddrIdx(modelParams,paramIdx);
            paramAddress    = (void *) rtwCAPI_GetDataAddress(dataAddrMap,addrIdx);
            SUBdimIdx       = rtwCAPI_GetModelParameterDimensionIdx(modelParams, paramIdx);
            SUBnumDims      = rtwCAPI_GetNumDims(dimMap, SUBdimIdx);
            SUBdimArrayIdx  = rtwCAPI_GetDimArrayIndex(dimMap, SUBdimIdx);

            StreamString diminfo = "[";
            bool structarray = false;
            
            for(unsigned int idx2=0; idx2<SUBnumDims; idx2++)
            {
                
                if(dimArray[SUBdimArrayIdx + idx2]>1) structarray=true;
                
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
            ScanParametersStruct(dataTypeIdx ,1, (uint8*)paramAddress, paramNameAndSeparator, absDeltaAddress, "");
            if(structarray)
            {
                RTWCAPIV2LOG(ErrorManagement::Warning, "Array of structure detected, not yet supported! Please flatten the previous params structure!");
                paramsHaveStructArrays=true;
            }
        }
    }
}

void SimulinkWrapperGAM::ScanParametersStruct(uint_T dataTypeIdx, uint_T depth, void *startaddr, StreamString basename, uint32_T baseoffset, StreamString spacer)
{
    uint_T        idx;
    const char_T  *elementName;
    uint16_T      numElements;
    uint16_T      elemMapIdx;
    uint16_T      SUBdataTypeIndex;
    uint8_T       SUBslDataID;
    uint16_T      SUBnumElements;
    uint32_T      SUBdataTypeOffset;
    void*         byteptr = startaddr;
    void*         runningbyteptr = startaddr;
    StreamString  tempstr;
    uint16_T      SUBdimIdx;
    uint8_T       SUBnumDims;
    uint_T        SUBdimArrayIdx;
    uint16_T      SUBdataTypeSize;

    elemMapIdx  = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap,dataTypeIdx);
    numElements = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);

    StreamString specificSpacer; // spacer of the current parameter
    StreamString passoverSpacer; // spacer that will be passed to a recursive call

    for(idx=0; idx<numElements; idx++)
    {
        elementName         = rtwCAPI_GetElementName(elementMap, idx+elemMapIdx);
        SUBdimIdx           = rtwCAPI_GetElementDimensionIdx(elementMap, idx+elemMapIdx);
        SUBnumDims          = rtwCAPI_GetNumDims(dimMap, SUBdimIdx);
        SUBdimArrayIdx      = rtwCAPI_GetDimArrayIndex(dimMap, SUBdimIdx);
        SUBdataTypeIndex    = rtwCAPI_GetElementDataTypeIdx(elementMap, idx+elemMapIdx);
        SUBdataTypeOffset   = rtwCAPI_GetElementOffset(elementMap, idx+elemMapIdx);
        SUBslDataID         = rtwCAPI_GetDataTypeSLId(dataTypeMap, SUBdataTypeIndex);
        SUBnumElements      = rtwCAPI_GetDataTypeNumElements(dataTypeMap,SUBdataTypeIndex);
        SUBdataTypeSize     = rtwCAPI_GetDataTypeSize(dataTypeMap,SUBdataTypeIndex);
        
        specificSpacer = spacer; // reset the spacer
        passoverSpacer = spacer;
        if (idx == numElements - 1u) {
            specificSpacer += "└ ";
        } 
        else {
            specificSpacer += "├ ";
        }
        
        StreamString diminfo = "[";
        bool structarray = false;
        for(unsigned int idx2=0; idx2<SUBnumDims; idx2++)
        {
            if(dimArray[SUBdimArrayIdx + idx2]>1) structarray=true;
            
            diminfo.Printf("%d", dimArray[SUBdimArrayIdx + idx2]);
            if (idx2 != SUBnumDims - 1u) {
                diminfo+=",";
            }
        }
        diminfo += "]";

        if(SUBslDataID!=SS_STRUCT)
        {
            ScanParameter(elemMapIdx+idx, specificSpacer, PAR_FROM_ELEMENTMAP, byteptr, basename, baseoffset, depth);
        }
        else
        {

            runningbyteptr=reinterpret_cast<void *>(static_cast<uint8*>(startaddr)+SUBdataTypeOffset);

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
                if(idx==0)
                {
                    lastAddressVector.Set(depth - 1u, runningbyteptr);
                    deltaaddr=0;
                }
                else
                {
                    deltaaddr = reinterpret_cast<uint64>(runningbyteptr) - reinterpret_cast<uint64>(lastAddressVector[depth - 1u]);
                }
            }

            // Calculating absolute delta address
            uint64 absDeltaAddress;
            absDeltaAddress=reinterpret_cast<uint64>(runningbyteptr)-reinterpret_cast<uint64>(currentParamBaseAddress);

            RTWCAPIV2LOG(ErrorManagement::Information,
                    "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, nested struct (idx %d) with %d elems, size(u16!): %d, offset: %d, base addr: %p, same lvl delta: %d, abs delta: %d, dims: %s",
                    specificSpacer.Buffer(), elementName, idx, SUBnumElements, SUBdataTypeSize, SUBdataTypeOffset, runningbyteptr, deltaaddr, absDeltaAddress, diminfo.Buffer());
            tempstr=elementName;
            if(structarray)
            {
                RTWCAPIV2LOG(ErrorManagement::Warning, "Array of structure detected, not yet supported! Please flatten the previous params structure!");
                paramsHaveStructArrays=true;
            }
            
            StreamString nameAndSeparators = basename;
            nameAndSeparators += tempstr;
            nameAndSeparators += paramSeparator;
            
            if (idx == numElements - 1u) { 
                passoverSpacer += "  "; 
            } else {
                passoverSpacer += "┆ ";
            }
            
            ScanParametersStruct(SUBdataTypeIndex, depth+1, runningbyteptr, nameAndSeparators, absDeltaAddress, passoverSpacer);
        }
    }
}

void SimulinkWrapperGAM::ScanParameter(uint_T paridx, StreamString spacer, enum rtwCAPI_printparmode mode, void* startaddr, StreamString basename, uint32_T baseoffset, uint_T depth)
{

    uint_T               idx;
    const char_T         *ELEelementName;
    uint32_T             ELEelementOffset;
    uint16_T             ELEdataTypeIndex;
    uint16_T             ELEdimIndex;
    uint8_T              ELEnumDims;
    uint_T               ELEdimArrayIdx;
    rtwCAPI_Orientation  ELEorientation;

    const char_T         *ELEctypename;
    uint_T               ELEaddrIdx;
    uint8*               ELEparamAddress;
    uint16_T             ELEdataTypeSize;
    uint32_T             ELEsize;
    uint32_T             ELEelements;
    //StreamString         elementname;
    StreamString         fullpathname;
    StreamString         ELEtypename;
    //void *               returnAddress;
    uint32               ELEMARTeNumDims = 0u;   // number of dimensions according to MARTe standard
    
    switch(mode) {
        
        case PAR_FROM_ELEMENTMAP: {
            
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
        
        case PAR_FROM_PARAMS: {
            
            // Parameter data is retrieved from main CAPI parameters data structure
            // (this case applies to not structured parameters at root level of the tree)
            ELEdataTypeIndex     = rtwCAPI_GetModelParameterDataTypeIdx(modelParams, paridx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize(dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetModelParameterName(modelParams, paridx);
            ELEelementOffset     = 0; // root level parameters have their address directly specified in the dataAddrMap structure
            ELEdimIndex          = rtwCAPI_GetModelParameterDimensionIdx(modelParams, paridx);
            
            ELEaddrIdx           = rtwCAPI_GetModelParameterAddrIdx(modelParams,paridx);
            ELEparamAddress      = (uint8 *) rtwCAPI_GetDataAddress(dataAddrMap,ELEaddrIdx);
            
            break;
        }
        
        default:
            return;
            
    }

    ELEctypename         = rtwCAPI_GetDataTypeCName(dataTypeMap, ELEdataTypeIndex);
    ELEnumDims           = rtwCAPI_GetNumDims      (dimMap,      ELEdimIndex);      // not number of dimensions, but number of slots occupied in the dimension array
    ELEdimArrayIdx       = rtwCAPI_GetDimArrayIndex(dimMap,      ELEdimIndex);
    ELEorientation       = rtwCAPI_GetOrientation  (dimMap,      ELEdimIndex);

    Vector<uint32> ELEactualDimensions(ELEnumDims);
    
    ELEsize=1;
    ELEelements=1;
    for(idx=0; idx<ELEnumDims; idx++)
    {
        ELEactualDimensions[idx] = dimArray[ELEdimArrayIdx + idx];
        ELEelements*=ELEactualDimensions[idx];
    }
    
    // Calculate number of dimensions in MARTe2 sense
    ELEMARTeNumDims = 0;
    for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
        if (ELEactualDimensions[dimIdx] > 1) {
            ELEMARTeNumDims++;
        }
    }
    
    ELEsize=ELEelements*ELEdataTypeSize;

    fullpathname=basename.Buffer();
    fullpathname+=ELEelementName;
    
    // Tree view
    StreamString paramInfoString = "";
    StreamString orientationName = "";
    paramInfoString.Printf(
        "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, offset %d, type %-6s (%d bytes), ndims %d, dims [",
        spacer.Buffer(), ELEelementName, ELEelementOffset, ELEctypename, ELEdataTypeSize, ELEMARTeNumDims);
    
    paramInfoString.Printf("%d", ELEactualDimensions[0]);
    
    for(idx=1; idx<ELEnumDims; idx++)
    {
        paramInfoString.Printf(",%d", ELEactualDimensions[idx]);
    }
    uint32 deltaAddress;
    if(paramlastaddress!=NULL)
        deltaAddress = reinterpret_cast<uint64>(ELEparamAddress)-reinterpret_cast<uint64>(paramlastaddress);
    else
        deltaAddress = 0;
    paramlastaddress=ELEparamAddress;
    
    // TODO printf version used to erase the leading zeros in front of the pointer
    paramInfoString.Printf("], addr: %p, pr par delta: %d, orient: ",ELEparamAddress, deltaAddress);
    rtwCAPI_GetOrientationName(ELEorientation, &orientationName);
    
    paramInfoString += orientationName;
    RTWCAPIV2LOG(ErrorManagement::Information, paramInfoString.Buffer());
    
    ELEtypename=ELEctypename;
    
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
    
    modelParameters.Add(currentParameter);
}

void SimulinkWrapperGAM::ScanRootIO(rtwCAPI_ModelMappingInfo* mmi, enum rtwCAPI_rootsigmode mode)
{
    //uint8_T       versionNumber;
    uint32        nsignals;
    const char_T* sigName;
    uint16_T      dataTypeIdx;
    uint8_T       slDataID;
    uint16_T      numElements;
    uint16_T      dataTypeSize;
    uint_T        addrIdx;
    void*         sigAddress;
    StreamString stemp;

    // Populating C API data structure pointers of the class from mmi
    rootInputs=rtwCAPI_GetRootInputs(mmi);
    if(rootInputs == NULL) return;
    rootOutputs=rtwCAPI_GetRootOutputs(mmi);
    if(rootInputs == NULL) return;
    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);
    if (dataTypeMap == NULL) return;
    elementMap = rtwCAPI_GetElementMap(mmi);
    if (elementMap == NULL) return;
    dimMap   = rtwCAPI_GetDimensionMap(mmi);
    if(dimMap ==NULL) return;
    dimArray = rtwCAPI_GetDimensionArray(mmi);
    if(dimArray == NULL) return;
    dataAddrMap = rtwCAPI_GetDataAddressMap(mmi);
    if(dataAddrMap == NULL) return;


    switch(mode)
    {
    case ROOTSIG_INPUTS:
        nsignals = rtwCAPI_GetNumRootInputs(mmi);
        if(nsignals==0)
        {
            RTWCAPIV1LOG(ErrorManagement::Information, "No root inputs found");
            return;
        }
        sigGroup=rootInputs;
        break;
    case ROOTSIG_OUTPUTS:
        nsignals = rtwCAPI_GetNumRootOutputs(mmi);
        if(nsignals==0)
        {
            RTWCAPIV1LOG(ErrorManagement::Information, "No root outputs found");
            return;
        }
        sigGroup=rootOutputs;
        break;
    default:
        return;
    }



    for (uint32 sigIdx = 0u; sigIdx < nsignals; sigIdx++) {
        
        dataTypeIdx  = rtwCAPI_GetSignalDataTypeIdx(sigGroup, sigIdx); // Index into the data type in rtwCAPI_DataTypeMap
        sigName      = rtwCAPI_GetSignalName(sigGroup, sigIdx);        // Name of the parameter
        slDataID     = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);           // Simulink type from data type map
        numElements  = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);     // Number of elements of the strucutre data type
        dataTypeSize = rtwCAPI_GetDataTypeSize(dataTypeMap,dataTypeIdx);            // Size of the datatype in bytes, WARNING: 16 bits maximum !!

        // clear lastAddressVector
        while (lastAddressVector.GetSize() != 0u) {
            lastAddressVector.Remove(0u);
        }
        
        switch(mode)
        {
        case ROOTSIG_INPUTS:
            currentPort = static_cast<SimulinkPort*>(new SimulinkInputPort());
            break;
        case ROOTSIG_OUTPUTS:
            currentPort = static_cast<SimulinkPort*>(new SimulinkOutputPort());
            break;
        }
        
        stemp=sigName;
        currentPort->fullName = stemp;
        currentPort->verbosity = verbosityLevel;

        if(slDataID!=SS_STRUCT) {
            // Not structured parameter, directly print it from main params structure
            // TODO: check if baseoffset 0 is correct
            
            ScanSignal(sigIdx, "", SIG_FROM_SIGS, NULL, "", 0, 1);
            //runningportptr->addRunningOffsetToOffsetBasedSize();
        }
        else
        {
            // Structured parameters, descend the tree
            addrIdx      = rtwCAPI_GetSignalAddrIdx(sigGroup,sigIdx);
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
            ScanSignalsStruct(dataTypeIdx ,1, sigAddress, nameAndSeparators, absDeltaAddress, "");
        }
        
        modelPorts.Add(currentPort);
    }

}

void SimulinkWrapperGAM::ScanSignalsStruct(uint_T dataTypeIdx, uint_T depth,  void *startaddr, StreamString basename, uint32_T baseoffset, StreamString spacer){
    
    
    uint_T        idx;
    const char_T  *elementName;
    uint16_T      numElements;
    uint16_T      elemMapIdx;
    uint16_T      SUBdataTypeIndex;
    uint8_T       SUBslDataID;
    uint16_T      SUBnumElements;
    uint32_T      SUBdataTypeOffset;
    void*         byteptr = startaddr;
    void*         runningbyteptr = startaddr;
    StreamString  tempstr;

    elemMapIdx  = rtwCAPI_GetDataTypeElemMapIndex(dataTypeMap,dataTypeIdx);
    numElements = rtwCAPI_GetDataTypeNumElements(dataTypeMap,dataTypeIdx);

    StreamString specificSpacer; // spacer of the current signal
    StreamString passoverSpacer; // spacer that will be passed to a recursive call
    
    for(idx=0; idx<numElements; idx++)
    {
        elementName         = rtwCAPI_GetElementName(elementMap, idx+elemMapIdx);
        SUBdataTypeIndex    = rtwCAPI_GetElementDataTypeIdx(elementMap, idx+elemMapIdx);
        SUBdataTypeOffset   = rtwCAPI_GetElementOffset(elementMap, idx+elemMapIdx);
        SUBslDataID         = rtwCAPI_GetDataTypeSLId(dataTypeMap, SUBdataTypeIndex);
        SUBnumElements      = rtwCAPI_GetDataTypeNumElements(dataTypeMap,SUBdataTypeIndex);
        
        
        specificSpacer = spacer; // reset the spacer
        passoverSpacer = spacer;
        if (idx == numElements - 1u) {
            specificSpacer += "└ ";
        } 
        else {
            specificSpacer += "├ ";
        }
        
        // Scan the parameter or structure
        if(SUBslDataID!=SS_STRUCT)
        {
            ScanSignal(elemMapIdx+idx, specificSpacer, SIG_FROM_ELEMENTMAP, byteptr, basename, baseoffset, depth);
        }
        else
        {

            //byteptr=reinterpret_cast<void *>(static_cast<uint8*>(byteptr)+SUBdataTypeOffset);
            //runningbyteptr=reinterpret_cast<void *>(static_cast<uint8*>(runningbyteptr)+SUBdataTypeOffset);
            runningbyteptr=reinterpret_cast<void *>(static_cast<uint8*>(startaddr)+SUBdataTypeOffset);

            // Calculating sam elevel delta address
            uint64 deltaaddr;
            if (lastAddressVector.GetSize() < depth)
            {
                lastAddressVector.Add(runningbyteptr);
                deltaaddr=0;
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
                if(idx==0)
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
                    specificSpacer.Buffer(), elementName, idx, SUBnumElements, SUBdataTypeOffset, runningbyteptr, deltaaddr, absDeltaAddress);
            tempstr=elementName;
            
            StreamString nameAndSeparators = basename;
            nameAndSeparators += tempstr;
            nameAndSeparators += signalSeparator;
            
            if (idx == numElements - 1u) { 
                passoverSpacer += "  "; 
            } else {
                passoverSpacer += "┆ ";
            }
            
            ScanSignalsStruct(SUBdataTypeIndex, depth+1, runningbyteptr, nameAndSeparators, absDeltaAddress, passoverSpacer);
            
        }
    }
}

void SimulinkWrapperGAM::ScanSignal(uint_T sigidx, StreamString spacer, enum rtwCAPI_printsigmode mode, void *startaddr, StreamString basename, uint32_T baseoffset, uint_T depth)
{
    uint_T               idx;
    const char_T         *ELEelementName;
    uint32_T             ELEelementOffset;
    uint16_T             ELEdataTypeIndex;
    uint16_T             ELEdimIndex;
    uint8_T              ELEnumDims;
    uint_T               ELEdimArrayIdx;
    rtwCAPI_Orientation  ELEorientation;
    
    const char_T         *ELEctypename;
    uint_T               ELEaddrIdx;
    uint8*               ELEparamAddress;
    uint16_T             ELEdataTypeSize;
    uint32_T             ELEsize;
    StreamString         fullpathname;
    uint32               ELEMARTeNumDims = 0u;   // number of dimensions according to MARTe standard
    
    SimulinkSignal*     currentSignal;

    switch (mode) {
        
        case SIG_FROM_ELEMENTMAP:
            // Signalss data is retrieved from the signals data structure
            // (this case applies to all other cases w.r.t. the one below)
            
            ELEdataTypeIndex     = rtwCAPI_GetElementDataTypeIdx(elementMap, sigidx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize(dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetElementName(elementMap, sigidx);
            ELEelementOffset     = rtwCAPI_GetElementOffset(elementMap, sigidx);
            ELEdimIndex          = rtwCAPI_GetElementDimensionIdx(elementMap, sigidx);
            
            ELEparamAddress      = static_cast<uint8*>(startaddr)+ELEelementOffset;
            //ELEparamAddress      = startaddr;
            
            break;
            
        case SIG_FROM_SIGS:
            // Signal data is retrieved from main CAPI signals data structure
            // (this case applies to not structured parameters at root level of the tree)
            
            ELEdataTypeIndex     = rtwCAPI_GetSignalDataTypeIdx(sigGroup, sigidx);
            ELEdataTypeSize      = rtwCAPI_GetDataTypeSize(dataTypeMap, ELEdataTypeIndex);
            ELEelementName       = rtwCAPI_GetSignalName(sigGroup, sigidx);
            ELEelementOffset     = 0u; // root level parameters have their address directly specified in the dataAddrMap structure
            ELEdimIndex          = rtwCAPI_GetSignalDimensionIdx(sigGroup, sigidx);
            
            ELEaddrIdx           = rtwCAPI_GetSignalAddrIdx(sigGroup,sigidx);
            ELEparamAddress      = (uint8 *) rtwCAPI_GetDataAddress(dataAddrMap,ELEaddrIdx);
            
            currentPort->address = ELEparamAddress;
            currentPort->baseAddress = ELEparamAddress;
            
            break;
            
        default:
            return;
    }

    ELEctypename         = rtwCAPI_GetDataTypeCName(dataTypeMap,ELEdataTypeIndex);
    ELEnumDims           = rtwCAPI_GetNumDims(dimMap, ELEdimIndex);
    ELEdimArrayIdx       = rtwCAPI_GetDimArrayIndex(dimMap, ELEdimIndex);
    ELEorientation       = rtwCAPI_GetOrientation(dimMap, ELEdimIndex);
    
    Vector<uint32> ELEactualDimensions(ELEnumDims); 

    ELEsize=1;
    for(idx=0; idx<ELEnumDims; idx++) {
        
        ELEactualDimensions[idx] = dimArray[ELEdimArrayIdx + idx];
        ELEsize*=ELEactualDimensions[idx];
    }

    // Calculate number of dimensions in MARTe2 sense
    ELEMARTeNumDims = 0;
    for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
        if (ELEactualDimensions[dimIdx] > 1) {
            ELEMARTeNumDims++;
        }
    }

    if(mode == SIG_FROM_SIGS)
    {
        currentPort->CAPISize = ELEsize*ELEdataTypeSize;
        currentPort->byteSize = ELEsize*ELEdataTypeSize; 
    }


    fullpathname=basename.Buffer();
    fullpathname+=ELEelementName;

    StreamString paramInfoString = "";
    StreamString orientationName = "";
    paramInfoString.Printf(
        "%s%-" PRINTFVARDEFLENGTH(SLVARNAMEDEFLENGTH) "s, offset %d, type %s (%d bytes), ndims %d, dims [",
        spacer.Buffer(), ELEelementName, ELEelementOffset, ELEctypename, ELEdataTypeSize, ELEMARTeNumDims);
    
    paramInfoString.Printf("%d", ELEactualDimensions[0]);
    
    for(idx=1; idx<ELEnumDims; idx++)
    {
        paramInfoString.Printf(",%d", ELEactualDimensions[idx]);
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

    StreamString sstemp;
    sstemp = ELEctypename;
    
    if (!currentPort->isTyped) {
        // this is first signal encountered in this port, set type
        currentPort->cTypeName     = sstemp;
        currentPort->MARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEctypename);
        currentPort->dataTypeSize  = ELEdataTypeSize;
        currentPort->orientation   = ELEorientation;
        
        currentPort->isTyped = true;
    }
    else {
        // not the first signal, check coherence with previous ones
        if (sstemp != currentPort->cTypeName) {
            currentPort->cTypeName     = "unsigned char";
            currentPort->MARTeTypeName = "uint8";
            
            currentPort->hasHomogeneousType = false;
        }
        
        if (ELEorientation != currentPort->orientation) {
            currentPort->hasHomogeneousOrientation = false;
        }
    }
    
    currentSignal = new SimulinkSignal();
    
    currentSignal->fullName      = fullpathname;
    
    currentSignal->numberOfDimensions = ELEMARTeNumDims;
    currentSignal->totalNumberOfElements = ELEsize;
    for (uint32 dimIdx = 0u; dimIdx < ELEnumDims; dimIdx++) {
        currentSignal->numberOfElements[dimIdx] = ELEactualDimensions[dimIdx];
    }
    
    currentSignal->cTypeName     = ELEctypename; 
    currentSignal->MARTeTypeName = GetMARTeTypeNameFromCTypeName(ELEctypename);
    currentSignal->type          = TypeDescriptor::GetTypeDescriptorFromTypeName((currentSignal->MARTeTypeName).Buffer());

    currentSignal->dataTypeSize  = ELEdataTypeSize;
    
    currentSignal->offset        = baseoffset + ELEelementOffset;
    
    currentPort->AddSignal(currentSignal);
    
}


bool SimulinkWrapperGAM::CheckrtwCAPITypeAgainstMARTe(StreamString rtwCAPItype, StreamString &MARTeType)
{
    const char8* csCAPItype         = GetMARTeTypeNameFromCTypeName(rtwCAPItype.Buffer());
    const char8* MARTeTypeStdString = MARTeType.Buffer();
    
    bool ret = (StringHelper::Compare(csCAPItype, MARTeTypeStdString) == 0u);
    
    return ret;
}

bool SimulinkWrapperGAM::CheckrtwCAPITypeAgainstSize(StreamString rtwCAPItype, uint16_T checksize)
{
    return GetTypeSizeFromCTypeName(rtwCAPItype.Buffer()) == checksize;
}

void SimulinkWrapperGAM::PrintAlgoInfo()
{
    Simulinkalgoinfo info;
    //char buffer[80];

    if( (static_cast<void(*)(void *)>(NULL) != getalgoinfoFunction) )
    {
        (*getalgoinfoFunction)((void *)&info);

        REPORT_ERROR(ErrorManagement::Information,"Algorithm expcode : %d", info.expcode);

        //strncpy(buffer, info.githash, 9);
        //buffer[8]='\0';

        //REPORT_ERROR(ErrorManagement::Information,"Algorithm git hash: %s", buffer);
        REPORT_ERROR(ErrorManagement::Information,"Algorithm git hash: %s", info.githash);

        //strncpy(buffer, info.gitlog, 81);
       // buffer[80]='\0';
        //REPORT_ERROR(ErrorManagement::Information,"Algorithm git log : %s", buffer);
        REPORT_ERROR(ErrorManagement::Information,"Algorithm git log : %s", info.gitlog);

    }

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
            // Homogeneus signal checks, in this case we check datatype and number of elements
            
            if(!modelPorts[portIdx]->isContiguous)
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "Simulink port '%s' is homogeneous but not contiguous, mapping not supported.",
                    (modelPorts[portIdx]->fullName).Buffer());
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
            
            GetSignalNumberOfDimensions(direction, signalIdx, GAMNumberOfDimensions);
            
            if (GAMNumberOfDimensions != 1u)
            {
                REPORT_ERROR(ErrorManagement::ParametersError,
                    "%s signal %s dimension mismatch (only vector signals supported)",
                    directionName.Buffer(), GAMSignalName.Buffer());
                return false;
            }
            
        }
        else
        {
            // Not homogeneus port checks, in this case we check only the size and the GAM datatype must be uint8
            // i.e. we treat the port as a continuous array of bytes
            
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
                    "%s signal %s dimension mismatch (only vector signals supported)",
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

