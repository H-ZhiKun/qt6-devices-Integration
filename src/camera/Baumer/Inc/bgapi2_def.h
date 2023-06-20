/* Copyright 2014-2018 Baumer Optronic */

#ifndef TLCONSUMER_BGAPI2_GENICAM_BGAPI2_DEF_H_
#define TLCONSUMER_BGAPI2_GENICAM_BGAPI2_DEF_H_

#include "./bgapi2_types.h"

class CEventPnPObj;
class CEventDeviceEventObj;

namespace BGAPI2 {

class BGAPI2_DECL String {
 public:
    String();
    String(const char * text);
    String(const char * text, int length);
    String(const char & ch, int length);
    String(const String & Obj);
    String(const char & ch);
    ~String();

    operator char *();
    operator const char *();
    bool operator == (const char * text);
    bool operator == (String & ExStr) const;  // NOLINT(runtime/references)
    bool operator != (const char * text);
    bool operator != (const String & ExStr);
    bool operator < (const String & ExStr) const;
    const String & operator = (char &);  // NOLINT(runtime/references)
    const String & operator = (const char *);
    const String & operator = (const String & ExStr);

#ifndef _WIN32
    friend std::ostream& operator << (std::ostream& out, BGAPI2::String const &ExStr) BGAPI2_DECL;
#endif
    const char* get() const;
    //        const char* get();

    void set(const char * text);
    int size();

 private:
    void* data;
};

#ifndef _WIN32
std::ostream& operator << (std::ostream& out, BGAPI2::String const &ExStr) BGAPI2_DECL;
#endif

struct _sSystemListData;
typedef struct BGAPI2::_sSystemListData tSystemListData, *ptSystemListData;

struct _sInterfaceListData;
typedef struct BGAPI2::_sInterfaceListData tInterfaceListData, *ptInterfaceListData;

struct _sSystemData;
typedef struct BGAPI2::_sSystemData tSystemData, *ptSystemData;

struct _sDeviceListData;
typedef struct BGAPI2::_sDeviceListData tDeviceListData, *ptDeviceListData;

struct _sDataStreamListData;
typedef struct BGAPI2::_sDataStreamListData tDataStreamListData, *ptDataStreamListData;

struct _sBufferListData;
typedef struct BGAPI2::_sBufferListData tBufferListData, *ptBufferListData;

struct _sBufferData;
typedef struct BGAPI2::_sBufferData tBufferData, *ptBufferData;

struct _sDataStreamData;
typedef struct BGAPI2::_sDataStreamData tDataStreamData, *ptDataStreamData;

struct _sDeviceData;
typedef struct BGAPI2::_sDeviceData tDeviceData, *ptDeviceData;

struct _sInterfaceData;
typedef struct BGAPI2::_sInterfaceData tInterfaceData, *ptInterfaceData;

struct _sImageData;
typedef struct BGAPI2::_sImageData tImageData, *ptImageData;

struct _sImageProcessorData;
typedef struct BGAPI2::_sImageProcessorData tImageProcessorData, *ptImageProcessorData;

struct _sBrightnessAutoData;
typedef struct BGAPI2::_sBrightnessAutoData tBrightnessAutoData, *ptBrightnessAutoData;

class System;
class BGAPI2_DECL _pairs {
 public:
    String first;
    System* second;
};

class Interface;
class BGAPI2_DECL _pairi {
 public:
    String first;
    Interface* second;
};

class Device;
class BGAPI2_DECL _paird {
 public:
    String first;
    Device* second;
};

class DataStream;
class BGAPI2_DECL _pairds {
 public:
    String first;
    DataStream* second;
};

class Buffer;
class BGAPI2_DECL _pairb {
 public:
    String first;
    Buffer* second;
};

#ifdef _WIN32
#pragma pack(push, 8)
#endif

class Node;
class BGAPI2_DECL _pairn {
 public:
    bo_int64 first;
    Node* second;
};

class BGAPI2_DECL _pairnm {
 public:
    String first;
    _pairn second;
};

#ifdef _WIN32
#pragma pack(pop)
#endif

#ifdef _WIN32
#pragma pack(push, 8)
#endif
    typedef struct {
        bo_ushort *pcRed;       // pointer to hist array offered by user, can be zero
        bo_ushort *pcGreen;     // pointer to hist array offered by user, can be zero
        bo_ushort *pcBlue;      // pointer to hist array offered by user, can be zero
        bo_ushort *pcLuma;      // pointer to hist array offered by user, can be zero
        int   length;
        int * pSizeFilled;      // pointer to fill info for user
        bool ThresEnable;
        int  ThresMin;
        int  ThresMax;
        int* red_under;
        int* red_over;
        int* green_under;
        int* green_over;
        int* blue_under;
        int* blue_over;
    } bo_tHistRecords;

    /**
    \struct    tRGB16QUAD
    \brief  The tRGB16QUAD structure specifies the information for one color look up table entry
    \brief  according to RGBQUAD of WinGdi
    */
    typedef struct {
        bo_ushort rgbBlue;
        bo_ushort rgbGreen;
        bo_ushort rgbRed;
        bo_ushort rgbReserved;
    } bo_tRGB16QUAD;
#ifdef _WIN32
#pragma pack(pop)
#endif

}  // namespace BGAPI2

#endif  // TLCONSUMER_BGAPI2_GENICAM_BGAPI2_DEF_H_
