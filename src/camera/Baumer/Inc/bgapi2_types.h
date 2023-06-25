/**
  Copyright 2014-2018 Baumer Optronic
  \file bgapi2_types.h
*/

#ifndef TLCONSUMER_BGAPI2_GENICAM_BGAPI2_TYPES_H_
#define TLCONSUMER_BGAPI2_GENICAM_BGAPI2_TYPES_H_

#ifndef BGAPI2_NO_DECL
#ifdef _WIN32
#ifdef BGAPI2_GENICAM_EXPORTS
#define BGAPI2_DECL __declspec(dllexport)
#else
#define BGAPI2_DECL __declspec(dllimport)
#endif
#elif defined(__GNUC__) && (__GNUC__ >= 4) && (defined(__linux__) || defined(__APPLE__))
#define BGAPI2_DECL __attribute__((visibility("default")))
#endif
#endif
#ifndef BGAPI2_DECL
#define BGAPI2_DECL
#endif

#ifndef BGAPI2_C_NO_DECL
#ifdef _WIN32
#ifdef BGAPI2_GENICAM_EXPORTS
#define BGAPI2_C_DECL __declspec(dllexport)
#else
#define BGAPI2_C_DECL __declspec(dllimport)
#endif
#elif defined(__GNUC__) && (__GNUC__ >= 4) && (defined(__linux__) || defined(__APPLE__))
#define BGAPI2_C_DECL __attribute__((visibility("default")))
#endif
#endif
#ifndef BGAPI2_C_DECL
#define BGAPI2_C_DECL
#endif

#if defined(_WIN32) && (defined (_M_IX86) || defined (__i386__))
#define BGAPI2CALL __stdcall
#else
#define BGAPI2CALL
#endif

#if !defined(UNREFERENCED_PARAMETER) && !defined(_MSC_VER)
#   define UNREFERENCED_PARAMETER(x)    (x)=(x)
#endif  // UNREFERENCED_PARAMETER

#ifdef _WIN32
typedef __int64                bo_int64;
typedef unsigned __int64       bo_uint64;
typedef __int32                bo_int;
typedef unsigned __int32       bo_uint;
typedef __int16                bo_short;
typedef unsigned __int16       bo_ushort;
typedef __int8                 bo_char;
typedef unsigned __int8        bo_uchar;
#else
#ifdef __cplusplus
#include <iostream>
#endif /* #ifdef __cplusplus */
#include <inttypes.h>
typedef int64_t                bo_int64;
typedef uint64_t               bo_uint64;
typedef int32_t                bo_int;
typedef uint32_t               bo_uint;
typedef int16_t                bo_short;
typedef uint16_t               bo_ushort;
typedef int8_t                 bo_char;
typedef uint8_t                bo_uchar;
#endif

typedef char*               bo_string;
#ifdef __cplusplus
typedef bool bo_bool;
#else
typedef bo_uchar bo_bool;
#endif
typedef double              bo_double;

/**
    \ingroup    BGAPI2
    \brief      An enumeration containing return result codes
*/
enum BGAPI2_RESULT_LIST {
    BGAPI2_RESULT_SUCCESS = 0,                  ///< Success
    BGAPI2_RESULT_ERROR = -1001,                ///< Error
    BGAPI2_RESULT_NOT_INITIALIZED = -1002,      ///< Not initialized
    BGAPI2_RESULT_NOT_IMPLEMENTED = -1003,      ///< Not implemented
    BGAPI2_RESULT_RESOURCE_IN_USE = -1004,      ///< Resource in use
    BGAPI2_RESULT_ACCESS_DENIED = -1005,        ///< Access denied
    BGAPI2_RESULT_INVALID_HANDLE = -1006,       ///< Invalid pointer
    BGAPI2_RESULT_NO_DATA = -1008,              ///< No data
    BGAPI2_RESULT_INVALID_PARAMETER = -1009,    ///< Invalid parameter
    // GC_ERR_IO                    = -1010,
    BGAPI2_RESULT_TIMEOUT = -1011,              ///< Timeout
    BGAPI2_RESULT_ABORT = -1012,                ///< Abort
    BGAPI2_RESULT_INVALID_BUFFER = -1013,       ///< Invalid buffer
    BGAPI2_RESULT_NOT_AVAILABLE = -1014,        ///< Not available
    // GC_ERR_INVALID_ADDRESS            = -1015, /* GenTL v1.3 */
    // GC_ERR_BUFFER_TOO_SMALL           = -1016, /* GenTL v1.4 */
    // GC_ERR_INVALID_INDEX              = -1017, /* GenTL v1.4 */
    // GC_ERR_PARSING_CHUNK_DATA         = -1018, /* GenTL v1.4 */
    BGAPI2_RESULT_OBJECT_INVALID = -1098,       ///< Object invalid
    BGAPI2_RESULT_LOWLEVEL_ERROR = -1099        ///< Low-level error
};

