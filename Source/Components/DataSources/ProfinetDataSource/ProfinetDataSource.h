/**
 * @file ProfinetDataSource.h
 * @brief Header file for class ProfinetDataSource
 * @date 15/01/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class ProfinetDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_PROFINETDATASOURCE_H_
#define DATASOURCES_PROFINET_PROFINETDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//lint -estring(1960, "*ProfinetDataSource*")
//lint -estring(1960, "*cc_assert*")

#include "CompilerTypes.h"
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "Endianity.h"
#include "FastPollingMutexSem.h"
#include "HighResolutionTimer.h"

//lint ++flb "Utility libraries"
#include "ICyclicNotifiable.h"
#include "ILoggerAdapter.h"
#include "IMainThreadEntryPoint.h"
#include "IOperationalSignalsEntryPoint.h"
#include "IProfinetEventNotifiable.h"
#include "ISynchronisableInput.h"
#include "ISynchronisableOutput.h"
#include "ITimerEntryPoint.h"
//lint --flb

#include "MemoryMapSynchNMutexInputBroker.h"
#include "MemoryMapSynchNMutexOutputBroker.h"
#include "MessageI.h"
#include "MultiThreadService.h" 
#include "MutexSem.h"
#include "ObjectRegistryDatabase.h"

#ifndef LINT
//lint ++flb "Internals"
#include "ProfinetDataSourceAdapter.h"
#include "ProfinetEventType.h"
//lint --flb
#endif

#include "ProfinetMainThreadHelper.h"
#include "ProfinetTimerHelper.h"

//lint ++flb "Internals"
#include "ProfinetToMARTeLogAdapter.h"
//lint --flb

#ifdef LINT
//lint ++flb "Internals"
#include "ProfinetDataSourceAdapter_lint.h"
//lint --flb
#endif

#include "StreamString.h"
/**
 * @brief Flag for base network card parameter presence 
 */
#define PNETDS_MASK_BASENIC                     static_cast<MARTe::uint32>(1u)
/**
 * @brief Flag for station name parameter presence
 */
#define PNETDS_MASK_BASESTATIONNAME             static_cast<MARTe::uint32>(2u)
/**
 * @brief Flag for periodic interval parameter presence
 */
#define PNETDS_MASK_BASEPERIODICINTERVAL        static_cast<MARTe::uint32>(4u)
/**
 * @brief Flag for reduction ratio parameter presence
 */
#define PNETDS_MASK_BASEREDUCTIONRATIO          static_cast<MARTe::uint32>(8u)
/**
 * @brief Cumulative flag for mandatory parameters among base group
 */
#define PNETDS_MASK_BASE_MANDATORIES            (PNETDS_MASK_BASENIC | \
                                                PNETDS_MASK_BASESTATIONNAME | \
                                                PNETDS_MASK_BASEPERIODICINTERVAL | \
                                                PNETDS_MASK_BASEREDUCTIONRATIO)


/**
 * @brief Flag for vendor identifier parameter presence
 */
#define PNETDS_MASK_BASEIDVENDORID                      static_cast<MARTe::uint32>(1u)
/**
 * @brief Flag for device identifier parameter presence
 */
#define PNETDS_MASK_BASEIDDEVICEID                      static_cast<MARTe::uint32>(2u)
/**
 * @brief Flag for oem vendor identifier parameter presence
 */
#define PNETDS_MASK_BASEIDOEMVENDORID                   static_cast<MARTe::uint32>(4u)
/**
 * @brief Flag for oem device identifier parameter presence
 */
#define PNETDS_MASK_BASEIDOEMDEVICEID                   static_cast<MARTe::uint32>(8u)
/**
 * @brief Cumulative flag for mandatory parameters among base identification group
 */
#define PNETDS_MASK_BASEID_MANDATORIES                  (PNETDS_MASK_BASEIDVENDORID | \
                                                        PNETDS_MASK_BASEIDDEVICEID | \
                                                        PNETDS_MASK_BASEIDOEMVENDORID | \
                                                        PNETDS_MASK_BASEIDOEMDEVICEID)

/**
 * @brief Flag for I&M vendor parameter presence
 */
#define PNETDS_MASK_IMVENDOR                    static_cast<MARTe::uint32>(1u)
/**
 * @brief Flag for I&M hardware revision parameter presence
 */
#define PNETDS_MASK_IMHARDWAREREVISION          static_cast<MARTe::uint32>(2u)
/**
 * @brief Flag for I&M software revision parameter presence
 */
#define PNETDS_MASK_IMSOFTWAREREVISION          static_cast<MARTe::uint32>(4u)
/**
 * @brief Flag for I&M functional enhancement parameter presence
 */
#define PNETDS_MASK_IMFUNCTIONALENHANCEMENT     static_cast<MARTe::uint32>(8u)
/**
 * @brief Flag for I&M bugfix parameter presence
 */
#define PNETDS_MASK_IMBUGFIX                    static_cast<MARTe::uint32>(16u)
/**
 * @brief Flag for I&M internal change parameter presence
 */
