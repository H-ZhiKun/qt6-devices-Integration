/**
 *-------------------------------------------------------------------
 *
 *  MicroEnable Io Library
 *  Copyright (C) Silicon Software GmbH, 1997-2017
 *
 * This includes code from:
 * LIBTIFF, Version 3.7.4
 *    Copyright (c) 1988-1996 Sam Leffler
 *    Copyright (c) 1991-1996 Silicon Graphics, Inc.
 *
 * --------------------------------------------------------------------
 *
 * \file sisoIo.h
 *
 * \brief Functions to load and store image files.
 *
 * \author Silicon Software GmbH
 */

#ifndef _SISOIO_H
#define _SISOIO_H

//#include "os_type.h"

#ifdef _WIN32
#include <stdio.h>
#include <vfw.h>
#else
typedef char* LPCWSTR;
#endif

#define	    COMPRESSION_NONE		1	// dump mode 
#define	    COMPRESSION_CCITTRLE	2	// CCITT modified Huffman RLE 
#define	    COMPRESSION_CCITTFAX3	3	// CCITT Group 3 fax encoding 
#define     COMPRESSION_CCITT_T4    3       // CCITT T.4 (TIFF 6 name) 
#define	    COMPRESSION_CCITTFAX4	4	// CCITT Group 4 fax encoding 
#define     COMPRESSION_CCITT_T6    4       // CCITT T.6 (TIFF 6 name) 
#define	    COMPRESSION_LZW			5       // Lempel-Ziv  & Welch 
#define	    COMPRESSION_OJPEG		6	// !6.0 JPEG 
#define	    COMPRESSION_JPEG		7	// %JPEG DCT compression 
#define	    COMPRESSION_NEXT		32766	// NeXT 2-bit RLE 
#define	    COMPRESSION_CCITTRLEW	32771	// #1 w/ word alignment 
#define	    COMPRESSION_PACKBITS	32773	// Macintosh RLE 
#define	    COMPRESSION_THUNDERSCAN	32809	// ThunderScan RLE 


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef IMP_DLL
#define dlldir __declspec( dllimport)
#else
#ifndef dlldir
#define dlldir
#endif
#endif

/**
 * \warning this function only works on Windows
 */
dlldir int IoCreateAVIGray(void **AviRef, const char *filename,int width,int height,double fps);
/**
 * \warning this function only works on Windows
 */
dlldir int IoCreateAVIGrayW(void **AviRef, const LPCWSTR filename,int width,int height,double fps);
/**
 * \warning this function only works on Windows
 */
dlldir int IoCreateAVIColor(void **AviRef, const char *filename,int width,int height,double fps);
/**
 * \warning this function only works on Windows
 */
dlldir int IoCreateAVIColorW(void **AviRef, const LPCWSTR filename,int width,int height,double fps);
/**
 * \warning this function only works on Windows
 */
dlldir int IoWriteAVIPicture(void *AviRef,int PicNr,unsigned char *buffer);
/**
 * \warning this function only works on Windows
 */
dlldir int IoReadAVIPicture(void *AviRef,int PicNr,unsigned char *buffer);
/**
 * \warning this function only works on Windows
 */
dlldir int IoCloseAVI(void *AviRef);
/**
 * \warning this function only works on Windows
 */
dlldir int IoOpenAVI(void **AviRef, const char *fileName,int* width,int* height,int *bitDepth);

/**
 * \brief Calculate the buffer size according to the bit depth
 * This function will return the buffer size for a given image format.
 * For the packed formats (bit depth not a multiple of 8) the assumption is made that a line always starts on a byte boundary
 * and that the last byte of the line will be padded with 0 as necessary.
 * To calculate the buffer size fpr TIFF files, pass bitsPerSample*samplesPerPixel to bits.
 *
 * \param width dimension of the image
 * \param height dimension of the image
 * \param bits bitwidth of a pixel
 *
 * \returns number of bytes used in buffer
 */
dlldir size_t IoCalculateBufferSize(int width, int height, int bits);

/**
* \brief Load data from a bitmap file to a memory buffer
* This functions loads the image data of a bitmap file to a memory buffer. The function allocates the memory buffer according
* to the detected image information inside the bitmap file and returns the pointer to the memory. In case of colored images, the ordering
* sequence of the color channels is BGR. 
* Please note, that the image information inside the buffer is starting at the point ( X = 0, Y = 0), which means, that the data of the bitmap file is mirrored
* along a diagonal line compared to the bitmap format.
* 
* \param filename filename including file extension
* \param data resulting memory buffer. 
* \param width dimension of the image
* \param height dimension of the image
* \param bits bitwidth of a pixel
*
* \warning this function only works on Windows
 */
