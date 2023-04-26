/**
 * @file UEIMockupManager.cpp
 * @brief Source file for class UEIMockupManager
 * @date 22/03/2023
 * @author Xavier Ruche
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
 * the class UEIMockupManager (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIMapResponse.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
UEIMapResponse::UEIMapResponse() : Object() {
    for (uint32 i = 0u; i < 12u; i++){
        deviceResponses[i] = NULL_PTR(uint8*);
    }
    for (uint32 i = 0u; i < 12u; i++){
        responseLengths[i] = 0u;
    }
}

UEIMapResponse::~UEIMapResponse(){
    for (uint32 i = 0u; i < 12u; i++){
        if (deviceResponses[i] != NULL_PTR(uint8*)){
            delete [] deviceResponses[i];
        }
    }
}

bool UEIMapResponse::Initialise(StructuredDataI &data){
    bool ok = true;
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    if (ok){
        StreamString responseStr;
        ok &= data.Read("Response", responseStr);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "No Response parameter provided in map response");
        }else{
            if (responseStr == StreamString("SUCCESS")){
                response = DQ_SUCCESS;
            }else if (responseStr == StreamString("FIFO_OVERFLOW")){
                response = -1;
            }else{
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid response provided in Response parameter");
            }
        }
    }
    if (ok){
        StreamString dev = "dev";
        const char8* const numbersToChars [12] = {"0","1","2","3","4","5","6","7","8","9","10","11"};
        for (uint32 i = 0u; i < 12u; i++){
            StreamString thisDev = dev;
            thisDev += numbersToChars[i];
            helper.ReadArray(thisDev.Buffer(), deviceResponses[i], responseLengths[i]);
        }
    }
    return ok;
}
CLASS_REGISTER(UEIMapResponse, "1.0")
}