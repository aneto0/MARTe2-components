/**
 * @file SimulinkWrapperGAM.h
 * @brief Header file for class SimulinkWrapperGAM
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

 * @details This header file contains the declaration of the class SimulinkWrapperGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIMULINKWRAPPERGAM_H_
#define SIMULINKWRAPPERGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "LoadableLibrary.h"
#include "MessageI.h"
#include "ReferenceT.h"
#include "SimulinkInterfaces.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                            C-API interface                                */
/*---------------------------------------------------------------------------*/
#include "rtw_modelmap.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which loads and runs Simulink(r) models.
 * 
 * @details
 * 
 * 
 * 
 * Summary                                                           {#summary}
 * ============================================================================
 * 
 * The GAM encapsulates a shared library (*the library*) generated 
 * from a Simulink(r) model (*the model*) with the Embedded Coder(r)
 * tool, and exposes its input and output signals and tunable
 * parameters to the real-time application. 
 * 
 * The library contains a compiled version of the model that can be
 * introspected and run via C APIs. Information about model parameters,
 * input and outputs are stored as structures in the library, and are
 * retrieved using the C API interface exposed by the library.
 * The library also expose *entry-point functions*, i.e. functions
 * that are called to run the model.
 * 
 * The inputs, outputs and parameters in the GAM configuration
 * must match those in the generated library.
 * 
 * For information about how to build a model compatible with this GAM see
 * the [Model configuration](#model-configuration) section.
 * 
 * @warning This GAM is compiled and available for usage only if the
 *          environment variable `MATLAB_DIR` has been defined before
 *          the compilation of MARTe2-components.
 *          The `MATLAB_DIR` variable must point to a valid MATLAB(r)
 *          installation directory.
 * 
 * @warning The GAM is tested with MATLAB(r) version 2018a or higher.
 * 
 * @warning Enumerations are supported with version 2019a or higher.
 * 
 * 
 * 
 * GAM configuration                                       {#gam-configuration}
 * ============================================================================
 * 
 * The configuration syntax is (names and signal quantity are only given
 * as an example):
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 * +SimulinkWrapper1 = {
 *     
 *     // See below for details about each option.
 *     
 *     Class           = SimulinkWrapperGAM                 // Compulsory
 *     Library         = "modelFileName.so"                 // Compulsory
 *     SymbolPrefix    = "modelName"                        // Compulsory
 * 
 *     Verbosity                   = ( 0 | 1 | 2 )                  // Optional. Default: 0
 *     SkipInvalidTunableParams    = ( 0 | 1 )                      // Optional. Default: 1
 *     EnforceModelSignalCoverage  = ( 0 | 1 )                      // Optional. Default: 1
 *     TunableParamExternalSource  = "ExternalSourceName"           // Optional.
 *     NonVirtualBusMode           = ( "ByteArray" | "Structured" ) // Optional. Default: "ByteArray"
 * 
 *     InputSignals  = {                                // As appropriate based on the Simulink(r) generated structure
 *         InSignal1 = {
 *             DataSource         = DDB1
 *             Type               = float32
 *             NumberOfDimensions = 0
 *             NumberOfElements   = 1
 *         }
 *     }
 * 
 *     OutputSignals = {                                // As appropriate based on the Simulink(r) generated structure
 *         OutSignal1 = {
 *             DataSource         = DDB1
 *             Type               = uint32
 *             NumberOfDimensions = 1
 *             NumberOfElements   = 10
 *         }
 *         StructOutSignal = {                          // Example of nonvirtual bus as output
 *             Element1 = {
 *                 DataSource         = DDB1
 *                 Type               = uint32
 *                 NumberOfDimensions = 0
 *                 NumberOfElements   = 1
 *             }
 *             Element2 = {
 *                 DataSource         = LoggerDataSource
 *                 Type               = float32
 *                 NumberOfDimensions = 2
 *                 NumberOfElements   = 4
 *             }
 *         }
 *     }
 *     
 *     Parameters    =  {
 *         scalarParameter = (float32) 3.14
 *         vectorParameter = (uint32)  { 0, 1, 2, 3 }
 *         matrixParameter = (float64) { {0.0, 1.0}, {2.0, 3.0} }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Details of the configuration syntax:
 * - **Required nodes** (the following nodes must be specified in the
 *   configuration, otherwise the GAM execution will stop):
 *    - *Class*: the class name of this GAM, as required by standard MARTe
 *      configuration syntax. It must be `SimulinkWrapperGAM`
 *    - *Library*: file name or full path to the library, i.e. the shared object
 *      generated from the model. The value of this node can be:
 *        - the plain file name if the library is already in the linker
 *          search path (e.g. in a directory already listed in `LD_LIBRARY_PATH`)
 *        - the full path otherwise (e.g. `Library = "/path/to/library/modelName.so"`)
 *    - *SymbolPrefix*: the model name, used during code generation as a
 *      prefix for the symbols exposed by the library. Note that the model
 *      name is often, but not always, the same as the file name
 *      without the extension.
 *    - *InputSignals* and *OutputSignals*: the signals to and from this
 *      GAM, as required by standard MARTe configuration syntax. See
 *      [Input and output signals](#input-and-output-signals) section
 *      for details.
 * - **Optional nodes**:
 *    - *Verbosity*: the amount of information that shall be printed
 *      during GAM initialisation. `0` prints errors only, `1` prints errors,
 *      warnings and basic information, `2` prints errors, warnings and additional
 *      information. Minimum value: 0, maximum value: 2, default value: 0
 *    - *SkipInvalidTunableParams*: can be 0 or 1. When set to 0, the 
 *      GAM execution will stop if the actualisation of one parameter
 *      fails (see [Model parameters](#model-parameters) section
 *      for details). When set to 1, the compile time value of the
 *      parameter is used. Default value: 1 (`true`) 
 *    - *TunableParamExternalSource*: name of the external source of
 *      parameters (see [Model parameters](#model-parameters) section
 *      for details). It must refer to a `MARTe::ReferenceContainer`
 *      containing references to `MARTe::AnyObject`s
 *    - *NonVirtualBusMode*: treat model nonvirtual buses as structured signals
 *      or as a raw byte array of data. See [Structured signals](#structured-signals)
 *      section for details.
 *    - *EnforceModelSignalCoverage*: Ensures that every port of the Simulink model
 *      is connected to the GAM, eventually preventing the GAM startup.
 *      Setting this option to `0` means that some of the model output signals
 *      can be omitted in the configuration file and their value will not be stored.
 *      Valid only when `NonVirtualBusMode == "Structured"`.
 *      Default value: `1`.
 *    - *Parameters*: local list of parameters. See
 *      [Model parameters](#model-parameters) section for details.
 * 
 * 
 * 
 * Model parameters                                         {#model-parameters}
 * ============================================================================
 * 
 * The model behavior can be tuned by setting the values of block parameters.
 * For information about setting model parameters as tunable see the
 * [Model configuration](#model-configuration) section.
 * 
 * Each block parameter can be:
 *   - a numeric value (*inlined parameter*), whose value is inlined in the
 *     code and cannot be modified at runtime
 *   - a variable (*tunable parameter*), whose value is modifiable
 *     at runtime using the library C APIs. 
 * 
 * The GAM offers an interface to modify the value of tunable parameters
 * so that the model can be compiled once and then reused in different
 * situations by specifying different values of the parameters. This process
 * is called *actualisation*.
 * 
 * 
 * Parameter actualisation
 * ----------------------------------------------------------------------------
 * 
 * The GAM can Actualise() (i.e. update the value of) tunable parameters
 * contained in the model.
 * New values for the parameters can be provided in two ways:
 *   1. By declaring a `Parameters` node in the GAM configuration.
 *   2. By linking the GAM to an external source of parameters.
 * The external source of parameters is expected to be a ReferenceContainer
 * populated by references to AnyObject whose name is the same of the
 * parameters to be updated. Such an object can be created from any other
 * object of the framework, thus guaranteeing interoperability.
 * 
 * During Setup(), the GAM will cycle over all tunable parameters found
 * in the model and will then look for a parameter of the same name
 * in the `Parameters` node and in the external parameter source. In
 * case such a parameter is found, the GAM will compare the model parameter
 * and the actualisation parameter and, if the actualisation parameter is 
 * compatible, update the old value with the new value.
 * Actualisation fails if the model parameter and the actualisation parameter
 * differ in one of the following properties: name, datatype, dimensions
 * (scalar, vector, matrix or 3D matrix), number of elements for each dimension,
 * total data size.
 * Thus, when a tunable parameter of the model needs to be updated, the new
 * value must have the same name, datatype, dimensions and elements of
 * the model parameter.
 * 
 * When actualisation fails, the parameter compile time value (that is,
 * the value stored in the library) can be used if `SkipInvalidTunableParams`
 * is set to 1. If `SkipInvalidTunableParams` is set to 0 the Setup() method
 * will fail.
 * 
 * If the `Verbosity` level is high enough, the GAM prints out information
 * about tunable parameters retrieved in the library. Such a piece of
 * information include, for each parameter: name, datatype, dimensions (scalar,
 * vector, matrix or 3D matrix) and number of elements for each dimension.
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
 * the `ExternalTunableParamSource` node. The `ExternalTunableParamSource`
 * parameter will be the absolute path of the external source in the
 * configuration file. The GAM expects as external
 * tunable parameter source a MARTe::ReferenceContainer which contains
 * References to AnyObject. Such an external source can be generated
 * by another class of the framework and initialised in the Standard 
 * Heap. The GAM will then look in the root level of the
 * MARTe::ObjectRegistryDatabase for an Object with the name specified
 * in the `ExternalTunableParamSource` node, check if it is a ReferenceContainer,
 * and if it is, look for References with the same names as the tunable
 * parameters in the model.
 * 
 *
 * Structured parameters
 * ----------------------------------------------------------------------------
 * 
 * The model can contain structured parameters, that is, structure fields
 * can be used as block parameters. For example, a `Gain`
 * block can have its gain value set to `structParameters.gain1`, or
 * even more nested: `structParameters.gains.gain1`.
 * 
 * ### In the `Parameters` node of the configuration file ###
 *
 * If this is the case, then the parameter name in the `Parameters` node
 * or in the external parameter source must be specified in a structured
 * fashion:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     structParameters = {
 *         gains = {
 *             gain1 = (float32) { 2.0 2.0 2.0 2.0 }
 *             gain2 = (uint16)  2
 *         }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * In newer versions of MARTe2 it is possibile to declare structured
 * parameters in a flattened fashion, using a dot (`.`) as separator.
 * This is useful for example if only some of the structured
 * tunable parameters must be actualised, and declaring the whole
 * structure can be avoided:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     structParameters.gains.gain1 = (float32) { 2.0 2.0 2.0 2.0 }
 *     structParameters.gains.gain2 = (uint16)  2
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * To maintain retrocompatibility with previous versions of this GAM,
 * it is still possible to use a dash (`-`) instead of the dot as separator.
 * The dash (`-`) was used instead of the dot (`.`) since in the
 * standard MARTe2 configuration language the dot is a reserved character
 * and names containing a dot used to be not permitted:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     structParameters-gains-gain1 = (float32) { 2.0 2.0 2.0 2.0 }
 *     structParameters-gains-gain2 = (uint16)  2
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ### In the external parameters source ###
 *
 * When using an external parameter source, the parameters must be
 * `AnyObject`s contained in a `ReferenceContainer`.
 * The name of the `AnyObject`s shall be the flattened version
 * of the parameters name:
 *
 * @warning The following is NOT a valid MARTe2 configuration syntax.
 *          It is only given as an example of the structure that
 *          `ExtParamSource` shall have and that must be created
 *          programmatically.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +ExtParamSource = {
 *     Class = InheritingFromReferenceContainer
 *     // WARNING: NOT a valid MARTe2 configuration syntax!
 *     +structParameters.gains.gain1 = { Class = AnyObject Value = { 2.0 2.0 2.0 2.0 } }
 *     +structParameters.gains.gain2 = { Class = AnyObject Value = 2                   }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *
 * Structure arrays                                         {#structure-arrays}
 * ----------------------------------------------------------------------------
 *
 * The same structure can be repeated with the same fields but different
 * values associated to each field. The resulting data structure is called
 * an *array of structure*, *structure array* or simply *struct array*.
 * A model may use struct arrays as parameters or as input/output signals.
 * In the latter case, the struct arrays are called *arrays of buses*.
 *
 * This section is about the use of structured arrays as parameters. For usage
 * of arrays of buses as signals see [Arrays of buses](#arrays-of-buses).
 *
 * ### In the `Parameters` node of the configuration file ###
 *
 * Struct arrays can be declared in a structured fashion:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     structParameters[0][0] = {
 *         gains = {
 *             gain1 = (float32) { 1.0 1.0 1.0 1.0 }
 *             gain2 = (uint16)  1
 *         }
 *     }
 *     structParameters[0][1] = {
 *         gains = {
 *             gain1 = (float32) { 2.0 2.0 2.0 2.0 }
 *             gain2 = (uint16)  2
 *         }
 *     }
 *    // ...
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Struct arrays can also be declared in a flattened fashion:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     structParameters[0][0].gains.gain1 = (float32) { 1.0 1.0 1.0 1.0 }
 *     structParameters[0][0].gains.gain2 = (uint16)  1
 *     structParameters[0][1].gains.gain1 = (float32) { 2.0 2.0 2.0 2.0 }
 *     structParameters[0][1].gains.gain2 = (uint16)  2
 *     // ...
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ### In the external parameters source ###
 *
 * The external parameter source shall contain AnyObjects with names
 * declared in the flattened fashion of the section above.
 *
 *
 * Enumeration parameters
 * ----------------------------------------------------------------------------
 * 
 * The model can contain enumeration parameters, that is, enum fields can be
 * used as block parameters. For example, a `Constant` block can have its
 * value set to `enumParam` where `enumParam` can be `MyEnum.On` or `MyEnum.Off`.
 * `MyEnum` shall be an enumeration defined in the model.
 * 
 * In this case the actualisation value of the parameter specified
 * in the `Parameters` node or in the external parameter source must be an integer
 * of the same type as the underlying type of the model enumeration:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Parameters = {
 *     enumParam = (int16) 1
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @warning While the model enumeration can assume a limited set of values (e.g.
 *          `1` or `0` for `MyEnum.On` and `MyEnum.Off`) no check is performed
 *          on the actualisation value specified in the configuration file
 *          or from an external parameter source to verify that the value is within
 *          the model enumeration range.
 * 
 *
 * Actualisation mechanism
 * ----------------------------------------------------------------------------
 * 
 * The actualisation mechanism works as follows:
 * 1. ScanInterfaces() method retrieves informations about model
 *    parameters from the model shared library using the C APIs,
 *    and saves them in the modelParameters array 
 * 2. The GAM cycles over all the retrieved tunable parameters in the
 *    modelParameters array and looks for a souce of data of the same
 *    name in the `Parameters` configuration node and in the declared
 *    external parameter source (if any).
 * 3. If a source of data is found, the GAM retrieves the corresponding
 *    AnyType and passes it to the SimulinkInterface::Actualise() method.
 * 4. The SimulinkInterface::Actualise() method checks that the parameter
 *    from the model shared library and the input AnyType are coherent
 *    (same type, same dimensions). If all checks are OK, then the
 *    parameter value in the model shared library is updated with
 *    the value pointed by the input AnyType.
 * 
 * 
 * 
 * Input and output signals                          {#input-and-output-signals}
 * =============================================================================
 * 
 * The model input and output ports are mapped to the GAM input and
 * output signals. This means that, if everything is set up correctly,
 * the GAM input signals are received by the model, and the model
 * output signals are then received by the GAM. Thus, the model can
 * act as a custom MARTe component.
 * For details about how to set up the model to correctly export signals to
 * the GAM see the [Model configuration](#model-configuration) section. 
 * 
 * The GAM supports both array signals (scalar, vector, matrix)
 * and structured signals, i.e. signals whose content is a structure.
 * 
 * 
 * Signal declaration
 * ----------------------------------------------------------------------------
 * 
 * For the GAM to work correctly, input and output signals as declared
 * in the GAM configuration must match input and output signals of the
 * model. In particular, data type, number of dimensions and number of
 * elements must match.
 * 
 * Data types use a slightly different naming convention in the model,
 * so the `Type` node of each signal can be set by using the following table:
 * 
 * | C type name     | Model    | MARTe     |
 * | :-------------- | :------- | :-------- |
 * | `unsigned char` | `uint8`  | `uint8`   |
 * | `signed char`   | `int8`   | `int8`    |
 * | `char`          | `int8`   | `int8`    |
 * | `unsigned short`| `uint16` | `uint16`  |
 * | `short`         | `int16`  | `int16`   |
 * | `unsigned int`  | `uint32` | `uint32`  |
 * | `int`           | `int32`  | `int32`   |
 * | `float`         | `single` | `float32` |
 * | `double`        | `double` | `float64` |
 * 
 * @warning `int64` and `uint64` types are not completely supported by the code
 *          generation mechanism and should be avoided while creating the model.
 * 
 * For example, if the model has only one input signal named `vec1`,
 * which is a vector of 10 `single` elements, then the `InputSignals`
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
 * 
 * Nonvirtual buses (Structured signals)                  {#structured-signals}
 * ----------------------------------------------------------------------------
 * 
 * A structured signal is a signal whose content is not an array
 * of data but a structure of arrays or even a structure of structures.
 * 
 * The GAM allows to manage nonvirtual bus inputs and outputs in two ways:
 *   1. *mapping to structured signals*: nonvirtual buses are mapped in a corresponding
 *      MARTe2 structured signals
 *   2. *mapping to byte arrays*: nonvirtual buses are raw-copied onto `uint8` arrays
 *      of bytes
 * 
 * To select the requested mapping set the `NonVirtualBusMode` option.
 * 
 * 
 * @warning The GAM cannot map nonvirtual buses whose last element is smaller
 *          that the element before it. Always ensure that buses are populated
 *          with largest signals at the end.
 * 
 * ### Mapping nonvirtual buses into MARTe2 structured signals ###
 * 
 * Set `NonVirtualBusMode = "Structured"` to use this mode.
 * 
 * When this mode is selected, the GAM maps each element of the model
 * nonvirtual buses into an element of a MARTe2 structured signal. The configuration
 * syntax for a mapped structured signal can be explicit or make use of a previously
 * declared structure.
 * 
 * #### Explicit syntax #####
 * 
 * The explicit syntax is as follows:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {                               // Works also for InputSignals
 *     BusSignal = {
 *         BusElement1 = {                         // Vector (10 elements)
 *             Type               = uint32
 *             NumberOfDimensions = 1
 *             NumberOfElements   = 10
 *             DataSource = DDB1
 *         }
 *         BusElementStruct = {                    // SubStruct
 *             SubElement1 = {                     // Scalar
 *                 Type               = uint16
 *                 NumberOfDimensions = 0
 *                 NumberOfElements   = 1
 *                 DataSource = DDB2
 *             }
 *             SubElement2 = {                     // Matrix (2x2)
 *                 Type               = float64
 *                 NumberOfDimensions = 2
 *                 NumberOfElements   = 4
 *                 DataSource = LoggerDataSource
 *             }
 *         }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Explicit syntax allows to send each element of the nonvirtual bus to a different
 * DataSource and also to select only some of the elements of the nonvirtual bus.
 * 
 * #### Previously declared structure ####
 * 
 * A nonvirtual bus can be mapped to a structure previously declared using the
 * framework IntrospectionStructure component:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * // Outside the RealTimeApplication
 * +Types = {
 *     Class = ReferenceContainer
 * 
 *     +BusElementStruct = {                       // SubStruct
 *         Class = IntrospectionStructure
 *         SubElement1 = {                         // Scalar
 *             Type               = uint16
 *             NumberOfElements   = 1
 *         }
 *         SubElement2 = {                         // Matrix (2x2)
 *             Type               = float64
 *             NumberOfElements   = {2, 2}
 *         }
 *     }
 * 
 *     +BusSignalStruct = {
 *         Class = IntrospectionStructure
 *         BusElement1 = {                         // Vector (10 elements)
 *             Type               = uint32
 *             NumberOfElements   = 10
 *             DataSource = DDB1
 *         }
 *         BusElement2 = {
 *             Type = BusElementStruct
 *         }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * The GAM configuration syntax is then as follows:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {                          // Works also for InputSignals
 *     BusSignal = {
 *         Type       = BusSignalStruct
 *         DataSource = LoggerDataSource
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * This syntax allows easier management of the bus as a whole.
 * 
 * @note The way signal dimensions are expressed is different when using
 *       IntrospectionStructure: there is no `NumberOfDimensions` leaf and
 *       in case of non-scalar signals `NumberOfElements` is an array
 *       that on each n-th element has the dimension of the signal along
 *       the n-th axis.
 * 
 * ### Mapping nonvirtual buses into `uint8` byte arrays ###
 * 
 * Set `NonVirtualBusMode = "ByteArray"` to use this mode.
 * 
 * The structured signal must then be declared as a `uint8` array of bytes. The
 * same bus as above would then become:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {                   // Works also for InputSignals
 *     BusSignal = {
 *         Type               = uint8
 *         NumberOfDimensions = 1
 *         NumberOfElements   = 74     // As many as the signal total byte size
 *         DataSource = DDB1
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Arrays of buses                                           {#arrays-of-buses}
 * ----------------------------------------------------------------------------
 *
 * The same structure can be repeated with the same fields but different
 * values associated to each field. The resulting data structure is called
 * an *array of structure*, *structure array* or simply *struct array*.
 * A model may use struct arrays as parameters or as input/output signals.
 * In the latter case, the struct arrays are called *arrays of buses*.
 *
 * This section is about the use of structured arrays as signals. For usage
 * of structure arrays as parameters see [Structured arrays](#structured-arrays).
 *
 * Arrays of buses can be mapped:
 * 1. by declaring the flattened array of buses directly into the GAM input
 *    or output signal section (explicit syntax)
 * 2. by declaring the structure of the array of buses as an IntrospectionStructure
 *    and then using this as the signal type (previously declared structure)
 *
 * The following examples show how to declare an array of buses of size [2x3],
 * each entry of which is a bus of 2 elements, the first being a `float32` scalar
 * and the second being a `uint32` vector of 8 elements:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ArrayOfBuses [2x3]
 *  |
 *  +-- .BusElement1 (float32)[1]
 *  |
 *  +-- .BusElement2 (uint32)[8]
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @warning The array of buses is expressed differently whether it is
 *          a root-level input/output or it is an element within another bus.
 *          This is due to the fact that the shape of root-level GAM inputs/outputs
 *          cannot be specified, and only the number of elements of such signals
 *          can be specified. As a result, a root-level array of buses
 *          shall always be expressed as a one-dimensional array.
 *
 * ### Explicit syntax ###
 *
 * #### As root-level signal ####
 *
 * Use the following syntax to explicitly declare an array of buses
 * as a *root-level input/output*:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     ArrayOfBuses[0].BusElement1  = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     ArrayOfBuses[1].BusElement2  = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     ArrayOfBuses[2].BusElement1  = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     ArrayOfBuses[3].BusElement2  = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     ArrayOfBuses[4].BusElement1  = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     ArrayOfBuses[5].BusElement2  = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     ArrayOfBuses[6].BusElement1  = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     ArrayOfBuses[7].BusElement2  = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     ArrayOfBuses[8].BusElement1  = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     ArrayOfBuses[9].BusElement2  = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     ArrayOfBuses[10].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     ArrayOfBuses[11].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * #### Within a bus ####
 *
 * Use the following syntax to explicitly declare an array of buses *within*
 * a bus with another `uint8` scalar signal:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     BusSignal.Scalar                         = { Type = uint8    NumberOfElements = 2  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0][0].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0][0].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0][1].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0][1].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0][2].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0][2].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1][0].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1][0].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1][1].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1][1].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1][2].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1][2].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Note that, if `ArrayOfBuses` is a vector array and not a matrix array,
 * the syntax is the same as the root-level array of buses:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     BusSignal.Scalar                      = { Type = uint8    NumberOfElements = 2  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[0].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1].BusElement1 = { Type = float32  NumberOfElements = 1  DataSource = DDB1 }
 *     BusSignal.ArrayOfBuses[1].BusElement2 = { Type = uint32   NumberOfElements = 8  DataSource = DDB1 }
 *     // ...
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ### Previously declared structure ###
 *
 * #### As root-level signal ####
 *
 * An array of buses can be mapped to a structure previously declared using the
 * framework IntrospectionStructure component. Use the following syntax to
 * explicitly declare an array of buses as a *root-level input/output*:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * // Outside the RealTimeApplication
 * +Types = {
 *     Class = ReferenceContainer
 *
 *     +ArrayOfBuses_Type = {
 *         Class = IntrospectionStructure
 *         BusElement1 = {
 *             Type               = float64
 *             NumberOfElements   = 1
 *         }
 *         BusElement2 = {
 *             Type               = uint32
 *             NumberOfElements   = 8
 *         }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The GAM configuration syntax is then as follows (note that the bus cannot
 * be declared as 3x2, but must be declared as `NumberOfElements = 6`):
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     ArrayOfBuses = {
 *         Type = ArrayOfBuses_Type
 *         NumberOfElements = 6      // 3x2
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * #### Within a bus ####
 *
 * Use the following syntax to explicitly declare an array of buses *within*
 * a bus with another `uint8` scalar signal:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * // Outside the RealTimeApplication
 * +Types = {
 *     Class = ReferenceContainer
 *
 *     +ArrayOfBuses_Type = {
 *         Class = IntrospectionStructure
 *         BusElement1 = {
 *             Type               = float64
 *             NumberOfElements   = 1
 *         }
 *         BusElement2 = {
 *             Type               = uint32
 *             NumberOfElements   = 8
 *         }
 *     }
 *
 *     +BusSignal_Type =  {
 *         Class = IntrospectionStructure
 *         Scalar = {
 *             Type = uint8
 *             NumberOfElements = 1
 *         }
 *         ArrayOfBuses = {
 *             Type = ArrayOfBuses_Type
 *             NumberOfElements = {3, 2}
 *         }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The GAM configuration syntax is then as follows (note that in this case
 * the `ArrayOfBuses` has been declared as [3x2] in the IntrospectionStructure):
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     BusSignal = {
 *         Type = BusSignal_Type
 *         NumberOfElements = 1
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *
 * Enumeration signals
 * ----------------------------------------------------------------------------
 * 
 * The model can contain enumeration signals, that is, enum fields can be
 * used as input or output values. For example, an `Outport` block can have its
 * value set to `MyEnum.On` or `MyEnum.Off` where `MyEnum` is an enumeration
 * defined in the model.
 * 
 * In this case the signal configuration in the configuration file must declare
 * the signal to be of integer type (and of the same type as the underlying type
 * of the model enumeration):
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     enumSignal = {
 *         Type               = int16
 *         NumberOfDimensions = 0
 *         NumberOfElements   = 1
 *         DataSource = DDB1
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @warning While the model enumeration can assume a limited set of values (e.g.
 *          `1` or `0` for `MyEnum.On` and `MyEnum.Off`) no check is performed
 *          on the signal value to verify that the value is within
 *          the model enumeration range.
 * 
 * 
 * Signal logging                                             {#signal-logging}
 * ----------------------------------------------------------------------------
 *
 * Internal signals of the model (that is, signals that are not connected to
 * root-level input/output ports) can be made available to the GAM. These
 * signals are called *logging signals*.
 * For details on how to configure the model to log a signal see
 * [Configuration of logging signals](#logging-signals-conf).
 *
 * Logging signals are available as GAM outputs and all the rules described
 * above for inputs and outputs apply:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * OutputSignals = {
 *     output_port = {
 *         Type               = int16
 *         NumberOfDimensions = 0
 *         NumberOfElements   = 1
 *         DataSource = DDB1
 *     }
 *     // this signal does not map an output port but a logging signal
 *     logging_signal = {
 *         Type               = float32
 *         NumberOfDimensions = 1
 *         NumberOfElements   = 6
 *         DataSource         = DDB1
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The name of the GAM output signal must match that of the logging signal
 * for the signal to be mapped.
 *
 * @warning The connection of logging signals to a GAM output *is not*
 *          enforced by setting the `EnforceModelSignalCoverage` to true.
 *          This means that if the model containg a logging signal named
 *          `logging_signal` but no GAM signal named `logging_signal`
 *          is declared, the logging signal *will not* be logged and no
 *          error or warning will be issued.
 *
 * Model configuration                                   {#model-configuration}
 * =============================================================================
 * 
 * This section shows how to set up the model so that is is compatible with the GAM.
 * 
 * Configuration of input and output signals
 * ----------------------------------------------------------------------------
 * 
 * In order to be compatible with the SimulinkWrapperGAM, the model must receive
 * inputs from `Inport` blocks and send outputs to `Outport` blocks. The GAM
 * will map MARTe2 I/O to Simulink I/O ports.
 * 
 * In order for a signal to be correctly linked to the GAM, *each input and
 * output signal* must be named and *each input and output port* must display
 * the signal name in its icon.
 * 1. To name a signal, double click on the line representing the signal and
 *   write the name on the textbox that appears
 * 2. To display the signal name on the port, double click on the port and
 *   in the `Block parameters` set the `Icon display` field to `Signal name`
 * 
 * ### Structured signal specific settings ###
 * 
 * In order to have a structured signal as model input/output the next steps
 * must be followed:
 * 
 *   1. For *model outputs*:
 *        - create a `Bus Creator` block and attach all required signals to it
 *        - on the Block Parameters dialog of the bus creator set
 *           Output data type to `Bus: <object name>`
 *        - if a bus object has not yet been created for that signal, click on
 *           `Edit` and create it
 *        - on the Block Parameters dialog check `Output as nonvirtual` bus
 *        - attach the Bus Creator output to an output port
 *   2. For *model inputs*:
 *        - create an `Input port` block
 *        - on the Block Parameters dialog of the port set `Output data type`
 *           to `Bus: <object name>`
 *        - if a bus object has not yet been created for that signal, click on
 *           `Edit` and create it
 *        - on the Block Parameters dialog check `Output as nonvirtual bus`
 *        - attach the port output to a `Bus Selector` block to use its elements
 *           in the model.
 *
 * Configuration of parameters
 * ----------------------------------------------------------------------------
 * 
 * In order for parameter values to be modifiable by the GAM, they shall be
 * declared as variable and set as tunable in the model. 
 * 1. To declare a parameter value as variable use a variable name instead
 *    of a numeric value in the block parameter
 * 2. To set parameter values as tunable go to `Model Setting > Code Generation >
 *    Optimization` and set `Default parameter behavior` to `Tunable`.
 *    To set the same option for each single parameter, click on `Configure...`
 *    in the same window.
 * 
 * The script that is shown in the [Code generation options](#code-gen-opt)
 * section automatically sets all parameters to tunable.
 * 
 *
 * Configuration of logging signals                     {#logging-signals-conf}
 * ----------------------------------------------------------------------------
 *
 * Internal signals of the model (that is, signals that are not connected to
 * root-level input/output ports) can be made available to the GAM. These
 * signals are called *logging signals*.
 *
 * To prepare a signal to be logged the next steps must be followed:
 *
 *   1. Make sure that the signal to be logged is named
 *   2. Open the `Embedded Coder` app
 *   3. Select the signal to be logged in the model
 *   4. Move the pointer to the 3 dots that appear (`...`) and click
 *      on the first button to the left (`Add selected signals to code mappings`)
 *   5. Open the `C Code` tab
 *   6. Go to `Code Interface > Default Code Mappings`
 *   7. In the `Signals/States` tab, make sure that the storage class of
 *      the signal to be logged is set to `MultiInstance`
 *
 * For details on how to map these signals to the GAM, see the
 * [Signal logging](#signal-logging) section.
 *
 * Code generation options                                      {#code-gen-opt}
 * ----------------------------------------------------------------------------
 * 
 * The code generated from the model must meet some requirements.
 * The following script can be used to setup a model for code generation with
 * the correct settings. Note that the `model_name` variable must be set in the
 * workspace before running the script and must match the name of an open model.
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * % Solver
 * set_param(model_name, 'SolverType', 'Fixed-step');
 * 
 * % Code Generation
 * set_param(model_name, 'SystemTargetFile', 'ert_shrlib.tlc');
 * set_param(model_name, 'RTWVerbose',       0               );
 * 
 * % Optimization
 * set_param(model_name, 'DefaultParameterBehavior', 'Tunable');
 * set_param(model_name, 'OptimizationCustomize',    1        );
 * set_param(model_name, 'GlobalVariableUsage',      'None'   );
 * 
 * % Report
 * set_param(model_name, 'GenerateReport', 0);
 * 
 * % Comments
 * set_param(model_name, 'GenerateComments', 1);
 * 
 * % Custom code (MODEL is a coder keyword for the model name)
 * set_param(model_name, 'CustomSourceCode', ...
 *     [  ...
 *     '#define UTSTRUCTNAME(NAME) RT_MODEL_##NAME##_T'                         newline ...
 *     '#define TSTRUCTNAME(NAME) UTSTRUCTNAME(NAME)'                           newline ...
 *     '#define UGETMMIFCN(NAME) NAME##_GetCAPImmi'                             newline ...
 *     '#define GETMMIFCN(NAME) UGETMMIFCN(NAME)'                               newline ...
 *                                                                              newline ...
 *     'rtwCAPI_ModelMappingInfo* GETMMIFCN(MODEL) ( TSTRUCTNAME(MODEL) *rtm )' newline ...
 *     '{'                                                                      newline ...
 *     '    return &(rtmGetDataMapInfo(rtm).mmi);'                              newline ...
 *     '}'                                                                      newline ...
 *     ] ...
 * );
 * 
 * % Interface
 * set_param(model_name, 'SupportComplex',      0);
 * set_param(model_name, 'SupportAbsoluteTime', 0);
 * set_param(model_name, 'SuppressErrorStatus', 1);
 * 
 * set_param(model_name, 'CodeInterfacePackaging', 'Reusable function');
 * 
 * set_param(model_name, 'RootIOFormat', 'Part of model data structure');
 * 
 * set_param(model_name, 'RTWCAPIParams', 1);
 * set_param(model_name, 'RTWCAPIRootIO', 1);
 * 
 * set_param(model_name, 'GenerateAllocFcn', 1);
 * 
 * set_param(model_name, 'IncludeMdlTerminateFcn',    0);
 * set_param(model_name, 'CombineSignalStateStructs', 1);
 * 
 * set_param(model_name, 'ArrayLayout', 'Column-major');
 * 
 * % Templates
 * set_param(model_name, 'GenerateSampleERTMain', 0);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Once set for a model, the configuration settings can then be exported
 * from the Model Explorer and imported to other models.
 * 
 * @todo Fix int64/uint64 issue: models with [u]int64 signals when compiled
 *       export such signals as [u]fix64, which is currently incompatible
 *       with the GAM.
 * 
 * @todo Develop a more efficient transposition algorithm for column-major
 *       matrix signals. 
 * 
 */
class SimulinkWrapperGAM: public GAM, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
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
     * @brief Destructor. NOOP.
     */
    virtual ~SimulinkWrapperGAM();

    /**
     * @brief     Initializes and retrieves informations from
     *            configuration file.
     * @param[in] data the GAM configuration specified in the
     *                 configuration file.
     * @return    `true` on succeed.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief   Checks signals and parameters and initialises the model. 
     * @details This method:
     *          1. checks if signal and parameter configuration in the
     *             configuration file is compatible with the model
     *          2. actualises tunable parameters
     *          3. calls the model initialisation function
     * 
     * @return  `true` on succeed.
     * @pre     
     *          1. Initialise() == `true`
     *          2. initFunction != `NULL`
     */
    virtual bool Setup();

    /**
     * @brief  Copies memory to and from the model
     *         and calls the model code step function.
     * @return `true` on succeed.
     * @pre    
     *         1. Initialise() == `true`
     *         2. Setup() == `true`
     *         3. stepFunction != `NULL`
     */
    virtual bool Execute();

protected:
    
    // those members are protected for testing purpose
    
    // TODO change these to StaticLists or Vectors?
    SimulinkRootInterface* inputs;      //!< Array of data about the model inputs retrieved from the model using the C APIs
    SimulinkRootInterface* outputs;     //!< Array of data about the model outoputs retrieved from the model using the C APIs
    SimulinkRootInterface* params;      //!< Array of data about the model tunable parameters retrieved from the model using the C APIs
    SimulinkRootInterface* signals;     //!< Array of data about the model internal logging signals retrieved from the model using the C APIs

    /**
     * @name Number of interfaces
     */
    //@{
    uint32 modelNumOfInputs;            //!< Number of inputs of the model.
    uint32 modelNumOfOutputs;           //!< Number of outputs of the model.
    uint32 modelNumOfParameters;        //!< Number of tunable parameters of the model.
    uint32 modelNumOfSignals;           //!< Number of internal logging signals of the model.
    //@}

    ErrorManagement::ErrorType status;  //!< The error state of the GAM
    
    /**
     * @brief Experimental function. Print model version info
     *        if previously retrieved from the model.
     */
    void PrintAlgoInfo() const;

private:

    /**
     * @name    GAM settings
     * @brief   Setting retrieved from the configuration files.
     * @details See [GAM configuration](#gam-configuration) section for details.
     */
    //@{
    StreamString libraryName;                       //!< Name of the shared library file or full path to it.
    StreamString symbolPrefix;                      //!< Name of the model (may or may not differ from the name of the library).
    StreamString tunableParamExternalSource;        //!< Name of the ReferenceContainer containing References to AnyObjects that hold value for parameter actualisation.
    uint8        verbosityLevel;                    //!< How verbose should the output be. Min: 0, max: 2, default: 0.
    bool         skipInvalidTunableParams;          //!< If `true`, when a parameter actualisation fails the compile time value is used.
    bool         enforceModelSignalCoverage;        //!< When enabled, all the Simulink model I/O must be matched to a GAM I/O
    SimulinkNonVirtualBusMode nonVirtualBusMode;    //!< Copy mode of structured signals (element by element or raw whole copy)
    //@}
    
    /**
     * @brief Handle to the library hosting the model code.
     */
    LoadableLibrary* libraryHandle;

    /**
     * @name Library symbols
     * @brief Symbols retrieved from the library.
     */
    //@{
    void* (*instFunction)         (void);           //!< Pointer to the model function responsible for allocating model data.
    void  (*initFunction)         (void*);          //!< Pointer to the model function responsible for initialising the model.
    void  (*stepFunction)         (void*);          //!< Pointer to the model function responsible for running one step of the model.
    void  (*termFunction)         (void*);          //!< Pointer to the model function responsible for terminating the model execution (currently unused).
    void* (*getMmiFunction)       (void*);          //!< Pointer to the custom function returning the model mapping info (MMI) structure, which holds model data.
    void  (*getAlgoInfoFunction)  (void*);          //!< Pointer to the custom function returning information about model versioning.
    //@}
    
    /**
     * @brief Buffer hosting tunable parameters and internal states.
     */
    void* states;
    
    /**
     * @name    Simulink C API data structures
     * @brief   Pointers to Simulink C API data structures.
     * @details These struct pointers are used by Simulink C APIs
     *          to navigate into the loaded Simulink C code for
     *          data introspection.
     */
    //@{
    const rtwCAPI_ModelParameters* modelParams;     //!< Pointer to the structure storing tunable parameter data.
    const rtwCAPI_Signals*         rootInputs;      //!< Pointer to the structure storing root input data.
    const rtwCAPI_Signals*         rootOutputs;     //!< Pointer to the structure storing root output data.
    const rtwCAPI_Signals*         logSignals;      //!< Pointer to the structure storing internal logging signals.
    const rtwCAPI_DataTypeMap*     dataTypeMap;     //!< Pointer to the structure storing data types of parameters, inputs and outputs.
    const rtwCAPI_ElementMap*      elementMap;      //!< Pointer to the structure storing data for elements of a structured parameter or signal.
    const rtwCAPI_DimensionMap*    dimMap;          //!< Pointer to the structure storing the dimensions of parameters, inputs and outputs.
    const uint32*                  dimArray;        //!< Pointer to the array storing the number of elements for each dimension of parameters, inputs and outputs.
    void**                         dataAddrMap;     //!< Pointer to the structure storing the memory location of parameters, inputs and outputs.
    //@}

    /**
     * @name  Interface scan methods
     * @brief These methods are used to retrieve information about the model interfaces
     */
    //@{

    /**
     * @brief Scan the root level interfaces of a certain type
     * @param[out] interfaceArray   array of `SimulinkRootInterface`s that will contain the information
     * @param[in]  interfaceStruct  struct provided by the C APIs
     * @param[in]  numOfInterfaces  number of root interfaces to be retrieved
     * @param[in]  mode             interface type (InputPort, OutputPort, Parameter, Signal, Element)
     */
    ErrorManagement::ErrorType ScanInterfaces(SimulinkRootInterface* interfaceArray, const void* const interfaceStruct, const uint32 numOfInterfaces, const InterfaceType mode);

    /**
     * @brief Scan the single interfaces of a certain type
     * @param[in,out] interfaceArray   array of `SimulinkRootInterface`s that will contain the information
     * @param[in]     interfaceStruct  struct provided by the C APIs
     * @param[in]     sigIdx           index of the interface
     * @param[in]     numOfInterfaces  number of root interfaces to be retrieved
     * @param[in]     mode             interface type (InputPort, OutputPort, Parameter, Signal, Element)
     * @param[in]     parentAddr       pointer to the memory of the parent interface
     * @param[in]     parentName       name of the parent interface
     */
    ErrorManagement::ErrorType ScanInterface(SimulinkRootInterface &interfaceArray, const void* const interfaceStruct, const uint32 sigIdx, const InterfaceType mode, void* const parentAddr = NULL_PTR(void*), const StreamString parentName = "");
    //@}

    /**
     * @brief     Check coherence between model ports and GAM signals and map them.
     * @param[in] interfaceType specifies if the method will map inputs, outputs,
     *                          parameters or logging signals
     */
    ErrorManagement::ErrorType MapPorts(const InterfaceType interfaceType);
    
    /**
     * @brief Prepare model for execution.
     */
    ErrorManagement::ErrorType SetupSimulink();
    
    /**
     * @brief     Returns `true` if a C API type has the same size of checkSize.
     * @param[in] cTypeName the type name in C standard.
     * @param[in] checkSize the size in bytes
     * @returns   `true` if a C type has the same size of checkSize.
     */
    bool CheckrtwCAPITypeAgainstSize(StreamString cTypeName, const uint16 checkSize) const;

    /**
     * @brief Set to `true` if one of the parameters is an array of structures (currently unsupported).
     */
    bool paramsHaveStructArrays;

    /**
     * @brief Set to true if the corresponding GAM signal is mapped to a model input/output port
     *        or logging signal. Used to ensure that all GAM signals are mapped.
     */
    Vector<bool> isInputMapped;
    Vector<bool> isOutputMapped;

    /**
     * @brief Container of the parameters retrieved from the configuration file.
     */
    ReferenceT<ReferenceContainer> cfgParameterContainer;
    
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
