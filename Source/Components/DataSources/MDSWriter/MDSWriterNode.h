/**
 * @file MDSWriterNode.h
 * @brief Header file for class MDSWriterNode
 * @date 10/02/2017
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

 * @details This header file contains the declaration of the class MDSWriterNode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSWRITER_MDSWRITERNODE_H_
#define MDSWRITER_MDSWRITERNODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "mdsobjects.h"
/*lint -D__cplusplus*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Provides an interface between a MARTe signal and an MDSplus::TreeNode.
 * @details This class allocates a shared memory area capable of storing several time samples of a MARTe signal.
 * (i.e. a scalar or an array which describes the MARTe signal in a given time instant).
 *
 * When the number of time samples is deemed sufficient, an MDS+ segment is created and all the shared memory data is
 * stored inside this segment.
 * A segment will be created when GetNumberOfExecuteCalls() == GetMakeSegmentAfterNWrites() or, in case
 * IsUseTimeVector() == true, when a discontinuity on the time signal is detected, i.e., when the distance between
 *  two time samples is greater than GetExecutePeriod.
 */
class MDSWriterNode {
public:
    /**
     * @brief Constructor.
     * @post
     *   IsDecimatedMinMax() == false &&
     *   GetDecimatedNodeName() == "" &&
     *   GetExecutePeriod() == 0 &&
     *   IsFlush() == false &&
     *   GetMakeSegmentAfterNWrites() == 0 &&
     *   GetMinMaxResampleFactor() == 0 &&
     *   GetNodeName() == "" &&
     *   GetNodeType() == 0 &&
     *   GetNumberOfElements() == 0 &&
     *   GetPeriod() == 0 &&
     *   GetPhaseShift() == 0 &&
     *   GetStart() == 0 &&
     *   GetTypeMultiplier() == 0 &&
     *   IsUseTimeVector() == 0 &&
     *   GetNumberOfExecuteCalls() == 0
     */
    MDSWriterNode();

    /**
     * @brief Destructor. Deletes any allocated nodes and frees the shared memory.
     */
    ~MDSWriterNode();

    /**
     * @brief Loads all the class parameters from a StructuredDataI.
     * @details Loads and verifies the following configuration parameters:
     *  - NodeName: name of MDS+ node
     *  - DecimatedNodeName (optional): name of the MDS+ node where the decimated data (decimated by MDS+) is to be stored.
     *  - Type: signal type (as defined in the RealTimeApplicationConfigurationBuilder)
     *  - NumberOfElements (>0): signal number of elements (as defined in the RealTimeApplicationConfigurationBuilder)
     *  - Period (>0): period between samples in seconds
     *  - SamplePhase (optional): shift the time vector by SamplePhase * Period
     *  - MakeSegmentAfterNWrites (>0): a segment will be written after MakeSegmentAfterNWrites time samples are available (which in practice means after the Execute method has been called MakeSegmentAfterNWrites)
     *  - MinMaxResampleFactor (>0): the decimation factor to be applied by MDS+ when a DecimatedNodeName is specified
     * @param data the StructuredDataI with all the parameters described above.
     * @return true if all the parameters above are correctly specified.
     */
    bool Initialise(StructuredDataI & data);

    /**
     * @brief Writes the signal data into the shared memory buffer and possibly into an MDS+ segment.
     * @details Copies the current signal data (see SetSignalMemory) into the shared memory buffer.
     * * A segment will be created when GetNumberOfExecuteCalls() == GetMakeSegmentAfterNWrites() or, in case
     * IsUseTimeVector() == true, when a discontinuity on the time signal is detected, i.e., when the distance between
     *  two time samples is greater than the GetExecutePeriod.
     * @return if the data can be successfully copied.
     * @pre
     *   SetSignalMemory() &&
     *   Initialise() &&
     *   AllocateTreeNode()
     */
    bool Execute();

    /**
     * @brief Sets the source signal memory.
     * @param[in] signalMemoryIn pointer to the signal to be stored in MDS+.
     */
    void SetSignalMemory(void *signalMemoryIn);

