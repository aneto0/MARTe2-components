/**
 * @file FormatDescriptor.h
 * @brief Header file for class FormatDescriptor
 * @date 26/08/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class FormatDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FORMATDESCRIPTOR_H_
#define FORMATDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "BitRange.h"
#include "BitBoolean.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

/**
 * Notations used for float types representation.
 */
typedef uint3 FloatNotation;

/**
 * Fixed point notation.
 * The precision is the number of decimals (i.e. 123.45 => precision=2).
 */
const FloatNotation FixedPointNotation = 0u;

/**
 * Fixed point relative notation.
 * The precision is the total number of significant digits (i.e. 123.45 => precision=5).
 */
const FloatNotation FixedPointRNotation = 1u;

/**
 * Exponent notation.
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const FloatNotation ExponentNotation = 2u;

/**
 * Engineering notation.
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const FloatNotation EngineeringNotation = 3u;

/**
 * Engineering notation with size symbols.
 * Symbols defined within [E+24, E-24]
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const FloatNotation SmartNotation = 6u;

/**
 * Most meaningful notation fitting within constraints
 * Choice among FixedPointRNotation, ExponentNotation and SmartNotation.
 */
const FloatNotation CompactNotation = 7u;

/**
 * Notations used for binary representations.
 */
typedef uint2 BinaryNotation;

/**
 * Decimal notation.
 */
const BinaryNotation DecimalNotation = 0u;

/**
 * Hexadecimal notation.
 */
const BinaryNotation HexNotation = 1u;

/**
 * Binary notation.
 */
const BinaryNotation BitNotation = 2u;

/**
 * Octal notation.
 */
const BinaryNotation OctalNotation = 3u;

/**
 * Enumeration-like type for encoding the desired printing action */
typedef uint3 DesiredAction;

/**
 * The user intention was to print whatever is passed
 * the value has to be 0 as it used to mark no decision and it will or-combine correctly with the other constants
 */
const DesiredAction PrintAnything = 0u;

/**
 * The user intention was to print an integer (of any size)
 */
const DesiredAction PrintInteger = 1u;

/**
 * The user intention was to print a float type
 */
const DesiredAction PrintFloat = 2u;

/**
 * The user intention was to print a pointer
 */
const DesiredAction PrintPointer = 3u;

/**
 * The user intention was to print a sequence of characters (from a char * or char[] or Stream ... )
 */
const DesiredAction PrintString = 4u;

/**
 * The user intention was to print information about the object
 */
const DesiredAction PrintInfo = 5u;

/**
 * The user intention was to print structural information about object
 */
const DesiredAction PrintStruct = 6u;

/**
 * @brief This class defines a format descriptor.
 * @details This class contains a structure to store the informations the print of each type.
 * @details It defines also functions due to fill the structure getting the information from
 * a printf like constant char8* string which specifies the desired print format.
 * @details Defines the conversion from AnyType to String based types
 */
class FormatDescriptor {
public:

