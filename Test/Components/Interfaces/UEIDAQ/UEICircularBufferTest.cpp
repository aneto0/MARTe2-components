/**
 * @file UEICircularBufferTest.cpp
 * @brief Source file for class UEICircularBufferTest
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

 * @details This source file contains the definition of all the methods for
 * the class UEICircularBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "TypeDescriptor.h"
#include "UEICircularBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UEICircularBufferHL : public UEICircularBuffer {
public:
    CLASS_REGISTER_DECLARATION()
    UEICircularBufferHL();
    ~UEICircularBufferHL();
    bool GetTimestampRequiredHL()           {return timestampRequired;}   
    uint8* GetHeadPointerHL()               {return headPointer;}
    uint32 GetBufferLengthHL()              {return bufferLength;}
    uint32 GetFullBufferLengthHL()              {return fullBufferLength;}
    uint32 GetNOfBuffersHL()                {return nOfBuffers;}
    uint32 GetSingleBufferLengthHL()        {return singleBufferLength;}
    uint32 GetRunawayZoneLengthHL()         {return runawayZoneLength;}
    uint32 GetSamplesPerExternalWriteHL()       {return samplesPerExternalWrite;}
    uint32 GetNChannelsHL()                 {return nChannels;}
    uint8 GetSizeOfSamplesHL()             {return sizeOfSamples;}
    uint32 GetSamplesPerExternalReadHL()               {return samplesPerExternalRead;}
    UEIBufferPointer* GetPointerListHL()    {return pointerList;}
    UEIBufferPointer GetTimestampListHL()   {return timestampList;}
};
UEICircularBufferHL::UEICircularBufferHL() : UEICircularBuffer(){

}
UEICircularBufferHL::~UEICircularBufferHL(){

}
CLASS_REGISTER(UEICircularBufferHL, "1.0")


UEICircularBufferTest::UEICircularBufferTest(){
}

UEICircularBufferTest::~UEICircularBufferTest(){
}

bool UEICircularBufferTest::TestConstructor() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    ok &= SafeMath::IsEqual(testDevice.writePointer, NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetTimestampRequiredHL(), false);
    ok &= SafeMath::IsEqual(testDevice.GetHeadPointerHL(), NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetBufferLengthHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetNOfBuffersHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSingleBufferLengthHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetRunawayZoneLengthHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetBufferLengthHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSamplesPerExternalWriteHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetNChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSizeOfSamplesHL(), (uint8) 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSamplesPerExternalReadHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetPointerListHL(), NULL_PTR(UEIBufferPointer*));
    // With the buffer not set through InitialiseBuffer method, check that the rest of functions fail with no segfault
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), false);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(10u), false);
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(10u), false);
    bool dummyOk = true;
    testDevice.ReadBuffer(dummyOk);
    ok &= SafeMath::IsEqual(dummyOk, false);
    dummyOk = true;
    UEIBufferPointer dummyPointer;
    dummyOk = testDevice.ReadTimestamp(dummyPointer);
    ok &= SafeMath::IsEqual(dummyOk, false);
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), false);
    return ok;
}

bool UEICircularBufferTest::TestInitialiseBuffer() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 10u; 
    uint32 channels = 2u;
    uint32 samplesPerMapRequest = 10u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 20u;
    bool tStampRequired = true;
    uint32 singleBufferLength = ((channels*sOfSamples)+tStampRequired*sizeof(uint32))*nReadSamples;
    uint32 runawayZoneLength = ((channels*sOfSamples)+tStampRequired*sizeof(uint32))*samplesPerMapRequest;
    uint32 bufferLength = singleBufferLength*numberOfBuffers;
    uint32 fullBufferLength = bufferLength + runawayZoneLength;
    //Check a correct initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), true);
    //Check that the values are correctly set
    ok &= (testDevice.writePointer != NULL_PTR(uint8*));
    ok &= (testDevice.GetHeadPointerHL()!= NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetHeadPointerHL(), testDevice.writePointer);
    ok &= (testDevice.GetTimestampRequiredHL(), true);
    ok &= SafeMath::IsEqual(testDevice.GetFullBufferLengthHL(), fullBufferLength);
    ok &= SafeMath::IsEqual(testDevice.GetBufferLengthHL(), bufferLength);
    ok &= SafeMath::IsEqual(testDevice.GetNOfBuffersHL(), numberOfBuffers);
    ok &= SafeMath::IsEqual(testDevice.GetSingleBufferLengthHL(), singleBufferLength);
    ok &= SafeMath::IsEqual(testDevice.GetRunawayZoneLengthHL(), runawayZoneLength);
    ok &= SafeMath::IsEqual(testDevice.GetSamplesPerExternalWriteHL(), samplesPerMapRequest);
    ok &= SafeMath::IsEqual(testDevice.GetNChannelsHL(), channels);
    ok &= SafeMath::IsEqual(testDevice.GetSizeOfSamplesHL(), (uint8) sOfSamples);
    ok &= SafeMath::IsEqual(testDevice.GetSamplesPerExternalReadHL(), nReadSamples);
    ok &= (testDevice.GetPointerListHL() != NULL_PTR(UEIBufferPointer*));
    return ok;
}

bool UEICircularBufferTest::TestInitialiseBuffer_NChannelsFail() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 10u; 
    uint32 channels = 0u;
    uint32 samplesPerMapRequest = 10u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 20u;
    bool tStampRequired = true;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), false);
    return ok;
}

bool UEICircularBufferTest::TestInitialiseBuffer_MapSamplesFail() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 10u; 
    uint32 channels = 2u;
    uint32 samplesPerMapRequest = 0u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 20u;
    bool tStampRequired = true;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), false);
    return ok;
}

bool UEICircularBufferTest::TestInitialiseBuffer_ReadSamplesFail() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 10u; 
    uint32 channels = 2u;
    uint32 samplesPerMapRequest = 10u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 0u;
    bool tStampRequired = true;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), false);
    return ok;
}

bool UEICircularBufferTest::TestInitialiseBuffer_SampleSize() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 10u; 
    uint32 channels = 2u;
    uint32 samplesPerMapRequest = 10u;
    uint32 nReadSamples = 20u;
    bool tStampRequired = true;
    //Check a correct initialisation of the buffer
    for (uint8 sOfSamples = 0; sOfSamples < 128 && ok; sOfSamples++){
        bool expectedResult = (sOfSamples > 0 && sOfSamples <= 4)? true:false;
        ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), expectedResult);
    }
    return ok;
}

bool UEICircularBufferTest::TestInitialiseBuffer_nBuffersFail() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 0u; 
    uint32 channels = 2u;
    uint32 samplesPerMapRequest = 10u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 20u;
    bool tStampRequired = true;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), false);
    numberOfBuffers = 1u;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), false);
    numberOfBuffers = 2u;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), true);
    return ok;
}

bool UEICircularBufferTest::FunctionalTest() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 2u;
    uint32 channels = 2u;
    uint32 samplesPerMapRequest = 5u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 8u;
    bool tStampRequired = false;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), true);
    //Check if the buffer is ready to be read
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    //Write some data into the buffer
    uint32 dataPacket_1 [4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(dataPacket_1), 4*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(4*4), true);
    //Check that it still not prepared to ship a set of samples, since we only wrote 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(4*4));

    //Write some more data into the buffer
    uint32 dataPacket_2 [10] = {0x55555555, 0x66666666, 0x77777777, 0x88888888,
                                0x99999999, 0x10101010, 0x11111111, 0x12121212,
                                0x13131313, 0x14141414};
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(dataPacket_2), 10*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(10*4), true);
    //Check that it still not prepared to ship a set of samples, since we only wrote 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(14*4));

    //Write some more data into the buffer
    uint32 dataPacket_3 [2] =  {0x15151515, 0x16161616};
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(dataPacket_3), 2*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(2*4), true);
    //Check that it still not prepared to ship a set of samples, since we only wrote 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(16*4));
    //Check the data returned
    uint32 channel_1_data [8] = {0x11111111, 0x33333333, 0x55555555, 0x77777777, 0x99999999, 0x11111111, 0x13131313, 0x15151515};
    uint32 channel_2_data [8] = {0x22222222, 0x44444444, 0x66666666, 0x88888888, 0x10101010, 0x12121212, 0x14141414, 0x16161616};
    uint32* channel_data[2];
    channel_data[0] = channel_1_data;
    channel_data[1] = channel_2_data;
    UEIBufferPointer* myChannels;
    if (ok){
        //Retrieve the data from the inner interleaved channels on the buffer
        myChannels = testDevice.ReadBuffer(ok);
    }
    if (ok){
        for (uint32 chan = 0; chan < 2 && ok; chan++){
            for (uint32 sam = 0; sam < 8 && ok; sam++){
                uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam)));
                ok &= SafeMath::IsEqual(thisSample, channel_data[chan][sam]);
            }
        }
    }
    //In this situation, we've filled 8 samples for each channel, since we specified 2 buffers of 8 samples per channel, fill an additional 5
    //samples per channel without checking out the current subbuffer.
    uint32 fillerPacket [5*2];
    for (uint32 i = 0; i < 5*2; i++){
        fillerPacket[i] = (uint32) i;
    }
    //The availablespace call returns true as it checks for available space of VMap response (40 bytes)
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(5*2*sizeof(uint32)), true);
    //We cannot copy a new batch of 16 samples as we've filled the first 16 samples and to write the next ones
    //We do need at least a free byte after the operation (design decision)
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 5*2*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(5*2*4), true);
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(13*2*4));
    //Now the circularBuffer must be partially filled, since we've written a total of 26 samples (2channels*13samples)
    //The RunawayZone + 24 bytes are still available to write, but since the sub buffer 0 has still not been checked out the buffer cannot write into that
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), false);
    //If we try to write the 24 bytes to complete the buffer we get an error since the buffer always needs 1byte of space space
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(3*2*sizeof(uint32)), false);
    //Now we do checkout the current buffer 0, so new data can be over-written into it
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), true);
    //Now that the sub-buffer 0 is checked out, we can write into it again
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    //And moreover, the subbuffer 1 should not be ready to read, as we're still missing a total of 6 samples in that buffer
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    //Check if we now can write the new 6 samples into the buffer
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(3*2*sizeof(uint32)), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    //Write the next 6 samples
    for (uint32 i = 0; i < 6; i++){
        fillerPacket[i] = (uint32) i;
    }
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 3*2*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(3*2*4), true);
    //Once copied, check that the subbuffer 1 is ready to read
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    //And the write pointer should be located at head due to circular nature of the buffer
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL());
    //Check if a new VMap content is allowed in the buffer (in subbuffer 0)
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    //We should be able to write up to (but not exceeding 8 samples per channel, a total og 64 bytes)
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(8*2*sizeof(uint32)), false);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(8*2*sizeof(uint32)-1), true);
    if (ok){
        //Retrieve the data from the inner interleaved channels on the buffer
        myChannels = testDevice.ReadBuffer(ok);
    }
    if (ok){
        for (uint32 chan = 0; chan < 2 && ok; chan++){
            for (uint32 sam = 0; sam < 5 && ok; sam++){
                uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam)));
                ok &= SafeMath::IsEqual(thisSample, (uint32)(2*sam+(chan)));
            }
            for (uint32 sam = 0; sam < 3 && ok; sam++){
                uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam+5)));
                ok &= SafeMath::IsEqual(thisSample, (uint32)(2*sam+(chan)));
            }
        }
    }
    //Try to write 16 samples into subbuffer 1, it must succeed
    for (uint32 i = 0; i < 16; i++){
        fillerPacket[i] = (uint32) (15-i);
    }
    //Perform the same checks as before, now the Subbuffer 0 is free, but subbuffer 1 is filled and not checked out
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(16*4), false);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(16*4-1), true);
    //We cannot copy the new contents until we allow the subbuffer 1 free
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), true);
    //Check again
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(16*4), true);
    //Now perform the copy
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 16*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(8*2*4), true);
    //As we've written all the data needed for subbuffer 0 in a single blow, it should be ready to read
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    //The subbuffer 0 is ready to read, but the subbuffer 1 is free now
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    //Check what would happen if we try to write into the runawayZone
    //Write 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(2*2*sizeof(uint32)), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    //Advance the index
    for (uint32 i = 0; i < 16; i++){
        fillerPacket[i] = 0xBADDCAFE;
    }
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 3*2*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(3*2*4), true);
    //Try to write 1 sample more. We cannot because that would leave the circularbuffer out of space, and without the mandatory
    //1 byte free space
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(1*2*sizeof(uint32)), true);  //This method does not consider runaway zone limitation
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), false);
    //To write some more data into the buffer, we do need to check out the subbuffer 0
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), true);
    //With the subbuffer 0 checked out, and subbuffer 1 not ready, we cannot read from the circularbuffer
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    //We still have a total of 5 samples left in the subbuffer 1, we can write up to that plus the runawayzone length into
    //the buffer, but not more, check that
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(5*2*sizeof(uint32)+40), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(5*2*sizeof(uint32)+40+1), false);
    //We will now write a total of 14 samples now, to allow for a write into runaway zone
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 14*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(7*2*4), true);
    //Now the write pointer should be 4 samples ahead of head (2 per channel)
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(4*4));
    //The subbuffer 1 should be ready to read now
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    //Read it to find a nice hexspeak
    myChannels = testDevice.ReadBuffer(ok);
    for (uint32 chan = 0; chan < 2 && ok; chan++){
        for (uint32 sam = 0; sam < 8 && ok; sam++){
            uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam)));
            ok &= SafeMath::IsEqual(thisSample, (uint32)(0xBADDCAFE));
        }
    }
    //If everything is ok, checkout the buffer, advance the index the missing 6 samples per channel and retrieve the result
    //We should see the same hexspeak on the first 2 samples per channel on subbuffer 0 if the copy went correctly, set the rest
    // to 0 to differentiate
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(6*2*sizeof(uint32)),false);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(6*2*sizeof(uint32)-1),true);
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), true);
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(6*2*sizeof(uint32)),true);
    for (uint32 i = 0; i < 16; i++){
        fillerPacket[i] = 0x00000000;
    }
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 6*2*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(6*2*4), true);
    //Check the write pointer position now
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(8*2*4));
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    //Read it to find a nice hexspeak
    myChannels = testDevice.ReadBuffer(ok);
    for (uint32 chan = 0; chan < 2 && ok; chan++){
        for (uint32 sam = 0; sam < 2 && ok; sam++){
            uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam)));
            ok &= SafeMath::IsEqual(thisSample, (uint32)(0xBADDCAFE));
        }
        for (uint32 sam = 2; sam < 8 && ok; sam++){
            uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam)));
            ok &= SafeMath::IsEqual(thisSample, (uint32)(0x00000000));
        }
    }
    //If we reach here, the working principle of the circularbuffer is demonstrated
    return ok;
}

bool UEICircularBufferTest::FunctionalTest_WithTimestamp() {
    bool ok = true;
    UEICircularBufferHL testDevice;
    uint32 numberOfBuffers = 2u;
    uint32 channels = 1u;
    uint32 samplesPerMapRequest = 5u;
    uint8 sOfSamples = sizeof(uint32);
    uint32 nReadSamples = 8u;
    bool tStampRequired = true;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), true);
    //Check if the buffer is ready to be read
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    //Write some data into the buffer
    uint32 dataPacket_1 [4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(dataPacket_1), 4*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(4*4), true);
    //Check that it still not prepared to ship a set of samples, since we only wrote 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(4*4));

    //Write some more data into the buffer
    uint32 dataPacket_2 [10] = {0x55555555, 0x66666666, 0x77777777, 0x88888888,
                                0x99999999, 0x10101010, 0x11111111, 0x12121212,
                                0x13131313, 0x14141414};
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(dataPacket_2), 10*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(10*4), true);
    //Check that it still not prepared to ship a set of samples, since we only wrote 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(14*4));

    //Write some more data into the buffer
    uint32 dataPacket_3 [2] =  {0x15151515, 0x16161616};
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(dataPacket_3), 2*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(2*4), true);
    //Check that it still not prepared to ship a set of samples, since we only wrote 2 samples
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(16*4));
    //Check the data returned
    uint32 channel_1_data [8] = {0x11111111, 0x33333333, 0x55555555, 0x77777777, 0x99999999, 0x11111111, 0x13131313, 0x15151515};
    uint32 channel_2_data [8] = {0x22222222, 0x44444444, 0x66666666, 0x88888888, 0x10101010, 0x12121212, 0x14141414, 0x16161616};
    uint32* channel_data[2];
    channel_data[0] = channel_1_data;
    channel_data[1] = channel_2_data;
    UEIBufferPointer* myChannels;
    UEIBufferPointer myTimestampChannel;
    if (ok){
        //Retrieve the data from the inner interleaved channels on the buffer
        myChannels = testDevice.ReadBuffer(ok);
    }
    if (ok){
        //Retrieve the data from the inner interleaved channels on the buffer
        ok &= testDevice.ReadTimestamp(myTimestampChannel);
    }
    if (ok){
        //The timestamp is always the first of the channels in the buffer
        for (uint32 sam = 0; sam < 8 && ok; sam++){
            uint32 thisSample = *(reinterpret_cast<uint32*>(myTimestampChannel.GetSample(sam)));
            ok &= SafeMath::IsEqual(thisSample, channel_data[0][sam]);
        }
        for (uint32 sam = 0; sam < 8 && ok; sam++){
            uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[0].GetSample(sam)));
            ok &= SafeMath::IsEqual(thisSample, channel_data[1][sam]);
        }
    }
    return ok;
}