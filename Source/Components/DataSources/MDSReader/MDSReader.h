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
 * @brief MDSReader is a data source which allows to read data from a MDSplus tree.
 * @details MDSReader is an input data source which takes data from MDSPlus nodes (as many as desired) and publishes it on a real time application.
 *
 * The MDSReader can either interpolate, decimate or take the raw data, as it is, from the tree depending on the parameter called "DataManagement" which is given
 * in the configuration file.
 * Moreover, this data source can deal with discontinuous data and has a configuration parameter for managing the absence of data.
 * DataManagement can take the following values:
 * <ul>
 * <li>0 --> MDSReader takes the data from the tree as it is (raw). In this configuration, the frequency/numberOfElements must be the same than the node sampling frequency.</li>
 * <li>1--> MDSReader interpolates the signal taking as a reference the two nearest data values. If the frequency/numberOfElements is smaller than the sample frequency
 * of the MDSplus node the data source interpolates the signal. If the frequency/numberOfElements larger than the node sample frequency the signals is decimated.</li>
 * <li>2 --> MDSReader holds the value following the criteria of the nearest value given specific time. I.e the node data is (t1, d1) = (1, 1) and (t2, d2) = (2, 5) and the currentTime is t = 1.6 the
 * nearest data to the given time is 5.</li>
 * </ul>
 *
 * HoleManagement can take the following values:
 * <ul>
 * <li>0 --> MDSreader fills the absence of data with 0</li>
 * <li>1 --> MDSReader fills the absence of data with the last value.</li>
 * </ul>
 *
 * Even if the MDSReader can deal with the absence of data, the sampling time must be constant with-in the node, however the sampling time between
 * nodes can be different.
 *
 * The MDSReader can handle as many nodes as desired. Each node can have their on data type, maximum number of segments, elements per segment and sampling time. When the
 * end of a node is reached the data of the corresponding node is filled with 0 and the data source continuous running until all nodes reach the end.
 *
 * The supported types for the nodes are:
 * <ul>
 * <li>uint8</li>
 * <li>int8</li>
 * <li>uint16</li>
 * <li>int16</li>
 * <li>uint32</li>
 * <li>int32</li>
 * <li>uint64</li>
 * <li>int64</li>
 * <li>float32</li>
 * <li>float64</li>
 * </ul>
 *
 * The last signal specified must be the time.
 * The supported type for the time are:
 * <ul>
 * <li>uint32</li>
 * <li>int32</li>
 * <li>uint64</li>
 * <li>int64</li>
 * </ul>
 *
 *The configuration syntax is (names and signal quantity are only given as an example):
 *<pre>
 * +MDSReader_0 = {
 *     Class = MDSReader
 *     TreeName = "test_tree" //Compulsory. Name of the MDSplus tree.
 *     ShotNumber = 1 //Compulsory. 0 --> last shot number (to use 0 shotid.sys must exist)
 *     Frequency = 1000 // in Hz. Is the cycle time of the real time application.
 *
 *     Signals = {
 *         S_uint8 = {
 *             NodeName = "S_uint8" // node of the tree node
 *             Type = "uint8" //Can be any of the node supported types
 *             NumberOfElements = 32
 *             DataManagement = 0 //could be 0, 1 or 2
 *             HoleManagement = 1 //could be 0 or 1
 *         }
 *         S_int8 = {
 *             NodeName = "S_int8" //node of the tree node
 *             NumberOfElements = 3
 *             DataManagement = 2 //could be 0, 1 or 2
 *             HoleManagement = 0 //could be 0 or 1
 *         }
 *         ....
 *         ....
 *         ....
 *         Time = { //Compulsory
 *             Type = "uint32" //can be any of the supported types
 *             NumberOfElements = 1 //must be always one.
 *         }
 *     }
 * }
 * </pre>
 */
