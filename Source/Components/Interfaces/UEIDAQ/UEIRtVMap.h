/**
 * @file UEIRtVMap.h
 * @brief Header file for class UEIRtVMap
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
#include "EmbeddedServiceMethodBinderT.h"
#include "SingleThreadService.h"
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
 * @brief Class implementing the Data acquisition mechanics for RtVMap Data Acquisition map on UEIDAQ devices.
 * @details This class manages and performs all the data-acquisition operations related to a single configured
 * RtVMap data acquisition map. Objects of this class must be instantiated under the Maps Reference Container in
 * the UEIMasterObject to be properly configured. This class is derived from the generic UEIMapContainer class to
 * provide generic acces to maps from DataSource.
 *
 * The data acquisition mode managed through this specific class is capable and well-suited to retrieve data in buffered
 * operation where multiple samples are retrieved for each of the configured channels at the expense of a given time-delay.
 *
 * An RtVMap is configured on a specific UEIDAQ device (the one represented by the UEIMasterObject where this map is
 * instantiated) to acquire data from a set Input/Output layers defined for such device. The devices from which to
 * retrieve the data must be declared in the Devices reference container of the UEIMasterObject for the UEIDAQ device.
 *
 * The RtVMap's devices must be declared under the Inputs or Outputs section of its configuration. A Samples parameter must
 * be configured for the map, stating the maximum ammount of data samples to be retrieved for each configured channel to its
 * associated hardware layer.
 * 
 * The DataSource associated to this map object must set the ammount of samples per channel to be retrieved upon a PollForNewPacket
 * call. The samples to be retrieved must be the same for every channel. Since the hardware layers on the UEIDAQ may return a different
 * ammount of samples during polling for each of the hardware layers, a UEICircularBuffer object is instantiated for every hardware
 * layer, which provides synchronisation in terms of ammount of samples retrieved from hardware to allow synchronous operation
 * on the containing DataSource. A BufferSamples parameter must be provided to set the maximum ammount of samples stored in the
 * UEICircularBuffers associated with each device.
 *
 * This map supports two operation modes, RealTimeThread and IndependentThread modes, configured through the ExecutionMode parameter.
 * - RealTimeThread : In this mode the map exchange is only performed during the datasource synchronisation procedures (via GetInputs and SetOutpus).
 * therefore the MARTe loop needs to be fast-enough to ensure no FIFO overflow errors happen due to long periods of no map syncs.
 * - IndependentThread : In this mode the map spawns an independent thread upon map starting procedure. The separate thread is in charge of performing
 * the map synchronisation procedures in an async manner. While MARTe loop executes, the separate thread performs the map synchronisation in parallel,
 * allowing for decoupling the map exchange and MARTe loop times. In this mode, the DataSource sync points are only used to retrieve the data being
 * updated by the separate thread and making it available to the MARTe application when ready.
 *
 * Following a sample VMap configuration scheme is provided:
 * <pre>
 * +Map1 = {
 *     Class           = UEIRtVMap
 *     Samples         = 128
 *     BufferSamples   = 1000
 *     ExecutionMode   = RealTimeThread
 *     PollSleepPeriod = 1
 *     Inputs = {
 *         dev0 = {
 *             Devn     = 0
 *             Channels = {0,1}
 *         }
 *         dev1 = {
 *             Devn     = 1
 *             Channels = {0,1}
 *         }
 *     }
 * }
 * </pre>
 */

class UEIRtVMap : public UEIMapContainer, public EmbeddedServiceMethodBinderT<UEIRtVMap> {
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
     * @brief Method to retrieve the inputs of the map packet.
     * @details This method checks wether there's new data available to be retrieved from the DAQ map and retrieves
     * it from the devices input buffers into the MARTe signals (the ones configured in the DataSources)
     *
     * In this specific implementation, data is retrieved from the IOM during the call only if map is configured in ExecutionMode RealTimeThread, forcing
     * a VMap exchange by calling to MapExvhange method and gathering new input data and setting new otuput data.
     *
     * If the map is configured in IndependentThread ExecutionMode, the call checks in a loop if the input devices buffers are ready for a full sample read.
     * The map exchange retrieving new data to the IOM id performed thorugh the ExchangeMap call in the IndependentThreadCallback.
     * @param[out] outputCode Variable holding the status code for the procedure performed during the call.
     * @return true no error occurred during the retrieve procedure.
     */
    bool GetInputs(MapReturnCode& outputCode);

