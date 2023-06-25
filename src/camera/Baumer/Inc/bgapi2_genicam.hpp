/* Copyright 2014-2018 Baumer Optronic */

#ifndef _BGAPI2_HPP__
#define _BGAPI2_HPP__

//! @cond suppress_docu
#include <ctime>
#include "./bgapi2_def.h"
#include "./bgapi2_featurenames.h"
//! @endcond


/**
\defgroup MainClasses Main Classes
\brief This group defined the main classes of BGAPI2. These classes represent the fundamental logical and physical components of the image processing system.
*/

/**
\defgroup ListClasses List Classes
\brief This classes aim to the discovery and listing of the main objects.
*/

/**
\defgroup InterfaceClasses Interface Classes
\brief This group defined fundamental functions which are used by the main classes.
*/

/**
\defgroup AdditionalClasses Additional Classes
\brief This group provided additional functions to extend the application spectrum of BGAPI2.
*/

/**
\defgroup ExceptionClasses Exception Classes
\brief This group provided the BGAPI2 exeption handling.
*/

/**
\brief The global namespace of Baumer GAPI SDK 2.
*/

namespace BGAPI2 {

class System;
class Interface;
class Device;
class DataStream;
class Buffer;

/**
\ingroup ListClasses
\brief This class is used to search and list system objects and may be instantiated only once.
*/
class BGAPI2_DECL SystemList {
 public:
    ~SystemList();

    /**
    * \fn        GetInstance
    * \brief     This function creates and delivers a static instance of SystemList. The Instance will be created with the first call of this function. Every following call delivers always the same instance until the function ReleaseInstance is called. This approach makes sure that only one instance of this class can be created.
    * \brief     If the function CreateInstanceFromPath is used before the first call to this function, this function delivers the pointer to the instance created by CreateInstanceFromPath.
    * \return    SystemList * The Pointer to the static instance of class SystemList.
    */
    static SystemList *GetInstance();

    /**
    * \fn ReleaseInstance
    * \brief     This function releases the static instance of SystemList.
    * \return    void
    */
    static void ReleaseInstance();

    /**
    * \fn        CreateInstanceFromPath
    * \brief     This function creates and delivers a specified static instance of SystemList. Only the passed path will be used while searching for GenTL producers. The searching procedure described in function SystemList::Refresh is not used when the Systemlist is created with this function.
    * \brief     The specified instance of SystemList class is only created when it is used before the first call of function SystemList::GetInstance.
    * \param     producerpath This is the searching path for GenTL producers.
    * \return    The Pointer to the static instance of class SystemList.
    * \exception Exceptions::ResourceInUseException This exception will be thrown by repeated use or if the instance was already created by function SystemList::GetInstance.
    */
    static SystemList *CreateInstanceFromPath(String producerpath);

    /**
    * \fn        Add
    * \brief     This function adds a GenTL producer to the system list. It can be used to e.g. load GenTL producer which were not found by the searching procedure of the Refresh function.
    * \param     pSystem The GenTL producer to be added.
    * \return    void
    * \exception Exceptions::InvalidParameterException The passed parameter is not a valid System object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Add(System * pSystem);

    /**
    * \fn        Refresh
    * \brief     This functions starts the search for GenTL producers with file extension 'cti' and follows a fixed searching procedure. First, the directory containing the application is searched, then the directory containing the BGAPI GenICam-Consumer and then the directories specified in the GenICam variable (see remarks). By using the function SystemList::CreateInstanceFromPath the GenICam variable will be ignored and the specified path is used instead. The found GenTL producers will be inserted into the SystemList.
    * \return    void
    * \exception Exceptions::ErrorException Internal sytem error, thrown if the GenTL producer couldn't be loaded. Check trace output for a detailed error description.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   The GenICam variable on 64bit systems is defined as 'GENICAM_GENTL64_PATH', on 32bit systems 'GENICAM_GENTL32_PATH'.
    * \remarks   Already found GenTL producers remain unaffected by the Refresh function.
    * \remarks   If a GenTL producer is found twice in different directories, it is up to the user to use the correct one.
    */
    void Refresh();

    /**
    * \fn        size
    * \brief     This function delivers the number of GenTL producers in the system list.
    * \return    bo_uint64 The number of GenTL producers in the system list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 size();

    /**
    * \fn        clear
    * \brief     This function removes all GenTL producer from the system list.
    * \return    void
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void clear();

    /**
    * \fn        operator[]()
    * \brief     This operator allows the direct access to an object of the system list.
    * \param     systemid For this ID, the associated system object is delivered.
    * \return    System* The requested system object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException No object in the system list has the passed ID.
    */
    System* operator[](const String& systemid);

    /**
    \brief This class provides an iterator that can read or modify any object of the list.
    */
    class BGAPI2_DECL iterator {
     public:
        ~iterator();
        /**
        * \fn        operator=()
        * \brief     Assignment operator. Store the value of the second operand in the object specified by the first operand.
        * \param     _iterator The second operand.
        * \return    iterator The first operand.
        */
        iterator& operator=(const iterator& _iterator);

        /**
        * \fn        operator!=()
        * \brief     This operator compares their operands for inequality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator!=(const iterator& _right) const;

        /**
        * \fn        operator==()
        * \brief     This operator compares their operands for equality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator==(const iterator& _right) const;

        /**
        * \fn        operator++()
        * \brief     Postfix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator& operator++();

        /**
        * \fn        operator++(int)
        * \brief     Prefix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator operator++(int);

        /**
        * \fn        operator->()
        * \brief     Member access operator. This operator dereferences the list pointer.
        * \return    _pairs The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairs* operator->();

        /**
        * \fn        operator*()
        * \brief     The indirection operator dereferences the list pointer.
        * \return    _pairs The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairs* operator*();

     private:
        iterator();
        iterator(void* _data);
        _pairs p;
        void* pp;
        friend class SystemList;
    };

    /**
    * \fn        begin
    * \brief     This function delivers an iterator on the top of the system list.
    * \return    iterator The iterator on the top of the system list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator begin();

    /**
    * \fn        end
    * \brief     This function delivers an iterator at the end of the system list.
    * \return    iterator The iterator at the end of the system list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator end();

    /**
    * \fn        find
    * \brief     This function delivers an iterator on an object that is supposed to be found. If the object cannot be found, this functions delivers with an end-iterator.
    * \param     _keyval The ID to the object to be found.
    * \return    iterator The iterator to the found object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator find(const String& _keyval);

 protected:
    //! @cond suppress_docu
    static SystemList *ms_pInstance;
    //! @endcond
 private:
    SystemList();
    SystemList(const SystemList & Obj);
    tSystemListData *m_pSystemListImpl;
    friend class System;
};

/**
\ingroup ListClasses
\brief This class is used to search and list interface objects.
*/
class BGAPI2_DECL InterfaceList {
 public:
    ~InterfaceList();
    /**
    * \fn        Refresh
    * \brief     This function starts the search for Interfaces and provides a list of interface objects. The search is controlled by a timeout parameter. This function delivers, if at least one interface object was found, but not later than after the set timeout period (see comments).
    * \param     iTimeout After this time, the function delivers latest.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   This function has a minimum execution time, which also works when the timeout set is smaller then the minimum execution time.
    */
    void Refresh(bo_uint64 iTimeout);

    /**
    * \fn        size
    * \brief     This function delivers the number of interface objects in the interface list.
    * \return    bo_uint64 The number of interface objects in the interface list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 size();

    /**
    * \fn        clear
    * \brief     This function removes all interface objects from the interface list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void clear();

    /**
    * \fn        operator[]()
    * \brief     This operator allows the direct access to an object of the interface list.
    * \param     ifaceid For this ID, the associated system object is delivered.
    * \return    Interface* The requested interface object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException No object in the interface list has the passed ID.
    */
    Interface* operator[](const String& ifaceid);

    /**
    \brief This class provides a iterator that can read or modify any element in the list.
    */
    class BGAPI2_DECL iterator {
     public:
         ~iterator();
        /**
        * \fn        operator=()
        * \brief     Assignment operator. Store the value of the second operand in the object specified by the first operand.
        * \param     _iterator The second operand.
        * \return    iterator The first operand.
        */
        iterator& operator=(const iterator& _iterator);

        /**
        * \fn        operator!=()
        * \brief     This operator compares their operands for inequality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator!=(const iterator& _right) const;

        /**
        * \fn        operator==()
        * \brief     This operator compares their operands for equality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator==(const iterator& _right) const;

        /**
        * \fn        operator++()
        * \brief     Postfix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator& operator++();

        /**
        * \fn        operator++(int)
        * \brief     Prefix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator operator++(int);

        /**
        * \fn        operator->()
        * \brief     Member access operator. This operator dereferences the list pointer.
        * \return    _pairi The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairi* operator->();

        /**
        * \fn        operator*()
        * \brief     The indirection operator dereferences the list pointer.
        * \return    _pairi The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairi* operator*();

     private:
        iterator();
        iterator(void* _data);
        _pairi p;
        void* pp;
        friend class InterfaceList;
    };

    /**
    * \fn        begin
    * \brief     This functions delivers an iterator on the top of the interface list.
    * \return    iterator The iterator on the top of the interface list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator begin();

    /**
    * \fn        end
    * \brief     This functions delivers an iterator at the end of the interface list.
    * \return    iterator The iterator at the end of the interface list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator end();

    /**
    * \fn        find
    * \brief     This function delivers an iterator on an object to be found. If the object cannot be found, this functions delivers an end-iterator.
    * \param     _keyval The ID to the object to be found.
    * \return    iterator The iterator to the found object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator find(const String& _keyval);

 private:
    InterfaceList();
    InterfaceList(const InterfaceList & Obj);

    void ClearBgapiObjects(System* const pParent);
    bo_bool UpdateBgapiObjects(System* const pParent, bo_uint64 iTimeout);
    bo_uint64 NumberOfBgapiObjects(System* const pParent);
    String GetId(System* const pParent, const bo_uint64 index);
    Interface* createBgapi2Object(System* const pParent, String id);
    tInterfaceListData *m_pInterfaceListImpl;
    friend class System;
};

/**
\ingroup ListClasses
\brief This class is used to discover and list device objects.
*/
class BGAPI2_DECL DeviceList {
 public:
    ~DeviceList();

    /**
    * \fn        Refresh
    * \brief     This function starts the search for Devices and provides a list of device objects. The search is controlled by a timeout parameter. This function delivers, if at least one device object were found, but no later than after the set timeout period (see comments).
    * \param     iTimeout After this time, the function delivers latest.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   This function has a minimum execution time, which also works when the timeout set is smaller then the minimum execution time.
    */
    void Refresh(bo_uint64 iTimeout);

    /**
    * \fn        size
    * \brief     This function delivers the number of device objects in the device list.
    * \return    bo_uint64 The number of device objects in the device list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 size();

    /**
    * \fn        clear
    * \brief     This function removes all device objects from the device list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void clear();

    /**
    * \fn        operator[]()
    * \brief     This operator allows the direct access to an object of the device list.
    * \param     devid For this ID, the associated device object is delivered.
    * \return    Device* The requested device object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException No object in the device list has the passed ID.
    */
    Device* operator[](const String& devid);

    /**
    \brief This class provides a iterator that can read or modify any element in the list.
    */
    class BGAPI2_DECL iterator {
     public:
         ~iterator();
        /**
        * \fn        operator=()
        * \brief     Assignment operator. Store the value of the second operand in the object specified by the first operand.
        * \param     _iterator The second operand.
        * \return    iterator The first operand.
        */
        iterator& operator=(const iterator& _iterator);

        /**
        * \fn        operator!=()
        * \brief     This operator compares their operands for inequality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator!=(const iterator& _right) const;

        /**
        * \fn        operator==()
        * \brief     This operator compares their operands for equality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator==(const iterator& _right) const;

        /**
        * \fn        operator++()
        * \brief     Postfix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator& operator++();

        /**
        * \fn        operator++(int)
        * \brief     Prefix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator operator++(int);

        /**
        * \fn        operator->()
        * \brief     Member access operator. This operator dereferences the list pointer.
        * \return    _paird The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _paird* operator->();

        /**
        * \fn        operator*()
        * \brief     The indirection operator dereferences the list pointer.
        * \return    _paird The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _paird* operator*();

     private:
        iterator();
        iterator(void* _data);
        _paird p;
        void* pp;
        friend class DeviceList;
    };

    /**
    * \fn        begin
    * \brief     This function delivers an iterator on the top of the device list.
    * \return    iterator The iterator on the top of the device list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator begin();

    /**
    * \fn        end
    * \brief     This function delivers an iterator at the end of the device list.
    * \return    iterator The iterator at the end of the device list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator end();

    /**
    * \fn        find
    * \brief     This function delivers an iterator on an object to be found. The object is not found, this functions delivers an end-iterator.
    * \param     _keyval The ID to the object to be found.
    * \return    iterator The iterator to the found object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator find(const String& _keyval);

 private:
    DeviceList();
    DeviceList(const DeviceList & Obj);

    static bo_bool UpdateBgapiObjects(Interface* const pParent, const bo_uint64 iTimeout);
    static bo_uint64 NumberOfBgapiObjects(Interface* const pParent);
    static String GetId(Interface* const pParent, const bo_uint64 index);
    static Device* createBgapi2Object(Interface* const pParent, String id);
    tDeviceListData *m_pDeviceListImpl;
    friend class Interface;
};

/**
\ingroup ListClasses
\brief This class is used to discover and list data stream objects.
*/
class BGAPI2_DECL DataStreamList {
 public:
    ~DataStreamList();

