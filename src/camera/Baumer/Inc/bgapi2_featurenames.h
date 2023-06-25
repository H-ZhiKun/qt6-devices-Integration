/**
  Copyright 2014-2018 Baumer Optronic

  \file bgapi2_featurenames.h
*/

#ifndef TLCONSUMER_BGAPI2_GENICAM_BGAPI2_FEATURENAMES_H_
#define TLCONSUMER_BGAPI2_GENICAM_BGAPI2_FEATURENAMES_H_

/**     \ingroup    Feature
        \brief       */
#define SFNCVERSION 1.5

/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICECONTROL "DeviceControl"
/**     \ingroup    Feature
        \brief      Name of the manufacturer of the device. */
#define SFNC_DEVICE_VENDORNAME "DeviceVendorName"
/**     \ingroup    Feature
        \brief      Model of the device. */
#define SFNC_DEVICE_MODELNAME "DeviceModelName"
/**     \ingroup    Feature
        \brief      Manufacturer information about the device. */
#define SFNC_DEVICE_MANUFACTURERINFO "DeviceManufacturerInfo"
/**     \ingroup    Feature
        \brief      Version of the device. */
#define SFNC_DEVICE_VERSION "DeviceVersion"
/**     \ingroup    Feature
        \brief      Version of the firmware in the device. */
#define SFNC_DEVICE_FIRMWAREVERSION "DeviceFirmwareVersion"
/**     \ingroup    Feature
        \brief      Major version of the Standard Features Naming Convention that was used to create the device's GenICam XML. */
#define SFNC_DEVICE_SFNCVERSIONMAJOR "DeviceSFNCVersionMajor"
/**     \ingroup    Feature
        \brief      Minor version of the Standard Features Naming Convention that was used to create the device's GenICam XML. */
#define SFNC_DEVICE_SFNCVERSIONMINOR "DeviceSFNCVersionMinor"
/**     \ingroup    Feature
        \brief      Sub minor version of Standard Features Naming Convention that was used to create the device's GenICam XML. */
#define SFNC_DEVICE_SFNCVERSIONSUBMINOR "DeviceSFNCVersionSubMinor"
/**     \ingroup    Feature
        \brief      Selects the manifest entry to reference. */
#define SFNC_DEVICE_MANIFESTENTRYSELECTOR "DeviceManifestEntrySelector"
/**     \ingroup    Feature
        \brief      Indicates the major version number of the GenICam XMLfile of the selected manifest entry. */
#define SFNC_DEVICE_MANIFESTXMLMAJORVERSION "DeviceManifestXMLMajorVersion"
/**     \ingroup    Feature
        \brief      Indicates the minor version number of the GenICam XMLfile of the selected manifest entry. */
#define SFNC_DEVICE_MANIFESTXMLMINORVERSION "DeviceManifestXMLMinorVersion"
/**     \ingroup    Feature
        \brief      Indicates the subminor version number of the GenICam XMLfile of the selected manifest entry. */
#define SFNC_DEVICE_MANIFESTXMLSUBMINORVERSION "DeviceManifestXMLSubMinorVersion"
/**     \ingroup    Feature
        \brief      Indicates the major version number of the schema file of the selected manifest entry. */
#define SFNC_DEVICE_MANIFESTSCHEMAMAJORVERSION "DeviceManifestSchemaMajorVersion"
/**     \ingroup    Feature
        \brief      Indicates the minor version number of the schema file of the selected manifest entry. */
#define SFNC_DEVICE_MANIFESTSCHEMAMINORVERSION "DeviceManifestSchemaMinorVersion"
/**     \ingroup    Feature
        \brief      Indicates the first URL to the GenICam XMLdevice description file of the selected manifest entry. */
#define SFNC_DEVICE_MANIFESTPRIMARYURL "DeviceManifestPrimaryURL"
/**     \ingroup    Feature
        \brief      Indicates the second URL to the GenICam XMLdevice description file of the selected manifest entry.*/
#define SFNC_DEVICE_MANIFESTSECONDARYURL "DeviceManifestSecondaryURL"
/**     \ingroup    Feature
        \brief      depreciated SFNC2.0, replaced with "DeviceSerialNumber" */
#define SFNC_DEVICE_ID "DeviceID"
/**     \ingroup    Feature
        \brief      Device's serial number. This string is a unique identifier of the device. */
#define SFNC_DEVICE_SERIALNUMBER  "DeviceSerialNumber"
/**     \ingroup    Feature
        \brief      User-programmable device identifier. */
#define SFNC_DEVICE_USERID "DeviceUserID"
/**     \ingroup    Feature
        \brief      Resets the device to its power up state. After reset, the device must be rediscovered. */
#define SFNC_DEVICE_RESET "DeviceReset"
/**     \ingroup    Feature
        \brief      Prepare the device for registers streaming without checking for consistency. */
#define SFNC_DEVICE_REGISTERSSTREAMINGSTART "DeviceRegistersStreamingStart"
/**     \ingroup    Feature
        \brief      Announce the end of registers streaming. This will do a register set validation for consistency and activate it. This will also update the DeviceRegistersValidflag. */
#define SFNC_DEVICE_REGISTERSSTREAMINGEND "DeviceRegistersStreamingEnd"
/**     \ingroup    Feature
        \brief      Perform the validation of the current register setfor consistency.This will update the DeviceRegistersValidflag. */
#define SFNC_DEVICE_REGISTERSCHECK "DeviceRegistersCheck"
/**     \ingroup    Feature
        \brief      Returns if the current register setis valid and consistent. */
#define SFNC_DEVICE_REGISTERSVALID "DeviceRegistersValid"
/**     \ingroup    Feature
        \brief      Maximum bandwidth of the data that can be streamed out of the device. This can be used to estimate if the physical connection(s)can sustain transfer of free-running images from the camera at its maximum speed. */
#define SFNC_DEVICE_MAXTHROUGHPUT "DeviceMaxThroughput"
/**     \ingroup    Feature
        \brief      Selects the location within the device, where the temperature will be measured. */
#define SFNC_DEVICE_TEMERATURESELECTOR "DeviceTemperatureSelector"
/**     \ingroup    Feature
        \brief      Device temperature in degrees Celsius (C). It is measured at the location selected by DeviceTemperatureSelector. */
#define SFNC_DEVICE_TEMPERATURE "DeviceTemperature"
/**     \ingroup    Feature
        \brief      Selects the clock frequency to access from the device. */
#define SFNC_DEVICE_CLOCKSELECTOR "DeviceClockSelector"
/**     \ingroup    Feature
        \brief      Returns the frequency of the selected Clock. */
#define SFNC_DEVICE_CLOCKFREQUENCY "DeviceClockFrequency"
/**     \ingroup    Feature
        \brief      Selects which serial port of the device to control. */
#define SFNC_DEVICE_SERIALPORTSELECTOR "DeviceSerialPortSelector"


/**     \ingroup    Feature
        \brief      Category for Image Format Control features. */
#define SFNC_IMAGEFORMATCONTROL "ImageFormatControl"
/**     \ingroup    Feature
        \brief      Effective width of the sensor in pixels. */
#define SFNC_SENSORWIDTH "SensorWidth"
/**     \ingroup    Feature
        \brief      Effective height of the sensor in pixels. */
#define SFNC_SENSORHEIGHT "SensorHeight"
/**     \ingroup    Feature
        \brief      Number of taps of the camera sensor. */
#define SFNC_SENSORTABS "SensorTaps"
/**     \ingroup    Feature
        \brief      Number of digitized samples outputted simultaneously by the camera A/D conversion stage. */
#define SFNC_SENSORDIGITIZATIONTABS "SensorDigitizationTaps"
/**     \ingroup    Feature
        \brief      Maximum width of the image (in pixels). The dimension is calculated after horizontal binning, decimation or any other function changing the horizontal dimension of the image. */
#define SFNC_WIDTHMAX "WidthMax"
/**     \ingroup    Feature
        \brief      Maximum height of the image (in pixels). This dimension is calculated after vertical binning, decimation or any other function changing the vertical dimension of the image. */
#define SFNC_HEIGHTMAX "HeightMax"
/**     \ingroup    Feature
        \brief      Width of the image provided by the device (in pixels). */
#define SFNC_WIDTH "Width"
/**     \ingroup    Feature
        \brief      Height of the image provided by the device (in pixels). */
#define SFNC_HEIGHT "Height"
/**     \ingroup    Feature
        \brief      Horizontal offset from the origin to the region of interest (in pixels). */
#define SFNC_OFFSETX "OffsetX"
/**     \ingroup    Feature
        \brief      Vertical offset from the origin to the region of interest (in pixels). */
#define SFNC_OFFSETY "OffsetY"
/**     \ingroup    Feature
        \brief      Total number of bytes between the startsof 2 consecutivelines. This feature is used to facilitate alignment of image data. */
#define SFNC_LINEPITCH "LinePitch"
/**     \ingroup    Feature
        \brief      Number of horizontal photo-sensitive cells to combine together. This reduces the horizontal resolution (width) of the image. */
#define SFNC_BINNINGHORIZONTAL "BinningHorizontal"
/**     \ingroup    Feature
        \brief      Number of vertical photo-sensitive cells to combine together. This reduces the vertical resolution (height) of the image. */
#define SFNC_BINNINGVERTICAL "BinningVertical"
/**     \ingroup    Feature
        \brief      Horizontal sub-sampling of the image. This reduces the horizontal resolution (width) of the image by the specified horizontal decimation factor. */
#define SFNC_DECIMATIONHORIZONTAL "DecimationHorizontal"
/**     \ingroup    Feature
        \brief      Vertical sub-sampling of the image. This reduces the vertical resolution (height) of the image by the specified vertical decimation factor. */
#define SFNC_DECIMATIONVERTICAL "DecimationVertical"
/**     \ingroup    Feature
        \brief      Flip horizontally the image sent by the device. The Region of interest is applied after the flipping. */
#define SFNC_REVERSEX "ReverseX"
/**     \ingroup    Feature
        \brief      Flip vertically the image sent by the device. The Region of interest is applied after the flipping. */
#define SFNC_REVERSEY "ReverseY"
/**     \ingroup    Feature
        \brief      Format of the pixels provided by the device. It represents all the information provided by PixelSize, PixelColorFilter combined in a single feature. */
