/**
 * @file UEIBufferPointerTest.cpp
 * @brief Source file for class UEIBufferPointerTest
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
 * the class UEIBufferPointerTest (public, protected, and private). Be aware that some 
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
#include "UEIBufferPointerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UEIBufferPointerHL : public UEIBufferPointer {
public:
    CLASS_REGISTER_DECLARATION()
    UEIBufferPointerHL();
    ~UEIBufferPointerHL();
    uint8* GetBlockHeadPointerHL()      {return blockHeadPointer;}
    uint32 GetOffsetHL()                {return offset;}
    uint32 GetPointerGainHL()           {return pointerGain;}
    uint32 GetMaxLengthHL()           {return maxLength;}
};
UEIBufferPointerHL::UEIBufferPointerHL() : UEIBufferPointer(){

}
UEIBufferPointerHL::~UEIBufferPointerHL(){

}
CLASS_REGISTER(UEIBufferPointerHL, "1.0")


UEIBufferPointerTest::UEIBufferPointerTest(){
}

UEIBufferPointerTest::~UEIBufferPointerTest(){
}

bool UEIBufferPointerTest::TestConstructor() {
    bool ok = true;
    UEIBufferPointerHL testDevice;
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetOffsetHL(), (uint32) 0u);
    ok &= SafeMath::IsEqual(testDevice.GetPointerGainHL(), (uint32) 0u);
    ok &= SafeMath::IsEqual(testDevice.GetMaxLengthHL(), (uint32) 0u);
    return ok;
}

bool UEIBufferPointerTest::TestSetter() {
    bool ok = true;
    UEIBufferPointerHL testDevice;
    testDevice.SetPointerCharacteristics(10u, sizeof(uint32), 10u);
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetOffsetHL(), (uint32) 10u);
    ok &= SafeMath::IsEqual(testDevice.GetPointerGainHL(), (uint32) 4u);
    ok &= SafeMath::IsEqual(testDevice.GetMaxLengthHL(), (uint32) 10u);
    return ok;
}

bool UEIBufferPointerTest::TestPointerSetter() {
    bool ok = true;
    UEIBufferPointerHL testDevice;
    ok &= !testDevice.SetHead(NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), NULL_PTR(uint8*));
    uint8* memoryArea = new uint8 [10];
    ok &= testDevice.SetHead(memoryArea);
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), memoryArea);
    uint8* secondMemoryArea = new uint8 [10];
    ok &= testDevice.SetHead(secondMemoryArea);
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), secondMemoryArea);
    return ok;
}

bool UEIBufferPointerTest::TestCheckPointer() {
    bool ok = true;
    UEIBufferPointerHL testDevice;
    ok &= !testDevice.SetHead(NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(testDevice.CheckPointer(), false);
    uint8* memoryArea = new uint8 [10];
    ok &= testDevice.SetHead(memoryArea);
    ok &= SafeMath::IsEqual(testDevice.GetBlockHeadPointerHL(), memoryArea);
    ok &= SafeMath::IsEqual(testDevice.CheckPointer(), false);
    testDevice.SetPointerCharacteristics(10u, sizeof(uint32), 10u);
    ok &= SafeMath::IsEqual(testDevice.CheckPointer(), true);
    return ok;
}

bool UEIBufferPointerTest::FunctionalTest() {
    bool ok = true;
    UEIBufferPointerHL testDevice;
    uint32 memoryArea [12u] = { 0x517bdf07, 0x2d3a19eb, 0x9e17bd7e, 0xcfb6eb67, 
                                0xc16cc4be, 0x937b65dd, 0xd372dcde, 0x939a24dd, 
                                0x93bd3455, 0x55523bd7, 0x41ea8e19, 0xf9849926 };
    //In this test we do try to access the same memory area with different parameters
    ok &= testDevice.SetHead(reinterpret_cast<uint8*>(memoryArea));
    //Access them as uint32 normal buffer
    testDevice.SetPointerCharacteristics(0u, sizeof(uint32), 12u);
    for (uint32 i = 0; i < 12u && ok; i++){
        uint32 thisvalue = *(reinterpret_cast<uint32*>(testDevice.GetSample(i)));
        ok &= SafeMath::IsEqual(thisvalue , memoryArea[i]);
    }
    //Test the overrun condition;
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint32*>(testDevice.GetSample(12))) , memoryArea[0]);
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint32*>(testDevice.GetSample(13))) , memoryArea[0]);
    //Try to access with an offset
    testDevice.SetPointerCharacteristics(2u*sizeof(uint32), sizeof(uint32), 10u);
    for (uint32 i = 0; i < 10u && ok; i++){
        uint32 thisvalue = *(reinterpret_cast<uint32*>(testDevice.GetSample(i)));
        ok &= SafeMath::IsEqual(thisvalue , memoryArea[i+2]);
    }
    //Test the overrun condition;
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint32*>(testDevice.GetSample(12))) , memoryArea[2]);
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint32*>(testDevice.GetSample(13))) , memoryArea[2]);
    //Try to access the same memory are as uint8
    testDevice.SetPointerCharacteristics(0u, sizeof(uint8), 48u);
    for (uint32 i = 0; i < 48u && ok; i++){
        uint8 thisvalue = *(reinterpret_cast<uint8*>(testDevice.GetSample(i)));
        ok &= SafeMath::IsEqual(thisvalue , reinterpret_cast<uint8*>(memoryArea)[i]);
    }

    //Test the overrun condition;
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint8*>(testDevice.GetSample(48))) , reinterpret_cast<uint8*>(memoryArea)[0]);
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint8*>(testDevice.GetSample(49))) , reinterpret_cast<uint8*>(memoryArea)[0]);
    //Try to access the same memory are as uint8 interleaved
    testDevice.SetPointerCharacteristics(sizeof(uint8)*8, 4*sizeof(uint8), 6u);
    for (uint32 i = 0; i < 6u && ok; i++){
        uint8 thisvalue = *(reinterpret_cast<uint8*>(testDevice.GetSample(i)));
        ok &= SafeMath::IsEqual(thisvalue , (uint8)(0x000000FF & memoryArea[i+2])); //This implementation only works in big-endian mode
    }
    //Test the overrun condition;
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint8*>(testDevice.GetSample(6))) , reinterpret_cast<uint8*>(memoryArea)[8]);
    ok &= SafeMath::IsEqual(*(reinterpret_cast<uint8*>(testDevice.GetSample(7))) , reinterpret_cast<uint8*>(memoryArea)[8]);
    return ok;
}