/**
 * @file ConversioNHelperTTest.h
 * @brief Header file for class ConversionHelperTTest
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

 * @details This header file contains the declaration of the class ConversionHelperTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONVERSIONGAM_CONVERSIONHELPERTTEST_H_
#define CONVERSIONGAM_CONVERSIONHELPERTTEST_H_

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
 * @brief Tests the ConversionHelper/ConversionHelperT public methods.
 */
class ConversionHelperTTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Convert method.
     */
    bool TestConvert();

    /**
     * @brief Tests the LoadGain method.
     */
    bool TestLoadGain();

    /**
     * @brief Tests the GetInputMemory method.
     */
    bool TestGetInputMemory();

    /**
     * @brief Tests the GetNumberOfElements method.
     */
    bool TestGetNumberOfElements();

    /**
     * @brief Tests the SetNumberOfElements method.
     */
    bool TestSetNumberOfElements();

    /**
     * @brief Tests the GetNumberOfSamples method.
     */
    bool TestGetNumberOfSamples();

    /**
     * @brief Tests the SetNumberOfSamples method.
     */
    bool TestSetNumberOfSamples();

    /**
     * @brief Tests the GetOutputMemory method.
     */
    bool TestGetOutputMemory();

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONVERSIONGAM_CONVERSIONHELPERTTEST_H_ */

