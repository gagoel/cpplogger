/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         logger_config.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed Apr 24 17:36:56 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file implements LoggerConfig class.

 **/

#include "cpplogger/cpplogger_config.h"

namespace cpputils
{
namespace cpplogger
{

LoggerLogging LoggerConfig::logger("cpputils.cpplogger.LoggerConfig");
bool LoggerConfig::initialized = false;
std::map<std::string, LoggerConfig*>* 
    LoggerConfig::app_name_config_obj_map = NULL;

// Private methods.
// Log Level Conversion methods.
LoggerConfig::LogLevel LoggerConfig::_ConvertLogLevelStringToEnum(
    const std::string& in_log_level
)
{
    if(in_log_level == "DEFAULT")
    {
        return DEFAULT;
    }
    if(in_log_level == "FINE")
    {
        return FINE;
    }
    else if(in_log_level == "FINER")
    {
        return FINER;
    }
    else if(in_log_level == "FINEST")
    {
        return FINEST;
    }
    else
    {
        logger.LogWarning("Log level string is not supported.");
        return DEFAULT;
    }
}

LoggerConfig::LogLevel LoggerConfig::_ConvertLogLevelIntToEnum(
    int in_log_level
)
{
    switch(in_log_level)
    {
        case 0 :
            return DEFAULT;
        case 1 :
            return FINE;
        case 2 :
            return FINER;
        case 3 :
            return FINEST;
        default :
            return DEFAULT;
    }
}

std::string LoggerConfig::_ConvertLogLevelEnumToString(
    LoggerConfig::LogLevel in_log_level
)
{
    switch(in_log_level)
    {
        case DEFAULT :
            return "DEFAULT";
        case FINE :
            return "FINE";
        case FINER :
            return "FINER";
        case FINEST :
            return "FINEST";
    }
    return "DEFAULT";
}

int LoggerConfig::_ConvertLogLevelEnumToInt(LoggerConfig::LogLevel in_log_level)
{
    switch(in_log_level)
    {
        case DEFAULT :
            return 0;
        case FINE :
            return 1;
        case FINER :
            return 2;
        case FINEST :
            return 3;
    }
    return 0;
}

bool LoggerConfig::_SetLogLevelEnum(
    const std::string& in_group_name,
    LogLevel in_log_level
)
{
    logger.LogInfo(
        "Setting log level for input group."
        "Group name is " + in_group_name +
        "Log Level is " + this->_ConvertLogLevelEnumToString(in_log_level));

    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not correct");
        return false;
    }

    (*this->groupname_loglevel_map)[in_group_name] = in_log_level;

    if(!_SaveConfiguration())
    {
        logger.LogError("Error in saving configuration to disk.");
        return false;
    }
    return true;
}

LoggerConfig::LogLevel LoggerConfig::_GetLogLevelEnum(
    const std::string& in_group_name
)
{
    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not ok.");
        return DEFAULT;
    }

    // We are searching for group name in groupname_loglevel_map
    // Search starts from top to down hirearchy fashion.
    // If input groupname is package1.package2.class
    // Search happens in below order and will return loglevel
    // for first match.
    // package1
    // package1.package2
    // package1.package2.class
    // If no match found it will return DEFAULT value.

    std::istringstream iss(in_group_name);

    // Getting first component name.
    std::string previous_comp, current_comp;
    std::getline(iss, previous_comp, '.');

    std::map<std::string, LogLevel>::iterator str_loglevel_map_it;
    while(std::getline(iss, current_comp, '.'))
    {
        if((str_loglevel_map_it = this->groupname_loglevel_map->find(
            previous_comp)) != groupname_loglevel_map->end())
        {
            std::string info_msg = 
                "Log Level found for given package "
                "Package name " + in_group_name +
                "Log Level " + 
                this->_ConvertLogLevelEnumToString(
                  str_loglevel_map_it->second);
            logger.LogInfo(info_msg);

            return str_loglevel_map_it->second; 
        }
        
        previous_comp = previous_comp + "." + current_comp;
    }

    // Searching in last package name.
    if((str_loglevel_map_it = this->groupname_loglevel_map->find(
        previous_comp)) != groupname_loglevel_map->end())
    {
        std::string info_msg = 
            "Log Level found for given package "
            "Package name " + in_group_name +
            "Log Level " + 
            this->_ConvertLogLevelEnumToString(
              str_loglevel_map_it->second);
        logger.LogInfo(info_msg);
        return str_loglevel_map_it->second; 
    }
     
