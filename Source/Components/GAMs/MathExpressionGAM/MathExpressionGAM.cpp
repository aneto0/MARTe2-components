/**
 * @file MathExpressionGAM.cpp
 * @brief Source file for class MathExpressionGAM
 * @date 12/06/2020
 * @author RFX
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

 * @details This source file contains the definition of all the methods for
 * the class MathExpressionGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MathExpressionGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
MathExpressionGAM::MathExpressionGAM() :
        GAM() {

}

MathExpressionGAM::~MathExpressionGAM() {

}

bool MathExpressionGAM::Initialise(StructuredDataI &data) {
    
    bool ok = GAM::Initialise(data);
    
    // Read the mathematical expression in infix form from the configuration file.
    //if (ok) {
        
        //ok = data.Read("Expression", expr);
        
        //if (ok) {
            //REPORT_ERROR(ErrorManagement::Information, "STRING: %s", expr.Buffer());
        //}
        //else {
            //REPORT_ERROR(ErrorManagement::FatalError, "Cannot find Expression among MathExpressionGAM parameters.");
        //}
    
    //}
    
    return ok;
    
}

bool MathExpressionGAM::Setup() {
    
    bool ok = false;
    
    StreamString errStr;
    
    expr.Seek(0);
    
    MathExpressionParser mathParser(expr, &errStr);
    
    bool parseOk = mathParser.Parse();
    
    REPORT_ERROR(ErrorManagement::Debug, "Parsed? %u", parseOk);
    
    REPORT_ERROR(ErrorManagement::Information, "\n%s", (mathParser.GetStackMachineExpression()).Buffer());
    
    
    return ok;
}

bool MathExpressionGAM::Execute() {
    
    return true;
    
}
CLASS_REGISTER(MathExpressionGAM, "1.0")
}

