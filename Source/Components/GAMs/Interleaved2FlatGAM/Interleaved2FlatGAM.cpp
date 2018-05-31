/**
 * @file Interleaved2FlatGAM.cpp
 * @brief Source file for class Interleaved2FlatGAM
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

 * @details This source file contains the definition of all the methods for
 * the class Interleaved2FlatGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Interleaved2FlatGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
Interleaved2FlatGAM::Interleaved2FlatGAM() :
        GAM() {
    numberOfInputSamples = NULL_PTR(uint32 *);
    numberOfOutputSamples = NULL_PTR(uint32 *);
    inputByteSize = NULL_PTR(uint32 *);
    outputByteSize = NULL_PTR(uint32 *);

    numberOfInputPacketChunks = NULL_PTR(uint32 *);
    packetInputChunkSize = NULL_PTR(uint32 *);

    numberOfOutputPacketChunks = NULL_PTR(uint32 *);
    packetOutputChunkSize = NULL_PTR(uint32 *);
    totalSignalsByteSize = 0u;
}

Interleaved2FlatGAM::~Interleaved2FlatGAM() {
    if (numberOfInputSamples != NULL_PTR(uint32 *)) {
        delete[] numberOfInputSamples;
    }

    if (numberOfOutputSamples != NULL_PTR(uint32 *)) {
        delete[] numberOfOutputSamples;
    }

    if (inputByteSize != NULL_PTR(uint32 *)) {
        delete[] inputByteSize;
    }

    if (outputByteSize != NULL_PTR(uint32 *)) {
        delete[] outputByteSize;
    }

    if (numberOfInputPacketChunks != NULL_PTR(uint32 *)) {
        delete[] numberOfInputPacketChunks;
    }

    if (packetInputChunkSize != NULL_PTR(uint32 *)) {
        delete[] packetInputChunkSize;
    }

    if (numberOfOutputPacketChunks != NULL_PTR(uint32 *)) {
        delete[] numberOfOutputPacketChunks;
    }

    if (packetOutputChunkSize != NULL_PTR(uint32 *)) {
        delete[] packetOutputChunkSize;
    }

}

/*lint -e{613} null pointer checked.*/
bool Interleaved2FlatGAM::Setup() {

    numberOfInputSamples = new uint32[numberOfInputSignals];
    bool ret = (numberOfInputSamples != NULL_PTR(uint32 *));
    if (ret) {
        numberOfOutputSamples = new uint32[numberOfOutputSignals];
        ret = (numberOfOutputSamples != NULL_PTR(uint32 *));
    }
    if (ret) {
        inputByteSize = new uint32[numberOfInputSignals];
        ret = (inputByteSize != NULL_PTR(uint32 *));
    }
    if (ret) {

        outputByteSize = new uint32[numberOfOutputSignals];
        ret = (outputByteSize != NULL_PTR(uint32 *));
    }
    if (ret) {

        numberOfInputPacketChunks = new uint32[numberOfInputSignals];
        ret = (numberOfInputPacketChunks != NULL_PTR(uint32 *));
    }
    if (ret) {

        numberOfOutputPacketChunks = new uint32[numberOfOutputSignals];
        ret = (numberOfOutputPacketChunks != NULL_PTR(uint32 *));
    }

    //store info for input signals
    if (ret) {
        ret = signalsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = signalsDatabase.MoveRelative("InputSignals");
    }

    uint32 totalNumberOfInputChunkSizes = 0u;
    for (uint32 i = 0u; (i < numberOfInputSignals) && (ret); i++) {
        ret = signalsDatabase.MoveToChild(i);
        if (ret) {
            //read the packet config
            AnyType at = signalsDatabase.GetType("PacketMemberSizes");
            if (!at.IsVoid()) {
                numberOfInputPacketChunks[i] = at.GetNumberOfElements(0u);
                totalNumberOfInputChunkSizes += numberOfInputPacketChunks[i];
            }
            else {
                numberOfInputPacketChunks[i] = 0u;
            }
        }
        if (ret) {
            ret = signalsDatabase.MoveToAncestor(1u);
        }
    }
    packetInputChunkSize = new uint32[totalNumberOfInputChunkSizes];

    uint32 cnt = 0u;
    uint32 index = 0u;
    for (uint32 i = 0u; (i < numberOfInputSignals) && (ret); i++) {
        ret = signalsDatabase.MoveToChild(i);
        if (ret) {
            if (numberOfInputPacketChunks[i] > 0u) {
                Vector<uint32> packetChunkSizeVec(&packetInputChunkSize[index], numberOfInputPacketChunks[i]);
                ret = signalsDatabase.Read("PacketMemberSizes", packetChunkSizeVec);
                index += numberOfInputPacketChunks[i];
            }
        }

        if (ret) {
            ret = GetSignalByteSize(InputSignals, i, inputByteSize[i]);
            if (ret) {
                uint32 packetSize = 0u;
                //check that the byte size is consistent with the packet configuration
                for (uint32 j = 0u; j < numberOfInputPacketChunks[i]; j++) {
                    packetSize += packetInputChunkSize[cnt];
                    cnt++;
                }
                if (packetSize == 0u) {
                    packetSize = inputByteSize[i];
                }

                uint32 fakeSamples = (inputByteSize[i] / packetSize);
                ret = ((inputByteSize[i] % packetSize) == 0u);

                if (ret) {
                    ret = GetSignalNumberOfSamples(InputSignals, i, numberOfInputSamples[i]);
                    totalSignalsByteSize += (inputByteSize[i] * numberOfInputSamples[i]);

                    numberOfInputSamples[i] *= fakeSamples;
                    inputByteSize[i] /= fakeSamples;
                }
                else {
                    const uint32 ii = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError,
                                 "The signal %d byte size %d must be divided exactly by the sum of the packet member sizes %d", ii, inputByteSize[ii],
                                 packetSize);
                }
            }
        }
        if (ret) {
            ret = signalsDatabase.MoveToAncestor(1u);
        }
    }

    //store info for output signals
    if (ret) {
        ret = signalsDatabase.MoveToRoot();
        if (ret) {
            ret = signalsDatabase.MoveRelative("OutputSignals");
        }
    }

    uint32 totalNumberOfOutputChunkSizes = 0u;
    uint32 totalSignalsOutByteSize = 0u;
    cnt = 0u;
    index = 0u;
    for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
        ret = signalsDatabase.MoveToChild(i);
        if (ret) {
            //read the packet config
            AnyType at = signalsDatabase.GetType("PacketMemberSizes");
            if (!at.IsVoid()) {
                numberOfOutputPacketChunks[i] = at.GetNumberOfElements(0u);
                totalNumberOfOutputChunkSizes += numberOfOutputPacketChunks[i];
            }
            else {
                numberOfOutputPacketChunks[i] = 0u;
            }
        }
        if (ret) {
            ret = signalsDatabase.MoveToAncestor(1u);
        }
    }
    packetOutputChunkSize = new uint32[totalNumberOfOutputChunkSizes];

    for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
        ret = signalsDatabase.MoveToChild(i);
        if (ret) {
            if (numberOfOutputPacketChunks[i] > 0u) {

                Vector<uint32> packetChunkSizeVec(&packetOutputChunkSize[index], numberOfOutputPacketChunks[i]);
                ret = signalsDatabase.Read("PacketMemberSizes", packetChunkSizeVec);
                index += numberOfOutputPacketChunks[i];
            }
        }
        if (ret) {
            ret = GetSignalByteSize(OutputSignals, i, outputByteSize[i]);
            if (ret) {
                uint32 packetSize = 0u;
                //check that the byte size is consistent with the packet configuration
                for (uint32 j = 0u; j < numberOfOutputPacketChunks[i]; j++) {
                    packetSize += packetOutputChunkSize[cnt];
                    cnt++;
                }
                if (packetSize == 0u) {
                    packetSize = outputByteSize[i];
                }

                uint32 fakeSamples = (outputByteSize[i] / packetSize);
                ret = ((outputByteSize[i] % packetSize) == 0u);

                if (ret) {
                    ret = GetSignalNumberOfSamples(OutputSignals, i, numberOfOutputSamples[i]);
                    totalSignalsOutByteSize += (outputByteSize[i] * numberOfOutputSamples[i]);

                    numberOfOutputSamples[i] *= fakeSamples;
                    outputByteSize[i] /= fakeSamples;
                }
                else {
                    const uint32 ii = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError,
                                 "The signal %d byte size %d must be divided exactly by the sum of the packet member sizes %d", ii, outputByteSize[ii],
                                 packetSize);
                }

            }
        }
        if (ret) {
            ret = signalsDatabase.MoveToAncestor(1u);
        }
    }

    //check that input size equal to output size
    if (ret) {
        ret = (totalSignalsOutByteSize == totalSignalsByteSize);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The input size %d and the output size %d must be equal!", totalSignalsByteSize,
                         totalSignalsOutByteSize);
        }
    }

    //check alignment
    if (ret) {
        uint32 i = 0u;
        uint32 j = 0u;
        while ((i < numberOfInputSignals) && (j < numberOfOutputSignals) && (ret)) {
            if ((outputByteSize[j] * numberOfOutputSamples[j]) > (inputByteSize[i] * numberOfInputSamples[i])) {
                uint32 size = 0u;
                while (((size < (outputByteSize[j] * numberOfOutputSamples[j])) && (i < numberOfInputSignals)) && (ret)) {
                    size += (inputByteSize[i] * numberOfInputSamples[i]);
                    ret = (numberOfInputPacketChunks[i] * numberOfOutputPacketChunks[j]) == 0u;
                    i++;
                }
                uint32 indexIn = (i - 1u);
                if (ret) {
                    ret = (size == (outputByteSize[j] * numberOfOutputSamples[j]));
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Input signal %d and output signal %d are not aligned", indexIn, j);
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot define packet configuration in both signals input %d and output %d", indexIn, j);
                }
                j++;
            }
            else {
                uint32 size = 0u;
                while (((size < (inputByteSize[i] * numberOfInputSamples[i])) && (j < numberOfOutputSignals)) && (ret)) {
                    size += (outputByteSize[j] * numberOfOutputSamples[j]);
                    ret = (numberOfInputPacketChunks[i] * numberOfOutputPacketChunks[j]) == 0u;
                    j++;
                }
                uint32 indexOut = (j - 1u);
                if (ret) {
                    ret = (size == (inputByteSize[i] * numberOfInputSamples[i]));
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Input signal %d and output signal %d are not aligned", i, indexOut);
                    }
                }
                else {

                    REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot define packet configuration in both signals input %d and output %d", i,
                                 indexOut);
                }
                i++;
            }
        }

    }

    return ret;
}

