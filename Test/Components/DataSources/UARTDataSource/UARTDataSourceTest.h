/**
 * @file DataSourceTest.h
 * @brief Header file for class UARTDataSourceTest
 * @date 07/08/2018
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

 * @details This header file contains the declaration of the class CRIODataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_UARTDATASOURCE_UARTDATASOURCETEST_H_
#define TEST_COMPONENTS_DATASOURCES_UARTDATASOURCE_UARTDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the UARTDataSourceTest public methods.
 */
class UARTDataSourceTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests that the Initialise method fails if no PortName is specified.
     */
    bool TestInitialise_False_PortName();

    /**
     * @brief Tests that the Initialise method fails if no BaudRate is specified.
     */
    bool TestInitialise_False_BaudRate();

    /**
     * @brief Tests that the Initialise method fails if no Timeout is specified.
     */
    bool TestInitialise_False_Timeout();

    /**
     * @brief Tests that the Initialise method fails if no SerialTimeout is specified.
     */
    bool TestInitialise_False_SerialTimeout();

    /**
     * @brief Tests that the Initialise method fails if the port cannot be open.
     */
    bool TestInitialise_False_Port();

    /**
     * @brief Tests that the Initialise method fails if the baud cannot be set.
     */
    bool TestInitialise_False_Baud();

    /**
     * @brief Tests the Initialise method with the TimestampProvider specified in the configuration.
     */
    bool TestInitialise_TimestampProvider();

    /**
     * @brief Tests the Initialise method fails if the baud cannot be set.
     */
    bool TestInitialise_TooManyTimestampProviders();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests that the SetConfiguredDatabase method fails if the number of specified signals is not 2.
     */
    bool TestSetConfiguredDatabase_False_3_Signals();

    /**
     * @brief Tests that the SetConfiguredDatabase method fails if the first signal is not uint8.
     */
    bool TestSetConfiguredDatabase_False_Signal1_Not_UInt8();

    /**
     * @brief Tests that the SetConfiguredDatabase method fails if the first signal has not 1 element.
     */
    bool TestSetConfiguredDatabase_False_Signal1_Not_1_Element();

    /**
     * @brief Tests that the SetConfiguredDatabase method fails if the second signal is not uint64.
     */
    bool TestSetConfiguredDatabase_False_Signal2_Not_UInt64();

    /**
     * @brief Tests the GetInputOffset method.
     */
    bool TestGetInputOffset();

    /**
     * @brief Tests the PrepareInputOffsets method.
     */
    bool TestPrepareInputOffsets();

    /**
     * @brief Tests the TerminateInputCopy method.
     */
    bool TestTerminateInputCopy();

    /**
     * @brief Tests the CRIOThreadCallback method.
     */
    bool TestCRIOThreadCallback();

    /**
     * @brief Tests that the CRIOThreadCallback handles a bad UART read.
     */
    bool TestCRIOThreadCallback_FailedRead();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the StopAcquisition method.
     */
    bool TestStopAcquisition();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_UARTDATASOURCE_UARTDATASOURCETEST_H_ */