class MDSReader: public DataSourceI {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MDSReader)
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief default constructor
     */
    MDSReader ();

    /**
     * @brief default destructor.
     */
    virtual ~MDSReader();

    /**
     * @brief Copy data from the tree nodes to the dataSourceMemory
     * @details When a node does not have more data to retrieve the dataSourceMemory is filled with 0.
     * @return true if all nodes read return false.
     */
    virtual bool Synchronise();

    /**
     * @brief Reads, checks and initialises the DataSource parameters
     * @details Load from a configuration file the DataSource parameters.
     * If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads tree name </li>
     * <li>Reads the shot number </li>
     * <li>Opens the tree with the shot number </li>
     * <li>Reads the real-time thread Frequency parameter.</li>
     * </ul>
     * @param[in] data is the configuration file.
     * @return true if all parameters can be read and the values are valid
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
     * @param[in] data is the configuration file.
     * @return true if all parameters can be read and the values are valid
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
     * @return MemoryMapSynchronisedInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details adds a MemoryMapSynchronisedInputBroker instance to the inputBrokers.
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
     * @param[in] idx node index
     */
    bool GetTypeNode(uint32 idx);

    /**
     * @brief validates the type of the node idx.
     * @param[in] idx node index to be validated.
     * @return true if the type is supported
     */
    bool IsValidTypeNode(const uint32 idx) const;

    /**
     * @brief Cross check the specified type with the node type.
     * @param[in] idx node index to be validated.
     * @return true if the node type is the same than the type specified in the configuration file.
     */
    bool CheckTypeAgainstMdsNodeTypes(const uint32 idx) const;

    /**
     * @brief Gets the data node for a real time cycle.
     * @brief First determine the topology of the chunk of data to be read (i.e if there is enough data in the node, if the data has holes)
     * end then decides how to copy the data.
     * @param[in] nodeNumber node number to be copied to the dataSourceMemory.
     * @return true if node data can be copied. false if is the end of the node
     */
    bool GetDataNode(const uint32 nodeNumber);

    /**
     * @brief copy the time internally generated to the dataSourceMemory.
     */
    void PublishTime();

    /**
     * @brief Finds which segment contains the time t for the signal index specified
     * @details The algorithm goes through all segments until the time t is found or the end of the segments are reached
     * @param[in] t indicate the time to be found
     * @param[out] segment if t is found the segment holds the segment where t was found.
     * @param[in] index indicates which node must be used.
     * @return -1 if the end of data
     * @return 0 if not found but is not the end of the data. It could happen if the data is stored in trigger events
     * @return 1 if the time t is in a segment.
     */
    int8 FindSegment(const float64 t,
                     uint32 &segment,
                     const uint32 nodeIdx);

    /**
     * @brief Counts how many discontinuities are in the specified period
     * @param[in] nodeNumber is the node number.
     * @param[in] initialSegment first segment from where the algorithm starts to count discontinuities.
     * @param[in] finalSemgnet the last segment to where the algorithm stops to count discontinuities.
     * @return the number of discontinuities.
     */
    uint32 CheckDiscontinuityOfTheSegments(const uint32 nodeNumber,
                                           const uint32 initialSegment,
                                           const uint32 finalSegment) const;

    /**
     * @brief Calculates the time difference between the first samples.
     * @details It assumes that if there is one element per segment there is no holes between the first
     * and the second element.
     * @param[in] idx indicates the node number from where the time is extracted
     * @param[out] tDiff hold the sample difference.
     * @return true on succeed.
     */
    bool GetNodeSamplingTime(const uint32 idx,
                             float64 &tDiff) const;

    /**
     * @brief Copy the same value as many times as indicated.
     * @details this function decides the type of data to copy and then calls the MDSReader::CopyTheSameValue()
     * @param[in] idxNumber is the node number from which the data must be copied.
     * @param[in] numberOfTimes how many samples must be copied.
     * @param[in] samplesOffset indicates how many samples has already copied.
     */
    void CopyTheSameValue(const uint32 idxNumber,
                          const uint32 numberOfTimes,
                          const uint32 samplesOffset);

    /**
     * @brief Template functions which actually performs the copy
     * @param[in] idxNumber is the node number from which the data must be copied.
     * @param[in] numberOfTimes how many samples must be copied.
     * @param[in] samplesOffset indicates how many samples has already copied.
     */
    template<typename T>
    void CopyTheSameValueTemplate(uint32 idxNumber,
                                  uint32 numberOfTimes,
                                  uint32 samplesOffset);

    /**
     * @brief First fills a hole and then copy data from the node
     * @param[in] nodeNumber node number from where copy data.
     * @param[in] minSegment indicates the segment from where start to be copied.
     * @param[in] numberOfDiscontinuities indicates the number of times that the algorithm must be applied
     */
    bool AddValuesCopyData(const uint32 nodeNumber,
                           uint32 minSegment,
                           const uint32 numberOfDiscontinuities);

    /**
     * @brief First copies data and then adds values
     * @param[in] nodeNumber node number from where copy data.
     * @param[in] minSegment indicates the segment from where start to be copied.
     * @param[in] numberOfDiscontinuities indicates the number of times that the algorithm must be applied
     * @param[in] samplesToRead Samples to copy
     * @param[in] samplesRead Samples already copied.
     */
    bool CopyDataAddValues(const uint32 nodeNumber,
                           uint32 minSegment,
                           const uint32 numberOfDiscontinuities,
                           const uint32 samplesToRead,
                           const uint32 samplesRead);

    /**
     * @brief First it fills the holes with data then copy data from MDSplus and the continue copying data
     */
    bool AddValuesCopyDataAddValues(const uint32 nodeNumber,
                                    const uint32 minSegment,
                                    const uint32 numberOfDiscontinuities);

    /**
     * @brief First it copies real data from MDSplus then fill the holes with some data and then continues copying data
     */
    bool CopyDataAddValuesCopyData(const uint32 nodeNumber,
                                   uint32 minSegment,
                                   const uint32 numberOfDiscontinuities);

    /**
     * @brief Finds the next discontinuity
     * @param[in] segment indicates the current segment from where starts to look the discontinuity.
     * @param[out] beginningTime indicates the last time which contains data.
     * @param[out] endTime indicates the first time after the discontinuity with data.
     * @return true if the discontinuity exist.
     */
    bool FindDiscontinuity(const uint32 nodeNumber,
                           uint32 &segment,
                           float64 &beginningTime,
                           float64 &endTime) const;

    /**
     * @brief Copies the data from the tree to the allocated memory
     * @details It uses MemoryOperationsHelper::Copy().
     * @param[in] nodeNumber indicates from which node the data must be copied.
     * @param[in] minSeg indicates the first segment which must be copied from. Notice that not necessarily the data must be copied from the beginning.
     * @param[in] SamplesToCopy indicates how man samples must be copied. The node must have more than SamplesToCopy remaining to be copied.
     * @param[in] OffsetSamples indicates how many samples are already copied. It is used to know in which position of the dataSourceMemory the data should be copied.
     * @return the number of samples copied from the tree to the allocated memory
     */
    uint32 MakeRawCopy(const uint32 nodeNumber,
                       const uint32 minSeg,
                       const uint32 samplesToCopy,
                       const uint32 offsetSamples);

    /**
     * @brief template which copies data using MemoryOperationsHelper::Copy
     */
    template<typename T>
    uint32 MakeRawCopyTemplate(uint32 nodeNumber,
                               uint32 minSeg,
                               uint32 SamplesToCopy,
                               uint32 OffsetSamples);

    /**
     * @brief Copies data interpolating the samples
     * @brief this function decides the type of data and the calls the LinearInterpolationCopyTemplate()
     */
    uint32 LinearInterpolationCopy(const uint32 nodeNumber,
                                   const uint32 minSeg,
                                   const uint32 samplesToCopy,
                                   const uint32 offsetSamples);

    /**
     * @brief Template function which reads the data from MDSplus and performs the interpolation
     */
    template<typename T>
    uint32 LinearInterpolationCopyTemplate(uint32 nodeNumber,
                                           uint32 minSeg,
                                           uint32 samplesToCopy,
                                           uint32 offsetSamples);

    /**
     * @brief Fills the holes with the last value.
     * @details this function calls the HoldCopyTemplate.
     */
    uint32 HoldCopy(const uint32 nodeNumber,
                    const uint32 minSeg,
                    const uint32 samplesToCopy,
                    const uint32 samplesOffset);

    /**
     * @brief Template function which actually fills the dataSourceMemory buffer
     */
    template<typename T>
    uint32 HoldCopyTemplate(uint32 nodeNumber,
                            uint32 minSeg,
                            uint32 samplesToCopy,
                            uint32 samplesOffset);

    /**
     * @brief Copy the remaining data of the MDSpls into the dataSourceMemory buffer
     * @details this function is called just to managed the end of the MDSplus data.
     */
    bool CopyRemainingData(const uint32 nodeNumber,
                           const uint32 minSegment);

    /**
     * @brief Given tstart and tend this function decide how many samples to copy
     */
    uint32 ComputeSamplesToCopy(const uint32 nodeNumber,
                                const float64 tstart,
                                const float64 tend) const;

    /**
     * @brief modifies samples in case it was not well calculated due to numeric errors.
     */
    void VerifySamples(const uint32 nodeNumber,
                       uint32 &samples,
                       const float64 tstart,
                       const float64 tend) const;

    /**
     * @brief Compute the interpolation given to samples.
     */
    template<typename T>
    bool SampleInterpolation(float64 cT,
                             T data1,
                             T data2,
                             float64 t1,
                             float64 t2,
                             float64 *ptr);

    /**
     * @brief Convert the the MDSplus type into MARTe type.
     */
    TypeDescriptor ConvertMDStypeToMARTeType(StreamString mdsType) const;

    bool AllNodesEnd() const;

    /**
     * The name of the MDSplus tree to be opened.
     */
    StreamString treeName;

    /**
     * The MDSplus tree to be opened.
     */
    MDSplus::Tree *tree;

    /**
     * The names of the nodes to be managed.
     */
    StreamString *nodeName;

    /**
     * The nodes to be managed.
     */
    MDSplus::TreeNode **nodes;

    /**
     * The number of nodes to be managed.
     */
    uint32 numberOfNodeNames;

    /**
     * The number of input signals.
     */
    uint32 nOfInputSignals;

    /**
     * The number of input on each function.
     */
    uint32 nOfInputSignalsPerFunction;

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
     * Signal type expected to be read. It is optional parameter on the configuration file. If exist it is check against signalType.
     */
    TypeDescriptor *type;

    /**
     * Number of bytes of each signal to be read.
     */
    uint32 *bytesType;

    /**
     * Number of elements that should be read each MARTe cycle. It is read from the configuration file
     */
    uint32 *numberOfElements;

    /**
     * Data source memory.
     */
    char8 *dataSourceMemory;

    /**
     * Signals offsets in the memory.
     */
    uint32 *offsets;

    /**
     * Time seconds. It indicates the beginning of each segment
     */
    float64 timeCycle;

    /**
     * Time in seconds. It indicates the time of each sample. At the beginning of each cycle currentTime = time.
     */
    float64 currentTime;

    /**
     * Time increment between Synchronisations. It is he inverse of frequency;
     */
    float64 period;

    /**
     * Real-time application frequency.
     */
    float64 frequency;

    /**
     * Holds the maximum number of segments for each node of the tree.
     */
    uint32 *maxNumberOfSegments;

    /**
     * hold the last segments where the t was found. It is used for optimization since the time could not go back.
     */
    uint32 *lastSegment;

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

    /**
     * Last values to be copied.
     */
    char8 *lastValue;

    float64 *lastTime;

    uint32 *offsetLastValue;

    /**
     * Elements of the current segment already consumed. It is used to know from where the segment must be copied.
     */
    uint32 *elementsConsumed;

    bool *endNode;
    float64 *nodeSamplingTime;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