dlldir void *IoReadBmp(const char *filename,unsigned char **data,int *width,int *height,int *bits);
/**
 * \warning this function only works on Windows
 */
dlldir int IoCloseBmp(void *bmpHandle);
/**
 * \warning this function only works on Windows
 */
dlldir int IoWriteBmp(const char *filename, unsigned char *data, int width, int height, int bits);
/**
 * \warning this function only works on Windows
 */
dlldir int IoWriteBmpW(const LPCWSTR filename,unsigned char *data,int width,int height,int bits);

dlldir int IoWriteRaw(const char *filename, unsigned char *data, int width, int height, int bits);
dlldir int IoWriteRawW(const LPCWSTR filename, unsigned char *data, int width, int height, int bits);
dlldir int IoReadRaw(const char *filename, unsigned char**data, int *width, int *height, int *bits);
dlldir int IoFreeRaw(unsigned char *data);

// create a image sequence file
dlldir int IoCreateSeq(void **SeqRef, const char *pFilename, int width, int height, int bitdepth, int format);
// put the given image at the given sequence index
dlldir int IoWriteSeqPicture(void *SeqRef,int PicNr,unsigned char *buffer);
// close image sequence file
dlldir int IoCloseSeq(void *SeqRef);

// Open file with image sequence
dlldir int IoOpenSeq(void **SeqRef, const char *pFilename, int* width, int* height, int* bitdepth, int mode);
// get information about image sequence file
dlldir int IoGetSeqInfo(void *SeqRef,int *nrOfImages,int *nrOfLostImages);
// load image with given sequence number
dlldir int IoReadSeqPicture(void *SeqRef,int PicNr,unsigned char *buffer);
// load next picture in sequence
dlldir int IoReadNextSeqPicture(void *SeqRef,int* PicNr,unsigned char *buffer);
// create an AVI file from the given range of images
dlldir int IoSeqCreateAvi(void *SeqRef,const char* aviFilename,int StartPicNr,int StopPicNr);


/**
* \brief Save Image data to to a file in TIF format.
*
* \sa IOWriteTiffEx
* \param filename 
* \param data
* \param width
* \param height
* \param bitPerSample
* \param samplePerPixel
*/
dlldir int IoWriteTiff(const char *filename, unsigned char *data, int width, int height, int bitPerSample, int samplePerPixel);

/**
* \brief Save Image data to to a file in TIF format.
*
* \param filename name of the target file to store the data
* \param data pointer to a memory buffer e.g. retrieved by a call to IOReadTif
* \param width image dimension at the memory buffer, in this case the width in pixel
* \param height image dimension at the memory buffer, in this case the height in pixel
* \param bitPerSample no. of bits for a sample
* \param samplePerPixel no. of samples per pixel
* \param RGBSequence allows to swap the RGB data a the image data (in case of color formats). Note that the buffer data will be modified in this case
* \returns 0 in case of success, otherwise various error codes
*
* This functions writes image form a memory buffer to the according file. The image data is located at the given buffer (data), 
* which is filled according to the IOReadTiff functions. 
* When using color formats, the sequence of RGB at the file can be swapped in order to allow conversions from different image formats 
* (e.g. BMP-> TIFF). The buffer itself doesn't get modified in this case.
* 
*/
dlldir int IoWriteTiffEx(const char *filename, unsigned char *data, int width, int height, int bitPerSample, int samplePerPixel, int RGBSequence);


/**
* \brief Save Image data to to a file in TIF format.
*
* Widestring version of IoWriteTiff
* \sa IoWriteTiff
*
* \param filename name of the target file to store the data
* \param data pointer to a memory buffer e.g. retrieved by a call to IOReadTif
* \param width image dimension at the memory buffer, in this case the width in pixel
* \param height image dimension at the memory buffer, in this case the height in pixel
* \param bitPerSample no. of bits for a sample
* \param samplePerPixel no. of samples per pixel
* \returns 0 in case of success, otherwise various error codes
*
*/
dlldir int IoWriteTiffW(const LPCWSTR filename, unsigned char *data, int width, int height, int bitPerSample, int samplePerPixel);


