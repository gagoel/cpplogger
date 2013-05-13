/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         logger_conf.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed Apr 24 17:36:56 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file implements LoggerConf class.

 **/

#include "cpplogger/cpplogger_conf.h"

namespace cpputils
{
namespace cpplogger
{

LoggerLogging LoggerConf::logger("cpputils.cpplogger.LoggerConf");

bool LoggerConf::initialized = false;

// CPPLOGGER_CONFIG_FILE is preprocessor which gets value from cpplogger
// configuration. This value can be set while configuring cpplogger.
std::string LoggerConf::conf_file_name = CPPLOGGER_CONFIG_FILE;

// Group name log level settings implementations
std::map<std::string, LoggerConf::LogLevel>* 
    LoggerConf::groupname_loglevel_map = NULL;

bool LoggerConf::SetLogLevelEnum(
    const std::string& in_group_name,
    LogLevel in_log_level)
{
    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not correct");
        return false;
    }

    (*groupname_loglevel_map)[in_group_name] = in_log_level;

    if(!SaveConfiguration())
    {
        logger.LogError("Error in saving configuration to disk.");
        return false;
    }
    return true;
}

LoggerConf::LogLevel LoggerConf::GetLogLevelEnum(
    const std::string& in_group_name)
{
    std::map<std::string, LogLevel>::iterator str_loglevel_map_it;

    if((str_loglevel_map_it = groupname_loglevel_map->find(in_group_name)) 
        == groupname_loglevel_map->end())
    {
        return DEFAULT;
    }
    return str_loglevel_map_it->second; 
}

LoggerConf::LogLevel LoggerConf::ConvertLogLevelStringToEnum(
    const std::string& in_log_level)
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

LoggerConf::LogLevel LoggerConf::ConvertLogLevelIntToEnum(
    int in_log_level)
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

std::string LoggerConf::ConvertLogLevelEnumToString(
    LoggerConf::LogLevel in_log_level)
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

int LoggerConf::ConvertLogLevelEnumToInt(
    LoggerConf::LogLevel in_log_level)
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

// Handler name libpath settings.
std::map<std::string, std::string>* 
    LoggerConf::handler_name_libpath_map = NULL;

bool LoggerConf::SetHandler(const std::string& in_handler_name,
    const std::string& in_handler_libpath)
{
    if(in_handler_name == "" || in_handler_libpath == "")
    {
        logger.LogError("Handler name or Library path can not be empty.");
        return false;
    }

    (*handler_name_libpath_map)[in_handler_name] = in_handler_libpath;

    if(!SaveConfiguration())
    {
        logger.LogError("Error in saving configuration to disk.");
        return false;
    }
    return true;
}

std::string LoggerConf::GetHandler(const std::string& in_handler_name)
{
    std::map<std::string, std::string>::iterator find_it;
    if((find_it = handler_name_libpath_map->find(in_handler_name)) 
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
std::map<std::string, LoggerConf::CheckProperty>*
    LoggerConf::property_name_validator_map = NULL;

bool LoggerConf::SetPropertyValidator(const std::string& in_property_name,
    CheckProperty in_call_back_function)
{
    if(in_property_name == "" || in_call_back_function == NULL)
    {
        logger.LogError("Input argument syntax is not correct.");
        return false;
    }
    
    (*property_name_validator_map)[in_property_name] = in_call_back_function;

    if(logger.IsLogVerbose())
    {
        std::string info_msg =
            std::string("Setting property validator for property name - ") +
            in_property_name;
        logger.LogInfo(info_msg);
    }

    return true;
}

LoggerConf::CheckProperty LoggerConf::GetPropertyValidator(
    const std::string& in_property_name)
{
    std::map<std::string, LoggerConf::CheckProperty>::iterator find_it;
    if((find_it = property_name_validator_map->find(in_property_name))
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
std::map<std::string, std::string>* LoggerConf::property_name_value_map = NULL;

// TODO - Add more constraint on property name in future.
bool LoggerConf::IsValidPropertyName(const std::string& in_property_name)
{
    if(in_property_name == "")
    {
        return false;
    }
    return true;
}

bool LoggerConf::IsPropertyExists(const std::string& in_property_name)
{
    if(property_name_value_map->count(in_property_name) == 0)
    {
         return false;
    }
    return true;
}

bool LoggerConf::SetPropertyValue(const std::string& in_property_name,
    const std::string& in_property_value)
{
    LoggerConf::CheckProperty call_back;
    call_back = GetPropertyValidator(in_property_name);

    if(call_back == NULL)
    {
        logger.LogWarning("Property was not registerd.");
        return false;
    }

    if(IsValidPropertyName(in_property_name) == false)
    {
        logger.LogError("Property name syntax is not correct");
        return false;
    }

    if(call_back(in_property_value) == false)
    {
        logger.LogError("Property value is not valid.");
        return false;
    }

    (*property_name_value_map)[in_property_name] = in_property_value;

    if(!SaveConfiguration())
    {
        logger.LogError("Error in saving configuration to disk.");
        return false;
    }
    return true;
}

std::string LoggerConf::GetPropertyValue(const std::string& in_property_name)
{
    std::map<std::string, std::string>::iterator str_str_it;
    if((str_str_it = property_name_value_map->find(in_property_name)) == 
        property_name_value_map->end())
    {
        return "";
    }
    return str_str_it->second;
}

bool LoggerConf::ParseLogConfigFile()
{
    std::multimap<std::string, std::string> key_value_map;
    if(!LoggerUtils::ParseKeyValueFile(key_value_map, 
        LoggerConf::conf_file_name))
    {
        logger.LogError(
            "Error in getting key value pair from configuration file.");
	    return false;
    }
    
    std::fstream stream(GetConfigFile().c_str(), std::fstream::in);
    
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

        LoggerConf::CheckProperty call_back;
        
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
                (*handler_name_libpath_map)[handler_name_lib_path.first] =
                    handler_name_lib_path.second;
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
                (*groupname_loglevel_map)[gp_pair.first] = 
                    ConvertLogLevelStringToEnum(gp_pair.second);
            }
        }
        else if((call_back = GetPropertyValidator(key)) != NULL &&
            call_back(value) == true)
        {
            (*property_name_value_map)[key] = value;
        }
    }

    if(logger.IsLogVerbose())
    {
        std::map<std::string, LoggerConf::LogLevel>::iterator temp_it;
        for(temp_it = groupname_loglevel_map->begin();
            temp_it != groupname_loglevel_map->end();
            temp_it++)
        {
            std::string info_str = std::string("Group name: ") + \
                temp_it->first + std::string(" Loglevel: ") + \
                ConvertLogLevelEnumToString(temp_it->second);
            logger.LogInfo(info_str);
        }
    }
    return true;
}

// Save Configuration on disk.
// Create a new temp configuration file, Load cache configuration to
// new created file. Rename Old configuration file with .old extension.
// Rename temp file with right configuration file name. Delete the
// old configuration file and return true.
// TODO - It needs to handle locks for multithreading.
bool LoggerConf::SaveConfiguration()
{
    std::string tmp_file_name = GetConfigFile() + std::string(".tmp");

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
    std::string conf_new_name = GetConfigFile() + std::string(".old");

    if(rename(GetConfigFile().c_str(), conf_new_name.c_str()) != 0)
    {
        //remove(tmp_file_name.c_str());
        logger.LogError("Error in renaming configuration file for saving.");
        return false;
    }

    // Renaming temp file.
    if(rename(tmp_file_name.c_str(), GetConfigFile().c_str()) != 0)
    {
        remove(tmp_file_name.c_str());
        logger.LogError("Error in renaming temp file for saving.");
        return false;
    }

    remove(conf_new_name.c_str());
    return true;
}

bool LoggerConf::SetConfigFile(const std::string& in_conf_file)
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

    LoggerConf::conf_file_name = in_conf_file;
    return true;
}

std::string LoggerConf::GetConfigFile()
{
    return LoggerConf::conf_file_name;
}

bool LoggerConf::IsGroupNameExists(const std::string& in_group_name)
{
    if(groupname_loglevel_map->find(in_group_name) == 
        groupname_loglevel_map->end())
    {
        return false;
    }
    return true;
}

bool LoggerConf::SetLogLevelString(
    const std::string& in_group_name,
    const std::string& in_log_level)
{
    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not ok.");
        return false;
    }