#define SFNC_PIXELFORMAT "PixelFormat"
/**     \ingroup    Feature
        \brief      This feature is deprecated. It represents the coding of the pixels in the image. Raw gives the data in the native format of the sensor. */
#define SFNC_PIXELCODING "PixelCoding"
/**     \ingroup    Feature
        \brief      Total size in bits of a pixel of the image.  */
#define SFNC_PIXELSIZE "PixelSize"
/**     \ingroup    Feature
        \brief      Type of color filter that is applied to the image. */
#define SFNC_PIXELCOLORFILTER "PixelColorFilter"
/**     \ingroup    Feature
        \brief      Minimum value that can be returned during the digitization process. This corresponds to the darkest value of the camera. For color camera, this returns the smallest value that each color component can take. */
#define SFNC_PIXELDYNAMICRANGEMIN "PixelDynamicRangeMin"
/**     \ingroup    Feature
        \brief      Maximum value that will be returned during the digitization process. This corresponds to the brightest value of the camera. For color camera, this returns the biggest value that each color component can take. */
#define SFNC_PIXELDYNAMICRANGEMAX "PixelDynamicRangeMax"
/**     \ingroup    Feature
        \brief      This feature is deprecated(See TestPattern). Selects the type of test image that is sent by the device. */
#define SFNC_TESTIMAGESELECTOR "TestImageSelector"


/**     \ingroup    Feature
        \brief      Category for the acquisition and trigger control features.  */
#define SFNC_ACQUISITIONCONTROL "AcquisitionControl"
/**     \ingroup    Feature
        \brief      Sets the acquisition mode of the device. It defines mainly the number of frames to capture during an acquisition and the way the acquisition stops. */
#define SFNC_ACQUISITION_MODE "AcquisitionMode"
/**     \ingroup    Feature
        \brief      Starts the Acquisition of the device. The number of frames captured is specified by AcquisitionMode. */
#define SFNC_ACQUISITION_START "AcquisitionStart"
/**     \ingroup    Feature
        \brief      Stops the Acquisition of the device at the end of the current Frame. It is mainly used when AcquisitionMode is Continuous but can be used in any acquisition mode. */
#define SFNC_ACQUISITION_STOP "AcquisitionStop"
/**     \ingroup    Feature
        \brief      Aborts the Acquisition immediately. This will end the capture without completing the current Frame or waiting on a trigger. If no Acquisition is in progress, the command is ignored. */
#define SFNC_ACQUISITION_ABORT "AcquisitionAbort"
/**     \ingroup    Feature
        \brief      Arms the device before an AcquisitionStartcommand. This optional command validates all the current features for consistency and prepares the device for a fast start of the Acquisition. */
#define SFNC_ACQUISITION_ARM "AcquisitionArm"
/**     \ingroup    Feature
        \brief      Number of frames to acquire in MultiFrame Acquisition mode. */
#define SFNC_ACQUISITION_FRAMECOUNT "AcquisitionFrameCount"
/**     \ingroup    Feature
        \brief      Controls the acquisition rate (in Hertz) at which the frames are captured. */
#define SFNC_ACQUISITION_FRAMERATE "AcquisitionFrameRate"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_ACQUISITION_FRAMERATEABS "AcquisitionFrameRateAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_ACQUISITION_FRAMERATERAW "AcquisitionFrameRateRaw"
/**     \ingroup    Feature
        \brief      Controls the rate (in Hertz) at which the Lines in a Frame are captured. */
#define SFNC_ACQUISITION_LINERATE "AcquisitionLineRate"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_ACQUISITION_LINERATEABS "AcquisitionLineRateAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_ACQUISITION_LINERATERAW "AcquisitionLineRateRaw"
/**     \ingroup    Feature
        \brief      Selects the internal acquisition signal to read using AcquisitionStatus. */
#define SFNC_ACQUISITION_STATUSSELECTOR "AcquisitionStatusSelector"
/**     \ingroup    Feature
        \brief      Reads the state of the internal acquisition signal selected using AcquisitionStatusSelector. */
#define SFNC_ACQUISITION_STATUS "AcquisitionStatus"
/**     \ingroup    Feature
        \brief      Selects the type of trigger to configure. */
#define SFNC_TRIGGERSELECTOR "TriggerSelector"
/**     \ingroup    Feature
        \brief      Controls if the selected trigger is active. */
#define SFNC_TRIGGERMODE "TriggerMode"
/**     \ingroup    Feature
        \brief      Generates an internal trigger. TriggerSource must be set to Software. */
#define SFNC_TRIGGERSOFTWARE "TriggerSoftware"
/**     \ingroup    Feature
        \brief      Specifies the internal signal or physical input Line to use as the trigger source. The selected trigger must have its TriggerMode set to On. */
#define SFNC_TRIGGERSOURCE "TriggerSource"
/**     \ingroup    Feature
        \brief      Specifies the activation mode of the trigger. */
#define SFNC_TRIGGERACTIVATION "TriggerActivation"
/**     \ingroup    Feature
        \brief      Specifies the type trigger overlap permitted with the previous frame or line. This defines when a valid trigger will be accepted (or latched) for a new frame or a new line. */
#define SFNC_TRIGGEROVERLAP "TriggerOverlap"
/**     \ingroup    Feature
        \brief      Specifies the delay in microseconds (us) to apply after the trigger reception before activating it. */
#define SFNC_TRIGGERDELAY "TriggerDelay"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TRIGGERDELAYABS "TriggerDelayAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TRIGGERDELAYRAW "TriggerDelayRaw"
/**     \ingroup    Feature
        \brief      Specifies a division factor for the incoming trigger pulses. */
#define SFNC_TRIGGERDIVIDER "TriggerDivider"
/**     \ingroup    Feature
        \brief      Specifies a multiplication factor for the incoming trigger pulses.It is used generally used in conjunction with TriggerDividerto control the ratio of triggers that are accepted. */
#define SFNC_TRIGGERMULTIPLIER "TriggerMultiplier"
/**     \ingroup    Feature
        \brief      Sets the operation mode of the Exposure. */
#define SFNC_EXPOSUREMODE "ExposureMode"
/**     \ingroup    Feature
        \brief      Sets the Exposure time when ExposureMode is Timed and ExposureAuto is Off. This controls the duration where the photosensitive cells are exposed to light. */
#define SFNC_EXPOSURETIME "ExposureTime"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_EXPOSURETIMEABS "ExposureTimeAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_EXPOSURETIMERAW "ExposureTimeRaw"
/**     \ingroup    Feature
        \brief      Sets the automatic exposure mode when ExposureMode is Timed. The exact algorithm used to implement this control is device-specific. */
#define SFNC_EXPOSUREAUTO "ExposureAuto"


/**     \ingroup    Feature
        \brief      Category that contains the digital input and output control features. */
#define SFNC_DIGITALIOCONTROL "DigitalIOControl"
/**     \ingroup    Feature
        \brief      Selects the physical line (or pin) of the external device connector or the virtual line of the Transport Layer to configure. */
#define SFNC_LINESELECTOR "LineSelector"
/**     \ingroup    Feature
        \brief      Controls if the physical Line is used to Input or Output a signal. */
#define SFNC_LINEMODE "LineMode"
/**     \ingroup    Feature
        \brief      Controls the inversion of the signal of the selected input or output Line. */
#define SFNC_LINEINVERTER "LineInverter"
/**     \ingroup    Feature
        \brief      Returns the current status of the selected input or output Line. */
#define SFNC_LINESTATUS "LineStatus"
/**     \ingroup    Feature
        \brief      Returns the current status of all available Line signals at time of polling in a single bitfield. */
#define SFNC_LINESTATUSALL "LineStatusAll"
/**     \ingroup    Feature
        \brief      Selects which internal acquisition or I/O source signal to output on the selected Line. LineMode must be Output. */
#define SFNC_LINESOURCE "LineSource"
/**     \ingroup    Feature
        \brief      Controls the current electrical format of the selected physical input or output Line. */
#define SFNC_LINEFORMAT "LineFormat"
/**     \ingroup    Feature
        \brief      Selects which bit of the User Output register will be set by UserOutputValue. */
#define SFNC_USEROUTPUTSELECTOR "UserOutputSelector"
/**     \ingroup    Feature
        \brief      Sets the value of the bit selected by UserOutputSelector. */
#define SFNC_USEROUTPUTVALUE "UserOutputValue"
/**     \ingroup    Feature
        \brief      Sets the value of all the bits of the User Output register. It is subject to the UserOutputValueAllMask. */
#define SFNC_USEROUTPUTVALUEALL "UserOutputValueAll"
/**     \ingroup    Feature
        \brief      Sets the write mask to apply to the value specified by UserOutputValueAllbefore writing it in the User Output register. If the UserOutputValueAllMaskfeature is present, setting the user Output register using UserOutputValueAllwill only change the bits that have a corresponding bit in the mask set to one. */
#define SFNC_USEROUTPUTVALUEALLMASK "UserOutputValueAllMask"


/**     \ingroup    Feature
        \brief      Category that contains the CounterandTimer control features.  */
#define SFNC_COUNTERANDTIMERCONTROL "CounterAndTimerControl"
/**     \ingroup    Feature
        \brief      Selects which Counter to configure. */
#define SFNC_COUNTERSELECTOR "CounterSelector"
/**     \ingroup    Feature
        \brief      Select the events that will be the source to increment the Counter. */
#define SFNC_COUNTEREVENTSOURCE "CounterEventSource"
/**     \ingroup    Feature
        \brief      Selects the Activation mode Event Source signal. */
#define SFNC_COUNTEREVENTACTIVATION "CounterEventActivation"
/**     \ingroup    Feature
        \brief      Selects the signals that will be the source to reset the Counter. */
#define SFNC_COUNTERRESETSOURCE "CounterResetSource"
/**     \ingroup    Feature
        \brief      Selects the Activation mode of the Counter Reset Source signal. */
#define SFNC_COUNTERRESETACTIVATION "CounterResetActivation"
/**     \ingroup    Feature
        \brief      Does a software reset of the selected Counter and starts it. The counter starts counting events immediately after the reset unless a Counter trigger is active. CounterReset can be used to reset the Counter independently from the CounterResetSource. To disable the counter temporarily, set CounterEventSource to Off. */
#define SFNC_COUNTERRESET "CounterReset"
/**     \ingroup    Feature
        \brief      Reads or writes the current value of the selected Counter. */
