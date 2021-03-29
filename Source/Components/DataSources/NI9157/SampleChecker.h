/**
 * @file SampleChecker.h
 * @brief Header file for class SampleChecker.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * SampleChecker with all of its public, protected and private members. It may
 * also include definitions for inline methods which need to be visible to the
 * compiler.
 */

#ifndef NI9157SAMPLECHECKER_H_
#define NI9157SAMPLECHECKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe { 

/**
 * @brief The SampleChecker class. This class is used by the CounterChecker and
 * MarkerBitChecker classes of the NI9157 DataSource.
 * @see SampleChecker::Initialise for details on the configuration parameters.
 */
class SampleChecker: public Object {
public:

    /**
     * @brief Constructor.
     * @post
     *  sampleSize = 0u;
     *  nFrameForSync = 0u;
     */
    SampleChecker();

    /**
     * @brief Destructor.
     */
    virtual ~SampleChecker();

    /**
     * @see Object::Initialise.
     * @details Follows the list of parameters to be configured by the user
     * for initialisation: \n
     * SampleSize: The size of each sample in bytes. Must be defined. \n
     * NumOfFrameForSync: The number of frames to resync/sync. If not defined
     * then the default value 1 is used and a warning message is issued.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Object::Check.
     */
    virtual bool Check(uint8 *sample,
                       bool &write) = 0;

    /**
     * @brief Gets the value of the NumOfFrameForSync parameter.
     * @details Retruns the value of the nFrameForSync parameter.
     * @return nFrameForSync.
     */
    uint8 GetNumberOfFramesToSync() const;

    /**
     * @see Object::Synchronise.
     */
    virtual bool Synchronise(uint8 *frames,
                               uint32 sizeToRead,
                               uint32 &idx,
                               bool &write) = 0;

protected:

    /**
     * The size of each sample in bytes.
     */
    uint8 sampleSize;

    /**
     * The number of frames to resync/sync.
     */
    uint8 nFrameForSync;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157SAMPLECHECKER_H_ */
