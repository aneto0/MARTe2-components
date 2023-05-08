/**
 * @file UEIMapContainer.h
 * @brief Header file for class UEIMapContainer
 * @date 22/03/2023
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
#include "CLASSMETHODREGISTER.h"
#include "MemoryDataSourceI.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "AdvancedErrorManagement.h"
#include "StreamString.h"
#include "MutexSem.h"
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

typedef enum{
    NoMode,
    UEIMapRealTimeThreadExecutionMode,
    UEIMapIndependentThreadExecutionMode
}UEIMapExecutionMode;

/** @struct IOMapMember
 *  @brief This structure contains the information of a map member on a specific direction (input/output)
 */
typedef struct{
    /**
    *   Boolean flag defining if this direction for the map member is configured.
    */
    bool defined;
    /**
    *   Array containing the channel identifier list for this specific map member and direction.
    */
    uint32* channels;
    /**
    *   Variable stating the number of different channel identifiers present in channels variable of this structure.
    */
    uint32  nChannels;
    /**
    *   Size of the packet to be requested for this map member and direction in bytes.
    */
    uint32 requestSize;
    /**
    *   Flag indicating if a timestamp is required to be retrieved for this map member (only meaningful if the member is an input member).
    */
    bool timestampRequired;
}IOMapMember;

/** @struct MapMember
 *  @brief This structure contains the information of a map member (hardware layer defined within the map)
 */
typedef struct{
    /**
    *   Boolean flag defining if this map member is configured.
    */
    bool defined;
    /**
    *   ReferenceT to a UEIDevice Object associated to this map member (the object associated to the actual hardware layer).
    */
    ReferenceT<UEIDevice> reference;
    /**
    *   Variable holding the device identifier (devn) for the hardware layer associated to this member.
    */
    uint8 devn;
    /**
    *   IOMapMember structure holding the information for this map member as input member
    */
    IOMapMember Inputs;
    /**
    *   IOMapMember structure holding the information for this map member as output member
    */
    IOMapMember Outputs;
}MapMember;

