/**
 * @file FilterGAMTest.cpp
 * @brief Source file for class FilterGAMTest
 * @date Jan 30, 2017 TODO Verify the value and format of the date
 * @author codac TODO Verify the name and format of the author
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
 * the class FilterGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "FilterGAM.h"
#include "FilterGAMTest.h"
#include "Vector.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FilterGAMTest::FilterGAMTest() {
//Auto-generated constructor stub for FilterGAMTest

//TODO Verify if manual additions are needed here
}

FilterGAMTest::~FilterGAMTest() {
//Auto-generated destructor stub for FilterGAMTest

//TODO Verify if manual additions are needed here
}

bool FilterGAMTest::TestConstructor() {
    using namespace MARTe;
    FilterGAM gam;
    uint32 sizeMem = 3u;
    float32 *num = new float32[sizeMem];
    float32 *den = new float32[sizeMem];
    bool ok = true;

    for (uint32 i = 0; i < sizeMem; i++) {
        num[i] = 0;
        den[i] = 0;
    }

    //try to get the num and denominator vales of the filter
    ok &= !gam.GetNumCoeff(num);
    ok &= !gam.GetDenCoeff(den);

    //Check that the values are not changed
    for (uint32 i = 0; i < sizeMem; i++) {
        ok &= (num[i] == 0);
        ok &= (den[i] == 0);
    }

    //Check that the static gain is 0
    ok &= (0 == gam.GetStaticGain());
    //Check that the number of numerator coefficients is 0
    ok &= (0 == gam.GetNumberOfNumCoeff());
    // Idem denominator
    ok &= (0 == gam.GetNumberOfDenCoeff());
    //Check normalisation
    ok &= !gam.CheckNormalisation();

    return ok;
}

bool FilterGAMTest::TestInitialise() {
    using namespace MARTe;
    FilterGAM gam;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestNumberOfNumCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    Vector<float32> denVec(den, numberOfDenCoeff);

    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    ok &= (gam.GetNumberOfNumCoeff() == (numberOfNumCoeff));
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestValuesOfNumCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    Vector<float32> denVec(den, numberOfDenCoeff);

    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    float32 *numRet = new float32[numberOfNumCoeff];
    gam.GetNumCoeff(numRet);
    for (uint32 i = 0; i < numberOfNumCoeff; i++) {
        ok &= (num[i] == numRet[i]);
    }
    delete[] num;
    delete[] numRet;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialiseNum0() {
    using namespace MARTe;
    FilterGAM gam;
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("otherNames", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return !ok;
}

bool FilterGAMTest::TestNumberOfDenCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    Vector<float32> denVec(den, numberOfDenCoeff);

    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    ok &= (gam.GetNumberOfDenCoeff() == (numberOfDenCoeff));
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestValuesOfDenCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 2;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    den[1] = 2;
    Vector<float32> denVec(den, numberOfDenCoeff);

    float32 *denRet = new float32[numberOfDenCoeff];
    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    gam.GetDenCoeff(denRet);
    for (uint32 i = 0; i < numberOfDenCoeff; i++) {
        ok &= (den[i] == denRet[i]);
    }
    delete[] num;
    delete[] den;
    delete[] denRet;
    return ok;
}

bool FilterGAMTest::TestInitialiseDen0() {
    using namespace MARTe;
    FilterGAM gam;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);
    bool ok = config.Write("Num", numVec);
    ok &= gam.Initialise(config);
    ok &= (0 == gam.GetNumberOfDenCoeff());
    delete[] num;
    return !ok;
}

bool FilterGAMTest::TestStaticGain() {
    using namespace MARTe;
    FilterGAM gam;
    bool ok = true;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    float32 *den = new float32[numberOfDenCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    den[0] = 1;
    Vector<float32> numVec(num, numberOfNumCoeff);
    Vector<float32> denVec(den, numberOfDenCoeff);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    ok &= (1 == gam.GetStaticGain());
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestFailNormalise() {
    using namespace MARTe;
    FilterGAM gam;
    bool ok = true;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    float32 *den = new float32[numberOfDenCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    den[0] = 2;
    Vector<float32> numVec(num, numberOfNumCoeff);
    Vector<float32> denVec(den, numberOfDenCoeff);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= !gam.Initialise(config);
    ok &= !gam.CheckNormalisation();
    delete[] num;
    delete[] den;
    return ok;
}
