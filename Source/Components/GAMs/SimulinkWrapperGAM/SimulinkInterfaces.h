/**
 * @file SimulinkClasses.h
 * @brief Header file for class SimulinkClasses
 * @date 27/05/2019
 * @author Bertrand Bauvir
 * @author Cristian Galperti 
 * @author Nicolo Ferron 
 * @author Giuseppe Avon
 *
 * @copyright Copyright 2019 Swiss Plasma Center, EPFL Lausanne Switzerland
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of th    ClassRegistryDatabase *crdSingleton = ClassRegistryDatabase::Instance();e Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the classes in SimulinkClasses.cpp
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIMULINKINTERFACES_H_
#define SIMULINKINTERFACES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "StructuredDataIHelper.h"

/*---------------------------------------------------------------------------*/
/*                            C-API interface                                */
/*---------------------------------------------------------------------------*/
#include "rtw_modelmap.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe
{

/**
 * @brief Maximum number of dimensions for model signals and parameters.
 */
 /*lint -esym(551,MARTe::maxNumOfDims) the symbol is infact accessed in SimulinkDataI declaration and in SimulinkWrapperGAM.cpp */
static const uint32 maxNumOfDims = 3u;

/**
 * @brief Maximum name length for model signals and parameters (longer names are truncated).
 * @warning This variable should never be less than 35, otherwise the interface formatting routine may break.
 */
static const uint32 maxVariableNameLentgh = 50u;

/**
 * @brief Enum that specifies whether model nonvirtual buses should be
 *        mapped into structured signals or raw uint8 byte arrays.
 */
enum SimulinkNonVirtualBusMode {
    
    StructuredBusMode,
    ByteArrayBusMode
};

/**
* @brief Enum that specifies the source of an interface (signal, parameter or element)
*/
enum InterfaceType {
    InputPort,              //<! Root-level input
    OutputPort,             //<! Root-level output
    Parameter,              //<! Root-level tunable parameters
    Signal,                 //<! Signal logging interface
    Element,                //<! Sub-element of one of the above interfaces
    InvalidInterface
};

/*---------------------------------------------------------------------------*/
/*                             SimulinkInterface                             */
/*---------------------------------------------------------------------------*/

class SimulinkInterface {

public:

    SimulinkInterface();

    StreamString interfaceName;                 //!< Name of this interface
    StreamString parentPath;                    //!< The parent path of this interface
    StreamString fullPath;                      //!< Fully expanded signal path (different from interfaceName in structured interfaces only)

    InterfaceType interfaceType;

    /**
     * @name Simulink data
     * @brief Information retrieved from Simulink C APIs.
     */
    //@{
    const char8* CTypeName;              //!< data type name (in C terminology). If this interface is an enumeration this string is not the actual type name and is instead `numeric`.
    const char8* className;              //!< data class name (datatype if numeric, class name if struct or enum).
    StreamString MARTeTypeName;          //!< data type name (in MARTe terminology). If this interface is an enumeration this string is nevertheless the actual underlying type name.
    TypeDescriptor typeDesc;             //!< data type. If this SimulinkInterface is an enumeration this is nevertheless the actual underlying data type.

    uint32 numElements;                  //!< Total number of elements.
    uint32 numDimensions;                //!< Data number of dimensions
    Vector<uint32> dimensions;           //!< Data number of elements in each dimension
    rtwCAPI_Orientation orientation;     //!< data orientation (row-major or column-major)

    void* dataAddr;                      //!< pointer to the starting address of the memory allocated by the model to hold this interface
    void* MARTeAddress;                  //!< pointer to the corresponding MARTe memory
    uint32 byteSize;                     //!< Size in bytes required to hold this data.
    uint32 dataTypeSize;                 //!< Size of the type of this data.
    //@}

    void* destPtr;                       //<! Pointer to where the buffer of this interface should be copied to
    void* sourcePtr;                     //<! Pointer to where this interface shall retrive its buffer from
    bool transpose;                      //<! Whether the interface needs to be converted from col-major to row-major layout or viceversa.
    bool isStructured;                   //<! Whether the interface carries a structured data.


    /**
     * @brief   Updates the value of a parameter inside the model
     *          shared library based on an AnyType.
     * @details This methods updates the parameter with name #fullPath of the
     *          Simulink(R) model with the value of the input AnyType.
     *          Before updating, coherence checks between the input AnyType
     *          and the model parameter are carried out. Type, dimensionality
     *          and data size must agree, otherwise the actualization fails.
     *          If the parameter to be actualised is stored by the model
     *          as a 2D matrix in column-major orientation, then this
     *          method takes care of transposing it.
     * @warning The data pointed by the input AnyType are assumed to be
     *          in row-major format, and this is *not* checked by the
     *          Actualise() method. If the source parameter comes from
     *          MARTe2 configuration then correct orientation is
     *          guaranteed by MARTe2, but if the parameter comes from
     *          an external source by using SimulinkWrapperGAM
     *          `TunableParamExternalSource` option then care must be
     *          put in guaranteeing that the external source imports
     *          data in the correct row-major orientation.
     * @return  `true` if parameter is correctly actualized, `false` otherwise.
     * @param[in] sourceParameter a reference to the AnyType pointing to the
     *                            parameter value that will be used to update the
     *                            model parameter.
     */
    bool Actualise(const AnyType& sourceParameter);

    /**
     * @brief   Copy data from the source parameter to the model memory
     *          but transpose it in the process.
     * @details This method is used by parameters during actualisation
     *          (see SimulinkParameter::Actualise()) and by signals
     *          (see SimulinkSignal::CopyData()) when they require
     *          transposition. The model almost always uses column-major
     *          orientation to store data, while MARTe2 uses row-major
     *          orientation. Thus, when actualising matrix parameters
     *          or copying matrix signals often transposition is required.
     */
    bool TransposeAndCopy(void *const destination, const void *const source, const InterfaceType mode);

    /**
     * @brief Templated version of TransposeAndCopy().
     */
    template<typename T>
    bool TransposeAndCopyT(void *const destination, const void *const source, const InterfaceType mode);
};

/**
 * @brief   Represents each root interface (port, parameter, logging signal)
 * @details The root interface is represented both as a StaticList<SimulinkInterface*>
 *          (to be cycled over as a copy map) and as a structure (to preserve information
 *          about the structured nature of the interface and for printing purpose)
 */
class SimulinkRootInterface : public SimulinkInterface, public StaticList<SimulinkInterface*> {

public:

    /**
     * @brief Default constructor.
     */
    SimulinkRootInterface();

    /**
     * @brief   Default destructor.
     * @details Deallocates the underlying StaticList
     */
    ~SimulinkRootInterface();

    ConfigurationDatabase rootStructure;    //!< A ConfigurationDatabase representation of this interface's structure (mostly for printing).
    InterfaceType interfaceType;            //!< Whether this interface is an input/output, parameter or signal

    /**
     * @brief   Prints information about the interface.
     * @details This method prints information about a root-level interface
     *          and all its carried signals or parameters.
     */
    void Print(uint64 paddingLength = 50u, StreamString parentSpacer = "");

    /**
     * @brief   Calulate the bytesize of an interface.
     * @details This method calculates the bytesize of an interface, being it
     *          numeric or structure, by recursively navigating the rootStructure
     *          interface database.
     */
    uint32 GetInterfaceBytesize(ConfigurationDatabase structureIn);

    /**
     * @brief   Copy data from the model to the GAM or viceversa.
     * @details In case of `InputPort` and `Parameter` interfaces:
     *          data is copied from the GAM to the model.
     *          In case of `OutputPort` and `Signal` interfaces:
     *          data is copied from the model to the GAM.
     *          The method checks if the signal needs transposition
     *          before copying, and transpose the signal if needed.
     *          Only column-major matrix signals require transposition.
     *          MARTe2 works in row-major orientation, so if the model
     *          has column-major matrix signals they get transposed.
     * @returns `true` if data is successfully copied, `false` otherwise.
     */
    bool CopyData(const SimulinkNonVirtualBusMode copyMode);

};

} /* namespace MARTe */

#endif /* SIMULINKINTERFACES_H_ */
