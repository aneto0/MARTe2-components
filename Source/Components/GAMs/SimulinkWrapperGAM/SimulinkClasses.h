/**
 * @file SimulinkClasses.h
 * @brief Header file for classes inside SimulinkClasses.cpp
 * @date 27/05/2019
 * @author Galperti Cristian
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

#ifndef SIMULINKCLASSES_H_
#define SIMULINKCLASSES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"

//TODO Verify
#include "ClassRegistryDatabase.h"

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
 */
static const uint32 maxVariableNameLentgh = 40u;

/**
 * @brief Enum that specifies whether model nonvirtual buses should be
 *        mapped into structured signals or raw uint8 byte arrays.
 */
enum SimulinkNonVirtualBusMode {
    
    StructuredBusMode,
    ByteArrayBusMode
};

/*---------------------------------------------------------------------------*/
/*                               SimulinkDataI                               */
/*---------------------------------------------------------------------------*/

/**    ClassRegistryDatabase *crdSingleton = ClassRegistryDatabase::Instance();
 * @brief   Class that manages informations about Simulink(R) objects retrieved
 *          from the model shared library.
 * @details This class holds revelant informations about Simulink(R) objects
 *          (i.e. parameters, ports and signals). Properties that are specific
 *          to each object are managed by subclasses of this class.
 */
/*lint -esym(1790,MARTe::SimulinkDataI) it's ok for MARTe::SimulinkDataI to have no non-destructor virtual functions */
class SimulinkDataI {
    
public:
    StreamString dataClass;                 //!< Name of the class (parameter, signal or port).
    
    StreamString fullName;                  //!< Fully expanded Simulink data name.
    
    uint32 numberOfDimensions;              //!< Data number of dimensions
    uint32 numberOfElements[maxNumOfDims];  //!< Data number of elements in each dimension
    uint32 totalNumberOfElements;           //!< Total number of elements.
    
    rtwCAPI_Orientation orientation;        //!< data orientation retrieved from model .so
    
    uint32 byteSize;                        //!< Size in bytes occupied by this data.
    uint16 dataTypeSize;                    //!< Size of the type of this data.
    uint64 offset;                          //!< Data offset (used if the parameter or signal is part of a structure).
    
    StreamString   cTypeName;               //!< data type name (in C terminology)
    StreamString   MARTeTypeName;           //!< data type name (in MARTe terminology)
    TypeDescriptor type;                    //!< data type
    
    void *address;                          //!< allocated starting address
    
    uint32 verbosity;                       //!< The amount of informations that will be printed (derived from the GAM verbosity).
    
    /**
     * @brief Default constructor.
     */
    SimulinkDataI();
    
    /**
     * @brief Destructor.
     */
    virtual ~SimulinkDataI();
    
    /**
     * @brief Prints informations about the element, being it parameter, port or signal.
     * @param[in] maxNameLength max number of characters reserved for the
     *                          parameter name in the printed line.
     */
    void PrintData(const uint64 maxNameLength = 0u, StreamString additionalText = "");
    
protected:

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
    bool TransposeAndCopy(void *const destination, const void *const source); 
    
    /**
     * @brief Templated version of TransposeAndCopy().
     */
    template<typename T>
    bool TransposeAndCopyT(void *const destination, const void *const source);
};

/*---------------------------------------------------------------------------*/
/*                            SimulinkParameter                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief   Class that manages informations about a Simulink(R) parameter retirieved
 *          from the model shared library.
 * @details This class inherits from SimulinkDataI and only adds the SimulinkParameter::Actualise()
 *          method, which is used to copy data from an input AnyType to the address
 *          pointed by the #address class member.
 */
class SimulinkParameter : public SimulinkDataI {
    
public:

    /**
     * @brief   Updates the value of a parameter inside the model
     *          shared library based on an AnyType.
     * @details This methods updates the parameter with name #fullName of the
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
     * @warning 3D matrices are supported, but their data orientation
     *          is *not* changed in any way. Make sure that they are stored
     *          in the source with the same orientation with which they
     *          are stored in the model. 
     * @return  `true` if parameter is correctly actualized, `false` otherwise.
     * @param[in] sourceParameter a reference to the AnyType pointing to the
     *                            parameter value that will be used to update the
     *                            model parameter.
     * 
     */
    bool Actualise(const AnyType& sourceParameter);

};

/*---------------------------------------------------------------------------*/
/*                              SimulinkSignal                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief   Class that manages informations about a Simulink(R) signal
 *          retirieved from the model shared library.
 * @details This class inherits from SimulinkDataI and only adds two
 *          members: #MARTeAddress and #offset (see their description
 *          for details).
 */
/*lint -esym(1790,MARTe::SimulinkSignal) it's ok for MARTe::SimulinkDataI to have no non-destructor virtual functions */
class SimulinkSignal : public SimulinkDataI {

public:
    
    /**
     * @brief Default constructor.
     */
    SimulinkSignal();
    
    void*  MARTeAddress;            //!< Addess of the MARTe signal that maps this model signal.
    
