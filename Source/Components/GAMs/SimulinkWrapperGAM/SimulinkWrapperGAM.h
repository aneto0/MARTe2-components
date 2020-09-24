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

namespace MARTe {

/**
 * @brief GAM which loads and calls Simulink(r) models.
 * 
 * @details
 * 
 * Summary                                                  {#summary}
 * =======
 * 
 * The GAM encapsulates a shared library (*the library*) generated 
 * from a Simulink(r) model (*the model*) with the Embedded Coder(r)
 * tool, and exposes its input and output signals and tunable
 * parameters to the real-time application. 
 * 
 * The library contains a compiled version of the model that can be
 * introspected and run via C APIs. Information about model parameters,
 * input and outputs is stored as structures in the library, and is
 * retrieved using the C API interface exposed by the library.
 * The library also expose *entry-point functions*, i.e. functions
 * that are called to run the model.
 * 
 * The inputs, outputs and parameters in the GAM configuration
 * must match those in the generated library.
 * 
 * @warning This GAM is compiled and available for usage only if the
 *          environmental variable `MATLAB_DIR` has been defined before
 *          the compilation of MARTe2-components.
 *          The `MATLAB_DIR` variable must be the path to a valid MATLAB(r)
 *          installation directory.
 * 
 * @warning The GAM requires MATLAB(r) version 2018b or higher.
 * 
 * GAM configuration                              {#gam-configuration}
 * =================
 * 
 * The configuration syntax is:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +SimulinkWrapper1 = {
 *     Class           = SimulinkWrapperGAM
 *     Library         = "modelFileName.so"
 *     SymbolPrefix    = "modelName"
 * 
 *     Verbosity                  = 2
 *     SkipInvalidTunableParams   = 0
 *     TunableParamExternalSource = ExternalSource
 * 
 *     InputSignals  = {
 *         // As appropriate based on the Simulink(r) generated structure
 *         InSignal1 = {
 *             DataSource         = DDB1
 *             Type               = float32
 *             NumberOfDimensions = 0
 *             NumberOfElements   = 1
 *         }
 *     }
 * 
 *     OutputSignals = {
 *         // As appropriate based on the Simulink(r) generated structure
 *         OutSignal1 = {
 *             DataSource         = DDB1
 *             Type               = uint32
 *             NumberOfDimensions = 1
 *             NumberOfElements   = 10
 *         }
 *     }
 *     
 *     Parameters    =  {
 *         acalarParameter = (float32) 3.14
 *         vectorParameter = (uint32)  { 0, 1, 2, 3 }
 *         matrixParameter = (float64) { {0.0, 1.0}, {2.0, 3.0} }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Details of the configuration syntax:
 * - Required nodes (the following nodes must be specified in the
 *   configuration, otherwise the GAM execution will stop):
 *    - *Class*: the class name of this GAM, as required by standard MARTe
 *      configuration syntax. It must be `SimulinkWrapperGAM`
 *    - *Library*: file name or full path to the library, i.e. the shared object
 *      generated by the Embedded Coder (r) tool. The value of this node can be:
 *        - the plain file name if the library is already in the linker
 *          search path (e.g. in a directory already listed in `LD_LIBRARY_PATH`)
 *        - the full path otherwise (e.g. `Library = "/path/to/library/modelName.so"`)
 *    - *SymbolPrefix*: the model name, used during code generation as a
 *      prefix for the symbols exposed by the library. Note that the model
 *      name is often, but not always, the same as the file name.
 *    - *InputSignals* and *OutputSignals*: the signals to and from this
 *      GAM, as required by standard MARTe configuration syntax. See
 *      [Input and output signals](#input-and-output-signals) section
 *      for details.
 * - Optional nodes:
 *    - *Verbosity*: the amount of information that shall be printed
 *      during GAM initialisation. Minimum value: 0, maximum value: 2,
 *      default value: 0
 *    - *SkipInvalidTunableParams*: can be 0 or 1. When set to 0, the 
 *      GAM execution will stop if the actualisation of one parameter
 *      fails (see [Model parameters](#model-parameters) section
 *      for details). Default value: 1 (`true`) 
 *    - *TunableParamExternalSource*: name of the external source of
 *      parameters (see [Model parameters](#model-parameters) section
 *      for details). It must refer to a MARTe::ReferenceContainer
 *      containing references to MARTe::AnyObjects
 *    - *Parameters*: local source of parameters (see
 *      [Model parameters](#model-parameters) section for details).
 * 
 * 
 * Model parameters                                {#model-parameters}
 * ================
 * 
 * The model block behavior can be specified by setting the block parameters.
 * Each block parameter can be:
 *   - a numeric value (*inlined parameter*), whose is inlined in the
 *     code and cannot be modified at runtime
 *   - a variable (*tunable parameter*), whose value is modifiable
 *     at runtime using the library C APIs. 
 * 
 * The GAM offers an interface to change the value of tunable parameters
 * so that the model can be compiled once and then reused in different
 * situations by specifying different values of the parameters.
 * 
 * Parameter actualisation
 * -----------------------
 * 
 * The GAM can Actualise() (i.e. update the value of) tunable parameters
 * contained in the model.
 * New values for the parameters can be provided in two ways:
 * 1. By declaring a `Parameters` leaf in the GAM configuration.
 * 2. By linking the GAM to an external source of parameters.
 * The external source of parameters is expected to be a ReferenceContainer
 * populated by references to AnyObject whose name is the same of the
 * parameters to be updated. Such an object can be created from any other
 * object of the framework, thus guaranteeing interoperability.
 * 
 * During Setup(), the GAM will loop over all tunable parameters found
 * in the model and will then look for a parameter of the same name
 * in the `Parameters` node and in the external parameter source. In
 * case such a parameter is found, the GAM will compare the model parameter
 * and the new parameter and, if the new parameter is appropriate, update
 * the old value with the new value.
 * Actualisation fails if old parameter and new parameter differ in one
 * of the following: name, datatype, dimensions (scalar, vector, matrix
 * or 3D matrix), number of elements for each dimension, total data size.
 * Thus, when a tunable parameter of the model needs to be updated, the new
 * value must have the same name, datatype, dimensions and elements of
 * the model parameter.
 * 
 * If the `Verbosity` level is high enough, the GAM
 * prints out information about tunable parameters retrieved in the
 * library. Such a piece of information include, for each parameter:
 * name, datatype, dimensions (scalar, vector, matrix or 3D matrix) and
 * number of elements for each dimension.
 * 
 * The `Parameters` node has precedence over the parameter external
 * source, so if an appropriate new value for a parameter is present
 * in both of them, the one in the `Parameters` node will be used. 
 * 
 * ### Actualise a parameter using the GAM configuration ###
 * 
 * Suppose the model has a tunable parameter `tunVector` which is
 * a `float32` vector of 4 elements whose value is `[1.0 1.0 1.0 1.0]`,
 * and it is required to change its value to `[2.0 2.0 2.0 2.0]`.
 * The `Parameters` configuration node will then look as follows:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     tunVector = (float32) { 2.0 2.0 2.0 2.0 }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ### Actualise a parameter using an external source ###
 * 
 * Alternatively, one can specify a tunable parameter source by using
 * the `ExternalTunableParamSource` node. The GAM expects as external
 * tunable parameter source a MARTe::ReferenceContainer which contains
 * References to AnyObject. Such an external source can be generated
 * by another class of the framework and initialised in the Standard 
 * Heap. The GAM will then look in the root level of the
 * MARTe::ObjectRegistryDatabase for an Object with the name specified
 * in the `ExternalTunableParamSource` node.
 * 
 * Structured parameters
 * ---------------------
 * 
 * The model can contain structured parameters, that is, structure fields
 * can be used as block parameters. For example, a `Gain`
 * block can have its gain value set to `structParameters.gain1`, or
 * even more nested: `structParameters.gains.gain1`.
 * 
 * If this is the case, then the parameter name in the `Parameters` node
 * or in the external parameter source must be specified in a structured
 * fashion:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     structParameters-gains-gain1 = (float32) { 2.0 2.0 2.0 2.0 }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Note that the minus sign (`-`) is here used instead of the dot (`.`).
 * This is required since in the standard MARTe2 configuration language
 * the dot is a reserved character. 
 * 
 * Actualisation mechanism
 * -----------------------
 * 
 * The actualisation mechanism works as follows:
 * 1. ScanParameters() method retrieves informations about model
 *    parameters from the model shared library
 *    using the C APIs, and saves them in the modelParameters array 
 * 2. The GAM loops over all the retrieved tunable parameters in the
 *    modelParameters array and looks for a souce of data of the same
 *    name in the `Parameters` configuration node and in the declared
 *    external parameter source (if any).
 * 3. If a source of data is found, the GAM retrieves the corresponding
 *    AnyType and passes it to the SimulinkParameter::Actualise() method.
 * 4. The SimulinkParameter::Actualise() method checks that the parameter
 *    from the model shared library and the input AnyType are coherent
 *    (same type, same dimension...). If all checks are OK, then the
 *    parameter value in the model shared library is updated with
 *    the value pointed by the input AnyType.
 * 
 * Input and output signals                {#input-and-output-signals}
 * ========================
 * 
 * The model input and output ports are mapped to the GAM input and
 * output signals. This means that, if everything is set up correctly,
 * the GAM input signals are received from the model, and the model
 * output signals are then received by the GAM. Thus, the model can
 * act as a custom MARTe component.
 * 
 * The GAM supports both standard array signals (scalar, vector, matrix)
 * and structured signals, i.e. signals whose content is a structure.
 * 
 * Signal declaration
 * ------------------
 * 
 * For the GAM to work correctly, input and output signals as declared
 * in the GAM configuration must match input and output signals of the
 * model. In particular, data type, number of dimensions and number of
 * elements must match.
 * 
 * Data types use a slightly different naming convention in the model,
 * so the `Type` node of each signal can be set by using the following table:
 * 
 * | Model           | MARTe     |
 * | :-------------- | :-------- |
 * | `unsigned char` | `uint8`   |
 * | `signed char`   | `int8`    |
 * | `char`          | `int8`    |
 * | `unsigned short`| `uint16`  |
 * | `short`         | `int16`   |
 * | `unsigned int`  | `uint32`  |
 * | `int`           | `int32`   |
 * | `float`         | `float32` |
 * | `double`        | `float64` |
 * 
 * For example, if the model has only one input signal named `vec1`,
 * which is a vector of 10 `float32` elements, then the `InputSignals`
 * node of the configuration should look like this:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * InputSignals = {
 *     vec1 = {
 *         DataSource         = DDB1
 *         Type               = float32
 *         NumberOfDimensions = 1
 *         NumberOfElements   = 10
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * 
 * The same applies for output signals. For example, if the model has
 * two output signals and the first is a scalar `uint16` signal named
 * `scalar1`, and the second is a 3×2 matrix of `int32` named `mat1`,
 * then the `OutputSignals` node of the configuration should look like this:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Outputsignals = {
 *     scalar1 = {
 *         DataSource         = DDB1
 *         Type               = uint16
 *         NumberOfDimensions = 0
 *         NumberOfElements   = 1
 *     }
 *     mat1 = {
 *         DataSource         = DDB1
 *         Type               = int32
 *         NumberOfDimensions = 2
 *         NumberOfElements   = 6  // since it's a 3×2 matrix
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Structured signals (Nonvirtual buses)         {#structured-signals}
 * -------------------------------------
 * 
 * A structured signal is a signal whose content is not an array
 * of data but a structure of arrays or even a structure of structures.
 * 
 * @note *Nonvirtual bus* and *structured signal* are used interchangeably
 *       in this documentation.
 * 
 * By convention a structred signal must be declared as a `uint8` vector:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * BuSignal = {
 *     Type               = uint8
 *     NumberOfDimensions = 1
 *     NumberOfElements   = 10     // As many as the signal total bytesize
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @todo Fix nonvirtual bus padding bug: when a bus is populated
 *       with data of different types, if the last element is
 *       small in size a padding is introduced. The size
 *       inconsistency is detected by the GAM that stops execution.
 *             
 */
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

protected:
    
