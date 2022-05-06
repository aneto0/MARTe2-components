/**
 * @file FlattenedStructIOGAM.h
 * @brief Header file for class FlattenedStructIOGAM
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

 * @details This header file contains the declaration of the class FlattenedStructIOGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FLATTENED_STRUCT_IOGAM_H_
#define FLATTENED_STRUCT_IOGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "Introspection.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief An IOGAM that flattens (and copies) an input signal with arrays of structures into arrays of basic types.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 *
 * +Types = {
 *   Class = ReferenceContainer
 *   +MyType0 = {
 *     Class = IntrospectionStructure
 *     MyUInt16 = {
 *       Type = uint16
 *       NumberOfElements = 1
 *     }
 *   }
 *   +MyType1 = {
 *     Class = IntrospectionStructure
 *     MyUInt32 = {
 *       Type = uint32
 *       NumberOfElements = 1
 *     }
 *     MyFloat32Array = {
 *       Type = float32
 *       NumberOfElements = 8
 *     }
 *     MyType0Array = {
 *       Type = MyType0
 *       NumberOfElements = 2
 *     }
 *   }
 *   +MyType2 = {
 *     Class = IntrospectionStructure
 *     MyFloat64 = {
 *       Type = float64
 *       NumberOfElements = 1
 *     }
 *     MyType1Array= {
 *       Type = MyType1
 *       NumberOfElements = 2
 *     }
 *   }
 * }
 * 
 * +FlatIOGAM = {
 *   Class = FlattenedStructIOGAM
 *   InputSignals = {
 *     StructArrayType = { //Exactly one structure signal shall be defined.
 *       Type = MyType2 //The Type shall be structured and is required.
 *       DataSource = DDB1 //Compulsory.
 *     } 
 *   }
 *   OutputSignals = {
 *     StructArrayType_MyFloat64 = {
 *       Type = float64
 *       NumberOfElements = 1
 *     }
 *     StructArrayType_MyType1Array_MyUInt32s = {
 *       Type = uint32
 *       NumberOfElements = 2
 *     }
 *     StructArrayType_MyType1Array_MyFloat32s = {
 *       Type = float32
 *       NumberOfElements = 16
 *     }
 *     StructArrayType_MyType1Array_MyType0Array_MyUInt16s = {
 *       Type = uint16
 *       NumberOfElements = 4
 *     }
 *   }
 * }
 * </pre>
 * <pre>
 * Will be "automatically transformed" into:
 * +FlatIOGAM = {
 *   Class = FlattenedStructIOGAM
 *   InputSignals = {
 *     S0 = {
 *       Type = float64
 *       Alias = "StructArrayType.MyFloat64"
 *       DataSource = DDB1
 *     }
 *     S1 = {
 *       Type = uint32
 *       Alias = "StructArrayType.MyType1Array[0].MyUInt32"
 *       DataSource = DDB1
 *     }
 *     S2 = {
 *       Type = uint32
 *       Alias = "StructArrayType.MyType1Array[1].MyUInt32"
 *       DataSource = DDB1
 *     }
 *     S3 = {
 *       Type = float32
 *       Alias = "StructArrayType.MyType1Array[0].MyFloat32"
 *       NumberOfElements = 8
 *       DataSource = DDB1
 *     }
 *     S4 = {
 *       Type = float32
 *       Alias = "StructArrayType.MyType1Array[1].MyFloat32"
 *       NumberOfElements = 8
 *       DataSource = DDB1
 *     }
 *     S5 = {
 *       Type = uint16
 *       Alias = "StructArrayType.MyType1Array[0].MyType0Array[0].UInt16"
 *       DataSource = DDB1
 *     }
 *     S6 = {
 *       Type = uint16
 *       Alias = "StructArrayType.MyType1Array[0].MyType0Array[1].UInt16"
 *       DataSource = DDB1
 *     }
 *     S7 = {
 *       Type = uint16
 *       Alias = "StructArrayType.MyType1Array[1].MyType0Array[0].UInt16"
 *       DataSource = DDB1
 *     }
 *     S8 = {
 *       Type = uint16
 *       Alias = "StructArrayType.MyType1Array[1].MyType0Array[1].UInt16"
 *       DataSource = DDB1
 *     }
 *   }
 *   OutputSignals = {
     *   ...
 * </pre>
 */
class FlattenedStructIOGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    FlattenedStructIOGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~FlattenedStructIOGAM();

    /**
     * @brief see GAM::Initialise.
     * @details Flattens the input structured signal with the rules described above.
     * @return true if the signal is successfully flattened.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Checks that the total input signal memory size is equal to the total output signal memory size.
     * @return true is the pre-conditions are met.
     */
    virtual bool Setup();

    /**
     * @brief Copies the input signals memory to the output signal memory.
     * @return true if all the signals memory can be successfully copied.
     */
    virtual bool Execute();

private:
    /**
     * @brief Helper method that returns the number of elements of an IntrospectionEntry.
     * @param[in] entry the element to query.
     * @return the number of elements.
     */
    uint32 GetNumberOfElements(const IntrospectionEntry &entry) const;

    /**
     * @brief Recursively transverses the input structure and stores the path leading to each basic type member on the signalPathCDB. Once a basic type member is found, it is added to the gamInputSignals.
     * @param[in] intro the structure to visit.
     * @param[in, out] signalPathCDB stores the path leading to each basic type member (which will be the Alias).
     * @param[out] gamInputSignals where to write the discovered signal members.
     * @param[in,out] signalCounter number of signals found.
     * @param[in] dataSourceName the signals DataSource.
     * @return true if all the signals are successfully added.
     */
    bool TransverseStructure(const Introspection *intro, ConfigurationDatabase &signalPathCDB, StructuredDataI &gamInputSignals, uint32 &signalCounter, const char8 * const dataSourceName);

    /**
     * @brief Helper recursive method called by the TransverseStructure everytime a basic type member is found. The path is visited, until the BasicType is found, in order to construct the Alias field.
     * @param[in] path the full signal path (as a tree).
     * @param[in] fullPathName the full signal path (as a string).
     * @param[out] gamInputSignals where to write the signal (and its properties).
     * @param[in, out] signalCounter the number of the signal to add. Once its added it will be incremented by one.
     * @param[in] dataSourceName the name of the DataSource.
     * @return true if the signal is successfully written.
     */
    bool WriteSignal(ConfigurationDatabase &path, const char8 * const fullPathName, StructuredDataI &gamInputSignals, uint32 &signalCounter, const char8 * const dataSourceName);

    /**
     * Total number of bytes to copy.
     */
    uint32 totalSignalsByteSize;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FLATTENED_STRUCT_IOGAM_H_ */

