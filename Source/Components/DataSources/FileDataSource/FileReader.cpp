/**
 * @file FileReader.cpp
 * @brief Source file for class FileReader
 * @date 11/08/2017
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
 * the class FileReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "Directory.h"
#include "FileReader.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
static const int32 FILE_FORMAT_BINARY = 1;
static const int32 FILE_FORMAT_CSV = 2;

FileReader::FileReader() :
        DataSourceI(), MessageI() {
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);
    numberOfBinaryBytes = 0u;
    xAxisSignal = 0u;
    xAxisSignalIdx = 0u;
    xAxisSignalPtr = NULL_PTR(uint64 *);
    interpolationPeriod = 0u;
    xAxisSignalType = InvalidType;
    fileFormat = FILE_FORMAT_BINARY;
    filename = "";
    fatalFileError = false;
    interpolate = false;
    interpolatedInputBroker = NULL_PTR(MemoryMapInterpolatedInputBroker *);
    signalsAnyType = NULL_PTR(AnyType *);
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} -e{1579} the destructor must guarantee that the memory is freed and the file is flushed and closed.. The brokerAsyncTrigger is freed by the ReferenceT */
FileReader::~FileReader() {
    if (dataSourceMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
    }
    if (signalsAnyType != NULL_PTR(AnyType *)) {
        delete[] signalsAnyType;
    }
    (void) CloseFile();
}

bool FileReader::AllocateMemory() {
    return true;
}

uint32 FileReader::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool FileReader::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (dataSourceMemory != NULL_PTR(char8 *));
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction and on the storeOnTrigger property (which is load before).*/
const char8* FileReader::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == InputSignals) {
        if (interpolate) {
            brokerName = "MemoryMapInterpolatedInputBroker";
        }
        else {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
    }
    return brokerName;
}