#define SFNC_COUNTERVALUE "CounterValue"
/**     \ingroup    Feature
        \brief      Reads the value of the selected Counter when it was reset by a trigger or by an explicit CounterReset command. */
#define SFNC_COUNTERVALUEATRESET "CounterValueAtReset"
/**     \ingroup    Feature
        \brief      Sets the duration (or number of events) before the CounterEnd event is generated. */
#define SFNC_COUNTERDURATION "CounterDuration"
/**     \ingroup    Feature
        \brief      Returns the current statusof the Counter. */
#define SFNC_COUNTERSTATUS "CounterStatus"
/**     \ingroup    Feature
        \brief      Selects the source to start the Counter.  */
#define SFNC_COUNTERTRIGGERSOURCE "CounterTriggerSource"
/**     \ingroup    Feature
        \brief      Selects the activation mode of the trigger to start the Counter. */
#define SFNC_COUNTERTRIGGERACTIVATION "CounterTriggerActivation"
/**     \ingroup    Feature
        \brief      Selects which Timer to configure. */
#define SFNC_TIMERSELECTOR "TimerSelector"
/**     \ingroup    Feature
        \brief      Sets the duration (in microseconds) of the Timer pulse. */
#define SFNC_TIMERDURATION "TimerDuration"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TIMERDURATIONABS "TimerDurationAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TIMERDURATIONRAW "TimerDurationRaw"
/**     \ingroup    Feature
        \brief      Sets the duration (in microseconds) of the delay to apply at the reception of a trigger before starting the Timer. */
#define SFNC_TIMERDELAY "TimerDelay"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TIMERDELAYABS "TimerDelayAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TIMERDELAYRAW "TimerDelayRaw"
/**     \ingroup    Feature
        \brief      Does a software reset of the selected timer and starts it. The timer starts immediately after the reset unless a timer trigger is active. */
#define SFNC_TIMERRESET "TimerReset"
/**     \ingroup    Feature
        \brief      Reads or writes the current value (in microseconds) of the selected Timer. */
#define SFNC_TIMERVALUE "TimerValue"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TIMERVALUEABS "TimerValueAbs"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_TIMERVALUERAW "TimerValueRaw"
/**     \ingroup    Feature
        \brief      Returns the current statusof the Timer. */
#define SFNC_TIMERSTATUS "TimerStatus"
/**     \ingroup    Feature
        \brief      Selects the source of the trigger to start the Timer. */
#define SFNC_TIMERTRIGGERSOURCE "TimerTriggerSource"
/**     \ingroup    Feature
        \brief      Selects the activation mode of the trigger to start the Timer. */
#define SFNC_TIMERTRIGGERACTIVATION "TimerTriggerActivation"


/**     \ingroup    Feature
        \brief      Category that contains Event control features. */
#define SFNC_EVENTCONTROL "EventControl"
/**     \ingroup    Feature
        \brief      Selects which Event to signal to the host application. */
#define SFNC_EVENTSELECTOR "EventSelector"
/**     \ingroup    Feature
        \brief      Activate or deactivate the notification to the host application of the occurrence of the selected Event. */
#define SFNC_EVENTNOTIFICATION "EventNotification"
/**     \ingroup    Feature
        \brief      Category that contains all the data features related to the FrameTrigger Event. */
#define SFNC_EVENT_FRAMETRIGGERDATA "EventFrameTriggerData"
/**     \ingroup    Feature
        \brief      Returns the unique Identifier of the FrameTrigger type of Event. It can be used to register a callback function to be notified of the event occurrence. Its value uniquely identifiesthe type event received. */
#define SFNC_EVENT_FRAMETRIGGER "EventFrameTrigger"
/**     \ingroup    Feature
        \brief      Returns the Timestamp of the FrameTrigger Event. It can be used to determine precisely when the event occurred.  */
#define SFNC_EVENT_FRAMETRIGGERTIMESTAMP "EventFrameTriggerTimestamp"
/**     \ingroup    Feature
        \brief      Returns the unique Identifier of the Frame (or image) that generated the FrameTrigger Event. */
#define SFNC_EVENT_FRAMETRIGGERFRAMEID "EventFrameTriggerFrameID"
/**     \ingroup    Feature
        \brief      Category that contains all the data features related to the ExposureEnd Event. */
#define SFNC_EVENT_EXPOSUREENDDATA "EventExposureEndData"
/**     \ingroup    Feature
        \brief      Returns the unique identifier of the ExposureEnd type of Event. This feature can be used to register a callback function to be notified of the event occurrence. Its value uniquely identifies the type of event that will be received. */
#define SFNC_EVENT_EXPOSUREEND "EventExposureEnd"
/**     \ingroup    Feature
        \brief      Returns the Timestamp of the ExposureEnd Event. It can be used to determine precisely when the event occurred. */
#define SFNC_EVENT_EXPOSUREENDTIMESTAMP "EventExposureEndTimestamp"
/**     \ingroup    Feature
        \brief      Returns the unique Identifier of the Frame (or image) that generated the ExposureEnd Event. */
#define SFNC_EVENT_EXPOSUREENDFRAMEID "EventExposureEndFrameID"
/**     \ingroup    Feature
        \brief      Category that contains all the data features related to the Error Event. */
#define SFNC_EVENT_ERRORDATA "EventErrorData"
/**     \ingroup    Feature
        \brief      Returns the unique identifier of the Error type of Event. It can be used to register a callbackfunction to be notified of the Error event occurrence. Its value uniquely identifies that the event received was an Error. */
#define SFNC_EVENT_ERROR "EventError"
/**     \ingroup    Feature
        \brief      Returns the Timestamp of the Error Event. It can be used to determine when the event occurred. */
#define SFNC_EVENT_ERRORTIMESTAMP "EventErrorTimestamp"
/**     \ingroup    Feature
        \brief      If applicable, returns the unique Identifier of the Frame (or image) that generated the Error Event. */
#define SFNC_EVENT_ERRORFRAMEID "EventErrorFrameID"
/**     \ingroup    Feature
        \brief      Returns an error code for the error(s) that happened. */
#define SFNC_EVENT_ERRORCODE "EventErrorCode"


/**     \ingroup    Feature
        \brief      Category that contains the Analog control features. */
#define SFNC_ANALOGCONTROL "AnalogControl"
/**     \ingroup    Feature
        \brief      Selects which Gain is controlled by the various Gain features. */
#define SFNC_GAINSELECTOR "GainSelector"
/**     \ingroup    Feature
        \brief      Controls the selected gain as an absolute physical value. This is an amplification factor applied to the video signal. */
#define SFNC_GAIN "Gain"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GAINRAW "GainRaw"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GAINABS "GainAbs"
/**     \ingroup    Feature
        \brief      Sets the automatic gain control (AGC) mode. The exact algorithm used to implement AGC is device-specific. */
#define SFNC_GAINAUTO "GainAuto"
/**     \ingroup    Feature
        \brief      Sets the mode for automatic gain balancing between the sensor color channels or taps. The gain coefficients of each channel or tap are adjusted so they are matched. */
#define SFNC_GAINAUTOBALANCE "GainAutoBalance"
/**     \ingroup    Feature
        \brief      Selects which Black Level is controlled by the various Black Level features. */
#define SFNC_BLACKLEVELSELECTOR "BlackLevelSelector"
/**     \ingroup    Feature
        \brief      Controls the analog black level as an absolute physical value. This represents a DC offset applied to the video signal. */
#define SFNC_BLACKLEVEL "BlackLevel"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_BLACKLEVELRAW "BlackLevelRaw"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_BLACKLEVELABS "BlackLevelAbs"
/**     \ingroup    Feature
        \brief      Controls the mode for automatic black level adjustment. The exact algorithm used to implement this adjustment is device-specific. */
#define SFNC_BLACKLEVELAUTO "BlackLevelAuto"
/**     \ingroup    Feature
        \brief      Controls the mode for automatic black level balancing between the sensor color channels or taps. Theblack level coefficients of each channel are adjusted so they are matched. */
#define SFNC_BLACKLEVELAUTOBALANCE "BlackLevelAutoBalance"
/**     \ingroup    Feature
        \brief      Selects which White Clip to control. */
#define SFNC_WHITECLIPSELECTOR "WhiteClipSelector"
/**     \ingroup    Feature
        \brief      Controls the maximal intensity taken by the video signal before being clipped as an absolute physical value. The video signal will never exceed the white clipping point: it will saturate at that level.  */
#define SFNC_WHITECLIP "WhiteClip"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_WHITECLIPRAW "WhiteClipRaw"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_WHITECLIPABS "WhiteClipAbs"
/**     \ingroup    Feature
        \brief      Selects which Balance ratio to control.  */
#define SFNC_BALANCERATIOSELECTOR "BalanceRatioSelector"
/**     \ingroup    Feature
        \brief      Controls ratio of the selected color component to a reference color component. It is used for white balancing. */
#define SFNC_BALANCERATIO "BalanceRatio"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_BALANCERATIOABS "BalanceRatioAbs"
/**     \ingroup    Feature
        \brief      Controls the mode for automatic white balancing between the color channels. The white balancing ratios are automatically adjusted.  */
#define SFNC_BALANCEWHITEAUTO "BalanceWhiteAuto"
/**     \ingroup    Feature
        \brief      Controls the gamma correction of pixel intensity. This is typically used to compensate for non-linearity of the display system (such as CRT). */
#define SFNC_GAMMA "Gamma"


/**     \ingroup    Feature
        \brief      Category that includes the LUT control features. */
#define SFNC_LUTCONTROL "LUTControl"
/**     \ingroup    Feature
        \brief      Selects which LUT to control. */
#define SFNC_LUTSELECTOR "LUTSelector"
/**     \ingroup    Feature
        \brief      Activates the selected LUT. */
#define SFNC_LUTENABLE "LUTEnable"
/**     \ingroup    Feature
        \brief      Control the index (offset) of the coefficient to access in the selected LUT. */
#define SFNC_LUTINDEX "LUTIndex"
/**     \ingroup    Feature
        \brief      Returns the Value at entry LUTIndexof the LUT selected by LUTSelector. */
#define SFNC_LUTVALUE "LUTValue"
/**     \ingroup    Feature
        \brief      Accesses all the LUT coefficients in a single access without using individual LUTIndex. */
#define SFNC_LUTVALUEALL "LUTValueAll"


/**     \ingroup    Feature
        \brief      Provides the Root of the GenICam features tree.  */
