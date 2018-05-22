/**
 * @file SigblockDoubleBufferTest.h
 * @brief Header file for class SigblockDoubleBufferTest
 * @date 31/01/2017
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class SigblockDoubleBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCKDOUBLEBUFFERTEST_H_
#define SIGBLOCKDOUBLEBUFFERTEST_H_

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
 * @brief Class for testing SigblockDoubleBuffer class.
 */
class SigblockDoubleBufferTest {
public:

    /**
     * @brief Tests the Init method.
     */
    bool TestInit();

    /**
     * @brief Tests the Get method.
     */
    bool TestGet();

    /**
     * @brief Tests the Put method.
     */
    bool TestPut();

private:

    /**
     * @brief Test the interchange of data between a producer and a consumer
     * using one single thread.
     */
    template<typename SignalType>
    bool TestProducerConsumerInSingleThread(const unsigned int numberOfSignals,
                                            const unsigned int maxTests);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIGBLOCKDOUBLEBUFFERTEST_H_ */