void MDSReader::CopyTheSameValueTemplate(uint32 idxNumber,
                                         uint32 numberOfTimes,
                                         uint32 samplesOffset) {
    uint32 extraOffset = samplesOffset * bytesType[idxNumber];
    T *ptr = reinterpret_cast<T *>(&dataSourceMemory[offsets[idxNumber] + extraOffset]);
    for (uint32 i = 0u; i < numberOfTimes; i++) {
        ptr[i] = *reinterpret_cast<T *>(&lastValue[offsetLastValue[idxNumber]]);
    }
}

template<typename T>
uint32 MDSReader::MakeRawCopyTemplate(uint32 nodeNumber,
                                      uint32 minSeg,
                                      uint32 SamplesToCopy,
                                      uint32 OffsetSamples) {

    MDSplus::Data *dataD = NULL_PTR(MDSplus::Data *);
    int32 nElements = 0u;
    uint32 bytesToCopy = 0u;
    uint32 extraOffset = OffsetSamples * bytesType[nodeNumber];
    bool endSegment = false;
    uint32 samplesCopied = 0u;
    uint32 remainingSamplesOnTheSegment = 0u;
    T* data = NULL_PTR(T *);
    for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
        dataD = nodes[nodeNumber]->getSegment(currentSegment);
        if (type[nodeNumber] == UnsignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongArray(&nElements));
        }
        else if (type[nodeNumber] == Float32Bit) {
            data = reinterpret_cast<T *>(dataD->getFloatArray(&nElements));
        }
        else if (type[nodeNumber] == Float64Bit) {
            data = reinterpret_cast<T *>(dataD->getDoubleArray(&nElements));
        }

        remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
        endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
        if (!endSegment) {        // no end of segment but no more data need to be copied
            bytesToCopy = SamplesToCopy * bytesType[nodeNumber];
            samplesCopied += SamplesToCopy;
        }
        else {        // end segment but still more data must be copied
            bytesToCopy = remainingSamplesOnTheSegment * bytesType[nodeNumber];
            samplesCopied += remainingSamplesOnTheSegment;

            //
        }
        MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                     reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);
        extraOffset += bytesToCopy;