    // Returning default log level if not found in map. 
    std::string info_msg = 
        "Log Level found for given package "
        "Package name " + in_group_name +
        "Log Level " + 
        this->_ConvertLogLevelEnumToString(DEFAULT);
    logger.LogInfo(info_msg);

    return DEFAULT;
}


// Handler name libpath settings.
bool LoggerConfig::_SetHandler(
    const std::string& in_handler_name,
    const std::string& in_handler_libpath
)
{
    if(in_handler_name == "" || in_handler_libpath == "")
    {
        logger.LogError("Handler name or Library path can not be empty.");
        return false;
    }

    (*this->handler_name_libpath_map)[in_handler_name] = in_handler_libpath;

    if(!_SaveConfiguration())
    {
        logger.LogError("Error in saving configuration to disk.");
        return false;
    }
    return true;
}

std::string LoggerConfig::_GetHandler(const std::string& in_handler_name)
{
    std::map<std::string, std::string>::iterator find_it;
    if((find_it = this->handler_name_libpath_map->find(in_handler_name)) 
        == handler_name_libpath_map->end())
    {
        std::string warning_msg = "Library path for " + in_handler_name + \
            " handler does not exist in configuration.";
        logger.LogWarning(warning_msg);
        return "";  
    }
    return find_it->second;
}

// Property name validator settings.
bool LoggerConfig::_SetPropertyValidator(
    const std::string& in_property_name,
    CheckProperty in_call_back_function
)
{
    if(in_property_name == "" || in_call_back_function == NULL)
    {
        logger.LogError("Input argument syntax is not correct.");
        return false;
    }
    
    (*this->property_name_validator_map)[in_property_name] 
        = in_call_back_function;

    if(logger.IsLogVerbose())
    {
        std::string info_msg =
            std::string("Setting property validator for property name - ") +
            in_property_name;
        logger.LogInfo(info_msg);
    }

    return true;
}

LoggerConfig::CheckProperty LoggerConfig::_GetPropertyValidator(
    const std::string& in_property_name
)
{
    std::map<std::string, LoggerConfig::CheckProperty>::iterator find_it;
    if((find_it = this->property_name_validator_map->find(in_property_name))
        == property_name_validator_map->end())
    {
        std::string info_msg = 
            std::string("Property validator does not exist that means \
            property was not registerd yet - ") + in_property_name;
        logger.LogInfo(info_msg);
        return NULL;
    }
    return find_it->second;
}

// Property name value settings.
// TODO - Add more constraint on property name in future.
bool LoggerConfig::_IsValidPropertyName(const std::string& in_property_name)
{
    if(in_property_name == "")
    {
        return false;
    }
    return true;
}

bool LoggerConfig::_ParseLogConfigFile()
{
    std::multimap<std::string, std::string> key_value_map;
    if(!LoggerUtils::ParseKeyValueFile(key_value_map, 
        LoggerConfig::config_file_name))
    {
        logger.LogError(
            "Error in getting key value pair from configuration file.");
	    return false;
    }
    
    std::fstream stream(this->config_file_name.c_str(), std::fstream::in);
    
    if(!stream.good())
    {
        logger.LogError("Error in opening configuration file.");
        return false;
    }

    std::multimap<std::string, std::string>::iterator key_value_map_it;

    for(key_value_map_it = key_value_map.begin();
        key_value_map_it != key_value_map.end();
	    key_value_map_it++)
    {
        std::string key = key_value_map_it->first;
	    std::string value = key_value_map_it->second;

        if(logger.IsLogVerbose())
        {
            std::string info_str = std::string("Key: ") + \
                key + std::string(" Value: ") + value;
            logger.LogInfo(info_str);
        }

        LoggerConfig::CheckProperty call_back;
        
        // Comment string
        if(key.at(0) == '#' || key.size() == 0)
        {
            continue;
        }
	    // Handler name library path string.
        else if(strcmp(key.c_str(), "AddHandler") == 0)
	    {
            std::pair<std::string, std::string> handler_name_lib_path;
            LoggerUtils::GetKeyValuePair(handler_name_lib_path, 
                value.c_str());

            if(handler_name_lib_path.first.size() != 0 &&
                handler_name_lib_path.second.size() != 0)
            {
                (*this->handler_name_libpath_map)[handler_name_lib_path.first] 
                    = handler_name_lib_path.second;
            }
	    }
        else if(strcmp(key.c_str(), "SetLogLevel") == 0)
        {
            std::pair<std::string, std::string> gp_pair;
            LoggerUtils::GetKeyValuePair(gp_pair, 
                value.c_str());

            if(logger.IsLogVerbose())
            {
                std::string info_str = std::string("Group name: ") + \
                    gp_pair.first + std::string(" LogLevel: ") + \
                    gp_pair.second;
                logger.LogInfo(info_str);
            }

            if(gp_pair.first.size() != 0 && gp_pair.second.size() != 0)
            {
                (*this->groupname_loglevel_map)[gp_pair.first] = 
                    _ConvertLogLevelStringToEnum(gp_pair.second);
            }
        }
        else if((call_back = _GetPropertyValidator(key)) != NULL &&
            call_back(value) == true)
        {
            (*this->property_name_value_map)[key] = value;
        }
    }

    if(logger.IsLogVerbose())
    {
        std::map<std::string, LoggerConfig::LogLevel>::iterator temp_it;
        for(temp_it = groupname_loglevel_map->begin();
            temp_it != groupname_loglevel_map->end();
            temp_it++)
        {
            std::string info_str = std::string("Group name: ") + \
                temp_it->first + std::string(" Loglevel: ") + \
                _ConvertLogLevelEnumToString(temp_it->second);
            logger.LogInfo(info_str);
        }
    }

    stream.close();
    return true;
}