bool FileReader::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    bool ok = true;
    if (interpolate) {
        ReferenceT<MemoryMapInterpolatedInputBroker> brokerNew("MemoryMapInterpolatedInputBroker");
        interpolatedInputBroker = brokerNew.operator ->();
        ok = interpolatedInputBroker->Init(InputSignals, *this, functionName, gamMemPtr);
        if (ok) {
            ok = GetSignalMemoryBuffer(xAxisSignalIdx, 0u, xAxisSignalPtr);
            if (ok) {
                interpolatedInputBroker->SetIndependentVariable(&xAxisSignal, interpolationPeriod);
            }
        }
        if (ok) {
            ok = inputBrokers.Insert(brokerNew);
        }
        if (ok) {
            //Read the first line so that the Broker can be correctly initialised.
            ok = Synchronise();
        }
        if (ok) {
            interpolatedInputBroker->Reset();
        }
    }
    else {
        ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
        if (ok) {
            ok = inputBrokers.Insert(broker);
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: OutputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool FileReader::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

/*lint -e{613} xAxisSignalPtr cannot be NULL as otherwise SetConfiguredDatabase would have failed.*/
void FileReader::ConvertXAxisSignal() {
    if (xAxisSignalType == UnsignedInteger8Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<uint8 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == SignedInteger8Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<int8 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == UnsignedInteger16Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<uint16 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == SignedInteger16Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<int16 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == UnsignedInteger32Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<uint32 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == SignedInteger32Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<int32 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == UnsignedInteger64Bit) {
        xAxisSignal = *reinterpret_cast<uint64 *>(xAxisSignalPtr);
    }
    else if (xAxisSignalType == SignedInteger64Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<int64 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == Float32Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<float32 *>(xAxisSignalPtr));
    }
    else if (xAxisSignalType == Float64Bit) {
        xAxisSignal = static_cast<uint64>(*reinterpret_cast<float64 *>(xAxisSignalPtr));
    }
    else {
        //Unreachable...
    }
}

bool FileReader::Synchronise() {
    bool ok = !fatalFileError;
    if (ok) {
        bool lockAtLast = false;
        if (inputFile.Position() == inputFile.Size()) {
            if (eofBehaviour == EOFRewind) {
                if (fileFormat == FILE_FORMAT_BINARY) {
                    const uint32 SIGNAL_NAME_MAX_SIZE = 32u;
                    uint32 headerSize = static_cast<uint32>(sizeof(uint16));
                    headerSize += SIGNAL_NAME_MAX_SIZE;
                    headerSize += static_cast<uint32>(sizeof(uint32));
                    headerSize *= GetNumberOfSignals();
                    headerSize += static_cast<uint32>(sizeof(uint32));
                    ok = inputFile.Seek(0LLU);
                    if (ok) {
                        ok = inputFile.Seek(static_cast<uint64>(headerSize));
                    }
                }
                else {
                    ok = inputFile.Seek(0LLU);
                    if (ok) {
                        StreamString header;
                        //Skip the header
                        ok = inputFile.GetLine(header);
                    }
                }
            }
            else if (eofBehaviour == EOFLast) {
                lockAtLast = true;
            }
            else {
                //Unreachable by design.
            }
        }
        if (!lockAtLast) {
            if (fileFormat == FILE_FORMAT_BINARY) {
                uint32 readSize = numberOfBinaryBytes;
                ok = inputFile.Read(dataSourceMemory, readSize);
                if (ok) {
                    ok = (readSize == numberOfBinaryBytes);
                }
            }
            else {
                StreamString token;
                char8 saveTerminator;
                uint32 nSignals = GetNumberOfSignals();
                uint32 signalIdx = 0u;
                StreamString line;
                ok = inputFile.GetLine(line);
                if (ok) {
                    ok = line.Seek(0LLU);
                }
                if (ok) {
                    ok = line.GetToken(token, csvSeparator.Buffer(), saveTerminator);
                }
                /*lint -e{613} signalsAnyType cannot be NULL as otherwise SetConfiguredDatabase would have failed.*/
                while ((ok) && (signalIdx < nSignals)) {
                    bool isString = (signalsAnyType[signalIdx].GetTypeDescriptor() == CharString);
                    if (!isString) {
                        isString = (signalsAnyType[signalIdx].GetTypeDescriptor() == Character8Bit);
                    }
                    if ((signalsAnyType[signalIdx].GetNumberOfDimensions() == 1u) && (!isString)) {
                        uint32 nElements = signalsAnyType[signalIdx].GetNumberOfElements(0u);
                        StreamString tokenArray;
                        uint32 arrayIdx = 0u;
                        void *signalAddress = signalsAnyType[signalIdx].GetDataPointer();
                        char8 *signalAddressChr = reinterpret_cast<char8 *>(signalAddress);
                        if (token[static_cast<uint32>(token.Size()) - 1u] != '}') {
                            token += csvSeparator;
                            ok = line.GetToken(token, "}", saveTerminator);
                            if (ok) {
                                token += saveTerminator;
                            }
                        }
                        if (ok) {
                            ok = token.Seek(0LLU);
                        }
                        if (ok) {
                            ok = token.GetToken(tokenArray, "{},", saveTerminator);
                        }
                        while ((ok) && (arrayIdx < nElements)) {
                            AnyType sourceStr(CharString, 0u, tokenArray.Buffer());
                            uint32 byteSize = signalsAnyType[signalIdx].GetByteSize();
                            uint32 arrayIdxByteSize = (arrayIdx * byteSize);
                            AnyType destination(signalsAnyType[signalIdx].GetTypeDescriptor(), 0u, &signalAddressChr[arrayIdxByteSize]);
                            ok = TypeConvert(destination, sourceStr);
                            tokenArray = "";
                            arrayIdx++;
                            if (ok) {
                                ok = token.GetToken(tokenArray, "{},", saveTerminator);
                            }
                        }
                        ok = (arrayIdx == nElements);
                        if (!ok) {
                            StreamString signalName;
                            (void) GetSignalName(signalIdx, signalName);
                            REPORT_ERROR(ErrorManagement::FatalError, "Inconsistent number of elements found in array of signal %s. [%s]", signalName.Buffer(), line.Buffer());
                        }
                    }
                    else {
                        if (!isString) {
                            AnyType sourceStr(CharString, 0u, token.Buffer());
                            ok = TypeConvert(signalsAnyType[signalIdx], sourceStr);
                        }
                        else {
                            ok = StringHelper::CopyN(reinterpret_cast<char8 *>(signalsAnyType[signalIdx].GetDataPointer()), token.Buffer(), signalsAnyType[signalIdx].GetNumberOfElements(0u));
                        }
                    }
                    signalIdx++;
                    token = "";
                    if (ok) {
                        if (signalIdx != nSignals) {
                            ok = line.GetToken(token, csvSeparator.Buffer(), saveTerminator);
                        }
                    }
                }
                if (ok) {
                    ok = (signalIdx == nSignals);
                    if (!ok) {
                        StreamString signalName;
                        (void) GetSignalName(signalIdx, signalName);
                        REPORT_ERROR(ErrorManagement::FatalError, "Inconsistent number of signals found [%s]", line.Buffer());
                    }
                }
            }
            fatalFileError = !ok;
            if (fatalFileError) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed to read from file. No more attempts will be performed.");
                if (fileRuntimeErrorMsg.IsValid()) {
                    //Reset any previous replies
                    fileRuntimeErrorMsg->SetAsReply(false);
                    if (!MessageI::SendMessage(fileRuntimeErrorMsg, this)) {
                        StreamString destination = fileRuntimeErrorMsg->GetDestination();
                        StreamString function = fileRuntimeErrorMsg->GetFunction();
                        REPORT_ERROR(ErrorManagement::FatalError, "Could not send TreeRuntimeError message to %s [%s]", destination.Buffer(), function.Buffer());
                    }
                }
            }
        }
        if (ok) {
            if (interpolate) {
                ConvertXAxisSignal();
            }
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool FileReader::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool FileReader::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("FileFormat", fileFormatStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "FileFormat shall be specified");
        }
    }
    if (ok) {
        if (fileFormatStr == "csv") {
            fileFormat = FILE_FORMAT_CSV;
        }
        else if (fileFormatStr == "binary") {
            fileFormat = FILE_FORMAT_BINARY;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Invalid FileFormat specified");
        }
    }
    if (ok) {
        if (fileFormat == FILE_FORMAT_CSV) {
            ok = data.Read("CSVSeparator", csvSeparator);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "FileFormat=csv and CSVSeparator was not specified");
            }
        }
    }
    if (ok) {
        ok = data.Read("Filename", filename);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Filename shall be specified.");
        }
    }
    if (ok) {
        StreamString interpolateStr;
        ok = data.Read("Interpolate", interpolateStr);
        if (ok) {
            if (interpolateStr == "yes") {
                interpolate = true;
            }
            else if (interpolateStr == "no") {
                interpolate = false;
            }
            else {
                ok = false;
            }
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Interpolate shall be specified as either yes or no");
        }
    }
    if (ok) {
        if (interpolate) {
            if (!data.Read("XAxisSignal", xAxisSignalName)) {
                REPORT_ERROR(ErrorManagement::Warning,
                             "Interpolate=yes and the XAxisSignal was not specified. This will fail if none of the signals interacting with this FileReader has Frequency > 0");
            }
            if (!data.Read("InterpolationPeriod", interpolationPeriod)) {
                REPORT_ERROR(ErrorManagement::Warning,
                             "Interpolate=yes and the InterpolationPeriod was not specified. This will fail if none of the signals interacting with this FileReader has Frequency > 0");
            }
        }
    }
    if (ok) {
        StreamString eofStr;
        if (data.Read("EOF", eofStr)) {
            if (eofStr == "Error") {
                eofBehaviour = EOFError;
            }
            else if (eofStr == "Rewind") {
                eofBehaviour = EOFRewind;
            }
            else if (eofStr == "Last") {
                eofBehaviour = EOFLast;
            }
            else {
                ok = false;
            }
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "EOF set to %s", eofStr.Buffer());
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unknown EOF %s", eofStr.Buffer());
            }
        }
        else {
            eofBehaviour = EOFRewind;
            REPORT_ERROR(ErrorManagement::Information, "EOF set to Rewind");
        }
    }
    if (ok) {
        //Check if there are any Message elements set
        if (Size() > 0u) {
            ReferenceT<ReferenceContainer> msgContainer = Get(0u);
            if (msgContainer.IsValid()) {
                uint32 j;
                uint32 nOfMessages = msgContainer->Size();
                for (j = 0u; (j < nOfMessages) && (ok); j++) {
                    ReferenceT<Message> msg = msgContainer->Get(j);
                    ok = msg.IsValid();
                    if (ok) {
                        StreamString msgName = msg->GetName();
                        if (msgName == "FileRuntimeError") {
                            fileRuntimeErrorMsg = msg;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Message %s is not supported.", msgName.Buffer());
                            ok = false;
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Found an invalid Message in container %s", msgContainer->GetName());
                        ok = false;
                    }

                }
            }
        }
    }
    if (ok) {
        ok = signalsDatabase.MoveRelative("Signals");
        if (!ok) {
            ok = signalsDatabase.CreateRelative("Signals");
        }
        if (ok) {
            ok = (OpenFile(signalsDatabase) == ErrorManagement::NoError);
        }
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
    }

    return ok;
}

