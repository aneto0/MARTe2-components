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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#define RTDMAP          1u
#define RTVMAP          2u
#define ADMAP           3u
#define AVMAP           4u


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
*    +Maps = { 
 *      Class=ReferenceContainer
 *      + Map1 = {
 *          Class           = DAQMapContainer
 *          Type            = "RtDMap"            //Map type, currently supported : RtDMap and RtVMap
 *          ScanRate        = 1.0                 //internal Map scan frequency in Hz
 *          Devices = {                
 *              dev0 = {
 *                  Devn        = 0
 *                  Channels    = {0, 1, 2}
 *              }
 *              dev1 = {
 *                  Devn        = 1
 *                  Channels    = {0, 1, 2}   
 *              }
 *          }
 *       }
 */

 typedef struct{
    uint8 devn;
    uint32* channels;
    uint32 nChannels;
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
    bool CleanupMap(int32 DAQ_handle);
    uint32 GetNMembers();
    uint8 GetDevInMember(uint32 MemberIdx);


private:
    StreamString name;
    uint8 mapType;
    float scanRate;
    int32 mapid;
    uint32* inputMapPtr;
    uint32 nDevices;
    mapMember* members;
};
}
#endif /* DAQMapContainer_H_ */