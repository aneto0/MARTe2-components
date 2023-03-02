/**
 * @file UEIMapContainer.h
 * @brief Header file for class UEIMapContainer
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

 * @details This header file contains the declaration of the class UEIMapContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIMapContainer_H_
#define UEIMapContainer_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabase.h"
#include "MemoryDataSourceI.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include <algorithm>
//interface specific includes
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
    ReferenceT<UEICircularBuffer> buffer;
    bool timestampRequired;
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
    ReferenceT<UEIDevice> reference;
    uint8 devn;
    IOMapMember Inputs;             //IO map member containing the Input members to the map (electrical signals into the ADC)
    IOMapMember Outputs;            //IO map member containing the Output members to the map (signals fed into the DAC to convert to electrical signals)
}mapMember;

class UEIMapContainer : public ReferenceContainer {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIMapContainer();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIMapContainer();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Method to perform map initialisation in the IOM.
     * @details This method performs the initialisation procedures for the map by issuing the appropriate commands directly to IOM.
     * The IOM handle supplied to this method will be saved for later usage on polling/data recieving operations.
     * This function must be reimplmented by each of the derived map classes, as the map configuration procedures is map-type dependant.
     * @param[in] DAQ_handle_ handle to the IOM, must be provided by UEIMasterObject to this method.
     * @return true if the initialisation and starting procedure succeeds for this map.
     */
    virtual bool StartMap(int32 DAQ_handle_);
    
    /**
     * @brief Method to poll the IOM for new data on the map.
     * @param[out] destinationAddr pointer to the memory region where the contents of the newly recived (if so) map packet are copied.
     * @return true if a new packet has been recieved, false otherwise.
     */
    virtual bool PollForNewPacket(float64* destinationAddr);

    /**
     * @brief Getter for the type of the map.
     * @details This function is implemented in the base class to return NOMAP type as this function must be reimplemented
     * by the child class and return the appropriate class for the selected map.
     * @return the type of the map as defined in UEIDefinitions.h.
     */
    virtual uint8 GetType();

    /**
     * @brief Close the DAQ Map structure in a clean way.
     * @details This function must be called by UEIMasterObject before destruction
     * of the IOM handle to ensure clean closing of the DAQ Map. Implementation of this method
     * must be done in each of the child map classes.
     * @return true if the map has been closed correctly and cleanly.
     */
    virtual bool CleanupMap();

    /**
     * @brief Method check the map coherency with the information on the devices assigned to each of the members.
     * @details This method is executed by UEIMasterObject right after the devices references are set into the corresponding map members
     * by the call to SetDevices method on this class. During initialisation of the map objects, the object has no access to the device objects and
     * therefore cannot execute a certain set of checks regarding the information. This method implement such checks and is to be called once the
     * device references have been set appropriately.
     * @return true if coherency check for the map has succeeded, false otherwise.
     */
    bool CheckMapCoherency();

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
     * @brief Getter for specific channel identifier of a map member (device within map) in a specified direction.
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
    bool SetDevices(ReferenceT<UEIDevice>* referenceList);

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

protected:
    /**
     * @brief Protected method which translates the 32-bit timestamp into a 64-bit timestamp.
     * @details This method implemented in the base class and common for all child classes performs the translation from 32-bit
     * timestamp received from the device into 64-bit precision timestamp (by monitoring the 32-bit timestamp overflow). 
     * @param[in] inputTimestamp the 32-bit timestamp received from the device.
     * @param[out] outputTimestamp the corrected 64-bit timestamp.
     * @return true if timestamp correction was performed successfully.
     */
    bool GetTimestamp(uint32 inputTimestamp, uint64 &outputTimestamp);
    /**
     * @brief Protected method which translates the 32-bit timestamp into a 64-bit timestamp.
     * @details This method implemented in the base class and common for all child classes performs the translation from 32-bit
     * timestamp received from the device into 64-bit precision timestamp (by monitoring the 32-bit timestamp overflow). 
     * @param[in] inputTimestamp a pointer to the 32-bit timestamp array received from the device.
     * @param[in] timestampListLength length of the supplied array of 32-bit timestamps. The destination array of 64-bit timestamps must be able
     * to accomodate such number of timestamps.
     * @param[out] outputTimestamp pointer to the destination array of corrected 64-bit timestamps.
     * @return true if timestamp correction was performed successfully.
     */
    bool GetTimestamp(uint32* inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp);

    /**
    *   Variable holding the name of the Map Container (node name).
    */
    StreamString name;
    
    /**
    *   Id of this map as assigned during map initialisation.
    */
    int32 mapid;

    /**
    *   Variable holding IOM handle. This handle should be acquired once the StartMap method is called.
    */
    int32 DAQ_handle;

    /**
    *   Variable holding the configured sample rate for this specific map's IO layers (for XVMap).
    */
    float sampleRate;

    /**
    *   Variable holding the configured scan rate for this specific map.
    */
    float scanRate;
    
    /**
    *   Array containing the members of this map regarding output signals (the ones coming from IOM) (indexed by devn).
    */
    mapMember members [MAX_IO_SLOTS];

    /**
    *   Pointer to array of references of output members (in configuration order). Ordered array pointing to members.
    */
    mapMember** outputMembersOrdered;

    /**
    *   Pointer to array of references of input members (in configuration order). Ordered array pointing to members.
    */
    mapMember** inputMembersOrdered;

    /**
    *   Variable holding the number of members defined as Output members.
    */
    uint32 nOutputMembers;

    /**
    *   Variable holding the number of members defined as Input members.
    */
    uint32 nInputMembers;

    /**
    *   Variable holding the total number of channels defined within the Output members of the map.
    */
    uint32 nOutputChannels;
    
    /**
    *   Variable holding the total number of channels defined within the Input members of the map.
    */
    uint32 nInputChannels;

    /**
    *   Flag signaling if this Map has already been requested to be part of a DataSource providing Output signals.
    */
    bool outputAssignedToDS;
    
    /**
    *   Flag signaling if this Map has already been requested to be part of a DataSource requesting Input signals.
    */
    bool inputAssignedToDS;
    
    /**
    *   Variable holding the max number of outputs to be retrieved for each map query. (The map can only provide a fixed amount of samples for 
    *   every channel, no different sample number per channel are allowed)
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
#endif /* UEIMapContainer_H_ */