    /**
     * @brief Sets the source signal memory.
     * @param[in] timeSignalMemoryIn pointer to the time vector to be stored in MDS+.
     * @param[in] timeSignalTypeIn the type of the timeSignalMemoryIn.
     * @param[in] timeSignalMultiplierIn multiplier to convert from the units of the time signal into seconds (e.g. from micro-seconds to seconds).
     */
    void SetTimeSignalMemory(void *timeSignalMemoryIn, const TypeDescriptor & timeSignalTypeIn, float64 timeSignalMultiplierIn);

    /**
     * @brief Opens the MDSplus::TreeNode.
     * @param[in] tree an opened MDSplus::TreeNode with an existent GetNodeName() and, if applicable,
     * with an existent GetDecimatedNodeName()
     * @pre
     *   Initialise() &&
     *   tree != NULL
     */
    bool AllocateTreeNode(MDSplus::Tree *tree);

    /**
     * @brief Forces the creation of a segment even if no sufficient time samples have been created.
     * @return Execute()
     */
    bool Flush();

    /**
     * @brief Returns true if MDS+ is to automatically compute a decimated version of the stored signal.
     * @return true if MDS+ is to automatically compute a decimated version of the stored signal.
     */
    bool IsDecimatedMinMax() const;

    /**
     * @brief Returns the name of MDS+ node where the decimated version of the signal is stored.
     * @return the name of MDS+ node where the decimated version of the signal is stored.
     */
    const StreamString& GetDecimatedNodeName() const;

    /**
     * @brief Returns the period of a segment.
     * @return GetNumberOfElements()  * (GetPeriod() * GetMultiplier(see SetTimeSignalMemory));
     */
    uint32 GetExecutePeriod() const;

    /**
     * @brief A segment will be created after the Execute has been called GetMakeSegmentAfterNWrites()
     * @return the number of times Execute has to be called before a segment is created.
     */
    uint32 GetMakeSegmentAfterNWrites() const;

    /**
     * @brief Returns the decimation factor MDS+ will use to automatically compute a decimated version of the stored signal.
     * @return the decimation factor MDS+ will use to automatically compute a decimated version of the stored signal.
     */
    int32 GetMinMaxResampleFactor() const;

    /**
     * @brief Gets the name of the MDS+ node where the signal is to be stored.
     * @return the name of the MDS+ node where the signal is to be stored.
     */
    const StreamString& GetNodeName() const;

    /**
     * @brief Gets the type of the MDS+ node where the signal is to be stored.
     * @return the type of the MDS+ node where the signal is to be stored.
     */
    int32 GetNodeType() const;

    /**
     * @brief Gets the number of elements of the signal to be stored, 1 for a scalar, N for an array.
     * @details A segment will contain many time samples of such signal.
     * @return the number of elements of the signal to be stored, 1 for a scalar, N for an array.
     */
    uint32 GetNumberOfElements() const;

    /**
     * @brief Gets the time period between signal samples.
     * @return the time period between signal samples.
     */
    float64 GetPeriod() const;

    /**
     * @brief Gets the initial shift to apply to the time vector (in samples).
     * @return the initial shift to apply to the time vector (in samples).
     */
    int32 GetPhaseShift() const;

    /**
     * @brief Gets the start time of the next segment.
     * @return the start time of the next segment.
     */
    float64 GetStart() const;

    /**
     * @brief Gets the size in bytes of the signal type.
     * @return the size in bytes of the signal type.
     */
    uint32 GetTypeMultiplier() const;

    /**
     * @brief Returns true if the time vector is to be retrieved from a signal (see SetTimeSignalMemory).
     * @return true if the time vector is to be retrieved from a signal (see SetTimeSignalMemory).
     */
    bool IsUseTimeVector() const;

    /**
     * @brief Gets the number of time the Execute method has been called between two segments.
     * @return the number of time the Execute method has been called between two segments.
     */
    uint64 GetNumberOfExecuteCalls() const;

private:

    /**
     * @brief Gets the time from the TimeSignalMemory.
     * @return  the time from the TimeSignalMemory.
     */
    uint64 GetTimeSignalMemoryTime() const;

    /**
     * The name of the MDSplus node
     */
    StreamString nodeName;

    /**
     * The node type (as defined in mdsobjects.h)
     */
    int32 nodeType;

