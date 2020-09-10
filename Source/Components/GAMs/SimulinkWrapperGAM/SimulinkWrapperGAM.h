/**
 * @file SimulinkWrapperGAM.h
 * @brief Header file for class SimulinkWrapperGAM
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

 * @details This header file contains the declaration of the class SimulinkWrapperGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIMULINKWRAPPERGAM_H_
#define SIMULINKWRAPPERGAM_H_

#define GETINFOFCN

//#define SLVARNAMEDEFLENGTH 20
//#define UPRINTFVARDEFLENGTH(N) #N
//#define PRINTFVARDEFLENGTH(N) UPRINTFVARDEFLENGTH(N)

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h> // TODO debug only

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "LoadableLibrary.h"
#include "MessageI.h"
#include "ReferenceT.h"
#include "SimulinkClasses.h"
#include "StreamString.h"

#include "rtw_modelmap.h"           // Matlab coder C-API interface

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief GAM which loads and calls Simulink(r) generated models.
 * @details The GAM encapsulates code generated using Simulink(r) embedded coder
 * and exposes input and output signals, and tunable parameters to the real-time
 * application. The inputs, outputs and parameters must match the generated C
 * structures;
 * Tunable parameters, input and outputs are completely build from
 * Simulink CAPI interface.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +SimulinkWrapper1 = {
 *     Class = SimulinkWrapperGAM
 *     Library = "SCD_rtccode_02_02.so"
 *     SymbolPrefix = "SCD_rtccode_02_02"
 *     rtwCAPverbosity = 0
 *     InputSignals = { // As appropriate based on the Simulink(r) generated structure
 *
 *     }
 *     OutputSignals = { // As appropriate based on the Simulink(r) generated structure
 *
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Model parameters
 * ================
 * 
 * The GAM can Actualise() (i.e. update) the tunable parameters contained in the model.
 * New values for the parameters can be provided in two ways:
 * 1. By declaring a `Paramters` leaf in the GAM configuration.
 * 2. By linking the GAM to an external source of parameters.
 * The external source of parameters is expected to be a ReferenceContainer
 * populated by references to AnyObject whose name is the same of the
 * parameters to be updated. Such an object can be created from any other
 * object of the framework, thus guaranteeing interoperability.
 * 
 * @warning This GAM is compiled and available for usage only if the
 *          environmental variable `MATLAB_DIR` has been defined before
 *          the compilation of MARTe2-components.
 *          The `MATLAB_DIR` variable must be the path to a valid MATLAB(r)
 *          installation directory.
 * 
 * @warning The GAM requires MATLAB(r) version 2018b or higher.
 * 
 * @todo    1. Fix nonvirtual bus padding bug: when a bus is populated
 *             with data of different types, if the last element is
 *             small in size a padding is introduced. The size
 *             inconsistency is detected by the GAM that stops esecution.
 *             
 */

enum rtwCAPI_printparmode
{
    PAR_FROM_PARAMS,
    PAR_FROM_ELEMENTMAP
};

enum rtwCAPI_printsigmode
{
    SIG_FROM_SIGS,
    SIG_FROM_ELEMENTMAP
};


/**
 * @brief Helper function to print a parameter orientation
 * @param[in] ELEorientation orientation of the element according CAPI enumerated type
 */
void rtwCAPI_PrintOrientation(rtwCAPI_Orientation  &ELEorientation);


namespace MARTe {

struct _Simulinkalgoinfo {
  char githash[9];
  char gitlog[81];
  unsigned int expcode;
};

typedef struct _Simulinkalgoinfo Simulinkalgoinfo;

class SimulinkWrapperGAM: public GAM, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post 
     *   statesSize = 5u
     *   params = NULL
     *   states = NULL
     *   libraryHandle = NULL
     *   initFunction = NULL
     *   stepFunction = NULL
     */
    SimulinkWrapperGAM();

    /**
     * @brief Destructor.
     * @details Free resources.
     */
    virtual ~SimulinkWrapperGAM();

