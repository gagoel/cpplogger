/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_config.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed Apr 24 17:37:30 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file declares LoggerConfig class. 
 LoggerConfig handles all low level I/O operations from configuration file and 
 provides getter and setter to handle configuration.
 
 This class can not be used directly, it needs to inherit before use. 
 This class methods are only used by cpplogger and handlers.

 **/

#ifndef CPPUTILS_CPPLOGGER_CONFIG_H_
#define CPPUTILS_CPPLOGGER_CONFIG_H_

#include <cstring>
#include <cctype>

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "cpplogger/cpplogger_logging.h"
#include "cpplogger/cpplogger_utils.h"

namespace cpputils
{
namespace cpplogger
{

// @class
// This class handles all the configuration settings used for cpplogger.
// CPPLogger uses file to store configuration settings. Configuration file
// path can be change while compiling the CPPLogger. Default configuration
// file name is $prefix/share/cpplogger/cpplogger.conf
// Configuration file name can also be changed with an API "SetConfigFileName"
// before initialization call. It will be valid only within the current session
// (valid till CleanUp method was not called.). If you need to change the
// configuration file name with active session, call CleanUp method before it
// and Init method after it.
// 
// (document here if you are adding new configuration setting)
//
// CPPLOGGER global configuration settings : 
// SetLogLevel  package_name  -  LogLevel (FINE, FINER FINEST)
// AddHandler   handler_name  -  handler_libpath
//
// Handlers can register new property by calling method 
// RegisterProperty(property_name, call_back)
// Callback method is used to validate property value before storing it into
// cache. For successful format it should return true.
// Similar it has unregister method to remove property. 

class LoggerConfig
{
    private :
        
        static LoggerLogging logger; 
        static bool initialized;
        static std::map<std::string, LoggerConfig*>* app_name_config_obj_map;

        // @type
        // This log level is used only by this class.
        // To get and set log level use string or integer values only.
        typedef enum _log_level {
            DEFAULT, FINE, FINER, FINEST    
        }LogLevel; 
        
        std::string config_file_name;
        std::string appname;
        
        // Groupname loglevel data structure and getter, setter methods
        // This map is filled up by Init method at initialization time.  
        std::map<std::string, LogLevel>* groupname_loglevel_map;

        LogLevel _ConvertLogLevelStringToEnum(const std::string& in_log_level);
        LogLevel _ConvertLogLevelIntToEnum(int in_log_level);
        std::string _ConvertLogLevelEnumToString(LogLevel in_log_level);
        int _ConvertLogLevelEnumToInt(LogLevel in_log_level);

        bool _SetLogLevelEnum(
            const std::string& in_group_name,
            LogLevel in_log_level
        );
	    LogLevel _GetLogLevelEnum(const std::string& in_group_name);
        
        // Handlername libpath data strucutre and getter, setter methods.
        // This map is filled up by Init method at initialization time.
        std::map<std::string, std::string>* handler_name_libpath_map;

        bool _SetHandler(
            const std::string& in_handler_name,
            const std::string& in_handler_libpath
        );

        std::string _GetHandler(const std::string& in_handler_name);
        
        // Propertyname validator data structure and getter, setter methods.
        // This map is filled by handler register method.
        typedef bool (*CheckProperty) (const std::string& property_value);

        std::map<std::string, CheckProperty>* property_name_validator_map;

        bool _SetPropertyValidator(
            const std::string& in_property_name,
            CheckProperty in_call_back_function
        );

        CheckProperty _GetPropertyValidator(
            const std::string& in_property_name
        ); 
        
        // Propertyname value data structure and getter, setter methods.
        // This map is filled up by Init method at initializtion time
        // as per registered properties by handlers - 
        // property_name_validator_map
        std::map<std::string, std::string>* property_name_value_map;

        bool _IsValidPropertyName(const std::string& in_property_name); 

        // @method
        // Parse configuration file retured by GetConfigFile method and
        // store its data into cache memory.
        // @return true/false parsed successful or not.             
        bool _ParseLogConfigFile();

        // @method
        // Save current cache memory data into configuration file returned
        // by GetConfigFile.
        // @return true/false saved successful or not.
        bool _SaveConfiguration();

        // constructor
        LoggerConfig(
            const std::string in_app_name, 
            const std::string in_app_config_file
        );

    public :

        ~LoggerConfig();

        // @method
        // This is the first call before calling any other method in this class
        // except SetConfigFile (Sets the configuration file path). It opens
        // configuration file, parse all packages log level, handlers name
        // and registered properties. Initializes all data structures used
        // by configuration.
        // @return true/false configruation was initialized successful or not.
        static bool Init();

        // @method
        // Cleanup all data structure heap memory. close configuration file.
	    static void CleanUp();
        
        // @method
        // Creates LoggerConfig object for input application, if it is already
        // not present in cache, and add it into cache.
        // @param in_app_name  application name string
        // @return reference to LoggerConfig object for input application.
        static LoggerConfig& GetLoggerConfigObject(
            const std::string in_app_name
        );
        static LoggerConfig& GetLoggerConfigObject(
            const std::string in_app_name,
            const std::string in_app_config_file
        );

