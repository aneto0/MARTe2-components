/**
 * @file EPICSPVAStructureDataITest.h
 * @brief Header file for class EPICSPVAStructureDataITest
 * @date 13/06/2018
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

 * @details This header file contains the declaration of the class EPICSPVAStructureDataITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVASTRUCTUREDATAI_H_
#define EPICSPVASTRUCTUREDATAI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <pv/rpcClient.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EPICSPVAStructureDataI.h"
#include "EPICSPVAStructureDataITest.h"
#include "EPICSRPCClient.h"
#include "File.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the EPICSPVStructureDataI public methods.
 */
class EPICSPVAStructureDataITest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Read with an uint8.
     */
    bool TestRead_UInt8();

    /**
     * @brief Tests the Read with a boolean.
     */
    bool TestRead_Boolean();

    /**
     * @brief Tests the Read with an uint16.
     */
    bool TestRead_UInt16();

    /**
     * @brief Tests the Read with an uint32.
     */
    bool TestRead_UInt32();

    /**
     * @brief Tests the Read with an uint64.
     */
    bool TestRead_UInt64();

    /**
     * @brief Tests the Read with an int8.
     */
    bool TestRead_Int8();

    /**
     * @brief Tests the Read with an int16.
     */
    bool TestRead_Int16();

    /**
     * @brief Tests the Read with an int32.
     */
    bool TestRead_Int32();

    /**
     * @brief Tests the Read with an int64.
     */
    bool TestRead_Int64();

    /**
     * @brief Tests the Read with a float32.
     */
    bool TestRead_Float32();

    /**
     * @brief Tests the Read with a float64.
     */
    bool TestRead_Float64();

    /**
     * @brief Tests the Read with a StreamString.
     */
    bool TestRead_StreamString();

    /**
     * @brief Tests the Read with a uint8[].
     */
    bool TestRead_UInt8_Array();

    /**
     * @brief Tests the Read with a boolean[].
     */
    bool TestRead_Boolean_Array();

    /**
     * @brief Tests the Read with a uint16[].
     */
    bool TestRead_UInt16_Array();

    /**
     * @brief Tests the Read with a uint32[].
     */
    bool TestRead_UInt32_Array();

    /**
     * @brief Tests the Read with a uint64[].
     */
    bool TestRead_UInt64_Array();

    /**
     * @brief Tests the Read with a int8[].
     */
    bool TestRead_Int8_Array();

    /**
     * @brief Tests the Read with a int16[].
     */
    bool TestRead_Int16_Array();

    /**
     * @brief Tests the Read with a int32[].
     */
    bool TestRead_Int32_Array();

    /**
     * @brief Tests the Read with a uint64[].
     */
    bool TestRead_Int64_Array();

    /**
     * @brief Tests the Read with a float32[].
     */
    bool TestRead_Float32_Array();

    /**
     * @brief Tests the Read with a float64[].
     */
    bool TestRead_Float64_Array();

    /**
     * @brief Tests the Read with a StreamString[].
     */
    bool TestRead_StreamString_Array();

    /**
     * @brief Tests that the Read returns false if called before FinaliseStructure()
     */
    bool TestRead_False_FinaliseStructure();

    /**
     * @brief Tests that the Read returns false if called with a char *
     */
    bool TestRead_False_Char8String();

    /**
     * @brief Tests that the Read returns false if called with an invalid type
     */
    bool TestRead_False_InvalidType();

    /**
     * @brief Tests that the Read of an Array returns false if there is a dimension mismatch on the array size
     */
    bool TestRead_False_Array_Dimension();

    /**
     * @brief Tests that the Read of an array returns false if called with a char *
     */
    bool TestRead_False_Array_Char8String();

    /**
     * @brief Tests that the Read of an array returns false if called with an invalid type
     */
    bool TestRead_False_Array_InvalidType();

    /**
     * @brief Tests that the Read correctly converts between different types.
     */
    bool TestRead_DifferentTypes();

    /**
     * @brief Tests that the Read correctly converts from a numeric type to a string.
     */
    bool TestRead_Numeric_ToString();

    /**
     * @brief Tests that the Read correctly converts from a string to a numeric type.
     */
    bool TestRead_String_ToNumeric();

    /**
     * @brief Tests that the Read of an array correctly converts between different types.
     */
    bool TestRead_DifferentTypes_Array();

    /**
     * @brief Tests that the Read of an array correctly converts from a numeric type to a string.
     */
    bool TestRead_Numeric_ToString_Array();

    /**
     * @brief Tests that the Read of an array correctly converts from a string to a numeric type.
     */
    bool TestRead_String_ToNumeric_Array();

    /**
     * @brief Tests the GetType with an uint8.
     */
    bool TestGetType_UInt8();

    /**
     * @brief Tests the GetType with an uint16.
     */
    bool TestGetType_UInt16();

    /**
     * @brief Tests the GetType with an uint32.
     */
    bool TestGetType_UInt32();

    /**
     * @brief Tests the GetType with an uint64.
     */
    bool TestGetType_UInt64();

    /**
     * @brief Tests the GetType with an int8.
     */
    bool TestGetType_Int8();

    /**
     * @brief Tests the GetType with an int16.
     */
    bool TestGetType_Int16();

    /**
     * @brief Tests the GetType with an int32.
     */
    bool TestGetType_Int32();

    /**
     * @brief Tests the GetType with an int64.
     */
    bool TestGetType_Int64();

    /**
     * @brief Tests the GetType with a float32.
     */
    bool TestGetType_Float32();

    /**
     * @brief Tests the GetType with a float64.
     */
    bool TestGetType_Float64();

    /**
     * @brief Tests the GetType with a StreamString.
     */
    bool TestGetType_StreamString();

    /**
     * @brief Tests the GetType with a uint8[].
     */
    bool TestGetType_UInt8_Array();

    /**
     * @brief Tests the GetType with a uint16[].
     */
    bool TestGetType_UInt16_Array();

    /**
     * @brief Tests the GetType with a uint32[].
     */
    bool TestGetType_UInt32_Array();

    /**
     * @brief Tests the GetType with a uint64[].
     */
    bool TestGetType_UInt64_Array();

    /**
     * @brief Tests the GetType with a int8[].
     */
    bool TestGetType_Int8_Array();

    /**
     * @brief Tests the GetType with a int16[].
     */
    bool TestGetType_Int16_Array();

    /**
     * @brief Tests the GetType with a int32[].
     */
    bool TestGetType_Int32_Array();

    /**
     * @brief Tests the GetType with a uint64[].
     */
    bool TestGetType_Int64_Array();

    /**
     * @brief Tests the GetType with a float32[].
     */
    bool TestGetType_Float32_Array();

    /**
     * @brief Tests the GetType with a float64[].
     */
    bool TestGetType_Float64_Array();

    /**
     * @brief Tests the GetType with a StreamString[].
     */
    bool TestGetType_StreamString_Array();

    /**
     * @brief Tests that the GetType returns voidAnyType if called before FinaliseStructure.
     */
    bool TestGetType_False_FinaliseStructure();

    /**
     * @brief Tests that the GetType returns voidAnyType if called for a parameter that does not exist.
     */
    bool TestGetType_False_Parameter_Does_Not_Exist();

    /**
     * @brief Tests the Write method for an uint8 (note that the Read methods already call the Write for the cached version (i.e. pre FinaliseStructure).
     */
    bool TestWrite_UInt8();

    /**
     * @brief Tests the Write method for an uint16.
     */
    bool TestWrite_UInt16();

    /**
     * @brief Tests the Write method for an uint32.
     */
    bool TestWrite_UInt32();

    /**
     * @brief Tests the Write method for an uint64.
     */
    bool TestWrite_UInt64();

    /**
     * @brief Tests the Write method for an int8.
     */
    bool TestWrite_Int8();

    /**
     * @brief Tests the Write method for an int16.
     */
    bool TestWrite_Int16();

    /**
     * @brief Tests the Write method for an int32.
     */
    bool TestWrite_Int32();

    /**
     * @brief Tests the Write method for an int64.
     */
    bool TestWrite_Int64();

    /**
     * @brief Tests the Write method for a float32.
     */
    bool TestWrite_Float32();

    /**
     * @brief Tests the Write method for a float64.
     */
    bool TestWrite_Float64();

    /**
     * @brief Tests the Write method for a StreamString.
     */
    bool TestWrite_StreamString();

    /**
     * @brief Tests the Write method for a char8 *.
     */
    bool TestWrite_Char8String();

    /**
     * @brief Tests the Write method for an uint8[].
     */
    bool TestWrite_UInt8_Array();

    /**
     * @brief Tests the Write method for an uint16[].
     */
    bool TestWrite_UInt16_Array();

    /**
     * @brief Tests the Write method for an uint32[].
     */
    bool TestWrite_UInt32_Array();

    /**
     * @brief Tests the Write method for an uint64[].
     */
    bool TestWrite_UInt64_Array();

    /**
     * @brief Tests the Write method for an int8[].
     */
    bool TestWrite_Int8_Array();

    /**
     * @brief Tests the Write method for an int16[].
     */
    bool TestWrite_Int16_Array();

    /**
     * @brief Tests the Write method for an int32[].
     */
    bool TestWrite_Int32_Array();

    /**
     * @brief Tests the Write method for an int64[].
     */
    bool TestWrite_Int64_Array();

    /**
     * @brief Tests the Write method for a float32[].
     */
    bool TestWrite_Float32_Array();

    /**
     * @brief Tests the Write method for a float64[].
     */
    bool TestWrite_Float64_Array();

    /**
     * @brief Tests the Write method for a StreamString[].
     */
    bool TestWrite_StreamString_Array();

    /**
     * @brief Tests the Write method for a char*[].
     */
    bool TestWrite_Char8String_Array();

    /**
     * @brief Tests that the Write method fails if one attempts to write a scalar in a vector field.
     */
    bool TestWrite_False_Scalar_Vector();

    /**
     * @brief Tests that the Write method fails if one attempts to write a vector in a scalar field.
     */
    bool TestWrite_False_Vector_Scalar();

    /**
     * @brief Tests that the Write returns false if called with an invalid type
     */
    bool TestWrite_False_InvalidType();

    /**
     * @brief Tests that the Write of an array returns false if called with an invalid type
     */
    bool TestWrite_False_Array_InvalidType();

    /**
     * @brief Tests that the Write of an Array returns false if there is a dimension mismatch on the array size
     */
    bool TestWrite_False_Array_Dimension();

    /**
     * @brief Tests the Copy method.
     */
    bool TestCopy();

    /**
     * @brief Tests the Copy method with structures and arrays of structures.
     */
    bool TestCopy_Structures();

    /**
     * @brief Tests that the Copy method returns false if called before FinaliseStructure.
     */
    bool TestCopy_False_FinaliseStructure();

    /**
     * @brief Tests the MoveToRoot method.
     */
    bool TestMoveToRoot();

    /**
     * @brief Tests the MoveToAncestor method.
     */
    bool TestMoveToAncestor();

    /**
     * @brief Tests the MoveAbsolute method.
     */
    bool TestMoveAbsolute();

    /**
     * @brief Tests the MoveRelative method.
     */
    bool TestMoveRelative();

    /**
     * @brief Tests the MoveRelative method.
     */
    bool TestMoveToChild();

    /**
     * @brief Tests the CreateAbsolute method.
     */
    bool TestCreateAbsolute();

    /**
     * @brief Tests the CreateAbsolute method returns false if called after FinaliseStructure.
     */
    bool TestCreateAbsolute_False_FinaliseStructure();

    /**
     * @brief Tests the CreateRelative method.
     */
    bool TestCreateRelative();

    /**
     * @brief Tests that the CreateRelative method returns false if called after FinaliseStructure.
     */
    bool TestCreateRelative_False_FinaliseStructure();

    /**
     * @brief Tests the Delete method.
     */
    bool TestDelete();

    /**
     * @brief Tests that the Delete method returns false if called after FinaliseStructure.
     */
    bool TestDelete_False_FinaliseStructure();

    /**
     * @brief Tests the GetName method.
     */
    bool TestGetName();

    /**
     * @brief Tests that the GetName method returns an empty string if called before FinaliseStructure..
     */
    bool TestGetName_False_FinaliseStructure();

    /**
     * @brief Tests the GetChildName method.
     */
    bool TestGetChildName();

    /**
     * @brief Tests that the GetChildName method returns an empty string if called before FinaliseStructure..
     */
    bool TestGetChildName_False_FinaliseStructure();

    /**
     * @brief Tests the GetNumberOfChildren method.
     */
    bool TestGetNumberOfChildren();

    /**
     * @brief Tests that the GetNumberOfChildren method returns an empty string if called before FinaliseStructure..
     */
    bool TestGetNumberOfChildren_False_FinaliseStructure();

    /**
     * @brief Tests the SetStructure method.
     */
    bool TestSetStructure();

    /**
     * @brief Tests the InitStructure method.
     */
    bool TestInitStructure();

    /**
     * @brief Tests the FinaliseStructure method.
     */
    bool TestFinaliseStructure();

    /**
     * @brief Tests the GetRootStruct method.
     */
    bool TestGetRootStruct();

    /**
     * @brief Tests the IsStructureFinalised method.
     */
    bool TestIsStructureFinalised();

    /**
     * @brief Tests the CopyValuesFrom function.
     */
    bool TestCopyValuesFrom();

    /**
     * @brief Tests that the CopyValuesFrom function fails when.
     */
    bool TestCopyValuesFrom_False();

    /**
     * @brief Tests the performance.
     */
    bool TestPerformance();

    /**
     * @brief Tests the ToString method.
     */
    bool TestToString();

    /**
     * @brief Tests that the ToString method fails if FinaliseStructure is not called before.
     */
    bool TestToString_False();

