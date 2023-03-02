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

    /**
     * @brief Initialise the RtVMap from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity. This implementation redefines the one in UEIMapContainer altough that implementation
     * is used to check and retrieve general parameters common among data acquisition map classes.
     * @return true if every parameter has been read correctly and validated.
     */
    bool Initialise(StructuredDataI &data);
    
    /**
     * @brief Close the Map structure in a clean way.
     * @details Function called from UEIMasterObject prior to destruction of the object to grant a 
     * clean exit from map operation. This function redefines the procedure defined in UEIMapContainer class for RtVMap.
     * @return true if the map has been closed correctly and cleanly.
     */
    bool CleanupMap();
    
    /**
     * @brief Method to perform map initialisation in the IOM.
     * @details This method performs the initialisation procedures for the map by issuing the appropriate commands directly to IOM.
     * The IOM handle supplied to this method will be saved for later usage on polling/data recieving operations.
     * Specific implementation for RtVMap operation
     * @param[in] DAQ_handle_ handle to the IOM, must be provided by UEIMasterObject to this method.
     * @return true if the initialisation and starting procedure succeeds for this map.
     */
    bool StartMap();

    /**
     * @brief Method to poll the IOM for new data on the map.
     * @details This method polls the IOM for a new packet of data. This implementation redefines the behavior defined in UEIMapContainer
     * for RtVMap data acquisition.
     * @param[out] destinationAddr pointer to the memory region where the contents of the newly recived (if so) map packet are copied.
     * @return true if a new packet has been recieved, false otherwise.
     */
    bool PollForNewPacket(float32* destinationAddr);
    
    /**
     * @brief Getter for the type of the map.
     * @details Redefinition of the method stated in UEIMapContainer.
     * @return RTVMAP as defined in UEIDefinitions.h.
     */
    uint8 GetType();

    bool SetMARTeSamplesPerSignal(uint32 MARTeSampleN);
private:
    /**
     * @brief Private method to calculate the correction indexes for the channels on a VMap.
     * @param[in] configuredChannelList pointer to an array of channels for which to generate the correction indexes.
     * @param[out] correctionCoefficientsList pointer to an array containing the correction indexes for the channel list provided.
     * @return true if the calculation was performed correclty, false otherwise.
     */
    bool CalculateCorrectionIndexes(uint32* configuredChannelList, int8* correctionCoefficientsList);

    /**
    *   Variable holding the last processed timestamp, used to detect timestamp overflow.
    */
    uint32 nSamplesinMarte;
    uint32 bufferSampleNumber;
};
}
#endif /* UEIRtVMap_H_ */