// Save Configuration on disk.
// Create a new temp configuration file, Load cache configuration to
// new created file. Rename Old configuration file with .old extension.
// Rename temp file with right configuration file name. Delete the
// old configuration file and return true.
// TODO - It needs to handle locks for multithreading.
bool LoggerConfig::_SaveConfiguration()
{
    std::string tmp_file_name = this->config_file_name + std::string(".tmp");

    std::fstream tmp_file_stream(tmp_file_name, std::fstream::in | \
        std::fstream::out | std::fstream::trunc);
    
    if(!tmp_file_stream.good())
    {
        logger.LogError("Error in creating tmp configuration file.");
        return false;
    }

    std::map<std::string, std::string>::iterator str_str_map_it;
    std::map<std::string, LogLevel>::iterator str_loglevel_map_it;

    // Loading package logging info.
    std::string loglevel_comment = 
        "# Package name and Package Log Level.";
    tmp_file_stream << loglevel_comment << "\n\n";
    
    for(str_loglevel_map_it = groupname_loglevel_map->begin();
        str_loglevel_map_it != groupname_loglevel_map->end();
        str_loglevel_map_it++)
    {
        tmp_file_stream << "SetLogLevel " << str_loglevel_map_it->first \
            << "\t\t" << GetLogLevelString(str_loglevel_map_it->first) \
            << "\n";
    }

    tmp_file_stream << "\n\n\n";

    // Loading Handler name and Library path.
    std::string handler_comment =
        "# Handler name and Handler library path.";
    tmp_file_stream << handler_comment << "\n\n";
    for(str_str_map_it = handler_name_libpath_map->begin();
        str_str_map_it != handler_name_libpath_map->end();
        str_str_map_it++)
    {
        tmp_file_stream << "AddHandler " << str_str_map_it->first << "\t\t" \
            << str_str_map_it->second << "\n";
    }

    tmp_file_stream << "\n\n\n";
    
    // Loading properties name and value.
    std::string properties_comment =
        "# Properties name and Properties value.";
    tmp_file_stream << properties_comment << "\n\n";
    
    for(str_str_map_it = property_name_value_map->begin();
        str_str_map_it != property_name_value_map->end();
        str_str_map_it++)
    {
        tmp_file_stream << str_str_map_it->first << "\t\t" << \
            str_str_map_it->second << "\n";
    }

    tmp_file_stream << "\n\n\n";

    tmp_file_stream.close();

    // Renaming configuration file with extension .old
    std::string conf_new_name = this->config_file_name + std::string(".old");

    if(rename(this->config_file_name.c_str(), conf_new_name.c_str()) != 0)
    {
        //remove(tmp_file_name.c_str());
        logger.LogError("Error in renaming configuration file for saving.");
        return false;
    }

    // Renaming temp file.
    if(rename(tmp_file_name.c_str(), this->config_file_name.c_str()) != 0)
    {
        remove(tmp_file_name.c_str());
        logger.LogError("Error in renaming temp file for saving.");
        return false;
    }

    remove(conf_new_name.c_str());
    return true;
}

