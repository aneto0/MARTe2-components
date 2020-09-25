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

#ifndef MDSPARAMETERS_H_
#define MDSPARAMETERS_H_

#define CSTRINGMAXLEN 512

#define MDSTHINCLIENT
//#define MDSDISTCLIENT

//#define IGNORE_MDSSUCCESS_EXCEPTION
#ifdef IGNORE_MDSSUCCESS_EXCEPTION
#define __MDSEXCHECK__ strncmp(ex.what(), "%SS-W-SUCCESS", 13)
#else
#define __MDSEXCHECK__ 1
#endif


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "ReferenceContainer.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "mdsobjects.h"

#include "ObjParameters.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief
 * 
 * `DataOrientation` is used to specify the orientation of data as it is
 * stored in MDSplus. MDSplus is not aware of the orientation of data,
 * so this must be know to the user. Valid orientations are `RowMajor`
 * and `ColumnMajor`. In case no `DataOrientation` is specified, or the
 * value is not valid, `ColumnMajor` is assumed.
 * 
 * @warning Data stored in MDSplus from MATLAB(r) are usually `ColumnMajor`.
 * 
 * @warning No assumption is made for 3D matrices: their value is copied
 *          as-is. Make sure they are stored in MDSplus with the
 *          correct data orientation.
 * 
 * ~~~~
 * DataOrientation = RowMajor
 * ~~~~
 * 
 * When using `StartIdx` and `StopIdx` the returned type is the highest
 * datatype found while forming the array.
 * 
 */
class MDSParameter: public ObjParameter {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MDSParameter();

    /**
     * @brief Destructor
     */
    virtual ~MDSParameter();

    /**
     * @see ReferenceContainer::Initialise()
     */
    virtual bool Initialise(StructuredDataI &data);

    inline  void SetConnection(StreamString &setServer, StreamString &setTree, StreamString &setName) { serverName = setServer; treeName = setTree; connectionName = setName; }

    virtual bool Actualize(ConfigurationDatabase &targetcdb, MDSplus::Connection *conn, MDSplus::Tree *tree, StreamString &baseClassName);
    
protected:
    
    StreamString MDSPath;               //!< Path of the parameter in the MDSplus tree from which it is retrieved.
    StreamString MDSOrientation;        //!< Name of the orientation of the data retrieved from MDSplus (not known by MDSplus, must be specified by the user).
    uint32       targetDim;             //!< Take the first `targetDim` elements of an array if != 0.
    uint32       startIdx;                 
    uint32       stopIdx;
    
private:

    StreamString serverName;
    StreamString treeName;
    StreamString connectionName;
    
    /**
     * @brief Used to store orientation of multidimensional data (matrices and 3D matrices).
     */
    enum DataOrientation {
        RowMajor,
        ColumnMajor,
        None
    };
    
    DataOrientation sourceParamOrientation; //!< Orientation of the data retrieved from MDSplus (not known by MDSplus, must be specified by the user).
    
    /**
     * @brief   Copy the memory converting from column-major to row-major ordering.
     * @details Required since source parameters may be in column-major order, but
     *          MARTe2 uses row-major order.
     */
    bool TransposeAndCopy(TypeDescriptor type, void *const destination, const void *const source);
    
    /**
     * @brief   Templated method to reorder data from column-major to row-major.
     */
    template<typename T>
    bool TransposeAndCopyT(void *const destination, const void *const source);

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSPARAMETERS_H_ */
