/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sat Apr 20 20:00:39 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file has implementation for Logger class.

 **/

#include "cpputils/cpplogger/cpplogger.h"

namespace cpputils { namespace cpplogger {

LoggerLogging LogManager::logger("cpputils.cpplogger.LogManager");
bool LogManager::initialized = false;

std::string LogManager::appname = "";
std::string LogManager::app_config_file = APP_DEFAULT_CONFIG_FILE;
std::map<std::string, Logger*>* LogManager::component_name_obj_map = NULL;

void LogManager::signal_callback_sigsegv(int in_signal_number)
{
    std::cout << "Segmentation fault occured." << std::endl;
    
    // Printing stack trace.
    const int max_backtrace_len = 50;
    void *backtrace_buff[max_backtrace_len];
    size_t backtrace_size;
    char **backtrace_strings;

    backtrace_size = backtrace(backtrace_buff, max_backtrace_len);
    backtrace_strings = backtrace_symbols(backtrace_buff, backtrace_size);
    
    for(uint32_t i = 0; i < backtrace_size; i++)
    {
       std::cout << backtrace_strings[i] << std::endl;
    }

    // Exiting as it is critical signal
    exit(in_signal_number);
}

bool LogManager::Init(
    const std::string in_app_name,
    const std::string in_app_config_file
)
{
    // Adding singals callback methods.
    signal(SIGSEGV, signal_callback_sigsegv);

    if(LogManager::initialized == false)
    {
        // Logging initialization. It should be first initialization.
        LoggerLogging::Init();
        logger.LogInfo("Initialized log manager.");

        if(in_app_name.size() == 0 || in_app_config_file.size() == 0)
        {
            std::string error_msg =
                "app name or config file can not be empty string.";
            logger.LogError(error_msg);

            throw std::runtime_error(error_msg);
        }

        if(LoggerUtils::IsFileAccessible(in_app_config_file) == false)
        {
            std::string error_msg =
                "configuration file is not accessible.";
            logger.LogError(error_msg);

            throw std::runtime_error(error_msg);
        }

        appname = in_app_name;
        app_config_file = in_app_config_file;

        // Creating configuration object and initializing it.
        
        if(LoggerConfig::Init() == false)
        {
            std::string error_msg =
                "Error in initializing configuration object";
            logger.LogError(error_msg);

            return false;
        }

        LoggerConfig::GetLoggerConfigObject(in_app_name, in_app_config_file);
       
        logger.LogInfo("Configuration object created successfully.");
          
        // Creating output object and initializing it.
        
        if(LoggerOutput::Init() == false)
        {
            std::string error_msg =
                "Error in initializing logger output object";
            logger.LogError(error_msg);

            return false;
        }
        else
        {
            logger.LogInfo("Initialized Logger Output object successfully.");
        }

        LoggerOutput::GetLoggerOutputObject(in_app_name);
        
        logger.LogInfo("Created Logger Output object successfully."); 

        // Initializing component map.

        component_name_obj_map = new std::map<std::string, Logger*>();

        logger.LogInfo(
            std::string("cppLogger was initialized successfully for app ") +
            in_app_name);
        
        LogManager::initialized = true;
    }

    return true;
}

bool LogManager::Init(const std::string in_app_name)
{
    return LogManager::Init(in_app_name, APP_DEFAULT_CONFIG_FILE);
}

void LogManager::CleanUp()
{
    if(LogManager::initialized == true)
    {
        logger.LogDebug("Cleaning log manager.");

        std::map<std::string, Logger*>::iterator str_logger_it;
        for(str_logger_it = component_name_obj_map->begin();
            str_logger_it != component_name_obj_map->end();
            str_logger_it++)
        {
            str_logger_it->second->CleanUp();
            delete str_logger_it->second;

            // Deleting component from map. It should be delete here to 
            // prevent problem in _FlushLogs method.
            component_name_obj_map->erase(str_logger_it);
        }

        delete component_name_obj_map;
        
        LoggerOutput::CleanUp();
        LoggerConfig::CleanUp();
        LoggerLogging::CleanUp();
        
        LogManager::initialized = false;
    }
}

Logger& LogManager::GetLogger(const std::string in_comp_name)
{
    if(LogManager::initialized == false)
    {
        std::string error_msg =
            "LogManager is not initialized yet. You need to call "
            "LogManager::Init method before calling GetLogger method "
            "for component " + in_comp_name;
        throw std::runtime_error(error_msg);
        std::terminate();
    }

    if(in_comp_name.size() == 0)
    {
        std::string error_msg =
            "component name can not be empty string.";
        logger.LogError(error_msg);

        throw std::runtime_error(error_msg);
    }

    // Checking group name string syntax.
    if(!LoggerUtils::IsGroupNameSyntaxOk(in_comp_name))
    {
        std::string error_msg =
            "Group name string syntax is not correct. Group name allows only"
            " [a-z] and . chars, Every package/module name should be seperated"
            " with one dot . char only.";

        logger.LogError(error_msg);
	    throw std::runtime_error(error_msg);
    }

    std::map<std::string, Logger*>::iterator str_logger_it;

    // Checking group name object exist in cache or not. If not creating
    // new object and adding it into cache and returns the same.
    if((str_logger_it = component_name_obj_map->find(
        in_comp_name)) != component_name_obj_map->end())
    {
        if(logger.IsLogVerbose())
	    {
            logger.LogInfo(
                "Logger object found in cache for group " + in_comp_name);
	    }
        return *(str_logger_it->second); 
    }
    
    if(logger.IsLogVerbose())
    {
        logger.LogInfo(
            "Logger object is not present in cache for group" + \
            in_comp_name + \
            " creating new one and adding to cache"); 
    }

    Logger* new_logger_obj = Logger::GetLoggerObject(appname, in_comp_name);
    
    if(new_logger_obj->Init() == false)
    {
        delete new_logger_obj;

        std::string error_msg =
            "false to initialized logger object.";
        logger.LogError(error_msg);

        throw std::runtime_error(error_msg);
    }

    (*component_name_obj_map)[in_comp_name] = new_logger_obj;

    if(logger.IsLogVerbose())
    {
        logger.LogInfo(
            "Logger object created successfully and added to the cache." \
            " Log leve for group " + in_comp_name + " is " + \
            LoggerConfig::GetLoggerConfigObject(appname).GetLogLevelString(
            in_comp_name));
    }
    
    return *new_logger_obj;
}

// Logger class implementation

LoggerLogging Logger::logger("cpputils.cpplogger.Logger");

void Logger::_Log(
    const char* in_log_type, 
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& in_message
)
{
    if(logger.IsLogVVerbose())
    {
        std::string info_msg = 
            std::string("Logging message of type:- ") + \
            std::string(in_log_type) + \
            std::string(" Message:- ") + in_message;
        logger.LogDebug(info_msg);
    }

    LoggerOutput& logger_output_obj = 
        LoggerOutput::GetLoggerOutputObject(this->appname);

    if(!strcmp(in_log_type, "CRITICAL"))
    {
        logger_output_obj.OutputCriticalLog(
            this->component_name,
            in_file_name, 
            in_line_number, 
            in_method_name, 
            in_method_signature, 
            in_message);
    }
    else if(!strcmp(in_log_type, "ERROR"))
    {
        logger_output_obj.OutputErrorLog(
            this->component_name,
            in_file_name, 
            in_line_number, 
            in_method_name, 
            in_method_signature, 
            in_message);
    }
    else if(!strcmp(in_log_type, "WARNING"))
    {
        logger_output_obj.OutputWarningLog(
            this->component_name,
            in_file_name, 
            in_line_number, 
            in_method_name, 
            in_method_signature, 
            in_message);
    }
    else if(!strcmp(in_log_type, "INFO"))
    {
        if(!this->IsFine()) return;

        logger_output_obj.OutputInfoLog(
            this->component_name,
            in_file_name, 
            in_line_number, 
            in_method_name, 
            in_method_signature, 
            in_message);
    }
    else if(!strcmp(in_log_type, "DEBUG"))
    {
        if(!this->IsFiner()) return;

        logger_output_obj.OutputDebugLog(
            this->component_name,
            in_file_name, 
            in_line_number, 
            in_method_name, 
            in_method_signature, 
            in_message);
    }
    else if(!strcmp(in_log_type, "HACK"))
    {
        if(!this->IsFinest()) return;

        logger_output_obj.OutputHackLog(
            this->component_name,
            in_file_name, 
            in_line_number, 
            in_method_name, 
            in_method_signature, 
            in_message);
    }
    else
    {
        logger.LogDebug("Unsupported logging type.");
    }
}

void Logger::_FlushLogs()
{
    if(LogManager::component_name_obj_map == NULL)
    {
        std::string error_msg =
            "component_name_obj_map is not initialized yet. you need to "
            "call LogManager::Init() method before using it.";
        logger.LogError(error_msg);
        return;
    }

    std::map<std::string, Logger*>::iterator str_logger_it;
    for(str_logger_it = LogManager::component_name_obj_map->begin();
        str_logger_it != LogManager::component_name_obj_map->end();
        str_logger_it++)
    {
        Logger* logger_obj = str_logger_it->second;

        if(logger_obj->cache_message_logged == true)
        {
            logger_obj->_Log(
                logger_obj->cache_log_type.c_str(), 
                logger_obj->cache_file_name.c_str(),
                logger_obj->cache_line_number, 
                logger_obj->cache_method_name.c_str(),
                logger_obj->cache_method_signature.c_str(), 
                logger_obj->cache_message_string);

            logger_obj->cache_message_logged = false;
        }
    }
}

Logger::Logger(const std::string& in_app_name, 
    const std::string& in_component_name)
{
    this->appname = in_app_name;
    this->component_name = in_component_name;
    this->component_log_level = 
        LoggerConfig::GetLoggerConfigObject(in_app_name).GetLogLevelString(
            in_component_name);
    this->cache_message_logged = false;
}

Logger::~Logger()
{
    logger.LogInfo("Destructing the Logger object.");
    _FlushLogs();
}

bool Logger::Init()
{
    // For future purpose, Does not do anything for now.
    return true;
}

void Logger::CleanUp()
{
    // For future purpose, Does not do anything for now.
}

Logger* Logger::GetLoggerObject(
    const std::string& in_app_name,
    const std::string& in_component_name
)
{
    return new Logger(in_app_name, in_component_name);
}

Logger& Logger::operator() (std::string in_log_type, const char* in_file_name,
    int in_line, const char* in_method_name, const char* in_method_sig)
{
    // Flushing logging before creating new one.
    _FlushLogs();

    if(logger.IsLogVerbose())
    {
        std::string info_msg = 
            std::string("Logging << operator logging of type ") + in_log_type;
        logger.LogInfo(info_msg); 
    }

    // Saving current << operator logging type info.
    this->cache_log_type = in_log_type;
    this->cache_file_name = std::string(in_file_name);
    this->cache_line_number = in_line;
    this->cache_method_name = std::string(in_method_name);
    this->cache_method_signature = std::string(in_method_sig);
    this->cache_message_string = "";
    this->cache_message_logged = true;
    
    return *this;
}

Logger& Logger::operator<< (bool in_message)
{
    if(in_message) this->cache_message_string += "TRUE";
    else this->cache_message_string += "FALSE";
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }

    return *this;
}

Logger& Logger::operator<< (short in_message)
{
    this->cache_message_string += std::to_string(in_message);
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    
    return *this;
}

Logger& Logger::operator<< (unsigned short in_message)
{
    this->cache_message_string += std::to_string(in_message);
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    
    return *this;
}

Logger& Logger::operator<< (int in_message)
{
    this->cache_message_string += std::to_string(in_message);
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    
    return *this;
}
        
Logger& Logger::operator<< (unsigned int in_message)
{
    this->cache_message_string += std::to_string(in_message);
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    
    return *this;
}
        
Logger& Logger::operator<< (long in_message)
{
    this->cache_message_string += std::to_string(in_message);
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    
    return *this;
}
        
Logger& Logger::operator<< (unsigned long in_message)
{
    this->cache_message_string += std::to_string(in_message);
    
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    
    return *this;
}

Logger& Logger::operator<< (float in_message)
{
    this->cache_message_string += std::to_string(in_message);
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    return *this;
}
        
Logger& Logger::operator<< (double in_message)
{
    this->cache_message_string += std::to_string(in_message);
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    return *this;
}

Logger& Logger::operator<< (long double in_message)
{
    this->cache_message_string += std::to_string(in_message);
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    return *this;
}

Logger& Logger::operator<< (const char* in_message)
{
    this->cache_message_string += std::string(in_message);
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    return *this;
}

Logger& Logger::operator<< (const std::string& in_message)
{
    this->cache_message_string += in_message;
    // This is for end manipulator, after end manipulator cache message
    // logger will finish but if after end maniuplator you are calling to 
    // << operator it will take finished logger and enable it.
    if(this->cache_message_logged == false)
    {
        this->cache_message_logged = true;    
    }
    return *this;
}

Logger& Logger::operator<< (LoggerManipulator in_manipulator)
{
    if(in_manipulator == end)
    {
        // Logged the message, empty the message string and return reference.
        this->_Log(
            this->cache_log_type.c_str(), 
            this->cache_file_name.c_str(),
            this->cache_line_number, 
            this->cache_method_name.c_str(),
            this->cache_method_signature.c_str(), 
            this->cache_message_string
        );
        this->cache_message_string = "";
        this->cache_message_logged = false;
        return *this;
    }

    return *this;
}

bool Logger::IsFine()
{
    // Checking if configuration was changed at runtime.
    if(this->component_log_level != LoggerConfig::GetLoggerConfigObject(
        this->appname).GetLogLevelString(this->component_name))
    {
        this->component_log_level = LoggerConfig::GetLoggerConfigObject(
            this->appname).GetLogLevelString(this->component_name);
    }

    if(this->component_log_level == "FINEST" || 
        this->component_log_level == "FINER" || 
        this->component_log_level == "FINE")
    {
        logger.LogInfo("Logging level fine is true.");
        return true;
    }
    logger.LogInfo("Logging level fine is false.");
    return false;
}

bool Logger::IsFiner()
{
    // Checking if configuration was changed at runtime.
    if(this->component_log_level != LoggerConfig::GetLoggerConfigObject(
        this->appname).GetLogLevelString(this->component_name))
    {
        this->component_log_level = LoggerConfig::GetLoggerConfigObject(
            this->appname).GetLogLevelString(this->component_name);
    }

    if(this->component_log_level == "FINEST" || 
        this->component_log_level == "FINER")
    {
        logger.LogInfo("Logging level finer is true.");
        return true;
    }
    logger.LogInfo("Logging level finer is true.");
    return false;
}

bool Logger::IsFinest()
{
    // Checking if configuration was changed at runtime.
    if(this->component_log_level != LoggerConfig::GetLoggerConfigObject(
        this->appname).GetLogLevelString(this->component_name))
    {
        this->component_log_level = LoggerConfig::GetLoggerConfigObject(
            this->appname).GetLogLevelString(this->component_name);
    }
    
    if(this->component_log_level == "FINEST")
    {
        logger.LogInfo("Logging level finest is true.");
        return true;
    }
    logger.LogInfo("Logging level finest is true.");
    return false;
}

void Logger::Log(
    const char* in_log_type, 
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& in_message, 
    ...)
{
    // Flushing logging before creating new one.
    _FlushLogs();
    
    va_list log_vargs_list;
    va_start(log_vargs_list, in_message);

    std::string unformatted_string;
    LoggerUtils::ParseFormattedString(unformatted_string, in_message, 
        log_vargs_list);

    va_end(log_vargs_list);

    _Log(in_log_type, in_file_name, in_line_number, in_method_name,
        in_method_signature, unformatted_string);
}

void Logger::Log(
    const char* in_log_type, 
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    const char* in_message, 
    ...)
{
    // Flushing logging before creating new one.
    _FlushLogs();
    
    std::string in_message_str = std::string(in_message);
    va_list log_vargs_list;
    va_start(log_vargs_list, in_message);

    std::string unformatted_string;
    LoggerUtils::ParseFormattedString(unformatted_string, in_message_str, 
        log_vargs_list);
    va_end(log_vargs_list);

    _Log(in_log_type, in_file_name, in_line_number, in_method_name,
        in_method_signature, unformatted_string);
}

}    // namespace logger
}    // namespace cpputils
