/**
 * @file MDSWriterNodeTest.h
 * @brief Header file for class MDSWriterNodeTest
 * @date 16/02/2017
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

 * @details This header file contains the declaration of the class MDSWriterNodeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSWRITER_MDSWRITERNODETEST_H_
#define MDSWRITER_MDSWRITERNODETEST_H_

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
 * @brief Tests the MDSWriterNode public methods.
 */
class MDSWriterNodeTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Test the Initialise without specifying a DecimatedNodeName
     */
    bool TestInitialise_NoDecimatedNodeName();

    /**
     * @brief Test the Initialise specifying a DecimatedNodeName
     */
    bool TestInitialise_DecimatedNodeName();

    /**
     * @brief Test the Initialise specifying a SamplePhase
     */
    bool TestInitialise_SamplePhase();

    /**
     * @brief Test the Initialise without specifying a NodeName
     */
    bool TestInitialise_False_NoNodeName();

    /**
     * @brief Test the Initialise without specifying a Type
     */
    bool TestInitialise_False_NoType();

    /**
     * @brief Test the Initialise specifying an invalid Type
     */
    bool TestInitialise_False_BadType();

    /**
     * @brief Test the Initialise without specifying the Period
     */
    bool TestInitialise_False_NoPeriod();

    /**
     * @brief Test the Initialise specifying an invalid Period
     */
    bool TestInitialise_False_BadPeriod();

    /**
     * @brief Test the Initialise without specifying the NumberOfElements
     */
    bool TestInitialise_False_NoNumberOfElements();

    /**
     * @brief Test the Initialise specifying an invalid NumberOfElements
     */
    bool TestInitialise_False_BadNumberOfElements();

    /**
     * @brief Test the Initialise without specifying the MakeSegmentAfterNWrites
     */
    bool TestInitialise_False_NoMakeSegmentAfterNWrites();

    /**
     * @brief Test the Initialise specifying an invalid MakeSegmentAfterNWrites
     */
    bool TestInitialise_False_BadMakeSegmentAfterNWrites();

    /**
     * @brief Test the Initialise without specifying the MinMaxResampleFactor
     */
    bool TestInitialise_False_NoMinMaxResampleFactor();

    /**
     * @brief Test the Initialise specifying an invalid MinMaxResampleFactor
     */
    bool TestInitialise_False_BadMinMaxResampleFactor();

    /**
     * @brief Test the Initialise with a uint16 type.
     */
    bool TestInitialise_Type_UInt16();

    /**
     * @brief Test the Initialise with a int16 type.
     */
    bool TestInitialise_Type_Int16();

    /**
     * @brief Test the Initialise with a uint32 type.
     */
    bool TestInitialise_Type_UInt32();

    /**
     * @brief Test the Initialise with a int32 type.
     */
    bool TestInitialise_Type_Int32();

    /**
     * @brief Test the Initialise with a uint64 type.
     */
    bool TestInitialise_Type_UInt64();

    /**
     * @brief Test the Initialise with a int32 type.
     */
    bool TestInitialise_Type_Int64();

    /**
     * @brief Test the Initialise with a float32 type.
     */
    bool TestInitialise_Type_Float32();

    /**
     * @brief Test the Initialise with a float64 type.
     */
    bool TestInitialise_Type_Float64();

#if 0
    /**
     * TODO
     */
    bool TestOpenTree();

    /**
     * TODO
     */
    bool TestIntegratedInApplication_NoTrigger();

    /**
     * TODO
     */
    bool TestIntegratedInApplication_NoTrigger_Flush();

    /**
     * TODO
     */
    bool TestIntegratedInApplication_Trigger();

    /**
     * TODO
     */
    bool TestIntegratedInApplication_NoTrigger_Elements();

    /**
     * TODO
     */
    bool TestIntegratedInApplication_Trigger_Elements();
#endif
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSWRITER_MDSWRITERNODETEST_H_ */

