/* Copyright (C) 2014 Baumer Optronic GmbH */

#ifndef TLCONSUMER_BGAPI2_EXT_SC_BGAPI2_EXT_SC_H_
#define TLCONSUMER_BGAPI2_EXT_SC_BGAPI2_EXT_SC_H_

// The following ifdef block is the standard way of creating macros which make
// exporting from a DLL simpler. All files within this DLL are compiled with
// the BGAPI2_EXT_SC_EXPORTS symbol defined on the command line. This symbol
// should not be defined on any project that uses this DLL. This way any other
// project whose source files include this file see
// BGAPI2_EXT_SC_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.
#ifndef _GNULINUX
#   ifndef BGAPI2_EXT_SC_API
#      ifdef BGAPI2_EXT_SC_EXPORTS
#          define BGAPI2_EXT_SC_API __declspec(dllexport)
#      else
#          define BGAPI2_EXT_SC_API __declspec(dllimport)
#      endif
#   endif
#elif defined(__GNUC__) && (__GNUC__ >= 4) && (defined(__linux__) || defined(__APPLE__))
#define BGAPI2_EXT_SC_API __attribute__((visibility("default")))
#else
#define BGAPI2_EXT_SC_API
#endif

#include "bgapi2_genicam/bgapi2_genicam.hpp"

namespace BGAPI2 {
namespace Ext {
namespace Sc {

class BGAPI2_EXT_SC_API Shading {
 public:
     enum ReturnCode {
         eSuccess = 0,
         eError   = -1,
         eError_Dark = -2,
         eError_Bright = -3,
         eError_PixelFormat = -4,
     };

    /**
    \fn  Shading(Device* pDevice)
    \brief  Function category: Global Baumer Extended API functions
    \brief  This class support the functionality of shading correction.
    \param 'pDevice' Device pointer to a camera.
    \exception Exceptions::InvalidParameterException The parameters is null.
    */
    explicit Shading(Device* pDevice);
    ~Shading(void);

    /**
    \fn  void Load(String filename)
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function load a shading reference.
    \param  'bLightFrame' (IN) dark- or light-frame reference.
    \param  'sFilename' (IN) filename of shading reference.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException.
    */
    void ReferenceLoad(const bo_bool bLightFrame, const String sFilename);

    /**
    \fn  void ReferenceStart(bo_bool bLightFrame)
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function starts the shading reference accumulation.
    \param  'bLightFrame' (IN) dark- or light-frame referencing.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException.
    */
    void ReferenceStart(const bo_bool bLightFrame);

    /**
    \fn  void ReferenceStart(bo_bool bLightFrame)
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function ends the shading reference accumulation. The reference can be saved to file
    \param  'sFilename' (IN) filename of shading reference.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException.
    */
    void ReferenceEnd(const String sFilename);

    /**
    \fn  void ReferenceApply()
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function enables/disables usage of shading correction.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    */
    void ReferenceApply();

    /**
    \fn  void ReferenceClear()
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function enables/disables usage of shading correction.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    */
    void ReferenceClear();

    /**
    \fn  void ReferenceAdd(BGAPI2::Image* pImage)
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function accumulate the shading reference.
    \param  'sPixelFormat' (IN) filename of shading correction.
    \param  'width'        (IN) The width of the image in pixel.
    \param  'height'       (IN) The height of the image in pixel.
    \param  'pBuffer'      (IN) The image buffer.
    \param  'bufferSize'   (IN) The size of image buffer in byte.
    \return ReturnCode.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException.
    */
    ReturnCode ReferenceAdd(const String sPixelFormat, const bo_uint64 width, const bo_uint64 height
        , void* const pBuffer, const bo_uint64 bufferSize);

    /**
    \fn  void SetEnable(bo_bool bSet)
    \brief  Function category: Global Baumer Extended API functions
    \brief  This function enables/disables usage of shading correction.
    \param  'bSet' (IN) enable / disable of shading correction.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::ErrorException Shading is not invalid (shading enable).
    */
    void SetEnable(bo_bool bSet);

    enum eFilterType {
        BoxFilter,
        MedianFilter,
    };

    /**
    \fn  void SetFilter(const eFilterType filter, const bo_bool bLightFrame, bo_uint boxRadius)
    \brief  Function category: Global Baumer Extended API functions
    \brief     This function sets the filter radius.
    \param  'type'        (IN) filter type.
    \param  'bLightFrame' (IN) dark- or light-frame filter.
    \param  'radius'      (IN) filter radius.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException The passed value is out of range.
    */
    void SetFilter(const eFilterType filter, const bo_bool bLightFrame, bo_uint radius);

    /**
    \fn  bo_uint GetFilter(const eFilterType filter, const bo_bool bLightFrame)
    \brief  Function category: Global Baumer Extended API functions
    \brief     This function get the box filter radius.
    \param  'type'        (IN) filter type.
    \param  'bLightFrame' (IN) dark- or light-frame filter.
    \return filter radius.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException The passed value is out of range.
    */
    bo_uint GetFilter(const eFilterType filter, const bo_bool bLightFrame);

    /**
    \fn  bo_uint GetFilterMin(const eFilterType filter, const bo_bool bLightFrame)
    \brief  Function category: Global Baumer Extended API functions
    \brief     This function get the box filter radius min.
    \param  'type'        (IN) filter type.
    \param  'bLightFrame' (IN) dark- or light-frame filter.
    \return filter radius.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException The passed value is out of range.
    */
    bo_uint GetFilterMin(const eFilterType filter, const bo_bool bLightFrame);

    /**
    \fn  bo_uint GetFilterMax(const bo_bool bLightFrame)
    \brief  Function category: Global Baumer Extended API functions
    \brief     This function get the box filter radius max.
    \param  'type'        (IN) filter type.
    \param  'bLightFrame' (IN) dark- or light-frame filter.
    \return filter radius.
    \exception Exceptions::NotAvailableException Shading correction is not supported.
    \exception Exceptions::InvalidParameterException The passed value is out of range.
    */
    bo_uint GetFilterMax(const eFilterType filter, const bo_bool bLightFrame);


 private:
    void * m_pReserved;
};


}  //  namespace Sc
}  //  namespace Ext
}  //  namespace BGAPI2
#endif  // TLCONSUMER_BGAPI2_EXT_SC_BGAPI2_EXT_SC_H_