    /**
    * \fn        Refresh
    * \brief     This function provides a list of DataStream objects.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Refresh();

    /**
    * \fn        size
    * \brief     This function delivers the number of DataStream objects in the DataStream list.
    * \return    bo_uint64 The number of DataStream objects in the DataStream list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 size();

    /**
    * \fn        clear
    * \brief     This function removes all DataStream objects from the DataStream list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void clear();

    /**
    * \fn        operator[]()
    * \brief     This operator allows the direct access to an object of the DataStream list.
    * \param     streamid For this ID, the associated DataStream object is delivered.
    * \return    DataStream* The requested DataStream object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException No object in the DataStream list has the passed ID.
    */
    DataStream* operator[](const String& streamid);

    /**
    \brief This class provides an iterator that can read or modify any element in the list.
    */
    class BGAPI2_DECL iterator {
     public:
         ~iterator();
        /**
        * \fn        operator=()
        * \brief     Assignment operator. Store the value of the second operand in the object specified by the first operand.
        * \param     _iterator The second operand.
        * \return    iterator The first operand.
        */
        iterator& operator=(const iterator& _iterator);

        /**
        * \fn        operator!=()
        * \brief     This operator compare their operands for inequality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator!=(const iterator& _right) const;

        /**
        * \fn        operator==()
        * \brief     This operator compares their operands for equality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator==(const iterator& _right) const;

        /**
        * \fn        operator++()
        * \brief     Postfix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator& operator++();

        /**
        * \fn        operator++(int)
        * \brief     Prefix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator operator++(int);

        /**
        * \fn        operator->()
        * \brief     Member access operator. This operator dereferences the list pointer.
        * \return    _pairds The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairds* operator->();

        /**
        * \fn        operator*()
        * \brief     The indirection operator dereferences the list pointer.
        * \return    _pairds The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairds* operator*();

     private:
        iterator();
        iterator(void* _data);
        _pairds p;
        void* pp;
        friend class DataStreamList;
    };

    /**
    * \fn        begin
    * \brief     This function delivers an iterator on the top of the DataStream list.
    * \return    iterator The iterator on the top of the DataStream list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator begin();

    /**
    * \fn        end
    * \brief     This function delivers an iterator at the end of the DataStream list.
    * \return    iterator The iterator at the end of the DataStream list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator end();

    /**
    * \fn        find
    * \brief     This function delivers an iterator on an object to be found. If the object is not found, this functions delivers an end-iterator.
    * \param     _keyval The ID to the object to be found.
    * \return    iterator The iterator to the found object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator find(const String& _keyval);

 private:
    DataStreamList();
    DataStreamList(const DataStreamList & Obj);

    bo_bool UpdateBgapiObjects();
    bo_uint64 NumberOfBgapiObjects(Device* const pParent);
    String GetId(Device* const pParent, const bo_uint64 index);
    DataStream* createBgapi2Object(Device* const pParent, String id);
    tDataStreamListData *m_pDataStreamListImpl;
    friend class Device;
};

/**
\ingroup ListClasses
\brief This class is used for discovery and listing of buffer objects.
*/
class BGAPI2_DECL BufferList {
 public:
    ~BufferList();

    /**
    * \fn        Add
    * \brief     This function adds a Buffer object to the buffer list. If the Buffer object is created without external memory, this function allocates the necessary memory.
    * \brief     See the Buffer constructors Buffer::Buffer(), Buffer::Buffer(void * pUserObj) and Buffer::Buffer(void *pUserBuffer, bo_uint64 uUserBufferSize, void *pUserObj).
    * \param     pBuffer The Buffer object to be added.
    * \exception Exceptions::InvalidParameterException The passed parameter is not a valid Buffer object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Add(Buffer *pBuffer);

    /**
    * \fn        RevokeBuffer
    * \brief     This function removes a Buffer object from the buffer list.
    * \param     pBuffer The Buffer object to be removed.
    * \return    void* This function delivers the user pointer of the Buffer object. See constructor of Buffer class.
    * \exception Exceptions::InvalidParameterException The passed parameter is not a valid Buffer object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void* RevokeBuffer(Buffer *pBuffer);

    /**
    * \fn        clear
    * \brief     This function removes all Buffer objects from the Buffer list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void clear();

    /**
    * \fn        size
    * \brief     This function delivers the number of Buffer objects in the Buffer list.
    * \return    bo_uint64 The number of Buffer objects in the Buffer list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 size();

    /**
    * \fn        operator[]()
    * \brief     This operator allows the direct access to an object of the Buffer list.
    * \param     bufid For this ID, the associated Buffer object is delivered.
    * \return    Buffer* The requested Buffer object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException No object in the Buffer list has the passed ID.
    */
    Buffer* operator[](const String& bufid);

    /**
    * \fn        FlushInputToOutputQueue
    * \brief     This function moves all Buffer objects from the input buffer queue to the output buffer queue. See function Buffer::QueueBuffer and DataStreamEventControl::GetFilledBuffer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void FlushInputToOutputQueue();

    /**
    * \fn        FlushAllToInputQueue
    * \brief     This function moves all Buffer objects of the Buffer list to the input buffer queue even those in the output buffer queue. See function Buffer::QueueBuffer and DataStreamEventControl::GetFilledBuffer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void FlushAllToInputQueue();

    /**
    * \fn        FlushUnqueuedToInputQueue
    * \brief     This function moves all free (not queued) Buffer objects of the Buffer list to the input buffer queue. See function Buffer::QueueBuffer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void FlushUnqueuedToInputQueue();

    /**
    * \fn        DiscardOutputBuffers
    * \brief     This function discards all Buffer objects in the output buffer queue. The discarded Buffer objects are free.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void DiscardOutputBuffers();

    /**
    * \fn        DiscardAllBuffers
    * \brief     This function discards all Buffer objects in the input buffer queue and output buffer queue. The discarded Buffer objects are free.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void DiscardAllBuffers();

    /**
    * \fn        GetDeliveredCount
    * \brief     This function delivers the number of Buffer objects that have been delivered since the start of the DataStream object.
    * \return     bo_uint64 The number of Buffer object that have been delivered since the start of the DataStream object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetDeliveredCount();

    /**
    * \fn        GetUnderrunCount
    * \brief     This function delivers the number of lost frames due to queue underrun since the DataStream object was started. This counter is incremented every time the data could not be acquired because there was no Buffer object in the input buffer queue.
    * \return    bo_uint64 The number of lost frames.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetUnderrunCount();

    /**
    * \fn        GetAnnouncedCount
    * \brief     This function delivers the number of Buffer objects in the Buffer list. Same as BufferList::size.
    * \return    bo_uint64 The number of Buffer objects in the Buffer list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetAnnouncedCount();

    /**
    * \fn        GetQueuedCount
    * \brief     This function delivers the number of Buffer objects in the input buffer queue. See function Buffer::QueueBuffer.
    * \return    bo_uint64 The number of Buffer objects in the input buffer queue.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetQueuedCount();

    /**
    * \fn        GetAwaitDeliveryCount
    * \brief     This function delivers the number of Buffer objects in the output buffer queue. See function DataStreamEventControl::GetFilledBuffer.
    * \return    bo_uint64 The number of Buffer objects in the output buffer queue.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetAwaitDeliveryCount();

    /**
    * \fn        GetStartedCount
    * \brief     This function delivers the number of Buffer objects which are currently being filled.
    * \return    bo_uint64 The number of Buffer objects which are currently being filled.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetStartedCount();

    /**
    \brief This class provides a iterator that can read or modify any element in the list.
    */
    class BGAPI2_DECL iterator {
     public:
         ~iterator();
        /**
        * \fn        operator=()
        * \brief     Assignment operator. Store the value of the second operand in the object specified by the first operand.
        * \param     _iterator The second operand.
        * \return    iterator The first operand.
        */
        iterator& operator=(const iterator& _iterator);

        /**
        * \fn        operator!=()
        * \brief     This operator compares their operands for inequality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator!=(const iterator& _right) const;

        /**
        * \fn        operator==()
        * \brief     This operator compares their operands for equality.
        * \param     _right The second operand.
        * \return    bo_bool The result of comparison.
        */
        bo_bool operator==(const iterator& _right) const;

        /**
        * \fn        operator++()
        * \brief     Postfix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator& operator++();

        /**
        * \fn        operator++(int)
        * \brief     Prefix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator operator++(int);

        /**
        * \fn        operator->()
        * \brief     Member access operator. This operator dereferences the list pointer.
        * \return    _pairb The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairb* operator->();

        /**
        * \fn        operator*()
        * \brief     The indirection operator dereferences the list pointer.
        * \return    _pairb The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairb* operator*();

     private:
        iterator();
        iterator(void* _data);
        _pairb p;
        void* pp;
        friend class BufferList;
    };

    /**
    * \fn        begin
    * \brief     This function delivers an iterator on the top of the Buffer list.
    * \return    iterator The iterator on the top of the Buffer list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator begin();

    /**
    * \fn        end
    * \brief     This function delivers an iterator at the end of the Buffer list.
    * \return    iterator The iterator at the end of the Buffer list.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator end();

    /**
    * \fn        find
    * \brief     This function delivers an iterator on an object to be found. The object is not found, this functions delivers an end-iterator.
    * \param     _keyval The ID to the object to be found.
    * \return    iterator The iterator to the found object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    iterator find(const String& _keyval);

    /**
    * \fn        GetReserved
    * \brief     Undocumented function.
    */
    void* GetReserved();

 private:
    BufferList();
    BufferList(const BufferList & Obj);
    tBufferListData *m_pBufferListImpl;
    friend class DataStream;
};

/**
\ingroup AdditionalClasses
\brief The class Trace offers the possibility to monitor the program flow and detect errors. This class belongs to the additional classes.
\brief This class consists exclusively of static functions.
*/
class BGAPI2_DECL Trace {
 public:
    /**
    * \fn        Enable
    * \brief     This function switches the trace on.
    * \param     benable This flag switches the trace on.
    */
    static void Enable(bo_bool benable);

    /**
    * \fn        ActivateOutputToFile
    * \brief     This function activates the writing of the trace messages into the specified file.
    * \param     bactive This flag activates the writing of the trace messages into a file.
    * \param     tracefilename The name of the file to which the trace messages is written.
    */
    static void ActivateOutputToFile(bo_bool bactive, String tracefilename);

    /**
    * \fn        ActivateOutputToDebugger
    * \brief     This function activates the writing of the trace messages into the debugger output.
    * \param     bactive This flag activates the writing of the trace messages into the debugger output.
    */
    static void ActivateOutputToDebugger(bo_bool bactive);

    /**
    * \fn        ActivateMaskError
    * \brief     This function activates the writing of errors.
    * \param     bactive This flag activates the writing of errors.
    */
    static void ActivateMaskError(bo_bool bactive);

    /**
    * \fn        ActivateMaskWarning
    * \brief     This function activates the writing of warnings.
    * \param     bactive This flag activates the writing of warnings.
    */
    static void ActivateMaskWarning(bo_bool bactive);

    /**
    * \fn        ActivateMaskInformation
    * \brief     This function activates the writing of information.
    * \param     bactive This flag activates the writing of information.
    */
    static void ActivateMaskInformation(bo_bool bactive);

    /**
    * \fn        ActivateOutputOptionTimestamp
    * \brief     This function inserts a timestamp into the trace message.
    * \param     bactive This flag activates the inserting of the timestamp.
    */
    static void ActivateOutputOptionTimestamp(bo_bool bactive);

    /**
    * \fn        ActivateOutputOptionTimestampDiff
    * \brief     This function inserts a timestamp difference to the last message into the current message.
    * \param     bactive This flag activates the inserting of the timestamp difference.
    */
    static void ActivateOutputOptionTimestampDiff(bo_bool bactive);

    /**
    * \fn        ActivateOutputOptionThreadId
    * \brief     This function inserts the thread ident to the last message into the current message.
    * \param     bactive This flag activates the inserting of the threadid.
    */
    static void ActivateOutputOptionThreadId(bo_bool bactive);

    /**
    * \fn        ActivateOutputOptionPrefix
    * \brief     This function inserts a short prefix into the current message which specifies the source of the message, e.g. bgapi2_genicam.dll or a Baumer GenTL producer.
    * \param     bactive This flag activates the inserting of a short prefix.
    */
    static void ActivateOutputOptionPrefix(bo_bool bactive);

