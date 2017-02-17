/**
 * @file PlatformTest.h
 * @brief Header file for class PlatformTest
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

 * @details This header file contains the declaration of the class PlatformTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGNALTEST_H_
#define SIGNALTEST_H_

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
 * @brief Class for testing Platform class.
 */
class PlatformTest {
public:

    /**
     * @brief Tests the MakeShm method.
     * @param[in] name The name of the shared area memory to create and use.
     * @param[in] fullname The absolute name of the file which holds the
     * shared area memory.
     */
    bool TestMakeShm(const char* const name,
                     const char* const fullname);

    /**
     * @brief Tests the JoinShm method.
     * @param[in] name The name of the shared area memory to create and use.
     * @param[in] fullname The absolute name of the file which holds the
     * shared area memory.
     */
    bool TestJoinShm(const char* const name,
                     const char* const fullname);

    /**
     * @brief Tests the DettachShm method.
     * @param[in] name The name of the shared area memory to create and use.
     * @param[in] fullname The absolute name of the file which holds the
     * shared area memory.
     */
    bool TestDettachShm(const char* const name,
                        const char* const fullname);

    /**
     * @brief Tests the DestroyShm method.
     * @param[in] name The name of the shared area memory to create and use.
     * @param[in] fullname The absolute name of the file which holds the
     * shared area memory.
     */
    bool TestDestroyShm(const char* const name,
                        const char* const fullname);

private:

    /**
     * @brief DataSet class meant for unit tests.
     */
    class DataSet {
    public:
        /**
         * @brief Default constructor.
         */
        DataSet();
        /**
         * @brief Gets a pointer to an immutable query token .
         */
        const char* GetQueryToken();
        /**
         * @brief Gets a pointer to an immutable response token .
         */
        const char* GetResponseToken();
        /**
         * @brief Get the token's length.
         */
        const size_t GetTokenLen();
    private:
        /**
         * The length of the token.
         */
        const size_t tokenlen;
        /*
         * An immutable forward token shared between all instances.
         */
        static const char QTOKEN[];
        /*
         * An immutable reverse token shared between all instances.
         */
        static const char RTOKEN[];
    };

    /**
     * @brief Test the interchange of data using the Platform methods between
     * a master and a slave tasks executing on two different processes.
     * @param[in] name The name of the shared area memory to create and use.
     * @param[in] fullname The absolute name of the file which holds the
     * shared area memory.
     */
    bool TestMasterSlaveWithTwoProcesses(const char* const name,
                                         const char* const fullname);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIGNALTEST_H_ */