#define PNETDS_MASK_IMINTERNALCHANGE            static_cast<MARTe::uint32>(32u)
/**
 * @brief Flag for I&M profile identifier parameter presence
 */
#define PNETDS_MASK_IMPROFILEIDENTIFIER         static_cast<MARTe::uint32>(64u)
/**
 * @brief Flag for I&M profile specific type parameter presence
 */
#define PNETDS_MASK_IMPROFILESPECIFICTYPE       static_cast<MARTe::uint32>(128u)
/**
 * @brief Flag for I&M version major number parameter presence
 */
#define PNETDS_MASK_IMVERSIONMAJOR              static_cast<MARTe::uint32>(256u)
/**
 * @brief Flag for I&M version minor number parameter presence
 */
#define PNETDS_MASK_IMVERSIONMINOR              static_cast<MARTe::uint32>(512u)
/**
 * @brief Flag for I&M order identifier parameter presence
 */
#define PNETDS_MASK_IMORDERID                   static_cast<MARTe::uint32>(1024u)
/**
 * @brief Flag for I&M serial number parameter presence
 */
#define PNETDS_MASK_IMSERIALNUMBER              static_cast<MARTe::uint32>(2048u)
/**
 * @brief Flag for I&M function parameter presence
 */
#define PNETDS_MASK_IMFUNCTION                  static_cast<MARTe::uint32>(4096u)
/**
 * @brief Flag for I&M location parameter presence
 */
#define PNETDS_MASK_IMLOCATION                  static_cast<MARTe::uint32>(8192u)
/**
 * @brief Flag for I&M date parameter presence
 */
#define PNETDS_MASK_IMDATE                      static_cast<MARTe::uint32>(16384u)
/**
 * @brief Flag for I&M descriptor parameter presence
 */
#define PNETDS_MASK_IMDESCRIPTOR                static_cast<MARTe::uint32>(32768u)
/**
 * @brief Flag for I&M signature parameter presence
 */
#define PNETDS_MASK_IMSIGNATURE                 static_cast<MARTe::uint32>(65536u)
/**
 * @brief Cumulative flag for I&M mandatory parameters
 */
#define PNETDS_MASK_IM_MANDATORIES      (PNETDS_MASK_IMVENDOR | \
                                        PNETDS_MASK_IMHARDWAREREVISION | \
                                        PNETDS_MASK_IMSOFTWAREREVISION | \
                                        PNETDS_MASK_IMFUNCTIONALENHANCEMENT | \
                                        PNETDS_MASK_IMBUGFIX | \
                                        PNETDS_MASK_IMINTERNALCHANGE | \
                                        PNETDS_MASK_IMPROFILEIDENTIFIER | \
                                        PNETDS_MASK_IMPROFILESPECIFICTYPE | \
                                        PNETDS_MASK_IMVERSIONMAJOR | \
                                        PNETDS_MASK_IMVERSIONMINOR | \
                                        PNETDS_MASK_IMORDERID | \
                                        PNETDS_MASK_IMSERIALNUMBER | \
                                        PNETDS_MASK_IMFUNCTION | \
                                        PNETDS_MASK_IMLOCATION | \
                                        PNETDS_MASK_IMDATE | \
                                        PNETDS_MASK_IMDESCRIPTOR | \
                                        PNETDS_MASK_IMSIGNATURE)

/**
 * @brief Flag for LLDP Port identifier parameter presence (no longer used)
 */
#define PNETDS_MASK_LLDP_PORTID                 static_cast<MARTe::uint32>(1u)
/**
 * @brief Flag for LLDP RT Class 2 status parameter presence
 */
#define PNETDS_MASK_LLDP_RTCLASS2STATUS         static_cast<MARTe::uint32>(2u)
/**
 * @brief Flag for RT Class 3 status parameter presence
 */
#define PNETDS_MASK_LLDP_RTCLASS3STATUS         static_cast<MARTe::uint32>(4u)
/**
 * @brief Flag for LLDP AutoNegotiation parameter presence (no longer used, p-net autodiscovery)
 */
#define PNETDS_MASK_LLDP_AUTONEGENABLED         static_cast<MARTe::uint32>(8u)
/**
 * @brief Flag for LLDP AutoNegotiation support parameterpresence (no longer used, p-net autodiscovery)
 */
#define PNETDS_MASK_LLDP_AUTONEGSUPPORTED       static_cast<MARTe::uint32>(16u)
/**
 * @brief Flag for LLDP AutoNegotiation speed parameter presence (no longer used, p-net autodiscovery)
 */
#define PNETDS_MASK_LLDP_AUTONEGSPEED           static_cast<MARTe::uint32>(32u)
/**
 * @brief Flag for LLDP MAU parameter presence (no longer used, p-net autodiscovery)
 */
#define PNETDS_MASK_LLDP_MAUTYPE                static_cast<MARTe::uint32>(64u)
/**
 * @brief Flag for subslot number parameter presence
 */
