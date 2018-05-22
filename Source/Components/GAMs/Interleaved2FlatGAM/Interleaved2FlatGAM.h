/**
 * @file Interleaved2FlatGAM.h
 * @brief Header file for class Interleaved2FlatGAM
 * @date 24/04/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class Interleaved2FlatGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERLEAVED2FLATGAM_H_
#define INTERLEAVED2FLATGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which allows to translate an interleaved memory region into a flat memory area (and vice-versa).
 * @details Some data-sources are expected to produce signals which are interleaved, i.e. which are in the form:
 * | S1_1 | S2_1 | ... | SN_1 | S1_2 | S2_2 | ... | SN_2 | ... | S1_M1 | S2_M2 |... | SN_MN |, where N is the signal number and MN is the number of samples of the Nth signal.
 *
 * Typically the signal memory in the GAMs is flat, i.e. it has the form:
 * | S1_1 | S1_2 | ... | S1_M1 |
 * | S2_1 | S2_2 | ... | S2_M2 |
 * | SN_1 | SN_2 | ... | SN_MN |, again, where N is the signal number and MN is the number of samples of the Nth signal.
 *
 * The GAM also allows to perform the inverse operation, i.e. from flat to interleaved memory.
 *
 * The structure of the interleaved memory region is expressed with the PacketMemberSizes definition. The size of each member shall be defined in bytes.
 * For example, PacketMemberSizes = {2, 4, 2} would mean that the first signal has two bytes, the second four bytes and the third, two bytes again.
 *
 * The Samples parameter defines how many times the pattern above is repeated. So that, for example, Samples = 3 would imply the following memory structure {2, 4, 2}{2, 4, 2}{2, 4, 2}
 *
 * The size of the input signals shall match the size of the output signals and the sum of the PacketMemberSizes shall be a sub-multiple of the signal size.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 *
 * <pre>
 * +InterleavedGAM1 = {
 *     Class = Interleaved2FlatGAM
 *     InputSignals = {
 *         InterleavedSignal = {
 *             DataSource = Drv1
 *             Type = uint8
 *             NumberOfDimensions = 1
 *             NumberOfElements = 20
 *             Samples = 3
 *             PacketMemberSizes = {8, 2, 2, 4, 1, 1, 2} //Sum shall be 20
 *         }
 *     }
 *     OutputSignals = {
 *         PacketS0 = {
 *             DataSource = DDB1
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = 3 //The number of samples is 3 and as such there will be 3 PacketS0
 *         }
 *         PacketS1 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS2 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS3 = {
 *              DataSource = DDB1
 *              Type = uint32
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS4 = {
 *              DataSource = DDB1
 *              Type = uint8
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS5 = {
 *              DataSource = DDB1
 *              Type = uint8
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS6 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *     }
 * }
 * </pre>
 *
 * To go from flat to interleaved a possible configuration could be:
 * <pre>
 * +InterleavedGAM1 = {
 *     Class = Interleaved2FlatGAM
 *     InputSignals = {
 *         PacketS0 = {
 *             DataSource = DDB1
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = 3
 *         }
 *         PacketS1 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS2 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS3 = {
 *              DataSource = DDB1
 *              Type = uint32
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS4 = {
 *              DataSource = DDB1
 *              Type = uint8
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS5 = {
 *              DataSource = DDB1
 *              Type = uint8
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS6 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *     }
 *     OutputSignals = {
 *         InterleavedSignal = {
 *             DataSource = Drv1
 *             Type = uint8
 *             NumberOfDimensions = 1
 *             NumberOfElements = 20
 *             Samples = 3
 *             PacketMemberSizes = {8, 2, 2, 4, 1, 1, 2} //Sum shall be 20
 *         }
 *     }
 * }
 * </pre>
 *
 * Note that the Samples is not compulsory, provided that the NumberOfElements is a multiple of PacketMemberSizes. For example:
 *
 * <pre>
 * +InterleavedGAM1 = {
 *     Class = Interleaved2FlatGAM
 *     InputSignals = {
 *         InterleavedSignal = {
 *             DataSource = Drv1
 *             Type = uint8
 *             NumberOfDimensions = 1
 *             NumberOfElements = 60 // 3 x sum(PacketMemberSizes)
 *             PacketMemberSizes = {8, 2, 2, 4, 1, 1, 2} //Sum shall be 20
 *         }
 *     }
 *     OutputSignals = {
 *         PacketS0 = {
 *             DataSource = DDB1
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = 3
 *         }
 *         PacketS1 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS2 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS3 = {
 *              DataSource = DDB1
 *              Type = uint32
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS4 = {
 *              DataSource = DDB1
 *              Type = uint8
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS5 = {
 *              DataSource = DDB1
 *              Type = uint8
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *         PacketS6 = {
 *              DataSource = DDB1
 *              Type = uint16
 *              NumberOfDimensions = 1
 *              NumberOfElements = 3
 *         }
 *     }
 * }
 * </pre>
 */
class Interleaved2FlatGAM: public GAM {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
Interleaved2FlatGAM    ();

    /**
     * @brief Destructor. Deallocate memory.
     */
    virtual ~Interleaved2FlatGAM();

    /**
     * @brief Verifies correctness of the GAM configuration.
     * @details Checks that:
     *  - The memory size of the input signals is the equal to the memory size of the output signals.
     *  - For each signal, the sum(PacketMemberSizes) is a sub-multiple of the signal size.
     * @return true if all the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @brief Translates from interleaved to flat memory (or vice-versa).
     * @return true.
     */
    virtual bool Execute();

protected:
    /**
     * Number of input samples for each signal.
     */
    uint32 *numberOfInputSamples;

    /**
     * Number of output samples for each signal.
     */
    uint32 *numberOfOutputSamples;

    /**
     * Size of each input signal.
     */
    uint32 *inputByteSize;

    /**
     * Size of each output signal.
     */
    uint32 *outputByteSize;

    /**
     * Number of members for each input signal.
     */
    uint32 *numberOfInputPacketChunks;
    /**
     * Dimensions of the member sizes for each input signal.
     */
    uint32 *packetInputChunkSize;

    /**
     * Number of members for each output signal.
     */
    uint32 *numberOfOutputPacketChunks;

    /**
     * Dimensions of the member sizes for each output signal.
     */
    uint32 *packetOutputChunkSize;

    /**
     * Total signal size (shall be the same for input and output signals).
     */
    uint32 totalSignalsByteSize;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_GAMS_INTERLEAVED2FLATGAM_INTERLEAVED2FLATGAM_H_ */