    /**
     * @brief Converts a const char8* string to a FormatDescriptor structure.
     * @param[in] string is the string which contains a printf like format.
     * @return false if string is null or empty.
     *
     * @details Takes a printf like string already pointing at the character after % (see below format)
     * and parses it recovering all the useful information, discarding all redundant ones,
     * and fills up the fields in this structure. At the end the pointer string is moved to the next
     * character after the parsed block.
     */
    /*
     * The overall printf-like format supported is the following:
     * %[flags][width][.precision]type
     * Note that the full printf would be this:
     * %[parameter][flags][width][.precision][length]type
     * !which is not supported!

     * [flags]: // slightly different from standard printf notation
     * ' ' Activates padding:
     * fills up to width using spaces
     * -  Left-align : put padding spaces after printing the object
     * #   Activate fullNotation:
     * + in front of integers
     * 0x/0b/0o in front of Hex/octal/binary
     * 0  Prepends zeros for Hex Octal and Binary notations (binaryPadded activated)
     * Number of zeros depends on number precision and chosen notation (64 bit int and binary notation = up to 64 zeros)

     * [width].[precision]  two numbers
     * [width] specifies the MAXIMUM or EXACT number of characters to output, depending on the padding [flags] being set on or not
     * NOTE that in a normal printf this would be the MINIMUM or EXACT number of characters...
     * [Precision]
     * This is the minimum number of meaningful digits used to represent a number
     * Differently from printf this includes numbers before .
     * if the exact representation of the number uses less digits [precision] is not considered
     * if [width] is such that a numeric representation with the given precision cannot be fully represented than the number is replaced with a ?
     * type
     * This is one character among the following
     *
     * Note if data type does not match the selection below a warning will be issued but correct print will be performed
     * d,i,u --> intention integer printing
     * s --> intention string printing (any source of characters is ok)
     * f --> fixed point numeric format selected - absolute precision mode : precision is the number of digits below 1
     * F --> fixed point relative precision selected
     * e --> exponential format
     * E --> engineering format
     * g --> smart format   - like E but replaces E-12 to E+12 with one of the following letters "TGMK munp"
     * G --> compact format
     * x,p --> activate hexadecimal display (p activates full notation: header+trailing zeros)
     * p --> pointer printing intention (not compatible with integers)
     * o --> activate octal display
     * b --> activate binary display
     * ######## NON STANDARD TBD
     * ! --> Any type is fine - prints the default value
     * ? --> Any type is fine - prints information about the type
     * @ --> Any type is fine - prints full content in case of known structures
     */
    bool InitialiseFromString(const char8 *&string);

    /**
     * @brief Default constructor.
     *
     * @details Precision = -1 means default precision for float types.
     */
    inline FormatDescriptor();

    /**
     * @brief Constructor from unsigned integer.
     * @details Just copy bit by bit.
     * @param[in] x contains the bits for the FormatDescriptor structure.
     */
    inline FormatDescriptor(const uint32 x);

    /**
     * @brief Copy operator.
     * @param[in] src is the format descriptor to be copied in this.
     */
    inline FormatDescriptor& operator=(const FormatDescriptor &src);

    /**
     * @brief Bitwise or operator
     * @param[in] src is format descriptor argument.
     */
    inline void operator |=(const FormatDescriptor &src);