#define PNETDS_MASK_SUBSLOT_NUMBER              static_cast<MARTe::uint32>(1u)

/**
 * @brief Flag for Device Access Point parameter presence
 */
#define PNETDS_MASK_SUBSLOT_ISDAP               static_cast<MARTe::uint32>(2u)
/**
 * @brief Flag for Subslot expected direction parameter presence
 */
#define PNETDS_MASK_SUBSLOT_EXPDIR              static_cast<MARTe::uint32>(4u)
/**
 * @brief Flag for Subslot expected input bank size parameter presence
 */
#define PNETDS_MASK_SUBSLOT_EXPINSIZE           static_cast<MARTe::uint32>(4u)
/**
 * @brief Flag for Subslot expected output bank size parameter presence
 */
#define PNETDS_MASK_SUBSLOT_EXPOUTSIZE          static_cast<MARTe::uint32>(8u)
/**
 * @brief Cumulative flag for mandatory subslot parameters
 */
#define PNETDS_MASK_SUBSLOT_MANDATORIES         (PNETDS_MASK_SUBSLOT_NUMBER | \
                                                PNETDS_MASK_SUBSLOT_ISDAP | \
                                                PNETDS_MASK_SUBSLOT_EXPDIR | \
                                                PNETDS_MASK_SUBSLOT_EXPINSIZE | \
                                                PNETDS_MASK_SUBSLOT_EXPOUTSIZE)

/**
 * @brief Flag for maximum MARTe2 acceptable signal size 
 */