LoggerConfig::LoggerConfig(const std::string in_app_name,
    const std::string in_app_config_file)
{
    logger.LogDebug("Running LoggerConfig Constructor.");

    this->appname = in_app_name;
    this->config_file_name = in_app_config_file;
    
    // Creating data structures for configuration object.
    this->groupname_loglevel_map = new std::map<std::string, LogLevel>();
    this->handler_name_libpath_map = new std::map<std::string, std::string>();
    this->property_name_value_map = new std::map<std::string, std::string>();
    this->property_name_validator_map = 
        new std::map<std::string, LoggerConfig::CheckProperty>;
}

// Public methods
LoggerConfig::~LoggerConfig() 
{
    logger.LogDebug("Running LoggerConfig Destructor."); 

    delete this->groupname_loglevel_map;
    delete this->handler_name_libpath_map;
    delete this->property_name_value_map;
    delete this->property_name_validator_map;

    logger.LogInfo("LoggerConfig object destructed successfully.");
}

bool LoggerConfig::Init()
{
    if(initialized == false)
    {
        LoggerLogging::Init();
        app_name_config_obj_map = new std::map<std::string, LoggerConfig*>();
        initialized = true;
    }

    return true;
}

void LoggerConfig::CleanUp()
{
    if(initialized == true)
    {
        logger.LogInfo("Cleaning up logger configuration.");

        std::map<std::string, LoggerConfig*>::iterator str_logger_conf_it;
        for(str_logger_conf_it = app_name_config_obj_map->begin();
            str_logger_conf_it != app_name_config_obj_map->end();
            str_logger_conf_it++)
        {
            delete str_logger_conf_it->second;
        }

        LoggerLogging::CleanUp();
        initialized = false;

        logger.LogInfo("logger configuration cleaned up successfully.");
    }
}

LoggerConfig& LoggerConfig::GetLoggerConfigObject(
    const std::string in_app_name,
    const std::string in_app_config_file
)
{
    logger.LogDebug("GetLoggerConfigObject method called.");

    if(in_app_name.size() == 0)
    {
        std::string error_msg = 
            "Application name string can not be empty string.";
        logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }

    if(app_name_config_obj_map == NULL)
    {
        std::string error_msg =
            "app_name_config_obj_map is not initialized yet. you need to "
            "run LoggerConfig::Init() method before calling this. "
            "application name is " + in_app_name +
            " application configuration file is " + in_app_config_file;

        logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }

    std::map<std::string, LoggerConfig*>::iterator str_logger_conf_it;
    if((str_logger_conf_it = app_name_config_obj_map->find(in_app_name)) != 
        app_name_config_obj_map->end())
    {
        logger.LogInfo(
            "Application configuration object found in cache for application " 
            + in_app_name);

        return *str_logger_conf_it->second;
    }

    logger.LogInfo(
        "Application configuration object did not found in cache, "
        "creating new one. Application name is " + in_app_name );

    LoggerConfig* new_logger_conf_obj = 
        new LoggerConfig(in_app_name, in_app_config_file);

    // Parsing application configuration file.
    if(!new_logger_conf_obj->_ParseLogConfigFile())
    {
        delete new_logger_conf_obj;

        logger.LogError("Error in parsing configuration file.");
        throw std::runtime_error("Error in parsing configuration file");
    }

    // Adding configuration object to cache.
    (*app_name_config_obj_map)[in_app_name] = new_logger_conf_obj;

    return *new_logger_conf_obj;
}

LoggerConfig& LoggerConfig::GetLoggerConfigObject(const std::string in_app_name)
{
    // CPPLOGGER_CONFIG_FILE is preprocessor which gets value from cpplogger
    // configuration. This value can be set while configuring cpplogger.
    return GetLoggerConfigObject(in_app_name, APP_DEFAULT_CONFIG_FILE);
}

bool LoggerConfig::SetConfigFile(const std::string& in_conf_file)
{
    if(in_conf_file.size() == 0)
    {
        logger.LogError("Config file name can not be empty.");
        return false;
    }
    
    std::ifstream tmp_stream(in_conf_file);
    if(!tmp_stream.good())
    {
        logger.LogError("File does not exist or not have access to read.");
        return false;
    }
    tmp_stream.close();

    this->config_file_name = in_conf_file;
    return true;
}

std::string LoggerConfig::GetConfigFile()
{
    return this->config_file_name;
}

bool LoggerConfig::IsGroupNameExists(const std::string& in_group_name)
{
    if(this->groupname_loglevel_map->find(in_group_name) == 
        this->groupname_loglevel_map->end())
    {
        return false;
    }
    return true;
}