typedef bo_int BGAPI2_RESULT;

#define BGAPI2_PAYLOADTYPE_UNKNOWN "Unknown"
#define BGAPI2_PAYLOADTYPE_IMAGE "Image"
#define BGAPI2_PAYLOADTYPE_RAW_DATA "RawData"
#define BGAPI2_PAYLOADTYPE_FILE "File"
#define BGAPI2_PAYLOADTYPE_CHUNK_DATA "ChunkData"
#define BGAPI2_PAYLOADTYPE_CUSTOM_ID "CustomID_1000"

#define BGAPI2_PAYLOADTYPE_IMAGE_EXT "ImageExt"        /* PAYLOADTYPE_IMAGE_EXT */
// other custom id's were delivered in the following format CustomID_xxxx starting with CustomID_1000

#define BGAPI2_NODEINTERFACE_CATEGORY "ICategory"
#define BGAPI2_NODEINTERFACE_INTEGER "IInteger"
#define BGAPI2_NODEINTERFACE_REGISTER "IRegister"
#define BGAPI2_NODEINTERFACE_BOOLEAN "IBoolean"
#define BGAPI2_NODEINTERFACE_COMMAND "ICommand"
#define BGAPI2_NODEINTERFACE_FLOAT "IFloat"
#define BGAPI2_NODEINTERFACE_ENUMERATION "IEnumeration"
#define BGAPI2_NODEINTERFACE_STRING "IString"
#define BGAPI2_NODEINTERFACE_PORT "IPort"

#define BGAPI2_NODEVISIBILITY_BEGINNER "Beginner"
#define BGAPI2_NODEVISIBILITY_EXPERT "Expert"
#define BGAPI2_NODEVISIBILITY_GURU "Guru"
#define BGAPI2_NODEVISIBILITY_INVISIBLE "Invisible"

#define BGAPI2_NODEACCESS_READWRITE "RW"
#define BGAPI2_NODEACCESS_READONLY "RO"
#define BGAPI2_NODEACCESS_WRITEONLY "WO"
#define BGAPI2_NODEACCESS_NOTAVAILABLE "NA"
#define BGAPI2_NODEACCESS_NOTIMPLEMENTED "NI"

#define BGAPI2_NODEREPRESENTATION_LINEAR "Linear"            // valid for BGAPI2_NODEINTERFACE_INTEGER, .._FLOAT
#define BGAPI2_NODEREPRESENTATION_LOGARITHMIC "Logarithmic"  // valid for BGAPI2_NODEINTERFACE_INTEGER, .._FLOAT
#define BGAPI2_NODEREPRESENTATION_PURENUMBER "PureNumber"    // valid for BGAPI2_NODEINTERFACE_INTEGER, .._FLOAT
#define BGAPI2_NODEREPRESENTATION_BOOLEAN "Boolean"          // valid for BGAPI2_NODEINTERFACE_INTEGER
#define BGAPI2_NODEREPRESENTATION_HEXNUMBER "HexNumber"      // valid for BGAPI2_NODEINTERFACE_INTEGER
#define BGAPI2_NODEREPRESENTATION_IPV4ADDRESS "IPV4Address"  // valid for BGAPI2_NODEINTERFACE_INTEGER
#define BGAPI2_NODEREPRESENTATION_MACADDRESS "MACAddress"    // valid for BGAPI2_NODEINTERFACE_INTEGER

#endif  // TLCONSUMER_BGAPI2_GENICAM_BGAPI2_TYPES_H_