//lint -e1923 Macro is only used once, to avoid magic numbers around the profinet_marte_signal_t structure
#define PNETDS_MAXIMUM_MARTE2_SIGNALNAME_SIZE   256
//lint +e1923

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

        /**
         * @brief Signal data holder structure.
         */
        typedef struct profinet_marte_signal {

                /**
         	* @brief Direction of the signal, from the Profinet slave point of view
         	*/
                uint8           direction;

        	/**
         	* @brief Pointer to the first byte of the signal
         	*/
                uint8           *firstByteOfSignal;

        	/**
         	* @brief Size in byte of the whole MARTe2 signal
         	*/
                uint32          signalSize;

        	/**
         	* @brief Name of the signal in MARTe2
         	*/
                char8           marteName[PNETDS_MAXIMUM_MARTE2_SIGNALNAME_SIZE];

        	/**
         	* @brief Index of the signal in MARTe2
         	*/
                uint32          marteIndex;

        	/**
         	* @brief Indicates whether the signal needs endianness correction
         	*/
                bool            needsSwapping;
        }profinet_marte_signal_t;

        /**
         * @brief The ProfinetDataSource works as a Profinet slave in a Profinet bus. This DataSource appears in the Profinet bus
         * as a real Profinet slave, by reflecting inputs and outputs to/from Profinet/MARTe.
         * @details The ProfinetDataSource is an input/output DataSource:
         * <ul>
         *      <li>DataSource inputs become the Profinet slave inputs, becoming master inputs consequentially (PLC Inputs)</li>
         *      <li>Profinet slave output become the DataSource outputs, becoming master outputs consequentially (PLC Outputs)</li>
         * <ul>
         * 
         * ProfinetDataSource structure
         * At a glance, the ProfinetDataSource is structured in 3 layers:
         * <ul>
         *      <li>P-NET library</li>
         *      <li>ProfinetDataSourceAdapter</li>
         *      <li>ProfinetDataSource</li>
         * </ul>
         * 
         * The P-NET library [https://rt-labs.com/docs/p-net/index.html]
         * The P-NET library is a C library which implements the Profinet stack, which specified features and under
         * certain limitations. The P-NET library needs also OSAL, which in this specific case is kept for the networking
         * configuration. 
         * NOTE: in the future, the OSAL dependency may be reduced, removed or reworked to suit MARTe2.
         * 
         * The ProfinetDataSourceAdapter
         * The Profinet DataSource Adapter is an adapter (not in strict GoF pattern definition) which
         * adapts the pure C library and I/O to MARTe2 suitable primitives. The adapter is a wrapper, in first instance,
         * where all the functions and callbacks are mapped and an internal state is kept. Moreover, the adapter as a subset
         * of methods to easily map the process image. * The adapter is designed to run independently from the datasource, although it is designed to fit into it.
         * 
         * The ProfinetDataSource
         * The DataSource is reduced to 2 simple memory segment (1 for the inputs, 1 for the outputs) in which the following
         * happens:
         * <ul>
         *      <li>Each segment (input/output) is split into two halfs, one shared with the pnetdsadapter, the other exclusive property of MARTe2</li>
         *      <li>Every synchronisation checkpoint the MARTe input part is copied inside the Profinet input part</li>
         *      <li>Every synchronisation checkpoint the Profinet output part is copied inside the Profinet output part</li>
         * </ul>
         * Mechanisms of callback, implemented in form of interfaces from the DataSource, are exposed to specific portions
         * of the adapter, in order to propagate events from the library into the DataSource.
         *
         * As the library, the adapter and the DataSource are mostly event-driven, all the relevant events are transported
         * to and from the DataSource. Moreover, as a main source of events (the timer) is needed, timing facility is offered
         * using the EmbeddedServiceMethodBinderT from MARTe2.
         * 
         * The Helpers (ProfinetMainThreadHelper and ProfinetTimerHelper)
         * For the ProfinetDataSource correct operations, two running services are needed.
         * One is the timer, which ticks at a precise frequency generating the "Timer" event, the other is the Main, which
         * monitors the "Event flag" containing all the possible Profinet occurring events.
         * The Main thread stands still, waiting for an event to occur. Once an event has occurred, the right handler is executed.
         * The vital event for the Profinet slave is the Timer event, which happens at a specific frequency, matching with the
         * master specified (expected).
         * The Timer event is generated inside the Timer helper and handled inside the MainThread.
         * While no specific logic resides inside the helpers (excluding the trivial relevant logic), the real handling
         * is always done at adapter level.
         * 
         * The brokers
         * ProfinetDataSource uses custom brokers, derived from the MemoryMap[Input,Output]Broker. They are similar
         * to the MemoryMapSynchronised[Input,Output]Broker version with a difference in the Synchronise/Terminate phase.
         * As concurrent access happens at DataSource level, on the shared heap devoted to process image, the DataSource
         * initiates the copying (and the locking) inside the Synchronise call and terminates it signaling with the ù
         * Terminate[Input,Output]Copy (unlock).
         * Terminate[Input,Output]Copy methods are mildly abused in terms of MARTe intended usage, 
         * as they are called ignoring their calling parameters.
         * 
         * Ancillary types
         * A series of ancillary types are used across the whole implementation. Every added feature to the DataSource
         * is presented in form of interface, to avoid over-coupling and promote tidyness. Coupled ancillaries use the
         * portion of interface of interest, instead of seeing the whole DataSource.
         * Aside from interfaces, a profinet_marte_signal_t is defined, as an accelerator structure where all the signals
         * are conveniently stored together with Profinet specific and MARTe handy information. 
         * A double linked-list is also presented, with included forward and reverse iterator, to navigate signal structures.
         * 
         * Memory representation of signals
         * Profinet devices are composed of Slots and Subslots. Each slot can contain 1:N subslots.
         * A slot can host a module while a subslot a submodule. A submodule for the P-NET library is essentially
         * a linear contiguous segment in which the whole signal block is represented, no matter the types and their
         * representation on the master side. As this representation well suits MARTe2 DataSource, the same structure is
         * kept, by making two adjacent buffers where the adapter and MARTe2 can exchange data by simply copying them.
         * Moreover, as the Profinet representation is Big-Endian, if the signal has markings which specify the need of byte swapping
         * (Endianess adaptation) the signal is reversed using the MARTe2 [From,To]BigEndian in order to ensure portability
         * coherence.
         * 
         * Configuration file
         * Following reported a full configuration file with the notes on how to configure each element on the Profinet slave
         * 
         * +DDB1 = {
         *       //Mandatory, the ProfinetDataSource class 
         *       Class = ProfinetDataSource
         *
         *       //Networking interface reserved for Profinet communication
         *       NetworkInterface = "enp0s9"	    
         * 
         *       //Station name, which must match the controller GSDML specified one
         *       StationName = "rt-labs-dev"
         * 
         *       //Time expressed in microseconds, for the Cyclic timer
         *       PeriodicInterval = 10000
         *       
         *       //Number of periodic intervals must pass before updating the Cyclic data
         *       ReductionRatio = 100
         * 
         *       //Vendor identification and manufacturer data, which must match the controller GSDML specified ones
         *       VendorIdentifier = 0xFEED
         *       DeviceIdentifier = 0xBEEF
         *       OEMVendorIdentifier = 0xC0FF
         *       OEMDeviceIdentifier = 0xEE01
         *       DeviceVendor = "rt-labs"
         *       ManufacturerSpecificString = "PNET demo"
         * 
         *       //I&M identification and maintainance data, which must match the controller GSDML specified ones
         *       IMVendor = 0xFEED
         *       IMHardwareRevision = 1
         *       IMSoftwareRevision = 'V'
         *       IMFunctionalEnhancement = 1
         *       IMBugFix = 0
         *       IMInternalChange = 0
         *       IMProfileIdentifier = 0x1234
         *       IMProfileSpecificType = 0x5678
         *       IMVersionMajor = 0
         *       IMVersionMinor = 0
         *       IMFunction = ""
         *       IMLocation = ""
         *       IMDate = ""
         *       IMDescriptor = ""
         *       IMSignature = ""
         * 
         *       //LLDP and extended device configuration, which must match the controller GSDML specified one
         *       LLDPPortIdentifier = "port-001"
         *       RTClass2Status = 0
         *       RTClass3Status = 0
         *       AutoNegotiationCapability = 0x03
         *       AutoNegotiationSpeed = 0x0010
         *       MAUType = 0x00
         * 
         *       //Mandatory: the Main thread helper, which is implemented using MARTe2 binder. Here the Timeout
         *       //and CPU Mask for the thread can be specified
         *       +MainThreadHelper = {
         *               Class = ProfinetMainThreadHelper
         *               Timeout = 1000
         *               CPUMask = 0xFF
         *       }
         *       //Mandatory: the Timer thread helper, which is implemented using MARTe2 binder. Here the Timeout
         *       //and CPU Mask for the thread can be specified
         *       +TimerHelper = {
         *               Class = ProfinetTimerHelper
         *               Timeout = 1000
         *               CPUMask = 0xFF
         *       }
         *       //Mandatory section for the Slots / Subslots configuration
         *       //Must match the GSDML configuration 1:1 (e.g. no slots can be left unplugged)
         *       //The structure is a 2-level hierarchy where the N slots are siblings and contain
         *       //related M subslots.
         *       //The Device Access Point (DAP) is a Profinet characteristic slot which is always present in the
         *       //Profinet devices. It is also mandatory and is usually located at Slot 0. 
         *       //DAP Slots/Subslots can have 0/0 Expected[Input,Output]Size.
         *       //ExpectedDataDirection allowed values are
         *       // 0 - No Input Output
         *       // 1 - Input Only
         *       // 2 - Output Only
         *       // 3 - Combined Input/Output
         *       //Depending on ExpectedDataDirection, the corresponding Expected[Input,Output] size must be configured.
         *       //NOTE: Relevant thing is that the couple SlotNumber / SubslotNumber matches GSDML configuration.
         *       //FAQ:         Q: Why the properties are called with the "expected" prefix?
         *                      A: Because they are matched with master advertised before plugging them into modules.
         *                      Q: What happens if expected size does not match master specified size?
         *                      A: No plugging can happen, the slot is not filled and IOXS remains BAD. Master sees a faulty slave.
         *                      Q: What does a InputOutput type do?
         *                      A: It represents a slot which is hybrid and has portions of input and portions of output.
         *                      Q: What happens if I mess with DeviceAccessPoint property?
         *                      A: The master expects a configuration which is not matched in the slave. No plugging happens and the slave will not start.
         *                      Q: What happens if I mess with Expected sizes?
         *                      A: The master expects a configuration which is not matched in the slave.
         *                      Q: What happens if I mess with DataDirection?
         *                      A: See above.
         *                      Q: Are there limitation on Slot / Subslot names? I see a naming pattern in the example.
         *                      A: As long as the SlotNumber and SubslotNumber are correct, the naming is fully customizable following MARTe2 rules.
         *                      Q: Do I have to invent Slot / Subslot numbers? Do they have to be contiguous?
         *                      A: No, they must match their counterpart in the GSDML file.
         *       Slots = {
         *               Slot0 = {
         *                       SlotNumber = 0
         *                       Subslots = {
         *                               Subslot1 = {
         *                               SubslotNumber = 1
         *                               DeviceAccessPoint = 1
         *                               ExpectedDataDirection = 1
         *                               ExpectedInputSize = 0
         *                               ExpectedOutputSize = 0
         *                       }
         *                       Subslot32768 = {
         *                               SubslotNumber = 32768
         *                               DeviceAccessPoint = 1
         *                               ExpectedDataDirection = 1
         *                               ExpectedInputSize = 0
         *                               ExpectedOutputSize = 0
         *                       }
         *                       Subslot32769 = {
         *                               SubslotNumber = 32769
         *                               DeviceAccessPoint = 1
         *                               ExpectedDataDirection = 1
         *                               ExpectedInputSize = 0
         *                               ExpectedOutputSize = 0	
         *                       }
         *                       }
         *               }
         *               Slot1 = {
         *                       SlotNumber = 1
         *                       Subslots = {
         *                               Subslot1 = {
         *                                       SubslotNumber = 1
         *                                       DeviceAccessPoint = 0
         *                                       ExpectedDataDirection = 2
         *                                       ExpectedInputSize = 0
         *                                       ExpectedOutputSize = 13
         *                               }
         *                       }
         *               }
         *               Slot2 = {
         *                       SlotNumber = 2
         *                       Subslots = {
         *                               Subslot1 = {
         *                                       SubslotNumber = 1
         *                                       DeviceAccessPoint = 0
         *                                       ExpectedDataDirection = 1
         *                                       ExpectedInputSize = 269
         *                                       ExpectedOutputSize = 0
         *                               }
         *                       }
         *               }
         *       }
         *       //The relevant signal section, which has some extended attributes over MARTe2 common / base.
         *       //Knowing that Profinet sees only a chunk of memory, the "signal" interpretation is up to the user.
         *       //This means that the user *must* know where the signal are located inside the chunk.
         *       //Profinet signal coordinates are essentially 3 (+ 1 Direction)
         *       //Slot and Subslot numbers, which represent the module/submodule of the slave.
         *       //The Offset is the byte offset counting from the slot / subslot start, where the signal is located.
         *       //The direction is the direction of the signal (0 Input - 1 Output).
         *       //The NeedsSwapping (optional, default to 0 = false) attribute specifies if the [Big-To-Little, Little-To-Big] endianess correction should be done.
         *       //NOTE: Two special *optional* signals ProfinetLED and ProfinetDeviceReady are given in order to:
         *               - Implement the mandatory Profinet signalling LED (which can be also stimulated from the master)
         *               - Bring to downstream MARTe2 GAMs the knowledge about the Profinet slave status, which is 1 if correctly
         *                 connected and updating the master, 0 otherwise.
         *       //NOTE: A knowledge of the slave and of its corresponding GSDML clarifies the next section, considering that
         *               Profinet sees only a chunk of memory and signals are a particular way of seeing data.
         *       //FAQ: Q: What happens if I mess with Slot / Subslot / Offset / Direction values?
         *              A: If you are unlucky, a wrong value is represented using a random portion of the I/O heap.
         *                 If you are lucky, instead, an error will prevent the DataSource execution.
         *              Q: How do I know which Endianess has my system and if I have to apply swapping?
         *              A: If you apply swapping and your system architecture has the same representation of Profinet,
         *                 the Endianess swap just does nothing.
         *              Q: What happens if I ask for endianess correction on a non-standard size?
         *              A: Endianess correction happens only for 2 / 4 / 8 bytes wide data (type is irrelevant).
         *                 Blobs are not endian corrected (e.g. you must know how to handle them),
         *              Q: Why do I need the LED?
         *              A: It may come handy, it brings some sort of information from the master and can be conveniently used.
         *              Q: Why do I need the ProfinetReady signal?
         *              A: If the synch with the master is lost (e.g. disconnection, problems, ...) the last value assumed by 
         *                 signals is kept. This may lead to wrong / erratic behaviour on the downstream GAMs.
         *                 In order to prevent this, an indication about the "goodness" of the data can be extracted from the "ready" signal,
         *                 which is a boolean (0 not ready = bad or stale data, 1 ready = good data).
         *       Signals = {
         *               OutputBank = {
         *                       Type = uint8
         *                       NumberOfElements = 1
         *                       NumberOfDimensions = 0
         *                       Slot = 1
         *                       Subslot = 1
         *                       Offset = 0
         *                       Direction = 1
         *               }
         *               InputInt32A = {
         *                       Type = int32
         *                       NumberOfElements = 1
         *                       NumberOfDimensions = 0
         *                       Slot = 2
         *                       Subslot = 1
         *                       Offset = 5
         *                       Direction = 0
         *                       NeedsSwapping = 1
         *               }
         *               ProfinetDeviceLed = {
         *                       Type = uint8
         *                       NumberOfElements = 1
         *                       NumberOfDimensions = 0
         *               }
         *               ProfinetDeviceReady = {
         *                       Type = uint8
         *                       NumberOfElements = 1
         *                       NumberOfDimensions = 0
         *               }
         *       }
         * }
         * 
         */
        class ProfinetDataSource: 
                public DataSourceI, 
                public ProfinetDataSourceDriver::ICyclicNotifiable,
                public ITimerEntryPoint,
                public IMainThreadEntryPoint,
                public IProfinetEventNotifiable,
                public ISynchronisableInput,
                public ISynchronisableOutput,
                public IOperationalSignalsEntryPoint {

        public:
            CLASS_REGISTER_DECLARATION()

            /**
             * @brief Constructor for the DataSource.
             */
            ProfinetDataSource();

            /**
             * @brief Destructor for the DataSource.
             */
            virtual ~ProfinetDataSource();

            /**
             * @brief Do not use, this DataSource uses split synchronization method for Inputs and Outputs.
             * @details Do not use.
             * @return Always false
             */
            virtual bool Synchronise();

            //ErrorManagement::ErrorType ThreadCallback(ExecutionInfo &info);

            /**
             * @brief Reads, checks and initialises the DataSource parameters
             * @details Load from a configuration file the DataSource parameters.
             * If no errors occurs the following operations are performed:
             * The Slot / Subslot configuration is read and an internal representation is built upon.
             * Memory is also allocated at this stage.
             * @param[in] data is the configuration file.
             * @return true if all parameters can be read and the values are valid
             */
            virtual bool Initialise(StructuredDataI & data);

            /**
             * @brief Read, checks and initialises the Signals
             * @details If no errors occurs the following operations are performed:
             * Fills the internal Profinet/MARTe2 signal accelerator structure, where MARTe and Profinet data are stored.
             * @param[in] data is the configuration file.
             * @return true if all parameters can be read and the values are valid
             */
            virtual bool SetConfiguredDatabase(StructuredDataI & data);

            /**
             * @brief Does nothing as the DataSource only supports one state
             * @return true
             */
            virtual bool PrepareNextState(const char8 * const currentStateName,
                    const char8 * const nextStateName);

            /**
             * @brief Does nothing, as memory was previously allocated when Slot / Subslot configuration is acquired
             * @return true
             */
            virtual bool AllocateMemory();

            /**
             * @brief Returns always one, as the DataSource uses an internal double buffering mechanism which is not exposed to MARTe2
             * @return 1u
             */
            virtual uint32 GetNumberOfMemoryBuffers();

            /**
             * @brief Gets the signal memory buffer for the specified signal.
             * @param[in] signalIdx indicates the index of the signal to be obtained.
             * @param[in] bufferIdx indicate the index of the buffer to be obtained. Since only one buffer is allowed this parameter is always 0
             * @param[out] signalAddress is where the address of the desired signal is copied.
             */
            virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                    const uint32 bufferIdx,
                    void *&signalAddress);

            /**
             * @brief See DataSourceI::GetBrokerName.
             * @details 
             * @return
             */
            virtual const char8 *GetBrokerName(StructuredDataI &data,
                    const SignalDirection direction);

            /**
             * @brief See DataSourceI::GetInputBrokers.
             * @details Returns always MemoryMapSynchNMutexInputBroker
             */
            virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                    const char8* const functionName,
                    void * const gamMemPtr);

            /**
             * @brief See DataSourceI::GetOutputBrokers.
             * @return Returns always MemoryMapSynchNMutexOutputBroker.
             */
            virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                    const char8* const functionName,
                    void * const gamMemPtr);

            /**
             * @brief Triggers the Input Synchronisation routine, by locking the relevant heap block (I/O)
             * @return  True if the locking is successful, false otherwise (true, signals updated, false stale signals).
             *          Stale approach is done to avoid impairing MARTe2 RT performances.
             */
            virtual bool SynchroniseInput();

            /**
             * @brief Triggers the Output Synchronisation routine, by locking the relevant heap block (I/O)
             * @return  True if the locking is successful, false otherwise (true, signals updated, false stale signals).
             *          Stale approach is done to avoid impairing MARTe2 RT performances.
             */
            virtual bool SynchroniseOutput();

            /**
             * @brief Signals the ending of the input copy, from the broker, in order to release concurrent locks.
             * @return  true
             */
            virtual bool TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples);

            /**
             * @brief Signals the ending of the output copy, from the broker, in order to release concurrent locks.
             * @return  true
             */
            virtual bool TerminateOutputCopy (const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples);

            /**
             * @brief Notification receiver, indicating data memory bank update
             */
            virtual void NotifyCycle();

            /**
             * @brief Entry point implementation for the main thread.
             */
            virtual uint16 MainThread(const uint16 inputFlag);

            /**
             * @brief Entry point implementation for the timer ticks.
             */
            virtual void TimerTick();

            /**
             * @brief Entry point implementation for the event notification.
             */
            virtual void NotifyEvent(const ProfinetDataSourceEventType eventType);

            /**
             * @brief Entry point implementation for the LED signal event.
             */
            virtual void SetLED(const bool ledStatus);

            /**
             * @brief Entry point implementation for the ready signal event.
             */
            virtual void SetReady(const bool readyStatus);

            /**
             * @brief Entry point for the abort indication
             */
            virtual void Abort();

        private:
                /**
                 * @brief       The Profinet driver adapter. Handles all the underlying management of the Profinet protocol, bringing
                 *              high-level MARTe2-friendly in/out interface.
                 */
                ProfinetDataSourceDriver::ProfinetDataSourceAdapter *adapter;              

		/**
 		* @brief	The logger adapter, which is used internally, in the adapter, to redirect console outputs to MARTe2 REPORT_ERROR facility
 		*
 		*/	 
		ProfinetDataSourceDriver::ILoggerAdapter *loggerAdapter;

                /**
                * @brief        Safely tries to close and shutdown underlying layer facilities
                */
                void SafeShutDown();

                /**
                 * @brief       The name of the network interface which will be controlled by the Profinet driver.
                 *              Ensure that MARTe2 has permissions to change IP Address and put up/down the interface.
                 *              Should match OS naming convention (e.g. eth0 under *nix OSes) 
                 */
                StreamString networkInterface;


                /**
                 * @brief       Profinet station name attribute. 
                 */
                StreamString stationName;

                /**
                 * @brief       Profinet cycle interval expressed in microseconds. Determines the rate of exchange with the master
                 *              node. An underlying timer manages the scan cycle time, duration and overlapping.
                 */
                uint32 periodicIntervalus;

                /**
                 * @brief       Specifies how many cycles must pass before the driver sends to the master the cyclic data, as function
                 *              of the periodic interval. 
                 */
                uint32 reductionRatio;

                /**
                 * @brief       16 bit Profinet Vendor IDentifier (0xHHLL)
                 */
                uint16 vendorIdentifier;

                /**
                 * @brief       16 bit Profinet Devide IDentifier (0xHHLL)
                 */
                uint16 deviceIdentifier;

                /**
                 * @brief       16 bit Profinet OEM Vendor IDentifier (0xHHLL)
                 */
               uint16 oemVendorIdentifier;

               /**
                *  @brief       16 bit Profinet OEM Device IDentifier (0xHHLL)
                */
               uint16 oemDeviceIdentifier;

                /**
                 * @brief       Profinet Device Vendor String
                 */
               StreamString deviceVendor;

                /**
                 * @brief       Profinet Manufacturer Specific String
                 */
               StreamString manufacturerSpecificString;

                /**
                 * @brief       Identification & Maintenance Vendor
                 */
                uint16 imVendor;

                /**
                 * @brief       Identification & Maintenance Hardware Revision
                 */
                uint16 imHardwareRevision;

                /**
                 * @brief       Identification & Maintenance Software Revision
                 */
                ProfinetDataSourceDriver::softwarerevision_t imSoftwareRevision;

                /**
                 * @brief       Identification & Maintenance Functional Enhancement
                 */
                uint8 imFunctionalEnhancement;

                /**
                 * @brief       Identification & Maintenance Bug Fix
                 */
                uint8 imBugFix;

                /**
                 * @brief       Identification & Maintenance Internal Change
                 */
                uint8 imInternalChange;

                /**
                 * @brief       Identification & Maintenance Profile Identifier
                 */
                uint16 imProfileIdentifier;

                /**
                 * @brief       Identification & Maintenance Profile Specific Type
                 */
                uint16 imProfileSpecificType;

                /**
                 * @brief       Identification & Maintenance SW Revision Major Number
                 */
                uint8 imVersionMajor;

                /**
                 * @brief       Identification & Maintenance SW Revision Minor Number
                 */
                uint8 imVersionMinor;

                /**
                 * @brief       Identification & Maintenance device Order Identifier
                 */
                StreamString imOrderId;

                /**
                 * @brief       Identification & Maintenance device Serial Number
                 */
                StreamString imSerialNumber;

                /**
                 * @brief       Identification & Maintenance Function
                 */
                StreamString imFunction;

                /**
                 * @brief       Identification & Maintenance Location
                 */
                StreamString imLocation;

                /**
                 * @brief       Identification & Maintenance Date
                 */
                StreamString imDate;

                /**
                 * @brief       Identification & Maintenance Descriptor
                 */
                StreamString imDescriptor;

                /**
                 * @brief       Identification & Maintenance Signature
                 */
                StreamString imSignature;

                /**
                 * @brief       Link Layer Discovery Protocol (LLDP) Port Identifier.
                 */
                StreamString lldpPortIdentifier;

                /**
                 * @brief       Link Layer Discovery Protocol (LLDP) Class 2 Status
                 */
                uint16 rtClass2Status;

                /**
                 * @brief       Link Layer Discovery Protocol (LLDP) Class 3 Status
                 */
                uint16 rtClass3Status;

                /**
                 * @brief       Quick reference for each signal
                 */
                profinet_marte_signal_t         *signalIndexer[2];

                /**
                 * @brief       Quick reference to the signal count;
                 */
                uint32                        signalIndexerCount;

                /**
                 * @brief Heap space were all the input signals are stored
                 */
                uint8   *inputHeap;

                /**
                 * @brief Input heap half size (NOTE: half buffer is used from Profinet, the other half from MARTe)
                 */
                uint64 inputHeapHalfSize;

                /**
                 * @brief Heap space were all the output signals are stored
                 */
                uint8   *outputHeap;

                /**
                 * @brief Output heap half size (NOTE: half buffer is used from Profinet, the other half from MARTe. This indicates the half size)
                 */
                uint64 outputHeapHalfSize;

                /**
                 * @brief Protects access to the input signal copy phase
                 */
                FastPollingMutexSem inputSignalsSemaphore;

                /**
                 * @brief Protects access to the output signal copy phas
                 */
                FastPollingMutexSem outputSignalsSemaphore;

                /**
                 * @brief Timer helper reference, ticking at the Profinet scan cycle frequency in order for the protocol to evolve
                 */
                ReferenceT<ProfinetTimerHelper> timerHelper;

                /**
                 * @brief Main Thread helper reference, which is a very simple event handler for all the possible Profinet events that may occur
                 */
                ReferenceT<ProfinetMainThreadHelper> mainHelper;

                /**
                 * @brief Indicates whether the LED signal is enabled.
                 */
                bool profinetLedSignalEnabled;

                /**
                 * @brief Once enabled, keeps the position in the accelerator structure for the LED signal.
                 */
                uint32 profinetLedSignalIndex;

                /**
                 * @brief Indicates whether the Ready signal is enabled.
                 */
                bool profinetReadySignalEnabled;

                /**
                 * @brief Once enabled, keeps the position in the accelerator structure for the Ready signal.
                 */
                uint32 profinetReadySignalIndex;

    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* DATASOURCES_PROFINET_PROFINETDATASOURCE_H_ */

