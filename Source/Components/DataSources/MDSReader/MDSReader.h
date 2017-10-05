/**
 * @file MDSReader.h
 * @brief Header file for class MDSReader
 * @date 22/09/2017
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class MDSReader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_MDSREADER_MDSREADER_H_
#define DATASOURCES_MDSREADER_MDSREADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "mdsobjects.h"
/*lint -D__cplusplus*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "MessageI.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Segments of one element are not allowed
 */
class MDSReader: public DataSourceI, public MessageI {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MDSReader)
public:
    CLASS_REGISTER_DECLARATION()

    MDSReader();
    virtual ~MDSReader();

    virtual bool Synchronise();

    /**
     * @brief Reads, checks and initialises the DataSource parameters
     * @details Load from a configuration file the DataSource parameters.
     * If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads tree name </li>
     * <li>Reads the shot number </li>
     * <li>Opens the tree with the shot number </li>
     * </ul>
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Read, checks and initialises the Signals
     * @details If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads nodes name (could be 1 or several nodes)</li>
     * <li>Opens nodes</li>
     * <li>Gets the node type</li>
     * <li>Verifies the node type</li>
     * <li>Gets number of elements per node (or signal).
     * <li>Gets the the size of the type in bytes</li>
     * <li>Allocates memory
     * </ul>
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Do nothing
     * @return true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Do nothing
     * @return true
     */
    virtual bool AllocateMemory();

    /**
     * @return 1u
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief Gets the signal memory buffer for the specified signal.
     * @param[in] signalIdx indicates the index of the signal to be obtained.
     * @param[in] bufferIdx indicate the index of the buffer to be obtained. Since only one buffer is allowed this parameter is always 0
     * @param[out] signalAddress is where the address of the desired signal is copied.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only InputSignals are supported.
     * @return MemoryMapSynchronisedInputBroker if interpolate = false, MemoryMapInterpolatedInputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details adds a MemoryMapSynchronisedInputBroker instance to the intputBrokers.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);
private:
    /**
     * @brief Open MDS tree
     * @details Open the treeName and copy the pointer of the object to tree variables.
     */
    bool OpenTree();

    /**
     * @brief Open MDS node.
     * @details Open MDS node called nodeNames[idx] and copy the object pointer to nodes[idx].
     * The order of the nodes is the other of the nodes names given in the configuration file.
     */
    bool OpenNode(uint32 idx);

    /**
     * @brief Gets the MDS type of the node [idx].
     */
    bool GetTypeNode(uint32 idx);

    /**
     * @brief validates the type of the node idx
     * @return true if the type is supported
     */
    bool IsValidTypeNode(uint32 idx);

    bool CheckTypeAgainstMdsNodeTypes(uint32 idx);

    bool GetDataNode(uint32 nodeNumber);

    /**
     * @brief find tin which segment contains the time t for the signal index specified
     * @details The algorithm goes throw all segments until the time t is found or the end of the segments are reached
     * @param[in] t indicate the time to be found
     * @param[out] if t is found the segment holds the segment where t was found.
     * @param[in] index indicates which node number must be used.
     * @return -1 if the end of data
     * @return 0 if not found but is not the end of the data. It could happen if the data is only stored in trigger events
     * @return 1 if the time t is in a segment.
     */
    int8 FindSegment(float64 t,
                     uint32 &segment,
                     uint32 index);

    /**
     * @brief Calculates the time difference between the first samples.
     * @details It assumes that if there is one element per segment there is no holes between the first
     * and the second element.
     * @param[in] idx indicates the node number from where the time is extracted
     * @param[out] tDiff hold the sample difference.
     * @return true on succeed.
     */
    bool GetNodeSamplingTime(uint32 idx,
                             float64 &tDiff);

    void CopyTheSameValue(uint32 idxNumber,
                          uint32 nTimes,
                          uint32 samplesOffset);

    void AddValuesCopyData(uint32 nodeNumber,
                           uint32 minSegment,
                           uint32 maxSegment);

    bool CopyDataAddValues(uint32 nodeNumber,
                           uint32 minSegment,
                           uint32 maxSegment);

    /**
     * @brief Copy the data from the tree to the allocated memory
     * @details It uses MemoryOperationsHelper::Copy() assuming that there is enough data to be copied.
     * @param[in] nodeNumber indicates from which node the data must be copied.
     * @param[in] minSeg indicates the first segment which must be copied from. Notice that not necessarily the data must be copied from the beginning.
     * @param[in] SamplesToCopy indicates how man samples must be copied. The node must have more than SamplesToCopy remaining to be copied.
     * @return the number of samples copied from the tree to the allocated memory
     */
    uint32 MakeRawCopy(uint32 nodeNumber,
                       uint32 minSegu,
                       uint32 SamplesToCopy,
                       uint32 OffsetSamples);

    uint32 LinearInterpolationCopy(uint32 nodeNumber,
                                   uint32 minSegment,
                                   uint32 SamplesToCopy,
                                   uint32 offsetSamples);
    uint32 HoldCopy(uint32 nodeNumber,
                    uint32 minSegment,
                    uint32 SamplesToCopy,
                    uint32 samplesOffset);

    bool CopyData(uint32 nodeNumber,
                  uint32 minSegment);

    bool CopyRemainingData(uint32 nodeNumber,
                           uint32 minSegment);

    template<typename T>
    bool SampleInterpolation(float64 currentTime,
                             T data1,
                             T data2,
                             float64 t1,
                             float64 t2,
                             float64 *tr);

    TypeDescriptor ConvertMDStypeToMARTeType(StreamString mdsType);
    StreamString treeName;
    MDSplus::Tree *tree;
    StreamString *nodeName;
    MDSplus::TreeNode **nodes;
    uint32 numberOfNodeNames;
    /**
     * MDSplus signal type. If signalTypes is given as input, consistency between mdsSignalType signalType
     */
    StreamString *mdsNodeTypes;
    uint32 *byteSizeSignals;

    /**
     * In SetConfiguredDatabase() the information is modified. I.e the node name is not copied because is unknown parameter for MARTe.
     */
    ConfigurationDatabase originalSignalInformation;

    /**
     * Indicates the pulse number to open. If it is not specified -1 by default.
     */
    int32 shotNumber;

    /**
     * signal type expected to read. It is optional parameter on the configuration file. If exist it is check against signalType.
     */
    TypeDescriptor *type;

    /**
     * number of elements that should be read each MARTe cycle. It is read from the configuration file
     */
    uint32 *numberOfElements;

    char8 *dataSourceMemory;

    uint32 *offsets;

    /**
     * Time seconds. It indicates the beginning of each segment
     */
    float64 time;

    /**
     * Time in seconds. It indicates the time of each sample. At the beginning of each cycle currentTime = time.
     */
    float64 currentTime;

    /**
     * time increment between Synchronisations. It is he inverse of frequency;
     */
    float64 period;

    /**
     *
     */
    float64 frequency;

    /**
     * Holds the maximum number of segments for each node of the tree.
     */
    uint64 *maxNumberOfSegments;

    /**
     * hold the last segments where the t was found. It is used for optimization since the time could not go back.
     */
    uint64 *lastSegment;

    /**
     * Management of the data. indicates what to do with the data.
     * 0 --> nothing
     * 1 --> linear interpolation
     * 2 --> hold last value.
     *
     */
    uint8 *dataManagement;

    /**
     * Management of the  hole (when there is no data stored).
     * 0 --> add 0
     * 1 --> hold last value
     */
    uint8 *holeManagement;

    /**
     * Sampling time calculated from the configuration file. samplingTime[i]= 1/frequency/numberOfElements[i]
     */
    float64 *samplingTime;

    char8 *lastValue;

    float64 *lastTime;

    uint32 *offsetLastValue;

    /**
     * Elements of the current segment already consumed. It is used to know from where the segment must be copied.
     */
    uint32 * elementsConsumed;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_MDSREADER_MDSREADER_H_ */

