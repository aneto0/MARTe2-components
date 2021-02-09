/**
 * @file EPICSCAOutputTest.h
 * @brief Header file for class EPICSCAOutputTest
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class EPICSCAOutputTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSCA_EPICSCAOUTPUTTEST_H_
#define EPICSCA_EPICSCAOUTPUTTEST_H_

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
 * @brief Tests the EPICSCAOutput public methods.
 */
class EPICSCAOutputTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests that the GetBrokerName method correctly returns a MemoryMapInputBroker.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests that the GetBrokerName method correctly returns an empty string for InputSignals.
     */
    bool TestGetBrokerName_InputSignals();

    /**
     * @brief Tests the GetInputBrokers method
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with defaults
     */
    bool TestInitialise_Defaults();

    /**
     * @brief Tests the Initialise method without specifying the Signals
     */
    bool TestInitialise_False_Signals();

    /**
     * @brief Tests the Initialise method without specifying the NumberOfBuffers
     */
    bool TestInitialise_False_NumberOfBuffers();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method without specifying any signals.
     */
    bool TestSetConfiguredDatabase_False_NoSignals();

    /**
     * @brief Tests the SetConfiguredDatabase method without specifying the signal PV name.
     */
    bool TestSetConfiguredDatabase_False_PVName();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying more than one sample.
     */
    bool TestSetConfiguredDatabase_False_Samples();

    /**
     * @brief Tests the SetConfiguredDatabase method with an unsupported type.
     */
    bool TestSetConfiguredDatabase_False_UnsupportedType();

    /**
     * @brief Tests the SetConfiguredDatabase method with more than one GAM interacting with the DataSourceI.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneGAM();

    /**
     * @brief Tests the SetConfiguredDatabase method with a string that does not have 40 bytes.
     */
    bool TestSetConfiguredDatabase_False_WrongStringSize();

    /**
     * @brief Tests that the PV values are correctly written by the DataSourceI
     */
    bool TestExecute();

    /**
     * @brief Tests that the PV values are correctly written by the DataSourceI when using arrays.
     */
    bool TestExecute_Arrays();

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the GetNumberOfBuffers method.
     */
    bool TestGetNumberOfBuffers();

    /**
     * @brief Tests the IsIgnoringBufferOverrun method.
     */
    bool TestIsIgnoringBufferOverrun();

    /**
     * @brief Tests the AsyncCaPut method.
     */
    bool TestAsyncCaPut();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSCA_EPICSCAOUTPUTTEST_H_ */

