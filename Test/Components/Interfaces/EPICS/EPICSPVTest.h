/**
 * @file EPICSPVTest.h
 * @brief Header file for class EPICSPVTest
 * @date 25/03/2017
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

 * @details This header file contains the declaration of the class EPICSPVTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVTEST_H_
#define EPICSPVTEST_H_

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
 * @brief Tests the EPICSPV public methods.
 */
class EPICSPVTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method with no event and uint32 PVType.
     */
    bool TestInitialise_NoEvent_UInt32();

    /**
     * @brief Tests the Initialise method with no event and int32 PVType.
     */
    bool TestInitialise_NoEvent_Int32();

    /**
     * @brief Tests the Initialise method with no event and float32 PVType.
     */
    bool TestInitialise_NoEvent_Float32();

    /**
     * @brief Tests the Initialise method with no event and float64 PVType.
     */
    bool TestInitialise_NoEvent_Float64();

    /**
     * @brief Tests the Initialise method with no event and string PVType.
     */
    bool TestInitialise_NoEvent_String();

    /**
     * @brief Tests the Initialise method with no event and with a non-default timeout.
     */
    bool TestInitialise_NoEvent_Timeout();

    /**
     * @brief Tests the Initialise method without specifying the PV name.
     */
    bool TestInitialise_False_PVName();

    /**
     * @brief Tests the Initialise method without specifying the PV type.
     */
    bool TestInitialise_False_PVType();

    /**
     * @brief Tests the Initialise method specifying an invalid PV type.
     */
    bool TestInitialise_False_PVType_Invalid();

    /**
     * @brief Tests the Initialise method specifying a zero timeout.
     */
    bool TestInitialise_False_ZeroTimeout();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function.
     */
    bool TestInitialise_Event_Function();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=FunctionMap.
     */
    bool TestInitialise_Event_FunctionMap();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Parameter.
     */
    bool TestInitialise_Event_Parameter();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Parameter.
     */
    bool TestInitialise_Event_ParameterName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Ignore.
     */
    bool TestInitialise_Event_Ignore();

    /**
     * @brief Tests the Initialise method with an Event and without specifying an invalid PVValue.
     */
    bool TestInitialise_Event_False_PVValue();

    /**
     * @brief Tests the Initialise method with an Event and specifying an invalid PVValue.
     */
    bool TestInitialise_Event_False_PVValue_Invalid();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function and specifying the function.
     */
    bool TestInitialise_Event_False_Function_AndFunctionName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Parameter and without specifying the function.
     */
    bool TestInitialise_Event_False_Parameter_NoFunctionName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Ignore and without specifying the function.
     */
    bool TestInitialise_Event_False_Ignore_NoFunctionName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function specifying Function and a FunctionMap.
     */
    bool TestInitialise_Event_False_Function_And_FunctionMap();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function specifying a FunctionMap that is not a matrix.
     */
    bool TestInitialise_Event_False_FunctionMap_NotMatrix();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function specifying a FunctionMap that is a matrix with more than two columns.
     */
    bool TestInitialise_Event_False_FunctionMap_MatrixNotTwoColumns();

    /**
     * @brief Tests the Initialise method with an Event and PVValue!=Function specifying FunctionMap.
     */
    bool TestInitialise_Event_False_NotFunction_And_FunctionMap();

    /**
     * @brief Tests the Initialise method with an Event without specifying the Destination.
     */
    bool TestInitialise_Event_False_NoDestination();

    /**
     * @brief Tests the SetContext method.
     */
    bool TestSetContext();

    /**
     * @brief Tests the GetContext method.
     */
    bool TestGetContext();

    /**
     * @brief Tests the HandlePVEvent method calling a function with no parameter.
     */
    bool TestHandlePVEvent_Function_NoParameter();

    /**
     * @brief Tests the HandlePVEvent method calling a function with no parameter to a destination that does not exits.
     */
    bool TestHandlePVEvent_Function_NoParameter_False();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a function map.
     */
    bool TestHandlePVEvent_FunctionMap();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a function map with a key that does not exist.
     */
    bool TestHandlePVEvent_FunctionMap_NoKey();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an int parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Int();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an uint parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_UInt();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a float32 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Float32();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a float64 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Float64();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a string parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_String();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an int parameter.
     */
    bool TestHandlePVEvent_Function_ParameterName_Int();

    /**
     * @brief Tests the HandlePVEvent method calling a function in Ignore mode.
     */
    bool TestHandlePVEvent_Function_Ignore();

    /**
     * @brief Tests the GetPVName method.
     */
    bool TestGetPVName();

    /**
     * @brief Tests the GetPVChid method.
     */
    bool TestGetPVChid();

    /**
     * @brief Tests the SetPVChid method.
     */
    bool TestSetPVChid();

    /**
     * @brief Tests the GetPVEvid method.
     */
    bool TestGetPVEvid();

    /**
     * @brief Tests the SetPVEvid method.
     */
    bool TestSetPVEvid();

    /**
     * @brief Tests the GetPVType method.
     */
    bool TestGetPVType();

    /**
     * @brief Tests the GetMode method.
     */
    bool TestGetMode();

    /**
     * @brief Tests the GetTimeout method.
     */
    bool TestGetTimeout();

    /**
     * @brief Tests the GetDestination method.
     */
    bool TestGetDestination();

    /**
     * @brief Tests the GetFunction method.
     */
    bool TestGetFunction();

    /**
     * @brief Tests the GetFunctionFromMap method.
     */
    bool TestGetFunctionFromMap();

    /**
     * @brief Tests the CAPut (and CAGet) method with an int32.
     */
    bool TestCAPut_Int32();

    /**
     * @brief Tests the CAPut (and CAGet) method with an uint32.
     */
    bool TestCAPut_UInt32();

    /**
     * @brief Tests the CAPut (and CAGet) method with a float32.
     */
    bool TestCAPut_Float32();

    /**
     * @brief Tests the CAPut (and CAGet) method with a float64.
     */
    bool TestCAPut_Float64();

    /**
     * @brief Tests the CAPut (and CAGet) method with a string.
     */
    bool TestCAPut_String();

    /**
     * @brief Tests the CAGet method.
     */
    bool TestCAGet();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVTEST_H_ */