bool FileReader::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
    }

    //Check signal properties and compute memory
    numberOfBinaryBytes = 0u;
    if (ok) {
        //Do not allow samples
        uint32 functionNumberOfSignals = 0u;
        uint32 n;
        if (GetFunctionNumberOfSignals(InputSignals, 0u, functionNumberOfSignals)) {
            for (n = 0u; (n < functionNumberOfSignals) && (ok); n++) {
                uint32 nSamples;
                ok = GetFunctionSignalSamples(InputSignals, 0u, n, nSamples);
                if (ok) {
                    ok = (nSamples == 1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
                }
            }
        }

        uint32 nOfSignals = GetNumberOfSignals();
        if (ok) {
            ok = (nOfSignals > 0u);
            offsets = new uint32[nOfSignals];
        }

        //Count the number of bytes
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            /*lint -e{613} offsets cannot be null as otherwise ok would be false*/
            offsets[n] = numberOfBinaryBytes;
            uint32 nBytes = 0u;
            ok = GetSignalByteSize(n, nBytes);
            numberOfBinaryBytes += nBytes;
        }
    }
    //Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI");
        }
    }
    float32 frequency = -1.0F;
    if (ok) {
        uint32 nOfFunctionSignals = 0u;
        ok = GetFunctionNumberOfSignals(InputSignals, 0u, nOfFunctionSignals);

        if (ok) {
            uint32 i;
            bool found = false;
            for (i = 0u; (i < nOfFunctionSignals) && (ok) && (!found); i++) {
                ok = GetFunctionSignalReadFrequency(InputSignals, 0u, i, frequency);
                if (ok) {
                    found = ((frequency > 0.F) && (frequency <= 1e9));
                }
                if (found) {
                    /*lint -e{9122}  frequencies > 1e9 protected above.*/
                    interpolationPeriod = static_cast<uint64>(1e9 / frequency);
                    xAxisSignalName = "";
                    ok = GetFunctionSignalAlias(InputSignals, 0u, i, xAxisSignalName);
                }
            }
        }

    }
    //Look for the XAxisSignal
    if (interpolate) {
        ok = GetSignalIndex(xAxisSignalIdx, xAxisSignalName.Buffer());
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "XAxisSignal: %s was not found", xAxisSignalName.Buffer());
        }
        if (ok) {
            xAxisSignalType = GetSignalType(xAxisSignalIdx);
        }
        if (ok) {
            uint32 nElements;
            ok = GetSignalNumberOfElements(xAxisSignalIdx, nElements);
            if (ok) {
                ok = (nElements == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The XAxisSignal: %s cannot be a vector", xAxisSignalName.Buffer());
                }
            }
        }
    }

    //Allocate memory
    if (ok) {
        dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(numberOfBinaryBytes));
    }

    //If the type is text prepare the Printf properties in advanced
    if (fileFormat == FILE_FORMAT_CSV) {
        uint32 nOfSignals = GetNumberOfSignals();
        uint32 n;
        if (ok) {
            signalsAnyType = new AnyType[nOfSignals];
        }

        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            uint8 nDimensions = 0u;
            uint32 nElements = 0u;
            ok = GetSignalNumberOfDimensions(n, nDimensions);
            if (ok) {
                ok = GetSignalNumberOfElements(n, nElements);
            }
            /*lint -e{613} signalsAnyType, dataSourceMemory and offsets cannot be null as otherwise ok would be false*/
            if (ok) {
                char8 *memPtr = &dataSourceMemory[offsets[n]];
                void *signalAddress = reinterpret_cast<void *&>(memPtr);
                signalsAnyType[n] = AnyType(GetSignalType(n), 0u, signalAddress);
                signalsAnyType[n].SetNumberOfDimensions(nDimensions);
                signalsAnyType[n].SetNumberOfElements(0u, nElements);
            }
        }
    }

    return ok;
}