 private:
    Trace();
    Trace(const Trace & Obj);
    ~Trace();
};

class NodeMap;
/**
\ingroup InterfaceClasses
\brief The class Node represent one feature from the provided node list based on the underlying XML definition.
*/
class BGAPI2_DECL Node {
 public:
    /**
    * \fn        GetInterface
    * \brief     This function delivers the interface type of the Node object. Depending on the interface type the needed access functions are available. The available interface types are defined in header file bgapi2_def.h. See definitions BGAPI2_NODEINTERFACE_xxx.
    * \return    String The interface type of the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetInterface();

    /**
    * \fn        GetToolTip
    * \brief     This function delivers a short description text suitable for a ToolTip representation.
    * \return    String The short description text of the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetToolTip();

    /**
    * \fn        GetDescription
    * \brief     This function delivers a description text of the Node object.
    * \return    String The description text of the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetDescription();

    /**
    * \fn        GetName
    * \brief     This function delivers the name of the Node object.
    * \return    String The name of the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetName();

    /**
    * \fn        GetDisplayName
    * \brief     This function delivers a name of the node object suitable for displaying.
    * \return    String The display name of the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetDisplayName();

    /**
    * \fn        GetVisibility
    * \brief     This function delivers a value representing the visibility of the Node object.
    * \return    String The visibility of the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetVisibility();

    /**
    * \fn        GetImposedAccessMode
    * \brief     This function is deprecated. Please use IsReadable or IsWriteable instead.
    * \brief     This function delivers a value that specifies the general access to the Node object. This value is not affected by other Node objects. See also the function Node::GetCurrentAccessMode().
    * \brief     The available access modes are defined in header file bgapi2_def.h. See definitions BGAPI2_NODEACCESS_xxx.
    * \return    String The general access to the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetImposedAccessMode();

    /**
    * \fn        GetCurrentAccessMode
    * \brief     This function delivers a value that represents the current access to the Node object.
    * \brief     This value is affected by the change of dependent Node objects. In such a case, the current access mode is different and limited (i.e. less accessible) for general access mode. See also function Node::GetImposedAccessMode().
    * \brief     The available access modes are defined in header file bgapi2_def.h. See definitions BGAPI2_NODEACCESS_xxx.
    * \return    String The current access to the Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetCurrentAccessMode();

    /**
    * \fn        IsReadable
    * \brief     This function indicates if a Node object is readable.
    * \return    true when the Node object is readable, otherwise false.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsReadable();

    /**
    * \fn        IsWriteable
    * \brief     This function indicates if a Node object is writeable.
    * \return    true when the Node object is writable, otherwise false.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsWriteable();

    /**
    * \fn        GetAlias
    * \brief     This function delivers the name of a Node object specified as the alias. An alias describes the same feature in a different manner.
    * \return    String The name of the alias Node object.
    * \exception Exceptions::NotAvailableException No alias specified.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetAlias();

    /**
    * \fn        GetRepresentation
    * \brief     This function delivers a value, which recommends the representation of the Node object on a GUI. Only available for the interface types 'IFloat' and 'IInteger'.
    * \brief     The available display options are defined in the header file bgapi2_def.h. See the definitions BGAPI2_NODEREPRESENTATION_xxx.
    * \return    String The recommended value for the representation of the Node object on a GUI.
    * \exception Exceptions::NotAvailableException This exception is thrown when the interface type is not 'IFloat' and not 'IInteger'.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetRepresentation();

    /**
    * \fn        GetUnit
    * \brief     This function delivers the physical unit of the Node object. Only available for the interface types 'IFloat' and 'IInteger'.
    * \return    String The physical unit of the Node object.
    * \exception Exceptions::NotAvailableException This exception is thrown when the interface type is not 'IFloat' and not 'IInteger'.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetUnit();

    /**
    * \fn        HasUnit
    * \brief     This function delivers a flag that indicates whether the Node object has a physical unit. Only available for the interface types 'IFloat' and 'IInteger'.
    * \return    bo_bool The flag that indicates whether the Node object has a physical unit.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool HasUnit();

    /**
    * \fn        GetImplemented
    * \brief     This function delivers a flag that indicates whether the Node object is implemented.
    * \return    bo_bool The flag that indicates whether the Node object is implemented.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool GetImplemented();

    /**
    * \fn        GetAvailable
    * \brief     This function delivers a flag that indicates whether the Node object is available.
    * \brief     This status is equivalent to the access mode 'NA'. See functions Node::GetImposedAccessMode and Node::GetCurrentAccessMode.
    * \return    bo_bool The flag that indicates whether the Node object is available.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool GetAvailable();

    /**
    * \fn        GetLocked
    * \brief     This function is deprecated. Please use isReadable or isWriteable instead.
    * \brief     This function delivers a flag that indicates whether the Node object is locked for writing.
    * \brief     This status is equivalent to the access mode 'RO'. See functions Node::GetImposedAccessMode and Node::GetCurrentAccessMode.
    * \return    bo_bool The flag that indicates whether the Node object is locked for writing.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool GetLocked();

    /**
    * \fn        GetEventID
    * \brief     This function delivers an ID of a asynchronous event to which the Node object is linked.
    * \return    bo_int64 The ID of a asynchronous event.
    * \exception Exceptions::NotAvailableException The EventID is not specified in the XML description of this Node object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_int64 GetEventID();

    /**
    * \fn        GetExtension
    * \brief     This function delivers user specific data from the XML definition of this Node object.
    * \return    String vendor specific data.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetExtension();

    /**
    * \fn        GetEnumNodeList
    * \brief     This function delivers a NodeMap of available enumeration entries.
    * \brief     Only valid for interface type 'IEnumeration'.
    * \return    NodeMap* The list of all available enumeration entries of the Node object.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetEnumNodeList();

    /**
    * \fn        GetValue
    * \brief     This function delivers the value of the Node object in string format.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration', IString', 'IBoolean' and 'ICommand'.
    * \return    String The value of the Node object in string format.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetValue();

    /**
    * \fn        SetValue
    * \brief     This function writes a value in string format to the Node object.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration', IString', 'IBoolean' and 'ICommand'.
    * \param     Value The value to be written in string format.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void SetValue(String Value);

    /**
    * \fn        GetInt
    * \brief     This function delivers the current value of the Node object as Integer.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration' and 'IBoolean'.
    * \return    bo_int64 The current value as integer.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode, e.g. "WO".
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_int64 GetInt();

    /**
    * \fn        SetInt
    * \brief     This function writes a integer value to the Node object.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration' and 'IBoolean'.
    * \param     value The integer value to be written.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void SetInt(bo_int64 value);

    /**
    * \fn        GetIntMin
    * \brief     This function delivers the minimal allowed value of the Node object as Integer.
    * \brief     Valid for the interface types 'IInteger' and 'IFloat'.
    * \return    bo_int64 The minimal allowed value as integer.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_int64 GetIntMin();

    /**
    * \fn        GetIntMax
    * \brief     This function delivers the maximal allowed value of the Node object as Integer.
    * \brief     Valid for the interface types 'IInteger' and 'IFloat'.
    * \return    bo_int64 The maximal allowed value as integer.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_int64 GetIntMax();

    /**
    * \fn        GetIntInc
    * \brief     This function delivers the allowed step size for the value of the Node object as Integer.
    * \brief     Valid for the interface types 'IInteger' and 'IFloat'.
    * \return    bo_int64 The allowed step size for the value of the Node object as integer.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_int64 GetIntInc();

    /**
    * \fn        GetDouble
    * \brief     This function delivers the current value of the Node object as floating-point number.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration' and 'IBoolean'.
    * \return    bo_double The current value as floating-point number.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode, e.g. "WO".
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_double GetDouble();

    /**
    * \fn        SetDouble
    * \brief     This function writes a floating-point value to the Node object.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration' and 'IBoolean'.
    * \param     value The floating-point value to be written.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void SetDouble(bo_double value);

    /**
    * \fn        GetDoubleMin
    * \brief     This function delivers the minimal allowed value of the Node object as floating-point number.
    * \brief     Valid for the interface types 'IFloat' and 'IInteger'.
    * \return    bo_double The minimal allowed value as floating-point number.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_double GetDoubleMin();

    /**
    * \fn        GetDoubleMax
    * \brief     This function delivers the maximal allowed value of the Node object as floating-point number.
    * \brief    Valid for the interface types 'IFloat' and 'IInteger'.
    * \return    bo_double The maximal allowed value as floating-point number.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_double GetDoubleMax();

    /**
    * \fn        HasInc
    * \brief     This function delivers a flag that indicates whether the Node object has an increment. Only available for the interface types 'IFloat' and 'IInteger'.
    * \return    bo_bool The flag that indicates whether the Node object has an increment.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool HasInc();

    /**
    * \fn        GetDoubleInc
    * \brief     This function delivers the allowed step size for the value of the Node object as Float.
    * \brief     Valid for the interface types 'IFloat' and 'IInteger'.
    * \return    bo_double The allowed step size for the value of the Node object as Float.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::InvalidParameterException The Node object has no (valid) increment value.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_double GetDoubleInc();

    /**
    * \fn        GetDoublePrecision
    * \brief     This function delivers the precision for the corresponding double.
    * \brief     Only valid for the interface type 'IFloat'.
    * \return    bo_uint32 non-negative number for the precision the double is displayed
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 GetDoublePrecision();

    /**
    * \fn        GetMaxStringLength
    * \brief     This function delivers the length of the string.
    * \brief     Only valid for the interface type 'IString'.
    * \return    bo_int64 The length of the string.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_int64 GetMaxStringLength();

    /**
    * \fn        GetString
    * \brief     This function is deprecated. Please use GetValue instead.
    * \brief     This function delivers the current value of the Node object as String.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration', IString', 'IBoolean' and 'ICommand'.
    * \return    String The current value of the Node object as String.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode, e.g. "WO".
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetString();

    /**
    * \fn        SetString
    * \brief     This function is deprecated. Please use SetValue instead.
    * \brief     This function writes a string value to the Node object.
    * \brief     Valid for the interface types 'IFloat', 'IInteger', 'IEnumeration', IString', 'IBoolean' and 'ICommand'.
    * \param     value The string value to be written.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void SetString(String value);

    /**
    * \fn        Execute
    * \brief     This function executes the command of the Node object.
    * \brief     Only valid for the interface type 'ICommand'.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void Execute();

    /**
    * \fn        IsDone
    * \brief     This function delivers a flag that indicates whether the command of the Node object has been executed.
    * \brief     Only valid for the interface type 'ICommand'.
    * \return    bo_bool The flag that indicates whether the command of the Node object has been executed.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsDone();

    /**
    * \fn        GetBool
    * \brief     This function delivers the current value of the Node object as boolean value.
    * \brief     Only valid for the interface type 'IBoolean'.
    * \return    bo_bool The current value of the Node object as boolean value.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode, e.g. "WO".
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool GetBool();

    /**
    * \fn        SetBool
    * \brief     This function writes a boolean value to the Node object.
    * \brief     Only valid for the interface type 'IBoolean'.
    * \param     value The boolean value to be written.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void SetBool(bo_bool value);

    /**
    * \fn        GetNodeTree
    * \brief     This function provides the subordinate features of the Node object as tree structure in form of a NodeMap pointer. For a detailed description of the tree structure representation refer to the class description of NodeMap.
    * \brief     Only valid for the interface type 'ICategory'.
    * \return    NodeMap* The tree structure of all features of this Node object.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetNodeTree();

    /**
    * \fn        GetNodeList
    * \brief     This function provides the subordinate features of the Node object as unstructured list in form of a NodeMap pointer. For a detailed description of the unstructured list representation refer to the class description of NodeMap.
    * \brief     Only valid for the interface type 'ICategory'.
    * \return    NodeMap* The list of all features of this Node object.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetNodeList();

    /**
    * \fn        IsSelector
    * \brief     This function delivers a flag that indicates whether the Node object is a selector.
    * \brief     A selector is a posibility to define feature dependencies. The current value of a selector node has an impact on the value of another Node object.
    * \brief     Valid for the interface types 'IInteger' and 'IEnumeration'.
    * \return    bo_bool The flag that indicates whether the Node object is a selector.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsSelector();

    /**
    * \fn        GetSelectedNodeList
    * \brief     This function delivers a list of features that depend on this selector Node.
    * \brief     Valid for the interface types 'IInteger' and 'IEnumeration'.
    * \return    NodeMap* The list of all features that depend on this selector Node.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetSelectedNodeList();

    /**
    * \fn        getLength
    * \brief     This function delivers the length in bytes of the memory pointed to by the Node object.
    * \brief     Only valid for the interface type 'IRegister'.
    * \return    bo_uint64 The length in bytes of the memory pointed to by the Node object.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 getLength();

    /**
    * \fn        getAddress
    * \brief     This function delivers the address of the memory pointed to by the Node object.
    * \brief     Only valid for the interface type 'IRegister'.
    * \return    bo_uint64 The address of the memory pointed to by the Node object.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 getAddress();

    /**
    * \fn        get
    * \brief     This function reads the memory pointed to by the Node object and writes it into the provided buffer.
    * \brief     Only valid for the interface type 'IRegister'.
    * \param     pBuffer The destination buffer into which the read data is copied.
    * \param     len The size of the destination buffer. The function Node::getLength delivers the necessary size.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. Possible reasons for this exception are the loss of connection to the device and the wrong access mode, e.g. "WO".
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void get(void *pBuffer, bo_uint64 len);

    /**
    * \fn        set
    * \brief     This function writes the memory pointed to by the Node object.
    * \brief     Only valid for the interface type 'IRegister'.
    * \param     pBuffer The buffer for the data to be written.
    * \param     len The size of the buffer to the data to be written. The function Node::getLength delivers the necessary size.
    * \exception Exceptions::NotAvailableException The Node object has the wrong interface type.
    * \exception Exceptions::AccessDeniedException Error when accessing this function. A possible reason for this exception is the loss of connection to the device.
    * \exception Exceptions::NotImplementedException This feature is not implemented.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void set(void *pBuffer, bo_uint64 len);

 private:
    Node();
    Node(const Node& Obj);
};

/**
\ingroup InterfaceClasses
\brief The class NodeMap represents a collection of Node objects based on the underlying XML definition file. This class supports two representation forms, a unstructured list and a tree structure.
\brief The tree structure is a hierarchical representation. Special kinds of nodes can have subordinate nodes. These nodes are from interface type 'ICategory' (see function Node::GetInterface). The access to its subordinate nodes occurs by calling NodeMap::GetNodeTree.
\brief The unstructured list representation is linear. Use the NodeMap::iterator class and the NodeMap::begin and NodeMap::end functions for iterating through the list. All nodes from type 'ICategory' are removed and all its subordinate nodes are moved into the single list.
\brief The following functions use the unstructured list representation: INode::GetNodeList, Device::GetRemoteNodeList, Node::GetNodeList, Node::GetEnumNodeList, Node::GetSelectedNodeList
\brief The following functions use the tree structure representation: INode::GetNodeTree, Device::GetRemoteNodeTree, Node::GetNodeTree
*/
class BGAPI2_DECL NodeMap {
 public:
    ~NodeMap();
    /**
    * \fn        size
    * \brief     This function delivers the number of Node objects in the Node list.
    * \return    bo_uint64 The number of Node objects in the Node list.
    */
    bo_uint64 size();

    /**
    * \fn        GetNodePresent
    * \brief     This function delivers a flag that indicates whether the NodeMap contains a Node with the specified name.
    * \param     name The Node name to search for.
    * \return    bo_bool The flag which indicates whether the NodeMap contains a Node with the specified name.
    */
    bo_bool GetNodePresent(String name);

