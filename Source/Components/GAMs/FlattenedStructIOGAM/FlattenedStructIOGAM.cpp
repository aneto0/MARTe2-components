/**
 * @file FlattenedStructIOGAM.cpp
 * @brief Source file for class FlattenedStructIOGAM
 * @date 25/04/2022
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
 * the class FlattenedStructIOGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FlattenedStructIOGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
FlattenedStructIOGAM::FlattenedStructIOGAM() :
        GAM() {
    totalSignalsByteSize = 0u;
}

FlattenedStructIOGAM::~FlattenedStructIOGAM() {

}

bool FlattenedStructIOGAM::Initialise(StructuredDataI &data) {
    bool ret = data.MoveRelative("InputSignals");
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "The InputSignals shall be set");
    }
    if (ret) {
        ret = (data.GetNumberOfChildren() == 1u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Exactly one structured signal shall be set.");
        }
    }
    if (ret) {
        ret = data.MoveToChild(0u);
    }
    StreamString signalName;
    if (ret) {
        signalName = data.GetName();
    }
    uint32 numberOfElements = 1u;
    StreamString structType;
    if (ret) {
        if (!data.Read("NumberOfElements", numberOfElements)) {
            numberOfElements = 1u;
        }
        ret = data.Read("Type", structType);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The signal Type shall be set.");
        }
    }
    StreamString dataSourceName;
    if (ret) {
        ret = data.Read("DataSource", dataSourceName);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The signal DataSource shall be set.");
        }
    }
    if (ret) {
        ret = data.MoveToAncestor(1u);
    }
    if (ret) {
        ret = data.Delete(signalName.Buffer());
    }
    const Introspection *memberIntro = NULL_PTR(const Introspection *);
    const ClassRegistryItem *cri = NULL_PTR(const ClassRegistryItem *);
    if (ret) {
        cri = ClassRegistryDatabase::Instance()->Find(structType.Buffer());
        ret = (cri != NULL_PTR(const ClassRegistryItem *));
    }
    if (ret) {
        //lint -e{613} cri cannot be NULL as it is checked above.
        memberIntro = cri->GetIntrospection();
        ret = (memberIntro != NULL_PTR(const Introspection *));
    }
    ConfigurationDatabase signalPathCDB;
    if (ret) {
        ret = signalPathCDB.CreateAbsolute(signalName.Buffer());
    }
    if (ret) {
        ret = signalPathCDB.Write("NumberOfElements", numberOfElements);
    }
    if (ret) {
        uint32 signalCounter = 0u;
        ret = TransverseStructure(memberIntro, signalPathCDB, data, signalCounter, dataSourceName.Buffer());
    }
    if (ret) {
        ret = data.MoveToAncestor(1u);
    }
    if (ret) {
        ret = GAM::Initialise(data);
    }
    return ret;
}

bool FlattenedStructIOGAM::Setup() {
    bool ret = true;
    uint32 n;
    uint32 inTotalSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 inByteSize = 0u;
        uint32 inSamples = 1u;
        ret = GetSignalByteSize(InputSignals, n, inByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(InputSignals, n, inSamples);
        }
        if (ret) {
            inByteSize *= inSamples;
            inTotalSignalsByteSize += inByteSize;
        }
    }
    uint32 outTotalSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfOutputSignals()) && (ret); n++) {
        uint32 outByteSize = 0u;
        uint32 outSamples = 1u;
        ret = GetSignalByteSize(OutputSignals, n, outByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(OutputSignals, n, outSamples);
        }
        if (ret) {
            outByteSize *= outSamples;
            outTotalSignalsByteSize += outByteSize;
        }
    }
    if (ret) {
        ret = (inTotalSignalsByteSize == outTotalSignalsByteSize);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                         "GetSignalByteSize(InputSignals): %d != GetSignalByteSize(OutputSignals): %d",
                         inTotalSignalsByteSize, outTotalSignalsByteSize);
        }
    }
    if (ret) {
        totalSignalsByteSize = outTotalSignalsByteSize;
    }

    return ret;
}

bool FlattenedStructIOGAM::Execute() {
    return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
}

uint32 FlattenedStructIOGAM::GetNumberOfElements(const IntrospectionEntry &entry) const {
    uint32 numberOfElements = 1u;
    uint8 nOfDimensions = entry.GetNumberOfDimensions();
    if (nOfDimensions > 0u) {
        numberOfElements = 1u;
        uint32 ne = 0u;
        if (entry.GetMemberTypeDescriptor() == Character8Bit) {
            ne = 1u;
        }
        for (; ne < nOfDimensions; ne++) {
            uint32 nde = entry.GetNumberOfElements(ne);
            if (nde != 0u) {
                numberOfElements *= nde;
            }
        }
    }
    if (numberOfElements == 0u) {
        numberOfElements = 1u;
    }
    return numberOfElements;
}

bool FlattenedStructIOGAM::WriteSignal(ConfigurationDatabase &path, const char8 * const fullPathName, StructuredDataI &gamInputSignals, uint32 &signalCounter, const char8 * const dataSourceName) {
    bool ok = true;
    StreamString fullPathNameS = fullPathName;
    if (fullPathNameS.Size() == 0u) {
        fullPathNameS = path.GetName();
    }
    fullPathNameS += ".";
    uint32 numberOfChildren = path.GetNumberOfChildren();
    for(uint32 i=0u; (i<numberOfChildren) && (ok); i++) {
        bool isChildAStruct = path.MoveToChild(i); //Ignore the nodes like NumberOfElements
        if (isChildAStruct) {
            StreamString fullPathNameP = fullPathNameS;
            fullPathNameP += path.GetName();
            StreamString typeName;
            bool isStructured = !path.Read("Type", typeName);
            uint32 numberOfElements = 1u;
            (void) path.Read("NumberOfElements", numberOfElements);
            if (isStructured) {
                if (numberOfElements > 1u) {
                    for (uint32 j=0u; (j<numberOfElements) && (ok); j++) {
                        StreamString arrValue;
                        uint32 jj = j;
                        (void) arrValue.Printf("[%d]", jj);
                        StreamString tempPathName = fullPathNameP.Buffer();
                        tempPathName += arrValue.Buffer();
                        ok = WriteSignal(path, tempPathName.Buffer(), gamInputSignals, signalCounter, dataSourceName);
                    }
                }
                else {
                    ok = WriteSignal(path, fullPathNameP.Buffer(), gamInputSignals, signalCounter, dataSourceName);
                }
            }
            else {
                StreamString nName;
                (void)nName.Printf("S%d", signalCounter);
                ok = gamInputSignals.CreateRelative(nName.Buffer());
                if (ok) {
                    ok = gamInputSignals.Write("Type", typeName.Buffer());
                }
                if (ok) {
                    ok = gamInputSignals.Write("DataSource", dataSourceName);
                }
                if (ok) {
                    ok = gamInputSignals.Write("NumberOfElements", numberOfElements);
                }
                if (ok) {
                    ok = gamInputSignals.Write("Alias", fullPathNameP.Buffer());
                }
                if (ok) {
                    ok = gamInputSignals.MoveToAncestor(1u);
                }
                if (ok) {
                    signalCounter++;
                }
            }
            if (ok) {
                ok = path.MoveToAncestor(1u);
            }
        }
    }
    return ok;
}

/*lint -e{952} intro cannot be declared const*/
bool FlattenedStructIOGAM::TransverseStructure(const Introspection *intro, ConfigurationDatabase &signalPathCDB, StructuredDataI &gamInputSignals, uint32 &signalCounter, const char8 * const dataSourceName) {
    bool ok = (intro != NULL_PTR(const Introspection *));

    uint32 numberOfMembers = 0u;
    if (ok) {
        //lint -e{613} intro cannot be NULL as it is checked above.
        numberOfMembers = intro->GetNumberOfMembers();
    }
    uint32 i;
    //For each of the structure members...
    for (i = 0u; (i < numberOfMembers) && (ok); i++) {
        //lint -e{613} intro cannot be NULL as it is checked above.
        const IntrospectionEntry entry = intro->operator[](i);
        const char8 * const memberName = entry.GetMemberName();
        const char8 * const memberTypeName = entry.GetMemberTypeName();
        uint32 numberOfElements = GetNumberOfElements(entry);

        ok = signalPathCDB.CreateRelative(memberName);
        if (ok) {
            ok = signalPathCDB.Write("NumberOfElements", numberOfElements);
        }
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (ok) {
            if (isStructured) {
                const Introspection *memberIntro = NULL_PTR(const Introspection *);
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
                ok = (cri != NULL_PTR(const ClassRegistryItem *));
                if (ok) {
                    memberIntro = cri->GetIntrospection();
                    ok = (memberIntro != NULL_PTR(const Introspection *));
                }
                if (ok) {
                    ok = TransverseStructure(memberIntro, signalPathCDB, gamInputSignals, signalCounter, dataSourceName);
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Type %s is not registered", memberTypeName);
                }
            }
            else {
                ok = signalPathCDB.Write("Type", memberTypeName);
                ConfigurationDatabase cdbSignal = signalPathCDB;
                if (ok) {
                    ok = cdbSignal.MoveToRoot();
                }
                //Move to the first node. The cdbSignal should be a unique path.
                if (ok) {
                    ok = cdbSignal.MoveToChild(0u);
                }
                if (ok) {
                    ok = WriteSignal(cdbSignal, "", gamInputSignals, signalCounter, dataSourceName);
                }
            }
        }
        StreamString nodeName;
        if (ok) {
            nodeName = signalPathCDB.GetName();
            ok = signalPathCDB.MoveToAncestor(1u);
        }
        if (ok) {
            //Delete the leaf member.
            ok = signalPathCDB.Delete(nodeName.Buffer());
        }

    }
    return ok;
}

CLASS_REGISTER(FlattenedStructIOGAM, "1.0")
}

