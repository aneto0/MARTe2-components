/**
 * @file custom_datatype_1_3.h
 * @brief Header file for class custom_datatype_1_3
 * @date 08/01/2025
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

 * @details This header file contains the declaration of the class custom_datatype_1_3
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
                .memberName = UA_TYPENAME("x") 
                .memberType = &UA_TYPES[UA_TYPES_FLOAT],
                .padding = 0,
                .isArray = false,
                .isOptional = false 
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_FLOAT], 
                .padding = Point_padding_y, 
                .isArray = false,
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_FLOAT], 
                .padding = Point_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointType = {
        .typeName = UA_TYPENAME("Point")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4242}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5242}}, 
        .memSize = sizeof(Point),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = &Point_members[0]
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
                .memberName = UA_TYPENAME("x") 
                .memberType = &UA_TYPES[UA_TYPES_SBYTE], 
                .padding = 0,
                .isArray = false,
                .isOptional = false 
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_SBYTE], 
                .padding = PointArraySByte_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_SBYTE], 
                .padding = PointArraySByte_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArraySByteType = {
        .typeName = UA_TYPENAME("PointArraySByte") 
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4243}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5243}},
        .memSize = sizeof(PointArraySByte),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArraySByte_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_BYTE], 
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_BYTE], 
                .padding = PointArrayByte_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_BYTE], 
                .padding = PointArrayByte_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayByteType = {
        .typeName = UA_TYPENAME("PointArrayByte")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4244}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5244}},
        .memSize = sizeof(PointArrayByte),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArrayByte_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_INT16],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_INT16], 
                .padding = PointArrayInt16_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_INT16], 
                .padding  = PointArrayInt16_padding_z, 
                .isArray = true, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayInt16Type = {
        .typeName = UA_TYPENAME("PointArrayInt16") 
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4245}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5245}},
        .memSize = sizeof(PointArrayInt16),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false, 
        .membersSize = 3, 
        .members = PointArrayInt16_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_UINT16],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_UINT16], 
                .padding = PointArrayUInt16_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_UINT16], 
                .padding = PointArrayUInt16_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayUInt16Type = {
        .typeName = UA_TYPENAME("PointArrayUInt16")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4246}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5246}},
        .memSize = sizeof(PointArrayUInt16),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArrayUInt16_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_INT32],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_INT32], 
                .padding = PointArrayInt32_padding_y, 
                .isArray = false,
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_INT32], 
                .padding = PointArrayInt32_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayInt32Type = {
        .typeName = UA_TYPENAME("PointArrayInt32")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4247}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5247}},
        .memSize = sizeof(PointArrayInt32),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArrayInt32_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_UINT32],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_UINT32], 
                .padding = PointArrayUInt32_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_UINT32], 
                .padding = PointArrayUInt32_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayUInt32Type = {
        .typeName = UA_TYPENAME("PointArrayUInt32")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4248}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5248}},
        .memSize = sizeof(PointArrayUInt32),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArrayUInt32_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_INT64],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_INT64], 
                .padding = PointArrayInt64_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_INT64], 
                .padding = PointArrayInt64_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayInt64Type = {
        .typeName = UA_TYPENAME("PointArrayInt64")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4249}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {5249}},
        .memSize = sizeof(PointArrayInt64),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArrayInt64_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_UINT64],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_UINT64], 
                .padding = PointArrayUInt64_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_UINT64], 
                .padding = PointArrayUInt64_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayUInt64Type = {
        .typeName = UA_TYPENAME("PointArrayUInt64")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4250}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {0}},
        .memSize = sizeof(PointArrayUInt64),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = PointArrayUInt64_members
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_FLOAT],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_FLOAT], 
                .padding = PointArrayFloat_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_FLOAT], 
                .padding = PointArrayFloat_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayFloatType = {
        .typeName = UA_TYPENAME("PointArrayFloat")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4251}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {0}},
        .memSize = sizeof(PointArrayFloat),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = &PointArrayFloat_members[0]
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
                .memberName = UA_TYPENAME("x")
                .memberType = &UA_TYPES[UA_TYPES_DOUBLE],
                .padding = 0,
                .isArray = false,
                .isOptional = false
        },

        /* y */
        {
                .memberName = UA_TYPENAME("y")
                .memberType = &UA_TYPES[UA_TYPES_DOUBLE], 
                .padding = PointArrayDouble_padding_y, 
                .isArray = false, 
                .isOptional = false
        },

        /* z */
        {
                .memberName = UA_TYPENAME("z")
                .memberType = &UA_TYPES[UA_TYPES_DOUBLE], 
                .padding = PointArrayDouble_padding_z, 
                .isArray = false, 
                .isOptional = false
        }
};

static const UA_DataType PointArrayDoubleType = {
        .typeName = UA_TYPENAME("PointArrayDouble")
        .typeId = {1, UA_NODEIDTYPE_NUMERIC, {4252}},
        .binaryEncodingId = {1, UA_NODEIDTYPE_NUMERIC, {0}},
        .memSize = sizeof(PointArrayDouble),
        .typeKind = UA_DATATYPEKIND_STRUCTURE,
        .pointerFree = true,
        .overlayable = false,
        .membersSize = 3,
        .members = &PointArrayDouble_members[0]
};