    /**
    * \fn        operator[]()
    * \brief     This operator allows the direct access to an object of the Node list.
    * \param     val For this name, the associated Node object is delivered.
    * \return    Node* The requested Node object.
    * \exception Exceptions::InvalidParameterException No object in the Node list has the passed name.
    */
    Node* operator[](const String& val);

    /**
    * \fn        GetNode
    * \brief     This function delivers a certain object of the Node list.
    * \param     name For this name, the associated Node object is delivered.
    * \return    Node* The requested Node object.
    * \exception Exceptions::InvalidParameterException No object in the Node list has the passed name.
    */
    Node* GetNode(String name);

    /**
    * \fn        GetNodeCount
    * \brief     This function delivers the number of Node objects in the Node list.
    * \return    bo_uint64 The number of Node objects in the Node list.
    */
    bo_uint64 GetNodeCount();

    /**
    * \fn        GetNodeByIndex
    * \brief     This function delivers a pointer to a specified Node object.
    * \param     iIndex The index of the Node object.
    * \return    Node* The requested Node object.
    * \exception Exceptions::InvalidParameterException The passed index is invalid.
    */
    Node* GetNodeByIndex(bo_uint64 iIndex);

    /**
    \brief This class provides a iterator that can read or modify any element in the list.
    */
    class BGAPI2_DECL iterator {
     public:
         ~iterator();
        /**
        * \fn        operator=()
        * \brief     Assignment operator. Store the value of the second operand in the object specified by the first operand.
        * \param     _iterator The second operand.
        * \return    iterator The first operand.
        */
        iterator& operator=(const iterator& _iterator);

        /**
        * \fn        operator!=()
        * \brief     This operator compare their operands for inequality.
        * \param     _right The second operand.
        * \return    bool The result of comparison.
        */
        bool operator!=(const iterator& _right) const;

        /**
        * \fn        operator==()
        * \brief     This operator compares their operands for equality.
        * \param     _right The second operand.
        * \return    bool The result of comparison.
        */
        bool operator==(const iterator& _right) const;

        /**
        * \fn        operator++()
        * \brief     Postfix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator& operator++();

        /**
        * \fn        operator++(int)
        * \brief     Prefix increment operator.
        * \return    iterator The iterator of the next list element.
        */
        iterator operator++(int);

        /**
        * \fn        operator->()
        * \brief     Member access operator. This operator dereferences the list pointer.
        * \return    _pairnm The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairnm* operator->();

        /**
        * \fn        operator*()
        * \brief     The indirection operator dereferences the list pointer.
        * \return    _pairnm The result is a pair structure with a 'first' and a 'second' value. The 'first' value is the string ID of the object. The 'second' value is the pointer to the BGAPI2 object.
        */
        _pairnm operator*();

     private:
        iterator();
        iterator(void* _data);
        _pairnm p;
        void* pp;
        friend class NodeMap;
    };
    /**
    * \fn        begin
    * \brief     This function delivers an iterator on the top of the Node list.
    * \return    iterator The iterator on the top of the Node list.
    */
    iterator begin();

    /**
    * \fn        end
    * \brief     This function delivers an iterator at the end of the Node list.
    * \return    iterator The iterator at the end of the Node list.
    */
    iterator end();

    /**
    * \fn        find
    * \brief     This function delivers an iterator on an object to be found. The object is not found, this functions delivers an end-iterator.
    * \param     _keyval The ID to the object to be found.
    * \return    iterator The iterator to the found object.
    */
    iterator find(const String& _keyval);

 private:
    NodeMap();
    NodeMap(const NodeMap & Obj);
    friend class Node;
};

/**
\ingroup InterfaceClasses
\brief The class INode act as base for of the main classes and provided the access to the node objects (features).
*/
class BGAPI2_DECL INode {
 public:
    /**
    * \fn        GetNode
    * \brief     This function delivers a certain object of the Node list and provides the access to a node objcet without the detour across a NodeMap object. This function is functional identical to NodeMap::GetNode.
    * \param     name For this name, the associated Node object is delivered.
    * \return    Node* The requested Node object.
    * \exception Exceptions::InvalidParameterException No object in the Node list has the passed name.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    Node* GetNode(String name);

    /**
    * \fn        GetNodeTree
    * \brief     This function provides the features of a object derived from INode interface as tree structure in form of a NodeMap pointer. For a detailed description of the tree structure representation refer to the class description of NodeMap.
    * \return    NodeMap* The tree structure of all features of a object derived from INode interface.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetNodeTree();

    /**
    * \fn        GetNodeList
    * \brief     This function provides the features of a object derived from INode interface as unstructured list in form of a NodeMap pointer. For a detailed description of the unstructured list representation refer to the class description of NodeMap.
    * \return    NodeMap* The list of all features of a object derived from INode interface.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetNodeList();

 protected:
    INode();
    virtual ~INode();
    //! @cond suppress_docu
    void* m_pNodeBase;
    //! @endcond
};

/**
\brief The namespace Events consists of classes which belongs to the event interface.
*/
namespace Events {

/**
\brief Enumeration, which defines kinds of event modes.
*/
enum EventMode {
    /**
    \brief Event handling is disabled. No events can be retrieved.
    */
    EVENTMODE_UNREGISTERED = 0,
    /**
    \brief This EventMode allows the retrieval of occurred events by a function call of the corresponding get function.
    */
    EVENTMODE_POLLING = 1,
    /**
    \brief This EventMode allows the retrieval of occurred events by a previously registered callback function.
    */
    EVENTMODE_EVENT_HANDLER = 2
};

/**
\brief  Enumeration, which defines kinds of PnP events.
*/
enum PnPType {
    /**
    \brief Represents a device removed PnP event.
    */
    PNPTYPE_DEVICEREMOVED = 0,
    /**
    \brief Represents a device add PnP event.
    */
    PNPTYPE_DEVICEADDED = 1
};

/**
\ingroup InterfaceClasses
\brief The class EventControl provided access to custom events as well as the event mode.
*/
class BGAPI2_DECL EventControl {
 public:
    /**
    * \fn        GetEventMode
    * \brief     This function delivers the current event mode setting. The event mode is controlled by the event register functions. See also enumeration Events::EventMode.
    * \return    EventMode The current event mode.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    EventMode GetEventMode();

    /**
    * \fn         GetBase
    * \brief    Undocumented function.
    * \return    void *
    */
    void * GetBase();

 protected:
    EventControl();
    virtual ~EventControl();
    //! @cond suppress_docu
    void* m_pBase;
    //! @endcond
};

/**
\ingroup InterfaceClasses
\brief The class PnPEvent represented a plug'n play event and provides access to the event information.
*/
class BGAPI2_DECL PnPEvent {
 public:
    /**
    * \fn        PnPEvent
    * \brief     Constructor for creating an PnPEvent object.
    */
    PnPEvent();

    /**
    * \fn        ~PnPEvent
    * \brief     Destructor for deleting an PnPEvent object.
    */
    ~PnPEvent();

    /**
    * \fn        GetSerialNumber
    * \brief     This function delivers the serial number of the Device object which was added/removed from the Interface object.
    * \return    String The serial number of the Device object which was add/removed from the Interface object.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NotAvailableException The serial number was not included in the event.
    */
    String GetSerialNumber();

    /**
    * \fn        GetPnPType
    * \brief     This function delivers the kind of the PnPEvent. The two several kinds of PnPEvent are defined in enumeration Events::PnPType.
    * \return    PnPType The kind of the PnPEvent.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    PnPType GetPnPType();

    /**
    * \fn        GetId
    * \brief     This function delivers the event ID of the PnPEvent.
    * \return    String The ID of the PnPevent in string format.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NotAvailableException The ID of the PnPEvent was not included.
    */
    String GetId();

 private:
    CEventPnPObj *m_pCEventPnPObjImpl;
    friend class InterfaceEventControl;
    //            friend class CEventPnPObj;
};

/**
\brief  Function pointer for pnp event notification, which points to a user defined handler.
*/
typedef void (BGAPI2CALL *PnPEventHandler)(void * callBackOwner, PnPEvent * pBuffer);

/**
\ingroup InterfaceClasses
\brief The class InterfaceEventControl provides access to interface specific events, e.g. plug'n play event.
*/
class BGAPI2_DECL InterfaceEventControl : public EventControl {
 public:
    /**
    * \fn        RegisterPnPEvent
    * \brief     This function registers a PnPEvent at an Interface object to detect adding and removing of Device objects.
    * \param     eventMode The event mode specifies the way how to fetch an incoming event.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException Only the event mode polling is available. See enumeration EventMode.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void RegisterPnPEvent(EventMode eventMode);

    /**
    * \fn        UnregisterPnPEvent
    * \brief     This function unregisters a PnPEvent at an Interface object.
    * \brief     If RegisterPnPEventHandler was registered, Thread will be shutdown and Handler removed.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void UnregisterPnPEvent();

    /**
    * \fn        GetPnPEvent
    * \brief     This function fetches a PnPEvent from the Interface object.
    * \param     pPnPEvent A pointer to a valid object of type PnPEvent. See function Events::PnPEvent::PnpEvent().
    * \param     iTimeout After this time, the function delivers latest.
    * \return    bo_bool A flag that indicates whether a PnPEvent was fetched.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException The passed PnPEvent object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetPnPEvent(PnPEvent * pPnPEvent, bo_uint64 iTimeout);

    /**
    * \fn        CancelGetPnPEvent
    * \brief     This function cancels the current waiting operation on the GetPnPEvent function.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ErrorException Unexpected Error. An internal pointer is not valid.
    */
    void CancelGetPnPEvent();

    /**
    * \fn        RegisterPnPEventHandler
    * \brief     Register a callback function which is called any time a new plug 'n play event was received.
    * \param     callBackOwner Any object, stays at it is.
    * \param     pFunc Callback event handler.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ErrorException No memory available.
    */
    void RegisterPnPEventHandler(void * callBackOwner, PnPEventHandler pFunc);

 protected:
    InterfaceEventControl();
    ~InterfaceEventControl();
};

/**
\ingroup InterfaceClasses
\brief This class represents an device event which was received from the host. Use this class to get event information.
*/
class BGAPI2_DECL DeviceEvent : public INode {
 public:
    /**
    * \fn        DeviceEvent
    * \brief     Constructor for creating an DeviceEvent object.
    */
    DeviceEvent();

    /**
    * \fn        ~DeviceEvent
    * \brief     Destructor for deleting an DeviceEvent object.
    */
    ~DeviceEvent();

    /**
    * \fn        GetName
    * \brief     This function delivers the name of the DeviceEvent, which was specified in the XML description of the event.
    * \return    String The name of the device event.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NotAvailableException The name was not included in the event.
    */
    String GetName();

    /**
    * \fn        GetDisplayName
    * \brief     This function delivers the display name of the DeviceEvent, which was specified in the XML description of the event.
    * \return    String The display name of the device event.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NotAvailableException The display name was not included in the XML description of the event.
    */
    String GetDisplayName();

    /**
    * \fn        GetTimeStamp
    * \brief     This function delivers the timestamp of the DeviceEvent.
    * \return    bo_uint64 The timestamp of the device event.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NotAvailableException The timestamp was not included in the event.
    */
    bo_uint64 GetTimeStamp();

    /**
    * \fn        GetId
    * \brief     This function delivers the ID of the DeviceEvent, which was specified in the XML description of the event.
    * \return    String The ID of the device event in String format.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NoDataException The event includes no data.
    */
    String GetId();

 private:
    CEventDeviceEventObj *m_pCEventDeviceEventObjImpl;
    friend class DeviceEventControl;
};

/**
\brief  Function pointer for device event notification, which points to a user defined handler.
*/
typedef void (BGAPI2CALL *DeviceEventHandler)(void * callBackOwner, DeviceEvent * pDeviceEvent);

/**
\ingroup InterfaceClasses
\brief The class DeviceEventControl provides access to standard events transmitted from the device.
*/
class BGAPI2_DECL DeviceEventControl : public EventControl {
 public:
    /**
    * \fn        RegisterDeviceEvent
    * \brief     This function registers a DeviceEvent at an Device object to detect asynchronous events from the physical connected device.
    * \param     eventMode The event mode specifies the way how to fetch an incoming event.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException Only the event mode polling is available. See enumeration EventMode.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void RegisterDeviceEvent(EventMode eventMode);

    /**
    * \fn        UnregisterDeviceEvent
    * \brief     This function unregisters a DeviceEvent at an Device object.
    * \brief     If DeviceEventHandler was registered, Thread will be shutdown and Handler removed.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void UnregisterDeviceEvent();

    /**
    * \fn        GetDeviceEvent
    * \brief     This function fetches a DeviceEvent from the Device object.
    * \param     pDeviceEvent A pointer to a valid object of type DeviceEvent. See function Events::DeviceEvent::DeviceEvent().
    * \param     iTimeout After this time, the function delivers latest.
    * \return    bo_bool A flag that indicates whether a DeviceEvent was fetched.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::InvalidParameterException The passed DeviceEvent object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetDeviceEvent(DeviceEvent * pDeviceEvent, bo_uint64 iTimeout);

    /**
    * \fn        CancelGetDeviceEvent
    * \brief     This function cancels the current waiting operation on the GetDeviceEvent function.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::ErrorException Unexpected Error. An internal pointer is not valid.
    */
    void CancelGetDeviceEvent();

    /**
    * \fn       RegisterDeviceEventHandler
    * \brief    Register a callback function which is called any time a new device event was received.
    * \param    callBackOwner Any object, stays at it is.
    * \param    pFunc Callback event handler.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ErrorException No memory available.
    */
    void RegisterDeviceEventHandler(void * callBackOwner, DeviceEventHandler pFunc);

