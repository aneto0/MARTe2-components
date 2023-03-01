/**
 * @file UEIRtVMap.h
 * @brief Header file for class UEIRtVMap
 * @date 08/02/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIRtVMap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIRtVMap_H_
#define UEIRtVMap_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "MemoryDataSourceI.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include <algorithm>
//interface specific includes
#include "UEIMapContainer.h"
#include "UEIDevice.h"
#include "UEIDefinitions.h"
#include "UEICircularBuffer.h"
//PowerDNA library includes
#include "PDNA.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO
 * @details TODO
 *
 * <pre>
 *    +Map1 = {
 *        Class             = UEIRtVMap
 *         Type             = "RtDMap"            
 *         sampleRate         = 1.0   //Only meaningfull for RtDMap (VMap is synchronous to refresh function)    
 *         Samples          = 10    //Only meaningfull for VMap
 *         Inputs = {
 *           Devices = {                
 *             dev0 = {
 *                 Devn        = 0
 *                 Channels    = {0, 1, 2}
 *             }
 *             dev1 = {
 *                 Devn        = 1
 *                 Channels    = {0, 1, 2}   
 *             }
 *           }
 *         }
 *         Outputs = {
 *           Devices = {                
 *             dev0 = {
 *                 Devn        = 0
 *                 Channels    = {5, 6}
 *             }
 *             dev1 = {
 *                 Devn        = 1
 *                 Channels    = {7, 8}   
 *             }
 *           }
 *         }
 *      }
 * </pre>
 */

class UEIRtVMap : public UEIMapContainer {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIRtVMap();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIRtVMap();


    bool Initialise(StructuredDataI &data);

    bool CleanupMap();

    bool StartMap(int32 DAQ_handle_);

    bool PollForNewPacket(float64* destinationAddr);

    uint8 GetType();

private:
    /**
     * @brief Private method to calculate the correction indexes for the channels on a VMap.
     * @param[in] configuredChannelList pointer to an array of channels for which to generate the correction indexes.
     * @param[out] correctionCoefficientsList pointer to an array containing the correction indexes for the channel list provided.
     * @return true if the calculation was performed correclty, false otherwise.
     */
    bool CalculateCorrectionIndexes(uint32* configuredChannelList, int8* correctionCoefficientsList);
};
}
#endif /* UEIRtVMap_H_ */