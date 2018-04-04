/**
 * @file ConstantGAMTest.h
 * @brief Header file for class ConstantGAMTest
 * @date 22/03/2018
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class ConstantGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONSTANTGAMTEST_H_
#define CONSTANTGAMTEST_H_

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
 * @brief Tests the ConstantGAM public methods.
 */
class ConstantGAMTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    ConstantGAMTest();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConstantGAMTest();

    /**
     * @brief Tests the default constructor
     * @return true after GAM instantiation.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute() method
     * @return true if Execute succeeds.
     */
    bool TestExecute();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() succeeds.
     */
    bool TestSetup();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() succeeds.
     */
    bool TestSetup_OutputSignal_int8();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_Error_InputSignal();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_Error_NoOutputSignal();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_Error_NoDefault();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and succeeds.
     */
    bool TestSetOutput_int8();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and succeeds.
     */
    bool TestSetOutput_1Darray();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and succeeds.
     */
    bool TestSetOutput_2Darray();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and succeeds.
     */
    bool TestSetOutput_WithIndex();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and failss.
     */
    bool TestSetOutput_Error_NoName();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and failss.
     */
    bool TestSetOutput_Error_InvalidName();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and failss.
     */
    bool TestSetOutput_Error_NoValue();

    /**
     * @brief Tests the SetOutput() method
     * @details Verify the post-conditions
     * @return true if SetOutput is invoked() and failss.
     */
    bool TestSetOutput_Error_InvalidValue();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONSTANTGAMTEST_H_ */