#define SFNC_ROOT "Root"
/**     \ingroup    Feature
        \brief      Provides the default GenICam port of the Device.  */
#define SFNC_DEVICE "Device"
/**     \ingroup    Feature
        \brief      Used by the Transport Layer to prevent critical features from changing during acquisition. */
#define SFNC_TLPARAMSLOCKED "TLParamsLocked"


/**     \ingroup    Feature
        \brief      Category that contains the transport Layer control features. */
#define SFNC_TRANSPORTLAYERCONTROL "TransportLayerControl"
/**     \ingroup    Feature
        \brief      Provides the number of bytes transferred for each image or chunk on the stream channel. This includes any end-of-line, end-of-frame statistics or other stamp data. This is the total size of data payload for a data block.  */
#define SFNC_PAYLOADSIZE "PayloadSize"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceTLVersionMajor). It was representing the major version of the specification. */
#define SFNC_GEV_VERSIONMAJOR "GevVersionMajor"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceTLVersionMinor). It was representing the minor version of the specification. */
#define SFNC_GEV_VERSIONMINOR "GevVersionMinor"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceRegistersEndianness). It was representing the Endianness of the device registers. */
#define SFNC_GEV_DEVICEMODEISBIGENDIAN "GevDeviceModeIsBigEndian"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceType). It was representing the class of the device. */
#define SFNC_GEV_DEVICECALSS "GevDeviceClass"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceCharacterSet). It was representing the character set used by all the strings of the bootstrap registers. */
#define SFNC_GEV_DEVICEMODECHARACTERSET "GevDeviceModeCharacterSet"
/**     \ingroup    Feature
        \brief      Selects which logical link to control. */
#define SFNC_GEV_INTERFACESELECTOR "GevInterfaceSelector"
/**     \ingroup    Feature
        \brief      MAC address of the logical link. */
#define SFNC_GEV_MACADDRESS "GevMACAddress"
/**     \ingroup    Feature
        \brief      Selects the GEV option to interrogate for existing support. */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTOR "GevSupportedOptionSelector"
/**     \ingroup    Feature
        \brief      Returns if the selected GEV option is supported. */
#define SFNC_GEV_SUPPORTEDOPTION "GevSupportedOption"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDIPCONFIGURATIONLLA "GevSupportedIPConfigurationLLA"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDIPCONFIGURATIONDHCP "GevSupportedIPConfigurationDHCP"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDIPCONFIGURATIONPERSISTENTIP "GevSupportedIPConfigurationPersistentIP"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_CURRENTIPCONFIGURATION "GevCurrentIPConfiguration"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_CURRENTIPCONFIGURATIONLLA "GevCurrentIPConfigurationLLA"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_CURRENTIPCONFIGURATIONDHCP "GevCurrentIPConfigurationDHCP"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_CURRENTIPCONFIGURATIONPERSISTENTIP "GevCurrentIPConfigurationPersistentIP"
/**     \ingroup    Feature
        \brief      Reports the IP address for the given logical link. */
#define SFNC_GEV_CURRENTIPADDRESS "GevCurrentIPAddress"
/**     \ingroup    Feature
        \brief      Reports the subnet mask of the given logical link. */
#define SFNC_GEV_CURRENTSUBNETMASK "GevCurrentSubnetMask"
/**     \ingroup    Feature
        \brief      Reports the default gateway IP address to be used on the given logical link. */
#define SFNC_GEV_CURRENTDEFAULTGATEWAY "GevCurrentDefaultGateway"
/**     \ingroup    Feature
        \brief      Reports the current IP configuration status. */
#define SFNC_GEV_IPCONFIGURATIONSTATUS "GevIPConfigurationStatus"
/**     \ingroup    Feature
        \brief      Deprecated! Indicates the first URL to the GenICam XML device description file. The First URL is used as the first choice by the application to retrieve the GenICam XML device description file. */
#define SFNC_GEV_FIRSTURL "GevFirstURL"
/**     \ingroup    Feature
        \brief      Deprecated! Indicates the second URL to the GenICam XML device description file. This URL is an alternative if the application was unsuccessful to retrieve the device description file using the first URL.  */
#define SFNC_GEV_SECONDURL "GevSecondURL"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceLinkSelector). It was representing the number of logical links supported by this device. */
#define SFNC_GEV_NUMBEROFINTERFACES "GevNumberOfInterfaces"
/**     \ingroup    Feature
        \brief      Controls the Persistent IP address for this logical link. It is only used when the device boots with the Persistent IP configuration scheme. */
#define SFNC_GEV_PERSISTENTIPADDRESS "GevPersistentIPAddress"
/**     \ingroup    Feature
        \brief      Controls the Persistent subnet mask associated with the Persistent IP address on this logical link. It is only used when the device boots with the Persistent IP configuration scheme. */
#define SFNC_GEV_PERSISTENTSUBNETMASK "GevPersistentSubnetMask"
/**     \ingroup    Feature
        \brief      Controls the persistent default gateway for this logical link. It is only used when the device boots with the Persistent IP configuration scheme. */
#define SFNC_GEV_PERSISTENTDEFAULTGATEWAY "GevPersistentDefaultGateway"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceLinkSpeed). It was representing the speed of transmission negotiated by the given logical link. */
#define SFNC_GEV_GEVLINKSPEED "GevLinkSpeed"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceEventChannelCount). It was representing the number of message channels supported by this device.  */
#define SFNC_GEV_MESSAGECHANNELCOUNT "GevMessageChannelCount"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceStreamChannelCount). It was representing the number of stream channels supported by this device. */
#define SFNC_GEV_STREAMCHANNELCOUNT "GevStreamChannelCount"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSUSERDEFINEDNAME "GevSupportedOptionalCommandsUserDefinedName"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSSERIALNUMBER "GevSupportedOptionalCommandsSerialNumber"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSEVENTDATA "GevSupportedOptionalCommandsEVENTDATA"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSEVENT "GevSupportedOptionalCommandsEVENT"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSPACKETRESEND "GevSupportedOptionalCommandsPACKETRESEND"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSWRITEMEM "GevSupportedOptionalCommandsWRITEMEM"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONALCOMMANDSCONCATENATION "GevSupportedOptionalCommandsConcatenation"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceLinkHeartbeatTimeout). It was controling the current heartbeat timeout in milliseconds.  */
#define SFNC_GEV_HEARTBEATTIMEOUT "GevHeartbeatTimeout"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See the increment of the TimestampLatchValue feature). It was used to indicate the number of timestamp ticks in 1 second (frequency in Hz). If IEEE 1588 is used, this feature must return 1,000,000,000 (1 GHz). */
#define SFNC_GEV_TIMESTAMPTICKFREQUENCY "GevTimestampTickFrequency"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See TimestampLatch). It was used to latche the current timestamp counter into GevTimestampValue. */
#define SFNC_GEV_TIMESTAMPCONTROLLATCH "GevTimestampControlLatch"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See TimestampReset). It was used to reset the timestamp counter to 0. This feature is not available or as no effect when IEEE 1588 is used. */
#define SFNC_GEV_TIMESTAMPCONTROLRESET "GevTimestampControlReset"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See TimestampLatchValue). It was used to return the latched 64-bit value of the timestamp counter.  */
#define SFNC_GEV_TIMESTAMPVALUE "GevTimestampValue"
/**     \ingroup    Feature
        \brief      Indicates the maximum randomized delay the device will wait to acknowledge a discovery command. */
#define SFNC_GEV_DISCOVERYACKDELAY "GevDiscoveryAckDelay"
/**     \ingroup    Feature
        \brief      Enables the generation of extended status codes. */
#define SFNC_GEV_GVCPEXTENDEDSTATUSCODES "GevGVCPExtendedStatusCodes"
/**     \ingroup    Feature
        \brief      Enables the generation of PENDING_ACK. */
#define SFNC_GEV_GVCPPENDINGACK "GevGVCPPendingAck"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceHeartbeatMode).It was used to disable the GVCP heartbeat. */
#define SFNC_GEV_GVCPHEARTBEATDISABLE "GevGVCPHeartbeatDisable"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceLinkCommandTimeout). It was used to indicate the longest GVCP command execution time before a device returns a PENDING_ACK. */
#define SFNC_GEV_GVCPPENDINGTIMEOUT "GevGVCPPendingTimeout"
/**     \ingroup    Feature
        \brief      Controls the key to use to authenticate primary application switchover requests. */
#define SFNC_GEV_PRIMARYAPPLICATIONSWITCHOVERKEY "GevPrimaryApplicationSwitchoverKey"
/**     \ingroup    Feature
        \brief      Controls the device access privilege of an application.  */
#define SFNC_GEV_CCP "GevCCP"
/**     \ingroup    Feature
        \brief      Returns the UDP source port of the primary application. */
#define SFNC_GEV_PRIMARYAPPLICATIONSOCKET "GevPrimaryApplicationSocket"
/**     \ingroup    Feature
        \brief      Returns the address of the primary application. */
#define SFNC_GEV_PRIMARYAPPLICATIONIPADDRESS "GevPrimaryApplicationIPAddress"
/**     \ingroup    Feature
        \brief      Controls the port to which the device must send messages. Setting this value to 0 closes the message channel. */
#define SFNC_GEV_MCPHOSTPORT "GevMCPHostPort"
/**     \ingroup    Feature
        \brief      Controls the destination IP address for the message channel. */
#define SFNC_GEV_MCDA "GevMCDA"
/**     \ingroup    Feature
        \brief      Provides the transmission timeout value in milliseconds.  */
#define SFNC_GEV_MCTT "GevMCTT"
/**     \ingroup    Feature
        \brief      Controls the number of retransmissions allowed when a message channel message times out. */
#define SFNC_GEV_MCRC "GevMCRC"
/**     \ingroup    Feature
        \brief      This feature indicates the sourceport for the message channel. */
#define SFNC_GEV_MCSP "GevMCSP"
/**     \ingroup    Feature
        \brief      Selects the stream channel to control. */
#define SFNC_GEV_STREAMCHANNELSELECTOR "GevStreamChannelSelector"
/**     \ingroup    Feature
        \brief      Enables the alternate IP destination for stream packets resent due to a packet resend request. When True, the source IP address provided in the packet resend command packet is used. When False, the value set in the GevSCDA[GevStreamChannelSelector]featureis used. */
