/**
 * @file ConversionGAMTest.cpp
 * @brief Source file for class ConversionGAMTest
 * @date 21/01/2017
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
 * the class ConversionGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConversionGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Dummy DataSource to provide a fake interface to the ConversionGAMTest.
 */
class ConversionGAMDataSourceHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

ConversionGAMDataSourceHelper    () : MARTe::DataSourceI() {
    }

    virtual ~ConversionGAMDataSourceHelper() {

    }

    virtual bool AllocateMemory() {
        return true;
    }

    virtual MARTe::uint32 GetNumberOfMemoryBuffers() {
        return 1;
    }

    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx,
            const MARTe::uint32 bufferIdx,
            void *&signalAddress) {
        return true;
    }

    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data,
            const MARTe::SignalDirection direction) {
        if (direction == MARTe::InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(
            MARTe::ReferenceContainer &inputBrokers,
            const MARTe::char8* const functionName,
            void * const gamMemPtr) {
        MARTe::ReferenceT < MARTe::MemoryMapInputBroker > broker("MemoryMapInputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = inputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool GetOutputBrokers(
            MARTe::ReferenceContainer &outputBrokers,
            const MARTe::char8* const functionName,
            void * const gamMemPtr) {
        MARTe::ReferenceT < MARTe::MemoryMapOutputBroker > broker("MemoryMapOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = outputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool Synchronise() {
        return true;
    }

};

CLASS_REGISTER(ConversionGAMDataSourceHelper, "1.0");

CLASS_REGISTER(ConversionGAMTestHelper, "1.0");

//From uint8 using a Gain

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ConversionGAMTest::TestConstructor() {
    using namespace MARTe;
    ConversionGAM gam;
    return (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);
}

bool ConversionGAMTest::TestSetup() {
    return (TestExecute<MARTe::uint8>(255));
}

bool ConversionGAMTest::TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    if (destroy) {
        god->Purge();
    }
    return ok;
}

bool ConversionGAMTest::TestSetup_False_InvalidInputSignalType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.InputSignals");
    }
    if (ok) {
        StreamString signalName = "Signal1";
        ok = cdb.MoveRelative(signalName.Buffer());
        if (ok) {
            cdb.Delete("Type");
        }
        if (ok) {
            cdb.Write("Type", "string");
        }
        if (ok) {
            cdb.MoveToAncestor(1u);
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}

bool ConversionGAMTest::TestSetup_False_InvalidNumberOfInputsOutputs() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.InputSignals");
    }
    if (ok) {
        if (ok) {
            cdb.Delete("Signal1");
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}

bool ConversionGAMTest::TestSetup_False_InvalidInputElementsMismatch() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.InputSignals.Signal1");
    }
    if (ok) {
        if (ok) {
            cdb.Delete("NumberOfElements");
            cdb.Write("NumberOfElements", 10);
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}

bool ConversionGAMTest::TestSetup_False_InvalidInputSamplesMismatch() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.InputSignals.Signal1");
    }
    if (ok) {
        if (ok) {
            cdb.Delete("Samples");
            cdb.Write("Samples", 10);
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}

bool ConversionGAMTest::TestSetup_False_InvalidOutputElementsMismatch() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals.Signal1");
    }
    if (ok) {
        if (ok) {
            cdb.Delete("NumberOfElements");
            cdb.Write("NumberOfElements", 10);
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}

bool ConversionGAMTest::TestSetup_False_InvalidOutputSamplesMismatch() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals.Signal1");
    }
    if (ok) {
        if (ok) {
            cdb.Delete("Samples");
            cdb.Write("Samples", 10);
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}


