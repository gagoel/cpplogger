/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         logger.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sat Apr 20 20:00:39 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file has implementation for Logger class.

 **/

#include "cpplogger/cpplogger.h"

namespace cpputils
{
namespace cpplogger
{

LoggerLogging Logger::logger("cpputils.cpplogger.Logger");

bool Logger::initialized = false;

std::map<std::string, Logger*>* Logger::groupname_loggerobj_map = NULL;

Logger::Logger(const std::string& in_group_name)
{
    this->group_name = in_group_name;
}

void Logger::CriticalLog(
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    // Creating stack trace.
    const int max_backtrace_len = 50;
    void *backtrace_buff[max_backtrace_len];
    size_t backtrace_size;
    char **backtrace_strings;

    backtrace_size = backtrace(backtrace_buff, max_backtrace_len);
    backtrace_strings = backtrace_symbols(backtrace_buff, backtrace_size);

    LogDB log_struct = {
        this->group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    CriticalLogDB critical_log_struct = {
        log_struct,
        backtrace_strings,
        backtrace_size
    };

    LoggerHandler::ExecuteCriticalHandlers(critical_log_struct);
  
    // Freeing stack trace memory created by backtrace function
    free(backtrace_strings); 

    // Cleanup everything and Terminate the program
    Logger::CleanUp();
    exit(EXIT_FAILURE);
}

void Logger::ErrorLog(
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    ErrorLogDB error_log_struct = {
        log_struct
    };

    LoggerHandler::ExecuteErrorHandlers(error_log_struct);
}

void Logger::WarningLog(
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    WarningLogDB warning_log_struct = {
        log_struct
    };

    LoggerHandler::ExecuteWarningHandlers(warning_log_struct);
}

void Logger::InfoLog(
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    InfoLogDB info_log_struct = {
        log_struct
    };

    LoggerHandler::ExecuteInfoHandlers(info_log_struct);
}

void Logger::DebugLog(
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    DebugLogDB debug_log_struct = {
        log_struct
    };

    LoggerHandler::ExecuteDebugHandlers(debug_log_struct);
}

void Logger::HackLog(
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    HackLogDB hack_log_struct = {
        log_struct
    };

    LoggerHandler::ExecuteHackHandlers(hack_log_struct);
}

Logger::~Logger()
{
    if(this->cache_message_logged == true)
    {
        this->_Log(this->cache_log_type.c_str(), this->cache_file_name.c_str(),
            this->cache_line_number, this->cache_method_name.c_str(),
            this->cache_method_signature.c_str(), this->cache_message_string);
        this->cache_message_logged = false;
    }
}

Logger& Logger::operator() (std::string in_log_type, const char* in_file_name,
    int in_line, const char* in_method_name, const char* in_method_sig)
{
    if(this->cache_message_logged == true)
    {
        this->_Log(this->cache_log_type.c_str(), this->cache_file_name.c_str(),
            this->cache_line_number, this->cache_method_name.c_str(),
            this->cache_method_signature.c_str(), this->cache_message_string);
        this->cache_message_logged = false;
    }

    if(logger.IsLogVerbose())
    {
        std::string info_msg = 
            std::string("Logging << operator logging of type ") + in_log_type;
        logger.LogInfo(info_msg); 
    }

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
    return *this;
}

Logger& Logger::operator<< (short in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}

Logger& Logger::operator<< (unsigned short in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}

Logger& Logger::operator<< (int in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}
        
Logger& Logger::operator<< (unsigned int in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}
        
Logger& Logger::operator<< (long in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}
        
Logger& Logger::operator<< (unsigned long in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}

Logger& Logger::operator<< (float in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}
        
Logger& Logger::operator<< (double in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}

Logger& Logger::operator<< (long double in_message)
{
    this->cache_message_string += std::to_string(in_message);
    return *this;
}

Logger& Logger::operator<< (const char* in_message)
{
    this->cache_message_string += std::string(in_message);
    return *this;
}

Logger& Logger::operator<< (const std::string& in_message)
{
    this->cache_message_string += in_message;
    return *this;
}

Logger& Logger::operator<< (std::string in_message)
{
    this->cache_message_string += in_message;
    return *this;
}

Logger& Logger::CreateInstance(const std::string& in_group_name)
{
    if(!initialized)
    {
        std::string error_msg =
            "Error in creating logger object, before running this method"
            " 'Init' method call should be successful.";

    	logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }

    // Checking group name string syntax.
    if(!LoggerUtils::IsGroupNameSyntaxOk(in_group_name))
    {
        std::string error_msg =
            "Group name string syntax is not correct. Group name allows only"
            " [a-z] and . chars, Every package/module name should be seperated"
            " with one dot . char only.";

        logger.LogError(error_msg);
	    throw std::runtime_error(error_msg);
    }

    std::map<std::string, Logger*>::iterator groupname_loggerobj_it;
    Logger* out_logger_obj;

    // Checking group name object exist in cache or not. If not creating
    // new object and adding it into cache and returns the same.
    if((groupname_loggerobj_it = groupname_loggerobj_map->find(
        in_group_name)) != groupname_loggerobj_map->end())
    {
        if(logger.IsLogVerbose())
	    {
            logger.LogInfo(
                "Logger object found in cache for group " + in_group_name);
	    }
        out_logger_obj = groupname_loggerobj_it->second; 
    }
    else
    {
        if(logger.IsLogVerbose())
	    {
            logger.LogInfo(
                "Logger object is not present in cache for group" + \
                in_group_name + \
                " creating new one and adding to cache"); 
	    }

        out_logger_obj = new Logger(in_group_name);
        out_logger_obj->group_name = in_group_name;
        out_logger_obj->log_level = LoggerConf::GetLogLevelInt(in_group_name);
        groupname_loggerobj_map->insert(std::pair<std::string, 
            Logger*>(in_group_name, out_logger_obj));

	    if(logger.IsLogVerbose())
	    {
            logger.LogInfo(
                "Logger object created successfully and added to the cache."
                " Log leve for group " + in_group_name + \
                " is " + LoggerConf::GetLogLevelString(in_group_name));
	    }
    }
    return *out_logger_obj;
}


Logger& Logger::CreateInstance(const char* in_group_name)
{
    const std::string group_name_str = std::string(in_group_name);
    return CreateInstance(group_name_str);
}

bool Logger::IsFine()
{
    std::string log_level = LoggerConf::GetLogLevelString(this->group_name);

    if(log_level == "FINEST" || log_level == "FINER" || log_level == "FINE")
    {
        return true;
    }
    return false;
}

bool Logger::IsFiner()
{
    std::string log_level = LoggerConf::GetLogLevelString(this->group_name);

    if(log_level == "FINEST" || log_level == "FINER")
    {
        return true;
    }
    return false;
}

bool Logger::IsFinest()
{
    std::string log_level = LoggerConf::GetLogLevelString(this->group_name);

    if(log_level == "FINEST")
    {
        return true;
    }
    return false;
}

void Logger::_Log(
    const char* in_log_type, 
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& in_message)
{
    if(logger.IsLogVerbose())
    {
        std::string info_msg = std::string("Logging message of type ") + \
            std::string(in_log_type) + std::string(" messge ") + in_message;
        logger.LogInfo(info_msg);
    }

    if(!strcmp(in_log_type, "CRITICAL"))
    {
        this->CriticalLog(in_file_name, in_line_number, in_method_name, 
            in_method_signature, in_message);
    }
    else if(!strcmp(in_log_type, "ERROR"))
    {
        this->ErrorLog(in_file_name, in_line_number, in_method_name,
	        in_method_signature, in_message);
    }
    else if(!strcmp(in_log_type, "WARNING"))
    {
        this->WarningLog(in_file_name, in_line_number, in_method_name,
	        in_method_signature, in_message);
    }
    else if(!strcmp(in_log_type, "INFO"))
    {
        if(!this->IsFine()) return;

        this->InfoLog(in_file_name, in_line_number, in_method_name,
	        in_method_signature, in_message);
    }
    else if(!strcmp(in_log_type, "DEBUG"))
    {
        if(!this->IsFiner()) return;

        this->DebugLog(in_file_name, in_line_number, in_method_name,
	        in_method_signature, in_message);
    }
    else if(!strcmp(in_log_type, "HACK"))
    {
        if(!this->IsFinest()) return;

        this->HackLog(in_file_name, in_line_number, in_method_name,
	        in_method_signature, in_message);
    }
    else
    {
        logger.LogWarning("Unsupported logging type.");
    }
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

bool Logger::Init()
{
    // Logging initialization. It should be first initialization.
    LoggerLogging::Init();

    // Configuration initialization. It should be initialized before
    // accessing any configuration info.
    if(!LoggerConf::Init())
    {
        logger.LogError(
            "Logger configuration failed. Check configuration file exist"
            " or not and have correct syntax.");
    	return false;
    }

    // Handlers initialization.
    if(!LoggerHandler::Init())
    {
        logger.LogError("Error in setting up handler.");
        return false;
    }

    groupname_loggerobj_map = new std::map<std::string, Logger*>();

    if(logger.IsLogVerbose())
    {
        logger.LogInfo("Logger is initialized successfully.");
    }

    initialized = true;
    return true;
}

void Logger::CleanUp()
{
    // Deleting all objects which make sure to call destructor.
    std::map<std::string, Logger*>::iterator str_logger_map_it;
    for(str_logger_map_it = groupname_loggerobj_map->begin();
        str_logger_map_it != groupname_loggerobj_map->end();
        str_logger_map_it++)
    {
        delete str_logger_map_it->second;
    }

    delete groupname_loggerobj_map;
    LoggerHandler::CleanUp();
    LoggerConf::CleanUp();
    LoggerLogging::CleanUp();
}

}    // namespace logger
}    // namespace cpputils
