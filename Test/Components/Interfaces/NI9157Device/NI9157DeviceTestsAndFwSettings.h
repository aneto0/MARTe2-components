/**
 * @file NI9157DeviceTestsAndFwSettings.h
 * @brief Header file for class NI9157DeviceTestsAndFwSettings.
 * @date 23/03/2021
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the paths and settings
 * used in the test firmwares of the NI9157Device MARTe2-components Interface,
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157DEVICETESTSANDFWSETTINGS_H_
#define NI9157DEVICETESTSANDFWSETTINGS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * The number of devices to be test. Must match firmware config paths.
 */
const MARTe::uint32 nDevices                = 2;

/**
 * Test for all devices (true) or until one matches the firmware config paths
 * (false).
 */
const bool testAllRetTrue                   = false;

/**
 * Number of required parameters per bitstream (ID, FILE, SIGNATURE)
 */
const MARTe::uint32 nParams                 = 3;

/**
 * Path to the forder containing the LabView bitstream files.
 */
const MARTe::char8 * const firmwarePath     = "Test/Components/Interfaces/NI9157Device/TestLvFw";

/**
 * General Purpose Firmware with U64 W/R FIFOs multiple type registers, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const multiIOFirmware[]   = {"RIO0", "NiFpga_NI9157_MultiIO.lvbitx", "F14C2F2560ABDC0BF142DDAEF1716110",
                                                "RIO1", "NiFpga_NI9159_MultiIO.lvbitx", "03AB279CA6C34216C3ABAADB90262282"};

/**
 * Firmware with BOOL W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const boolFirmware[]   = {"RIO0", "NiFpga_NI9157_BoolFifoLoop.lvbitx", "3F31F91CF1327BF3DD74027A50C336AD",
                                                "RIO1", "NiFpga_NI9159_BoolFifoLoop.lvbitx", "4142897BDB5954CCE158C05C634F4D61"};

/**
 * Firmware with U8 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const u8Firmware[]     = {"RIO0", "NiFpga_NI9157_U8FifoLoop.lvbitx", "502481EC6C37AEF24346BB8F95834660",
                                                "RIO1", "NiFpga_NI9159_U8FifoLoop.lvbitx", "E20FC0B821C53C12CDB1CF1CFFDE9E3F"};

/**
 * Firmware with I8 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const i8Firmware[]     = {"RIO0", "NiFpga_NI9157_I8FifoLoop.lvbitx", "A0473A2546DC8CBF468B5FFCB9CE9AEA",
                                                "RIO1", "NiFpga_NI9159_I8FifoLoop.lvbitx", "1D78B0D488445F8046D8AA7841CA7F92"};

/**
 * Firmware with U16 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const u16Firmware[]    = {"RIO0", "NiFpga_NI9157_U16FifoLoop.lvbitx", "AA4F4EA580F600CF09332D65C1B52E43",
                                                "RIO1", "NiFpga_NI9159_U16FifoLoop.lvbitx", "0682A8270DCB30912E3855297CA35C1A"};

/**
 * Firmware with I16 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const i16Firmware[]    = {"RIO0", "NiFpga_NI9157_I16FifoLoop.lvbitx", "378C7F29D8675EE7E2002C55A95674E3",
                                                "RIO1", "NiFpga_NI9159_I16FifoLoop.lvbitx", "B2D0A5188F4DF27E5816FB536FECA87E"};

/**
 * Firmware with U32 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const u32Firmware[]    = {"RIO0", "NiFpga_NI9157_U32FifoLoop.lvbitx", "01507C2AD9112BBD9FA982D3AAB6322C",
                                                "RIO1", "NiFpga_NI9159_U32FifoLoop.lvbitx", "E3BDC175B00D4F16FB994A1852CC695F"};

/**
 * Firmware with I32 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const i32Firmware[]    = {"RIO0", "NiFpga_NI9157_I32FifoLoop.lvbitx", "62B58E0715D8A602D7334953AF5D7417",
                                                "RIO1", "NiFpga_NI9159_I32FifoLoop.lvbitx", "3743493619F557D68357D4D088217E05"};

/**
 * Firmware with U64 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const u64Firmware[]    = {"RIO0", "NiFpga_NI9157_U64FifoLoop.lvbitx", "2909691577F1F92DCB4EB278D5773B31",
                                                "RIO1", "NiFpga_NI9159_U64FifoLoop.lvbitx", "D19D1491A3C597E9F3C0E56F06AA272C"};

/**
 * Firmware with I64 W/R FIFOs, compiled for NI9157 and NI9159.
 */
const MARTe::char8 * const i64Firmware[]    = {"RIO0", "NiFpga_NI9157_I64FifoLoop.lvbitx", "544FB3933331C70D8D4AE6B5C4A2ABFD",
                                                "RIO1", "NiFpga_NI9159_I64FifoLoop.lvbitx", "217E99FD109188EF37C2FCAED84AC82E"};

/**
 * Example configuration to be used with the 'multiIOFirmware' LabView bitstream.
 * For NI9157DeviceGTest and NI9157DeviceOperatorTGTest.
 */
const MARTe::char8 * const multiIoConfig    = ""
    "+NiDevice = {"
    "    Class = NI9157DeviceTestIF"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 0"
    "    Reset = 0"
    "    Run = 0"
    "    Configuration = {"
    "        ControlBool_stop = 0"
    "        ControlBool_use_dsfifo_data = 0"
    "        ControlBool_use_counter = 1"
    "        ControlU32_cycle_ticks = 200"
    "        ControlU64_packet_size = 1"
    "    }"
    "}";

/**
 * Example configuration to be used with the remaining LabView bitstreams.
 * For NI9157DeviceGTest.
 */
const MARTe::char8 * const fifoLoopConfig   = ""
    "+NiDevice = {"
    "    Class = NI9157DeviceTestIF"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 1"
    "    Configuration = {"
    "        ControlU32_cycle_ticks = 40000"
    "    }"
    "}";

/**
 * Example configuration to be used with the remaining LabView bitstreams.
 * For NI9157DeviceOperatorTGTest.
 */
const MARTe::char8 * const fifoLoopConfig2  = ""
    "+NiDevice = {"
    "    Class = NI9157Device"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 1"
    "    Configuration = {"
    "        ControlU32_cycle_ticks = 40000"
    "    }"
    "}";

#endif /* NI9157DEVICETESTSANDFWSETTINGS_H_ */