    /**
     * @brief Method to set the outputs of the map packet.
     * @details This method pushes the output signal values from MARTe (through the DataSource) into the output buffers
     * of the devices to be later pushed into the DAQ map.
     *
     * In this specific implementation, data is pushed into the IOM during the call only if map is configured in ExecutionMode RealTimeThread, forcing
     * a VMap exchange by calling to MapExvhange method and gathering new input data and setting new otuput data.
     *
     * If the map is configured in IndependentThread ExecutionMode, the call only sets the new output data into the output buffer of the relevant devices.
     * The map exchange pushing the set data is later pushed to the IOM thorugh the ExchangeMap call in the IndependentThreadCallback.
     * @param[out] outputCode Variable holding the status code for the procedure performed during the call.
     * @return true no error occurred during the setting procedure.
     */
    bool SetOutputs(MapReturnCode& outputCode);

    /**
     * @brief Getter for the type of the map.
     * @details Redefinition of the method stated in UEIMapContainer.
     * @return RTVMAP as defined in UEIDefinitions.h.
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
    bool ConfigureOutputsForDataSource(uint32 nSamples, uint32 nChannels, uint8** signalAddresses, TypeDescriptor* signalTypes);
    
    /**
     * @brief Method to stop operation on the map.
     * @details This method is used to cease map operation.
     * @return true if the stopping request succeeded, false otherwise.
     */
    bool StopMap();
    
    /**
     * @brief Method to check the map coherency with the information on the devices assigned to each of the members.
     * @details This method is executed by UEIMasterObject right after the devices references are set into the corresponding map members
     * by the call to SetDevices method on this class. During initialisation of the map objects, the object has no access to the device objects and
     * therefore cannot execute a certain set of checks regarding the information. This method implement such checks and is to be called once the
     * device references have been set appropriately.
     * @return true if coherency check for the map has succeeded, false otherwise.
     */
    bool CheckMapCoherency();

protected:
    /**
     * @brief Thread Callback for the independent thread
     * @details This method is the callback function for the independent thread spawned upon map startup if the map is configured with
     * IndependentThread ExecutionMode. This callback calls the ExchangeMap method of the class with a sleep pause of PollSleepPeriod ms.
     * @return ErrorManagement::NoError.
     */
    ErrorManagement::ErrorType IndependentThreadCallback(ExecutionInfo &info);

    /**
     * @brief Protected internal method to perform the VMap packet exchange with the IOM
     * @details This method performs the refresh of the VMap at the IOM and retrieves the new input data into the device's input buffers
     * and pushes the output data into the Map paket to the IOM, emptying the output buffers in the process.
     * 
     * This method is invoked by both the GetInputs and SetOutputs methods if the map execution mode is set to RealTimeThread, in which case
     * the data is only pushed/retrieved from the IOM during this calls, making the map susceptible to FIFO overflows if such methods are not called
     * frequently enough.
     *
     * This method is invoked by the IndependetThreadCallback in a loop if the map execution mode is set to IndependentThread, in which case a separate
     * thread is spawned and the map is exchanged with the IOM in a loop, making the map data retrieve/push rate independent from the main MARTe thread 
     * execution and solving the FIFO overflow error in when long loop periods are expected for the main MARTe thread loop.
     * @param [out] outputCode code notifying the status of the map exchange process.
     * @return true if the exchange process was performed successfully, false otherwise.
     */
    bool ExchangeMap(MapReturnCode& outputCode);

    /**
     * @brief Private method to enable the map operation.
     * @details This method must be called before any map refreshing operation to enable the map structure in low-level
     * operation. This method performs all the map starting and configuration procedures prior to map usage.
     * @return true if the the map enabling sequence succeeded, false otherwise.
     */
    bool EnableMap();

    /**
     * @brief Private method to reset the map during operation.
     * @details This method performs a shutdown of the map in the low-level API and stops any data acquisition processes in such map.
     * Additionally the method performs a call to EnableMap() method to re-enable the map.
     * @return true if the map reset procedure succeeded, false otherwise.
     */
    bool ResetVMap();

    /**
    *   Variable holding the number of samples required to be delivered by the datasource for each channel.
    */
    uint32 nReadSamples;

    /**
    *   Variable holding the number of samples required to be delivered by the datasource for each channel.
    */
    uint32 nWriteSamples;

    /**
    *   Variable holding the length of the circular buffer for each hardware layer in terms of samples for each channel.
    */
    uint32 nBuffers;

    /**
    *   VMap specific single thread interface.
    */
    SingleThreadService executor;
};
}
#endif /* UEIRtVMap_H_ */