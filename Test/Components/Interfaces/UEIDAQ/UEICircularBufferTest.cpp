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
    uint32 GetNOfBuffersHL()                {return nOfBuffers;}
    uint32 GetSingleBufferLengthHL()        {return singleBufferLength;}
    uint32 GetRunawayZoneLengthHL()         {return runawayZoneLength;}
    uint32 GetCurrentActiveBufferHL()       {return currentActiveBuffer;}
    uint32 GetSamplesInMapRequestHL()       {return samplesInMapRequest;}
    uint32 GetNChannelsHL()                 {return nChannels;}
    uint8 GetSizeOfSamplesHL()             {return sizeOfSamples;}
    uint32 GetReadSamplesHL()               {return readSamples;}
    UEIBufferPointer* GetPointerListHL()    {return pointerList;}
    UEIBufferPointer GetTimestampListHL()   {return timestampList;}
    bool GetBufferSetHL()                   {return bufferSet;}
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
    ok &= SafeMath::IsEqual(testDevice.GetCurrentActiveBufferHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetBufferLengthHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSamplesInMapRequestHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetNChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSizeOfSamplesHL(), (uint8) 0u);
    ok &= SafeMath::IsEqual(testDevice.GetReadSamplesHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetPointerListHL(), NULL_PTR(UEIBufferPointer*));
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), false);
    // With the buffer not set through InitialiseBuffer method, check that the rest of functions fail with no segfault
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), false);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(10u), false);
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), false);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(10u), false);
    bool dummyOk = true;
    testDevice.ReadBuffer(dummyOk);
    ok &= SafeMath::IsEqual(dummyOk, false);
    dummyOk = true;
    testDevice.ReadTimestamp(dummyOk);
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
    uint32 bufferLength = singleBufferLength*numberOfBuffers + runawayZoneLength;
    //Check a correct initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), true);
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), true);
    //Check that the values are correctly set
    ok &= (testDevice.writePointer != NULL_PTR(uint8*));
    ok &= (testDevice.GetHeadPointerHL()!= NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetHeadPointerHL(), testDevice.writePointer);
    ok &= (testDevice.GetTimestampRequiredHL(), true);
    ok &= SafeMath::IsEqual(testDevice.GetBufferLengthHL(), bufferLength);
    ok &= SafeMath::IsEqual(testDevice.GetNOfBuffersHL(), numberOfBuffers);
    ok &= SafeMath::IsEqual(testDevice.GetSingleBufferLengthHL(), singleBufferLength);
    ok &= SafeMath::IsEqual(testDevice.GetRunawayZoneLengthHL(), runawayZoneLength);
    ok &= SafeMath::IsEqual(testDevice.GetCurrentActiveBufferHL(), 0u);
    ok &= SafeMath::IsEqual(testDevice.GetSamplesInMapRequestHL(), samplesPerMapRequest);
    ok &= SafeMath::IsEqual(testDevice.GetNChannelsHL(), channels);
    ok &= SafeMath::IsEqual(testDevice.GetSizeOfSamplesHL(), (uint8) sOfSamples);
    ok &= SafeMath::IsEqual(testDevice.GetReadSamplesHL(), nReadSamples);
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
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), false);
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
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), false);
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
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), false);
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
        ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), expectedResult);
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
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), false);
    numberOfBuffers = 1u;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), false);
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), false);
    numberOfBuffers = 2u;
    //Check initialisation of the buffer
    ok &= SafeMath::IsEqual(testDevice.InitialiseBuffer(numberOfBuffers, channels, samplesPerMapRequest, sOfSamples, nReadSamples, tStampRequired), true);
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), true);
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
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), true);
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
    //In this situation, we've filled 8 samples for each channel, since we specified 2 buffers of 8 samples per channel, fill an additional 8
    //samples per channel without checking out the current subbuffer.
    uint32 fillerPacket [16];
    for (uint32 i = 0; i < 16; i++){
        fillerPacket[i] = (uint32) i;
    }
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 16*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(16*4), true);
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    ok &= SafeMath::IsEqual(testDevice.writePointer, testDevice.GetHeadPointerHL()+(32*4));
    //Now the circularBuffer must be completely filled, since we've written the total 32 samples it can hold (2channels*8samples*2buffers)
    //The RunawayZone is still available to write, bu since the sub buffer 0 has still not been checked out the buffer cannot write into that
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), false);
    //In this situation we should not be able to write into the buffer any more until checking out the current buffer (even 1 bytes)
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(1), false);
    //Now we do checkout the current buffer 0, so new data can be over-written into it
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), true);
    //Now that the sub-buffer 0 is checked out, we can write into it again
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    //And moreover, the subbuffer 1 should be ready to read from (as we've previously filled it out)
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    if (ok){
        //Retrieve the data from the inner interleaved channels on the buffer
        myChannels = testDevice.ReadBuffer(ok);
    }
    if (ok){
        for (uint32 chan = 0; chan < 2 && ok; chan++){
            for (uint32 sam = 0; sam < 8 && ok; sam++){
                uint32 thisSample = *(reinterpret_cast<uint32*>(myChannels[chan].GetSample(sam)));
                ok &= SafeMath::IsEqual(thisSample, (uint32)(2*sam+(chan)));
            }
        }
    }
    //Try to write 16 samples into subbuffer 1, it must succeed
    for (uint32 i = 0; i < 16; i++){
        fillerPacket[i] = (uint32) (15-i);
    }
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), true);
    memcpy(testDevice.writePointer, reinterpret_cast<uint8*>(fillerPacket), 16*4);
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(16*4), true);
    ok &= SafeMath::IsEqual(testDevice.CheckReadReady(), true);
    ok &= SafeMath::IsEqual(testDevice.CheckAvailableSpace(), false);
    //In this situation we should not be able to write into the buffer any more until checking out the current buffer (even 1 bytes)
    ok &= SafeMath::IsEqual(testDevice.AdvanceBufferIndex(1), false);
    //Now we do checkout the current buffer 1, so new data can be over-written into it
    ok &= SafeMath::IsEqual(testDevice.CheckoutBuffer(), true);

    //Test reset buffer call
    ok &= SafeMath::IsEqual(testDevice.ResetBuffer(), true);
    ok &= SafeMath::IsEqual(testDevice.GetHeadPointerHL(), testDevice.writePointer);
    //TODO batch test of samples getting written into the buffer
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
    ok &= SafeMath::IsEqual(testDevice.GetBufferSetHL(), true);
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
        myTimestampChannel = testDevice.ReadTimestamp(ok);
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