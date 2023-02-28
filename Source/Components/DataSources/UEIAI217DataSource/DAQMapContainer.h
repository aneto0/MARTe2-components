/**
 * @file DAQMapContainer.h
 * @brief Header file for class DAQMapContainer
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

 * @details This header file contains the declaration of the class DAQMapContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAQMapContainer_H_
#define DAQMapContainer_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SingleThreadService.h"
#include "MemoryDataSourceI.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include <algorithm>
#include "PDNA.h"
#include "UEIDefinitions.h"
#include "UEIAI217_803.h"
#include "DAQCircularBuffer.h"
#include "ClassRegistryDatabase.h"


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
 *        Class             = DAQMapContainer
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

/** @struct IOMapMember
 *  @brief This structure contains the information of a map member on a specific directio (input/output)
 *  @var defined boolean defining if this direction for the map member is configured.
 *  @var channels pointer to an array containing the channel identifier list for this specific map member and direction.
 *  @var channels correctionIndexes array of correction coefficients to translate the channel order from natural order (VMap return order) to user-configured order.
 *  @var nChannels variable stating the number of different channel identifiers present in channels variable of this structure.
 *  @var samples number of samples to obtain from a VMap for a specific member (device).
 */
typedef struct{
    bool defined;
    uint32* channels;
    int8*   correctionIndexes;
    uint32  nChannels;
    ReferenceT<DAQCircularBuffer> buffer;
    uint32 requestSize;
}IOMapMember;

/** @struct mapMember
 *  @brief This structure contains the information of a map member (hardware layer defined within the map)
 *  @var defined boolean defining if this map member (hardware layer) is configured.
 *  @var reference smart pointer to the object holding the device configuration and configuration methods.
 *  @var devn identifier of the device or hardware layer to which this specific member is associated.
 *  @var Inputs IOMapMember structure containing the configured channels of this map member as input channels.
 *  @var Outputs IOMapMember structure containing the configured channels of this map member as output channels
 */
typedef struct{
    bool defined;
    ReferenceT<UEIAI217_803> reference;
    uint8 devn;
    IOMapMember Inputs;
    IOMapMember Outputs;
}mapMember;

class DAQMapContainer : public ReferenceContainer {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    DAQMapContainer();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~DAQMapContainer();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Close the DAQ Map structure in a clean way.
     * @details This function must be called by DAQMasterObject before destruction
     * of the IOM handle to ensure clean closing of the DAQ Map.
     * @param[in] DAQ_handle handler of the IOM to which the map belongs.
     * @return true if the map has been closed correctly and cleanly.
     */
    bool CleanupMap();
    
    /**
     * @brief Getter for defiened devices (by devn) in a map.
     * @param[in] devn device number to be checked for use in the map.
     * @return true if the specified device is used in this map and false if not.
     */    
    bool GetDevDefined(uint32 devn);

    /**
     * @brief Getter for defiened devices (by devn and direction) in a map.
     * @param[in] devn device number to be checked for use in the map.
     * @param[in] direction direction of the device to be checked for use in the map.
     * @return true if the device specified by the user is used in the map in the specified direction, false otherwise.
     */    
    bool GetDevDefined(uint32 devn, uint8 direction);

    /**
     * @brief Getter for number of defined channels in the map for a device in a specific direction.
     * @param[in] devn device number to be checked.
     * @param[in] direction direction of the signals to be checked for the selected device.
     * @param[out] nChannels number of channels defined for the specified device and direction on this map.
     * @return true if the device is valid and defined within the map and the direction of the signal is valid, false otherwise.
     */
    bool GetNumberOfChannels(uint32 devn, uint8 direction, uint32* nChannels);

    /**
     * @brief Getter for number of defined channels in the map for a specific direction.
     * @param[in] direction direction of the signals to be checked for the map.
     * @param[out] nChannels number of channels defined for the specified direction on this map.
     * @return true if direction of the signal is valid, false otherwise.
     */
    bool GetNumberOfChannels(uint8 direction, uint32 &nChannels);

    /**
     * @brief Getter for specifiec channel identifier of a map member (device within map) in a specified direction.
     * @param[in] devn device number (member identifier) from which to get channel identifier.
     * @param[in] direction direction of the signal to be retrieved.
     * @param[in] channelIdx index within channel list of the selected device and direction.
     * @param[out] channelNumber channel identifier for the selected channel index, device and direction in the map.
     * @return true if the specified device, direction and channelIdx are valid, false otherwise.
     */
    bool GetChannelOfMember(uint32 devn, uint8 direction, uint32 channelIdx, uint32* channelNumber);

    /**
     * @brief Setter for the reference of a device object into the map member.
     * @param[in] devn device number (member identifier) to which the reference is pointing.
     * @param[in] reference reference of the device object to be set in the specified map member.
     * @return true if the specified map member and reference are valid.
     */
    bool SetDevices(ReferenceT<UEIAI217_803>* referenceList); //TODO

