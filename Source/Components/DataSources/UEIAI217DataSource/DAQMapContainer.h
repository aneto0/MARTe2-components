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
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include "PDNA.h"
#include "UEIDefinitions.h"
#include "UEIAI217_803.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO
 * Modes:
 * * Point-to-Point
 * * RtDMap
 * * RtVMap
 * * ADMap
 * * AVMap
 * @details 
 *
 * <pre>
 *    +Map1 = {
 *        Class           = DAQMapContainer
          Type            = "RtDMap"            
          ScanRate        = 1.0    
          Inputs = {
            Devices = {                
              dev0 = {
                  Devn        = 0
                  Channels    = {0, 1, 2}
              }
              dev1 = {
                  Devn        = 1
                  Channels    = {0, 1, 2}   
              }
            }
          }
          Outputs = {
            Devices = {                
              dev0 = {
                  Devn        = 0
                  Channels    = {5, 6}
              }
              dev1 = {
                  Devn        = 1
                  Channels    = {7, 8}   
              }
            }
          }
       }
 */

typedef struct{
    bool defined;
    uint32* channels;
    uint32 nChannels;
}IOMapMember;

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
     * @details This function must be called by DAQMasterObject upon before
     * of the IOM handle to ensure clean closing of the DAQ Map.
     * @param[in] DAQ_handle handler of the IOM to which the map belongs.
     * @return true if the map has been closed correctly and cleanly.
     */
    bool CleanupMap(int32 DAQ_handle);
    
    /**
     * @brief Getter for the device number of a member of this Map Container.
     * @param[in] MemberIdx the id of the member of which the devn is to be retrieved.
     * @return devn for the device at the selected member of this Map Container.
     */    
    bool GetDevDefined(uint32 devn);

    /**
     * @brief Getter for the device number of a member of this Map Container.
     * @param[in] MemberIdx the id of the member of which the devn is to be retrieved.
     * @return devn for the device at the selected member of this Map Container.
     */    
    bool GetDevDefined(uint32 devn, uint8 direction);

    bool GetNumberOfChannels(uint32 devn, uint8 direction, uint32* nChannels);

    bool GetChannelOfMember(uint32 devn, uint8 direction, uint32 channelIdx, uint32* channelNumber);

    bool SetDevReference(uint32 devn, ReferenceT<UEIAI217_803> reference); //TODO

    bool StartMap(int32 DAQ_handle_);

    char8* GetName();
    
    float GetScanRate();

private:

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
    *   Variable holding the number of devices configured for this map. The number 
    *   of configured devices for this map must be equal to the number of members
    *   of the map.
    */
    uint32 nDevices;
    
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
};
}
#endif /* DAQMapContainer_H_ */