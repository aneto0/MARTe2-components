/**
 * @file CCSHelpersTest.cpp
 * @brief Source file for class CCSHelpersTest
 * @date 25/05/2022
 * @author Andre Neto
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
 * the class CCSHelpersTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <common/AnyValueHelper.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "AnyType.h"
#include "ConfigurationDatabase.h"
#include "CCSHelpersTest.h"
#include "IntrospectionStructure.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CCSHelpersTest::CCSHelpersTest() {
}

CCSHelpersTest::~CCSHelpersTest() {
}

bool CCSHelpersTest::TestCCSToMARTeStructuredDataI() {
    using namespace MARTe;
    ccs::types::CompoundType *inputType = new ccs::types::CompoundType("StructuredDataI_Type");
    ccs::types::AnyType *aType = new ccs::types::ArrayType("uint32Array", ccs::types::UnsignedInteger32, 2u);
    inputType->AddAttribute("A", aType);
    inputType->AddAttribute("B", ccs::types::UnsignedInteger16);

    ccs::types::CompoundType *cType = new ccs::types::CompoundType("CType");
    cType->AddAttribute("D", ccs::types::Float32);
    cType->AddAttribute("E", ccs::types::SignedInteger8);
    ccs::types::AnyType *sType = new ccs::types::ArrayType("StringArray", ccs::types::String, 2u);
    cType->AddAttribute("S", sType);
    inputType->AddAttribute("C", cType);

    struct CTypeStruct {
        float32 D;
        int8 E;
        char8 S[2][::ccs::types::MaxStringLength];
    } __attribute__((packed));

    struct InputTypeStruct {
        uint32 A[2];
        uint16 B;
        CTypeStruct C;
    } __attribute__((packed));

    ccs::types::AnyValue input(inputType);
    InputTypeStruct *mem = (InputTypeStruct*) input.GetInstance();
    mem->A[0] = 10u;
    mem->A[1] = 11u;

    mem->B = 2u;
    mem->C.D = 2.5;
    mem->C.E = -1;

    StringHelper::Copy(&mem->C.S[0][0], "STRING1");
    StringHelper::Copy(&mem->C.S[1][0], "STRING2");

    ConfigurationDatabase cdb;
    bool ret = CCSHelpers::CCSToMARTeStructuredDataI(cdb, input);
    if (ret) {
        Vector<uint32> aOut(2u);
        ret = cdb.Read("A", aOut);
        if(ret){
            ret = (aOut[0] == mem->A[0]);
            ret &= (aOut[1] == mem->A[1]);
        }
        if(ret){
            uint16 bOut;
            ret = cdb.Read("B", bOut);
            if(ret){
                ret = (bOut==mem->B);
            }
        }
        if(ret){
            ret = cdb.MoveRelative("C");
            if(ret){
                float32 dOut;
                ret = cdb.Read("D", dOut);
                if(ret){
                    ret=(dOut==mem->C.D);
                }
            }
            if(ret){
                int8 eOut;
                ret = cdb.Read("E", eOut);
                if(ret){
                    ret=(eOut==mem->C.E);
                }
            }
            if (ret) {
                Vector<StreamString> sOut(2u);
                ret = cdb.Read("S", sOut);
                if(ret){
                    ret = (sOut[0] == mem->C.S[0]);
                    ret &= (sOut[1] == mem->C.S[1]);
                }
            }
        }
    }
    return ret;
}

bool CCSHelpersTest::TestMARTeToCCSAnyValue() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A");
    cdb.Write("Type", "uint32");
    uint32 nElements0[] = {2};
    cdb.Write("NumberOfElements", nElements0); 
    cdb.MoveToAncestor(1u);

    cdb.CreateAbsolute("B");
    cdb.Write("Type", "int16");
    cdb.MoveToAncestor(1u);

    cdb.CreateAbsolute("C");
    cdb.Write("Type", "float32");
    cdb.MoveToAncestor(1u);

    cdb.CreateAbsolute("S");
    cdb.Write("Type", "char8");
    uint32 nElements[] = {2, 64};
    cdb.Write("NumberOfElements", nElements); 
    cdb.MoveToAncestor(1u);

    IntrospectionStructure type1;
    type1.SetName("Type1");
    type1.Initialise(cdb);

    ConfigurationDatabase cdb2;
    cdb2.CreateAbsolute("D");
    cdb2.Write("Type", "int32");
    cdb2.MoveToAncestor(1u);

    cdb2.CreateAbsolute("E");
    cdb2.Write("Type", "Type1");
    cdb2.MoveToAncestor(1u);

    IntrospectionStructure type2;
    type2.SetName("Type2");
    type2.Initialise(cdb2);

    ccs::types::AnyValue valOut;

    struct type1Struct {
        uint32 A[2];
        uint16 B;
        float32 C;
        char8 S[2][::ccs::types::MaxStringLength];
    } __attribute__((packed));

    struct type2Struct {
        int32 D;
        type1Struct E;
    } __attribute__((packed));

    type2Struct toConvertData;

    toConvertData.E.A[0] = 1u;
    toConvertData.E.A[1] = 2u;
    toConvertData.E.B = 2u;
    toConvertData.E.C = 1.5;

    toConvertData.D = -1;

    StringHelper::Copy(&toConvertData.E.S[0][0], "STRING1");
    StringHelper::Copy(&toConvertData.E.S[1][0], "STRING2");

    ClassRegistryItem *type2Item = ClassRegistryDatabase::Instance()->Find("Type2");
    bool ret = (type2Item != NULL);
    if (ret) {
        TypeDescriptor td(false, type2Item->GetClassProperties()->GetUniqueIdentifier());
        AnyType toConvert(td, 0u, (void*) &toConvertData);
        ccs::types::AnyValue valOut;

        ret = CCSHelpers::MARTeToCCSAnyValue(valOut, toConvert);

        if (ret) {
            uint32 *aOut = (uint32*) ccs::HelperTools::GetAttributeReference(&valOut, "E.A");
            uint16 *bOut = (uint16*) ccs::HelperTools::GetAttributeReference(&valOut, "E.B");
            float32 *cOut = (float32*) ccs::HelperTools::GetAttributeReference(&valOut, "E.C");
            int32 *dOut = (int32*) ccs::HelperTools::GetAttributeReference(&valOut, "D");
            
            ret &= (aOut[0] == toConvertData.E.A[0]);
            ret &= (aOut[1] == toConvertData.E.A[1]);
            ret &= (*bOut == toConvertData.E.B);
            ret &= (*cOut == toConvertData.E.C);
            ret &= (*dOut == toConvertData.D);

            type2Struct *mem = (type2Struct *) valOut.GetInstance();
            StreamString sOut0 = mem->E.S[0];
            StreamString sOut1 = mem->E.S[1];
            ret &= (sOut0 == "STRING1");
            ret &= (sOut1 == "STRING2");
        }
    }

    return ret;

}

bool CCSHelpersTest::TestMARTeToCCSAnyValue_StructuredDataI() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    uint32 a = 1u;
    cdb.Write("A", a);

    float32 b = 1.5;
    cdb.Write("B", b);

    cdb.CreateAbsolute("C");

    Vector < int16 > d(3u);
    d[0] = -1;
    d[1] = -2;
    d[2] = -3;

    cdb.Write("D", d);
    cdb.MoveToRoot();

    ccs::types::AnyValue valOut;
    bool ret = CCSHelpers::MARTeToCCSAnyValue(valOut, cdb);
    if (ret) {
        uint32 *aOut = (uint32*) ccs::HelperTools::GetAttributeReference(&valOut, "A");
        float32 *bOut = (float32*) ccs::HelperTools::GetAttributeReference(&valOut, "B");

        int16 *dOut = (int16*) ccs::HelperTools::GetAttributeReference(&valOut, "C.D");

        ret &= (a == *aOut);
        ret &= (b == *bOut);
        ret &= (d[0] == dOut[0]);
        ret &= (d[1] == dOut[1]);
        ret &= (d[2] == dOut[2]);
    }
    return ret;
}

bool CCSHelpersTest::TestGetMARTeBasicType() {
    using namespace MARTe;
    bool ret = CCSHelpers::GetMARTeBasicType(ccs::types::UnsignedInteger8) == UnsignedInteger8Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::UnsignedInteger16) == UnsignedInteger16Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::UnsignedInteger32) == UnsignedInteger32Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::UnsignedInteger64) == UnsignedInteger64Bit;

    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::SignedInteger8) == SignedInteger8Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::SignedInteger16) == SignedInteger16Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::SignedInteger32) == SignedInteger32Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::SignedInteger64) == SignedInteger64Bit;

    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::Float32) == Float32Bit;
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::Float64) == Float64Bit;

    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::String) == CharString;

    //missing boolean...
    ret &= CCSHelpers::GetMARTeBasicType(ccs::types::Boolean) == BooleanType;

    return ret;
}

bool CCSHelpersTest::TestCCSToMARTeAnyObject() {
    using namespace MARTe;
    ccs::types::AnyType *inputType = new ccs::types::ArrayType("uint32Array", ccs::types::UnsignedInteger32, 2u);

    ccs::types::AnyValue input(inputType);
    uint32 *mem = (uint32*) input.GetInstance();
    mem[0] = 10u;
    mem[1] = 11u;

    AnyObject valOut;
    bool ret = CCSHelpers::CCSToMARTeAnyObject(valOut, input);

    AnyType at = valOut.GetType();
    if (ret) {
        ret = !at.IsVoid();
    }
    if (ret) {
        ret = (at.GetNumberOfElements(0u) == 2u);
    }
    if (ret) {
        TypeDescriptor td = at.GetTypeDescriptor();
        ret = (td == UnsignedInteger32Bit);
    }
    if (ret) {
        uint32 *memOut = (uint32*) at.GetDataPointer();
        ret = (memOut[0] == mem[0]);
        ret &= (memOut[1] == mem[1]);
    }

    return ret;
}

bool CCSHelpersTest::TestCCSToMARTeAnyObject_StringArray() {
    using namespace MARTe;
    ccs::types::AnyType *inputType = new ccs::types::ArrayType("StringArray", ccs::types::String, 2u);

    ccs::types::AnyValue input(inputType);
    (void)::ccs::HelperTools::SafeStringCopy(
        static_cast<char8 *>(::ccs::HelperTools::GetElementReference(&input, 0)),
        "STRING1", ::ccs::types::MaxStringLength);
    (void)::ccs::HelperTools::SafeStringCopy(
        static_cast<char8 *>(::ccs::HelperTools::GetElementReference(&input, 1)),
        "STRING2", ::ccs::types::MaxStringLength);

    AnyObject valOut;
    bool ret = CCSHelpers::CCSToMARTeAnyObject(valOut, input);

    AnyType at = valOut.GetType();
    if (ret) {
        ret = !at.IsVoid();
    }
    if (ret) {
        ret = (at.GetNumberOfElements(0u) == 2u);
    }
    if (ret) {
        TypeDescriptor td = at.GetTypeDescriptor();
        ret = (td == CharString);
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Test", at);
        cdb.MoveToRoot();
        Vector<StreamString> ss(2);
        cdb.Read("Test", ss);
        ret = (ss[0] == "STRING1");
        ret &= (ss[1] == "STRING2");
    }

    return ret;
}
bool CCSHelpersTest::TestCCSToMARTeAnyObject_IntrospectedType() {
    using namespace MARTe;
    ccs::types::CompoundType *inputType = new ccs::types::CompoundType("IntrospectedType");

    ccs::types::AnyType *aType = new ccs::types::ArrayType("uint32Array", ccs::types::UnsignedInteger32, 2u);
    inputType->AddAttribute("A", aType);
    inputType->AddAttribute("B", ccs::types::UnsignedInteger16);

    ccs::types::CompoundType *cType = new ccs::types::CompoundType("CType");
    cType->AddAttribute("D", ccs::types::Float32);
    cType->AddAttribute("E", ccs::types::SignedInteger8);
    inputType->AddAttribute("C", cType);

    struct CTypeStruct {
        float32 D;
        int8 E;
    } __attribute__((packed));

    struct InputTypeStruct {
        uint32 A[2];
        uint16 B;
        CTypeStruct C;
    } __attribute__((packed));

    ccs::types::AnyValue input(inputType);
    InputTypeStruct *mem = (InputTypeStruct*) input.GetInstance();
    mem->A[0] = 10u;
    mem->A[1] = 11u;

    mem->B = 2u;
    mem->C.D = 2.5;
    mem->C.E = -1;

    AnyObject valOut;
    bool ret = CCSHelpers::CCSToMARTeAnyObject(valOut, input);

    AnyType at = valOut.GetType();
    if (ret) {
        ret = !at.IsVoid();
    }
    if (ret) {
        InputTypeStruct *memOut = (InputTypeStruct*) at.GetDataPointer();
        ret = (memOut->A[0] == mem->A[0]);
        ret &= (memOut->A[1] == mem->A[1]);
        ret &= (memOut->B == mem->B);
        ret &= (memOut->C.D == mem->C.D);
        ret &= (memOut->C.E == mem->C.E);
    }
    return ret;
}

bool CCSHelpersTest::TestGetCCSBasicType() {
    using namespace MARTe;
    ccs::types::AnyType *at = CCSHelpers::GetCCSBasicType(UnsignedInteger8Bit, 0u, 1u);
    bool ret = (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::UnsignedInteger8);
    }

    at = CCSHelpers::GetCCSBasicType(UnsignedInteger16Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::UnsignedInteger16);
    }

    at = CCSHelpers::GetCCSBasicType(UnsignedInteger32Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::UnsignedInteger32);
    }

    at = CCSHelpers::GetCCSBasicType(UnsignedInteger64Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::UnsignedInteger64);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger8Bit, 0u, 1u);
    ret = (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::SignedInteger8);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger16Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::SignedInteger16);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger32Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::SignedInteger32);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger64Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::SignedInteger64);
    }

    at = CCSHelpers::GetCCSBasicType(Float32Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::Float32);
    }

    at = CCSHelpers::GetCCSBasicType(Float64Bit, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::Float64);
    }

    at = CCSHelpers::GetCCSBasicType(BooleanType, 0u, 1u);
    ret &= (at != NULL);
    if (ret) {
        ret = (*at == *ccs::types::Boolean);
    }

    //array
    at = CCSHelpers::GetCCSBasicType(UnsignedInteger8Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("uint8Array", ccs::types::UnsignedInteger8, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(UnsignedInteger16Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("uint16Array", ccs::types::UnsignedInteger16, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(UnsignedInteger32Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("uint32Array", ccs::types::UnsignedInteger32, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(UnsignedInteger64Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("uint64Array", ccs::types::UnsignedInteger64, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger8Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("int8Array", ccs::types::SignedInteger8, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger16Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("int16Array", ccs::types::SignedInteger16, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger32Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("int32Array", ccs::types::SignedInteger32, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(SignedInteger64Bit, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("int64Array", ccs::types::SignedInteger64, 2u);
        ret = (*at == test);
    }

    at = CCSHelpers::GetCCSBasicType(BooleanType, 1u, 2u);

    ret &= (at != NULL);
    if (ret) {
        ccs::types::ArrayType test("boolArray", ccs::types::Boolean, 2u);
        ret = (*at == test);
    }

    return ret;
}