    /**
     * @brief Method to perform map initialisation and start in the IOM.
     * @param[in] DAQ_handle_ handle to the IOM, must be provided by DAQMasterObject to this method.
     * @return true if the initialisation and starting procedure succeeds for this map.
     */
    bool StartMap(int32 DAQ_handle_);

    /**
     * @brief Getter for the name of this map object.
     * @return The name of the map as a char8 array pointer.
     */
    char8* GetName();

    /**
     * @brief Getter for the specified scan rate of this map.
     * @return The scan rate of this map (in Hz).
     */
    float GetsampleRate();

    /**
     * @brief Method to poll the IOM for new data on the map.
     * @param[out] destinationAddr pointer to the memory region where the contents of the newly recived (if so) map packet are copied.
     * @return true if a new packet has been recieved, false otherwise.
     */
    bool PollForNewPacket(float64* destinationAddr, uint32 nChannels);

    /**
     * @brief Method to lock the map access by a DataSource in a certain direction.
     * @details This method is used to ensure that the map is only used by one DataSource in a given direction at a time to prevent uncontrolled access
     * by multiple DataSources. It is responsibility of the DataSource to check this method once upon initialisation to check for correct usage. Upon
     * usage of this method, the map gets locked in the specified direction for the calling DataSource.
     * @param[in] direction direction of the map access to be blocked.
     * @return true if the map has not been yet assigned to DataSource on the specified direction, false otherwise.
     */
    bool RegisterDS(SignalDirection direction);

    /**
     * @brief Method to check if the specified signal type is accepted as an output/input to the map in the specified direction.
     * @param[in] signalType type of the signal to be queried to the map for acceptance.
     * @param[in] direction direction of the signal to be queried to the map for acceptance.
     * @return true if the map accepts the signal type in the specified direction.
     */
    bool IsSignalAllowed(TypeDescriptor signalType, uint8 direction);

    bool CheckMapCoherency();
    uint8 GetType();

private:
    /**
     * @brief Private method to calculate the correction indexes for the channels on a VMap.
     * @param[in] configuredChannelList pointer to an array of channels for which to generate the correction indexes.
     * @param[out] correctionCoefficientsList pointer to an array containing the correction indexes for the channel list provided.
     * @return true if the calculation was performed correclty, false otherwise.
     */
    bool CalculateCorrectionIndexes(uint32* configuredChannelList, int8* correctionCoefficientsList);

  bool GetMapPointers();

    //Private function to calculate and correct the timestamp of the recieved Data packet and extend it to 64 bit precision
    //For example, a 32 bit time-stamp can only reach up to 1.2h on maximum resolution (0.1 us), but with 64 bit, up to 60000 years can be reached
    uint64 GetTiemstamp(uint32 timestamp);
    
    /**
    *   Variable holding the name of the Map Container (node name).
    */
    StreamString name;

    /**
    *   Variable holding the map type defined in configuration and translated
    *   to numerical value (see defines in DAQMapContainer.h).
    */
    uint8 mapType;
    
    /**
    *   Variable holding the configured sample rate for this specific map's IO layers (for XVMap).
    */
    float sampleRate;


    /**
    *   Variable holding the configured scan rate for this specific map.
    */
    float scanRate;

    /**
    *   Id of this map as assigned during map initialisation.
    */
    int32 mapid;
    
    /**
    *   Pointer to the start of the input memory map for this map.
    */
    uint32* inputMapPtr;
    
    
    /**
    *   Array containing the members of this map regarding output signals (the ones coming from IOM) (indexed by devn).
    */
    mapMember members [MAX_IO_SLOTS];

    /**
    *   Pointer to array of references of output members (in configuration order).
    */
    mapMember** outputMembersOrdered;

    uint32 nOutputMembers;
    uint32 nOutputChannels;
    
    /**
    *   Pointer to array of references of input members (in configuration order).
    */
    mapMember** inputMembersOrdered;

    uint32 nInputMembers;
    uint32 nInputChannels;
    int32 DAQ_handle;
    uint32* outputMap;
    uint32* inputMap;
    uint32 poll_sleep_period;
    bool outputAssignedToDS;
    bool inputAssignedToDS;
    
    /**
    *   Variable holding the max number of outputs to be retrieved for each VMap query.
    */
    uint32 sampleNumber; 

    /**
    *   Variable holding the corrector factor to the timestamp (the 32 MSB of the timestamp)
    *   This variable gets increased by 1 every time the recived timestamp decreases respective to its last value 
    *   (32-bit timestamp overflow).
    */
    uint32 timestampCorrector;
    
    /**
    *   Variable holding the last processed timestamp, used to detect timestamp overflow.
    */
    uint32 lastTimestamp;
};
}
#endif /* DAQMapContainer_H_ */