//Update values
        if (!endSegment) {        // no end of segment but no more data need to be copied
            elementsConsumed[nodeNumber] += SamplesToCopy;
            SamplesToCopy = 0u;
        }
        else {        // end segment but still more data must be copied
            SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
            elementsConsumed[nodeNumber] = 0u;
        }
        *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]) = data[nElements - 1];
        MDSplus::deleteData (dataD);
        delete data;

    }
    return samplesCopied;
}

template<typename T>
uint32 MDSReader::LinearInterpolationCopyTemplate(uint32 nodeNumber,
                                                  uint32 minSeg,
                                                  uint32 samplesToCopy,
                                                  uint32 offsetSamples) {

    MDSplus::Array *dataD = NULL_PTR(MDSplus::Array *);
    MDSplus::Data *timeNodeD = NULL_PTR(MDSplus::Data *);
    float64 *timeNode = NULL_PTR(float64 *);
    int32 nElements = 0u;
    uint32 extraOffset = offsetSamples * bytesType[nodeNumber];
    bool endSegment = false;
    uint32 samplesCopied = 0;
    uint32 iterations = 0u;
    uint32 remainingSamplesOnTheSegment = 0u;

    T* data = NULL_PTR(T *);
    for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (samplesToCopy != 0); currentSegment++) {
        //nodes[nodeNumber]->getSegmentAndDimension(currentSegment, dataD, timeNodeD);
        dataD = nodes[nodeNumber]->getSegment(currentSegment);
        timeNodeD = nodes[nodeNumber]->getSegmentDim(currentSegment);
        if (type[nodeNumber] == UnsignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongArray(&nElements));
        }
        else if (type[nodeNumber] == Float32Bit) {
            data = reinterpret_cast<T *>(dataD->getFloatArray(&nElements));
        }
        else if (type[nodeNumber] == Float64Bit) {
            data = reinterpret_cast<T *>(dataD->getDoubleArray(&nElements));
        }
        timeNode = timeNodeD->getDoubleArray(&nElements);
        float64 auxTime = timeNode[nElements - 1] + samplingTime[nodeNumber];
        remainingSamplesOnTheSegment = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime); //static_cast<uint32>(1 + ((timeNode[nElements - 1] - currentTime) / samplingTime[nodeNumber]));
        endSegment = (remainingSamplesOnTheSegment <= samplesToCopy);
        if (!endSegment) {        //no end of segment but no more data need to be copied
            samplesCopied += samplesToCopy;
            iterations = samplesToCopy;
        }
        else {        // end segment but still more data must be copied
            samplesCopied += remainingSamplesOnTheSegment;
            iterations = remainingSamplesOnTheSegment;
        }
        float64 outputInterpolation = 0.0;
        for (uint32 i = 0u; i < iterations; i++) {
            while ((currentTime >= timeNode[elementsConsumed[nodeNumber]]) && (elementsConsumed[nodeNumber] < static_cast<uint32>(nElements - 1))) {
                elementsConsumed[nodeNumber]++;
            }
            if (elementsConsumed[nodeNumber] == 0u) {
                SampleInterpolation<T>(currentTime, *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]), data[elementsConsumed[nodeNumber]],
                                       lastTime[nodeNumber], timeNode[elementsConsumed[nodeNumber]], &outputInterpolation);

            }
            else {
                SampleInterpolation<T>(currentTime, data[elementsConsumed[nodeNumber] - 1], data[elementsConsumed[nodeNumber]],
                                       timeNode[elementsConsumed[nodeNumber] - 1], timeNode[elementsConsumed[nodeNumber]], &outputInterpolation);
            }
            if ((type[nodeNumber] == Float32Bit) || (type[nodeNumber] == Float64Bit)) {
                *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = static_cast<T>(outputInterpolation);
            }
            else {
                *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = static_cast<T>(round(outputInterpolation));
            }
            extraOffset += bytesType[nodeNumber];
            currentTime += samplingTime[nodeNumber];
            samplesToCopy--;
        }
        if (endSegment) {
            *reinterpret_cast<T *>(&(lastValue[offsetLastValue[nodeNumber]])) = data[nElements - 1];
            lastTime[nodeNumber] = timeNode[nElements - 1];
            elementsConsumed[nodeNumber] = 0u;
        }
        MDSplus::deleteData (dataD);
        MDSplus::deleteData (timeNodeD);
        delete data;
        delete timeNode;
    }
    return samplesCopied;

}

