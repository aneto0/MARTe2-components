/**
 * @file EPICSPVAInputTest.h
 * @brief Header file for class EPICSPVAInputTest
 * @date 23/10/2018
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

 * @details This header file contains the declaration of the class EPICSPVAInputTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVAINPUTTEST_H_
#define EPICSPVA_EPICSPVAINPUTTEST_H_

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
 * @brief Tests the EPICSPVAInput public methods.
 */
class EPICSPVAInputTest {
public:
    /**
     * @brief Purges the ObjectRegistryDatabase
     */
    virtual ~EPICSPVAInputTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with arrays.
     */
    bool TestExecute_Arrays();

    /**
     * @brief Tests the Execute method with structured types.
     */
    bool TestExecute_StructuredType();

    /**
     * @brief Tests the Execute method with structured types that contain arrays.
     */
    bool TestExecute_StructuredType_Arrays();

    /**
     * @brief Tests that the Execute method fails with CharString.
     */
    bool TestExecute_False_CharString();

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
     * @brief Tests that the GetBrokerName method correctly returns an empty string for OutputSignals.
     */
    bool TestGetBrokerName_OutputSignals();

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
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

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
     * @brief Tests that the Synchronise method returns true when the synchronise option is on.
     */
    bool TestSynchronise();

    /**
     * @brief Tests that the Synchronise method returns false.
     */
    bool TestGetBrokerNameSynchronised_too_many_channels();

    /**
     * @brief Tests that the PrepareNextState method returns true.
     */
    bool TestPrepareNextState();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVAINPUTTEST_H_ */