private:
    /**
     * @brief Tests the Read method with any of the supported template types.
     */
    template<typename T>
    bool TestRead(T wvalue);

    /**
     * @brief Tests the Read method with an array of any of the supported template types.
     */
    template<typename T>
    bool TestReadArray(MARTe::Vector<T> &wvalue);

    /**
     * @brief Tests the GetType method with any of the supported template type.
     */
    template<typename T>
    bool TestGetType(T wvalue);

    /**
     * @brief Tests the GetType method with an array of any of the supported template type.
     */
    template<typename T>
    bool TestGetTypeArray(MARTe::Vector<T> &wvalue);

    /**
     * @brief Tests the Write method with any of the supported template types.
     */
    template<typename T>
    bool TestWrite(T wvalue1, T wvalue2);

    /**
     * @brief Tests the Write method with an array of any of the supported template types.
     */
    template<typename T>
    bool TestWriteArray(MARTe::Vector<T> &wvalue1, MARTe::Vector<T> &wvalue2);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool EPICSPVAStructureDataITest::TestRead(T wvalue) {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    T rvalue;
    test.Read("Test", rvalue);

    return (rvalue == wvalue);

}

template<typename T>
bool EPICSPVAStructureDataITest::TestReadArray(MARTe::Vector<T> &wvalue) {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<T> rvalue(wvalue.GetNumberOfElements());
    test.Read("Test", rvalue);
    bool ok = true;
    uint32 i;
    for (i = 0; (i < wvalue.GetNumberOfElements()) && (ok); i++) {
        ok = (rvalue[i] == wvalue[i]);
    }
    return ok;
}