ErrorManagement::ErrorType FileReader::OpenFile(StructuredDataI &cdb) {
    REPORT_ERROR(ErrorManagement::Information, "Going to open file with name %s", filename.Buffer());
    //File already exists!
    fatalFileError = !inputFile.Open(filename.Buffer(), (BasicFile::ACCESS_MODE_R));
    if (fatalFileError) {
        (void) inputFile.Close();
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to open File %s", filename.Buffer());
    }

    if (!fatalFileError) {
        uint32 n;
        uint32 nOfSignals;

        //Write the header
        if (fileFormat == FILE_FORMAT_CSV) {
            StreamString line;
            fatalFileError = !inputFile.GetLine(line);
            //Skip the #
            if (!fatalFileError) {
                fatalFileError = !line.Seek(1LLU);
            }

            char8 saveTerminator;
            StreamString token;
            while (line.GetToken(token, csvSeparator.Buffer(), saveTerminator) && (!fatalFileError)) {
                StreamString signalName;
                (void) token.Seek(0LLU);
                fatalFileError = !token.GetToken(signalName, " ", saveTerminator);

                StreamString signalTypeStr;
                if (!fatalFileError) {
                    fatalFileError = !token.GetToken(signalTypeStr, "()", saveTerminator);
                }

                StreamString signalElementsStr;
                if (!fatalFileError) {
                    fatalFileError = !token.GetToken(signalElementsStr, "[]", saveTerminator);
                }

                TypeDescriptor signalType;
                uint32 nElements = 0u;
                if (!fatalFileError) {
                    signalType = TypeDescriptor::GetTypeDescriptorFromTypeName(signalTypeStr.Buffer());
                    fatalFileError = (signalType == InvalidType);
                    if (fatalFileError) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Invalid signal type found %s:%s", signalName.Buffer(), signalTypeStr.Buffer());
                    }
                }
                if (!fatalFileError) {
                    AnyType src(CharString, 0u, signalElementsStr.Buffer());
                    AnyType dst(UnsignedInteger32Bit, 0u, &nElements);
                    fatalFileError = !TypeConvert(dst, src);
                }
                if (!fatalFileError) {
                    if (!cdb.CreateRelative(signalName.Buffer())) {
                        fatalFileError = !cdb.MoveRelative(signalName.Buffer());
                    }
                }
                if (!fatalFileError) {
                    if (!cdb.Write("Type", signalTypeStr.Buffer())) {
                        StreamString declaredType;
                        fatalFileError = !cdb.Read("Type", declaredType);
                        if (!fatalFileError) {
                            fatalFileError = (declaredType != signalTypeStr);
                        }
                    }
                }
                if (!fatalFileError) {
                    if (!cdb.Write("NumberOfElements", nElements)) {
                        uint32 declaredElements;
                        fatalFileError = !cdb.Read("NumberOfElements", declaredElements);
                        if (!fatalFileError) {
                            fatalFileError = (nElements != declaredElements);
                        }
                    }
                }
                if (nElements > 1u) {
                    if (!cdb.Write("NumberOfDimensions", 1u)) {
                        uint32 declaredDimensions;
                        fatalFileError = !cdb.Read("NumberOfDimensions", declaredDimensions);
                        if (!fatalFileError) {
                            fatalFileError = (declaredDimensions != 1u);
                        }
                    }
                }
                if (!fatalFileError) {
                    fatalFileError = !cdb.MoveToAncestor(1u);
                }
                if (!fatalFileError) {
                    REPORT_ERROR(ErrorManagement::Information, "Added signal %s:%s[%d]", signalName.Buffer(), signalTypeStr.Buffer(), nElements);
                }
                token = "";

            }
        }
        else {
            uint32 readSize = static_cast<uint32>(sizeof(uint32));
            /*lint -e{928}  [MISRA C++ Rule 5-2-7]. Justification: Need to cast to the type expected by the Read function.*/
            fatalFileError = !inputFile.Read(reinterpret_cast<char8 *>(&nOfSignals), readSize);
            for (n = 0u; (n < nOfSignals) && (!fatalFileError); n++) {
                //Write the signal type
                readSize = static_cast<uint32>(sizeof(uint16));
                TypeDescriptor signalType;
                /*lint -e{928}  [MISRA C++ Rule 5-2-7]. Justification: Need to cast to the type expected by the Read function.*/
                fatalFileError = !inputFile.Read(reinterpret_cast<char8 *>(&signalType.all), readSize);

                const uint32 SIGNAL_NAME_MAX_SIZE = 32u;
                char8 signalNameMemory[SIGNAL_NAME_MAX_SIZE + 1u];
                if (!fatalFileError) {
                    fatalFileError = !MemoryOperationsHelper::Set(&signalNameMemory[0], '\0', SIGNAL_NAME_MAX_SIZE + 1u);
                }
                if (!fatalFileError) {
                    readSize = SIGNAL_NAME_MAX_SIZE;
                    fatalFileError = !inputFile.Read(&signalNameMemory[0], readSize);
                }
                if (!fatalFileError) {
                    fatalFileError = (readSize != SIGNAL_NAME_MAX_SIZE);
                }
                StreamString signalName;
                if (!fatalFileError) {
                    //lint -e{645} signalNameMemory is always initialised if !fatalFileError
                    signalName = reinterpret_cast<const char8 * const >(&signalNameMemory[0]);
                }
                uint32 nOfElements = 0u;
                if (!fatalFileError) {
                    readSize = static_cast<uint32>(sizeof(uint32));
                    /*lint -e{928}  [MISRA C++ Rule 5-2-7]. Justification: Need to cast to the type expected by the Read function.*/
                    fatalFileError = !inputFile.Read(reinterpret_cast<char8 *>(&nOfElements), readSize);
                }
                if (!fatalFileError) {
                    fatalFileError = !cdb.CreateRelative(signalName.Buffer());
                }
                if (!fatalFileError) {
                    fatalFileError = !cdb.Write("Type", TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType));
                }
                if (!fatalFileError) {
                    fatalFileError = !cdb.Write("NumberOfElements", nOfElements);
                }
                if (!fatalFileError) {
                    fatalFileError = !cdb.MoveToAncestor(1u);
                }
                if (!fatalFileError) {
                    REPORT_ERROR(ErrorManagement::Information, "Added signal %s:%s[%d]", signalName.Buffer(), TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType), nOfElements);
                }
            }
        }

    }
    ErrorManagement::ErrorType ret(!fatalFileError);
    return ret;
}

ErrorManagement::ErrorType FileReader::CloseFile() {
    ErrorManagement::ErrorType err;
    if (inputFile.IsOpen()) {
        err = !inputFile.Close();
    }
    return err;
}

const StreamString& FileReader::GetFilename() const {
    return filename;
}

const StreamString& FileReader::GetFileFormat() const {
    return fileFormatStr;
}

const StreamString& FileReader::GetCSVSeparator() const {
    return csvSeparator;
}

bool FileReader::IsInterpolate() const {
    return interpolate;
}

const StreamString& FileReader::GetXAxisSignal() const {
    return xAxisSignalName;
}

uint64 FileReader::GetInterpolationPeriod() const {
    return interpolationPeriod;
}

CLASS_REGISTER(FileReader, "1.0")
CLASS_METHOD_REGISTER(FileReader, CloseFile)

}

