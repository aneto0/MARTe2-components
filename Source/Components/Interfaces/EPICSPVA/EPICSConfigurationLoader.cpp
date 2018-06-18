/**
 * @file EPICSConfigurationLoader.cpp
 * @brief Source file for class EPICSConfigurationLoader
 * @date 18/06/2018
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

 * @details This source file contains the definition of all the methods for
 * the class EPICSConfigurationLoader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSConfigurationLoader.h"
#include "ObjectRegistryDatabase.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSConfigurationLoader::EPICSConfigurationLoader() :
        Object() {

}

EPICSConfigurationLoader::~EPICSConfigurationLoader() {

}

epics::pvData::PVStructurePtr EPICSConfigurationLoader::request(epics::pvData::PVStructure::shared_pointer const & args)
        throw (epics::pvAccess::RPCRequestException) {
    //ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    epics::pvData::FieldBuilderPtr fieldBuilder = epics::pvData::getFieldCreate()->createFieldBuilder();
    //GetEPICSStructure(fieldBuilder, *ord);
    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
    epics::pvData::PVStructurePtr result(epics::pvData::getPVDataCreate()->createPVStructure(topStructure));
    //FillEPICSStructure(result, *ord, "");
    args->dumpValue(std::cout);
    return result;
}

CLASS_REGISTER(EPICSConfigurationLoader, "1.0")
}
