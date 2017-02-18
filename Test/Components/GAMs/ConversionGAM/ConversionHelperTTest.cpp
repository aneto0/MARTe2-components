/**
 * @file ConversionHelperTTest.cpp
 * @brief Source file for class ConversionHelperTTest
 * @date 22/01/2017
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
 * the class ConversionHelperTTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "ConversionHelperT.h"
#include "ConversionHelperTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ConversionHelperTTest::TestConstructor() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    return true;
}

bool ConversionHelperTTest::TestConvert() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    test.Convert();
    return true;
}

bool ConversionHelperTTest::TestLoadGain() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    ConfigurationDatabase cdb;
    bool ok = !test.LoadGain(cdb);
    cdb.Write("Gain", 10);
    if (ok) {
        ok = test.LoadGain(cdb);
    }
    return ok;
}

bool ConversionHelperTTest::TestGetInputMemory() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    ConfigurationDatabase cdb;
    bool ok = (test.GetInputMemory() == NULL);
    if (ok) {
        ConversionHelperT<uint32, float32> test2(this, NULL);
        ok = (test2.GetInputMemory() != NULL);
        if (ok) {
            ok = (test2.GetOutputMemory() == NULL);
        }
    }
    return ok;
}

bool ConversionHelperTTest::TestGetNumberOfElements() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    bool ok = (test.GetNumberOfElements() == 0);
    if (ok) {
        test.SetNumberOfElements(10);
        ok = (test.GetNumberOfElements() == 10);
    }
    return ok;
}

bool ConversionHelperTTest::TestSetNumberOfElements() {
    return TestGetNumberOfElements();
}

bool ConversionHelperTTest::TestGetNumberOfSamples() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    bool ok = (test.GetNumberOfSamples() == 0);
    if (ok) {
        test.SetNumberOfSamples(10);
        ok = (test.GetNumberOfSamples() == 10);
    }
    return ok;
}

bool ConversionHelperTTest::TestSetNumberOfSamples() {
    return TestGetNumberOfSamples();
}

bool ConversionHelperTTest::TestGetOutputMemory() {
    using namespace MARTe;
    ConversionHelperT<uint32, float32> test(NULL, NULL);
    ConfigurationDatabase cdb;
    bool ok = (test.GetOutputMemory() == NULL);
    if (ok) {
        ConversionHelperT<uint32, float32> test2(NULL, this);
        ok = (test2.GetOutputMemory() != NULL);
        if (ok) {
            ok = (test2.GetInputMemory() == NULL);
        }
    }
    return ok;
}