    SetLogLevelEnum(in_group_name, ConvertLogLevelStringToEnum(in_log_level));
    return true;
}

bool LoggerConf::SetLogLevelInt(
    const std::string& in_group_name,
    int in_log_level)
{
    if(LoggerUtils::IsGroupNameSyntaxOk(in_group_name) == false)
    {
        logger.LogError("Group name syntax is not ok.");
        return false;
    }

    SetLogLevelEnum(in_group_name, ConvertLogLevelIntToEnum(in_log_level));
    return true;
}

std::string LoggerConf::GetLogLevelString(const std::string& in_group_name)
{
    if(IsGroupNameExists(in_group_name))
    {
        LogLevel log_level = GetLogLevelEnum(in_group_name);
        return ConvertLogLevelEnumToString(log_level);
    }
    return "";
}

int LoggerConf::GetLogLevelInt(const std::string& in_group_name)
{
    if(IsGroupNameExists(in_group_name))
    {
        LogLevel log_level = GetLogLevelEnum(in_group_name);
        return ConvertLogLevelEnumToInt(log_level);
    }
    return -1;
}

const std::map<std::string, LoggerConf::LogLevel>& LoggerConf::GetGroups()
{
    return (*groupname_loglevel_map);
}

bool LoggerConf::IsHandlerExists(const std::string& in_handler_name)
{
    if(in_handler_name.size() == 0)
    {
        return false;
    }

    if(handler_name_libpath_map->find(in_handler_name) ==
        handler_name_libpath_map->end())
    {
        return false;
    }
    return true;
}