bool LoggerConfig::SetLogLevelString(
    const std::string& in_group_name,
    const std::string& in_log_level)
{
    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not ok.");
        return false;
    }

    _SetLogLevelEnum(in_group_name, _ConvertLogLevelStringToEnum(in_log_level));
    return true;
}

bool LoggerConfig::SetLogLevelInt(
    const std::string& in_group_name,
    int in_log_level)
{
    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not ok.");
        return false;
    }

    this->_SetLogLevelEnum(in_group_name, 
        this->_ConvertLogLevelIntToEnum(in_log_level));
    return true;
}

std::string LoggerConfig::GetLogLevelString(const std::string& in_group_name)
{
    // Returns the default log level if it was not set.
    LogLevel log_level = this->_GetLogLevelEnum(in_group_name);
    return _ConvertLogLevelEnumToString(log_level);
}

int LoggerConfig::GetLogLevelInt(const std::string& in_group_name)
{
    // Returns the default log level if it was not set.
    LogLevel log_level = _GetLogLevelEnum(in_group_name);
    return _ConvertLogLevelEnumToInt(log_level);
}

const std::map<std::string, LoggerConfig::LogLevel>& LoggerConfig::GetGroups()
{
    return (*this->groupname_loglevel_map);
}

bool LoggerConfig::IsHandlerExists(const std::string& in_handler_name)
{
    if(in_handler_name.size() == 0)
    {
        return false;
    }

    if(this->handler_name_libpath_map->find(in_handler_name) ==
        this->handler_name_libpath_map->end())
    {
        return false;
    }
    return true;
}

bool LoggerConfig::SetHandlerLibPath(const std::string& in_handler_name,
    const std::string& in_handler_libpath)
{
    return _SetHandler(in_handler_name, in_handler_libpath);
}

std::string LoggerConfig::GetHandlerLibPath(const std::string& in_handler_name)
{
    return _GetHandler(in_handler_name);
}

const std::map<std::string, std::string>& LoggerConfig::GetHandlers()
{
    logger.LogInfo(
        "Returning the handlers map for application " +
        this->appname);
    return *this->handler_name_libpath_map;
}

bool LoggerConfig::IsPropertyExists(const std::string& in_property_name)
{
    if(property_name_value_map->count(in_property_name) == 0)
    {
         return false;
    }
    return true;
}

bool LoggerConfig::RegisterProperty(const std::string& in_property_name,
    LoggerConfig::CheckProperty call_back_check)
{
    if(_IsValidPropertyName(in_property_name) == false)
    {
        logger.LogWarning("Property name is not valid.");
        return false;
    }

    if(call_back_check == NULL)
    {
        logger.LogWarning("Call back can not be null.");
        return false;
    }
    
    if(_SetPropertyValidator(in_property_name, call_back_check) == false)
    {
        logger.LogWarning("Setting property validator failed.");
        return false;
    }
    
    if(!_ParseLogConfigFile())
    {
        logger.LogError("Error in parsing configuration file.");
        return false;
    }
    return true;
}

void LoggerConfig::UnRegisterProperty(const std::string& in_property_name)
{
    this->property_name_validator_map->erase(in_property_name);
    if(!_ParseLogConfigFile())
    {
        logger.LogError("Error in parsing configuration file.");
        return;
    }
}

bool LoggerConfig::SetPropertyValue(
    const std::string& in_property_name,
    const std::string& in_property_value
)
{
    LoggerConfig::CheckProperty call_back;
    call_back = _GetPropertyValidator(in_property_name);

    if(call_back == NULL)
    {
        logger.LogWarning("Property was not registerd.");
        return false;
    }

    if(_IsValidPropertyName(in_property_name) == false)
    {
        logger.LogError("Property name syntax is not correct");
        return false;
    }

    if(call_back(in_property_value) == false)
    {
        logger.LogError("Property value is not valid.");
        return false;
    }

    (*this->property_name_value_map)[in_property_name] = in_property_value;

    if(!_SaveConfiguration())
    {
        logger.LogError("Error in saving configuration to disk.");
        return false;
    }
    return true;
}

std::string LoggerConfig::GetPropertyValue(const std::string& in_property_name)
{
    std::map<std::string, std::string>::iterator str_str_it;
    if((str_str_it = this->property_name_value_map->find(in_property_name)) == 
        property_name_value_map->end())
    {
        return "";
    }
    return str_str_it->second;
}

}    // namespace cpplogger
}    // namespace cpputils