template<typename T>
uint32 MDSReader::HoldCopyTemplate(uint32 nodeNumber,
                                   uint32 minSeg,
                                   uint32 samplesToCopy,
                                   uint32 samplesOffset) {

    MDSplus::Array *dataD = NULL_PTR(MDSplus::Array *);
    MDSplus::Data *timeNodeD = NULL_PTR(MDSplus::Data *);
    float64 *timeNode = NULL_PTR(float64 *);
    int32 nElements = 0u;
    uint32 extraOffset = samplesOffset * bytesType[nodeNumber];
    bool endSegment = false;
    uint32 samplesCopied = 0;
    uint32 iterations = 0u;
    uint32 remainingSamplesOnTheSegment = 0u;

    T* data = NULL_PTR(T *);
    for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (samplesToCopy != 0); currentSegment++) {
        //nodes[nodeNumber]->getSegmentAndDimension(currentSegment, dataD, timeNodeD);
        dataD = nodes[nodeNumber]->getSegment(currentSegment);
        timeNodeD = nodes[nodeNumber]->getSegmentDim(currentSegment);
        if (type[nodeNumber] == UnsignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongArray(&nElements));
        }
        else if (type[nodeNumber] == Float32Bit) {
            data = reinterpret_cast<T *>(dataD->getFloatArray(&nElements));
        }
        else if (type[nodeNumber] == Float64Bit) {
            data = reinterpret_cast<T *>(dataD->getDoubleArray(&nElements));
        }
        timeNode = timeNodeD->getDoubleArray(&nElements);
        float64 auxTime = timeNode[nElements - 1] + samplingTime[nodeNumber];
        remainingSamplesOnTheSegment = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime); //static_cast<uint32>(1 + ((timeNode[nElements - 1] - currentTime) / samplingTime[nodeNumber]));
        endSegment = (remainingSamplesOnTheSegment <= samplesToCopy);
        if (!endSegment) {        //no end of segment but no more data need to be copied
            samplesCopied += samplesToCopy;
            iterations = samplesToCopy;
        }
        else {        // end segment but still more data must be copied
            samplesCopied += remainingSamplesOnTheSegment;
            iterations = remainingSamplesOnTheSegment;
        }
        for (uint32 i = 0u; i < iterations; i++) {
            while ((currentTime >= timeNode[elementsConsumed[nodeNumber]]) && (elementsConsumed[nodeNumber] < static_cast<uint32>(nElements - 1))) {
                elementsConsumed[nodeNumber]++;
            }
            if (elementsConsumed[nodeNumber] == 0u) {
                float64 diff1 = currentTime - lastTime[nodeNumber];
                float64 diff2 = timeNode[0] - currentTime;
                if (diff1 < diff2) {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) =
                            *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]);
                }
                else {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = data[0];
                }
            }
            else {
                float64 diff1 = currentTime - timeNode[elementsConsumed[nodeNumber] - 1];
                float64 diff2 = timeNode[elementsConsumed[nodeNumber]] - currentTime;
                if (diff1 < diff2) {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = data[elementsConsumed[nodeNumber] - 1];
                }
                else {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = data[elementsConsumed[nodeNumber]];
                }
            }
            extraOffset += bytesType[nodeNumber];
            currentTime += samplingTime[nodeNumber];
            samplesToCopy--;
        }
        if (endSegment) {
            *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]) = data[nElements - 1];
            lastTime[nodeNumber] = timeNode[nElements - 1];
            elementsConsumed[nodeNumber] = 0u;
        }
        MDSplus::deleteData (dataD);
        MDSplus::deleteData (timeNodeD);
        delete data;
        delete timeNode;
    }
    return samplesCopied;
}

template<typename T>
bool MDSReader::SampleInterpolation(float64 cT,
                                    T data1,
                                    T data2,
                                    float64 t1,
                                    float64 t2,
                                    float64 *ptr) {
    bool ret = t2 > t1;
    if (ret) {
        float64 slope = (data2 - data1) / (t2 - t1);
        *ptr = slope * (cT - t1) + data1;
    }
    return ret;
}
}

#endif /* DATASOURCES_MDSREADER_MDSREADER_H_ */

