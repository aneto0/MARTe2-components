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
#include <cstdio>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EpicsDataSourceSupport.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool BuildConfigurationDatabase(MARTe::ConfigurationDatabase& cdb, const MARTe::uint32 numberOfSignals, const MARTe::uint32 numberOfFunctions) {
	using namespace MARTe;
    bool ok = true;

    ok = cdb.CreateRelative("Signals");

    for (uint32 i = 0; i < numberOfSignals; i++) {
    	const char type[] = "uint32";
    	StreamString idx;
    	StreamString name;

    	ok &= idx.Printf("%d", i);
    	ok &= name.Printf("Signal_%d", i);
    	{
    		ok &= cdb.CreateRelative(idx.Buffer());
    		ok &= cdb.Write("QualifiedName", name);
    		ok &= cdb.Write("Type", type);
    		ok &= cdb.Write("ByteSize", (TypeDescriptor::GetTypeDescriptorFromTypeName(type).numberOfBits / 8));
    		ok &= cdb.Write("NumberOfDimensions", 0);
    		ok &= cdb.Write("NumberOfElements", 1);
    		ok &= cdb.MoveToAncestor(1);
    	}
    }

    ok &= cdb.MoveToAncestor(1);

    ok &= cdb.CreateRelative("Functions");

    for (uint32 i = 0; i < numberOfFunctions; i++) {
    	const char type[] = "uint32";
    	StreamString idx;
    	StreamString name;

    	ok &= idx.Printf("%d", i);
    	ok &= name.Printf("Function_%d", i);

    	ok &= cdb.CreateRelative(idx.Buffer());
    	ok &= cdb.Write("QualifiedName", name);
    	ok &= cdb.Write("ByteSize", (TypeDescriptor::GetTypeDescriptorFromTypeName(type).numberOfBits / 8));
    	ok &= cdb.Write("GAMMemoryOffset", 0);

    	{
    		ok &= cdb.CreateRelative("InputSignals");
    		uint32 rawByteOffset[][2] = { { 0, 4 } };
    		Matrix<uint32> ByteOffset(rawByteOffset);
    		uint32 bytesize = 0;
    		for (uint32 i = 0; i < numberOfSignals; i++) {
    			StreamString idx;
    			StreamString name;

    			ok &= idx.Printf("%d", i);
    			ok &= name.Printf("Signal_%d", i);
    			{
    				ok &= cdb.CreateRelative(idx.Buffer());
    				ok &= cdb.Write("QualifiedName", name);
    				ok &= cdb.Write("Alias", name);
    				ok &= cdb.Write("Frequency", -1);
    				ok &= cdb.Write("Trigger", 0);
    				ok &= cdb.Write("Samples", 1);
    				ok &= cdb.Write("ByteOffset", ByteOffset);
    				ok &= cdb.Write("GAMMemoryOffset", 0);
    				ok &= cdb.Write("Broker", "MemoryMapSynchronisedInputBroker");
    				ok &= cdb.MoveToAncestor(1);
    			}
    			bytesize += (TypeDescriptor::GetTypeDescriptorFromTypeName(type).numberOfBits / 8);
    		}
    		ok &= cdb.Write("ByteSize", bytesize);
    		ok &= cdb.MoveToAncestor(1);
    	}

    	{
    		ok &= cdb.CreateRelative("OutputSignals");
    		uint32 rawByteOffset[][2] = { { 0, 4 } };
    		Matrix<uint32> ByteOffset(rawByteOffset);
    		uint32 bytesize = 0;
    		for (uint32 i = 0; i < numberOfSignals; i++) {
    			StreamString idx;
    			StreamString name;

    			ok &= idx.Printf("%d", i);
    			ok &= name.Printf("Signal_%d", i);
    			{
    				ok &= cdb.CreateRelative(idx.Buffer());
    				ok &= cdb.Write("QualifiedName", name);
    				ok &= cdb.Write("Alias", name);
    				ok &= cdb.Write("Frequency", -1);
    				ok &= cdb.Write("Trigger", 0);
    				ok &= cdb.Write("Samples", 1);
    				ok &= cdb.Write("ByteOffset", ByteOffset);
    				ok &= cdb.Write("GAMMemoryOffset", 0);
    				ok &= cdb.Write("Broker", "MemoryMapSynchronisedOutputBroker");
    				ok &= cdb.MoveToAncestor(1);
    			}
    			bytesize += (TypeDescriptor::GetTypeDescriptorFromTypeName(type).numberOfBits / 8);
    		}
    		ok &= cdb.Write("ByteSize", bytesize);
    		ok &= cdb.MoveToAncestor(1);
    	}
    	ok &= cdb.MoveToAncestor(1);
    }

    ok &= cdb.MoveToRoot();

    return ok;
}
