/**
 * @file UEIMasterObject.h
 * @brief Header file for class UEIMasterObject
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

 * @details This header file contains the declaration of the class UEIMasterObject
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIMasterObject_H_
#define UEIMasterObject_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
//MARTe includes
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "ReferenceContainer.h"
#include "StreamString.h"
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"

//Interface specific includes
#include "UEIDevice.h"
#include "UEIMapContainer.h"
#include "UEIDefinitions.h"
#include "UEIAI217_803.h"

//PowerDNA includes
#include "PDNA.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

#define DEFAULT_IOM_CONNECTION_TIMEOUT 200u

/**
 * @brief Class responsible to represent and manage a UEIDAQ device (Whole device, cpu + hardware layers installed)
 * @details This class implements the needed methods to manage and communicate with a UEIDAQ device. The class
 * allows operation on a local device (MARTe running in the UEIDAQ CPU) or as a remote device via UDP packets.
 * 
 * This class is the base of the interface to a UEIDAQ device. Configuration on this class must include the IP of the 
 * UEIDAQ device being configured (use 127.0.0.1 internal loopback if MARTe is running on UEIDAQ CPU) and the port in the 
 * UEIDAQ used to stream data via UDP packets (ignored for local devices).
 * 
 * The object configuration must also include the configured hardware layers for this device as the specific device objects
 * derived from UEIDevice class. Data is retrieved from the UEIDAQ device via DAQ Maps, which must be configured as Map-specific objects derived
 * from UEIMapContainer (see UEIRtDMap and UEIRtVMap).
 *
 *
 * +UEIDAQ1 = {
 *     Class    = UEIMasterObject
 *     Ip       = {127, 0, 0, 1}
 *     Port     =  6334  
 *     +Devices = {
 *       Class=ReferenceContainer
 *       +dev0={
 *          Class               = UEIAI217_803
 *          Devn                = 0
 *          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
 *          Sampling_ferquency  = 1000.0
 *       }
 *       +dev1={
 *          Class               = UEIAI217_803
 *          Devn                = 1
 *          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
 *          Sampling_ferquency  = 1000.0
 *       }
 *    } 
 *    +Maps = {
 *     Class=ReferenceContainer
 *     +Map1 = {
 *          Class           = UEIRtVMap
 *          ExecutionMode   = RealTimeThread
 *          Samples 	    = 128
 *	        BufferSamples   = 1000
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
 *   }
 * }
 */
class UEIMasterObject : public ReferenceContainer {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIMasterObject();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIMasterObject();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

protected:
    
    /**
    *   Variable holding the DAQ IOM reference
    */
    int32 DAQ_handle;

    /**
    *   Variable holding the number of defined devices
    */
    uint8 nDevices;
    
    /**
    *   Array containing the ip of the device
    */
    uint16 ip [4];

    /**
    *   Character array containing the ip of the device for PDNA library usage
    */
    char8* ip_string;

    /**
    *   Variable to store the UDP port for remote UEIDAQ devices
    */
    uint16 port;

    /**
    *   Variable containing the name of the device
    */
    StreamString name;
    
    /**
    *   Array of references to the device objects defined for this device. In this array, the index
    *   represents the devn of the device. If a slot is empty/not recognized, the reference is NULL,
    *   to be checked with IsValid() method before usage.
    */
    ReferenceT<UEIDevice> devices [MAX_IO_SLOTS];
    
    /**
    *   Array of references to the maps declared for this UEIDAQ device.
    */
    ReferenceT<UEIMapContainer>* maps;
    
    /**
    *   Variable holding the number of maps defined for this UEIDAQ device.
    */
    uint32 nMaps;

    /**
    *   Variable holding the Timeout period for the IOM connection process (default to 200ms).
    */
    uint32 IOMTimeOut;
};
}
#endif /* UEIMasterObject_H_ */