/**
 * @file ProfinetToMARTeLogAdapterTest.h
 * @brief Header file for class ProfinetToMARTeLogAdapterTest
 * @date 20/01/2021
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
 * @details This source file contains the definition of all the methods for
 * the class ProfinetToMARTeLogAdapterTest (public, protected, and private). 
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

#ifndef DATASOURCES_PROFINET_PROFINETTOMARTELOGADAPTERTEST_H_
#define DATASOURCES_PROFINET_PROFINETTOMARTELOGADAPTERTEST_H_

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
 * @brief Tests the ProfinetToMARTeLogAdapter public methods.
 */
class ProfinetToMARTeLogAdapterTest {
public:

    /**
     * @brief Tests the Constructor method.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Contructor minimum logger level parameter.
     */
    bool TestLoggerMinimumLevels();

    /**
     * @brief Tests the Log method using a message with size equals zero.
     */
    bool TestLoggerMessageZeroSize();

    /**
     * @brief Tests the Log method using a message with average size.
     */
    bool TestLoggerMessageAverageSize();

    /**
     * @brief Tests the Log method using a message with maximum size.
     */
    bool TestLoggerMessageMaximumSize();

    /**
     * @brief Tests the Log method using a message with size above the maximum.
     */
    bool TestLoggerMessageOverrunSize();

    /**
     * @brief Tests the Log method for Debug level.
     */
    bool TestLoggerMessageDebugLevel();

    /**
     * @brief Tests the Log method for Information level.
     */
    bool TestLoggerMessageInformationLevel();

    /**
     * @brief Tests the Log method for Warning level.
     */
    bool TestLoggerMessageWarningLevel();

    /**
     * @brief Tests the Log method for Error level.
     */
    bool TestLoggerMessageErrorLevel();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_PROFINETTOMARTELOGADAPTERTEST_H_ */
