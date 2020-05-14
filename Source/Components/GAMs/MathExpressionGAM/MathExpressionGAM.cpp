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
    if (ok) {
        
        ok = data.Read("Expression", expr);
        
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "STRING: %s", expr.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Cannot find Expression among MathExpressionGAM parameters.");
        }
    
    }
    
    return ok;
    
}

bool MathExpressionGAM::Setup() {
    
    bool ok = false;
    
    /// 1. Parser initialization
    expr.Seek(0);
    mathParser = new MathExpressionParser(expr, &errStr);
    ok = mathParser->Parse();
    
    // Debug printing
    if (ok) {
        REPORT_ERROR(ErrorManagement::Debug, "Parsed? %u", ok);
        REPORT_ERROR(ErrorManagement::Information, "\n%s", (mathParser->GetStackMachineExpression()).Buffer() );
    }
    
    /// 2. Evaluator initialization
    evaluator = new RuntimeEvaluator(mathParser->GetStackMachineExpression());
    ok = evaluator->ExtractVariables();
    
    /// 3. Types are assigned to variables based on input signal types
    uint32 index = 0U;
    VariableInformation *var;
    
    if (ok){

        while(evaluator->BrowseInputVariable(index,var)) {
            
            StreamString   signalName = "";
            TypeDescriptor signalType = InvalidType;
            
            for (uint32 signalIdx = 0u; signalIdx < numberOfInputSignals; signalIdx++) {
                
                ok = GetSignalName(InputSignals, signalIdx, signalName);
                if (signalName == var->name && ok) {
                    signalType = GetSignalType(InputSignals, signalIdx);
                    break;
                }
                signalName = "";
                
            }
            if (signalType != InvalidType) {
                evaluator->SetInputVariableType(index, signalType);
                printf("%s -> %s\n", (var->name).Buffer(), TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType));
            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not associate signal to variable %s", (var->name).Buffer());
                ok = false;
            }
            index++;
            
        }
        
    }
    
    if (ok) {
        
        index = 0;
        
        while(evaluator->BrowseOutputVariable(index,var)) {
            
            StreamString   signalName = "";
            TypeDescriptor signalType = InvalidType;
            
            for (uint32 signalIdx = 0u; signalIdx < numberOfOutputSignals; signalIdx++) {
                
                ok = GetSignalName(OutputSignals, signalIdx, signalName);
                if (signalName == var->name && ok) {
                    signalType = GetSignalType(OutputSignals, signalIdx);
                    break;
                }
                signalName = "";
                
            }
            
            if (signalType != InvalidType) {
                evaluator->SetOutputVariableType(index, signalType);
                printf("%s -> %s\n", (var->name).Buffer(), TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType));
            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not associate signal to variable %s", (var->name).Buffer());
                ok = false;
            }
            index++;
            
        }
    }
    
    /// 4. Compilation
    if (ok) {
        ok = evaluator->Compile();
    }
    
    return ok;
}

bool MathExpressionGAM::Execute() {
    
    bool ok = false;
    
    /// 1. Update values of input variables
    StreamString   signalName = "";
    for (uint32 signalIdx = 0u; signalIdx < numberOfInputSignals; signalIdx++) {
        
        GetSignalName(InputSignals, signalIdx, signalName);
        
        *((float32*)evaluator->GetInputVariableMemory(signalName)) = *((float32*)inputSignalsMemoryIndexer[signalIdx]);
        
        signalName = "";
    }
    
    /// 2. Execute
    ok = evaluator->Execute();
    
    /// 3. Update values of output signals
    for (uint32 signalIdx = 0u; signalIdx < numberOfOutputSignals; signalIdx++) {
        
        GetSignalName(OutputSignals, signalIdx, signalName);
        
        *((float32*)outputSignalsMemoryIndexer[signalIdx]) = *((float32*)evaluator->GetOutputVariableMemory(signalName));
        
        signalName = "";
    }
    
    return ok;
    
}

CLASS_REGISTER(MathExpressionGAM, "1.0")

} /* namespace MARTe */