 protected:
    DeviceEventControl();
    ~DeviceEventControl();
};

/**
\brief  Function pointer for buffer notification, which points to a user defined handler.
*/
typedef void (BGAPI2CALL *NewBufferEventHandler)(void * callBackOwner, Buffer * pBuffer);

/**
\ingroup InterfaceClasses
\brief The class DataStreamEventControl provides the new buffer event which is used for fetching images.
*/
class BGAPI2_DECL DataStreamEventControl : public EventControl {
 public:
    /**
    * \fn        RegisterNewBufferEvent
    * \brief     This function registers manually a NewBufferEvent at a DataStream object to detect the receiving of new images.
    * \brief     This function is currently not implemented.
    * \brief     The NewBuffer event is already registered internally, when opening the DataStream object.
    * \param     eventMode The event mode specifies the way how to fetch an incoming event.
    * \exception Exceptions::NotImplementedException This function is currently not implemented.
    * \exception Exceptions::NotInitializedException The object is not initialized properly.
    */
    void RegisterNewBufferEvent(EventMode eventMode);

    /**
    * \fn        UnregisterNewBufferEvent
    * \brief     This function unregisters manually a NewBufferEvent at a DataStream object.
    * \brief     The NewBuffer event is already unregistered internally, when closing the DataStream object.
    * \brief     This function is currently not implemented.
    * \brief     If NewBufferEventHandler was registered, Thread will be shutdown, handler removed and all buffers discarded.
    * \exception Exceptions::NotImplementedException This function is currently not implemented.
    * \exception Exceptions::NotInitializedException The object is not initialized properly.
    */
    void UnregisterNewBufferEvent();

    /**
    * \fn        GetFilledBuffer
    * \brief     This function fetches a new image from the DataStream object and removes it from the output buffer queue.
    * \brief     If the output buffer queue is empty after the timeout, the function delivers NULL.
    * \param     iTimeout After this time, the function delivers latest.
    * \return    Buffer* The buffer object which includes the new image or NULL in case of timeout.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \exception Exceptions::AbortException The waiting operation of this function was canceled. See function CancelGetFilledBuffer.
    */
    Buffer* GetFilledBuffer(bo_uint64 iTimeout);

    /**
    * \fn        CancelGetFilledBuffer
    * \brief     This function cancels the current waiting operation on the GetFilledBuffer function and not the image acquisition in the camera.
    * \brief     No image acquisition is aborted and an the data transfer will be finished. It is usefull to reduce stop/close operations for process and task.
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void CancelGetFilledBuffer();

    /**
    * \fn         RegisterNewBufferEventHandler
    * \brief    Register a callback function which is called any time a new buffer was received.
    * \param    callBackOwner Any object, stays at it is.
    * \param    pFunc Callback event handler.
    * \return    void
    * \exception Exceptions::NotInitializedException The BGAPI object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void RegisterNewBufferEventHandler(void * callBackOwner, NewBufferEventHandler pFunc);

 protected:
    DataStreamEventControl();
    ~DataStreamEventControl();
};

}  // namespace Events

/**
\ingroup MainClasses
\brief This class realizes the data access to the memory. It contains information about the received data (e.g. image size, pixel format). This class belongs to the BGAPI2 main classes.
*/
class BGAPI2_DECL Buffer : public INode, public Events::EventControl {
 public:
    /**
    * \fn        Buffer
    * \brief     Parameterless constructor for creating of a Buffer-Object. When using this constructor, the buffer object takes care of memory management. The necessary memory for this Buffer-Object will only be allocated when it is added to a Buffer list.
    */
    Buffer();

    /**
    * \fn        Buffer( void * pUserObj )
    * \brief     Constructor for creating of a Buffer-Object. When using this constructor, the buffer object takes care of memory management. The necessary memory for this Buffer-Object will only be allocated when added to a Buffer list.
    * \param     pUserObj A pointer to a user allocated memory. This pointer is stored in the Buffer-Object and can be queried at any time. See function Buffer::GetUserObj.
    */
    Buffer(void * pUserObj);

    /**
    * \fn        Buffer( void *pUserBuffer, bo_uint64 uUserBufferSize, void *pUserObj)
    * \brief     Constructor for creating of a Buffer-Object. When using this constructor, the user takes care of allocation of the necessary memory.
    * \brief     To use the actual necessary memory size the functions Device::GetPayloadSize and DataStream::GetPayloadSize are used respectively.
    * \brief     To use the maximum required memory size of a device the maximum of the 'PayloadSize' feature is queried. See Device::GetRemoteNode and Node::GetIntMax.
    * \param     pUserBuffer A pointer to a user allocated data buffer.
    * \param     uUserBufferSize The size of the user allocated data buffer.
    * \param     pUserObj A pointer to a user allocated memory. This pointer is stored in the Buffer-Object and can be queried at any time. See function Buffer::GetUserObj.
    */
    Buffer(void *pUserBuffer, bo_uint64 uUserBufferSize, void *pUserObj);

    /**
    * \fn        ~Buffer
    * \brief     Destructor to destroy an Buffer object.
    */
    ~Buffer();

    /**
    * \fn        GetID
    * \brief     This function delivers the unique string identifier of the Buffer object, which is used in the BufferList.
    * \return    String The unique string identifier.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetID();

    /**
    * \fn        QueueBuffer
    * \brief     This function moves a Buffer object of the BufferList into the input buffer queue and make it available for the image acquisition.
    * \brief     If the image aquisition is done the filled Buffer object is moved into the output buffer queue and is fetched with function DataStreamEventControl::GetFilledBuffer. See also the functions of the BufferList, to move the Buffer object between the internal lists.
    * \brief     The Buffer object has to previously added with BufferList::Add to a BufferList, otherwise the function raises an exception.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void QueueBuffer();

    /**
    * \fn        GetMemPtr
    * \brief     This function delivers a pointer to the memory of the Buffer object.
    * \return    void* The pointer to the memory of Buffer object.
    * \exception Exceptions::NotAvailableException The Buffer object is currently no memory allocated, because it was not added to a BufferList. This exception is thrown only when using the constructors Buffer() and Buffer(void * pUserObj).
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information. This exception is thrown only when using the constructors Buffer() and Buffer(void * pUserObj).
    */
    void* GetMemPtr();

    /**
    * \fn        GetMemSize
    * \brief     This function delivers the size of the allocated memory.
    * \return    bo_uint64 The size of the allocated memory.
    * \exception Exceptions::NotAvailableException The Buffer object is currently no memory allocated, because it was not added to a BufferList. This exception is thrown only when using the constructors Buffer() and Buffer(void * pUserObj).
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information. This exception is thrown only when using the constructors Buffer() and Buffer(void * pUserObj).
    */
    bo_uint64 GetMemSize();

    /**
    * \fn        GetUserObj
    * \brief     This function delivers a pointer to a user allocated memory. See the construcors Buffer(void * pUserObj) and Buffer(void *pUserBuffer, bo_uint64 uUserBufferSize, void *pUserObj).
    * \return    void* The pointer to a user allocated memory.
    * \exception Exceptions::NotAvailableException The Buffer object was created without user pointer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    void* GetUserObj();

    /**
    * \fn        GetTimestamp
    * \brief     This function delivers the timestamp of the buffer obtained by the camera. The basic requirement for this function is that the Buffer object needs valid image data. See also the exception description of Exceptions::LowLevelException.
    * \return    bo_uint64 The timestamp of the image.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Possible reasons of this exception are: the Buffer object is still empty,
                    the Buffer object is moved into the input buffer queue with Buffer::QueueBuffer,
                    the Buffer object is filled, but is not fetched with DataStreamEventControl::GetFilledBuffer,
                    the payload type of the received data is not supported by the GenTL producer. See function Buffer::GetPayloadType.
                    Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetTimestamp();

    /**
    * \fn        GetHostTimestamp
    * \brief     This function returns the host time stamp of the first received data packet of a new image obtained by a steady clock. The basic requirement for this function is that the Buffer object needs valid image data. See also the exception description of Exceptions::LowLevelException.
    * \return    bo_uint64 The timestamp of the image in ns.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_uint64 GetHostTimestamp();

    /**
    * \fn        GetNewData
    * \brief     This function delivers a flag that indicates whether the object buffer contains new data.
    * \brief     The flag is set when the Buffer object is moved into the output buffer queue.
    * \brief     The flag is reset when the Buffer object moved into the input buffer queue. See functions Buffer::QueueBuffer, BufferList::FlushAllToInputQueue and BufferList::FlushUnqueuedToInputQueue.
    * \return    bo_bool The flag that indicates whether the object buffer contains new data.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetNewData();

    /**
    * \fn        GetIsQueued
    * \brief     This function delivers a flag that indicates whether the Buffer object has been queued.
    * \brief     The flag is set when the Buffer object is moved into the input buffer queue. See functions Buffer::QueueBuffer, BufferList::FlushAllToInputQueue and BufferList::FlushUnqueuedToInputQueue.
    * \brief     The flag is reset when the Buffer object is fetched by DataStreamEventControl::GetFilledBuffer function.
    * \return    bo_bool The flag that indicates whether the Buffer object has been queued.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetIsQueued();

    /**
    * \fn        GetIsAcquiring
    * \brief     This function delivers a flag that indicates whether the Buffer object is being filled.
    * \brief     The flag is set when data is written in the memory of the Buffer object.
    * \brief     The flag is reset when the Buffer object is filled.
    * \return    bo_bool The flag that indicates whether the Buffer object is being filled.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. A possible reason for this exception is the use of this function with the Baumer Filter Driver.
                    Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetIsAcquiring();

    /**
    * \fn        GetIsIncomplete
    * \brief     This function delivers a flag that indicates whether the expected data has been completely transferred.
    * \brief     The flag is set when the expected data was not completely transferred.
    * \brief     The flag is reset when all expected data was transferred.
    * \return    bo_bool The flag that indicates whether the expected data has been completely transferred.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetIsIncomplete();

    /**
    * \fn        GetTLType
    * \brief     This function delivers the transport layer type with which the data were transmitted.
    * \return    String The transport layer type with which the data were transmitted.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetTLType();

    /**
    * \fn        GetSizeFilled
    * \brief     This function delivers the number of transferred bytes.
    * \return    bo_uint64 The number of transferred bytes.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetSizeFilled();

    /**
    * \fn        GetWidth
    * \brief     This function delivers the width of the image in pixel.
    * \return    bo_uint64 The width of the image in pixel.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, or when using the payload type chunk the chunk feature 'ChunkWidth' is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetWidth();

    /**
    * \fn        GetHeight
    * \brief     This function delivers the height of the image in pixel.
    * \return    bo_uint64 The height of the image in pixel.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, or when using the payload type chunk the chunk feature 'ChunkHeight' is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetHeight();

    /**
    * \fn        GetXOffset
    * \brief     This function delivers the offset in X direction in pixel.
    * \return    bo_uint64 The offset in X direction in pixel.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, or when using the payload type chunk the chunk feature 'ChunkOffsetX' is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetXOffset();

    /**
    * \fn        GetYOffset
    * \brief     This function delivers the offset in Y direction in pixel.
    * \return    bo_uint64 The offset in Y direction in pixel.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, or when using the payload type chunk the chunk feature 'ChunkOffsetY' is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetYOffset();

    /**
    * \fn        GetXPadding
    * \brief     This function delivers the number of extra bytes transmitted at the end of each line and it is only specified for the payload types Image and Extended Chunk Data. See function Buffer::GetPayloadType.
    * \return    bo_uint64 The number of extra bytes transmitted at the end of each line.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, when using the wrong the payload type.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, and when using the payload type chunk the chunk feature 'ChunkOffsetX' is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetXPadding();

    /**
    * \fn        GetYPadding
    * \brief     This function delivers the number of extra bytes transmitted at the end of the image and it is only specified for the payload types Image and Extended Chunk Data. See function Buffer::GetPayloadType.
    * \return    bo_uint64 The number of extra bytes transmitted at the end of the image.
    * \exception Exceptions::NotAvailableException Possible reasons for this exceptions are the Buffer object is not added to a BufferList, when using the wrong the payload type.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetYPadding();

    /**
    * \fn        GetFrameID
    * \brief     This function delivers a sequentially incremented number of the image. The number refers e.g. for GigE Vision and for USB3 Vision to the block ID of the stream.
    * \return    bo_uint64 The sequentially incremented number of the image.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetFrameID();

    /**
    * \fn        GetImagePresent
    * \brief     This function delivers true, if the Buffer object includes image data.
    * \exception Exceptions::NotAvailableException This function is currently not suported.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetImagePresent();

    /**
    * \fn        GetImageOffset
    * \brief     This function returns the offset into the memory of the Buffer object to the begin of the image data.
    * \exception Exceptions::NotAvailableException The Buffer object doesn't include image data. The possible reason for this exception is, the image chunk on the device is not enabled.
    */
    bo_uint64 GetImageOffset();

    /**
    * \fn        GetPayloadType
    * \brief     This function delivers the payload type of the Buffer object. See the payload type definitions in bgapi2_def.h (BGAPI2_PAYLOADTYPE_xxx).
    * \return    String The payload type of the Buffer object.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. A possible reason for this exception is a incomplete transferred image. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetPayloadType();

    /**
    * \fn        GetPixelFormat
    * \brief     This function delivers the pixel format of the Buffer object.
    * \return    String The pixelformat of the Buffer object.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList, or when using the payload type chunk the chunk feature 'ChunkPixelFormat' is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. A possible reason for this exception is a incomplete transferred image. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetPixelFormat();

    /**
    * \fn        GetDeliveredImageHeight
    * \brief     This function delivers the transmitted lines of an image. Only valid for the payload types 'Image' and 'ImageExt'.
    * \return    bo_uint64 The transmitted lines of an image.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::LowLevelException GenTL Producer error. Possible reasons for this exception are a incomplete transferred image and a wrong payload type. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   For matrix cameras this usually is the image height. This function finds application for variable size linescan images.
    */
    bo_uint64 GetDeliveredImageHeight();