    /**
     * @brief Verifies and parses instance parameters.
     * @param[in] data configuration in the form:
     * <pre>
     * +DiscretePIDWithReset = {
     *     Class = SimulinkWrapperGAM
     *     Library = "SCD_rtccode_02_02.so"
     *     SymbolPrefix = "SCD_rtccode_02_02"
     *     rtwCAPverbosity = 0
     * </pre>
     * @details The GAM loads the specified 'Library' and retrieves the necessary symbols associated
     * to the initialisation and step functions using the 'SymbolPrefix'. Memory hosting encapsulated 
     * code internal states is also allocated by this GAM.
     * The GAM uses Simulink introspection code (aka Simulink CAPI which must be compiled within the .so) to retrieve
     * detailed information on tunable parameters and root input/output ports of the
     * Simulink code. verbosityLevel can be used to control the verbosity level of this analysis:
     * 0 not verbose (no printout)
     * 1 final parameters and root I/O objects list printout
     * 2 same that 1 with full trees of parameters and root I/O printout
     *
     * DOC FROM HERE
     *
     *
     *
     * The GAM prepares memory area for tunable parameters by parsing the additional configuration
     * meant for the encapsulated code:
     * <pre>
     *     InputSignals = { // As appropriate based on the Simulink(r) generated structure
     *
     *     }
     *     OutputSignals = { // As appropriate based on the Simulink(r) generated structure
     *
     *     }
     * </pre>
     * @return true if the criteria above is met.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Verifies signals.
     * @details The GAM calls the encapsulated code initialisation function.
     * @return true if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() && NULL != initFunction
     */
    virtual bool Setup();

    /**
     * @brief Execute method.
     * @details The GAM calls the encapsulated code step function.
     * @return true if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() && NULL != stepFunction
     */
    virtual bool Execute();

private:

    /**
     * @brief GAM settings from configuration file.
     */
    StreamString libraryName;
    StreamString symbolPrefix;
    StreamString tunableParamExternalSource;
    bool         skipUnlinkedTunableParams;
    uint8        verbosityLevel;
  
    /**
     * @brief The library hosting the Simulink(R) generated code.
     */
    LoadableLibrary* libraryHandle;

    /**
     * @brief Symbols retrieved from the library.
     */
    void* (*instFunction)         (void);
    void  (*initFunction)         (void*);
    void  (*stepFunction)         (void*);
    void  (*termFunction)         (void*);  // currently unused
    void* (*getstaticmapFunction) (void);
    void* (*getmmiFunction)       (void*);
    void  (*getalgoinfoFunction)  (void*);
    
    /**
     * @brief Buffer hosting tunable parameters and internal states.
     */
    void* states;
    
    /**
     * @brief General informations about the model hosted by the library.
     */
    uint16 modelNumOfInputs;
    uint16 modelNumOfOutputs;
    uint16 modelNumOfParameters;
    
    /**
     * @brief   Simulink C API data structures local pointers
     * @details These struct pointers are used by Simulink C APIs
     *          to navigate into the loaded Simulink C code introspection data.
     */
    const rtwCAPI_ModelParameters* modelParams;
    const rtwCAPI_Signals*         rootInputs;
    const rtwCAPI_Signals*         rootOutputs;
    const rtwCAPI_Signals*         sigGroup;
    const rtwCAPI_DataTypeMap*     dataTypeMap;
    const rtwCAPI_ElementMap*      elementMap;
    const rtwCAPI_DimensionMap*    dimMap;
    const uint_T*                  dimArray;
    void**                         dataAddrMap;

    /**
     * @brief Scans the tunable parameters tree of the loaded Simulink .so code
     * @param[in] mmi pointer to the CAPI mmi (Model Mapping Information) data structure
     *                this pointer has to be retrieved from the loaded .so
     *                calling the <modelname>_GetCAPImmi function exposed
     *                by the .so (special EPFL-SPC TLC patch needed here during
     *                simulink code generation.
     */
    void ScanTunableParameters(rtwCAPI_ModelMappingInfo* mmi);

    /**
     * @brief Helper function to navigate the parameters tree
     * @param[in] dataTypeIdx index of the datatype in the dataTypeMap struct to analyze
     * @param[in] depth tree depth level at call (for recursive callings)
     * @param[in] startaddr current start allocated address at the callng (for recursive callings)
     */
    void ScanParametersStruct(uint_T dataTypeIdx, uint_T depth, void *startaddr, StreamString basename, uint32_T baseoffset, StreamString spacer);


    /**
     * @brief Helper function to scan a parameter
     * @param[in] paridx index of the element in the modelParams or elementMap to print
     * @param[in] spacer the spacer to prerpint before the function outputs the parameter
     *                   descriptive string
     * @param[in] mode   PAR_FROM_PARAMS if param to print stays in the modelParams data structure
     *                   PAR_FROM_ELEMENTMAP, if param to print stays in the elementMap data struct
     * @param[in] startaddr start address to compute parameter access virtual address
     */
    void ScanParameter(uint_T paridx, StreamString spacer, enum rtwCAPI_printparmode mode, void *startaddr, StreamString basename, uint32_T baseoffset, uint_T depth);

    /**
     * @brief Scans the root level input or output tree of the loaded Simulink .so code
     * @param[in] mmi pointer to the CAPI mmi (Model Mapping Information) data structure
     *                this pointer has to be retrieved from the loaded .so
     *                calling the <modelnale>_GetCAPImmi function exposed
     *                by the .so (special EPFL-SPC TLC patch needed here during
     *                simulink code generation.
     * @param[in] mode sets whether to analyze the inputs ports or the outputs ports
     */
    void ScanRootIO(rtwCAPI_ModelMappingInfo* mmi, enum rtwCAPI_rootsigmode mode);

    /**
     * @brief Helper function to navigate the signals tree
     * @param[in] dataTypeIdx index of the datatype in the dataTypeMap struct to analyze
     * @param[in] depth tree depth level at call (for recursive callings)
     * @param[in] startaddr current start allocated address at the callng (for recursive callings)
     * @param[in] baseoffset base offset of the structure in the port (for recursive callings)
     */
    void ScanSignalsStruct(uint_T dataTypeIdx, uint_T depth,  void *startaddr, StreamString basename, uint32_T baseoffset, StreamString spacer);

    /**
     * @brief Helper function to scan a signal
     * @param[in] paridx index of the element in the sigGroup or elementMap to print
     * @param[in] spacer the spacer to prerpint before the function outputs the parameter
     *                   descriptive string
     * @param[in] mode   SIG_FROM_SIGS if param to print stays in the sigGroup data structure
     *                   SIG_FROM_ELEMENTMAP, if param to print stays in the elementMap data struct
     * @param[in] startaddr start address to compute parameter access virtual address
     * @param[in] baseoffset base offset of the structure in the port (for recursive callings)
     * @param[in] depth  tree depth level at call (for recursive callings)
     */
    void ScanSignal(uint_T sigidx, StreamString spacer, enum rtwCAPI_printsigmode mode, void *startaddr, StreamString basename, uint32_T baseoffset, uint_T depth);
    
    /**
     * @brief Check coherence between model ports and GAM signals and map them.
     */
    bool MapPorts(SignalDirection direction);
    
    /**
     * @brief Prepare model for execution.
     */
    bool SetupSimulink();
    
    bool CheckrtwCAPITypeAgainstMARTe(StreamString rtwCAPItype, StreamString &MARTeType);
    bool CheckrtwCAPITypeAgainstSize(StreamString rtwCAPItype, uint16_T checksize);

    void PrintAlgoInfo();
    
    /**
     * @brief separator for building structured parameters fullpaths
     */
    StreamString paramSeparator;

    /**
     * @brief separator for building structured signals (aka buses) fullpaths
     */
    StreamString signalSeparator;
   
    /**
     * @brief   Pointer to the current port being analyzed by ScanSignal().
     * @details This pointer is a shared (within the class) pointer to a SimulinkPort
     *          object used by the ScanSignal function set to build a port object
     *          out of Simulink(R) CAPI introspection code.
     */
    SimulinkPort* currentPort;
    
    /**
     * @brief   List of SimulinkParameter objects.
     * @details This array is used to store informations about each parameter
     *          used in the dynamically linked model.
     */
    StaticList<SimulinkParameter*> modelParameters;
    
    /**
     * @brief   List of SimulinkPort objects.
     * @details This array is used to store informations about each port
     *          used in the dynamically linked model.
     */
    StaticList<SimulinkPort*> modelPorts;
    
    /**
     * @brief Holds port and parameter addresses while scanning.
     */
    StaticList<void*> lastAddressVector;
    
    void* currentParamBaseAddress;
    void* paramlastaddress;

    bool paramsHaveStructArrays;
    
    /**
     * @brief Container of the parameters derived from the configuration file.
     */
    ReferenceT<ReferenceContainer> cfgParameterContainer;
    
    
    /**
     * @brief A database storing absolute paths of parameters found in configuration file.
     */
    ConfigurationDatabase cfgParameterDatabase;
    
    /**
     * @brief A database storing absolute paths of parameters found in an external loader class.
     */
    ConfigurationDatabase externalParameterDatabase;
    
};



} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIMULINKWRAPPERGAM_H_ */
