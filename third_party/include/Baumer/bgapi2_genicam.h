/**
  Copyright 2014-2018 Baumer Optronic

  \file bgapi2_genicam.h

  \defgroup System BGAPI2 System Functions
  \brief Functions related to the GenICam System or Producer <br/>
  The System is the abstraction for an GenTL Producer. The System module provides functions to find, load/unload and
  open/close GenTL producers. There are also functions to retrieve informations like ID, vendor or name from a producer
  and to retrieve the the accessable Interfaces.


  \defgroup Interface BGAPI2 Interface Functions
  \brief Functions related to the GenICam Interface (USB or GigE transport layer)<br/>
  A System (GenTL producer) can connect devices through the transport layer via interfaces. An interface can be a physical interface such as an ethernet network adapter or a logical interface such as an USB port. The Interface module provides functions to open/close interfaces, get informations like the ID and name of an interface and retrieve the devices accessable through each interface.

  \defgroup Device BGAPI2 Device Functions
  \brief Functions related to the GenICam Device, typically the camera

  \defgroup DataStream BGAPI2 DataStream Functions
  \brief Functions related to the GenICam DataStream of each device

  \defgroup Buffer BGAPI2 Buffer Functions
  \brief Functions related to the GenICam Buffer

  \defgroup Node BGAPI2 Node Functions
  \brief Functions related to the GenICam Node or Feature

  \defgroup NodeMap BGAPI2 NodeMap Functions
  \brief Functions related to the GenICam NodeMap or FeatureMap

  \defgroup Image BGAPI2 Image Functions
  \brief Functions related to the BGAPI2 Images

  \defgroup ImgProc BGAPI2 ImageProcessor Functions
  \brief Functions related to the Image Processor

  \defgroup Polarizer BGAPI2 Polarization Functions
  \brief Functions related to the Polarization Camera

  \defgroup DeviceEvent BGAPI2 Device Event Functions
  \brief Functions related to the GenICam Device Events

  \defgroup PnPEvent BGAPI2 PnP Event Functions
  \brief Functions related to the GenICam Plug and Play Events

  \defgroup Trace BGAPI2 Trace Functions
  \brief Functions related to the BGAPI2 Trace

  \defgroup BGAPI2 BGAPI2 ...other stuff :)

  \defgroup Feature BGAPI2 Feature Names
*/

#ifndef TLCONSUMER_BGAPI2_GENICAM_BGAPI2_GENICAM_H_
#define TLCONSUMER_BGAPI2_GENICAM_BGAPI2_GENICAM_H_

#include "./bgapi2_types.h"
#include "./bgapi2_featurenames.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
      \ingroup    System
      \brief      Struct for pointers of type System
    */
    typedef struct BGAPI2_System BGAPI2_System;

    /**
      \ingroup    Interface
      \brief      Struct for pointers of type Interface
    */
    typedef struct BGAPI2_Interface BGAPI2_Interface;

    /**
      \ingroup    Device
      \brief      Struct for pointers of type Device
    */
    typedef struct BGAPI2_Device BGAPI2_Device;

    /**
      \ingroup    DataStream
      \brief      Struct for pointers of type DataStream
    */
    typedef struct BGAPI2_DataStream BGAPI2_DataStream;
    /**
      \ingroup    Buffer
      \brief      Struct for pointers of type Buffer
    */
    typedef struct BGAPI2_Buffer BGAPI2_Buffer;

    /**
      \ingroup    Node
      \brief      Struct for pointers of type Node
    */
    typedef struct BGAPI2_Node BGAPI2_Node;

    /**
      \ingroup    NodeMap
      \brief      Struct for pointers of type NodeMap
    */
    typedef struct BGAPI2_NodeMap BGAPI2_NodeMap;

    /**
      \ingroup    Image
      \brief      Struct for pointers of type Image
    */
    typedef struct BGAPI2_Image BGAPI2_Image;

    /**
      \ingroup    ImgProc
      \brief      Struct for pointers of type ImageProcessor
    */
    typedef struct BGAPI2_ImageProcessor BGAPI2_ImageProcessor;

    /**
      \ingroup    Polarizer
      \brief      Struct for pointers of type Polarizer
    */
    typedef struct BGAPI2_Polarizer BGAPI2_Polarizer;

    /**
      \ingroup    DeviceEvent
      \brief      Struct for pointers of type DeviceEvent
    */
    typedef struct BGAPI2_DeviceEvent BGAPI2_DeviceEvent;

    /**
      \ingroup    PnPEvent
      \brief      Struct for pointers of type PnPEvent
    */
    typedef struct BGAPI2_PnPEvent BGAPI2_PnPEvent;

    /**
      \ingroup    DeviceEvent
      \brief      An enumeration containing the string representation of the possible event configurations
    */
    typedef enum {
        EVENTMODE_UNREGISTERED  = 0,  ///< Event handling is disabled. No events can be retrieved
        EVENTMODE_POLLING       = 1,  ///< Events are retrieved by a function call of the corresponding get function
        EVENTMODE_EVENT_HANDLER = 2   ///< Events are retrieved by a previously registered callback function
    } BGAPI2_EventMode;

    /**
      \ingroup    Interface
      \brief      Declaration for callback functions for BGAPI2_PnPEventHandler
    */
    typedef void (BGAPI2CALL *BGAPI2_PnPEventHandler)(void * callBackOwner, BGAPI2_PnPEvent * pnpEvent);

    /**
      \ingroup    Device
      \brief      Declaration for callback functions for BGAPI2_DevEventHandler
    */
    typedef void (BGAPI2CALL *BGAPI2_DevEventHandler)(void * callBackOwner, BGAPI2_DeviceEvent * deviceEvent);

    /**
      \ingroup    Buffer
      \brief      Declaration for callback functions for BGAPI2_NewBufferEventHandler
    */
    typedef void (BGAPI2CALL *BGAPI2_NewBufferEventHandler)(void * callBackOwner, BGAPI2_Buffer * pBuffer);

    // SYSTEM Functions
    /**
      \ingroup    System
      \brief      Search for GenTL producers in the current directory and in GENICAM_GENTLxx_PATH

      This function creates an System object for each found producer.
      GenTL producer are files with the .cti extension and are synonymous with the system. After the list of systems
      is updated, you can use BGAPI2_GetNumSystems() function to get the number of producers found and the BGAPI2_GetSystem() function to open a specific system to work with it

      \retval     BGAPI2_RESULT_SUCCESS        No error
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR An error
      \see        BGAPI2_ReleaseSystem
      \see        BGAPI2_GetSystem
      \see        BGAPI2_System_Open
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_UpdateSystemList();

    /**
      \ingroup    System
      \brief      Search for GenTL producers only in the path specified

      This function creates an System object for each found producer.
      GenTL producer are files with the .cti extension and are synonymous with the system. After the list of systems
      is updated, you can use BGAPI2_GetNumSystems() function to get the number of producers found and the BGAPI2_GetSystem() function to open a specific system to work with it

      \param[in]  producer_path  Path where producers should be searched
      \retval     BGAPI2_RESULT_SUCCESS        No error
      \retval     BGAPI2_RESULT_ERROR          Internal error
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR An error
      \see        BGAPI2_System_Close
      \see        BGAPI2_ReleaseSystem
      \see        BGAPI2_GetSystem
      \see        BGAPI2_System_Open
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_UpdateSystemListFromPath(const char* producer_path);

    /**
      \ingroup    System
      \brief      Creates a system (GenTL producer) object, specified by filepath before opening it

      This function creates an System object for each found producer.
      You need to create and open a producer before you can get any information about it. Once
      a producer is created it will not be counted by subsequent BGAPI2_GetNumSystems() calls!

      \param[in]  file_path Path and Filename of the producer to create
      \param[out] system    Reference to a pointer to the system (producer)
      \retval     BGAPI2_RESULT_SUCCESS No error
      \see        BGAPI2_System_Open
      \see        BGAPI2_System_Close
      \see        BGAPI2_ReleaseSystem
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_LoadSystemFromPath(const char* file_path, BGAPI2_System** system);

    /**
      \ingroup    System
      \brief      Returns the number of systems (GenTL Producers) found by BGAPI2_UpdateSystemList()
      \param[out] count Count of found GenTL Producer
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_GetNumSystems(bo_uint* count);

    /**
      \ingroup    System
      \brief      Get a pointer to the System (GenTL Producer) specified by the index

      You need to open a System before you can get any information about it. Once a System is opened it will not be
      counted by subsequent BGAPI2_GetNumSystems calls!

      \param[in]  index   Index of the system (producer) to use
      \param[out] system  Reference to a pointer to the system (producer)
      \retval     BGAPI2_RESULT_SUCCESS            No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER  Error for invalid parameters
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE    Producer already loaded
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR     Error in producer (functions not found)
      \see        BGAPI2_System_Open
      \see        BGAPI2_System_Close
      \see        BGAPI2_ReleaseSystem
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_GetSystem(bo_uint index, BGAPI2_System** system);

    /**
      \ingroup    System
      \brief      Opens a system (GenTL producer) to work with it
      \param[in]  system  Pointer to the system (producer) obtained from BGAPI2_GetSystem
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Already opened
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_GetSystem
      \see        BGAPI2_System_Close
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_Open(BGAPI2_System* system);

    /**
      \ingroup    System
      \brief      Check if the system (GenTL producer) is opened
      \param[in]  system  Pointer to the system (producer)
      \param[out] is_open Pointer to the result variable
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_Open
      \see        BGAPI2_System_Close
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_IsOpen(BGAPI2_System* system, bo_bool* is_open);

    /**
      \ingroup    System
      \brief      Updates the list of of accessable interfaces and creates an object for each
      \param[in]  system   Pointer to the system (producer)
      \param[out] changed  Flag if interfaces are changed since last call
      \param[in]  timeout  Maximum time in milliseconds to search for interfaces
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_UpdateInterfaceList(
        BGAPI2_System* system,
        bo_bool* changed,
        bo_uint64 timeout);

    /**
      \ingroup    System
      \brief      Get a pointer to the interface with the specified index.

      You need to call BGAPI2_System_UpdateInterfaceList() first! A System (GenTL producer) connects devices through a
      transport layer via an interface. The Interface can be a physical interface such as an ethernet network adapter
      or a logical interface such as an USB port.

      \param[in]  system     Pointer to the system (producer)
      \param[in]  index      Index in the interface list
      \param[out] iface      Device instance
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_UpdateInterfaceList
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetInterface(
        BGAPI2_System* system,
        bo_uint index,
        BGAPI2_Interface** iface);

    /**
      \ingroup    System
      \brief      Returns the number of accessable interfaces
      \param[in]  system           Pointer to the system (producer)
      \param[out] count_interfaces Number of interfaces
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetInterface
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetNumInterfaces(
        BGAPI2_System* system,
        bo_uint* count_interfaces);

    /**
      \ingroup    System
      \brief      Closes a system (producer), you need to call BGAPI2_ReleaseSystem to unload the library
      \param[in]  system Pointer to the system (producer)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_Open
      \see        BGAPI2_ReleaseSystem
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_Close(BGAPI2_System* system);


    /**
      \ingroup    System
      \brief      Release a system (GenTL producer) specified. You need to call BGAPI2_System_Close first!
      \param[in]  system Pointer to the system (producer)
      \retval     BGAPI2_RESULT_SUCCESS            No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \retval     BGAPI2_RESULT_NOT_INITIALIZED    Error producer was not loaded
      \retval     BGAPI2_RESULT_ERROR             Internal   Error (nullptr as system)
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR     Error on close of producer
      \see        BGAPI2_System_Open
      \see        BGAPI2_System_Close
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ReleaseSystem(BGAPI2_System* system);

    /**
      \ingroup    System
      \brief      Get the named node (feature) of given map of system
      \param[in]  system  Pointer to the system (producer)
      \param[in]  name    Name of the node to return
      \param[out] node    Variable for returned node value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetNode(
        BGAPI2_System* system,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    System
      \brief      Get a tree of all system nodes (features)
      \param[in]  system    Pointer to the system (producer)
      \param[out] node_tree Variable for the returned node tree
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetNodeTree(
        BGAPI2_System* system, BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    System
      \brief      Get a list of all system nodes (features)
      \param[in]  system    Pointer to the system (producer)
      \param[out] node_list Variable for the returned node map
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetNodeList(
        BGAPI2_System* system,
        BGAPI2_NodeMap** node_list);


    /**
      \ingroup    System
      \brief      Returns the identifier of the system (GenTL producer).

      The BGAPI2 C-Interface utilizes a two step process for the retrieval of strings.

      __1. Get the size of the string:__
      For the first call to BGAPI2_System_GetID, you need to supply the function with an null-pointer for the parameter
      ID. In this case the function will return you the size of the ID. You can now use this size to set up the pointer
      with the right size.

      __2. Get the actual ID string:__
      Now you can supply the function with the right sized pointer you created for the ID. In that case, the function
      will return the ID into your provided memory pointer.

      Alternatively, to save the extra call to get the size, you can supply the function with a larger memory pointer
      than required (e.g. 1024 byte).

      \param[in]  system           Pointer to the system (producer)
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetID(
        BGAPI2_System* system,
        char* ID,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the vendor of the system (GenTL producer)
      \param[in]  system           Pointer to the system (producer)
      \param[in,out] vendor        Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetVendor(
        BGAPI2_System* system,
        char* vendor,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the name (model) of the system (GenTL producer)
      \param[in]  system           Pointer to the system (producer)
      \param[in,out] model         Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetModel(
        BGAPI2_System* system,
        char* model,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the version of the system (GenTL producer)
      \param[in]  system           Pointer to the system (producer)
      \param[in,out] version       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetVersion(
        BGAPI2_System* system,
        char* version,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the name of the transport layer of the system (GenTL producer)
      \param[in]  system           Pointer to the system (producer)
      \param[in,out] tl_type       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetTLType(
        BGAPI2_System* system,
        char* tl_type,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the file name of the system (GenTL producer)
      \param[in]  system        Pointer to the system (producer)
      \param[out] name          Nullptr to get string length or pointer to store result
      \param[out] string_length Result size, length of name
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetFileName(
        BGAPI2_System* system,
        char* name,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the complete path name of the system (GenTL producer)
      \param[in]  system        Pointer to the system (producer)
      \param[out] path_name     Nullptr to get string length or pointer to store result
      \param[out] string_length Result size, length of path name
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetPathName(
        BGAPI2_System* system,
        char* path_name,
        bo_uint64* string_length);

    /**
      \ingroup    System
      \brief      Returns the "user friendly" display name of the system (GenTL producer)
      \param[in]  system           Pointer to the system (producer)
      \param[in,out] display_name  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_System_GetDisplayName(
        BGAPI2_System* system,
        char* display_name,
        bo_uint64* string_length);

    // INTERFACE functions
    /**
      \ingroup    Interface
      \brief      Opens an interface
      \param[in]  iface  Pointer to the interface obtained from BGAPI2_System_GetInterface()
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Interface is already opened
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_System_GetInterface
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_Open(BGAPI2_Interface* iface);

    /**
      \ingroup    Interface
      \brief      Checks the open state of a interface
      \param[in]  iface      Pointer to the interface
      \param[out] is_open    Result variable for open state of interface
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Interface_Open
      \see        BGAPI2_System_GetInterface
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_IsOpen(BGAPI2_Interface* iface, bo_bool *is_open);

    /**
      \ingroup    Interface
      \brief      Updates the list of devices on the interface and creates an object for each found device
      \param[in]  iface     Pointer to the interface
      \param[out] changed   True if devices have changed since last call, otherwise false
      \param[in]  timeout   Maximum time in milliseconds to search for devices
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read device infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_UpdateDeviceList(
        BGAPI2_Interface* iface,
        bo_bool* changed,
        bo_uint64 timeout);

    /**
      \ingroup    Interface
      \brief      Get a pointer to the device with supplied index
      \param[in]  iface     Pointer to the interface
      \param[in]  index     Index in the device list
      \param[out] device    Device pointer
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read device infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Interface_UpdateDeviceList
      \see        BGAPI2_Device_Open
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetDevice(
        BGAPI2_Interface* iface,
        bo_uint index,
        BGAPI2_Device** device);

    /**
      \ingroup    Interface
      \brief      Returns count of devices on interface
      \param[in]  iface         Instance of interface
      \param[out] count_devices Variable for count of devices on interface
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read device infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetNumDevices(
        BGAPI2_Interface* iface,
        bo_uint* count_devices);

    /**
      \ingroup    Interface
      \brief      Returns the parent object (GenTL producer) which the interface belongs to
      \param[in]  iface     Pointer to the interface
      \param[out] parent    Parent object
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetParent(
        BGAPI2_Interface* iface,
        BGAPI2_System** parent);

    /**
      \ingroup    Interface
      \brief      Closes an interfaces
      \param[in]  iface   Pointer to the interface
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_Close(BGAPI2_Interface* iface);

    /**
      \ingroup    Interface
      \brief      Get a named node of the interface
      \param[in]  iface     Pointer to the interface
      \param[in]  name      Node name
      \param[out] node      Pointer to store node value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetNode(
        BGAPI2_Interface* iface,
        const char* name,
        BGAPI2_Node ** node);

    /**
      \ingroup    Interface
      \brief      Get the node tree of the interface
      \param[in]  iface      Pointer to the interface
      \param[out] node_tree  Pointer to store node tree
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetNodeTree(
        BGAPI2_Interface* iface,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Interface
      \brief      Get the node list of the interface
      \param[in]  iface     Pointer to the interface
      \param[out] node_list Pointer to store node list
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetNodeList(
        BGAPI2_Interface* iface,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Interface
      \brief      Set the event mode (polling, callback, off)
      \param[in]  iface      Pointer to the interface
      \param[in]  event_mode Event mode for the PnP events of interface
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_SetPnPEventMode(
        BGAPI2_Interface* iface,
        BGAPI2_EventMode event_mode);


    /**
      \ingroup    Interface
      \brief      Returns the current event mode setting
      \param[in]  iface      Pointer to the interface
      \param[out] event_mode Pointer for event mode
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetPnPEventMode(
        BGAPI2_Interface* iface,
        BGAPI2_EventMode* event_mode);

    /**
      \ingroup    Interface
      \brief      Creates a structure to store pnp events retrieved via `BGAPI2_Interface_GetPnPEvent`
      \param[in,out] pnp_event Pointer to a struct which can store events
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Interface_GetPnPEvent
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_CreatePnPEvent(BGAPI2_PnPEvent** pnp_event);

    /**
      \ingroup    Interface
      \brief      Destroys a pnp event structure
      \param[in]  pnp_event Pointer to a event struct
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_CreatePnPEvent
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ReleasePnPEvent(BGAPI2_PnPEvent* pnp_event);

    /**
      \ingroup    Interface
      \brief      Polls for event information until timeout is reached

      This function is only used if `BGAPI2_EventMode` is set to `EVENTMODE_POLLING`

      \param[in]  iface         Pointer to interface
      \param[in, out] pnp_event Pointer for event structure created with `BGAPI2_CreatePnPEvent`
      \param[in]  timeout       Maximum time to wait for events, if set to -1 wait indefinitely
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
      \see        BGAPI2_CreatePnPEvent
      \see        BGAPI2_Interface_CancelGetPnPEvent
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetPnPEvent(
        BGAPI2_Interface* iface,
        BGAPI2_PnPEvent* pnp_event,
        bo_uint64 timeout);

    /**
      \ingroup    Interface
      \brief      Cancels an actively running `BGAPI2_Interface_GetPnPEvent`
      \param[in]  iface   Pointer to the interface
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error (on kill event)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_CancelGetPnPEvent(BGAPI2_Interface* iface);

    /**
      \ingroup    Interface
      \brief      Register one callback function to handle all pnp events of the interface.

      This function is only used if `BGAPI2_EventMode` is set to `EVENTMODE_EVENT_HANDLER`. It starts internal an thread to retrieve and queue events

      \param[in]  iface             Pointer to interface
      \param[in]  callback_owner    Data, context pointer for use in callback function
      \param[in]  pnp_event_handler Pointer to callback function from type `BGAPI2_PnPEventHandler`
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Could not start event thread
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
      \todo       check... what is the callback owner???
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_RegisterPnPEventHandler(
        BGAPI2_Interface* iface,
        void * callback_owner,
        BGAPI2_PnPEventHandler pnp_event_handler);

    /**
      \ingroup    Interface
      \brief      Returns the identifier of the interface

      The BGAPI2 C-Interface utilizes a two step process for the retrieval of strings.

      __1. Get the size of the string:__
      For the first call to `BGAPI2_Interface_GetID`, you need to supply the function with an null-pointer for the
      parameter ID. In this case the function will return you the size of the ID. You can now use this size to set up
      the pointer with the right size.

      __2. Get the actual ID string:__
      Now you can supply the function with the right sized pointer you created for the ID. In that case, the function
      will return the ID into your provided memory pointer.

      Alternatively, to save the extra call to get the size, you can supply the function with a larger memory pointer
      than required (e.g. 1024 byte).

      \param[in]  iface            Pointer to interface
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error.
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetID(
        BGAPI2_Interface* iface,
        char* ID,
        bo_uint64* string_length);

    /**
      \ingroup    Interface
      \brief      Returns the "user friendly" display name of interface, can be called on an interface which is not open
      \param[in]  iface            Instance of interface
      \param[in,out] display_name  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error.
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Interface_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetDisplayName(
        BGAPI2_Interface* iface,
        char* display_name,
        bo_uint64* string_length);

    /**
      \ingroup    Interface
      \brief      Returns the transport layer name of interface, can be called on an interface which is not open
      \param[in]  iface            Instance of interface
      \param[in,out] tl_type       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Interface_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Interface_GetTLType(
        BGAPI2_Interface* iface,
        char* tl_type,
        bo_uint64* string_length);

    // Device
    /**
      \ingroup    Device
      \brief      Opens a device for reading and writing
      \param[in]  device Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    GenTL producer error
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Device is already opened
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Interface_UpdateDeviceList
      \see        BGAPI2_Interface_GetDevice
      \see        BGAPI2_Device_Close
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_Open(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      Opens a device in exclusive mode
      \param[in]  device Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    GenTL producer error
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Device is already opened
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_OpenExclusive(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      Opens a device in read-only-mode
      \param[in]  device Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Interface not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    GenTL producer error
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Device is already opened
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_OpenReadOnly(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      Checks if the device is opened
      \param[in]  device Pointer to the device
      \param[out] is_open Result variable for open state of device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_IsOpen(BGAPI2_Device* device, bo_bool*is_open);

    /**
      \ingroup    Device
      \brief      Returns a pointer to the data stream handle identified by index
      \param[in]  device      Pointer to the device
      \param[in]  index       Index of the data stream
      \param[out] data_stream Pointer to the data stream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetDataStream(
        BGAPI2_Device* device,
        bo_uint index,
        BGAPI2_DataStream** data_stream);

    /**
      \ingroup    Device
      \brief      Returns the number of datastreams of the device
      \param[in]  device             Pointer to the device
      \param[out] count_data_streams Number of datastreams
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetNumDataStreams(
        BGAPI2_Device* device,
        bo_uint* count_data_streams);

    /**
      \ingroup    Device
      \brief      Closes a device
      \param[in]  device Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_Open
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_Close(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      Get a named node of the device
      \param[in]  device    Pointer to the device
      \param[in]  name      Node name
      \param[out] node      Pointer to store node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetNode(
        BGAPI2_Device* device,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    Device
      \brief      Get the node tree of the device
      \param[in]  device    Pointer to the device
      \param[out] node_tree Pointer to store node tree
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetNodeTree(
        BGAPI2_Device* device,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Device
      \brief      Get the node list of the device
      \param[in]  device    Pointer to the device
      \param[out] node_list Pointer to store node list
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetNodeList(
        BGAPI2_Device* device,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Device
      \brief      Set the device event mode (polling, callback, off)
      \param[in]  device     Pointer to the device
      \param[in]  event_mode Event mode for device events
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_SetDeviceEventMode(
        BGAPI2_Device* device,
        BGAPI2_EventMode event_mode);

    /**
      \ingroup    Device
      \brief      Get the device event mode (polling, callback, off)
      \param[in]  device     Pointer to the device
      \param[out] event_mode Event mode of device events
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetDeviceEventMode(
        BGAPI2_Device* device,
        BGAPI2_EventMode* event_mode);

    /**
      \ingroup    Device
      \brief      Creates a structure to store device events retrieved via `BGAPI2_Device_GetDeviceEvent`
      \param[in,out] device_event Pointer to a struct which can store events
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetDeviceEvent
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_CreateDeviceEvent(BGAPI2_DeviceEvent** device_event);

    /**
      \ingroup    Device
      \brief      Destroys a device event structure
      \param[in]  device_event Pointer to a event struct
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ReleaseDeviceEvent(BGAPI2_DeviceEvent* device_event);

    /**
      \ingroup    Device
      \brief      Polls for event information until timeout is reached
      \param[in]  device       Pointer to the device
      \param[in]  device_event Pointer for events structure created with `BGAPI2_CreateDeviceEvent`
      \param[in]  timeout      Maximum time to wait for events, if set to -1 wait indefinitely
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
      \see        BGAPI2_CreateDeviceEvent
      \see        BGAPI2_Device_CancelGetDeviceEvent
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetDeviceEvent(
        BGAPI2_Device* device,
        BGAPI2_DeviceEvent* device_event,
        bo_uint64 timeout);

    /**
      \ingroup    Device
      \brief      Cancels a running `BGAPI2_Device_GetDeviceEvent`
      \param[in]  device Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_CancelGetDeviceEvent(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      Register one callback function to handle all device events of the interface
      \param[in]  device               Pointer to the device
      \param[in]  callback_owner       Data, context pointer for use in callback function
      \param[in]  device_event_handler Pointer to callback function from type `BGAPI2_DevEventHandler`
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Could not start event thread
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \todo       check... what is the callback owner???
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_RegisterDeviceEventHandler(
        BGAPI2_Device* device,
        void* callback_owner,
        BGAPI2_DevEventHandler device_event_handler);

    /**
      \ingroup    Device
      \brief      Returns the payload size in bytes, used to allocate image buffers
      \param[in]  device       Pointer to the device
      \param[out] payload_size Pointer to payload size in bytes
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetPayloadSize(
        BGAPI2_Device* device,
        bo_uint64* payload_size);

    /**
      \ingroup    Device
      \brief      Get the named node of given remote device
      \param[in]  device Pointer to the device
      \param[in]  name    Node name
      \param[out] node    Pointer to store the node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no node found)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetRemoteNode(
        BGAPI2_Device* device,
        const char* name,
        BGAPI2_Node** node);

    /**
    \ingroup    Device
    \brief      Get the update node tree of the device
    \param[in]  device    Pointer to the device
    \param[out] node_tree Pointer to store the node tree
    \retval     BGAPI2_RESULT_SUCCESS           No error
    \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
    \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
    \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetRemoteNodeTree(
        BGAPI2_Device* device,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Device
      \brief      Get the update node list of the device
      \param[in]  device    Pointer to the device
      \param[out] node_list Pointer to store the node list
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetRemoteNodeList(
        BGAPI2_Device* device,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Device
      \brief      Returns the identifier of device, can be called on a device which is not open

      The BGAPI2 C-Interface utilizes a two step process for the retrieval of strings.

      __1. Get the size of the string:__
      For the first call to `BGAPI2_Device_GetID`, you need to supply the function with an null-pointer for the
      parameter ID. In this case the function will return you the size of the ID. You can now use this size to set up
      the pointer with the right size.

      __2. Get the actual ID string:__
      Now you can supply the function with the right sized pointer you created for the ID. In that case, the function
      will return the ID into your provided memory pointer.

      Alternatively, to save the extra call to get the size, you can supply the function with a larger memory pointer
      than required (e.g. 1024 byte).

      \param[in]  device           Pointer to device
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetID(
        BGAPI2_Device* device,
        char* ID,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the vendor of device, can be called on a device which is not open
      \param[in]  device           Pointer to the device
      \param[in,out] vendor        Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetVendor(
        BGAPI2_Device* device,
        char* vendor,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the name (model) of the device, can be called on a device which is not open
      \param[in]  device           Pointer to the device
      \param[in,out] model         Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetModel(
        BGAPI2_Device* device,
        char* model,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the serial number of device, can be called on a device which is not open
      \param[in]  device            Pointer to the device
      \param[in,out] serial_number  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetSerialNumber(
        BGAPI2_Device* device,
        char* serial_number,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the transport layer of device, can be called on a device which is not open
      \param[in]  device           Pointer to the device
      \param[in,out] tl_type       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetTLType(
        BGAPI2_Device* device,
        char* tl_type,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the "user friendly" display name of device, can be called on a device which is not open
      \param[in]  device           Pointer to the device
      \param[in,out] display_name  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetDisplayName(
        BGAPI2_Device* device,
        char* display_name,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the access state of device, can be called on a device which is not open
      \param[in]  device           Pointer to the device
      \param[in,out] access_status Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetAccessStatus(
        BGAPI2_Device* device,
        char* access_status,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the configuration file (xml) of the remote device
      \param[in]  device           Pointer to the device
      \param[in,out] config_file   Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no remote device)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error device not opened
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetRemoteConfigurationFile(
        BGAPI2_Device* device,
        char* config_file,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Sets a configuration file (xml) to use with the attached remote device (camera)
      \param[in]  device      Pointer to the device
      \param[in]  config_file Configuration file
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Error device is opened
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_SetRemoteConfigurationFile(
        BGAPI2_Device* device,
        const char* config_file);

    /**
      \ingroup    Device
      \brief      Starts a stack of feature writes to the device (caching write operations).

      If many features are written to the device, this becomes slow as each write is acknowledged by the device.
      The feature stacking will send many commands at once and therefore reduces the roundtrip times

      \param[in]  device       Pointer to the device
      \param[in]  replace_mode True to enable combining of writes to the same register on the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (remote device not opened)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_StartStacking(
        BGAPI2_Device* device,
        bo_bool replace_mode);

    /**
      \ingroup    Device
      \brief      End the stacking and write all featues to the device
      \param[in]  device Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (remote device not opened)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_StartStacking
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_WriteStack(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      End the stacking without writing featues to the device
      \param[in]  device     Pointer to the device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (remote device not opened)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_StartStacking
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_CancelStack(BGAPI2_Device* device);

    /**
      \ingroup    Device
      \brief      Checks for availability of update state of device
      \param[in]  device       Pointer to the device
      \param[out] is_available Pointer for open state of device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_IsUpdateModeAvailable(
        BGAPI2_Device* device,
        bo_bool* is_available);

    /**
      \ingroup    Device
      \brief      Returns the update mode state of device.
      \param[in]  device    Pointer to the device
      \param[out] is_active Pointer to get update mode state
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_IsUpdateModeActive(
        BGAPI2_Device* device,
        bo_bool* is_active);

    /**
      \ingroup    Device
      \brief      Set the update mode state of device
      \param[in]  device      Pointer to the device
      \param[in]  update_mode Update mode state to set
      \param[in]  custom_key  Custom key for security
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_RESOURCE_IN_USE   Update mode is not enabled
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error for not opened device
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Update mode is not available
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_SetUpdateMode(
        BGAPI2_Device* device,
        bo_bool update_mode,
        const char* custom_key);

    /**
      \ingroup    Device
      \brief      Get the named node of the device
      \param[in]  device  Pointer to the device
      \param[in]  name    Node name
      \param[out] node    Pointer to store node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no node found)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error update mode is not set
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetUpdateNode(
        BGAPI2_Device* device,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    Device
      \brief      Get the update node tree of the device
      \param[in]  device    Pointer to the device
      \param[out] node_tree Update node map of device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetUpdateNodeTree(
        BGAPI2_Device* device,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Device
      \brief      Get the update node list of the device
      \param[in]  device    Pointer to the device
      \param[out] node_list Update node map of device
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetUpdateNodeList(
        BGAPI2_Device* device,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Device
      \brief      Returns the configuration file (xml) of the update device.
      \param[in]  device           Pointer to the device
      \param[in,out] config_file   Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error update mode is not set
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetUpdateConfigurationFile(
        BGAPI2_Device* device,
        char* config_file,
        bo_uint64* string_length);

    /**
      \ingroup    Device
      \brief      Returns the parent object (interface) of the device
      \param[in]  device  Pointer to the device
      \param[out] parent  The parent object
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Device_GetParent(
        BGAPI2_Device* device,
        BGAPI2_Interface** parent);

    // Node
    /**
      \ingroup    Node
      \brief      Returns the data type (interface) of the node

      The BGAPI2 C-Interface utilizes a two step process for the retrieval of strings.

      __1. Get the size of the string:__
      For the first call to BGAPI2_Node_GetInterface, you need to supply the function with an null-pointer for the parameter interface. In this case the function will return you the size of the interface. You can now use this size to set up the pointer with the right size.

      __2. Get the actual interface string:__
      Now you can supply the function with the right sized pointer you created for the interface. In that case, the function will return the interface into your provided memory pointer.

      Alternatively, to save the extra call to get the size, you can supply the function with a larger memory pointer
      than required (e.g. 1024 byte).

      \param[in]  node             Pointer to the node
      \param[in,out] iface         Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No interface name on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetInterface(
        BGAPI2_Node* node, char* iface,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the extension (vendor specific information) of node
      \param[in]  node             Pointer to the node
      \param[in,out] extension     Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetExtension(
        BGAPI2_Node* node,
        char* extension,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the tooltip of node
      \param[in]  node              Pointer to the node
      \param[in,out] tool_tip       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetToolTip(
        BGAPI2_Node* node,
        char*  tool_tip,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the description of node
      \param[in]  node             Pointer to the node
      \param[in,out] description   Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDescription(
        BGAPI2_Node* node,
        char*  description,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the name of node
      \param[in]  node             Pointer to the node
      \param[in,out] name          Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetName(
        BGAPI2_Node* node,
        char* name,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the "user friendly" display name of node
      \param[in]  node              Pointer to the node
      \param[in,out] display_name   Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDisplayname(
        BGAPI2_Node* node,
        char* display_name,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the visibility of node
      \param[in]  node             Pointer to the node
      \param[in,out] visibility    Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetVisibility(
        BGAPI2_Node* node,
        char*  visibility,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the event identifier of node
      \param[in]  node              Pointer to the node
      \param[out] event_id          Pointer to store the event identifier
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetEventID(BGAPI2_Node* node, bo_int64* event_id);

    /**
      \ingroup    Node
      \brief      Returns the implemented state of node.
      \param[in]  node            Pointer to the node
      \param[out] is_implemented  Pointer to store the implemented state of node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetImplemented(BGAPI2_Node* node, bo_bool* is_implemented);

    /**
      \ingroup    Node
      \brief      Returns the availability state of node
      \param[in]  node          Pointer to the node
      \param[out] is_available  Pointer to store the availability state of node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetAvailable(BGAPI2_Node* node, bo_bool* is_available);

    /**
      \ingroup    Node
      \brief      Returns the locked state of node
      \param[in]  node       Pointer to the node
      \param[out] is_locked  Pointer to store the locked state of node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetLocked(BGAPI2_Node* node, bo_bool* is_locked);

    /**
      \ingroup    Node
      \brief      Returns the imposed access mode of node
      \param[in]  node                   Pointer to the node
      \param[in,out] imposed_access_mode Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetImposedAccessMode(
        BGAPI2_Node* node,
        char* imposed_access_mode,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the current access mode of node
      \param[in]  node                   Pointer to the node
      \param[in,out] current_access_mode Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetCurrentAccessMode(
        BGAPI2_Node* node,
        char* current_access_mode,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the readability state of node
      \param[in]  node         Pointer to the node
      \param[out] is_readable  Pointer to store the readability state of node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_IsReadable(BGAPI2_Node* node, bo_bool* is_readable);

    /**
      \ingroup    Node
      \brief      Returns the writeability state of node
      \param[in]  node         Pointer to the node
      \param[out] is_writable  Pointer to store the writeability state of node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_IsWriteable(BGAPI2_Node* node, bo_bool* is_writable);

    /**
      \ingroup    Node
      \brief      Returns the alias name of node
      \param[in]  node             Pointer to the node
      \param[in,out] alias         Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetAlias(
        BGAPI2_Node* node,
        char* alias,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the value of node as a string
      \param[in]  node             Pointer to the node
      \param[in,out] value         Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetValue(
        BGAPI2_Node* node,
        char* value,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Sets a string value to a node
      \param[in]  node      Pointer to the node
      \param[in]  value     Pointer to the value to be written
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_SetValue(BGAPI2_Node* node, const char* value);

    /**
      \ingroup    Node
      \brief      Returns the representation of the node
      \param[in]  node              Pointer to the node
      \param[in,out] representation Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No extension on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetRepresentation(
        BGAPI2_Node* node,
        char* representation,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the minimal integer value of node
      \param[in]  node  Pointer to the node
      \param[out] min   Pointer to store the minimal integer value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetIntMin(BGAPI2_Node* node, bo_int64* min);

    /**
      \ingroup    Node
      \brief      Returns the maximum integer value of node
      \param[in]  node Pointer to the node
      \param[out] max  Pointer to store the maximum integer value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetIntMax(BGAPI2_Node* node, bo_int64* max);

    /**
      \ingroup    Node
      \brief      Returns the integer increment value of node
      \param[in]  node Pointer to the node
      \param[out] inc  Pointer to store the integer increment value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetIntInc(BGAPI2_Node* node, bo_int64* inc);

    /**
      \ingroup    Node
      \brief      Returns the integer value of the node
      \param[in]  node  Pointer to the node
      \param[out] value Pointer to store the integer value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetInt(BGAPI2_Node* node, bo_int64* value);

    /**
      \ingroup    Node
      \brief      Sets the integer value of node
      \param[in]  node  Pointer to the node
      \param[in]  value Integer value for node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_SetInt(BGAPI2_Node* node, bo_int64 value);

    /**
      \ingroup    Node
      \brief      Returns the availability of a unit for the node
      \param[in]  node  Pointer to the node
      \param[out] has_unit Pointer to store the availability of a unit for the node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_HasUnit(BGAPI2_Node* node, bo_bool* has_unit);

    /**
      \ingroup    Node
      \brief      Returns the unit of the node
      \param[in]  node             Pointer to the node
      \param[in,out] unit          Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No unit name on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetUnit(
        BGAPI2_Node* node,
        char*  unit,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Returns the minimal double value of the node
      \param[in]  node  Pointer to the node
      \param[out] min   Pointer to store the minimal double value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDoubleMin(BGAPI2_Node* node, bo_double* min);

    /**
      \ingroup    Node
      \brief      Returns the maximum double value of the node
      \param[in]  node  Pointer to the node
      \param[out] max   Pointer to store the maximum double value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDoubleMax(BGAPI2_Node* node, bo_double* max);

    /**
      \ingroup    Node
      \brief      Returns the double increment value of the node
      \param[in]  node Pointer to the node
      \param[out] inc  Pointer to store the double increment value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_HasInc
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDoubleInc(BGAPI2_Node* node, bo_double* inc);

    /**
      \ingroup    Node
      \brief      Returns the availability of an increment value of the node
      \param[in]  node     Pointer to the node
      \param[out] has_inc  Pointer to store the availability of increment value unit
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_HasInc(BGAPI2_Node* node, bo_bool* has_inc);

    /**
      \ingroup    Node
      \brief      Returns the double precision value of node
      \param[in]  node  Pointer to the node
      \param[out] prec  Pointer to store the double precision value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDoublePrecision(BGAPI2_Node* node, bo_uint64* prec);

    /**
      \ingroup    Node
      \brief      Returns the double value of node
      \param[in]  node  Pointer to the node
      \param[out] value Pointer to store the double value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetDouble(BGAPI2_Node* node, bo_double* value);

    /**
      \ingroup    Node
      \brief      Sets the double value of node
      \param[in]  node  Pointer to the node
      \param[in]  value New double value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_SetDouble(BGAPI2_Node* node, bo_double value);

    /**
      \ingroup    Node
      \brief      Returns the maximum string length value of node
      \param[in]  node              Pointer to the node
      \param[out] max_string_length Pointer to store the maximum string length value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetMaxStringLength(
        BGAPI2_Node* node,
        bo_int64* max_string_length);

    /**
      \ingroup    Node
      \brief      Returns the string value of the node
      \param[in]  node             Pointer to the node
      \param[in,out] value         Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No string on node
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Node_GetInterface for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetString(
        BGAPI2_Node* node,
        char*  value,
        bo_uint64* string_length);

    /**
      \ingroup    Node
      \brief      Sets the string value of the node
      \param[in]  node      Pointer to the node
      \param[in]  value     String value to set
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_SetString(BGAPI2_Node* node, const char* value);

    /**
      \ingroup    Node
      \brief      Gets the enumeration node map of the node
      \param[in]  node           Pointer to the node
      \param[out] enum_node_map  Pointer to store the enumeration node map
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetEnumNodeList(
        BGAPI2_Node* node,
        BGAPI2_NodeMap** enum_node_map);

    /**
      \ingroup    Node
      \brief      Executes the selected node
      \param[in]  node Pointer to the node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_Execute(BGAPI2_Node* node);

    /**
      \ingroup    Node
      \brief      Checks if the execution of node is done
      \param[in]  node     Pointer to the node
      \param[out] is_done  Pointer to store the execution done
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_IsDone(BGAPI2_Node* node, bo_bool *is_done);

    /**
      \ingroup    Node
      \brief      Returns the boolean value of the node
      \param[in]  node   Pointer to the node
      \param[out] value  Pointer to store the boolean value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetBool(BGAPI2_Node* node, bo_bool* value);

    /**
      \ingroup    Node
      \brief      Sets the boolean value of the node
      \param[in]  node      Pointer to the node
      \param[in]  value     A boolean value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_SetBool(BGAPI2_Node* node, bo_bool value);

    /**
      \ingroup    Node
      \brief      Gets the node tree of the node
      \param[in]  node       Pointer to the node
      \param[out] node_tree  Pointer to store the node tree
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetNodeTree(BGAPI2_Node* node, BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Node
      \brief      Gets the node list of the node
      \param[in]  node       Pointer to the node
      \param[out] node_list  Pointer to store the node list with node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetNodeList(BGAPI2_Node* node, BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Node
      \brief      Checks if the node is an selector
      \param[in]  node         Pointer to the node
      \param[out] is_selector  Pointer to store the is_selector value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_IsSelector(BGAPI2_Node* node, bo_bool* is_selector);

    /**
      \ingroup    Node
      \brief      Returns the selected features of node
      \param[in]  node              Pointer to the node
      \param[out] selected_features Pointer to store the selected features
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetSelectedFeatures(
        BGAPI2_Node* node,
        BGAPI2_NodeMap** selected_features);

    /**
      \ingroup    Node
      \brief      Returns the (data) length of node
      \param[in]  node    Pointer to the node
      \param[out] length  Pointer to store the (data) length value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetLength(BGAPI2_Node* node, bo_int64* length);

    /**
      \ingroup    Node
      \brief      Returns the (data) address of node
      \param[in]  node     Pointer to the node
      \param[out] address  Pointer to store the (data) address value
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_GetAddress(BGAPI2_Node* node, bo_int64* address);

    /**
      \ingroup    Node
      \brief      Returns the data of node
      \param[in]  node    Pointer to the node
      \param[out] buffer  Pointer to store the data
      \param[in]  length  Length of the data
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_Get(BGAPI2_Node* node, void* buffer, bo_int64 length);

    /**
      \ingroup    Node
      \brief      Sets the data of node
      \param[in]  node    Pointer to the node
      \param[in]  buffer  Variable with data
      \param[in]  length  Data length of node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (no nodemap found)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Node_Set(BGAPI2_Node* node, void* buffer, bo_int64 length);

    // Nodemap
    /**
      \ingroup    NodeMap
      \brief      Gets the named node of given map
      \param[in]  node_map  Pointer to the nodemap
      \param[in]  name      Node name
      \param[out] node      Pointer to store the node
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_NodeMap_GetNode(
        BGAPI2_NodeMap* node_map,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    NodeMap
      \brief      Returns the number of nodes in nodemap
      \param[in]  node_map  Pointer to the nodemap
      \param[out] count     Pointer to store the node count
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_NodeMap_GetNodeCount(BGAPI2_NodeMap* node_map, bo_uint64* count);

    /**
      \ingroup    NodeMap
      \brief      Returns the node with the index
      \param[in]  node_map Pointer to the nodemap
      \param[in]  index    Index of the node
      \param[out] node     Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, node with iIndex not found
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_NodeMap_GetNodeByIndex(
        BGAPI2_NodeMap* node_map,
        bo_uint64 index,
        BGAPI2_Node** node);

    /**
      \ingroup    NodeMap
      \brief      Checks if node is present in nodemap
      \param[in]  node_map    Pointer to the nodemap
      \param[in]  name        Name of node
      \param[out] is_present  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_NodeMap_GetNodePresent(
        BGAPI2_NodeMap* node_map,
        const char* name,
        bo_bool* is_present);

    // Events
     /**
      \ingroup    DeviceEvent
      \brief      Gets the named node of given map of interface
      \param[in]  device_event  Pointer to the device event
      \param[in]  name          Node name
      \param[out] node          Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetNode(
        BGAPI2_DeviceEvent* device_event,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    DeviceEvent
      \brief      Gets the node map of interface (tree elements)
      \param[in]  device_event  Pointer to the device event
      \param[out] node_tree     Pointer to store the node tree
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetNodeTree(
        BGAPI2_DeviceEvent* device_event,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    DeviceEvent
      \brief      Gets the node map of interface (list of entries)
      \param[in]  device_event  Pointer to the device event
      \param[out] node_tree     Pointer to store the node tree
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetNodeList(
        BGAPI2_DeviceEvent* device_event,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    DeviceEvent
      \brief      Returns the name of device event
      \param[in]  device_event     Pointer to the device event
      \param[in,out] name          Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_IMPLEMENTED   No unit name on device event
      \retval     BGAPI2_RESULT_ERROR             Internal error (access denied, invalid parameter, ..)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetName(
        BGAPI2_DeviceEvent* device_event,
        char* name,
        bo_uint64* string_length);

    /**
      \ingroup    DeviceEvent
      \brief      Returns the "user friendly" display name of device event
      \param[in]  device_event     Pointer to the device event
      \param[in,out] display_name  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     The display name was not included in the XML description of the event
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetDisplayName(
        BGAPI2_DeviceEvent* device_event,
        char* display_name,
        bo_uint64* string_length);

    /**
      \ingroup    DeviceEvent
      \brief      Returns the timestamp of device event
      \param[in]  device_event  Pointer to the device event
      \param[out] time_stamp    Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     No timestamp on device event of timestamp equal zero
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetTimeStamp(
        BGAPI2_DeviceEvent* device_event,
        bo_uint64* time_stamp);

    /**
      \ingroup    DeviceEvent
      \brief      Returns the identifier of device event
      \param[in]  device_event     Pointer to the device event
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NO_DATA           Internal error
      \retval     BGAPI2_RESULT_INVALID_BUFFER    Internal error, destination buffer too small
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeviceEvent_GetID(
        BGAPI2_DeviceEvent* device_event,
        char* ID,
        bo_uint64* string_length);

    /**
      \ingroup    PnPEvent
      \brief      Returns the serial number of pnp event
      \param[in]  pnp_event         Pointer to the pnp event
      \param[in,out] serial_number  Nullptr to get string length or pointer to store result
      \param[in,out] string_length  Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_BUFFER    Internal error, destination buffer too small
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_PnPEvent_GetSerialNumber(
        BGAPI2_PnPEvent* pnp_event,
        char* serial_number,
        bo_uint64* string_length);

    /**
      \ingroup    PnPEvent
      \brief      Returns the type of pnp event
      \param[in]  pnp_event  Pointer to the pnp event
      \param[out] pnp_type   Pointe to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_PnPEvent_GetPnPType(
        BGAPI2_PnPEvent* pnp_event,
        bo_uint64* pnp_type);

    /**
      \ingroup    PnPEvent
      \brief      Returns the identifier of pnp event
      \param[in]  pnp_event        Pointer to the pnp event
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error, no identifier available
      \retval     BGAPI2_RESULT_INVALID_BUFFER    Internal error, destination buffer too small
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_PnPEvent_GetID(
        BGAPI2_PnPEvent* pnp_event,
        char* ID,
        bo_uint64* string_length);

    // Buffer
    /**
      \ingroup    Buffer
      \brief      Creates a new buffer
      \param[out] buffer     Pointer to store new buffer object
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_CreateBuffer(BGAPI2_Buffer **buffer);

    /**
      \ingroup    Buffer
      \brief      Returns a new buffer, initialized with user parameter
      \param[out] buffer    Variable for new buffer object
      \param[in]  user_obj  Additional user parameter to store in buffer
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_CreateBufferWithUserPtr(BGAPI2_Buffer** buffer, void *user_obj);

    /**
      \ingroup    Buffer
      \brief      Returns a new buffer, initialized with user parameter and external memory

      Function to create a Buffer-Object. When using this function, the user takes care of allocation
      of the necessary memory. To use the actual necessary memory size the functions Device::GetPayloadSize and
      DataStream::GetPayloadSize are used respectively. To use the maximum required memory size of a device the
      maximum of the   PayloadSize   feature is queried. See Device::GetRemoteNode and Node::GetIntMax.

      \param[out] buffer           Variable for new buffer object
      \param[in]  user_buffer      Pointer to a user allocated data buffer.
      \param[in]  user_buffer_size The size of the user allocated data buffer
      \param[in]  user_obj         Pointer to a user allocated memory
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_CreateBufferWithExternalMemory(
        BGAPI2_Buffer** buffer,
        void* user_buffer,
        bo_uint64 user_buffer_size,
        void* user_obj);

    /**
      \ingroup    Buffer
      \brief      Destroys the buffer
      \param[in]  buffer     The buffer object
      \param[in]  user_obj   Pointer to a user allocated memory
      \retval     BGAPI2_RESULT_SUCCESS           No error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DeleteBuffer(BGAPI2_Buffer* buffer, void** user_obj);

    /**
      \ingroup    Buffer
      \brief      Gets a node of the buffer
      \param[in]  buffer    Pointer to the buffer
      \param[in]  name      Node name
      \param[out] node      Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetNode(
        BGAPI2_Buffer* buffer,
        char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    Buffer
      \brief      Gets the node tree of the buffer
      \param[in]  buffer    Pointer to the buffer
      \param[out] node_tree Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetNodeTree(
        BGAPI2_Buffer* buffer,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Buffer
      \brief      Gets the node list of the buffer
      \param[in]  buffer    Pointer to the buffer
      \param[out] node_list Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetNodeList(
        BGAPI2_Buffer* buffer,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Buffer
      \brief      Gets the chunk node list of given buffer
      \param[in]  buffer    Pointer to the buffer
      \param[out] node_list Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, chunk is not initialized
      \retval     BGAPI2_RESULT_ERROR             Error for invalid buffer
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetChunkNodeList(
        BGAPI2_Buffer* buffer,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    Buffer
      \brief      Returns the identifier of the buffer
      \param[in]  buffer           Pointer to the buffer
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer device infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetID(
        BGAPI2_Buffer* buffer,
        char* ID,
        bo_uint64* string_length);


    /**
      \ingroup    Buffer
      \brief      Returns the memory (data) pointer of buffer
      \param[in]  buffer    Pointer to the buffer
      \param[out] mem_ptr   Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetMemPtr(BGAPI2_Buffer* buffer, void** mem_ptr);

    /**
      \ingroup    Buffer
      \brief      Returns the memory size of buffer
      \param[in]  buffer      Pointer to the buffer
      \param[out] buffer_size Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetMemSize(BGAPI2_Buffer* buffer, bo_uint64* buffer_size);

    /**
      \ingroup    Buffer
      \brief      Returns the user pointer of buffer
      \param[in]  buffer  Pointer to the buffer
      \param[out] user    Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetUserPtr(BGAPI2_Buffer* buffer, void** user);

    /**
      \ingroup    Buffer
      \brief      Delivers the timestamp of the buffer obtained by the camera
      \param[in]  buffer     Pointer to the buffer
      \param[out] timestamp  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetTimestamp(BGAPI2_Buffer* buffer, bo_uint64* timestamp);

    /**
      \ingroup    Buffer
      \brief      Returns the host time stamp of the first received packet of a new image using a steady clock
      \param[in]  buffer         Pointer to the buffer
      \param[out] host_timestamp Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetHostTimestamp(
        BGAPI2_Buffer* buffer,
        bo_uint64* host_timestamp);

    /**
      \ingroup    Buffer
      \brief      Returns the flag for new data of buffer
      \param[in]  buffer     Pointer to the buffer
      \param[out] new_data   Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetNewData(BGAPI2_Buffer* buffer, bo_bool* new_data);

    /**
      \ingroup    Buffer
      \brief      Returns the flag for queued buffer
      \param[in]  buffer     Pointer to the buffer
      \param[out] is_queued  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetIsQueued(BGAPI2_Buffer* buffer, bo_bool* is_queued);

    /**
      \ingroup    Buffer
      \brief      Returns the flag for acquiring data of buffer
      \param[in]  buffer       Pointer to the buffer
      \param[out] is_acquiring Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetIsAcquiring(BGAPI2_Buffer* buffer, bo_bool* is_acquiring);

    /**
      \ingroup    Buffer
      \brief      Returns the flag for incomplete data of buffer
      \param[in]  buffer        Pointer to the buffer
      \param[out] is_incomplete Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetIsIncomplete(
        BGAPI2_Buffer* buffer, bo_bool* is_incomplete);

    /**
      \ingroup    Buffer
      \brief      Returns the transport layer of buffer
      \param[in]  buffer           Pointer to the buffer
      \param[in,out] tl_type       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetTLType(
        BGAPI2_Buffer* buffer,
        char* tl_type,
        bo_uint64* string_length);

    /**
      \ingroup    Buffer
      \brief      Returns the current size of data of buffer
      \param[in]  buffer      Pointer to the buffer
      \param[out] size_filled Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetSizeFilled(BGAPI2_Buffer* buffer, bo_uint64* size_filled);

    /**
      \ingroup    Buffer
      \brief      Returns the width (in pixel) of buffer
      \param[in]  buffer  Pointer to the buffer
      \param[out] width   Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetWidth(BGAPI2_Buffer* buffer, bo_uint64* width);

    /**
      \ingroup    Buffer
      \brief      Returns height (in pixel) of buffer
      \param[in]  buffer  Pointer to the buffer
      \param[out] height  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetHeight(BGAPI2_Buffer* buffer, bo_uint64* height);

    /**
      \ingroup    Buffer
      \brief      Returns x offset (in pixel) of buffer
      \param[in]  buffer     Pointer to the buffer
      \param[out] offset_x   Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetXOffset(BGAPI2_Buffer* buffer, bo_uint64* offset_x);

    /**
      \ingroup    Buffer
      \brief      Returns y offset (in pixel) of buffer
      \param[in]  buffer    Pointer to the buffer
      \param[out] offset_y  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetYOffset(BGAPI2_Buffer* buffer, bo_uint64* offset_y);

    /**
      \ingroup    Buffer
      \brief      Returns x padding bytes (number of extra bytes in each line) of buffer
      \param[in]  buffer     Pointer to the buffer
      \param[out] padding_x  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetXPadding(BGAPI2_Buffer* buffer, bo_uint64* padding_x);

    /**
      \ingroup    Buffer
      \brief      Returns y padding bytes (number of extra bytes at image end) of buffer
      \param[in]  buffer     Pointer to the buffer
      \param[out] padding_y  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetYPadding(BGAPI2_Buffer* buffer, bo_uint64* padding_y);

    /**
      \ingroup    Buffer
      \brief      Returns the frame identifier of buffer
      \param[in]  buffer    Pointer to the buffer
      \param[out] frame_id  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetFrameID(BGAPI2_Buffer* buffer, bo_uint64* frame_id);

    /**
      \ingroup    Buffer
      \brief      Returns the flag for available image of buffer
      \param[in]  buffer        Pointer to the buffer
      \param[out] image_present Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetImagePresent(
        BGAPI2_Buffer* buffer, bo_bool* image_present);

    /**
      \ingroup    Buffer
      \brief      Returns the offset into buffer memory to begin of data of buffer
      \param[in]  buffer       Pointer to the buffer
      \param[out] image_offset Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetImageOffset(
        BGAPI2_Buffer* buffer, bo_uint64* image_offset);

    /**
      \ingroup    Buffer
      \brief      Returns the payload type of buffer
      \param[in]  buffer           Pointer to the buffer
      \param[in,out] payload_type  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetPayloadType(
        BGAPI2_Buffer* buffer,
        char* payload_type,
        bo_uint64* string_length);

    /**
    \ingroup    Buffer
    \brief      Returns the payload type of buffer
    \param[in]  buffer           Pointer to the buffer
    \param[in,out] pixelformat   Nullptr to get string length or pointer to store result
    \param[in,out] string_length Result size, length of version string (including string end zero)
    \retval     BGAPI2_RESULT_SUCCESS           No error
    \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
    \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetPixelFormat(
        BGAPI2_Buffer* buffer,
        char* pixelformat,
        bo_uint64* string_length);

    /**
      \ingroup    Buffer
      \brief      Returns the delivered image height of buffer
      \param[in]  buffer                 Pointer to the buffer
      \param[out] delivered_image_height Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetDeliveredImageHeight(
        BGAPI2_Buffer* buffer,
        bo_uint64* delivered_image_height);

    /**
      \ingroup    Buffer
      \brief      Returns the delivered chunk payload size of buffer
      \param[in]  buffer                        Pointer to the buffer
      \param[out] delivered_chunk_payload_size  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetDeliveredChunkPayloadSize(
        BGAPI2_Buffer* buffer,
        bo_uint64* delivered_chunk_payload_size);

    /**
      \ingroup    Buffer
      \brief      Returns the flag to indicating existing chunk of buffer
      \param[in]  buffer          Pointer to the buffer
      \param[out] contains_chunk  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetContainsChunk(
        BGAPI2_Buffer* buffer,
        bo_bool* contains_chunk);

    /**
      \ingroup    Buffer
      \brief      Returns the chunk layout identifier size of buffer
      \param[in]  buffer           Pointer to the buffer
      \param[out] chunk_layout_id  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetChunkLayoutID(
        BGAPI2_Buffer* buffer,
        bo_uint64* chunk_layout_id);

    /**
      \ingroup    Buffer
      \brief      Returns the filename of buffer (only for payload type = file)
      \param[in]  buffer           Pointer to the buffer
      \param[in,out] file_name     Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetFileName(
        BGAPI2_Buffer* buffer,
        char* file_name,
        bo_uint64* string_length);

    /**
      \ingroup    Buffer
      \brief      Returns the parent object (data stream) which belongs to
      \param[in]  buffer     Pointer to the buffer
      \param[out] parent     Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     In case the buffer was revoked from the data stream
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Buffer_GetParent(
        BGAPI2_Buffer* buffer,
        BGAPI2_DataStream** parent);

    // DataStream
    /**
      \ingroup    DataStream
      \brief      Opens a datastream
      \param[in]  data_stream  Pointer to the DataStream created with BGAPI2_Device_GetDataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Can't read producer interface infos
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Device_GetDataStream
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_Open(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Closes a datastream
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error (init failed, data stream not opened)
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_Close(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Checks if the data stream is opened
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] is_open      Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Producer not initialized
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_IsOpen(BGAPI2_DataStream* data_stream, bo_bool* is_open);

    /**
      \ingroup    DataStream
      \brief      Gets the named node of given datastream
      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  name         Node name
      \param[out] node         Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNode(
        BGAPI2_DataStream* data_stream,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    DataStream
      \brief      Gets the node tree of given datastream
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] node_tree    Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNodeTree(
        BGAPI2_DataStream* data_stream,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    DataStream
      \brief      Gets the node list of given datastream
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] node_list    Node map
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNodeList(
        BGAPI2_DataStream* data_stream,
        BGAPI2_NodeMap** node_list);

    /**
      \ingroup    DataStream
      \brief      Sets the new buffer event mode. The event mode is controlled by the event register functions
      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  event_mode   Event mode for new buffer events of datastream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_EventMode
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_SetNewBufferEventMode(
        BGAPI2_DataStream* data_stream,
        BGAPI2_EventMode event_mode);

    /**
      \ingroup    DataStream
      \brief      Returns the new buffer event mode. The event mode is controlled by the event register functions
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] event_mode   Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNewBufferEventMode(
        BGAPI2_DataStream* data_stream,
        BGAPI2_EventMode* event_mode);

    /**
      \ingroup    DataStream
      \brief      Returns the identifier of datastream
      \param[in]  data_stream      Pointer to the DataStream
      \param[in,out] ID            Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetID(
        BGAPI2_DataStream* data_stream,
        char* ID,
        bo_uint64* string_length);

    /**
      \ingroup    DataStream
      \brief      Returns the number of delivered buffer of datastream
      \param[in]  data_stream   Pointer to the DataStream
      \param[out] num_delivered Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNumDelivered(
        BGAPI2_DataStream* data_stream,
        bo_uint64* num_delivered);

    /**
      \ingroup    DataStream
      \brief      Returns the number of underrun buffer of datastream
      \param[in]  data_stream   Pointer to the DataStream
      \param[out] num_underrun  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNumUnderrun(
        BGAPI2_DataStream* data_stream,
        bo_uint64* num_underrun);

    /**
      \ingroup    DataStream
      \brief      Returns the number of announced buffer of datastream
      \param[in]  data_stream   Pointer to the DataStream
      \param[out] num_announced Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNumAnnounced(
        BGAPI2_DataStream* data_stream,
        bo_uint64* num_announced);

    /**
      \ingroup    DataStream
      \brief      Returns the number of queued buffer of datastream
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] num_queued   Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNumQueued(
        BGAPI2_DataStream* data_stream,
        bo_uint64* num_queued);

    /**
      \ingroup    DataStream
      \brief      Returns the number of wait for delivery buffer of datastream
      \param[in]  data_stream        Pointer to the DataStream
      \param[out] num_await_delivery Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNumAwaitDelivery(
        BGAPI2_DataStream* data_stream,
        bo_uint64* num_await_delivery);

    /**
      \ingroup    DataStream
      \brief      Returns the number of currently filled buffer of datastream.
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] num_started  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetNumStarted(
        BGAPI2_DataStream* data_stream,
        bo_uint64* num_started);

    /**
      \ingroup    DataStream
      \brief      Returns the size of the expected data block of this DataStream object in bytes

      Based on the current device settings and including all control data (e.g. chunk header). This function is
      mainly used for devices which supports several data streams to allow stream based memory allocation

      \param[in]  data_stream  Pointer to the DataStream
      \param[out] payload_size Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetPayloadSize(
        BGAPI2_DataStream* data_stream,
        bo_uint64* payload_size);

    /**
      \ingroup    DataStream
      \brief      Returns the flag for started datastream
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] is_grabbing  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetIsGrabbing(
        BGAPI2_DataStream* data_stream,
        bo_bool* is_grabbing);

    /**
      \ingroup    DataStream
      \brief      Returns the size of the expecting data block of this DataStream object in bytes

      Based on the current device settings and including all control data (e.g. chunk header). This function is
      mainly used for devices which supports several data streams to allow stream based memory allocation

      \param[in]  data_stream          Pointer to the DataStream
      \param[out] defines_payload_size Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Internal error, not producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetDefinesPayloadSize(
        BGAPI2_DataStream* data_stream,
        bo_bool* defines_payload_size);

    /**
      \ingroup    DataStream
      \brief      Returns the transport layer of datastream
      \param[in]  data_stream      Pointer to the DataStream
      \param[in,out] tl_type       Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetTLType(
        BGAPI2_DataStream* data_stream,
        char* tl_type,
        bo_uint64* string_length);

    /**
      \ingroup    DataStream
      \brief      Acquires a defined number of buffers from datastream
      \param[in]  data_stream   Pointer to the DataStream
      \param[in]  num_to_acquire Number of buffer to acquire
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_StartAcquisition(
        BGAPI2_DataStream* data_stream,
        bo_uint64 num_to_acquire);

    /**
      \ingroup    DataStream
      \brief      Acquires buffers from datastream
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_StartAcquisitionContinuous(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Stop acquiring buffers from datastream
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_StopAcquisition(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Stops the DataStream immediately. Active transmissions are aborted.
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Buffer_GetIsIncomplete
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_AbortAcquisition(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Moves all Buffer objects from the input buffer queue to the output buffer queue
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Buffer_QueueBuffer
      \see        BGAPI2_DataStream_GetFilledBuffer
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_FlushInputToOutputQueue(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Moves all Buffers of the Buffer list to the input buffer queue even those in the output buffer queue
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Buffer_QueueBuffer
      \see        BGAPI2_DataStream_GetFilledBuffer
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_FlushAllToInputQueue(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Moves all free (not queued) Buffers of the Buffer list to the input buffer queue
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \see        BGAPI2_Buffer_QueueBuffer
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_FlushUnqueuedToInputQueue(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Discard all Buffer objects in the output buffer queue. The discarded Buffer objects are freed
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_DiscardOutputBuffers(BGAPI2_DataStream* data_stream);

    /**
    \ingroup    DataStream
    \brief      Discard all Buffer objects in the input buffer queue and output buffer queue
    \param[in]  data_stream  Pointer to the DataStream
    \retval     BGAPI2_RESULT_SUCCESS           No error
    \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
    \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
    \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
    \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_DiscardAllBuffers(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Adds the Buffer objects to the datastream
      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  buffer       Pointer to buffer to announce
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_AnnounceBuffer(
        BGAPI2_DataStream* data_stream,
        BGAPI2_Buffer* buffer);

    /**
      \ingroup    DataStream
      \brief      Removes a Buffer object from the buffer list
      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  buffer       Pointer to buffer to revoke
      \param[out] user_obj
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_RevokeBuffer(
        BGAPI2_DataStream* data_stream,
        BGAPI2_Buffer* buffer,
        void** user_obj);

    /**
      \ingroup    DataStream
      \brief      Moves a Buffer object into the input buffer queue and make it available for the image acquisition

      If the image aquisition is done the filled Buffer object is moved into the output buffer queue and is fetched
      with function BGAPI2_DataStream_GetFilledBuffer. See also the functions of the BufferList, to move
      the Buffer object between the internal lists

      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  buffer       Pointer to buffer to queue
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_QueueBuffer(
        BGAPI2_DataStream* data_stream,
        BGAPI2_Buffer* buffer);

    /**
      \ingroup    DataStream
      \brief      Fetches a new image from the DataStream object and removes it from the output buffer queue

      If the output buffer queue is empty after the timeout, the function delivers NULL

      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  buffer       Pointer to buffer to queue
      \param[in]  timeout      Timeout to stop if no image is delivered from the camera, -1 means indefinitely
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_TIMEOUT           No buffer filled in iTimeout
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetFilledBuffer(
        BGAPI2_DataStream* data_stream,
        BGAPI2_Buffer** buffer,
        bo_uint64 timeout);

    /**
      \ingroup    DataStream
      \brief      Cancels a currently running BGAPI2_DataStream_GetFilledBuffer
      \param[in]  data_stream  Pointer to the DataStream
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_CancelGetFilledBuffer(BGAPI2_DataStream* data_stream);

    /**
      \ingroup    DataStream
      \brief      Returns buffer from datastream with index number
      \param[in]  data_stream  Pointer to the DataStream
      \param[in]  index        The ID for the buffer to be returned
      \param[out] buffer       Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_LOWLEVEL_ERROR    Internal error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetBufferID(
        BGAPI2_DataStream* data_stream,
        bo_uint index,
        BGAPI2_Buffer** buffer);

    /**
      \ingroup    DataStream
      \brief      Register a callback for datastream new buffer events
      \param[in]  data_stream          Pointer to the DataStream
      \param[in]  callback_owner       Data, context pointer for use in callback function
      \param[in]  buffer_event_handler Address of function from type BGAPI2_NewBufferEventHandler
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Error, no producer loaded
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   Error, datastream is not initialized
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \todo       What is that???
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_RegisterNewBufferEventHandler(
        BGAPI2_DataStream* data_stream,
        void* callback_owner,
        BGAPI2_NewBufferEventHandler buffer_event_handler);

    /**
      \ingroup    DataStream
      \brief      Returns the parent object (device) which belongs to
      \param[in]  data_stream  Pointer to the DataStream
      \param[out] parent       Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_DataStream_GetParent(
        BGAPI2_DataStream* data_stream,
        BGAPI2_Device** parent);

    // Image
    /**
      \ingroup    Image
      \brief      Get the image width from given image
      \param[in]  image  Pointer to the image
      \param[out] width  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetWidth(BGAPI2_Image* image, bo_uint* width);

    /**
      \ingroup    Image
      \brief      Get the image height from given image
      \param[in]  image   Pointer to the image
      \param[out] height  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetHeight(BGAPI2_Image* image, bo_uint* height);

    /**
      \ingroup    Image
      \brief      Get the image pixelformat from given image
      \param[in]  image       Pointer to the image
      \param[in,out] pixelformat   Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_BUFFER    If given destination buffer is too small for pixelformat string
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetPixelformat(
        BGAPI2_Image* image,
        char* pixelformat,
        bo_uint64* string_length);

    /**
      \ingroup    Image
      \brief      Get the image pixel buffer from given image
      \param[in]  image   Pointer to the image
      \param[out] buffer  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Image has no buffer (ppBuffer results NULL)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetBuffer(BGAPI2_Image* image, void** buffer);

    /**
      \ingroup    Image
      \brief      Get the required image buffer size for new pixelformat of given image
      \param[in]  image       Pointer to the image
      \param[in]  pixelformat New pixelformat string
      \param[out] buffer_size Required size of buffer for new pixelformat
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             If required buffer size is NULL (*buffer_size == NULL)
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetTransformBufferLength(
        BGAPI2_Image* image,
        const char* pixelformat,
        bo_uint* buffer_size);

    /**
      \ingroup    Image
      \brief      Release (delete) a given image
      \param[in]  image Pointer to the image
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_Release(BGAPI2_Image* image);

    /**
      \ingroup    Image
      \brief      Initialize a given image with parameters
      \param[in]  image       Pointer to the image
      \param[in]  width       Image width
      \param[in]  height      Image height
      \param[in]  pixelformat Image pixelformat
      \param[in]  buffer      Buffer pointer
      \param[in]  buffer_size Size of buffer (must match to pixelformat!)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             If required buffer size is NULL common init error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_Init(
        BGAPI2_Image* image,
        bo_uint width,
        bo_uint height,
        const char* pixelformat,
        void* buffer,
        bo_uint64 buffer_size);

    /**
      \ingroup    Image
      \brief      Get the named node of given map of image
      \param[in]  image Pointer to the image
      \param[in]  name  Node name
      \param[out] node  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetNode(
        BGAPI2_Image* image,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    Image
      \brief      Get the node tree of the image
      \param[in]  image     Pointer to the image
      \param[out] node_tree Node map of image
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetNodeTree(BGAPI2_Image* image, BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    Image
      \brief      Get the node list of the image
      \param[in]  image     Pointer to the image
      \param[out] node_list Node map of image
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Image_GetNodeList(BGAPI2_Image* image, BGAPI2_NodeMap** node_list);

    // ImageProcessor
    /**
      \ingroup    ImgProc
      \brief      Creates an image processor
      \param[out] img_proc Pointer on image processor
      \retval     BGAPI2_RESULT_SUCCESS           No error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_CreateImageProcessor(BGAPI2_ImageProcessor** img_proc);

    /**

      \ingroup    ImgProc
      \brief      Release an image processor
      \param[in]  img_proc   Pointer on image processor
      \retval     BGAPI2_RESULT_SUCCESS           No error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ReleaseImageProcessor(BGAPI2_ImageProcessor* img_proc);

    /**
      \ingroup    ImgProc
      \brief      Get the version string of image processor
      \param[in]  img_proc   Pointer to the image processor
      \param[in,out] version        Nullptr to get string length or pointer to store result
      \param[in,out] string_length  Result size, length of string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Could not stop event thread
      \retval     BGAPI2_RESULT_INVALID_BUFFER    Given pVersion is too small
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \todo check params in,out
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_GetVersion(
        BGAPI2_ImageProcessor* img_proc,
        char* version,
        bo_uint64* string_length);

    /**
      \ingroup    ImgProc
      \brief      Creates an empty image by image processor
      \param[in]  img_proc   Pointer to the image processor
      \param[out] image      Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Some internal errors
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Image processor parts not available, not initialized
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_CreateEmptyImage(
        BGAPI2_ImageProcessor* img_proc,
        BGAPI2_Image** image);

    /**
      \ingroup    ImgProc
      \brief      Creates an image by image processor
      \param[in]  img_proc      Pointer to the image processor
      \param[in]  width         Width of image in pixel
      \param[in]  height        Height of image in pixel
      \param[in]  pixelformat   Name of pixelformat to use in image
      \param[in]  buffer        Pointer to raw image data buffer
      \param[in]  buffer_size   (maximum) size of raw image data buffer
      \param[out] image         Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_ERROR             Some internal errors
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Image processor parts not available, not initialized
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_CreateImage(
        BGAPI2_ImageProcessor* img_proc,
        bo_uint width,
        bo_uint height,
        const char* pixelformat,
        void* buffer,
        bo_uint64 buffer_size,
        BGAPI2_Image** image);

    /**
      \ingroup    ImgProc
      \brief      Transformes an given image using the pixelformat using the image processor
      \param[in]  img_proc     Pointer to the image processor
      \param[in]  image_input  Pointer to the given image
      \param[in]  pixelformat  Name of new pixelformat to use in image
      \param[out] image_result Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_CreateTransformedImage(
        BGAPI2_ImageProcessor* img_proc,
        BGAPI2_Image* image_input,
        const char* pixelformat,
        BGAPI2_Image** image_result);

    /**
      \ingroup       ImgProc
      \brief         Creates an new image based on given image with new pixelformat by image processor
      \param[in]     img_proc     Pointer to the image processor
      \param[in]     image        Pointer to the given image
      \param[in]     pixelformat  Name of new pixelformat for the result buffer
      \param[in,out] buffer       Destination buffer for new image with new pixelformat
      \param[in]     buffer_size  Buffer size for image with new pixelformat
      \retval        BGAPI2_RESULT_SUCCESS           No error
      \retval        BGAPI2_RESULT_ERROR             Error on image transformation
      \retval        BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_TransformImageToBuffer(
        BGAPI2_ImageProcessor* img_proc,
        BGAPI2_Image* image,
        const char* pixelformat,
        void* buffer,
        bo_uint64 buffer_size);

    /**
      \ingroup    ImgProc
      \brief      Get the named node of given map of image processor
      \param[in]  img_proc Pointer to the image processor
      \param[in]  name     Node name
      \param[out] node     Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS            No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_GetNode(
        BGAPI2_ImageProcessor* img_proc,
        const char* name,
        BGAPI2_Node** node);

    /**
      \ingroup    ImgProc
      \brief      Get the node map of image processor as a tree
      \param[in]  img_proc   Pointer to the image processor
      \param[out] node_tree  Pointer to store the result
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_GetNodeTree(
        BGAPI2_ImageProcessor* img_proc,
        BGAPI2_NodeMap** node_tree);

    /**
      \ingroup    ImgProc
      \brief      Get the node map of image processor as a list
      \param[in]  img_proc   Pointer to the image processor
      \param[out] node_list  Node map of image processor
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Error for missing root node
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_ImageProcessor_GetNodeList(
        BGAPI2_ImageProcessor* img_proc,
        BGAPI2_NodeMap** node_list);


    // POLARIZER

    /**
      \ingroup    Polarizer
      \brief      Enumeration containing the string representation of the possible polarization formats.
    */
    typedef enum {
        BGAPI2_POLARIZER_AOP = 0,               ///< Enum value AOP - Angle of polarization
        BGAPI2_POLARIZER_DOLP = 1,              ///< Enum value DOLP - Degree of linear polarization
        BGAPI2_POLARIZER_ADOLP = 2,             ///< Enum value ADOLP - Angle and degree of linear polarization
        BGAPI2_POLARIZER_INTENSITY = 3,         ///< Enum value Intensity - Gray scale image
        BGAPI2_POLARIZER_POL0_DEG = 4,          ///< Enum value Pol0Deg - The polarization information 0 degrees
        BGAPI2_POLARIZER_POL45_DEG = 5,         ///< Enum value Pol45Deg - The polarization information 45 degrees
        BGAPI2_POLARIZER_POL90_DEG = 6,         ///< Enum value Pol90Deg - The polarization information 90 degrees
        BGAPI2_POLARIZER_POL135_DEG = 7,        ///< Enum value Pol135Deg - The polarization information 135 degrees
        BGAPI2_POLARIZER_REFLECTION_MIN = 8,    ///< Enum value ReflectionMin - Image with suppressed reflections
        BGAPI2_POLARIZER_REFLECTION_MAX = 9,    ///< Enum value ReflectionMax - Image with enhanced reflections
    } BGAPI2_POLARIZER_FORMATS;

    /**
      \ingroup    Polarizer
      \brief      Creates a polarizer object
      \param[out] polarizer Pointer on polarizer object
      \retval     BGAPI2_RESULT_SUCCESS           No error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_Create(BGAPI2_Polarizer** polarizer);

    /**
      \ingroup    Polarizer
      \brief      Release a polarizer object
      \param[in]  polarizer  Pointer to the polarizer object
      \retval     BGAPI2_RESULT_SUCCESS           No error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_Release(BGAPI2_Polarizer* polarizer);

    /**
      \ingroup    Polarizer
      \brief      Initialize the Polarizer and provide the buffer with the raw polarized data for calculations
      \param[in]  polarizer Instance of polarizer.
      \param[in]  buffer    A valid buffer with polarized data acquired by a Baumer camera
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \retval     BGAPI2_RESULT_ERROR             Internal error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_Initialize(
        BGAPI2_Polarizer* polarizer,
        BGAPI2_Buffer* buffer);

    /**
      \ingroup    Polarizer
      \brief      Get the calibration data and angle offset from the camera

      Reads the calibration matrix and the configured polarization angle offset from the camera device to enhance
      the calculation of different polarization formats.

      \param[in]  polarizer Instance of polarizer.
      \param[in]  device    The polarization camera (opened, must be able to read features from the camera)
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \retval     BGAPI2_RESULT_NOT_INITIALIZED   The is not open.
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_ReadCalibrationData(
        BGAPI2_Polarizer* polarizer,
        BGAPI2_Device* device);

    /**
      \ingroup    Polarizer
      \brief      Keep output image the same size as the input buffer. The default is disabled.

      If enabled, the calculated images will be interpolated to have the same size as the raw image buffer provided

      \param[in]  polarizer   Instance of polarizer
      \param[in]  interpolate  If set to true the result images will be interpolated
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_EnableInterpolation(
        BGAPI2_Polarizer* polarizer,
        bo_bool interpolate);

    /**
      \ingroup    Polarizer
      \brief      Each component to be calculated must be enabled first

      To speed up the calculation of the different components it is necessary to enable them first. This allows
      for the calculation to re-use and combine some of the necessary calculations

      \param[in]  polarizer Instance of polarizer
      \param[in]  format     The format to enable or disable
      \param[in]  enable     Set to true to enable or false to disable
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_Enable(
        BGAPI2_Polarizer* polarizer,
        BGAPI2_POLARIZER_FORMATS format,
        bo_bool enable);

    /**
      \ingroup    Polarizer
      \brief      Get the calculated component (BGAPI_POLARIZER_AOP, etc.) from the buffer

    For performance reasons when handling more than one component, a component must first be enabled via the
    BGAPI2_Polarizer_Enable() method

      \param[in]  polarizer  Instance of polarizer
      \param[in]  format     The format to enable or disable
      \param[in,out] image   BGAPI2_Image to store the result of the calculation
      \see        BGAPI2_POLARIZER_FORMATS
      \see        BGAPI2_Polarizer_Initialize()
      \see        BGAPI2_ImageProcessor_CreateImage()
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \retval     BGAPI2_RESULT_NOT_AVAILABLE     Polarizer result not available
      \retval     BGAPI2_RESULT_ERROR             Internal error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_Get(
        BGAPI2_Polarizer* polarizer,
        BGAPI2_POLARIZER_FORMATS format,
        BGAPI2_Image* image);

    /**
      \ingroup    Polarizer
      \brief      Get the string of the polarization format
      \param[in]  polarizer     Instance of polarizer.
      \param[in]  format        The polarization format
      \param[out] format_string The buffer of the polarization format
      \param[in,out] size       The buffer size (in) / The format string size (out)
      \see        BGAPI2_POLARIZER_FORMATS
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_GetFormatString(
        BGAPI2_Polarizer* polarizer,
        BGAPI2_POLARIZER_FORMATS format,
        char* format_string,
        bo_uint64* size);

    /**
      \ingroup    Polarizer
      \brief      Set the number of threads the Polarizer can use for calculations

      To speed up the calculation of components more than one thread can be used internally.
      The default is 4 threads on processors which have 8 or more logical cores, otherwise half of the logical
      cores are used. Depending on your application you can change this here

      \param[in]  polarizer Instance of polarizer
      \param[in]  number     The amount of threads used internally
      \retval     BGAPI2_RESULT_INVALID_PARAMETER Error for invalid parameters
      \retval     BGAPI2_RESULT_SUCCESS           No error
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_Polarizer_SetMaxThreads(
        BGAPI2_Polarizer* polarizer,
        bo_uint number);


    // TRACE functions
    /**
      \ingroup    Trace
      \brief      Enable the BGAPI Trace
      \param[in]  benable       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceEnable(bo_bool benable);

    /**
      \ingroup    Trace
      \brief      Activate the tracing to an outputfile
      \param[in]  bactive       True to enable trace otherwise false
      \param[in]  tracefilename The output file including the path
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateOutputToFile(bo_bool bactive, const char* tracefilename);

    /**
      \ingroup    Trace
      \brief      Activate the tracing to the debugger
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateOutputToDebugger(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief      Trace errors
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateMaskError(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief      Trace warnings
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateMaskWarning(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief      Trace infos
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateMaskInformation(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief      Trace Timestamps
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateOutputOptionTimestamp(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief      Trace Timestamp differences
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateOutputOptionTimestampDiff(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief      Trace the thread ID
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateOutputOptionThreadID(bo_bool bactive);

    /**
      \ingroup    Trace
      \brief
      \param[in]  bactive       True to enable trace otherwise false
    */
    BGAPI2_C_DECL void BGAPI2CALL BGAPI2_TraceActivateOutputOptionPrefix(bo_bool bactive);

    // OTHER STUFF
    /**
      \ingroup    BGAPI2
      \brief      Sets the GENICAM_GENTL_PATHxx environment variable with given value
      \param[in]  producer_path  New path for searching GenTL producers
      \retval     BGAPI2_RESULT_SUCCESS   No error
      \retval     BGAPI2_RESULT_ERROR     Error on setting environment variable
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_SetEnv(const char* producer_path);

    /**
      \ingroup    BGAPI2
      \brief      Get the value of the GENICAM_GENTL_PATHxx environment variable
      \param[in,out] producer_path  Nullptr to get string length or pointer to store result
      \param[in,out] string_length Result size, length of version string (including string end zero)
      \retval     BGAPI2_RESULT_SUCCESS   No error
      \retval     BGAPI2_RESULT_ERROR     Error on getting environment variable
      \see        BGAPI2_System_GetID for detail how to retrieve strings with unknown size
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_GetEnv(char* producer_path, bo_uint64* string_length);

    /**
      \ingroup    BGAPI2
      \brief      Returns a description of the last occurred error
      \param[out] error_code    Last error code
      \param[out] error_text    Last error string
      \param[out] string_length Size of last error string
      \retval     BGAPI2_RESULT_SUCCESS           No error
      \todo   Check Module
    */
    BGAPI2_C_DECL BGAPI2_RESULT BGAPI2CALL BGAPI2_GetLastError(BGAPI2_RESULT* error_code, char* error_text,
        bo_uint64* string_length);


#ifdef __cplusplus
}  // extern c
#endif

#endif  // TLCONSUMER_BGAPI2_GENICAM_BGAPI2_GENICAM_H_