    /**
    * \fn        GetDeliveredChunkPayloadSize
    * \brief     This function delivers the number of transmitted bytes. Only valid for the chunk payload types 'ChunkData' and 'ImageExt'.
    * \return    bo_uint64 The number of transmitted bytes.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Possible reasons for this exception are a incomplete transferred image and a wrong payload type. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetDeliveredChunkPayloadSize();

    /**
    * \fn        GetContainsChunk
    * \brief     This function delivers true, if the Buffer object includes chunk data.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Possible reasons for this exception are a incomplete transferred image and a wrong payload type. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetContainsChunk();

    /**
    * \fn        GetChunkLayoutID
    * \brief     This function delivers a value representing the current structure of the chunk data. This value changes when changing the structure of the chunk data.
    * \brief     The change in this value initiates a new internal parsing of the chunk data. Only valid for chunk payload types 'ChunkData' and 'ImageExt'.
    * \return    bo_uint64 ID of the chunk data layout delivered in the buffer
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Possible reasons for this exception are a incomplete transferred image and a wrong payload type. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetChunkLayoutID();

    /**
    * \fn        GetChunkNodeList
    * \brief     This function delivers a NodeMap of available chunk information of the Buffer object. Only valid for chunk payload types 'ChunkData' and 'ImageExt'.
    * \return    NodeMap*  The list of all available chunk information of the Buffer object.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList. Wrong payload type used.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetChunkNodeList();

    /**
    * \fn        GetFileName
    * \brief     This function delivers the file name of the Buffer object. Only valid for payload types 'File'.
    * \return    String The file name of the Buffer object.
    * \exception Exceptions::NotAvailableException The Buffer object is not added to a BufferList.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException Baumer GenTL producer does not implement this feature. GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetFileName();

    /**
    * \fn        GetParent
    * \brief     This function delivers the superordinate DataStream object but only if the Buffer object was added to a BufferList.
    * \return    DataStream* A pointer to the superordinate DataStream object or NULL if the Buffer object is not added to a BufferList.
    */
    DataStream* GetParent();

    /**
    * \fn        GetReserved
    * \brief     Undocumented function.
    */
    void * GetReserved();

 private:
    tBufferData *m_pBufferImpl;
    friend class BufferList;
    friend class DataStream;
};

/**
\ingroup MainClasses
\brief This class represents a physical data stream from the device and it is responsible for the buffer handling. This class belongs to the BGAPI2 main classes.
*/
class BGAPI2_DECL DataStream : public INode, public Events::DataStreamEventControl {
 public:
    /**
    * \fn        Open
    * \brief     This function opens the DataStream object and makes their functions available. An exception are the info-functions, which are already available before opening the DataStream object.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the DataStream object is already open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Open();

    /**
    * \fn        Close
    * \brief     This function closes the DataStream object and releases the used resources.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Close();

    /**
    * \fn        IsOpen
    * \brief     This function delivers true, if the data stream is opened.
    * \return    delivers true, if the data stream is open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsOpen();

    /**
    * \fn        GetBufferList
    * \brief     This function delivers the buffer list of the DataStream object.
    * \return    BufferList* The pointer to the data buffer list of the DataStream object.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    BufferList* GetBufferList();

    /**
    * \fn        GetID
    * \brief     This function delivers the unique string identifier of the DataStream object, which is used in the DataStream list.
    * \return    String The unique string identifier.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetID();

    /**
    * \fn        GetTLType
    * \brief     This function delivers the transport layer type of the DataStream object.
    * \return    String The transport layer type of DataStream object.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetTLType();

    /**
    * \fn        GetDefinesPayloadSize
    * \brief     This function delivers true, if the DataStream object supports the function DataStream::GetPayloadSize.
    * \return    bo_bool If the delivered value is true, the DataStream object supports the function DataStream::GetPayloadSize.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetDefinesPayloadSize();

    /**
    * \fn        GetPayloadSize
    * \brief     This function delivers the size of the expecting data block of this DataStream object in bytes based on the current device settings and including all control data (e.g. chunk header).
    * \brief     This function is mainly used for devices which supports several data streams to allow stream based memory allocation.
    * \return    bo_uint64 The size of the expected data block in bytes.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetPayloadSize();

    /**
    * \fn        GetIsGrabbing
    * \brief     This function delivers true, if the DataStream object is started. See functions DataStream::StartAcquisition and DataStream:StartAcquisitionContinuous.
    * \return    bo_bool If the delivered value is true, the DataStream object is started.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_bool GetIsGrabbing();

    /**
    * \fn        StartAcquisition
    * \brief     This function starts the DataStream object. It is ready to receive data blocks. After receiving the specified number of data blocks the DataStrem object is automatically stopped.
    * \brief     A data block includes all data of the used payload type, e.g. for 'ChunkData' a data block includes all chunk blocks, for 'Image' a data block includes only image data.
    * \param     iNumToAcquire The number of expected data blocks.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void StartAcquisition(bo_uint64 iNumToAcquire);

    /**
    * \fn        StartAcquisitionContinuous
    * \brief     This function starts the DataStream object. It is ready to receive data blocks. The DataStream object is started for an undefined number of transmissions until it is stopped by one of the two functions DataStream::StopAcquisition or DataStream::AbortAcquisition.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void StartAcquisitionContinuous();

    /**
    * \fn        StopAcquisition
    * \brief     This function stops the DataStream object directly if no transmission is active or after finishing a active transmission. It is not longer possible to receive data blocks.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void StopAcquisition();

    /**
    * \fn        AbortAcquisition
    * \brief     This function stops the DataStream object immediately. Active transmissions are aborted. The aborted buffer gets the status 'incomplete'. See function Buffer::GetIsIncomplete.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void AbortAcquisition();

    /**
    * \fn        GetBufferByIndex
    * \brief     This function is deprecated. Please use instead the BufferList class.
    * \brief     This function delivers a pointer to a specified Buffer object.
    * \param     iIndex The index of the Buffer object.
    * \return    Buffer* The requested pointer to the Buffer object.
    * \exception Exceptions::NotInitializedException The DataStream object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    Buffer* GetBufferByIndex(bo_uint iIndex);

    /**
    * \fn        GetParent
    * \brief     This function delivers the superordinate Device object.
    * \return    Device* A pointer to the superordinate Device object.
    */
    Device* GetParent();

    /**
    * \fn        GetReserved
    * \brief     Undocumented function.
    */
    void * GetReserved();

 private:
    DataStream();
    DataStream(const DataStream &Obj);
    DataStream(String id, void * pReserved);
    ~DataStream();
    BufferList mBufferList;
    tDataStreamData *m_pDataStreamImpl;
    friend class DataStreamList;
    friend class BufferList;
    friend class Buffer;
};

/**
\ingroup MainClasses
\brief The class Device is used to retrieve information (e.g. model, manufacturer, access modes) of the device (camera) and also to control the device. This class belongs to the BGAPI2 main classes.
*/
class BGAPI2_DECL Device : public INode, public Events::DeviceEventControl {
 public:
    /**
    * \fn        Open
    * \brief     This function opens the device object and makes their functions available. Exceptions are the info-functions, which are already available before opening the device.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the device object is already opened.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   GigEVision (GEV) devices supports several access methods for opening. The several methods are read access, read and write access and exclusive read and write access. If a device is opened for reading another application can get read and write access. If a device is opened for reading and writing one or more other applications only can get read access. If a device is opened for exclusive reading and writing the access from other applications is not possible.
    * \remarks   Devices of other transport layer types (e.g. USB3Vision) supports only the exclusive read and write access, that means the open function opens the device for reading and writing. The access from other applications is not possible.
    */
    void Open();

    /**
    * \fn        OpenExclusive
    * \brief     This function opens the device object exclusive and makes their functions available. Exceptions are the info-functions, which are already available before opening the device.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the device object is already opened.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   GigEVision (GEV) devices supports several access methods for opening. The several methods are read access, read and write access and exclusive read and write access. If a device is opened for reading another application can get read and write access. If a device is opened for reading and writing one or more other applications only can get read access. If a device is opened for exclusive reading and writing the access from other applications is not possible.
    * \remarks   Devices of other transport layer types (e.g. USB3Vision) supports only the exclusive read and write access, that means the open function opens the device for reading and writing. The access from other applications is not possible.
    */
    void OpenExclusive();

    /**
    * \fn        OpenReadOnly
    * \brief     This function opens the device object only for reading and make their functions available. Exceptions are the info-functions, which are already available before opening the device.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the device object is already opened in exclusive mode.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   GigEVision (GEV) devices supports several access methods for opening. The several methods are read access, read and write access and exclusive read and write access. If a device is opened for reading another application can get read and write access. If a device is opened for reading and writing one or more other applications only can get read access. If a device is opened for exclusive reading and writing the access from other applications is not possible.
    * \remarks   Devices of other transport layer types (e.g. USB3Vision) supports only the exclusive read and write access, that means the open function opens the device for reading and writing. The access from other applications is not possible.
    */
    void OpenReadOnly();

    /**
    * \fn        Close
    * \brief     This function closes the device object and releases the used resources.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Close();

    /**
    * \fn        IsOpen
    * \brief     This function delivers true, if the device is opened.
    * \return    bo_bool delivers true, if the device is open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsOpen();

    /**
    * \fn        StartStacking
    * \brief     This function starts the stacked mode for write commands (control commands). See remarks.
    * \param     bReplaceMode Enable or disable the replace mode. This mode will control whether all accesses to a register will be transferred to the device or only the last one.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   The stacked mode is a high-performance method for transmitting write commands (control commands) to the device. The control commands were packed into one packet and transmitted as a concatenated command. The stacked mode is started by calling the function Device::StartStacking. All following control commands were collected and will not be transmitted until the calling of Device::WriteStack. If the replace mode is activated, only the last write acess will be transmitzted. After calling Device::WriteStack the stacked mode is stopped.
    */
    void StartStacking(bo_bool bReplaceMode);

    /**
    * \fn        WriteStack
    * \brief     This function writes the collected values to the device and stops the stacked mode.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NoDataException The stacked mode is not started or no control commands were written after calling the function Device::StartStacking.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void WriteStack();

    /**
    * \fn        CancelStack
    * \brief     This function drops the collected values and stops the stacked mode.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NoDataException The stacked mode is not started or no control commands were written after calling the function Device::StartStacking.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void CancelStack();

    /**
    * \fn        GetDataStreams
    * \brief     This function delivers the data stream list of the device.
    * \return    DataStreamList* The pointer to the data stream list of the device.
    * \exception Exceptions::NotInitializedException The interface object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    DataStreamList* GetDataStreams();

    /**
    * \fn        GetID
    * \brief     This function delivers the unique string identifier of the Device, which is used in the DeviceList.
    * \return    String The unique string identifier.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetID();

    /**
    * \fn        GetVendor
    * \brief     This function delivers the name of the device vendor.
    * \return    String The name of the device vendor.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetVendor();

    /**
    * \fn        GetModel
    * \brief     This function delivers the name of the device.
    * \return    String The name of the device.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetModel();

    /**
    * \fn        GetSerialNumber
    * \brief     This function delivers the serial number of the device.
    * \return    String The serial number of the device.
    * \exception Exceptions::NotImplementedException This function is currently not implemented.
    */
    String GetSerialNumber();

    /**
    * \fn        GetTLType
    * \brief     This function delivers the transport layer type of the Device.
    * \return    String The name of the device.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetTLType();

    /**
    * \fn        GetDisplayName
    * \brief     This function delivers a meaningful name of the Device for display only.
    * \brief     For Baumer devices this function delivers the value of the feature 'DeviceUserID'. If this feature is not available or empty the model name of the device is delivered instead.
    * \return    String The meaningful name of the Device.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetDisplayName();

    /**
    * \fn        GetAccessStatus
    * \brief     This function delivers the access mode to the device. If this function is used before opening the device, it provides the possible access mode. The following possibilities can occur before opening the device.
    * \brief     If the function delivers with "RW" before opening, the device can be opened with one of the functions Device::Open or Device::OpenExclusive.
    * \brief     If the function delivers with "RO" before opening, the device can be opend only with function Device::OpenReadOnly.
    * \brief     If the function delivers with "NA" before opening, the device can not be opened.
    * \brief     If the function delivers with "Unknown" before opening, the access mode could not be determined. The call to an open function may throw an exception.
    * \brief     Is the device already opened, this function delivers the current access mode. The following variants can occur after opening the device.
    * \brief     If the device was opened with the function Device::Open or Device::OpenExclusive, the function delivers "RW".
    * \brief     If the device was opened with the function Device::OpenReadOnly, the function delivers "RO".
    * \return    String "RW" - read and write access
    * \return    String "RO" - only read access
    * \return    String "NA" - No access, e.g. the device is not reachable (GEV).
    * \return    String "Unknown" - The access mode could not be determined.
    * \exception Exceptions::NotAvailableException The GenTL producer delivers an unexpected access mode.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetAccessStatus();

    /**
    * \fn        GetPayloadSize
    * \brief     If the device supports a streaming channel for data transfer, this function delivers the size of the expecting data block in bytes based on the current device settings and including all control data (e.g. chunk header).
    * \brief     This function is mainly used to determine the size of image buffers to allocate. See also the functions DataStream::GetPayloadSize and DataStream::GetDefinesPayloadSize.
    * \return    bo_uint64 The size of the expected data block in bytes.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException This function is not supported.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    bo_uint64 GetPayloadSize();

    /**
    * \fn        GetRemoteNode
    * \brief     The remote functions of the Device class are used to get access to the features of the physical (remote) device.
    * \brief     This function allows the access to a requested feature of the remote device. If the requested feature is not available, an exception is thrown.
    * \param     name The requested feature of the remote device. For standardized features the predefined items in bgapi2_featurenames.h should be used.
    * \return    Node* The requested feature of the remote device in form of a Node object.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::InvalidParameterException The requested feature is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    Node* GetRemoteNode(String name);

    /**
    * \fn        GetRemoteNodeTree
    * \brief     The remote functions of the Device class are used to get access to the features of the physical (remote) device.
    * \brief     This function provides the features of the remote device as tree structure in form of a NodeMap pointer. For a detailed description of the tree structure representation refer to the class description of NodeMap.
    * \brief     The access to the features (elements) of the subordinated levels of hierarchy the functions Node::GetNodeTree and Node::GetNodeList should be used respectively.
    * \return    NodeMap* The tree structure of all features of the remote device in form of a NodeMap object.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetRemoteNodeTree();

    /**
    * \fn        GetRemoteNodeList
    * \brief     The remote functions of the Device class are used to get access to the features of the physical (remote) device.
    * \brief     This function provides the features of the remote device as unstructured list in form of a NodeMap pointer. For a detailed description of the unstructured list representation refer to the class description of NodeMap.
    * \return    NodeMap* The list of all features of the remote device in form of a NodeMap object.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    NodeMap* GetRemoteNodeList();

    /**
    * \fn        GetRemoteConfigurationFile
    * \brief     The remote functions of the Device class are used to get access to the features of the physical (remote) device.
    * \brief     This function delivers the GenICam XML File of the remote device.
    * \return    String The GenICam XML file of the remote device.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetRemoteConfigurationFile();

    /**
    * \fn        SetRemoteConfigurationFile
    * \brief     The remote functions of the Device class are used to get access to the features of the physical (remote) device.
    * \brief     This function sets a custom GenICam XML File of the remote device.
    * \param     sConfigFile is a coustom config file of the remote device. Set to "" to use the default file of the device.
    * \exception Exceptions::ResourceInUseException The device object is open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   In order to set the configuration file the device has to be closed.
    */
    void SetRemoteConfigurationFile(String sConfigFile);