bool LoggerConf::SetHandlerLibPath(const std::string& in_handler_name,
    const std::string& in_handler_libpath)
{
    return SetHandler(in_handler_name, in_handler_libpath);
}

std::string LoggerConf::GetHandlerLibPath(const std::string& in_handler_name)
{
    return GetHandler(in_handler_name);
}

const std::map<std::string, std::string>& LoggerConf::GetHandlers()
{
    return *handler_name_libpath_map;
}

bool LoggerConf::RegisterProperty(const std::string& in_property_name,
    LoggerConf::CheckProperty call_back_check)
{
    if(IsValidPropertyName(in_property_name) == false)
    {
        logger.LogWarning("Property name is not valid.");
        return false;
    }

    if(call_back_check == NULL)
    {
        logger.LogWarning("Call back can not be null.");
        return false;
    }
    
    if(SetPropertyValidator(in_property_name, call_back_check) == false)
    {
        logger.LogWarning("Setting property validator failed.");
        return false;
    }
    
    if(!ParseLogConfigFile())
    {
        logger.LogError("Error in parsing configuration file.");
        return false;
    }
    return true;
}

void LoggerConf::UnRegisterProperty(const std::string& in_property_name)
{
    property_name_validator_map->erase(in_property_name);
    if(!ParseLogConfigFile())
    {
        logger.LogError("Error in parsing configuration file.");
        return;
    }
}

bool LoggerConf::Init()
{
    groupname_loglevel_map = new std::map<std::string, LogLevel>();
    handler_name_libpath_map = new std::map<std::string, std::string>();
    property_name_value_map = new std::map<std::string, std::string>();
    property_name_validator_map = 
        new std::map<std::string, LoggerConf::CheckProperty>;

    if(!ParseLogConfigFile())
    {
        CleanUp();
        logger.LogError("Error in parsing configuration file.");
        return false;
    }

    initialized = true;
    return true;
}

void LoggerConf::CleanUp()
{
    delete groupname_loglevel_map;
    delete handler_name_libpath_map;
    delete property_name_value_map;

    initialized = false;
}

}    // namespace cpplogger
}    // namespace cpputils