/**
* \brief  Save Image data to to a file in TIF format
*
* Widestring version of IoWriteTiffEx
* \sa IoWriteTiffEx
*
* \param filename name of the target file to store the data
* \param data pointer to a memory buffer e.g. retrieved by a call to IOReadTif
* \param width image dimension at the memory buffer, in this case the width in pixel
* \param height image dimension at the memory buffer, in this case the height in pixel
* \param bitPerSample no. of bits for a sample
* \param samplePerPixel no. of samples per pixel
* \param RGBSequence allows to swap the RGB data a the image data (in case of color formats). Note that the buffer data will be modified in this case
* \returns 0 in case of success, otherwise various error codes
*
*/
dlldir int IoWriteTiffExW(const LPCWSTR filename, unsigned char *data, int width, int height, int bitPerSample, int samplePerPixel, int RGBSequence);


/**
* \brief Load a TIF file into a memory buffer
*
* \param filename filename including file extension
* \param data a pointer to the newly created memory containing the image data
* \param width image dimension: width
* \param height image dimension: height
* \param bitPerSample no. of bits per sample (bitwidth)
* \param samplePerPixel no. of samples per pixel
*
* This functions reads the image information and puts the images data to a memory buffer, that will be allocated by the IO-library.
* In case of color formats, the ordering sequence of the RGB channels will always be BGR
*/
dlldir void *IoReadTiff(const char *filename, unsigned char**data, int *width, int *height, int *bitPerSample, int *samplePerPixel);

/**
* \brief Load a TIF file into a buffer, 
*
* Widestring version of IoWriteTiff
*
* \param filename filename including file extension
* \param data a pointer to the newly created memory containing the image data
* \param width image dimension: width
* \param height image dimension: height
* \param bitPerSample no. of bits per sample (bitwidth)
* \param samplePerPixel no. of samples per pixel
*
*/
dlldir void *IoReadTiffW(const LPCWSTR filename, unsigned char**data, int *width, int *height, int *bitPerSample, int *samplePerPixel);

/**
* \brief Load a TIF file into a memory buffer (enhanced version).
*
* \param filename filename including file extension
* \param data a pointer to the newly created memory containing the image data
* \param width image dimension: width
* \param height image dimension: height
* \param bitPerSample no. of bits per sample (bitwidth)
* \param samplePerPixel no. of samples per pixel
* \param RGBSequence defines the sequence of RGB channels at the resulting buffer: 0 order will be RGB, 1 order will be BGR
*
* \returns 0 in case of success, otherwise various error codes
*
* This functions reads the image information and puts the images data to a memory buffer, that will be allocated by the IO-library.
* The functions returns a pointer to then buffer of the image data, the dimensions of the image and details about the image format as well. 
* The buffer memory will be released by the IOFreTiff function. The buffer is organized as a array of char line by line.
*/

dlldir void *IoReadTiffEx(const char *filename, unsigned char**data, int *width, int *height, int *bitPerSample, int *samplePerPixel, int RGBSequence);

/**
* \brief Load a TIF file into a memory buffer. 
*
* Wide string version of the IoReadTiffEx function
*/
dlldir void *IoReadTiffExW(const LPCWSTR filename, unsigned char**data, int *width, int *height, int *bitPerSample, int *samplePerPixel, int RGBSequence);

/**
* \brief Release the memory resources of previously read image files. 
* \param data pointer to the memory buffer retrieved by the functions IoReadTiff, IoReadTiffEx
*/
dlldir int IoFreeTiff(unsigned char *data);

dlldir int IoWriteTiffCompression(const char *filename, unsigned char *data, int width, int height, int bitPerSample, int samplePerPixel, int compressionFlag);

// -------------------------------------------------------------------
#ifdef SISOIO_STRICT_POINTER
#ifdef __cplusplus
class SisoIoImageEngine;
#else /* __cplusplus */
typedef struct SisoIoImageEngine_s SisoIoImageEngine;
#endif /* __cplusplus */
#else /* SISOIO_STRICT_POINTER */
typedef void SisoIoImageEngine;
#endif /* SISOIO_STRICT_POINTER */

/**
 * \brief open an existing image file
 * \param filename name of the file that should be loaded
 * \returns image handle on success, NULL otherwise
 * \deprecated In new code you should use IoImageOpen() as this does the same
 * operation but returns an error code if what went wrong.
 *
 * Old code:
 * \code
 * SisoIoImageEngine *handle = IoOpenImageExt("example.tif");
 * if (handle == NULL) {
 *  // error handling
 * }
 * \endcode
 *
 * New code:
 * \code
 * SisoIoImageEngine *handle;
 * int err = IoImageOpen("example.tif", &handle);
 * if (err != 0) {
 *  // error handling
 * }
 * \endcode
 */
dlldir SisoIoImageEngine *IoOpenImage(const char *filename);