    /**
    * \fn        IsUpdateModeAvailable
    * \brief     The update functions of the Device class are used to get access to firmware of the physical (remote) device.
    * \brief     This function delivers true if the update mode is available.
    * \return    bo_bool delivers true, if the UpdateMode is available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   The update mode allows an firmware update of the device. The remote device features will not be available in the update mode.
    * \remarks   In order to enable the update mode all data streams of the device have to be closed.
    * \remarks   The update mode requires the BO_UpdateAPI2 library.
    */
    bo_bool IsUpdateModeAvailable();

    /**
    * \fn        IsUpdateModeActive
    * \brief     The update functions of the Device class are used to get access to firmware of the physical (remote) device.
    * \brief     This function delivers true if the update mode is active.
    * \return    bo_bool delivers true, if the UpdateMode is active.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   The update mode allows an firmware update of the device. The remote device features will not be available in the update mode.
    * \remarks   In order to enable the update mode all data streams of the device have to be closed.
    * \remarks   The update mode requires the BO_UpdateAPI2 library.
    */
    bo_bool IsUpdateModeActive();

    /**
    * \fn        SetUpdateMode
    * \brief     The update functions of the Device class are used to get access to firmware of the physical (remote) device.
    * \brief     This function enables or disables the update mode. See remarks.
    * \param     bActive Enable or disable the update mode. This mode will control whether the nodemap contains update or bgapi features.
    * \param     pcCustomKey is reserved for future use and should be set to "".
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the update mode is already in the target state.
    * \remarks   The update mode allows an firmware update of the device. The remote device features will not be available in the update mode.
    * \remarks   In order to enable the update mode all data streams of the device have to be closed.
    * \remarks   The update mode requires the BO_UpdateAPI2 library.
    * \remarks   If you perform an update, previously requested node objects of the remote device will become invalid.
    */
    void SetUpdateMode(bool bActive, String pcCustomKey);

    /**
    * \fn        GetUpdateNode
    * \brief     The update functions of the Device class are used to get access to the update features of the physical (remote) device.
    * \brief     This function allows the access to a requested update feature of the remote device. If the requested update feature is not available, an exception is thrown.
    * \param     name The requested update feature of the remote device.
    * \return    Node* The requested update feature of the remote device in form of a Node object.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::InvalidParameterException The requested feature is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    * \remarks   The update nodes are only available if the update mode is active.
    */
    Node* GetUpdateNode(String name);

    /**
    * \fn        GetUpdateNodeTree
    * \brief     The update functions of the Device class are used to get access to the update features of the physical (remote) device.
    * \brief     This function provides the update features of the remote device as tree structure in form of a NodeMap pointer. For a detailed description of the tree structure representation refer to the class description of NodeMap.
    * \brief     The access to the update features (elements) of the subordinated levels of hierarchy the functions Node::GetNodeTree and Node::GetNodeList should be used respectively.
    * \return    NodeMap* The tree structure of all update features of the remote device in form of a NodeMap object.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   The update node tree is only available if the update mode is active.
    */
    NodeMap* GetUpdateNodeTree();

    /**
    * \fn        GetUpdateNodeList
    * \brief     The update functions of the Device class are used to get access to the update features of the physical (remote) device.
    * \brief     This function provides the update features of the remote device as unstructured list in form of a NodeMap pointer. For a detailed description of the unstructured list representation refer to the class description of NodeMap.
    * \return    NodeMap* The list of all update features of the remote device in form of a NodeMap object.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   The update node list is only available if the update mode is active.
    */
    NodeMap* GetUpdateNodeList();

    /**
    * \fn        GetUpdateConfigurationFile
    * \brief     The update functions of the Device class are used to get access to the update features of the physical (remote) device.
    * \brief     This function delivers the GenICam XML File of the update features.
    * \return    String The GenICam XML file of the update features.
    * \exception Exceptions::NotInitializedException The device object is not open.
    * \exception Exceptions::NotAvailableException The GenICam XML file of the remote device is not available.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \remarks   The update configuration file is only available if the update mode is active.
    */
    String GetUpdateConfigurationFile();

    /**
    * \fn        GetParent
    * \brief     This function delivers the superordinate Interface object.
    * \return    Interface* A pointer to the superordinate Interface object.
    */
    Interface* GetParent();

    /**
    * \fn        GetReserved
    * \brief     Undocumented function.
    */
    void * GetReserved();

 private:
    Device();
    Device(const Device &Obj);
    Device(String id, void * pReserved);
    ~Device();
    DataStreamList mDataStreamList;
    tDeviceData *m_pDeviceImpl;
    friend class DeviceList;
    friend class DataStreamList;
    friend class DataStream;
};

/**
\ingroup MainClasses
\brief The class Interface represents a physical interface, e.g. GEV or a logical interface, such as USB and belongs to the BGAPI2 main classes.
*/
class BGAPI2_DECL Interface : public INode, public Events::InterfaceEventControl {
 public:
    /**
    * \fn        Open
    * \brief     This function opens the interface object and make their functions available. Exceptions are the info-functions, which are already available before opening the interface.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the interface object is already open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::NotInitializedException The corresponding system object is not initialized.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Open();

    /**
    * \fn        Close
    * \brief     This function closes the interface object and releases the used resources.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Close();

    /**
    * \fn        IsOpen
    * \brief     This function delivers true, if the interface is opened.
    * \return    delivers true, if the interface is open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsOpen();

    /**
    * \fn        GetDevices
    * \brief     This function delivers the device list of the interface.
    * \return    DeviceList* The pointer to the device list of the interface.
    * \exception Exceptions::NotInitializedException The interface object is not open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    DeviceList* GetDevices();

    /**
    * \fn        GetID
    * \brief     This function delivers the unique string identifier of the Interface, which is used in the InterfaceList.
    * \return    String The unique string identifier.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    String GetID();

    /**
    * \fn        GetDisplayName
    * \brief     This function delivers a meaningful name of the Interface for display only.
    * \return    String The meaningful name of the Interface.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetDisplayName();

    /**
    * \fn        GetTLType
    * \brief     This function delivers the transport layer type of the Interface.
    * \return    String The transport layer type of Interface.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetTLType();

    /**
    * \fn        GetParent
    * \brief     This function delivers the superordinate System object.
    * \return    System* A pointer to the superordinate System object.
    */
    System* GetParent();

 private:
    Interface();
    Interface(const Interface &Obj);
    Interface(String id, void *pReserved);
    ~Interface();
    DeviceList mDeviceList;
    tInterfaceData *m_pInterfaceImpl;
    friend class InterfaceList;
    friend class DeviceList;
    friend class Device;
};

/**
\ingroup MainClasses
\brief The class System is the abstraction of a Producer and belongs to the BGAPI2 main classes.
*/
class BGAPI2_DECL System : public INode, public Events::EventControl {
 public:
    /**
    * \fn        System(String filepath)
    * \brief     This constructor sets the path to the producer file.
    * \param     filepath The path including filename of GenTL producer to be loaded.
    */
    System(String filepath);

    /**
    * \fn        ~System
    * \brief     The destructor.
    */
    ~System();

    /**
    * \fn        Open
    * \brief     This function opens the system object and makes their functions available. Exceptions are the info-functions, which are already available before opening the system.
    * \exception Exceptions::ResourceInUseException This exception will be thrown if the system object is already open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Open();

    /**
    * \fn        Close
    * \brief     This function closes the system object and releases the used resources.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    void Close();

    /**
    * \fn        IsOpen
    * \brief     This function delivers true, if the system is opened.
    * \return    delivers true, if the system is open.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    bo_bool IsOpen();

    /**
    * \fn        GetInterfaces
    * \brief     This function delivers the interface list of the system.
    * \return    InterfaceList* The pointer to the interface list of the system.
    * \exception Exceptions::NotInitializedException The system object is not opened.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    */
    InterfaceList* GetInterfaces();

    /**
    * \fn        GetID
    * \brief     This function delivers the unique string identifier of the System which is used in the SystemList.
    * \return    String The unique string identifier.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetID();

    /**
    * \fn        GetVendor
    * \brief     This function delivers the name of the GenTL producer vendor.
    * \return    String The name of the GenTL producer vendor.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetVendor();

    /**
    * \fn        GetModel
    * \brief     This function delivers the name of the GenTL producer to distinguish different kinds of GenTL Producer implementations.
    * \return    String The name of the GenTL producer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetModel();

    /**
    * \fn        GetVersion
    * \brief     This function delivers the GenTL Producer version.
    * \return    String The GenTL producer version.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetVersion();

    /**
    * \fn        GetTLType
    * \brief     This function delivers the transport layer type of the GenTL Producer.
    * \return    String The transport layer type of GenTL Producer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetTLType();

    /**
    * \fn        GetFileName
    * \brief     This function delivers the file name of the GenTL Producer.
    * \return    String The file name of GenTL Producer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetFileName();

    /**
    * \fn        GetPathName
    * \brief     This function delivers the full path to the GenTL Producer including filename.
    * \return    String The full path to the GenTL Producer including filename.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetPathName();

    /**
    * \fn        GetDisplayName
    * \brief     This function delivers a meaningful name of the GenTL Producer for display only.
    * \return    String The meaningful name of the GenTL Producer.
    * \exception Exceptions::ObjectInvalidException The calling object is not valid.
    * \exception Exceptions::LowLevelException GenTL Producer error. Use IException::GetErrorDescription or check trace output for more detailed error information.
    */
    String GetDisplayName();

    /**
    * \fn        GetReserved
    * \brief     Undocumented function.
    */
    void* GetReserved();

 private:
    System();
    tSystemData *m_pSystemImpl;
    InterfaceList mInterfaceList;
    friend class SystemList;
    friend class InterfaceList;
    friend class Interface;
};

class ImageProcessor;
/**
\ingroup AdditionalClasses
\brief The class Image provides the ability of image transformation. This class belongs to the additional classes.
*/
class BGAPI2_DECL Image : public INode {
 public:
    /**
    * \fn        GetWidth
    * \brief     This function delivers the width of the image in pixel.
    * \return    bo_uint The width of the image in pixel.
    */
    bo_uint GetWidth();

    /**
    * \fn        GetHeight
    * \brief     This function delivers the height of the image in pixel.
    * \return    bo_uint The height of the image in pixel.
    */
    bo_uint GetHeight();

    /**
    * \fn        GetPixelformat
    * \brief     This function delivers the pixel format of the Image object.
    * \return    String The pixelformat of the Image object.
    */
    String GetPixelformat();

    /**
    * \fn        GetBuffer
    * \brief     This function delivers a pointer to the memory of the Image object.
    * \return    void* The pointer to the memory of Image object.
    */
    void* GetBuffer();

    /**
    * \fn        GetTransformBufferLength
    * \brief     This function delivers the destination buffer size after the transformation.
    * \param     sPixelFormat The pixel format for the transformation.
    * \return    bo_uint64 The destination buffer size after the transformation.
    * \exception Exceptions::ErrorException Function returned with error.
    */
    bo_uint64 GetTransformBufferLength(String sPixelFormat);

    /**
    * \fn        GetHistogram(bo_tHistRecords tHistogram, bo_uint offsetx, bo_uint offsety, bo_uint width, bo_uint height)
    * \brief     This function delivers a histogram of a specified region of the Image object.
    * \param     tHistogram A structure which includes the histogram data.
    * \param     offsetx The left coordinate of the ROI.
    * \param     offsety The top coordinate of the ROI.
    * \param     width The width of the ROI.
    * \param     height The height of the ROI.
    * \exception Exceptions::ErrorException Function returns with error.
    */
    void GetHistogram(bo_tHistRecords tHistogram, bo_uint offsetx, bo_uint offsety, bo_uint width, bo_uint height);

    /**
    * \fn        GetHistogram(bo_tHistRecords tHistogram)
    * \brief     This function delivers a histogram of the complete Image object.
    * \param     tHistogram A structure which includes the histogram data.
    * \exception Exceptions::ErrorException Function returns with error.
    */
    void GetHistogram(bo_tHistRecords tHistogram);

    /**
    * \fn        Release
    * \brief     This function frees all used resoures.
    */
    void Release();

    /**
    * \fn        Init
    * \brief     This function reinitialise an Image object.
    * \param     width The width of the Image object in pixel.
    * \param     height The height of the Image object in pixel.
    * \param     sPixelFormat The pixelformat of the Image object.
    * \param     pBuffer The user defined image buffer.
    * \param     uBufferSize The size of the user defined image buffer bytes.
    * \exception Exceptions::ErrorException Function returns with error.
    */
    void Init(bo_uint width, bo_uint height, String sPixelFormat, void* pBuffer, bo_uint64 uBufferSize);