    /**
     * Number of elements per sample
     */
    uint32 numberOfElements;

    /**
     * Number of samples to be stored (maybe 1 sample is an array or a matrix)
     */
    uint32 numberOfSamples;

    /**
     * Number of dimension of the current node
     */
    uint8 numberOfDimensions;

    /**
     * Segment dimensions. Indicates the dimensions of the data to be stored in the node.
     * segmentDim[0] is number of samples. segmentDim[1] is the number of elements of a row,
     * segmentDim[2] is the number of elements of a column (currently matrix not supported) then dim[2] = 1
     */
    int32 segmentDim[3];

    /**
     * Absolute start time of the next segment to be stored.
     */
    float64 start;

    /**
     * Period between samples of the signal to be stored in MDS+.
     */
    float64 period;

    /**
     * Number of bytes per sample of the signal type to be stored.
     */
    size_t typeMultiplier;

    /**
     * Period at which the Execute gets called
     */
    uint32 executePeriod;

    /**
     * True if the time vector is to be fed through an external signal.
     */
    bool useTimeVector;

    /**
     * Phase shift (in samples) of the first sample.
     */
    int32 phaseShift;

    /**
     * The MDSplus tree node where data is stored.
     */
    MDSplus::TreeNode *node;

    /**
     * Number of times Execute was called. Used to compute the time vector.
     */
    uint64 nOfExecuteCalls;

    /**
     * True if a decimated signal is also to be stored using makeSegmentMinMax
     */
    bool decimatedMinMax;

    /**
     * The MDS+ decimated node name for makeSegmentMinMax
     */
    StreamString decimatedNodeName;

    /**
     * The MDS+ decimated node for makeSegmentMinMax
     */
    MDSplus::TreeNode *decimatedNode;

    /**
     * Data is stored in this buffer before triggering a makeSegment/makeSegmentMinMax.
     * The segment write will be triggered when (currentBuffer == makeSegmentAfterNWrites)
     */
    void *bufferedData;

    /**
     * Current pointer where the bufferedData is being written to. Incremented every time the
     * Execute method is called.
     */
    uint32 currentBuffer;

    /**
     * Indicates the method to save data in MDSplus. automaticSegmentation = 1 --> putRow(), automaticSegmentation = 0 makeSegment()
     */
    bool automaticSegmentation;
    /**
     * Only write the segment to MDS+ after the Execute method has been called makeSegmentAfterNWrites times.
     */
    uint32 makeSegmentAfterNWrites;

    /**
     * MDS+ parameter which defines the down-sampling factor for signals with min-max decimation.
     */
    int32 minMaxResampleFactor;

    /**
     * Address of the signal memory where the data is going to be copied from.
     */
    void *signalMemory;

    /**
     * Address of the signal which provides the time information (only used when useTimeVector == true).
     */
    void *timeSignalMemory;

    /**
     * Multiplier to convert from the time signal units to seconds.
     */
    float64 timeSignalMultiplier;

    /**
     * Type of the time signal  (only used when useTimeVector == true).
     */
    TypeDescriptor timeSignalType;

    /**
     * Time at which the time signal was read for the last time.
     */
    uint64 lastWriteTimeSignal;

    /**
     * Set to true when all the data shall be flushed into MDSPlus, irrespectively of the fact that
     * not all the expected segments were written. Typically called when the experiment has ended.
     */
    bool flush;


    /**
     * For flushing if a discontinuity is found.
     */
    float64 discontinuityFactor;

    /**
     * @brief Save data in MDSplus using MDSplus::makeSegment() or MDSPlus::makeSegmentMaxMin()
     * @details the number of different time values per segment depends on Samples and the makeSegmentAfterNWrites
     * @return true if the data can be copied to the MDSplus database
     */
    bool ForceSegment();

    /**
     * @brief Save data in MDSplus tree using MDSplus::putRow()
     * @details the number of different time values per segment is automatically adjusted by MDSplus engine, they are directly
     * related with Samples.
     * @return true if the data can be copied to the MDSplus database.
     */
    bool AddDataToSegment();
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSWRITER_MDSWRITERNODE_H_ */