    /**
     * @brief Constructor field by field.
     * @param[in] desiredActionToSet specifies the action to be performed (i.e. print an integer or print a string)
     * @param[in] sizeToSet is the desired maximum size for the print.
     * @param[in] precisionToSet specifies the precision for float number print.
     * @param[in] isPadded specifies if the space remained until the desired size must be filled with ' '.
     * @param[in] isLeftAligned specifies if the padding is at the beginning (false) or at the end (true).
     * @param[in] floatNotationToSet specifies the desired notation for float numbers (fixed point, exponential, ...)
     * @param[in] binaryNotationToSet specifies the desired notation for integer numbers (decimal, exadecimal, ...)
     * @param[in] isBinaryPadded specifies if the trailing zeros must be added for integer prints.
     * @param[in] isFullNotation specifies if the header (0x, 0o, 0b) must be added for integer prints.
     */
    inline FormatDescriptor(const DesiredAction &desiredActionToSet,
                            const uint8 sizeToSet,
                            const uint8 precisionToSet,
                            const bool isPadded,
                            const bool isLeftAligned,
                            const FloatNotation &floatNotationToSet,
                            const BinaryNotation &binaryNotationToSet,
                            const bool isBinaryPadded,
                            const bool isFullNotation);

    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {

        /**
         * The whole set of bits fits in a 32 bit unsigned
         */
        uint32 format_as_uint32;

        /**
         * The maximum size of representation.
         * 0 = unlimited, maximum size = 255.
         */
        BitRange<uint32, 8u, 0u> size;

        /**
         * The minimum (whenever applicable) number of meaningful digits (unless overridden by width)\n
         * max 64 differently from printf this includes characters before comma\n
         * excludes characters used for the exponents or for the sign and the .\n
         * 0.34 has precision 2        -> (precision =8)  0.3400000\n
         * 234 has precision 3         -> (precision =8)  234.00000\n
         * 2345678000 has precision 10 -> (precision =8) unchanged still precision 10\n
         * 2.345678E9 has precision 7  -> (precision =8) 2.3456780E9\n
         * 234 (int) has precision 3   -> (precision =8) unchanged  still precision 3\n
         * 0x4ABCD has precision 5     -> (precision =8) unchanged  still precision 5\n
         */
        BitRange<uint32, 8u, 8u> precision;

        /**
         * True means produce a number of characters equal to width
         * fill up using spaces.
         */
        BitBoolean<uint32, 16u> padded;

        /**
         * True means to produce pad spaces after printing the object representation.
         */
        BitBoolean<uint32, 17u> leftAligned;

        /**
         * In case of a float, this field is used to determine how to print it.
         */
        BitRange<uint32, 3u, 18u> floatNotation;

        /**
         * The notation used for binary representation.
         */
        BitRange<uint32, 2u, 21u> binaryNotation;

        /**
         * Fills the number on the left with 0s up to the full representation.\n
         * Number of zeros depends on the size of the number (hex 64 bit ==> numbers+trailing zero = 16).\n
         */
        BitBoolean<uint32, 23u> binaryPadded;

        /**
         * Only meaningful for numbers.
         * Add the missing + or 0x 0B or 0o as header.
         */
        BitBoolean<uint32, 24u> fullNotation;

        /**
         * Specifies the type that the user wants to print.
         * This can be different from what the system can do,
         * i.e. print an integer and a float is passed.
         */
        BitRange<uint32, 3u, 25u> desiredAction;

        /**
         * Unnecessary bits.
         */
        BitRange<uint32, 4u, 28u> spareBits;

    };

};

/**
 * Default precision: is set if the user does not specify the desired precision.
 * A fixed default precision will be used depending on the type to be printed.
 */
const uint32 defaultPrecision = 0xffu;

/**
 * Default Format Descriptor.
 */
static const FormatDescriptor standardFormatDescriptor(PrintAnything, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


FormatDescriptor::FormatDescriptor() {
    desiredAction = PrintAnything;
    size = 0u;
    precision = defaultPrecision;
    padded = false;
    leftAligned = false;
    floatNotation = FixedPointNotation;
    binaryNotation = DecimalNotation;
    binaryPadded = false;
    fullNotation = false;
    spareBits = 0u;
}

FormatDescriptor::FormatDescriptor(const uint32 x) {
    format_as_uint32 = x;
}

FormatDescriptor& FormatDescriptor::operator=(const FormatDescriptor &src) {
    if (this != &src) {
        format_as_uint32 = src.format_as_uint32;
    }
    return *this;
}

void FormatDescriptor::operator |=(const FormatDescriptor &src) {
    format_as_uint32 |= src.format_as_uint32;
}

FormatDescriptor::FormatDescriptor(const DesiredAction &desiredActionToSet,
                                   const uint8 sizeToSet,
                                   const uint8 precisionToSet,
                                   const bool isPadded,
                                   const bool isLeftAligned,
                                   const FloatNotation &floatNotationToSet,
                                   const BinaryNotation &binaryNotationToSet,
                                   const bool isBinaryPadded,
                                   const bool isFullNotation) {

    desiredAction = desiredActionToSet;
    size = sizeToSet;
    precision = precisionToSet;
    padded = isPadded;
    leftAligned = isLeftAligned;
    floatNotation = floatNotationToSet;
    binaryNotation = binaryNotationToSet;
    binaryPadded = isBinaryPadded;
    fullNotation = isFullNotation;
    spareBits = 0u;
}

}

#endif /* FORMATDESCRIPTOR_H_ */