#define SFNC_GEV_SCCFGUNCONDITIONALSTREAMING "GevSCCFGUnconditionalStreaming"
/**     \ingroup    Feature
        \brief      Enables cameras to use the extended chunk data payload type for this stream channel. */
#define SFNC_GEV_SCCFGEXTENDEDCHUNKDATA "GevSCCFGExtendedChunkData"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceStreamChannelType). It was used to report the direction of the stream channel. */
#define SFNC_GEV_SCPDIRECTION "GevSCPDirection"
/**     \ingroup    Feature
        \brief      Index of the logical link to use. */
#define SFNC_GEV_SCPINTERFACEINDEX "GevSCPInterfaceIndex"
/**     \ingroup    Feature
        \brief      Controls the port of the selected channel to which a GVSP transmitter must send data stream or the port from which a GVSP receiver may receive data stream. Setting this value to 0 closes the stream channel. */
#define SFNC_GEV_SCPHOSTPORT "GevSCPHostPort"
/**     \ingroup    Feature
        \brief      Sends a test packet. When this feature is set, the device will fire one test packet.  */
#define SFNC_GEV_SCPSFIRETESTPACKET "GevSCPSFireTestPacket"
/**     \ingroup    Feature
        \brief      The state of this feature is copied into the "do not fragment" bit of IP header of eachstream packet. It can be used by the application to prevent IP fragmentation of packets on the stream channel. */
#define SFNC_GEV_SCPSDONOTFRAGMENT "GevSCPSDoNotFragment"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See DeviceStreamChannelEndianness). It was used to control the endianness of multi-byte pixel data for this stream.  */
#define SFNC_GEV_SCPSBIGENDIAN "GevSCPSBigEndian"
/**     \ingroup    Feature
        \brief      Version 2.3Standard FeaturesNaming Convention2016-5-26Page477of 519CategoryGigEVisionLevelRecommendedInterfaceIIntegerAccessRead/(Write)UnitBVisibilityExpertValues>0This GigE Vision specific feature corresponds to DeviceStreamChannelPacketSizeand should be kept in sync with it. It specifiesthe stream packet size, in bytes, to send on the selected channel for a GVSP transmitter or specifies the maximum packet size supported by a GVSP receiver.This does not include data leader and data trailer and the last data packet which might be of smaller size (since packet size is not necessarily a multiple of block size for stream channel). */
#define SFNC_GEV_SCPSPACKETSIZE "GevSCPSPacketSize"
/**     \ingroup    Feature
        \brief      Controls the delay (in GEV timestamp counter unit) to insert between each packet for this stream channel. This can be used as a crude flow-control mechanism if the application or the network infrastructure cannot keep up with the packets coming from the device. */
#define SFNC_GEV_SCPD "GevSCPD"
/**     \ingroup    Feature
        \brief      Controls the destination IP address of the selected stream channel to which a GVSP transmitter must send data stream or the destination IP address from which a GVSP receiver may receive data stream. */
#define SFNC_GEV_SCDA "GevSCDA"
/**     \ingroup    Feature
        \brief      Indicates the source port of the stream channel. */
#define SFNC_GEV_SCSP "GevSCSP"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTENTRYSELECTOR "GevManifestEntrySelector"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTXMLMAJORVERSION "GevManifestXMLMajorVersion"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTXMLMINORVERSION "GevManifestXMLMinorVersion"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTXMLSUBMINORVERSION "GevManifestXMLSubMinorVersion"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTSCHEMAMAJORVERSION "GevManifestSchemaMajorVersion"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTSCHEMAMINORVERSION "GevManifestSchemaMinorVersion"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTPRIMARYURL "GevManifestPrimaryURL"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_GEV_MANIFESTSECONDARYURL "GevManifestSecondaryURL"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_CL_CONFIGURATION "ClConfiguration"
/**     \ingroup    Feature
        \brief      Deprecated */
#define SFNC_CL_TIMESLOTSCOUNT "ClTimeSlotsCount"
/**     \ingroup    Feature
        \brief      This device tap geometry feature describes the geometrical properties characterizing the taps of a camera as presented at the output of the device. */
#define SFNC_DEVICETAPGEOMETRY "DeviceTapGeometry"


/**     \ingroup    Feature
        \brief      Category that contains the User Set control features. */
#define SFNC_USERSETCONTROL "UserSetControl"
/**     \ingroup    Feature
        \brief      Selects the feature User Set to load, save or configure. */
#define SFNC_USERSETSELECTOR "UserSetSelector"
/**     \ingroup    Feature
        \brief      Loads the User Set specified by UserSetSelectorto the device and makes it active. */
#define SFNC_USERSETLOAD "UserSetLoad"
/**     \ingroup    Feature
        \brief      Save the User Set specified by UserSetSelectorto the non-volatile memory of the device. */
#define SFNC_USERSETSAVE "UserSetSave"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See UserSetDefault). Selects the feature User Set to load and make active when the device is reset. */
#define SFNC_USERSETDEFAULTSELECTOR "UserSetDefaultSelector"


/**     \ingroup    Feature
        \brief      Category that contains the Chunk Data control features. */
#define SFNC_CHUNKDATACONTROL "ChunkDataControl"
/**     \ingroup    Feature
        \brief      Activates the inclusion of Chunk data in the payload of the image. */
#define SFNC_CHUNKMODEACTIVE "ChunkModeActive"
/**     \ingroup    Feature
        \brief      Selects which Chunk to enable or control. */
#define SFNC_CHUNKSELECTOR "ChunkSelector"
/**     \ingroup    Feature
        \brief      Enables the inclusion of the selected Chunk data in the payload of the image. */
#define SFNC_CHUNKENABLE "ChunkEnable"
/**     \ingroup    Feature
        \brief      Returns the entire image data included in the payload. */
#define SFNC_CHUNKIMAGE "ChunkImage"
/**     \ingroup    Feature
        \brief      Returns the OffsetXof the image included in the payload. */
#define SFNC_CHUNKOFFSETX "ChunkOffsetX"
/**     \ingroup    Feature
        \brief      Returns the OffsetYof the image included in the payload. */
#define SFNC_CHUNKOFFSETY "ChunkOffsetY"
/**     \ingroup    Feature
        \brief      Returns the Widthof the image included in the payload. */
#define SFNC_CHUNKWIDTH "ChunkWidth"
/**     \ingroup    Feature
        \brief      Returns the Heightof the image included in the payload. */
#define SFNC_CHUNKHEIGHT "ChunkHeight"
/**     \ingroup    Feature
        \brief      Returns the PixelFormatof the image included in the payload.  */
#define SFNC_CHUNKPIXELFORMAT "ChunkPixelFormat"
/**     \ingroup    Feature
        \brief      Returns the minimum value of dynamic range of the image included in the payload. */
#define SFNC_CHUNKPIXELDYNAMICRANGEMIN "ChunkPixelDynamicRangeMin"
/**     \ingroup    Feature
        \brief      Returns themaximum value of dynamic range of the image included in the payload. */
#define SFNC_CHUNKPIXELDYNAMICRANGEMAX "ChunkPixelDynamicRangeMax"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKDYNAMICRANGEMIN "ChunkDynamicRangeMin"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKDYNAMICRANGEMAX "ChunkDynamicRangeMax"
/**     \ingroup    Feature
        \brief      Returns the Timestamp of the image included in the payload at the time of the FrameStart internal event. */
#define SFNC_CHUNKTIMESTAMP "ChunkTimestamp"
/**     \ingroup    Feature
        \brief      Returns the status of all the I/O lines at the time of the FrameStart internal event. */
#define SFNC_CHUNKLINESTATUSALL "ChunkLineStatusAll"
/**     \ingroup    Feature
        \brief      Selects which counter to retrieve data from. */
#define SFNC_CHUNKCOUNTERSELECTOR "ChunkCounterSelector"
/**     \ingroup    Feature
        \brief      Returns the value of the selected Chunk counter at the time of the FrameStart event. */
#define SFNC_CHUNKCOUNTERVALUE "ChunkCounterValue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTER "ChunkCounter"
/**     \ingroup    Feature
        \brief      Selects which Timer to retrieve data from. */
#define SFNC_CHUNKTIMERSELECTOR "ChunkTimerSelector"
/**     \ingroup    Feature
        \brief      Returns the value of the selected Timer at the time of the FrameStart internal event.  */
#define SFNC_CHUNKTIMERVALUE "ChunkTimerValue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMER "ChunkTimer"
/**     \ingroup    Feature
        \brief      Returns the exposure time used to capture the image. */
#define SFNC_CHUNKEXPOSURETIME "ChunkExposureTime"
/**     \ingroup    Feature
        \brief      Selects which Gain to return.  */
#define SFNC_CHUNKGAINSELECTOR "ChunkGainSelector"
/**     \ingroup    Feature
        \brief      Returns the gain used to capture the image. */
#define SFNC_CHUNKGAIN "ChunkGain"
/**     \ingroup    Feature
        \brief      Selects which Black Level to return. */
#define SFNC_CHUNKBLACKLEVELSELECTOR "ChunkBlackLevelSelector"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVEL "ChunkBlackLevel"
/**     \ingroup    Feature
        \brief      Returns the LinePitchof the image included in the payload. */
#define SFNC_CHUNKLINEPITCH "ChunkLinePitch"
/**     \ingroup    Feature
        \brief      Returns the unique Identifier of the frame (or image) included in the payload. */
#define SFNC_CHUNKFRAMEID "ChunkFrameID"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBINNINGVERTICALID "ChunkBinningVertical"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBINNINGHORIZONTALID "ChunkBinningHorizontal"


/**     \ingroup    Feature
        \brief      Category that contains the File Access control features. */
#define SFNC_FILEACCESSCONTROL "FileAccessControl"
/**     \ingroup    Feature
        \brief      Selects the target file in the device. */
#define SFNC_FILESELECTOR "FileSelector"
/**     \ingroup    Feature
        \brief      Selects the target operation for the selected file in the device. This Operation is executed when the FileOperationExecutefeature is called. */
#define SFNC_FILEOPERATIONSELECTOR "FileOperationSelector"
/**     \ingroup    Feature
        \brief      Executes the operationselected by FileOperationSelector on the selected file. */
#define SFNC_FILEOPERATIONEXECUTE "FileOperationExecute"
/**     \ingroup    Feature
        \brief      Selects the access mode in which a file is opened in the device. */