template<typename T>
bool EPICSPVAStructureDataITest::TestGetType(T wvalue) {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue);
    test.FinaliseStructure();

    AnyType wat(wvalue);
    AnyType at = test.GetType("Test");
    bool ok = (at.GetTypeDescriptor() == wat.GetTypeDescriptor());
    if (!ok) {
        if (wat.GetTypeDescriptor().type == SString) {
            ok = (at.GetTypeDescriptor().type == BT_CCString);
        }
    }
    return ok;
}

template<typename T>
bool EPICSPVAStructureDataITest::TestGetTypeArray(MARTe::Vector<T> &wvalue) {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    AnyType wat(wvalue);
    AnyType at = test.GetType("Test");
    bool ok = (at.GetTypeDescriptor() == wat.GetTypeDescriptor());
    if (!ok) {
        if (wat.GetTypeDescriptor().type == SString) {
            ok = (at.GetTypeDescriptor().type == BT_CCString);
        }
    }
    if (ok) {
        ok = (at.GetNumberOfDimensions() == wat.GetNumberOfDimensions());
    }
    if (ok) {
        uint32 i;
        for (i = 0u; (i < 2u) && (ok); i++) {
            ok = (at.GetNumberOfElements(i) == wat.GetNumberOfElements(i));
        }
    }
    return ok;

}

template<typename T>
bool EPICSPVAStructureDataITest::TestWrite(T wvalue1, T wvalue2) {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    test.Write("Test", wvalue2);
    T rvalue;
    test.Read("Test", rvalue);

    return (rvalue == wvalue2);

}

template<typename T>
bool EPICSPVAStructureDataITest::TestWriteArray(MARTe::Vector<T> &wvalue1, MARTe::Vector<T> &wvalue2) {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    test.Write("Test", wvalue2);
    MARTe::Vector<T> rvalue(wvalue1.GetNumberOfElements());
    test.Read("Test", rvalue);
    bool ok = true;
    uint32 i;
    for (i = 0; (i < wvalue2.GetNumberOfElements()) && (ok); i++) {
        ok = (rvalue[i] == wvalue2[i]);
    }
    return ok;

}
#endif /* EPICSPVASTRUCTUREDATAI_H_ */

