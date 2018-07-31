/**
 * @file NI9157DeviceTest.h
 * @brief Header file for class NI9157DeviceTest
 * @date 22/05/2018
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class NI9157DeviceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157DEVICETEST_H_
#define NI9157DEVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157Device.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the NI9157Device methods
 */
class NI9157DeviceTest {
public:

    /**
     * @brief Constructor
     */
    NI9157DeviceTest();

    /**
     * @brief Destructor
     */
    virtual ~NI9157DeviceTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the NI9157Device::Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the NI9157Device::Initialise method with parameter Open=1
     */
    bool TestInitialiseIsOpened();

    /**
     * @brief Tests the NI9157Device::Initialise mixing the variable types
     */
    bool TestInitialiseRandomConfig();

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if NiRioDeviceName is not specified
     */
    bool TestInitialise_FalseNoDeviceName();

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if NiRioGenFile is not specified
     */
    bool TestInitialise_FalseNoGenFile();

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if NiRioGenSignature is not specified
     */
    bool TestInitialise_FalseNoGenSignature();

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if the type id is not found in the variable
     * to be initialised
     */
    bool TestInitialise_FalseNoType();

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if the type id is not found in the variable
     * is not found in the Labview project
     */
    bool TestInitialise_FalseVariableNotFound();

    /**
     * @brief Tests the NI9157Device::Open method
     */
    bool TestOpen();

    /**
     * @brief Tests the NI9157Device::Run method
     */
    bool TestRun();

    /**
     * @brief Tests the NI9157Device::IsOpened method
     */
    bool TestIsOpened();

    /**
     * @brief Tests the NI9157Device::IsRunning method
     */
    bool TestIsRunning();

    /**
     * @brief Tests the NI9157Device::GetSession method
     */
    bool TestGetSession();

    /**
     * @brief Tests the NI9157Device::Reset method
     */
    bool TestReset();

    /**
     * @brief Tests the NI9157Device::Close method
     */
    bool TestClose();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICETEST_H_ */