 private:
    Image(void * pReserved);
    ~Image();
    tImageData *m_pImageImpl;
    friend class ImageProcessor;
    friend class Polarizer;
};

/**
\ingroup AdditionalClasses
\brief The task of the class ImageProcessor are the creation of image objects and the transformation of pixel formats.
\brief This class belongs to the additional classes.
\brief A transformation can be the demosaicing of raw sensor pixel arrangements in a common displayable pixelformat, e.g. RAWBAYER to RGB8 or RGB to YUV.
\brief This class is based on DLL "bgapi2_img.dll".
*/
class BGAPI2_DECL ImageProcessor : public INode {
 public:
    /**
    * \fn        ImageProcessor()
    * \brief     Parameterless constructor for creating of an ImageProcessor-Object.
    */
    ImageProcessor();

    /**
    * \fn        ~ImageProcessor()
    * \brief     destructor to destroy an ImageProcessor object.
    */
    ~ImageProcessor();

    /**
    * \file
    * \fn        GetVersion
    * \brief     Get the version number of the ImageProcessor library. The delivered string is composed of these several tags: Major.Minor.LastChanged.RevId.
    * \return    String The version number of the ImageProcessor library.
    */
    String GetVersion();

    /**
    * \fn        CreateImage
    * \brief     This function creates an empty Image object.
    * \return    Image* The pointer to a new created Image object.
    */
    Image* CreateImage();

    /**
    * \fn        CreateImage
    * \brief     This function creates an Image object according the parameters width, height and pixelformat. The Image object uses a user defines buffer.
    * \param     width The width of the Image object in pixel.
    * \param     height The height of the Image object in pixel.
    * \param     pixelformat The pixelformat of the Image object.
    * \param     pBuffer The user defined image buffer.
    * \param     uBufferSize The size of the user defined image buffer bytes.
    * \return    Image* The pointer to a new created Image object.
    * \see         Image::GetTransformBufferLength
    */
    Image* CreateImage(bo_uint width, bo_uint height, String pixelformat, void* pBuffer, bo_uint64 uBufferSize);

    /**
    * \fn        CreateTransformedImage
    * \brief     This function transforms the pixelformat of the passed Image object into a new created Image object.
    * \param     pInputImage A pointer to a Image object whose image data is to be transformed.
    * \param     szDestinationPixelformat The desired destination pixel format of the delivered Image object.
    * \return    Image* A pointer to a new created Image object which includes the transformed image data.
    */
    Image* CreateTransformedImage(Image* pInputImage, const char* szDestinationPixelformat);

    /**
    * \fn        TransformImageToBuffer
    * \brief     This function transformes the pixel format of the Image object and writes the data into the passed destination buffer.
    * \param     pInputImage A pointer to an Image object whose image data is to be transformed.
    * \param     szDestinationPixelformat The destination pixel format.
    * \param     pBuffer The destination buffer.
    * \param     uBufferSize The destination buffer size.
    * \exception Exceptions::ErrorException Error while transform the image.
    */
    void TransformImageToBuffer(Image* pInputImage, const char* szDestinationPixelformat, void* pBuffer,
        bo_uint64 uBufferSize);

 private:
    ImageProcessor(const ImageProcessor &Obj);

    tImageProcessorData *m_pImageProcessorImpl;
};


/**
 \brief Provides functionality to calculate several different formats out of the raw polarized camera data

 */
class BGAPI2_DECL Polarizer {
 public:
    /**
    * \brief     Parameterless constructor for creating of an Polarizer object.
    */
    Polarizer();

    /**
    * \brief     Destructor to destroy an Polarizer object.
    */
    ~Polarizer();

    /**
    * \brief Initialize the Polarizer and provide the Buffer with the raw polarized data to calculate the
    * \brief different polarized formats from.
    *
    * \param[in] buffer A valid buffer with polarized data acquired by a Baumer camera.
    *
    * \exception BGAPI2::InvalidParameterException Invalid Buffer.
    */
    void Initialize(BGAPI2::Buffer* buffer);

    /**
    * \brief Get the calibration data and angle offset from the camera.
    *
    * Reads the calibration matrix and the configured polarization angle offset from the camera device to enhance
    * the calculation of different polarization formats.
    *
    * \param[in] device The BGAPI2::Device*, a pointer to the polarization camera (opened, must be able to read
    * features from the camera)
    *
    * \exception Exceptions::InvalidParameterException The passed BGAPI device is not valid.
    * \exception Exceptions::NotInitializedException The BGAPI device is not open.
    */
    void ReadCalibrationData(BGAPI2::Device* device);

    /**
    * \brief Keep output image the same size as the input buffer. The default is disabled.
    *
    * If enabled, the calculated images will be interpolated to have the same size as the raw image buffer
    * provided.
    *
    * \param[in] bo_bool interpolate If set to true the result images will be interpolated.
    */
    void EnableInterpolation(bo_bool interpolate);

    /**
    * \brief An enumeration containing the string representation of the possible polarization formats.
    */
    enum Formats {
        AOP,            /*!< Enum value AOP - Angle of polarization */
        DOLP,           /*!< Enum value DOLP - Degree of linear polarization */
        ADOLP,          /*!< Enum value ADOLP - Angle and degree of linear polarization */
        Intensity,      /*!< Enum value Intensity - Gray scale image */
        Pol0Deg,        /*!< Enum value Pol0Deg - The polarization information 0 degrees */
        Pol45Deg,       /*!< Enum value Pol45Deg - The polarization information 45 degrees */
        Pol90Deg,       /*!< Enum value Pol90Deg - The polarization information 90 degrees */
        Pol135Deg,      /*!< Enum value Pol135Deg - The polarization information 135 degrees */
        ReflectionMin,  /*!< Enum value ReflectionMin - Image with suppressed reflections */
        ReflectionMax   /*!< Enum value ReflectionMax - Image with enhanced reflections */
    };

    /**
    * \brief Each component to be calculated must be enabled first.
    *
    * To speed up the calculation of the different components it is necessary to enable them first. This allows
    * for the calculation to re-use and combine some of the necessary calculations.
    *
    * \param[in] format The format to enable or disable
    * \param[in] enable Set to true to enable or false to disable
    *
    * \see BGAPI2::Polarizer::Formats
    */
    void Enable(Formats format, bo_bool enable);

    /**
    * \brief Get the calculated component (AOP, Pol45deg etc.) from the buffer provided through the Initialize()
    * \brief method.
    *
    * For performance reasons when handling more than one component, a component must first be enabled via the
    * Polarizer::Enable() method.
    *
    * \param[in] format The format to enable or disable
    * \param[in/out] image A BGAPI2::Image to store the result of the calculation
    *
    * \see BGAPI2::Polarizer::Formats
    * \see BGAPI2::Polarizer::Initialize()
    * \see BGAPI2::ImageProcessor::CreateImage()
    *
    * \exception Exceptions::InvalidParameterException Invalid Image object
    */
    void Get(Formats format, BGAPI2::Image *image);

    /**
      \brief This class provides a iterator that can read or modify any element in the list.
    */
    class BGAPI2_DECL formatlist {
     public:
        formatlist();
        ~formatlist();
        bo_uint64 size();

        /**
         \brief This class provides a iterator that can read or modify any element in the list.
        */
        class BGAPI2_DECL const_iterator {
         public:
            /**
            * \fn        operator==()
            * \brief     This operator compares their operands for equality.
            * \param     right The second operand.
            * \return    bo_bool The result of comparison.
            */
            bo_bool operator==(const const_iterator& right) const;

            /**
            * \fn        operator!=()
            * \brief     This operator compares their operands for inequality.
            * \param     right The second operand.
            * \return    bo_bool The result of comparison.
            */
            bo_bool operator!=(const const_iterator& right) const;

            /**
            * \fn        operator++()
            * \brief     Postfix increment operator.
            * \return    const_iterator The iterator of the next list element.
            */
            const_iterator operator++();

            /**
            * \fn        operator++(int)
            * \brief     Prefix increment operator.
            * \return    const_iterator The iterator of the next list element.
            */
            const_iterator operator++(int dummy);

            /**
            * \fn        operator*()
            * \brief     The indirection operator dereferences the list pointer.
            * \return    Formats.
            */
            const Formats& operator*();

            /**
            * \fn        operator->()
            * \brief     Member access operator. This operator dereferences the list pointer.
            * \return    Formats.
            */
            const Formats* operator->();

         private:
            explicit const_iterator(const Formats* ptr);
            friend class formatlist;

            const Formats* p;
        };

        /**
        * \fn        begin
        * \brief     This function delivers a iterator on the top of the formatlist.
        * \return    const_iterator The iterator on the top of the formatlist.
        * \exception Exceptions::ObjectInvalidException The calling object is not valid.
        */
        const_iterator begin() const;

        /**
        * \fn        end
        * \brief     This function delivers an iterator at the end of the formatlist.
        * \return    const_iterator The iterator at the end of the formatlist.
        * \exception Exceptions::ObjectInvalidException The calling object is not valid.
        */
        const_iterator end() const;

     private:
        const Formats* list;
    };

    /**
    * \brief Get the string of the polarization format.
    *
    * \param[in] format The polarization format
    * String The string of the polarization format.
    *
    * \see BGAPI2::Polarizer::Formats
    *
    * \exception Exceptions::InvalidParameterException Invalid parameter
    */
    String GetFormatString(Formats format);


    /**
    * \brief Set the number of threads the Polarizer can use for calculations.
    *
    * To speed up the calculation of components more than one thread can be used internally.
    * The default is 4 threads on processors which have 8 or more logical cores, otherwise half of the logical
    * cores are used. Depending on your application you can change this here.
    *
    * \param[in] number The amount of threads used internally.
    */
    void SetMaxThreads(bo_uint number);

 private:
     void *m_pPolarizerImpl;
};

/**
\brief The namespace Exceptions consists of classes which are responsible for exception handling.
*/
namespace Exceptions {

/**
\ingroup ExceptionClasses
\brief This class is responable for the exception handling and represents the parent class of all exception classes.
*/
class BGAPI2_DECL IException {
 public:
    virtual ~IException() {}
    /**
    * \fn        GetErrorDescription
    * \brief     This function delivers a error description of the exception.
    * \return    String The error description of the exception.
    */
    virtual String GetErrorDescription() = 0;

    /**
    * \fn        GetFunctionName
    * \brief     This function delivers the name of the function which has thrown the exception.
    * \return    String The name of the function which has thrown the exception.
    */
    virtual String GetFunctionName() = 0;

    /**
    * \fn        GetType
    * \brief     This function delivers the type of the exception. The type of the exception is the name of the exception class.
    * \return    String The type of the exception.
    */
    virtual String GetType() = 0;
};

/**
\ingroup ExceptionClasses
\brief General purpose exception.
*/
class BGAPI2_DECL ErrorException : virtual public IException {
 public:
    virtual ~ErrorException() {}
};

/**
\ingroup ExceptionClasses
\brief The requested object is not initialized/opened.
*/
class BGAPI2_DECL NotInitializedException : virtual public IException {
 public:
    virtual ~NotInitializedException() {}
};

/**
\ingroup ExceptionClasses
\brief The requested function/feature is not implemented.
*/
class BGAPI2_DECL NotImplementedException : virtual public IException {
 public:
    virtual ~NotImplementedException() {}
};

/**
\ingroup ExceptionClasses
\brief The requested object is already used.
*/
class BGAPI2_DECL ResourceInUseException : virtual public IException {
 public:
    virtual ~ResourceInUseException() {}
};

/**
\ingroup ExceptionClasses
\brief The requested operation is not allowed/possible, e.g. lose the connection to the device.
*/
class BGAPI2_DECL AccessDeniedException : virtual public IException {
 public:
    virtual ~AccessDeniedException() {}
};

/**
\ingroup ExceptionClasses
\brief (Given handle does not support the operation.)
*/
class BGAPI2_DECL InvalidHandleException : virtual public IException {
 public:
    virtual ~InvalidHandleException() {}
};

/**
\ingroup ExceptionClasses
\brief An event contains no event data.
*/
class BGAPI2_DECL NoDataException : virtual public IException {
 public:
    virtual ~NoDataException() {}
};

/**
\ingroup ExceptionClasses
\brief One of the parameter given was not valid or out of range.
*/
class BGAPI2_DECL InvalidParameterException : virtual public IException {
 public:
    virtual ~InvalidParameterException() {}
};

/**
\ingroup ExceptionClasses
\brief An operation has been aborted before it could be completed.
\see BGAPI2::Events::DataStreamEventControl::CancelGetFilledBuffer and BGAPI2::Events::DataStreamEventControl::GetFilledBuffer
*/
class BGAPI2_DECL AbortException : virtual public IException {
 public:
    virtual ~AbortException() {}
};

/**
\ingroup ExceptionClasses
\brief Invalid buffer is used. The used Buffer object is not valid.
*/
class BGAPI2_DECL InvalidBufferException : virtual public IException {
 public:
    virtual ~InvalidBufferException() {}
};

/**
\ingroup ExceptionClasses
\brief The requested resource or information is not available at a given time in a current state.
*/
class BGAPI2_DECL NotAvailableException : virtual public IException {
 public:
    virtual ~NotAvailableException() {}
};

/**
\ingroup ExceptionClasses
\brief The referenced object is not a valid object of BGAPI2.
*/
class BGAPI2_DECL ObjectInvalidException : virtual public IException {
 public:
    virtual ~ObjectInvalidException() {}
};

/**
\ingroup ExceptionClasses
\brief Exception thrown by deeper software layers like GenTL producer.
*/
class BGAPI2_DECL LowLevelException : virtual public IException {
 public:
    virtual ~LowLevelException() {}
};

}  // namespace Exceptions

}  // namespace BGAPI2

#endif  // _BGAPI2_HPP__
