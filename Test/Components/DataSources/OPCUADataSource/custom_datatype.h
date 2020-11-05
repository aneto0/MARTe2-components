/**
 * @file custom_datatype.h
 * @brief Header file for class custom_datatype
 * @date 30/10/2019
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class custom_datatype
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */


#include "open62541.h"

/* Point */
typedef struct {
    UA_Float x;
    UA_Float y;
    UA_Float z;
} Point;

/* The datatype description for the Point datatype */
#define Point_padding_y offsetof(Point,y) - offsetof(Point,x) - sizeof(UA_Float)
#define Point_padding_z offsetof(Point,z) - offsetof(Point,y) - sizeof(UA_Float)

static UA_DataTypeMember Point_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_FLOAT,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_FLOAT, Point_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_FLOAT, Point_padding_z, true, false
        }
};

static const UA_DataType PointType = {
        UA_TYPENAME("Point")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4242}}, /* .typeId */
        sizeof(Point),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        Point_members
};


/* SBYTE */
typedef struct {
    UA_SByte x;
    UA_SByte y;
    size_t zLength;
    UA_SByte* z;
} PointArraySByte;

#define PointArraySByte_padding_y offsetof(PointArraySByte,y) - offsetof(PointArraySByte,x) - sizeof(UA_SByte)
#define PointArraySByte_padding_z offsetof(PointArraySByte,zLength) - offsetof(PointArraySByte,y) - sizeof(UA_SByte)

static UA_DataTypeMember PointArraySByte_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_SBYTE,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_SBYTE, PointArraySByte_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_SBYTE, PointArraySByte_padding_z, true, true
        }
};

static const UA_DataType PointArraySByteType = {
        UA_TYPENAME("PointArraySByte")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4243}}, /* .typeId */
        sizeof(PointArraySByte),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArraySByte_members
};

/* BYTE */
typedef struct {
    UA_Byte x;
    UA_Byte y;
    UA_Byte zLength;
    UA_Byte* z;
} PointArrayByte;

#define PointArrayByte_padding_y offsetof(PointArrayByte,y) - offsetof(PointArrayByte,x) - sizeof(UA_Byte)
#define PointArrayByte_padding_z offsetof(PointArrayByte,zLength) - offsetof(PointArrayByte,y) - sizeof(UA_Byte)

static UA_DataTypeMember PointArrayByte_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_BYTE,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_BYTE, PointArrayByte_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_BYTE, PointArrayByte_padding_z, true, true
        }
};

static const UA_DataType PointArrayByteType = {
        UA_TYPENAME("PointArrayByte")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4244}}, /* .typeId */
        sizeof(PointArrayByte),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayByte_members
};

/* INT16 */
typedef struct {
    UA_Int16 x;
    UA_Int16 y;
    size_t zLength;
    UA_Int16* z;
} PointArrayInt16;

#define PointArrayInt16_padding_y offsetof(PointArrayInt16,y) - offsetof(PointArrayInt16,x) - sizeof(UA_Int16)
#define PointArrayInt16_padding_z offsetof(PointArrayInt16,zLength) - offsetof(PointArrayInt16,y) - sizeof(UA_Int16)

static UA_DataTypeMember PointArrayInt16_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_INT16,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_INT16, PointArrayInt16_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_INT16, PointArrayInt16_padding_z, true, true
        }
};

static const UA_DataType PointArrayInt16Type = {
        UA_TYPENAME("PointArrayInt16")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4245}}, /* .typeId */
        sizeof(PointArrayInt16),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayInt16_members
};

/* UINT16 */
typedef struct {
    UA_UInt16 x;
    UA_UInt16 y;
    size_t zLength;
    UA_UInt16* z;
} PointArrayUInt16;

#define PointArrayUInt16_padding_y offsetof(PointArrayUInt16,y) - offsetof(PointArrayUInt16,x) - sizeof(UA_UInt16)
#define PointArrayUInt16_padding_z offsetof(PointArrayUInt16,zLength) - offsetof(PointArrayUInt16,y) - sizeof(UA_UInt16)

static UA_DataTypeMember PointArrayUInt16_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_UINT16,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_UINT16, PointArrayUInt16_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_UINT16, PointArrayUInt16_padding_z, true, true
        }
};

static const UA_DataType PointArrayUInt16Type = {
        UA_TYPENAME("PointArrayUInt16")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4246}}, /* .typeId */
        sizeof(PointArrayUInt16),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayUInt16_members
};

/* INT32 */
typedef struct {
    UA_Int32 x;
    UA_Int32 y;
    size_t zLength;
    UA_Int32* z;
} PointArrayInt32;

#define PointArrayInt32_padding_y offsetof(PointArrayInt32,y) - offsetof(PointArrayInt32,x) - sizeof(UA_Int32)
#define PointArrayInt32_padding_z offsetof(PointArrayInt32,zLength) - offsetof(PointArrayInt32,y) - sizeof(UA_Int32)

static UA_DataTypeMember PointArrayInt32_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_INT32,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_INT32, PointArrayInt32_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_INT32, PointArrayInt32_padding_z, true, true
        }
};

static const UA_DataType PointArrayInt32Type = {
        UA_TYPENAME("PointArrayInt32")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4247}}, /* .typeId */
        sizeof(PointArrayInt32),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayInt32_members
};