    bool requiresTransposition;     //!< `true` if this is signal is a ColumMajor matrix signal (MARTe2 uses row-major data orientation). 
    
    /**
     * @brief Print information about the signal.
     */
    void PrintSignal(const uint64 maxNameLength = 0u);
};

/*---------------------------------------------------------------------------*/
/*                               SimulinkPort                                */
/*---------------------------------------------------------------------------*/

/**
 * @brief   Class that manages informations about a Simulink(R) model port retirieved
 *          from the model shared library.
 * @details This class inherits from SimulinkSignal. Since a port in the model
 *          can carry more than one signal at time, SimulinkPort also encloses
 *          a #carriedSignals list of all signals carried by the port it is describing.
 * @warning Members with special behaviour:
 *          1. #type member of a port is meaningful if and only if `hasHomogeneousType == true`
 *          2. #orientation member of a port is meaningful if and only if `hasHomogeneousOrientation == true`
 */
class SimulinkPort : public SimulinkSignal {

public:
    
    /**
     * @brief Default constructor.
     */
    SimulinkPort();
    
    /**
     * @brief   Destructor.
     * @details Empties the #carriedSignals list.
     */
    virtual ~SimulinkPort();
    
    StreamString invalidationError;     //!< Invalidation error.
    
    bool isValid;                       //!< `true` if the port is valid.
    bool hasHomogeneousType;            //!< `true` if all signals carried by this port are of the same type.
    bool hasHomogeneousOrientation;     //!< `true` if all signals carried by this port are oriented in the same way.
    bool isTyped;                       //!< `true` if #type and #orientation for this port has already been set.
    bool isContiguous;                  //!< `true` if the port data is contiguous.
    //TODO Verify
    bool isStructured;                   //!< `true` if the port has to be treated like a structure of its child signals

    uint32 runningOffset;
    uint64 typeBasedSize;
    uint64 offsetBasedSize;
    uint32 CAPISize;
    
    void*  baseAddress;
    void*  lastSignalAddress;
    
    /**
     * @brief   Direction of the signal.
     * @details Can be input (`mode = InputSignals`) or output (`mode = OutputSignals`).
     */
    SignalDirection mode;
    
    /**
     * @brief A list of all signals passing through this port.
     */
    StaticList<SimulinkSignal*> carriedSignals;
    
    /**
     * @brief   Adds a signal to the list of signals carried by this port.
     * @details The method also increment the #offsetBasedSize of this port
     *          based on the offset of the new signal.
     * @return  `true` if the signal was correctly added to the list.
     */
    bool AddSignal(SimulinkSignal* const signalIn);
    
    /**
     * @brief  Check the contiguity of the port data.
     * @return `true` if the port has contiguous data.
     */
    inline bool IsContiguous() const { return (CAPISize == offsetBasedSize); }
    
    /**
     * @brief Print information about this port.
     */
    void PrintPort(const uint64 maxNameLength);
    
    /**
     * @brief Copy data from the associated MARTe2 signal to the associated model port.
     */
    virtual bool CopyData(SimulinkNonVirtualBusMode copyMode) = 0;
};

/*---------------------------------------------------------------------------*/
/*                             SimulinkInputPort                             */
/*---------------------------------------------------------------------------*/

/**
 * @brief   Class that manages informations about a Simulink(R) model input port retirieved
 *          from the model shared library.
 */
class SimulinkInputPort : public SimulinkPort {
public:
    
    /**
     * @brief Default constructor.
     */
    SimulinkInputPort();
    
    /**
     * @brief Destructor.
     */
    virtual ~SimulinkInputPort();
    
    /**
     * @brief   Copy data from the associated MARTe2 signal to the associated model port.
     * @details The method checks if the signal needs transposition
     *          before copying, and transpose the signal if needed.
     *          Only column-major matrix signals require transposition.
     *          MARTe2 works in row-major orientation, so if the model
     *          has column-major matrix signals they get transposed.
     * @returns `true` if data is successfully copied, `false` otherwise.
     */
    virtual bool CopyData(const SimulinkNonVirtualBusMode copyMode);
};

/*---------------------------------------------------------------------------*/
/*                            SimulinkOutputPort                             */
/*---------------------------------------------------------------------------*/

/**
 * @brief   Class that manages informations about a Simulink(R) model output port retirieved
 *          from the model shared library.
 */
class SimulinkOutputPort : public SimulinkPort {
public:
    
    /**
     * @brief Default constructor.
     */
    SimulinkOutputPort();
    
    /**
     * @brief Destructor.
     */
    ~SimulinkOutputPort();
    
    /**
     * @brief Copy data from the associated model port to the associated MARTe2 signal.
     * @details The method checks if the signal needs transposition
     *          before copying, and transpose the signal if needed.
     *          Only column-major matrix signals require transposition.
     *          MARTe2 works in row-major orientation, so if the model
     *          has column-major matrix signals they get transposed.
     * @returns `true` if data is successfully copied, `false` otherwise.
     */
    virtual bool CopyData(const SimulinkNonVirtualBusMode copyMode);
};



} /* namespace MARTe */

#endif /* SIMULINKCLASSES_H_ */
