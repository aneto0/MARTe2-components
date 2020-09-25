/**
 * @file MDSParameters.h
 * @brief Header file for class MDSParameters
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

 * @details This header file contains the declaration of the class MDSObjLoaderGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJPARAMETERS_H_
#define OBJPARAMETERS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief   AnyType wrapper class representing a parameter.
 * @details This class is a wrapper for AnyType inheriting also from ReferenceContainer,
 *          so that is has both MARTe::AnyType and MARTe::Object properties.
 *          This class can thus be instantiated in the configuration file,
 *          and its value can be used as a parameter by other MARTe objects
 *          using the AnyType APIs.
 *          
 *          The class is expected to be managed by a loader class that is
 *          responsible for populating the underlying AnyType by calling the
 *          ObjParameter::Actualize() method. The standard loader class for
 *          managing ObjParameter is ParObjLoader.
 * 
 *          The loader class shall be a derived class of ParObjLoader.
 * 
 *          The class inherits from AnyType so that parameter properties can be
 *          set and get with AnyType public interfaces.
 *          This class is de facto an AnyType with MARTe::Object properties.
 */
class ObjParameter: public ReferenceContainer, public AnyType {
    
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     */
    ObjParameter();

    /**
     * @brief Destructor
     */
    virtual ~ObjParameter();

    /**
     * @brief Generic initialization method.
     * @see   ReferenceContainer::Initialise()
     */
    virtual bool Initialise(StructuredDataI &data);
    
    /**
     * @brief   Mathod called by the parameter loader class to populate parameter properties.
     * @details 
     */
    virtual bool Actualize(ConfigurationDatabase &targetcdb);

    /**
     * @todo remove these methods, they are redundand since alternatives
     *       are provided by inheriting from AnyType.
     */
    //@{
        inline bool isValid() { return valid; }
        inline bool isUnlinked() { return unlinked; }
        inline StreamString &getType() { return type; }
        inline uint8 getDatatypesize() { return datatypesize; }
        inline uint8 getNumdims() { return  numdims; }
        inline uint32 getDimAt(uint8 idx) { return dims[idx]; }
        inline uint32 getSize() { return size; }
        inline void* getBuffer() { return databuffer; }
    //@}


protected:

    StreamString MARTePath;             //!< Absolute path of the parameter in MARTe2 ObjectRegistryDatabase.

    /**
     * @todo remove these members, they are redundant (alternatives provided by AnyType)
     */
    //@{
        bool valid;                         //!< validity flag, the parameter must be used only if this flag is true
        bool unlinked;                      //!< if true the parameter hasn't yet a source and can be safely skipped
        StreamString type;                  //!< C string containing the datatype name
        uint8 datatypesize;                 //!< size in bytes of the data type
        uint8 numdims;                      //!< number of dimensions
        uint32 *dims;                       //!< dimensions sizes
        uint32 size;                        //!< total size in bytes
        void *databuffer;                   //!< ready to copy databuffer
    //@}
    
    void* dataBuffer;

private:

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJPARAMETERS_H_ */