/* UINT32 */
typedef struct {
    UA_UInt32 x;
    UA_UInt32 y;
    size_t zLength;
    UA_UInt32* z;
} PointArrayUInt32;

#define PointArrayUInt32_padding_y offsetof(PointArrayUInt32,y) - offsetof(PointArrayUInt32,x) - sizeof(UA_UInt32)
#define PointArrayUInt32_padding_z offsetof(PointArrayUInt32,zLength) - offsetof(PointArrayUInt32,y) - sizeof(UA_UInt32)

static UA_DataTypeMember PointArrayUInt32_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_UINT32,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_UINT32, PointArrayUInt32_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_UINT32, PointArrayUInt32_padding_z, true, true
        }
};

static const UA_DataType PointArrayUInt32Type = {
        UA_TYPENAME("PointArrayUInt32")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4248}}, /* .typeId */
        sizeof(PointArrayUInt32),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayUInt32_members
};

/* INT64 */
typedef struct {
    UA_Int64 x;
    UA_Int64 y;
    size_t zLength;
    UA_Int64* z;
} PointArrayInt64;

#define PointArrayInt64_padding_y offsetof(PointArrayInt64,y) - offsetof(PointArrayInt64,x) - sizeof(UA_Int64)
#define PointArrayInt64_padding_z offsetof(PointArrayInt64,zLength) - offsetof(PointArrayInt64,y) - sizeof(UA_Int64)

static UA_DataTypeMember PointArrayInt64_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_INT64,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_INT64, PointArrayInt64_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_INT64, PointArrayInt64_padding_z, true, true
        }
};

static const UA_DataType PointArrayInt64Type = {
        UA_TYPENAME("PointArrayInt64")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4249}}, /* .typeId */
        sizeof(PointArrayInt64),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayInt64_members
};

/* UINT64 */
typedef struct {
    UA_UInt64 x;
    UA_UInt64 y;
    size_t zLength;
    UA_UInt64* z;
} PointArrayUInt64;

#define PointArrayUInt64_padding_y offsetof(PointArrayUInt64,y) - offsetof(PointArrayUInt64,x) - sizeof(UA_UInt64)
#define PointArrayUInt64_padding_z offsetof(PointArrayUInt64,zLength) - offsetof(PointArrayUInt64,y) - sizeof(UA_UInt64)

static UA_DataTypeMember PointArrayUInt64_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_UINT64,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_UINT64, PointArrayUInt64_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_UINT64, PointArrayUInt64_padding_z, true, true
        }
};

static const UA_DataType PointArrayUInt64Type = {
        UA_TYPENAME("PointArrayUInt64")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4250}}, /* .typeId */
        sizeof(PointArrayUInt64),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayUInt64_members
};

/* FLOAT */
typedef struct {
    UA_Float x;
    UA_Float y;
    size_t zLength;
    UA_Float* z;
} PointArrayFloat;

#define PointArrayFloat_padding_y offsetof(PointArrayFloat,y) - offsetof(PointArrayFloat,x) - sizeof(UA_Float)
#define PointArrayFloat_padding_z offsetof(PointArrayFloat,zLength) - offsetof(PointArrayFloat,y) - sizeof(UA_Float)

static UA_DataTypeMember PointArrayFloat_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_FLOAT,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_FLOAT, PointArrayFloat_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_FLOAT, PointArrayFloat_padding_z, true, true
        }
};

static const UA_DataType PointArrayFloatType = {
        UA_TYPENAME("PointArrayFloat")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4251}}, /* .typeId */
        sizeof(PointArrayFloat),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayFloat_members
};

/* DOUBLE */
typedef struct {
    UA_Double x;
    UA_Double y;
    size_t zLength;
    UA_Double* z;
} PointArrayDouble;

#define PointArrayDouble_padding_y offsetof(PointArrayDouble,y) - offsetof(PointArrayDouble,x) - sizeof(UA_Double)
#define PointArrayDouble_padding_z offsetof(PointArrayDouble,zLength) - offsetof(PointArrayDouble,y) - sizeof(UA_Double)

static UA_DataTypeMember PointArrayDouble_members[3] = {
        /* x */
        {
                UA_TYPENAME("x") /* .memberName */
                UA_TYPES_DOUBLE,  /* .memberTypeIndex, points into UA_TYPES since namespaceZero is true */
                0,               /* .padding */
                true,            /* .namespaceZero, see .memberTypeIndex */
                false            /* .isArray */
        },

        /* y */
        {
                UA_TYPENAME("y")
                UA_TYPES_DOUBLE, PointArrayDouble_padding_y, true, false
        },

        /* z */
        {
                UA_TYPENAME("z")
                UA_TYPES_DOUBLE, PointArrayDouble_padding_z, true, true
        }
};

static const UA_DataType PointArrayDoubleType = {
        UA_TYPENAME("PointArrayDouble")             /* .typeName */
        {1, UA_NODEIDTYPE_NUMERIC, {4252}}, /* .typeId */
        sizeof(PointArrayDouble),                   /* .memSize */
        0,                               /* .typeIndex, in the array of custom types */
        UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
        true,                            /* .pointerFree */
        false,                           /* .overlayable (depends on endianness and
                                         the absence of padding) */
        3,                               /* .membersSize */
        0,                               /* .binaryEncodingId, the numeric
                                         identifier used on the wire (the
                                         namespaceindex is from .typeId) */
        PointArrayDouble_members
};
