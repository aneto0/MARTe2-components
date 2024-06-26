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
 * The data acquisition mode managed through this specific class can retrieve data in real-time 
 * operation where the latest sample available must be retrieved but only one sample is to be used.
 *
 * An RtDMap is configured on a specific UEIDAQ device (the one represented by the UEIMasterObject where this map is
 * instantiated) to acquire data from a set Input/Output layers defined for such device. The devices from which to
 * retrieve the data must be declared in the Devices reference container of the UEIMasterObject for the UEIDAQ device.
 * The RtDMap's devices must be declared under the Inputs or Outputs section of its configuration.
 * 
 * The DMap retrieves data from the IOM at a rate specified by the ScanRate mandatory parameter (in Hz). The hardware layers
 * do sample the inputs/outputs at the frequency configured for each one of them (in their respective UEIDevice instance), but the
 * results of such sampling is read back into MARTe at the specified ScanRate. 
 *
 * DMap only supports RealTimeThread ExecutionMode parameter, as no FIFO overflow error can occur, there's no need for separate
 * threading. Therefore this map class only supports RealTimeThread.
 *
 * Example of DMap configuration in MARTe:
 * <pre>
 *     +Map1 = {
 *          Class           = UEIRtDMap
 *          ExecutionMode   = RealTimeThread
 *          ScanRate        = 2.0
 *	        Inputs = {
 *              dev0 = {
 *                  Devn        = 0
 *                  Channels    = {0, 1}
 *              }
 *              dev1 = {
 *                  Devn        = 1
 *                  Channels    = {0, 1}
 *              }
 *          }
 *      }
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
     * @brief Method to retrieve the inputs of the map packet.
     * @details This method checks wether there's new data available to be retrieved from the DAQ map and retrieves
     * it from the devices input buffers into the MARTe signals (the ones configured in the DataSources).
     *
     * In this specific implementation, the Map is blocked up until the next sync point time arrives (blocking call) or it is
     * surpassed, then the contents of the map are retrieved and copied into the DataSource input signal memory.
     * @param[out] outputCode Variable holding the status code for the procedure performed during the call.
     * @return true no error occurred during the retrieve procedure.
     */
    bool GetInputs(MapReturnCode& outputCode);

    /**
     * @brief Method to set the outputs of the map packet.
     * @details This method pushes the output signal values from MARTe (through the DataSource) into the output buffers
     * of the devices to be later pushed into the DAQ map
     *
     * In this specific implementation, the output signals memory from the DataSource are scaled and pushed into the IOM
     * during the method call.
     * @param[out] outputCode Variable holding the status code for the procedure performed during the call.
     * @return true no error occurred during the setting procedure.
     */
    bool SetOutputs(MapReturnCode& outputCode);
    
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
    *   Variable holding the configured scan rate for this specific map.
    */
    float32 scanRate;
    
    /**
    *   Variable holding the last sync time (Linux clock time) to compute the sleep if necessary.
    *   In ns as obtainde from HighResolutionTimer::Counter() method. No need for turnarround excpetion catching
    *   as the counter overflows after ~500 years
    */
    uint64 previousSyncTime;
};
}
#endif /* UEIRtDMap_H_ */