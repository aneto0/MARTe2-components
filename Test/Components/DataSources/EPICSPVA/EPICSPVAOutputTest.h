/**
 * @file EPICSPVAOutputTest.h
 * @brief Header file for class EPICSPVAOutputTest
 * @date 22/10/2018
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

 * @details This header file contains the declaration of the class EPICSPVAOutputTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVAOUTPUTTEST_H_
#define EPICSPVA_EPICSPVAOUTPUTTEST_H_

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
 * @brief Tests the EPICSPVAOutput public methods.
 */
class EPICSPVAOutputTest {
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
     * @brief Tests the SetConfiguredDatabase method specifying more than one sample.
     */
    bool TestSetConfiguredDatabase_False_Samples();

    /**
     * @brief Tests the SetConfiguredDatabase method with more than one GAM interacting with the DataSourceI.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneGAM();

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
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the Synchronise method with arrays.
     */
    bool TestSynchronise_Arrays();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVAOUTPUTTEST_H_ */