    // those members are protected for testing purpose
    
    /**
     * @brief General informations about the model hosted by the library.
     */
    uint16 modelNumOfInputs;
    uint16 modelNumOfOutputs;
    uint16 modelNumOfParameters;
    
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
     * @brief   Pointer to the current port being analyzed by ScanSignal().
     * @details This pointer is a shared (within the class) pointer to a SimulinkPort
     *          object used by the ScanSignal function set to build a port object
     *          out of Simulink(R) CAPI introspection code.
     */
    SimulinkPort* currentPort;
    
    /**
     * @brief Experimental function. Print model version info
     *        if previously retrieved from the model.
     */
    void PrintAlgoInfo();

private:

    /**
     * @brief GAM settings from configuration file.
     */
    StreamString libraryName;
    StreamString symbolPrefix;
    StreamString tunableParamExternalSource;
    bool         skipInvalidTunableParams;
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
    void* (*getStaticMapFunction) (void);
    void* (*getMmiFunction)       (void*);
    void  (*getAlgoInfoFunction)  (void*);
    
    /**
     * @brief Buffer hosting tunable parameters and internal states.
     */
    void* states;
    
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
    const uint32*                  dimArray;
    void**                         dataAddrMap;
    
    /**
     * @brief Enum that specifies where to retrieve data for a parameter
     */
    enum ParameterMode {
        
