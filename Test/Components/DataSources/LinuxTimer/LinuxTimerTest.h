/**
 * @file LinuxTimerTest.h
 * @brief Header file for class LinuxTimerTest
 * @date 25/10/2016
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

 * @details This header file contains the declaration of the class LinuxTimerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINUXTIMERTEST_H_
#define LINUXTIMERTEST_H_

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
 * @brief Tests the LinuxTimerTest public methods.
 */
class LinuxTimerTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the GetSignalMemoryBuffer method with an invalid signal index.
     */
    bool TestGetSignalMemoryBuffer_False();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with Busy sleep.
     */
    bool TestExecute_Busy();

    /**
     * @brief Tests the Execute method with Busy sleep and SleepPercentage.
     */
    bool TestExecute_Busy_SleepPercentage();

    /**
     * @brief Tests the Execute method in the context of the real-time thread.
     */
    bool TestExecute_RTThread();

	/**
	* @brief Tests the Execute method in the context of the real-time thread by using all the five signals
	*/
	bool TestExecute_RTThread_WithFive();

	/**
	* @brief Tests the Execute method exercising the phase section
	*/
	bool TestExecute_RTThread_WithPhase();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method with an empty StructuredDataI.
     */
    bool TestInitialise_Empty();

    /**
     * @brief Tests the Initialise method with a Default SleepNature.
     */
    bool TestInitialise_Default();

    /**
     * @brief Tests the Initialise method  with a Busy SleepNature.
     */
    bool TestInitialise_Busy();

    /**
     * @brief Tests the Initialise method  with a Busy SleepNature and specifying the SleepPercentage.
     */
    bool TestInitialise_Busy_SleepPercentage();

    /**
     * @brief Tests the Initialise method  with a Busy SleepNature and specifying a SleepPercentage > 100.
     */
    bool TestInitialise_Busy_SleepPercentage_gt_100();

    /**
     * @brief Tests the Initialise method  with a CPUMask.
     */
    bool TestInitialise_CPUMask();

    /**
     * @brief Tests the Initialise method  with a StackSize.
     */
    bool TestInitialise_StackSize();

    /**
     * @brief Tests the Initialise method with an invalid SleepNature.
     */
    bool TestInitialise_False_SleepNature();

    /**
     * @brief Tests the Initialise method with an invalid ExecutionMode.
     */
    bool TestInitialise_False_ExecutionMode();

    /**
     * @brief Tests the Initialise method with an invalid StackSize.
     */
    bool TestInitialise_False_StackSize();

    /**
     * @brief Tests the Initialise method by explicitly specifying the Time Provider class
     */
    bool TestInitialise_ExplicitTimeProvider();

    /**
     * @brief Tests the Initialise method by explicitly specifying the Time Provider class but specifying an invalid Class for it
     */
    bool TestInitialise_False_ExplicitWrongTimeProvider();

    /**
     * @brief Tests the TestGetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the TestGetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method with only one signal specified on each GAM.
     */
    bool TestSetConfiguredDatabase_One_Signal_Per_GAM();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying more than four signals.
     */
    bool TestSetConfiguredDatabase_False_MoreThan5Signals();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a first signal that has more than 32 bits.
     */
    bool TestSetConfiguredDatabase_False_No32BitsSignal1();

	/**
	* @brief Tests the SetConfiguredDatabase method specifying a wrong attributes third signal
	*/
	bool TestSetConfiguredDatabase_False_InvalidSignal3();

	/**
	* @brief Tests the SetConfiguredDatabase method specifying a wrong attributes fourth signal
	*/
	bool TestSetConfiguredDatabase_False_InvalidSignal4();

	/**
	* @brief Tests the SetConfiguredDatabase method specifying a wrong attributes fifth signal
	*/
	bool TestSetConfiguredDatabase_False_InvalidSignal5();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a first signal that is not (Un)SignedInteged.
     */
    bool TestSetConfiguredDatabase_False_IntegerSignal1();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a second signal that has more than 32 bits.
     */
    bool TestSetConfiguredDatabase_False_No32BitsSignal2();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a second signal that is not (Un)SignedInteged.
     */
    bool TestSetConfiguredDatabase_False_IntegerSignal2();

    /**
     * @brief Tests the SetConfiguredDatabase without setting any frequency.
     */
    bool TestSetConfiguredDatabase_False_NoFrequencySet();

	/**
	* @brief Tests the SetConfiguredDatabase with more than one pluggable time provider
	*/
	bool TestSetConfiguredDatabase_False_MoreThan1Provider();

    /**
     * @brief Test the GetSleepPercentage method.
     */
    bool TestGetSleepPercentage();

    /**
    * @brief Tests an explicit HRT provider with busy sleep and percentage
    */
    bool TestSetConfiguredDatabase_UseBusySleepAndPercentage();

    /**
    * @brief Tests an explicit HRT provider with an invalid sleep nature
    */
    bool TestSetConfiguredDatabase_False_InvalidSleepNature();

    /**
    * @brief Tests an explicit HRT provider with a sleep percentage over 100%
    */
    bool TestSetConfiguredDatabase_ExplicitHRTWithMoreThan100Perc();

    /**
    * @brief Tests explicit HRT provider pure busy sleep
    */
    bool TestSetConfiguredDatabase_PureBusySleep();

    /**
    * @brief Tests explicit HRT provider with parameter injection from LinuxTimer
    */
    bool TestSetConfiguredDatabase_WithBackwardCompatOnHRT();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINUXTIMERTEST_H_ */