/*lint -e{613} null pointer checked.*/
bool Interleaved2FlatGAM::Execute() {
    bool ret = MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
    uint32 cnt = 0u;
    uint32 offset = 0u;
    for (uint32 n = 0u; (n < numberOfInputSignals); n++) {
        //do the split of the signals
        uint8 *originSource = reinterpret_cast<uint8 *>(GetInputSignalMemory(n));

        uint8 *originDest = &(reinterpret_cast<uint8 *>(GetOutputSignalsMemory()))[offset];

        //this should be the number of samples
        /*lint -e{613} null pointer checked before.*/
        for (uint32 k = 0u; k < numberOfInputSamples[n]; k++) {
            //return to the gam pointet
            uint8 *dest = originDest;
            //go to the k-th packet in the data source
            /*lint -e{613} null pointer checked before.*/
            uint32 index = (k * inputByteSize[n]);
            /*lint -e{613} null pointer checked before.*/
            uint8 *source = &originSource[index];
            /*lint -e{613} null pointer checked before.*/
            for (uint32 i = 0u; i < numberOfInputPacketChunks[n]; i++) {
                //get the chunk size
                index = (cnt + i);
                /*lint -e{613} null pointer checked before.*/
                uint32 chunkSize = packetInputChunkSize[index];
                //put the k-th element
                index = (k * chunkSize);
                dest = &dest[index];
                (void) MemoryOperationsHelper::Copy(dest, source, chunkSize);
                //skip the N-k other samples to go to the next chunk mem
                index = ((numberOfInputSamples[n] - k) * chunkSize);
                dest = &dest[index];
                //get the next chunk in source
                source = &source[chunkSize];
            }
        }
        offset += (inputByteSize[n] * numberOfInputSamples[n]);
        cnt += numberOfInputPacketChunks[n];

    }

    cnt = 0u;
    offset = 0u;
    for (uint32 n = 0u; (n < numberOfOutputSignals); n++) {
        //do the split of the signals
        uint8 *originSource = &(reinterpret_cast<uint8 *>(GetInputSignalsMemory()))[offset];

        uint8 *originDest = reinterpret_cast<uint8 *>(GetOutputSignalMemory(n));

        //this should be the number of samples
        /*lint -e{613} null pointer checked before.*/
        for (uint32 k = 0u; k < numberOfOutputSamples[n]; k++) {
            //return to the gam pointet
            uint8 *source = originSource;
            //go to the k-th packet in the data source
            /*lint -e{613} null pointer checked before.*/
            uint32 index = (k * outputByteSize[n]);
            /*lint -e{613} null pointer checked before.*/
            uint8 *dest = &originDest[index];
            /*lint -e{613} null pointer checked before.*/
            for (uint32 i = 0u; i < numberOfOutputPacketChunks[n]; i++) {
                //get the chunk size
                index = (cnt + i);
                /*lint -e{613} null pointer checked before.*/
                uint32 chunkSize = packetOutputChunkSize[index];
                //put the k-th element
                index = (k * chunkSize);
                source = &source[index];
                (void) MemoryOperationsHelper::Copy(dest, source, chunkSize);
                //skip the N-k other samples to go to the next chunk mem
                index = ((numberOfOutputSamples[n] - k) * chunkSize);
                source = &source[index];
                //get the next chunk in source
                dest = &dest[chunkSize];
            }
        }
        offset += (outputByteSize[n] * numberOfOutputSamples[n]);
        cnt += numberOfOutputPacketChunks[n];

    }

    return ret;

}
CLASS_REGISTER(Interleaved2FlatGAM, "1.0")
}