        ParamFromParameters,
        ParamFromElementMap
    };
    
    /**
     * @brief Enum that specifies where to retrieve data for a signal
     */
    enum SignalMode {
        
        SignalFromSignals,
        SignalFromElementMap
    };

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
     * @param[in] depth       tree depth level at call (for recursive callings)
     * @param[in] startaddr   current start allocated address at the callng (for recursive callings)
     */
    void ScanParametersStruct(uint32 dataTypeIdx, uint32 depth, void *startaddr, StreamString basename, uint32 baseoffset, StreamString spacer);


    /**
     * @brief Helper function to scan a parameter
     * @param[in] paridx    index of the element in the modelParams or elementMap to print
     * @param[in] spacer    the spacer to prerpint before the function outputs the parameter
     *                      descriptive string
     * @param[in] mode      `PAR_FROM_PARAMS` if param to print stays in the modelParams data structure
     *                      `PAR_FROM_ELEMENTMAP`, if param to print stays in the elementMap data struct
     * @param[in] startaddr start address to compute parameter access virtual address
     */
    void ScanParameter(uint32 paridx, StreamString spacer, ParameterMode mode, void *startaddr, StreamString basename, uint32 baseoffset, uint32 depth);

    /**
     * @brief Scans the root level input or output tree of the loaded Simulink .so code
     * @param[in] mmi  pointer to the CAPI mmi (Model Mapping Information) data structure
     *                 this pointer has to be retrieved from the loaded .so
     *                 calling the <modelnale>_GetCAPImmi function exposed
     *                 by the .so (special EPFL-SPC TLC patch needed here during
     *                 simulink code generation).
     * @param[in] mode sets whether to analyze the inputs ports or the outputs ports
     */
    void ScanRootIO(rtwCAPI_ModelMappingInfo* mmi, SignalDirection mode);

