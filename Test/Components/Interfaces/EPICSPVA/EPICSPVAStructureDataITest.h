/**
 * @file EPICSPVStructureDataITest.h
 * @brief Header file for class EPICSPVStructureDataITest
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

 * @details This header file contains the declaration of the class EPICSPVStructureDataITest
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
     * @brief Tests the read with an uint8.
     */
    bool TestRead_UInt8();

    /**
     * @brief Tests the read with an uint16.
     */
    bool TestRead_UInt16();

    /**
     * @brief Tests the read with an uint32.
     */
    bool TestRead_UInt32();

    /**
     * @brief Tests the read with an uint64.
     */
    bool TestRead_UInt64();

    /**
     * @brief Tests the read with an int8.
     */
    bool TestRead_Int8();

    /**
     * @brief Tests the read with an int16.
     */
    bool TestRead_Int16();

    /**
     * @brief Tests the read with an int32.
     */
    bool TestRead_Int32();

    /**
     * @brief Tests the read with an int64.
     */
    bool TestRead_Int64();

    /**
     * @brief Tests the read with a float32.
     */
    bool TestRead_Float32();

    /**
     * @brief Tests the read with a float64.
     */
    bool TestRead_Float64();

    /**
     * @brief Tests the read with a StreamString.
     */
    bool TestRead_StreamString();

    /**
     * @brief Tests the read with a uint8[].
     */
    bool TestRead_UInt8_Array();

    /**
     * @brief Tests the read with a uint16[].
     */
    bool TestRead_UInt16_Array();

    /**
     * @brief Tests the read with a uint32[].
     */
    bool TestRead_UInt32_Array();

    /**
     * @brief Tests the read with a uint64[].
     */
    bool TestRead_UInt64_Array();

    /**
     * @brief Tests the read with a int8[].
     */
    bool TestRead_Int8_Array();

    /**
     * @brief Tests the read with a int16[].
     */
    bool TestRead_Int16_Array();

    /**
     * @brief Tests the read with a int32[].
     */
    bool TestRead_Int32_Array();

    /**
     * @brief Tests the read with a uint64[].
     */
    bool TestRead_Int64_Array();

    /**
     * @brief Tests the read with a float32[].
     */
    bool TestRead_Float32_Array();

    /**
     * @brief Tests the read with a float64[].
     */
    bool TestRead_Float64_Array();

    /**
     * @brief Tests the read with a StreamString[].
     */
    bool TestRead_StreamString_Array();
private:
    /**
     * @brief Tests the Read method with any of the supported template type.
     */
    template<typename T>
    bool TestRead(T wvalue);

    /**
     * @brief Tests the Read method with an array of any of the supported template type.
     */
    template<typename T>
    bool TestReadArray(MARTe::Vector<T> &wvalue);
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
    for (i=0; (i<wvalue.GetNumberOfElements()) && (ok); i++) {
        ok = (rvalue[i] == wvalue[i]);
    }
    return ok;

}
#endif /* EPICSPVASTRUCTUREDATAI_H_ */