#define SFNC_FILEOPENMODE "FileOpenMode"
/**     \ingroup    Feature
        \brief      Defines the intermediate access buffer that allows the exchange of data between the device file storage and the application. */
#define SFNC_FILEACCESSBUFFER "FileAccessBuffer"
/**     \ingroup    Feature
        \brief      Controls the Offset of the mapping between the device file storage and the FileAccessBuffer. */
#define SFNC_FILEACCESSOFFSET "FileAccessOffset"
/**     \ingroup    Feature
        \brief      Controls the Length of the mapping between the device file storage and the FileAccessBuffer. */
#define SFNC_FILEACCESSLENGTH "FileAccessLength"
/**     \ingroup    Feature
        \brief      Represents the file operation execution status.  */
#define SFNC_FILEOPERATIONSTATUS "FileOperationStatus"
/**     \ingroup    Feature
        \brief      Represents the file operation result. For Read or Write operations, the number of successfully read/written bytes is returned */
#define SFNC_FILEOPERATIONRESULT "FileOperationResult"
/**     \ingroup    Feature
        \brief      Represents the size of the selected file in bytes. */
#define SFNC_FILESIZE "FileSize"


/**     \ingroup    Feature
        \brief      Category that contains the Color Transformation control features. */
#define SFNC_COLORTRANSFORMATIONCONTROL "ColorTransformationControl"
/**     \ingroup    Feature
        \brief      Selects which Color Transformation module is controlled by the various Color Transformation features. */
#define SFNC_COLORTRANSFORMATIONSELECTOR "ColorTransformationSelector"
/**     \ingroup    Feature
        \brief      Activates the selected Color Transformation module. */
#define SFNC_COLORTRANSFORMATIONENABLE "ColorTransformationEnable"
/**     \ingroup    Feature
        \brief      Selects the Gain factor or Offset of the Transformation matrix to access in the selected Color Transformation module. */
#define SFNC_COLORTRANSFORMATIONVALUESELECTOR "ColorTransformationValueSelector"
/**     \ingroup    Feature
        \brief      Represents the value of the selected Gain factor or Offset inside the Transformation matrix. */
#define SFNC_COLORTRANSFORMATIONVALUE "ColorTransformationValue"


/**     \ingroup    Feature
        \brief      Category that contains the Action control features. */
#define SFNC_ACTIONCONTROL "ActionControl"
/**     \ingroup    Feature
        \brief      Provides the device key that allows the device to check the validity of action commands. The device internal assertion of an action signal is only authorized if the ActionDeviceKeyand the action device key value in the protocol message are equal. */
#define SFNC_ACTIONDEVICEKEY "ActionDeviceKey"
/**     \ingroup    Feature
        \brief      Selects to which Action Signal further Action settings apply. */
#define SFNC_ACTIONSELECTOR "ActionSelector"
/**     \ingroup    Feature
        \brief      Provides the mask that the device will use to validate the action on reception of the action protocol message. */
#define SFNC_ACTIONGROUPMASK "ActionGroupMask"
/**     \ingroup    Feature
        \brief      Provides the key that the device will use to validate the action on reception of the action protocol message.  */
#define SFNC_ACTIONGROUPKEY "ActionGroupKey"


/**     \ingroup    Feature
        \brief      The GenICam port through which the System module is accessed. */
#define GENTL_SFNC_TLPORT "TLPort"
/**     \ingroup    Feature
        \brief      Name of the GenTL Producer vendor. */
#define GENTL_SFNC_TLVENDORNAME "TLVendorName"
/**     \ingroup    Feature
        \brief      Name of the GenTL Producer to distinguish different kinds of GenTL Producer implementations from one vendor. */
#define GENTL_SFNC_TLMODELNAME "TLModelName"
/**     \ingroup    Feature
        \brief      Unique identifier of the GenTL Producer like a GUID */
#define GENTL_SFNC_TLID "TLID"
/**     \ingroup    Feature
        \brief      Vendor specific version string of the GenTL Producer.   */
#define GENTL_SFNC_TLVERSION "TLVersion"
/**     \ingroup    Feature
        \brief      Full path to the GenTL Producer including filename and extension.   */
#define GENTL_SFNC_TLPATH "TLPath"
/**     \ingroup    Feature
        \brief      Transport layer type of the GenTL Producer implementation. */
#define GENTL_SFNC_TLTYPE "TLType"
/**     \ingroup    Feature
        \brief      Major version number of the GenTL specification the GenTL Producer implementation complies with.  */
#define GENTL_SFNC_GENTLVERSIONMAJOR "GenTLVersionMajor"
/**     \ingroup    Feature
        \brief      Minor version number of the GenTL specification the GenTL Producer implementation complies with. */
#define GENTL_SFNC_GENTLVERSIONMINOR "GenTLVersionMinor"
/**     \ingroup    Feature
        \brief      Updates the internal list of the interfaces. This feature should be readable if the execution cannot performed immediately. The command then returns and the status can be polled. This function interacts with the TLUpdateInterfaceList function of the GenTL Producer. It is up to the GenTL Consumer to handle access in case both methods are used.  */
#define GENTL_SFNC_GENTLINTERFACEUPDATELIST "InterfaceUpdateList"
/**     \ingroup    Feature
        \brief      Selector for the different GenTL Producer interfaces. This interface list only changes on execution of "InterfaceUpdateList". The selector is 0-based in order to match the index of the C interface.  */
#define GENTL_SFNC_GENTLINTERFACESELECTOR "InterfaceSelector"
/**     \ingroup    Feature
        \brief      GenTL Producer wide unique identifier of the selected interface.  */
#define GENTL_SFNC_GENTLINTERFACEID "InterfaceID"


/**     \ingroup    Feature
        \brief      This feature is deprecated (See InterfaceTLVersionMajor). Major version number of the GigE Vision specification the GenTL Producer implementation complies with.  */
#define GENTL_SFNC_GEVVERSIONMAJOR "GevVersionMajor"
/**     \ingroup    Feature
        \brief      This feature is deprecated (See InterfaceTLVersionMinor). Minor version number of the GigE Vision specification the GenTL Producer implementation complies with.  */
#define GENTL_SFNC_GEVVERSIONMINOR "GevVersionMinor"
/**     \ingroup    Feature
        \brief      48-bit MAC address of the selected interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.  */
#define GENTL_SFNC_GEVINTERFACEMACADDRESS "GevInterfaceMACAddress"
/**     \ingroup    Feature
        \brief      IP address of the first subnet of the selected interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.   */
#define GENTL_SFNC_GEVINTERFACEDEFAULTIPADDRESS "GevInterfaceDefaultIPAddress"
/**     \ingroup    Feature
        \brief      Subnet mask of the first subnet of the selected interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.   */
#define GENTL_SFNC_GEVINTERFACEDEFAULTSUBNETMASK "GevInterfaceDefaultSubnetMask"
/**     \ingroup    Feature
        \brief      Gateway of the selected interface. */
#define GENTL_SFNC_GEVINTERFACEDEFAULTGATEWAY "GevInterfaceDefaultGateway"


/**     \ingroup    Feature
        \brief      The GenICam port through which the Interface module is accessed.  */
#define GENTL_SFNC_INTERFACEPORT "InterfacePort"
/**     \ingroup    Feature
        \brief      GenTL Producer wide unique identifier of the selected interface. */
#define GENTL_SFNC_INTERFACEID "InterfaceID"
/**     \ingroup    Feature
        \brief      Transport layer type of the interface.  */
#define GENTL_SFNC_INTERFACETYPE "InterfaceType"
/**     \ingroup    Feature
        \brief      Updates the internal device list. This feature should be readable if the execution cannot be performed immediately. The command then returns and the status can be polled. This feature interacts with the IFUpdateDeviceList function of the GenTL Producer. It is up to the GenTL Consumer to handle access in case both methods are used.   */
#define GENTL_SFNC_DEVICEUPDATELIST "DeviceUpdateList"
/**     \ingroup    Feature
        \brief      Selector for the different devices on this interface. This value only changes on execution of "DeviceUpdateList". The selector is 0-based in order to match the index of the C interface.   */
#define GENTL_SFNC_DEVICESELECTOR "DeviceSelector"
/**     \ingroup    Feature
        \brief      Interface wide unique identifier of the selected device. This value only changes on execution of the DeviceUpdateList command. */
#define GENTL_SFNC_DEVICEID "DeviceID"
/**     \ingroup    Feature
        \brief      Name of the device vendor. This value only changes on execution of the DeviceUpdateList command. */
#define GENTL_SFNC_DEVICEVENDORNAME "DeviceVendorName"
/**     \ingroup    Feature
        \brief      Name of the device model. This value only changes on execution of the DeviceUpdateList command. */
#define GENTL_SFNC_DEVICEMODELNAME "DeviceModelName"
/**     \ingroup    Feature
        \brief      Gives the device's access status at the moment of the last execution of the DeviceUpdateList command. This value only changes on execution of the DeviceUpdateList command.  */
#define GENTL_SFNC_DEVICEACCESSSTATUS "DeviceAccessStatus"


/**     \ingroup    Feature
        \brief      Selector for the different gateway entries for this interface. The selector is 0-based. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.  */
#define GENTL_SFNC_GEVINTERFACEGATEWAYSELECTOR "GevInterfaceGatewaySelector"
/**     \ingroup    Feature
        \brief      IP address of the selected gateway entry of this interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.  */
#define GENTL_SFNC_GEVINTERFACEGATEWAY "GevInterfaceGateway"
/**     \ingroup    Feature
        \brief      48-bit MAC address of this interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.  */
#define GENTL_SFNC_GEVINTERFACEMACADDRESS "GevInterfaceMACAddress"
/**     \ingroup    Feature
        \brief      Selector for the subnet of this interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.  */
#define GENTL_SFNC_GEVINTERFACESUBNETSELECTOR "GevInterfaceSubnetSelector"
/**     \ingroup    Feature
        \brief      IP address of the selected subnet of this interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory. */
#define GENTL_SFNC_GEVINTERFACESUBNETIPADDRESS "GevInterfaceSubnetIPAddress"
/**     \ingroup    Feature
        \brief      Subnet mask of the selected subnet of this interface. Note that for a GenTL Producer implementation supporting GigE Vision this feature is mandatory.  */
#define GENTL_SFNC_GEVINTERFACESUBNETMASK "GevInterfaceSubnetMask"


