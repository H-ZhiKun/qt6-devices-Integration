/* Copyright 2014-2019 Baumer Optronic */

#ifndef _BGAPI2_BRIGHTNESS_AUTO_HPP__
#define _BGAPI2_BRIGHTNESS_AUTO_HPP__

//! @cond suppress_docu
#include <ctime>
#include "./bgapi2_def.h"
//! @endcond

/**
\defgroup MainClasses Main Classes
\brief This group defined the main classes of BGAPI2. These classes represent the fundamental logical and physical
components of the image processing system.
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

/**
\ingroup FunctionalExtensions
\brief The class BrightnessAuto offers a automatic brightness correction on the camera.
*/

class BGAPI2_DECL BrightnessAuto {
 public:
        /**
         * \brief An enumeration to control the auto functions activation mode.
         */
        enum ActivationMode {
            /**
            \brief Automatic features runs only once, until the nominal value is reached or the abort condition applies.
            */
            ONCE,
            /**
            \brief Automatic features runs continuous until the auto feature is deactivated manually.
            */
            CONTINUOUS
        };

        /**
         * \brief An enumeration to select the algorithm of the auto feature.
         */
        enum AutoAlgorithm {
            /**
            \brief Selects the algorithm used by the hardware device.
            */
            HARDWARE,
            /**
            \brief Selects the software algorithm.
            */
            SOFTWARE,
        };

        /**
         * \brief An enumeration to select the algorithm for the auto feature.
         */
        enum ControlFeature {
            /**
            \brief Selects only exposure time feature.
            */
            EXPOSURE_ONLY,
            /**
            \brief Selects only gain feature.
            */
            GAIN_ONLY,

            /**
            \brief Selects the exposure and the gain feature. Exposure is controlled before gain.
            */
            EXPOSURE_PRIO,

            /**
            \brief Selects the gain and the exposure feature. Gain is controlled before exposure.
            */
            GAIN_PRIO
        };

        /**
         * \brief An enumeration to select the algorithm for the auto feature.
         */
        enum BrightnessAutoState {
            /**
            \brief The auto brightness algorithm has not run yet.
            */
            DIDNOT_RUN,
            /**
            \brief The auto brightness algorithm runs currently in once mode.
            */
            RUNNING_ONCE,
            /**
            \brief The auto brightness algorithm runs currently in continuous mode.
            */
            RUNNING_CONTINUOUS,
            /**
            \brief The auto brightness algorithm is stopped. The brightness value has reached the nominal value.
            */
            STOPPED_REACHED,
            /**
            \brief The auto brightness algorithm is stopped. The brightness value has not reached the nominal value.
            */
            STOPPED_NOT_REACHED,
            /**
            \brief The auto brightness algorithm is stopped. An error has occurred which prevents further execution.
            */
            STOPPED_ERROR
        };

        /**
         * \brief A structure which holds a region.
         */
        typedef struct Region {
            /**
            \brief Defines the x coordinate of the left upper point of the region.
            */
            bo_uint offset_x;
            /**
            \brief Defines the y coordinate of the left upper point of the region.
            */
            bo_uint offset_y;
            /**
            \brief Defines the width of the region.
            */
            bo_uint width;
            /**
            \brief Defines the height of the region.
            */
            bo_uint height;
        } Region;

        /**
         * \brief     Constructs an BrightnessAuto object. This class is used by the Device class and should not be used
         * directly. \brief     If used directly all function calls throw the Exceptions::ObjectInvalidException.
         */
        BrightnessAuto();

        /**
         * \brief     Destroy an BrightnessAuto object.
         */
        virtual ~BrightnessAuto();

        /**
         * \brief     Activates the auto brightness functionality by using exposure and gain as control features with
         * \brief     exposure as priority featured. This function enables the auto brightness functionality on the
         * \brief     camera if supported, if not the software will do it instead.
         * \param[in] mode The activation mode to enable once or continuous.
         * \exception Exceptions::NotAvailableException     The device object does not support any data streams.
         * \exception Exceptions::NotInitializedException   The device object is not open.
         * \exception Exceptions::InvalidParameterException The passed parameter is no valid ActivationMode.
         * \exception Exceptions::ObjectInvalidException    The calling object is not valid. E.g. the object was created
         *                                                  by a direct constructor call.
         */
        void SetMode(ActivationMode mode);

        /**
         * \brief     Activates the auto brightness feature and set how the algorithms handles the control features.
         * \brief     This function enables the auto brightness feature on the camera if supported,
         * \brief     otherwise it is done by software.
         * \param[in] mode The activation mode to enable once or continuous.
         * \param[in] control_feature Selects the strategy how exposure and gain are handled by the algorithm.
         * \exception Exceptions::NotAvailableException     The device object does not support any data streams.
         * \exception Exceptions::NotInitializedException   The device object is not open.
         * \exception Exceptions::InvalidParameterException The passed parameter is no valid ActivationMode.
         * \exception Exceptions::ObjectInvalidException    The calling object is not valid. E.g. the object was created by
         *                                                  a direct constructor call.
         */
        void SetMode(ActivationMode mode, ControlFeature control_feature);

        /**
         * \brief     Activates the auto brightness feature and set how the algorithms handles the control features.
         * \brief     Additionally selects the used algorithm.
         * \param[in] mode The activation mode to enable once or continuous.
         * \param[in] control_feature Selects the strategy how exposure and gain are handled by the algorithm.
         * \param[in] algo_priority Selects the auto feature algorithm.
         * \exception Exceptions::NotAvailableException     The device object does not support any data streams.
         * \exception Exceptions::NotInitializedException   The device object is not open.
         * \exception Exceptions::InvalidParameterException The passed parameter is no valid ActivationMode.
         * \exception Exceptions::ObjectInvalidException    The calling object is not valid. E.g. the object was created by
         *                                                  a direct constructor call.
         */
        void SetMode(ActivationMode mode, ControlFeature control_feature, AutoAlgorithm algorithm);

