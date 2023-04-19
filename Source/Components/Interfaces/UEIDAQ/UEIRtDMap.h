/**
 * @file UEIRtDMap.h
 * @brief Header file for class UEIRtDMap
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

 * @details This header file contains the declaration of the class UEIRtDMap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIRtDMap_H_
#define UEIRtDMap_H_

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
#include "HighResolutionTimer.h"
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
 * @brief Class implementing the Data acquisition mechanics for RtDMap Data Acquisition map on UEIDAQ devices.
 * @details This class manages and performs all the data-acquisition operations related to a single configured
 * RtDMap data acquisition map. Objects of this class must be instantiated under the Maps Reference Container in
 * the UEIMasterObject to be properly configured. This class is derived from the generic UEIMapContainer class to
 * provide generic acces to maps from DataSource.
 *
 * The data acquisition mode managed through this specific class is capable and well-suited to retrieve data in real-time 
 * operation where the latest sample available must be retrieved but only one sample is to be used.
 *
 * An RtDMap is configured on a specific UEIDAQ device (the one represented by the UEIMasterObject where this map is
 * instantiated) to acquire data from a set Input/Output layers defined for such device. The devices from which to
 * retrieve the data must be declared in the Devices reference container of the UEIMasterObject for the UEIDAQ device.
 * The RtDMap's devices must be declared under the Inputs or Outputs section of its configuration.
 * 
 * Upon a call to PollForNewPacket, the map will be refreshed with latest-available samples for each channel. The DataSource
 * must interface the Map through calls to this method to retrieve latest data.
 *
 * <pre>
 * </pre>
 */

class UEIRtDMap : public UEIMapContainer {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIRtDMap();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIRtDMap();

    /**
     * @brief Initialise the RtDMap from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity. This implementation redefines the one in UEIMapContainer altough that implementation
     * is used to check and retrieve general parameters common among data acquisition map classes.
     * @return true if every parameter has been read correctly and validated.
     */
    bool Initialise(StructuredDataI &data);

    /**
     * @brief Close the Map structure in a clean way.
     * @details Function called from UEIMasterObject prior to destruction of the object to grant a 
     * clean exit from map operation. This function redefines the procedure defined in UEIMapContainer class for RtDMap.
     * @return true if the map has been closed correctly and cleanly.
     */
    bool CleanupMap();
    
    /**
     * @brief Method to perform map initialisation in the IOM.
     * @details This method performs the initialisation procedures for the map by issuing the appropriate commands directly to IOM.
     * The IOM handle supplied to this method will be saved for later usage on polling/data recieving operations.
     * Specific implementation for RtDMap operation
     * @param[in] DAQ_handle_ handle to the IOM, must be provided by UEIMasterObject to this method.
     * @return true if the initialisation and starting procedure succeeds for this map.
     */
    bool StartMap();

    /**
     * @brief Method to poll the IOM for new data on the map.
     * @details This method polls the IOM for a new packet of data. This implementation redefines the behavior defined in UEIMapContainer
     * for RtDMap data acquisition.
     * @param[out] destinationAddr pointer to the memory region where the contents of the newly recived (if so) map packet are copied.
     * @return true if a new packet has been recieved, false otherwise.
     */
    bool PollForNewPacket(MapReturnCode& outputCode);
    
    /**
     * @brief Getter for the type of the map.
     * @details Redefinition of the method stated in UEIMapContainer.
     * @return RTDMAP as defined in UEIDefinitions.h.
     */
    MapType GetType();
    
    /**
     * @brief Configuration method for the destination signals in MARTe.
     * @details This function provides the Map object the knowledge of where to store the scaled samples in memory as input signals in MARTe,
     * the desired number of samples to be retrieved in such signals, the number of physical channels to be retrieved and the types to which scale each
     * of the input signals.
     * @param[in] nSamples number of samples the map must supply for each of the declared phyisical channels as MARTe signals.
     * @param[in] nChannels number of channels the map needs to provide as input signals, this parameters serves as a check for the length of the provided arrays.
     * @param[in] inputTimestampAddress pointer to the memory location in which to store the scaled samples for the timestamp channel mandatory for all the maps.
     * @param[in] signalAddresses array of pointers to the memory locations of each of the input signals, must have a length of nChannels and be supplied in the same
     * order as the configured physical channels.
     * @param[in] signalTypes Array of types for each of the channels, stating the desired input signal format and scaling.
     * @return true if the provided parameters are accepted for the Map, false otherwise.
     */
    bool ConfigureInputsForDataSource(uint32 nSamples, uint32 nChannels, uint64* inputTimestampAddress, uint8** signalAddresses, TypeDescriptor* signalTypes);
    
    /**
     * @brief Method to check the map coherency with the information on the devices assigned to each of the members.
     * @details This method is executed by UEIMasterObject right after the devices references are set into the corresponding map members
     * by the call to SetDevices method on this class. During initialisation of the map objects, the object has no access to the device objects and
     * therefore cannot execute a certain set of checks regarding the information. This method implement such checks and is to be called once the
     * device references have been set appropriately.
     * @return true if coherency check for the map has succeeded, false otherwise.
     */
    bool CheckMapCoherency();
    
     /**
     * @brief Method to stop operation on the map.
     * @details This method is used to cease map operation.
     * @return true if the stopping request succeeded, false otherwise.
     */
    bool StopMap();

protected:

    /**
     * @brief Private method to obtain the pointers to the memory area in which the DMap will be allocated upon a successful refresh call.
     * @details This private method assigns the input map location for the DMap to the pointer inputMap if such can be retrieved.
     * @return true if the pointers could be retrieved successfully.
     */
    bool GetMapPointers();
    
    /**
     *  Pointer holding the location in which the DMap is copied after a successfull refresh request.  
     */
    uint8* inputMap;   //TODO change to uint8* for portability to new devices

    /**
    *   Variable holding the configured scan rate for this specific map.
    */
    float32 scanRate;
    
    /**
    *   Variable holding the last recieved timestamp samples for the timestamp channel on this map, use to compute 64-bit
    *   timestamp through timestamp overflow.
    */
    uint32 previousTimestamp;
    
    /**
    *   Variable holding the last sync time (Linux clock time) to compute the sleep if necessary.
    *   In ns as obtainde from HighResolutionTimer::Counter() method. No need for turnarround excpetion catching
    *   as the counter overflows after ~500 years
    */
    uint64 previousSyncTime;    
};
}
#endif /* UEIRtDMap_H_ */