/**     \ingroup    Feature
        \brief      The GenICam port through which the Device module is accessed.Note that DevicePort is a port node (not a feature node) and is generally not accessed by the end user directly. */
#define GENTL_SFNC_DEVICEPORT "DevicePort"
/**     \ingroup    Feature
        \brief      Interface wide unique identifier of the selected device. */
#define GENTL_SFNC_DEVICEID "DeviceID"
/**     \ingroup    Feature
        \brief      Name of the device vendor. */
#define GENTL_SFNC_DEVICEVENDORNAME "DeviceVendorName"
/**     \ingroup    Feature
        \brief      Name of the device model. */
#define GENTL_SFNC_DEVICEMODELNAME "DeviceModelName"
/**     \ingroup    Feature
        \brief      Transport layer type of the device.  */
#define GENTL_SFNC_DEVICETYPE "DeviceType"
/**     \ingroup    Feature
        \brief      Selector for the different stream channels. */
#define GENTL_SFNC_STREAMSELECTOR "StreamSelector"
/**     \ingroup    Feature
        \brief      Device unique ID for the stream. */
#define GENTL_SFNC_STREAMID "StreamID"


/**     \ingroup    Feature
        \brief      Current IP address of the GVCP interface of the selected remote device. */
#define GENTL_SFNC_GEVDEVICEIPADDRESS "GevDeviceIPAddress"
/**     \ingroup    Feature
        \brief      Current subnet mask of the GVCP interface of the selected remote device. */
#define GENTL_SFNC_GEVDEVICESUBNETMASK "GevDeviceSubnetMask"
/**     \ingroup    Feature
        \brief      48-bit MAC address of the GVCP interface of the selected remote device.  */
#define GENTL_SFNC_GEVDEVICEMACADDRESS "GevDeviceMACAddress"
/**     \ingroup    Feature
        \brief      Current gateway IP address of the GVCP interface of the selected remote device.  */
#define GENTL_SFNC_GEVDEVICEGATEWAY "GevDeviceGateway"
/**     \ingroup    Feature
        \brief      Identifies the endianess handling mode. */
#define GENTL_SFNC_DEVICEENDIANESSMECHANISM "DeviceEndianessMechanism"


/**     \ingroup    Feature
        \brief      The GenICam port through which the Data Stream module is accessed. */
#define GENTL_SFNC_STREAMPORT "StreamPort"
/**     \ingroup    Feature
        \brief      Device unique ID for the stream. */
#define GENTL_SFNC_STREAMID "StreamID"
/**     \ingroup    Feature
        \brief      Number of announced (known) buffers on this stream. This value is volatile. It may change if additional buffers are announced and/or buffers are revoked by the GenTL Consumer. */
#define GENTL_SFNC_STREAMANNOUNCEDBUFFERCOUNT "StreamAnnouncedBufferCount"
/**     \ingroup    Feature
        \brief       */
#define GENTL_SFNC_STREAMACQUISITIONMODESELECTOR "StreamAcquisitionModeSelector"
/**     \ingroup    Feature
        \brief      Minimal number of buffers to announce to enable selected buffer handling mode. */
#define GENTL_SFNC_STREAMANNOUNCEBUFFERMINIMUM "StreamAnnounceBufferMinimum"
/**     \ingroup    Feature
        \brief      Transport layer type of the Data Stream. */
#define GENTL_SFNC_STREAMTYPE "StreamType"


/**     \ingroup    Feature
        \brief      The GenICam port through which the Buffer module is accessed. */
#define GENTL_SFNC_BUFFERPORT "BufferPort"
/**     \ingroup    Feature
        \brief      Entire buffer data. */
#define GENTL_SFNC_BUFFERDATA "BufferData"
/**     \ingroup    Feature
        \brief      Pointer to user data casted to an integer number referencing GenTL Consumer specific data. It is reflecting the pointer provided by the user data pointer at buffer announcement.This allows the GenTL Consumer to attach information to a buffer.  */
#define GENTL_SFNC_BUFFERUSERDATA "BufferUserData"
/**     \ingroup    Feature
        \brief       */
#define GENTL_SFNC_BUFFER_CUSTOM_HOSTTIMESTAMP "HostTimestamp"


// selector values
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_IMAGE "Image"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_OFFSETX "OffsetX"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_OFFSETY "OffsetY"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_WIDTH "Width"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_HEIGHT "Height"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_PIXELFORMAT "PixelFormat"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_DYNAMICRANGEMAX "DynamicRangeMax"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_DYNAMICRANGEMIN "DynamicRangeMin"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_PIXELDYNAMICRANGEMAX "PixelDynamicRangeMax"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_PIXELDYNAMICRANGEMIN "PixelDynamicRangeMin"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_TIMESTAMP "Timestamp"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_LINESTATUSALL "LineStatusAll"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_COUNTERVALUE "CounterValue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_TIMERVALUE "TimerValue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_EXPOSURETIME "ExposureTime"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_GAIN "Gain"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_BLACKLEVEL "BlackLevel"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_LINEPITCH "LinePitch"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKSELECTORVALUE_FRAMEID "FrameID"


/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICE_TEMERATURESELECTORVALUE_SENSOR "Sensor"
/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICE_TEMERATURESELECTORVALUE_MAINBOARD "Mainboard"


/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICE_CLOCKSELECTORVALUE_SENSOR "Sensor"
/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICE_CLOCKSELECTORVALUE_SENSORDIGITIZATION "SensorDigitization"
/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICE_CLOCKSELECTORVALUE_CAMERALINK "CameraLink"


/**     \ingroup    Feature
        \brief       */
#define SFNC_DEVICE_SERIALPORTSELECTORVALUE_CAMERALINK "CameraLink"


/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_OFF "Off"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_BLACK "Black"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_WHITE "White"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_GREYHORIZONTALRAMP "GreyHorizontalRamp"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_GREYVERTICALRAMP "GreyVerticalRamp"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_GREYHORIZONTALRAMPMOVING "GreyHorizontalRampMoving"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_GREYVERTICALRAMPMOVING "GreyVerticalRampMoving"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_HORIZONTALLINEMOVING "HorzontalLineMoving"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_VERTICALLINEMOVING "VerticalLineMoving"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_COLORBAR "ColorBar"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TESTIMAGESELECTORVALUE_FRAMECOUNTER "FrameCounter"


/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_ACQUISITIONTRIGGERWAIT "AcquisitionTriggerWait"
/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_ACQUISITIONACTIVE "AcquisitionActive"
/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_ACQUISITIONTRANSFER "AcquisitionTransfer"
/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_FRAMETRIGGERWAIT "FrameTriggerWait"
/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_FRAMEACTIVE "FrameActive"
/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_FRAMETRANSFER "FrameTransfer"
/**     \ingroup    Feature
        \brief       */
#define SFNC_ACQUISITION_STATUSSELECTORVALUE_EXPOSUREACTIVE "ExposureActive"


/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_ACQUISITIONSTART "AcquisitionStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_ACQUISITIONEND "AcquisitionEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_ACQUISITIONACTIVE "AcquisitionActive"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_FRAMESTART "FrameStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_FRAMEEND "FrameEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_FRAMEACTIVE "FrameActive"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_FRAMEBURSTSTART "FrameBurstStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_FRAMEBURSTEND "FrameBurstEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_FRAMEBURSTACTIVE "FrameBurstActive"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_LINESTART "LineStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_EXPOSURESTART "ExposureStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_EXPOSUREEND "ExposureEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TRIGGERSELECTORVALUE_EXPOSUREACTIVE "ExposureActive"


/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE0 "Line0"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE1 "Line1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE2 "Line2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE3 "Line3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE4 "Line4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE5 "Line5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE6 "Line6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_LINE7 "Line7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_CC1 "CC1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_CC2 "CC2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_CC3 "CC3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LINESELECTORVALUE_CC4 "CC4"


/**     \ingroup    Feature
        \brief       */
#define SFNC_USEROUTPUTSELECTORVALUE_USEROUTPUT0 "UserOutput0"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USEROUTPUTSELECTORVALUE_USEROUTPUT1 "UserOutput1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USEROUTPUTSELECTORVALUE_USEROUTPUT2 "UserOutput2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USEROUTPUTSELECTORVALUE_USEROUTPUT3 "UserOutput3"


/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER1 "Counter1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER2 "Counter2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER3 "Counter3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER4 "Counter4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER5 "Counter5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER6 "Counter6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER7 "Counter7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COUNTERSELECTORVALUE_COUNTER8 "Counter8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER1 "Timer1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER2 "Timer2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER3 "Timer3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER4 "Timer4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER5 "Timer5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER6 "Timer6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER7 "Timer7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_TIMERSELECTORVALUE_TIMER8 "Timer8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ACQUISITIONTRIGGER "AcquisitionTrigger"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ACQUISITIONSTART "AcquisitionStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ACQUISITIONEND "AcquisitionEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ACQUISITIONTRANSFERSTART "AcquisitionTransferStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ACQUISITIONTRANSFEREND "AcquisitionTransferEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ACQUISITIONERROR "AcquisitionError"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMETRIGGER "FrameTrigger"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMESTART "FrameStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMEEND "FrameEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMEBURSTSTART "FrameBurstStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMEBURSTEND "FrameBurstEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMETRANSFERSTART "FrameTransferStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_FRAMETRANSFEREND "FrameTransferEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_EXPOSURESTART "ExposureStart"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_EXPOSUREEND "ExposureEnd"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER1START "Counter1Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER2START "Counter2Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER3START "Counter3Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER4START "Counter4Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER5START "Counter5Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER6START "Counter6Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER7START "Counter7Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER8START "Counter8Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER1END "Counter1End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER2END "Counter2End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER3END "Counter3End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER4END "Counter4End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER5END "Counter5End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER6END "Counter6End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER7END "Counter7End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_COUNTER8END "Counter8End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER1START "Timer1Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER2START "Timer2Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER3START "Timer3Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER4START "Timer4Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER5START "Timer5Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER6START "Timer6Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER7START "Timer7Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER8START "Timer8Start"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER1END "Timer1End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER2END "Timer2End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER3END "Timer3End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER4END "Timer4End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER5END "Timer5End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER6END "Timer6End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER7END "Timer7End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_TIMER8END "Timer8End"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE0RISINGEDGE "Line0RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE1RISINGEDGE "Line1RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE2RISINGEDGE "Line2RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE3RISINGEDGE "Line3RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE4RISINGEDGE "Line4RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE5RISINGEDGE "Line5RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE6RISINGEDGE "Line6RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE7RISINGEDGE "Line7RisingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE0FALLINGEDGE "Line0FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE1FALLINGEDGE "Line1FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE2FALLINGEDGE "Line2FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE3FALLINGEDGE "Line3FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE4FALLINGEDGE "Line4FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE5FALLINGEDGE "Line5FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE6FALLINGEDGE "Line6FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE7FALLINGEDGE "Line7FallingEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE0ANYEDGE "Line0AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE1ANYEDGE "Line1AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE2ANYEDGE "Line2AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE3ANYEDGE "Line3AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE4ANYEDGE "Line4AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE5ANYEDGE "Line5AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE6ANYEDGE "Line6AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_LINE7ANYEDGE "Line7AnyEdge"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ERROR "Error"
/**     \ingroup    Feature
        \brief       */
