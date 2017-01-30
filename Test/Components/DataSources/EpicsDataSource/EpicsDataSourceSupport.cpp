/**
 * @file EpicsOutputDataSourceTest.cpp
 * @brief Source file for class EpicsDataSourceSupport
 * @date 30/01/2017
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

 * @details This source file contains the definition of all the methods for
 * the class EpicsDataSourceSupport (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EpicsDataSourceSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SetConfiguredDatabase(MARTe::DataSourceI& target, const MARTe::uint32 numberOfSignals) {
	using namespace MARTe;
    bool success = false;
    ConfigurationDatabase cdb;

    cdb.CreateRelative("Signals");

    for (uint32 i = 0; i < numberOfSignals; i++) {
    	StreamString idx;
    	StreamString name;
    	const char type[] = "uint32";

    	idx.Printf("%d", i);
    	name.Printf("Signal_%d", i);

    	cdb.CreateRelative(idx.Buffer());
    	cdb.Write("QualifiedName", name);
    	cdb.Write("Type", type);
    	cdb.Write("ByteSize", (TypeDescriptor::GetTypeDescriptorFromTypeName(type).numberOfBits / 8));
    	cdb.Write("NumberOfDimensions", 0);
    	cdb.Write("NumberOfElements", 1);

    	cdb.MoveToAncestor(1);
    }

    cdb.MoveToRoot();

    StreamString toPrintf;
    toPrintf.Printf("%!\n", cdb);

    success = target.SetConfiguredDatabase(cdb);

    return success;
}