       /**
        * \brief     Deactivates the auto brightness feature.
        * \exception Exceptions::NotInitializedException   The device object is not open.
        * \exception Exceptions::ObjectInvalidException    The calling object is not valid. E.g. the object was created by
        *                                                  a direct constructor call.
        */
        void SetOff();

        /**
         * \brief     Selects the region of interest for the auto brightness feature.
         * \brief     The specified region will automatically shrunk, if the device region will be changed. In this case,
         * please use function GetAutoBrightnessState to get the current used region. \param[in] region Defines the region.
         * \exception Exceptions::NotAvailableException The hardware algorithm does not support the region setting.
         * \exception Exceptions::NotAvailableException The device object does not support any data streams.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::InvalidParameterException The auto brightness region does not fit into the device region.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void SetRegion(Region region);

        /**
         * \brief     Sets nominal value for the auto brightness control.
         * \param[in] nominal_value Selects the nominal value in percent.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::InvalidParameterException The parameter should be in a range between 0 and 100.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void SetNominalValue(bo_double nominal_value);

        /**
         * \brief     Sets all values to default.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void Default();

        /**
         * \brief     Sets the maximum value of the exposure time used by the algorithm.
         * \param[in] max_exposure_time The maximum exposure time used by the algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::InvalidParameterException The parameter is out of range. Please refer to the corresponding
         * device feature which offers the possible range.
         * \exception Exceptions::NotAvailableException The device object does not support exposure.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void SetExposureMaxValue(bo_double max_exposure_time);

        /**
         * \brief     Sets the minimum value of the exposure time used by the algorithm.
         * \param[in] min_exposure_time The minimum exposure time used by the algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::InvalidParameterException The parameter is out of range. Please refer to the corresponding
         * device feature which offers the possible range.
         * \exception Exceptions::NotAvailableException The device object does not support exposure.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void SetExposureMinValue(bo_double min_exposure_time);

        /**
         * \brief     Sets the maximum value of the gain used by the algorithm.
         * \param[in] max_gain The maximum gain used by the algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::InvalidParameterException The parameter is out of range. Please refer to the corresponding
         * device feature which offers the possible range.
         * \exception Exceptions::NotAvailableException The device object does not support gain all.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void SetGainMaxValue(bo_double max_gain);

        /**
         * \brief     Sets the minimum value of the gain used by the algorithm.
         * \param[in] min_gain The minimum gain used by the algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::InvalidParameterException The parameter is out of range. Please refer to the corresponding
         * device feature which offers the possible range.
         * \exception Exceptions::NotAvailableException The device object does not support gain all.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        void SetGainMinValue(bo_double min_gain);

        /**
         * \brief     Returns the state of the auto brightness control.
         * \return    The current state of the auto brightness control.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        BrightnessAutoState GetState();

        /**
         * \brief     Returns the feature strategy used by the auto brightness algorithm.
         * \return    The feature which has the highest priority for the brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        ControlFeature GetControlFeature();

        /**
         * \brief     Returns the algorithm which is used for the auto brightness control.
         * \return    The algorithm which is used for the auto brightness control.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        AutoAlgorithm GetAlgorithm();

        /**
         * \brief     Returns a flag which indicates, if the auto brightness algorithm is used the exposure time as control
         *            feature.
         * \return    A flag which indicates, if the auto brightness algorithm is used the exposure time as control
         *            feature.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was
         *            created by a direct constructor call.
         */
        bo_bool IsExposureEnabled();

        /**
         * \brief     Returns a flag which indicates, if the auto brightness algorithm is used the gain as control feature.
         * \return    A flag which indicates, if the auto brightness algorithm is used the gain as control feature.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         *            direct constructor call.
         */
        bo_bool IsGainEnabled();

        /**
         * \brief     Returns the region used by the auto brightness algorithm.
         * \return    The region used by the auto brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         *            direct constructor call.
         */
        Region GetRegion();

        /**
         * \brief     Returns the nominal value of the auto brightness algorithm.
         * \return    The nominal value of the auto brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        bo_double GetNominalValue();

        /**
         * \brief     Returns the current maximum exposure time used by the auto brightness algorithm.
         * \return    The current maximum exposure time used by the auto brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::NotAvailableException The device object does not support exposure.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        bo_double GetExposureMaxValue();

        /**
         * \brief     Returns the current minimum exposure time used by the auto brightness algorithm.
         * \return    The current minimum exposure time used by the auto brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::NotAvailableException The device object does not support exposure.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        bo_double GetExposureMinValue();

        /**
         * \brief     Returns the current maximum gain used by the auto brightness algorithm.
         * \return    The current maximum gain used by the auto brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::NotAvailableException The device object does not support gain all.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        bo_double GetGainMaxValue();

        /**
         * \brief     Returns the current minimum gain used by the auto brightness algorithm.
         * \return    The current minimum gain used by the auto brightness algorithm.
         * \exception Exceptions::NotInitializedException The device object is not open.
         * \exception Exceptions::NotAvailableException The device object does not support gain all.
         * \exception Exceptions::ObjectInvalidException The calling object is not valid. E.g. the object was created by a
         * direct constructor call.
         */
        bo_double GetGainMinValue();

    /**
     * \fn        GetReserved
     * \brief     Undocumented function.
     */
    void *GetReserved();

 private:
    tBrightnessAutoData *m_pBrightnessAutoData;
};
}  // namespace BGAPI2

#endif  // _BGAPI2_BRIGHTNESS_AUTO_HPP__