        // @method
        // Set configuration file name, Initialization after calling this
        // method gets data from the file set by this method.
        // If configuration initialize is true when calling this method,
        // CleanUp method needs to be call before this method and Init method
        // after calling this method.
        // File name can not be empty string and should be exist before calling
        // this method.
        // @return true/false file name set successful or not.
        bool SetConfigFile(const std::string& in_log_file);

        // @method
        // Get current configuration file name, if set configuration file
        // name was not called before this or not executed successful, it
        // returns default configuration file name which was set at 
        // cpplogger compilation.
        // @return configuration file string.
        std::string GetConfigFile();

        // @method
        // Checks group name exists in cache or not.
        // @param in_group_name group name string, can not be empty
        // @return true/false group name exists or not.
        bool IsGroupNameExists(const std::string& in_group_name);

        // @method
        // Set log level for given group name, it accepts log level in
        // string form. Log level strings are FINE, FINER and FINEST.
        // If unknown log level string passed it will be set to DEFAULT.
        // Group name and log level string can not be empty.
        // @param in_group_name group name string, it should follow group
        // name semantics.
        // @param in_log_level log level string, can not be empty. 
        // @return true/false log level string set successful or not.
        bool SetLogLevelString(
            const std::string& in_group_name,
            const std::string& in_log_level
        );

        // @method
        // Set log level for given group name, it accepts log level in
        // int form. Log level int values are 1(FINE), 2(FINER) and 3(FINEST).
        // If unknown log level int passed it will be set to 0(DEFAULT).
        // Group name string can not be empty.
        // If group name already exist it will replace existing value.
        // @param in_group_name group name string, it should follow group
        // name semantics.
        // @param in_log_level log level int. 
        // @return true/false log level string set successful or not.
        bool SetLogLevelInt(
            const std::string& in_group_name,
            int in_log_level
        );

        // @method
        // Get log level string for input group name.
        // If group name exists in cache, log level string value can be
        // DEFAULT, FINE, FINER or FINEST.
        // If group name does not exist it return empty string.
        // @param in_group_name group name string.
        // @return out_log_level log level string. 
	    std::string GetLogLevelString(const std::string& in_group_name);

        // @method
        // Get log level int for input group name.
        // If group name exists in cache, log level int value can be
        // 0, 1, 2, 3.
        // If group name does not exist it return -1.
        // @param in_group_name group name string.
        // @return out_log_level log level int -1, 0, 1, 2, 3. 
        int GetLogLevelInt(const std::string& in_group_name);

        // @method
        // Returns group name - log level map exists in cache.
        // @return groupname_loglevel_map return reference of log level map.
        const std::map<std::string, LogLevel>& GetGroups();

        // @method
        // Check input handler exists or not.
        // @param in_handler_name handler name string.
        // @return true/false handler exists or not.
        bool IsHandlerExists(const std::string& in_handler_name);

        // @method
        // Set lib path for given handler name. If handler name already
        // exists in cache replace it with given value otherwise add it
        // to the cache.
        // @param in_handler_name - handler name string, can not be empty.
        // @param in_handler_libpath - handler library path, can not be empty.
        bool SetHandlerLibPath(
            const std::string& in_handler_name,
            const std::string& in_handler_libpath
        );

        // @method
        // Returns library path for given handler name. If handler does not
        // exist in cache returns empty string.
        // @param in_handler_name handler name string, can not be empty.
        // @return out_handler_libpath handler lib path string or empty string.
        std::string GetHandlerLibPath(const std::string& in_handler_name);

        // @method
        // Returns handler name libpath map reference.
        // @return handler name libpath map.
        const std::map<std::string, std::string>& GetHandlers();
        
        // @method
        // Checks property exist or not in property map.
        // @param in_property_name property name string, can not be empty.
        // @return true/false property exists or not in map.
        bool IsPropertyExists(const std::string& in_property_name); 

        // @method
        // Register new property, if register property was successful, parse
        // configuration file again.
        // @param in_property_name property name string, can not empty.
        // @param in_check_property_value_call_back this call back function
        // should be bool (*function) (const std::string& in_property_value)
        // format
        bool RegisterProperty(
            const std::string& property_name,
            CheckProperty in_check_property_value_call_back
        );

        // @method
        // Unregister property, erase property entry from the map.
        // If property does not exist, does not do anything, if property
        // unregister successful, parse configuration file again.
        // @param in_property_name property name string, can not be empty.
        void UnRegisterProperty(const std::string& property_name);

        // @method
        // Set property value, this property should be register before setting
        // its value. If property string is empty or was not registered, 
        // returns false.
        // @param in_property_name proeprty name string, can not be empty.
        // @return true/false property value set successful or not.
        bool SetPropertyValue(
            const std::string& in_property_name,
            const std::string& in_property_value
        );

        // @method
        // Get property value, if property does not exist returns empty string.
        // @param in_property_name property name can not be empty.
        // @return property value if exist otherwise empty string.
        std::string GetPropertyValue(
            const std::string& in_property_name
        );        
};

}    // namespace cpplogger
}    // namespace cpputils

#endif