/**
 * @brief Base class for DAQ Map implementation on the UEIDAQ interface.
 * @details This class serves as a generic base class for implementation of different specificic data acquisition maps on the UEIDAQ ecosystem.
 * The class provides generic access to different specific maps without the need to know exactly the class of the map in compile time as this base
 * class serves as generic interface. The methods interfacing with the API which are map-specific must be implemented on each of the derived map classes.
 *
 * This class also provides generic checks and parameters which are generic to all the maps, such as name of the map or list of members.
 * Objects of this class must not be directly instantiated on a MARTe application for data retrieving purposes as the methods implementing data retrieving
 * operations are declared but must be implemented on derived classes such as UEIRtVMap or UEIRtDMap.
 *
 */
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
     * their validity. This is a common implementation for all the different maps, general checks are performed but
     * map-specific checks must be implmented in derived classes for each specific map.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Method to perform map initialisation in the IOM.
     * @details This method performs the initialisation procedures for the map by issuing the appropriate commands directly to IOM.
     * The IOM handle supplied to this method will be saved for later usage on polling/data recieving operations.
     * This function must be reimplmented by each of the derived map classes, as the map configuration procedures is map-type dependant.
     * @return true if the initialisation and starting procedure succeeds for this map or the map is already started, false otherwise.
     */
    virtual bool StartMap();
    
    /**
     * @brief Method to retrieve the inputs of the map packet.
     * @details This method checks wether there's new data available to be retrieved from the DAQ map and retrieves
     * it from the devices input buffers into the MARTe signals (the ones configured in the DataSources)
     * @param[out] outputCode Variable holding the status code for the procedure performed during the call.
     * @return true no error occurred during the retrieve procedure.
     */
    virtual bool GetInputs(MapReturnCode& outputCode);

    /**
     * @brief Method to set the outputs of the map packet.
     * @details This method pushes the output signal values from MARTe (through the DataSource) into the output buffers
     * of the devices to be later pushed into the DAQ map
     * @param[out] outputCode Variable holding the status code for the procedure performed during the call.
     * @return true no error occurred during the setting procedure.
     */
    virtual bool SetOutputs(MapReturnCode& outputCode);

    /**
     * @brief Getter for the type of the map.
     * @details This function is implemented in the base class to return NOMAP type as this function must be reimplemented
     * by the child class and return the appropriate class for the selected map.
     * @return the type of the map as defined in UEIDefinitions.h.
     */
    virtual MapType GetType();

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
    virtual bool ConfigureInputsForDataSource(uint32 nSamples, uint32 nChannels, uint64* inputTimestampAddress, uint8** signalAddresses, TypeDescriptor* signalTypes);

    /**
     * @brief Configuration method for the destination signals in MARTe.
     * @details This function provides the Map object the knowledge of where to retrieve the output samples in memory as output signals in MARTe,
     * the desired number of samples to be pushed in such signals, the number of physical channels to be pushed and the types to which scale each
     * of the output signals.
     * @param[in] nSamples number of samples the map must push for each of the declared phyisical channels as MARTe signals.
     * @param[in] nChannels number of channels the map needs to psuh into as output signals, this parameters serves as a check for the length of the provided arrays.
     * @param[in] signalAddresses array of pointers to the memory locations of each of the output signals, must have a length of nChannels and be supplied in the same
     * order as the configured physical channels.
     * @param[in] signalTypes Array of types for each of the channels, stating the current output signal format and scaling.
     * @return true if the provided parameters are accepted for the Map, false otherwise.
     */
    virtual bool ConfigureOutputsForDataSource(uint32 nSamples, uint32 nChannels, uint8** signalAddresses, TypeDescriptor* signalTypes);

    /**
     * @brief Close the DAQ Map structure in a clean way.
     * @details This function must be called by UEIMasterObject before destruction
     * of the IOM handle to ensure clean closing of the DAQ Map. Implementation of this method
     * must be done in each of the child map classes.
     * @return true if the map has been closed correctly and cleanly.
     */
    virtual bool CleanupMap();

    /**
     * @brief Method to check the map coherency with the information on the devices assigned to each of the members.
     * @details This method is executed by UEIMasterObject right after the devices references are set into the corresponding map members
     * by the call to SetDevices method on this class. During initialisation of the map objects, the object has no access to the device objects and
     * therefore cannot execute a certain set of checks regarding the information. This method implement such checks and is to be called once the
     * device references have been set appropriately.
     * @return true if coherency check for the map has succeeded, false otherwise.
     */
    virtual bool CheckMapCoherency();

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
    bool GetDevDefined(uint32 devn, SignalDirection direction);

    /**
     * @brief Getter for number of defined channels in the map for a device in a specific direction.
     * @param[in] devn device number to be checked.
     * @param[in] direction direction of the signals to be checked for the selected device.
     * @param[out] nChannels number of channels defined for the specified device and direction on this map.
     * @return true if the device is valid and defined within the map and the direction of the signal is valid, false otherwise.
     */
    bool GetNumberOfChannels(uint32 devn, SignalDirection direction, uint32* nChannels);

    /**
     * @brief Getter for number of defined channels in the map for a specific direction.
     * @param[in] direction direction of the signals to be checked for the map.
     * @param[out] nChannels number of channels defined for the specified direction on this map.
     * @return true if direction of the signal is valid, false otherwise.
     */
    bool GetNumberOfChannels(SignalDirection direction, uint32 &nChannels);

    /**
     * @brief Getter for specific channel identifier of a map member (device within map) in a specified direction.
     * @param[in] devn device number (member identifier) from which to get channel identifier.
     * @param[in] direction direction of the signal to be retrieved.
     * @param[in] channelIdx index within channel list of the selected device and direction.
     * @param[out] channelNumber channel identifier for the selected channel index, device and direction in the map.
     * @return true if the specified device, direction and channelIdx are valid, false otherwise.
     */
    bool GetChannelOfMember(uint32 devn, SignalDirection direction, uint32 channelIdx, uint32 &channelNumber);

    /**
     * @brief Setter for the device references into the map member.
     * @details This method allows the Map object to obtain a reference to the different UEIDevices used within the map
     * to access device-specific functions. The list supplied to this method must be of length MAX_IO_SLOTS and contain
     * the references as ReferenceT objects to the different UEIDevices in the map indexed by device devn. This method uses the
     * implementation of Set to actually set the references.
     * If a devn is not used, the list must be contain an invalid ReferenceT object on such 
     * @param[in] referenceList List of ReferenceT<UEIDevice> in which the devices are indexed by devn.
     * @return true if the specified map member and reference are valid.
     */
    virtual bool SetDevices(ReferenceT<UEIDevice>* referenceList);

    /**
     * @brief Setter for the device references into the map member.
     * @details This method allows the Map object to obtain a reference to a UEIDevics used within the map
     * at the specific devn supplied.
     * @param[in] devn device id of the member to which the reference is set.
     * @param[in] reference ReferenceT of the device to be set as reference in device at devn specified
     * @return true if the specified map member and reference are valid.
     */
    bool SetDevice(uint8 devn, ReferenceT<UEIDevice> reference);

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
    bool IsSignalAllowed(TypeDescriptor signalType, SignalDirection direction);

    /**
     * @brief Method to set and check the DAQ handle to a map.
     * @details This method must be called by UEIMasterObject during map initialisation.
     * @param[in] DAQ_handle_ handle to the IOM supplied to this map.
     * @return true if DAQ handle is valid, false otherwise.
     */
    bool SetDAQHandle(int32 DAQ_handle_);

    /**
     * @brief Method to stop operation on the map.
     * @details This method is used to cease map operation.
     * @return true if the stopping request succeeded, false otherwise.
     */
    virtual bool StopMap();

    /**
     * @brief Getter for the map status, returning the current status.
     * @return true if the map is started, false otherwise.
     */
    bool GetMapStatus();
    
    /**
     * @brief Getter for the map execution mode (RealTimeThread or IndependentThread)
     * @return the current map execution mode.
     */
    UEIMapExecutionMode GetExecutionMode();

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
     * @brief Protected method which translates the 32-bit timestamp array into a 64-bit timestamp array.
     * @details Reimplementation of GetTimestamp method for timestamp arrays. 
     * @param[in] inputTimestamp a pointer to the 32-bit timestamp array received from the device.
     * @param[in] timestampListLength length of the supplied array of 32-bit timestamps. The destination array of 64-bit timestamps must be able
     * to accomodate such number of timestamps.
     * @param[out] outputTimestamp pointer to the destination array of corrected 64-bit timestamps.
     * @return true if timestamp correction was performed successfully.
     */
    bool GetTimestamp(uint32* inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp);

    /**
     * @brief Protected method which translates the 32-bit timestamp array into a 64-bit timestamp array.
     * @details Reimplementation of GetTimestamp method for timestamp arrays retrieved as UEIBufferPointer virtual arrays. 
     * @param[in] inputTimestamp a UEIBufferPointer object containing a virtual array reference to the timestamp channels in the UEICircularBuffer.
     * @param[in] timestampListLength length of the supplied array of 32-bit timestamps. The destination array of 64-bit timestamps must be able
     * to accomodate such number of timestamps.
     * @param[out] outputTimestamp pointer to the destination array of corrected 64-bit timestamps.
     * @return true if timestamp correction was performed successfully.
     */
    bool GetTimestamp(UEIBufferPointer inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp);

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
    *   Array containing the members of this map regarding output signals (the ones coming from IOM) (indexed by devn).
    */
    MapMember members [MAX_IO_SLOTS];

    /**
    *   Pointer to array of references of output members (in configuration order). Ordered array pointing to members.
    */
    MapMember** outputMembersOrdered;

    /**
    *   Pointer to array of references of input members (in configuration order). Ordered array pointing to members.
    */
    MapMember** inputMembersOrdered;

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
    uint32 nSamplesMapExchange;

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

    /**
    *   Flag signaling if the Map's packet needs to be fragmented.
    */
    bool fragmentedMap;

    /**
    *   Array of pointers to the memory locations of the input signals to the Map
    */
    uint8** inputSignalAddresses;

    /**
    *   Array of pointers to the memory locations of the input signals to the Map
    */
    uint8** outputSignalAddresses;

    /**
    *   Array of Descriptors for input signal types
    */
    TypeDescriptor* inputSignalTypes;

    /**
    *   Array of Descriptors for input signal types
    */
    TypeDescriptor* outputSignalTypes;

    /**
    *   Pointer to the timestamp input signal memory location
    */
    uint64* TimestampAddr;

    /**
    *   Flag signaling if a first packet has been recieved by this map.
    */
    bool firstPckt;
    
    /**
    *   Flag signaling if the map is properly started in the UEIDAQ.
    */
    bool mapStarted;
    
    /**
    *  Flag signaling if the input signals are correctly configured into the map.
    */
    bool inputSignalsConfigured;

    /**
    *  Flag signaling if the input signals are correctly configured into the map.
    */
    bool outputSignalsConfigured;

    /**
    *   Flag signaling if coherency check has been performed successfully and map is ready for further initialisation.
    */
    bool mapCoherent;

    /**
    *   Flag signaling the execution mode context for this map, either not set, RealTimeThread or IndependentThread
    */
    UEIMapExecutionMode executionMode;

    /**
    *   Mutex instance to ensure Map operation is thread safe, either for a multithread MARTe application (Write and Read
    *   DataSources in different MARTe RealTimeThreads) or by running the Map in a separate thread (executionMode UEIMapIndependentThreadMode)
    */
    MARTe::MutexSem mapMutex;

private:
    /**
     * @brief Private helper for this class to aid in Inputs/Outputs configuration block checking and assignment wihtout code repetition.
     * @details This method performs the basic checks on the Inputs and Outputs configuration blocks on any Map object to check for configuration
     * validity. This method should only be called twice during initialisation, once for Outputs block and another for Inputs blocks
     * @param[in] direction flag to signal which configuration block to check/retrieve
     * @param[in] nMembers number of members to be checked on such block
     * @param[in] orderedMembers pointer to ordered members list for convinience operations, this method will populate the supplied list.
     * @return true if all the checks were successful, false otherwise.
     */
    bool ParseIODevices(StructuredDataI &data, SignalDirection direction, uint32 nMembers, MapMember** orderedMembers);
};
}
#endif /* UEIMapContainer_H_ */