/**
 * \brief open an existing image file
 * \param filename name of the file that should be loaded
 * \param data image data will be referenced here
 * \param width width of opened image
 * \param height height of opened image
 * \param bits bit per pixel setting of opened image
 * \returns image handle on success, NULL otherwise
 * \deprecated In new code you should use IoImageOpen() and query the image
 * settings by the appropiate functions.
 *
 * Old code:
 * \code
 * unsigned char *data;
 * int width, height, bits;
 * SisoIoImageEngine *handle = IoOpenImageExt("example.tif", &data, &width, &height, &bits);
 * if (handle == NULL) {
 *  // error handling
 * }
 * \endcode
 *
 * New code:
 * \code
 * const unsigned char *data;
 * int width, height, bits;
 * SisoIoImageEngine *handle;
 * int err = IoImageOpen("example.tif", &handle);
 * if (err != 0) {
 *  // error handling
 * }
 * data = (const unsigned char *)IoImageGetData(handle);
 * width = IoGetWidth(handle);
 * height = IoGetHeight(handle);
 * bits = IoGetBitsPerPixel(handle);
 * \endcode
 */
dlldir SisoIoImageEngine *IoOpenImageExt(const char *filename, unsigned char **data, int *width, int *height, int *bits);

/**
 * \brief open an existing image file. The channel order for color images is BLUE GREEN RED.
 * Supported file formats:
 * - TIFF uncompressed, LZW, PackBits
 *   -# 1  Bit ( line width must be byte aligned ) 
 *	 -# 8  Bit gray
 *	 -# 16 Bit gray
 *	 -# 24 Bit color 
 *	 -# 48 Bit color
 * -BMP uncompressed
 *   -# 1  Bit ( line width must be byte aligned ) 
 *   -# 8  Bit gray
 *   -# 24 Bit color
 *
 * \param filename  name of the file that should be loaded
 * \param handle  the opened handle will be stored here
 * \returns 0 on success, negative values on error
 *
 * This function is superior to IoOpenImage() as you will get an error
 * code if what went wrong.
 * \param filename name of the file that should be loaded
 * \param handle return value for access to worker functions
 */
dlldir int IoImageOpen(const char *filename, SisoIoImageEngine **handle);

/**
 * \brief open an existing image file
 * \param filename name of the file that should be loaded
 * \param handle return value for access to worker functions
* \param RGBSequence 0 -> Buffer sequence always BGR, 1 -> always RGB
*/
dlldir int IoImageOpenEx(const char *filename, SisoIoImageEngine **handle, int RGBSequence);

dlldir int	IoFreeImage(SisoIoImageEngine *handle);
dlldir int	IoGetWidth(const SisoIoImageEngine *handle);
dlldir int	IoGetHeight(const SisoIoImageEngine *handle);
dlldir int	IoGetBitsPerPixel(const SisoIoImageEngine *handle);
dlldir int	IoGetBitsPerComponent(const SisoIoImageEngine *handle) ;
dlldir int	IoGetNrOfComponents(const SisoIoImageEngine *handle);
/**
* \brief save image to File. The channel order for color images is RED GREEN BLUE.
* Supported file formats:
* - TIFF uncompressed, LZW, PackBits
*   -# 1  Bit ( line width must be byte aligned ) 
*	 -# 8  Bit gray
*	 -# 16 Bit gray
*	 -# 24 Bit color 
*	 -# 48 Bit color
* -BMP uncompressed
*   -# 1  Bit ( line width must be byte aligned ) 
*   -# 8  Bit gray
*   -# 24 Bit color
*
* \param  filename  filename of the destination image
* \param  data  image data array
* \param  width image width
* \param  height image height
* \param  bits bits per pixel
* \returns 0 on success, negative values on error
*/
dlldir int	IoSaveImageExt(const char *filename, unsigned char *data, int width, int height, int bits);

/**
* \brief converts a bitmap in an 8 bit gray image. It normalizes the gray values. 
* \param handle the image handle as opened by IoImageOpen(), IoOpenImage(), or IoOpenImageExt().
* \returns 0 on success, negative values on error
*
*/
dlldir int  IoConvert1BitTo8BitData(SisoIoImageEngine *handle);

/**
 * \brief query the pixel data from an image. The channel order for color images is BLUE GREEN RED.
 * \param handle the image handle as opened by IoImageOpen(), IoOpenImage(), or IoOpenImageExt().
 * \return the pixel data of the image
 * \retval NULL the handle is invalid or no image data was loaded
 */
dlldir const void *IoImageGetData(const SisoIoImageEngine *handle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SISOIO_H */