#define SFNC_EVENTSELECTORVALUE_ERRORS "Errors"


/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ALL "All"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_BLUE "Blue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_Y "Y"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_U "U"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_V "V"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP1 "Tap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP2 "Tap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP3 "Tap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP4 "Tap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP5 "Tap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP6 "Tap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP7 "Tap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_TAP8 "Tap8"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGALL "AnalogAll"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGRED "AnalogRed"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGGREEN "AnalogGreen"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGBLUE "AnalogBlue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGY "AnalogY"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGU "AnalogU"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGV "AnalogV"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP1 "AnalogTap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP2 "AnalogTap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP3 "AnalogTap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP4 "AnalogTap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP5 "AnalogTap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP6 "AnalogTap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP7 "AnalogTap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_ANALOGTAP8 "AnalogTap8"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALALL "DigitalAll"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALRED "DigitalRed"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALGREEN "DigitalGreen"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALBLUE "DigitalBlue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALY "DigitalY"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALU "DigitalU"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALV "DigitalV"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP1 "DigitalTap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP2 "DigitalTap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP3 "DigitalTap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP4 "DigitalTap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP5 "DigitalTap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP6 "DigitalTap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP7 "DigitalTap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GAINSELECTORVALUE_DIGITALTAP8 "DigitalTap8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_ALL "All"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_BLUE "Blue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_Y "Y"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_U "U"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_V "V"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP1 "Tap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP2 "Tap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP3 "Tap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP4 "Tap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP5 "Tap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP6 "Tap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP7 "Tap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BLACKLEVELSELECTORVALUE_TAP8 "Tap8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_ALL "All"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_BLUE "Blue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_Y "Y"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_U "U"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_V "V"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP1 "Tap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP2 "Tap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP3 "Tap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP4 "Tap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP5 "Tap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP6 "Tap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP7 "Tap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_WHITECLIPSELECTORVALUE_TAP8 "Tap8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_ALL "All"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_BLUE "Blue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_Y "Y"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_U "U"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_V "V"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP1 "Tap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP2 "Tap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP3 "Tap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP4 "Tap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP5 "Tap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP6 "Tap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP7 "Tap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_BALANCERATIOSELECTORVALUE_TAP8 "Tap8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_LUTSELECTORVALUE_LUMINANCE "Luminance"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LUTSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LUTSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_LUTSELECTORVALUE_BLUE "Blue"


/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_IPCONFIGURATIONLLA "IPConfigurationLLA"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_IPCONFIGURATIONDHCP "IPConfigurationDHCP"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_IPCONFIGURATIONPERSISTENTIP "IPConfigurationPersistentIP"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNELSOURCESOCKET "StreamChannelSourceSocket"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_MESSAGECHANNELSOURCESOCKET "MessageChannelSourceSocket"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_COMMANDSCONCATENATION "CommandsConcatenation"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_WRITEMEM "WriteMem"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_PACKETRESEND "PacketResend"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_EVENT "Event"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_EVENTDATA "EventData"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_PENDINGACK "PendingAck"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_ACTION "Action"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_PRIMARYAPPLICATIONSWITCHOVER "PrimaryApplicationSwitchover"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_EXTENDEDSTATUSCODES "ExtendedStatusCodes"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_DISCOVERYACKDELAY "DiscoveryAckDelay"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_DISCOVERYACKDELAYWRITABLE "DiscoveryAckDelayWritable"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_TESTDATA "TestData"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_MANIFESTTABLE "ManifestTable"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_CCPAPPLICATIONSOCKET "CCPApplicationSocket"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_LINKSPEED "LinkSpeed"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_HEARTBEATDISABLE "HeartbeatDisable"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_SERIALNUMBER "SerialNumber"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_USERDEFINEDNAME "UserDefinedName"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL0BIGANDLITTLEENDIAN "StreamChannel0BigAndLittleEndian"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL0IPREASSEMBLY "StreamChannel0IPReassembly"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL0UNCONDITIONALSTREAMING \
"StreamChannel0UnconditionalStreaming"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL0EXTENDEDCHUNKDATA "StreamChannel0ExtendedChunkData"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL1BIGANDLITTLEENDIAN "StreamChannel1BigAndLittleEndian"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL1IPREASSEMBLY "StreamChannel1IPReassembly"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL1UNCONDITIONALSTREAMING \
"StreamChannel1UnconditionalStreaming"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL1EXTENDEDCHUNKDATA "StreamChannel1ExtendedChunkData"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL2BIGANDLITTLEENDIAN "StreamChannel2BigAndLittleEndian"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL2IPREASSEMBLY "StreamChannel2IPReassembly"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL2UNCONDITIONALSTREAMING \
"StreamChannel2UnconditionalStreaming"
/**     \ingroup    Feature
        \brief       */
#define SFNC_GEV_SUPPORTEDOPTIONSELECTORVALUE_STREAMCHANNEL2EXTENDEDCHUNKDATA "StreamChannel2ExtendedChunkData"


/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETSELECTORVALUE_DEFAULT "Default"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETSELECTORVALUE_USERSET1 "UserSet1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETSELECTORVALUE_USERSET2 "UserSet2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETSELECTORVALUE_USERSET3 "UserSet3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETSELECTORVALUE_USERSET4 "UserSet4"


/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETDEFAULTSELECTORVALUE_DEFAULT "Default"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETDEFAULTSELECTORVALUE_USERSET1 "UserSet1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETDEFAULTSELECTORVALUE_USERSET2 "UserSet2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETDEFAULTSELECTORVALUE_USERSET3 "UserSet3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_USERSETDEFAULTSELECTORVALUE_USERSET4 "UserSet4"


/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER1 "Counter1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER2 "Counter2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER3 "Counter3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER4 "Counter4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER5 "Counter5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER6 "Counter6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER7 "Counter7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKCOUNTERSELECTORVALUE_COUNTER8 "Counter8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER1 "Timer1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER2 "Timer2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER3 "Timer3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER4 "Timer4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER5 "Timer5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER6 "Timer6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER7 "Timer7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKTIMERSELECTORVALUE_TIMER8 "Timer8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ALL "All"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_BLUE "Blue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_Y "Y"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_U "U"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_V "V"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP1 "Tap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP2 "Tap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP3 "Tap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP4 "Tap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP5 "Tap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP6 "Tap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP7 "Tap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_TAP8 "Tap8"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGALL "AnalogAll"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGRED "AnalogRed"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGGREEN "AnalogGreen"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGBLUE "AnalogBlue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGY "AnalogY"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGU "AnalogU"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGV "AnalogV"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP1 "AnalogTap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP2 "AnalogTap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP3 "AnalogTap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP4 "AnalogTap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP5 "AnalogTap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP6 "AnalogTap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP7 "AnalogTap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_ANALOGTAP8 "AnalogTap8"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALALL "DigitalAll"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALRED "DigitalRed"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALGREEN "DigitalGreen"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALBLUE "DigitalBlue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALY "DigitalY"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALU "DigitalU"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALV "DigitalV"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP1 "DigitalTap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP2 "DigitalTap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP3 "DigitalTap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP4 "DigitalTap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP5 "DigitalTap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP6 "DigitalTap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP7 "DigitalTap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKGAINSELECTORVALUE_DIGITALTAP8 "DigitalTap8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_ALL "All"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_RED "Red"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_GREEN "Green"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_BLUE "Blue"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_Y "Y"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_U "U"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_V "V"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP1 "Tap1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP2 "Tap2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP3 "Tap3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP4 "Tap4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP5 "Tap5"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP6 "Tap6"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP7 "Tap7"
/**     \ingroup    Feature
        \brief       */
#define SFNC_CHUNKBLACKLEVELSELECTORVALUE_TAP8 "Tap8"


/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_USERSETDEFAULT "UserSetDefault"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_USERSET1 "UserSet1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_USERSET2 "UserSet2"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_USERSET3 "UserSet3"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_USERSET4 "UserSet4"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_LUTLUMINANCE "LUTLuminance"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_LUTRED "LUTRed"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_LUTGREEN "LUTGreen"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILESELECTORVALUE_LUTBLUE "LUTBlue"


/**     \ingroup    Feature
        \brief       */
#define SFNC_FILEOPERATIONSELECTORVALUE_OPEN "Open"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILEOPERATIONSELECTORVALUE_CLOSE "Close"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILEOPERATIONSELECTORVALUE_READ "Read"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILEOPERATIONSELECTORVALUE_WRITE "Write"
/**     \ingroup    Feature
        \brief       */
#define SFNC_FILEOPERATIONSELECTORVALUE_DELETE "Delete"


/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONSELECTORVALUE_RGBTORGB "RGBtoRGB"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONSELECTORVALUE_RGBTOYUV "RGBtoYUV"


/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain00 "Gain00"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain01 "Gain01"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain02 "Gain02"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain10 "Gain10"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain11 "Gain11"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain12 "Gain12"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain20 "Gain20"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain21 "Gain21"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Gain22 "Gain22"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Offset0 "Offset0"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Offset1 "Offset1"
/**     \ingroup    Feature
        \brief       */
#define SFNC_COLORTRANSFORMATIONVALUESELECTORVALUE_Offset2 "Offset2"

#endif  // TLCONSUMER_BGAPI2_GENICAM_BGAPI2_FEATURENAMES_H_