    /**
     * @brief Helper function to navigate the signals tree
     * @param[in] dataTypeIdx index of the datatype in the dataTypeMap struct to analyze
     * @param[in] depth       tree depth level at call (for recursive callings)
     * @param[in] startaddr   current start allocated address at the callng (for recursive callings)
     * @param[in] baseoffset  base offset of the structure in the port (for recursive callings)
     */
    void ScanSignalsStruct(uint32 dataTypeIdx, uint32 depth,  void *startaddr, StreamString basename, uint32 baseoffset, StreamString spacer);

    /**
     * @brief     Helper function to scan a signal
     * @param[in] paridx     index of the element in the sigGroup or elementMap to print
     * @param[in] spacer     the spacer to prerpint before the function outputs the parameter
     *                       descriptive string
     * @param[in] mode       `SIG_FROM_SIGS` if param to print stays in the sigGroup data structure
     *                       `SIG_FROM_ELEMENTMAP`, if param to print stays in the elementMap data struct
     * @param[in] startaddr  start address to compute parameter access virtual address
     * @param[in] baseoffset base offset of the structure in the port (for recursive callings)
     * @param[in] depth      tree depth level at call (for recursive callings)
     */
    void ScanSignal(uint32 sigidx, StreamString spacer, SignalMode mode, void *startaddr, StreamString basename, uint32 baseoffset, uint32 depth);
    
    /**
     * @brief Check coherence between model ports and GAM signals and map them.
     */
    bool MapPorts(SignalDirection direction);
    
    /**
     * @brief Prepare model for execution.
     */
    bool SetupSimulink();
    
    bool CheckrtwCAPITypeAgainstSize(StreamString rtwCAPItype, uint16 checksize);
    
    /**
     * @brief separator for building structured parameters fullpaths
     */
    StreamString paramSeparator;

    /**
     * @brief separator for building structured signals (aka buses) fullpaths
     */
    StreamString signalSeparator;
    
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
    
    /**
     * @brief Structure that holds data about the current version of the model.
     */
    struct SimulinkAlgoInfo {
        char8  gitHash[9];
        char8  gitLog[81];
        uint32 expCode;
    };
    
};



} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIMULINKWRAPPERGAM_H_ */
