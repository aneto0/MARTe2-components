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
#include "UEIMockupManager.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIMockupManager::UEIMockupManager() : ReferenceContainer() {
}

UEIMockupManager::~UEIMockupManager(){

}

bool UEIMockupManager::Initialise(StructuredDataI &data){
    bool ok = ReferenceContainer::Initialise(data);
    //Retrieve reference to mockup singleton
    mockup = UEIDAQMockupManager::getInstance();
    mockup->resetManager();
    ok &= mockup->GetIOM(1, &myIOM);
    myIOM->resetIOM();

    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve UEIMockupManager Object Name.");
        }
    }
    //Get connection timeout for the IOM if defiened, if not set default
    if (ok){
        uint32 nDevices;
        helper.ReadArray("Devices",Devices,nDevices);
        for (uint32 i = 0u; i < nDevices; i++){
            ok &= myIOM->SetDevice(i, i, 0u, 0u, Devices[i]);
        }
    }
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    //ok &= manager->registerCallbacks();
    // At this point, if ok is valid we've checked connection to the IOM, hardware configuration matching and device configuration
    return ok;
}

CLASS_REGISTER(UEIMockupManager, "1.0")
}