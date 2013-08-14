/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_output.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed May 22 01:13:18 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file implements LoggerOutput class. 

 **/

#include "cpputils/cpplogger/cpplogger_output.h"

namespace cpputils { namespace cpplogger {
    
LoggerLogging LoggerOutput::logger("cpputils.cpplogger.LoggerOutput");
bool LoggerOutput::initialized = false;
std::map<std::string, LoggerOutput*>* 
    LoggerOutput::app_name_logger_output_obj_map = NULL;

bool LoggerOutput::_LoadOutputHandler(
    const std::string& in_output_handler_name, 
    const std::string& in_output_handler_lib_path
)
{
    if(in_output_handler_name.size() == 0 || 
        in_output_handler_lib_path.size() == 0)
    {
        logger.LogError(
            "Handler name string is empty or handler library path is empty.");
        return false;
    }
    
    // Cleaning existing dl error if any exist.
    dlerror();

    void *os_handle;
    os_handle = dlopen(in_output_handler_lib_path.c_str(), 
        RTLD_NOW | RTLD_GLOBAL);
    
    if(os_handle == NULL)
    {
        std::string error_msg = "Error in loading library " + \
            in_output_handler_lib_path + " dlerror : " + dlerror();
        logger.LogError(error_msg);
        return false;
    }

    // Creating handler symbolic name from input handler name.
    std::string handler_sym = in_output_handler_name + \
        std::string("_cpplogger_handler");

    OutputHandler* output_handler = 
        (OutputHandler*) dlsym(os_handle, handler_sym.c_str());

    char* dlsym_error;
    if((dlsym_error = dlerror()) != NULL)
    {
        std::string error_msg = "Error in getting symbol " + \
            in_output_handler_name + " dlerror : " + std::string(dlsym_error);
        logger.LogError(error_msg);
        return false;
    }
    
    (*this->output_handler_name_struct_map)[in_output_handler_name] = 
        output_handler;
        
    return true;
}

bool LoggerOutput::_AddOutputHandler(
    const std::string& in_output_handler_name,
    const std::string& in_output_handler_lib_path
)
{
    if(!_LoadOutputHandler(in_output_handler_name, in_output_handler_lib_path))
    {
        std::string error_msg = "Error in loading handler " + \
            in_output_handler_name + " from lib path " + \
            in_output_handler_lib_path;
        logger.LogError(error_msg);
        return false;
    }

    // Running handler initialization.
    OutputHandler* curr_output_handler = 
        this->output_handler_name_struct_map->find(
        in_output_handler_name)->second;

    if(curr_output_handler->Init != NULL)
    {
        if(curr_output_handler->Init(this->appname) == false)
        {
            logger.LogWarning("Initialization falied for handler.");
            return false;
        }
    }
    return true;
}

void LoggerOutput::_RemoveOutputHandler(
    const std::string& in_output_handler_name
)
{
    if(in_output_handler_name.size() == 0)
    {
        logger.LogError("Handler name string can not be empty.");
        return;
    }

    // Running handler cleanup.
    OutputHandler* curr_output_handler = 
        this->output_handler_name_struct_map->find(
        in_output_handler_name)->second;

    if(curr_output_handler->CleanUp != NULL)
    {
        curr_output_handler->CleanUp(this->appname);
    }

    // Removing handler entry from cache map.
    if(this->output_handler_name_struct_map->erase(
        in_output_handler_name) != 1)
    {
        std::string  warning_msg = in_output_handler_name + 
            " handler does not have entry in output_handler_name_struct_map";
        logger.LogWarning(warning_msg);
    }
}

void LoggerOutput::_ExecuteCriticalOutputHandlers(
    CriticalLogDB& in_critical_log_struct
)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = this->output_handler_name_struct_map->begin();
        map_it != this->output_handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintCriticalLog != NULL)
        {
            map_it->second->PrintCriticalLog(in_critical_log_struct);
        }
    }
}

void LoggerOutput::_ExecuteErrorOutputHandlers(ErrorLogDB& in_error_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = this->output_handler_name_struct_map->begin();
        map_it != this->output_handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintErrorLog != NULL)
        {
            map_it->second->PrintErrorLog(in_error_log_struct);
        }
    }
}

void LoggerOutput::_ExecuteWarningOutputHandlers(
    WarningLogDB& in_warning_log_struct
)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = this->output_handler_name_struct_map->begin();
        map_it != this->output_handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintWarningLog != NULL)
        {
            map_it->second->PrintWarningLog(in_warning_log_struct);
        }
    }
}

void LoggerOutput::_ExecuteInfoOutputHandlers(InfoLogDB& in_info_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = this->output_handler_name_struct_map->begin();
        map_it != this->output_handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintInfoLog != NULL)
        {
            map_it->second->PrintInfoLog(in_info_log_struct);
        }
    }
}

void LoggerOutput::_ExecuteDebugOutputHandlers(DebugLogDB& in_debug_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = this->output_handler_name_struct_map->begin();
        map_it != this->output_handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintDebugLog != NULL)
        {
            map_it->second->PrintDebugLog(in_debug_log_struct);
        }
    }
}

void LoggerOutput::_ExecuteHackOutputHandlers(HackLogDB& in_hack_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = this->output_handler_name_struct_map->begin();
        map_it != this->output_handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintHackLog != NULL)
        {
            map_it->second->PrintHackLog(in_hack_log_struct);
        }
    }
}

LoggerOutput::LoggerOutput(const std::string& in_app_name)
{
    this->appname = in_app_name;
    this->output_handler_name_struct_map = 
        new std::map<std::string, OutputHandler*>();
}

// Public methods.
LoggerOutput::~LoggerOutput() 
{
    std::map<std::string, OutputHandler*>::iterator str_handler_map_it;

    for(str_handler_map_it = this->output_handler_name_struct_map->begin();
        str_handler_map_it != this->output_handler_name_struct_map->end();
        str_handler_map_it++)
    {
        this->_RemoveOutputHandler(str_handler_map_it->first);
    }

    delete this->output_handler_name_struct_map;
}

bool LoggerOutput::Init()
{
    if(initialized == false)
    {
        LoggerLogging::Init();

        app_name_logger_output_obj_map =
            new std::map<std::string, LoggerOutput*>();

        initialized = true;
    }
    return true;
}

void LoggerOutput::CleanUp()
{
    logger.LogInfo("Cleaning logger output");

    if(initialized == true)
    {
        std::map<std::string, LoggerOutput*>::iterator str_logger_output_it;

        for(str_logger_output_it = app_name_logger_output_obj_map->begin();
            str_logger_output_it != app_name_logger_output_obj_map->end();
            str_logger_output_it++)
        {
            delete str_logger_output_it->second;
        }

        delete app_name_logger_output_obj_map;
        LoggerLogging::CleanUp();
        initialized = false;
    }

    logger.LogInfo("logger output cleaned successfully.");
}

LoggerOutput& LoggerOutput::GetLoggerOutputObject(
    const std::string in_app_name
)
{
    if(in_app_name.size() == 0)
    {
        std::string error_msg =
            "Application name can not be empty string.";
        logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }

    // Searching in cache.
    if(app_name_logger_output_obj_map == NULL)
    {
        std::string error_msg =
            "app_name_logger_output_obj_map for application " +
            in_app_name +
            "is not initialized yet. Run LoggerOutput::Init method " +
            "before calling this method.";
        logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }

    if(app_name_logger_output_obj_map == NULL)
    {
        std::string error_msg = 
            "logger output object was not initialized yet.";
        logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }


    std::map<std::string, LoggerOutput*>::iterator str_logger_output_it;
    if((str_logger_output_it = app_name_logger_output_obj_map->find(
        in_app_name)) != app_name_logger_output_obj_map->end())
    {
        logger.LogInfo("Logger Output object found in cache.");
        return *str_logger_output_it->second;
    }

    logger.LogInfo(
        "Logger Output object did not found in cache for application " + \
        in_app_name + ", creating new one.");

    LoggerOutput* logger_output_obj = new LoggerOutput(in_app_name);

    // Running output handlers for this application added in configuration 
    // file.
    std::map<std::string, std::string> str_str_map =
        LoggerConfig::GetLoggerConfigObject(in_app_name).GetHandlers();

    logger.LogInfo("Fetched output handlers from configuration successfully.");

    if(str_str_map.size() != 0)
    {
        std::map<std::string, std::string>::iterator str_str_map_it;
        for(str_str_map_it = str_str_map.begin();
            str_str_map_it != str_str_map.end();
            str_str_map_it++)
        {
            if(!logger_output_obj->_AddOutputHandler(str_str_map_it->first, 
                str_str_map_it->second))
            {
                std::string warn_msg =
                    "Output handler was not added successfully."
                    "Handler name is " + str_str_map_it->first +
                    "Handler lib path is " + str_str_map_it->second;
                logger.LogWarning(warn_msg);
            }
        }
    }

    (*app_name_logger_output_obj_map)[in_app_name] = logger_output_obj;

    logger.LogInfo("Returning the logger output object.");

    return *logger_output_obj;
}

bool LoggerOutput::AddOutputHandler(
    const std::string& in_output_handler_name,
    const std::string& in_output_handler_lib_path
)
{
    return this->_AddOutputHandler(in_output_handler_name, 
        in_output_handler_lib_path);
}

void LoggerOutput::OutputCriticalLog(
    const std::string& group_name,
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string
)
{
    // Creating stack trace.
    const int max_backtrace_len = 50;
    void *backtrace_buff[max_backtrace_len];
    size_t backtrace_size;
    char **backtrace_strings;

    backtrace_size = backtrace(backtrace_buff, max_backtrace_len);
    backtrace_strings = backtrace_symbols(backtrace_buff, backtrace_size);

    LogDB log_struct = {
        this->appname,
        group_name,
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

    this->_ExecuteCriticalOutputHandlers(critical_log_struct);
  
    // Freeing stack trace memory created by backtrace function
    free(backtrace_strings); 

    std::terminate();
}

void LoggerOutput::OutputErrorLog(
    const std::string& group_name,
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->appname,
        group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    ErrorLogDB error_log_struct = {
        log_struct
    };

    this->_ExecuteErrorOutputHandlers(error_log_struct);
}

void LoggerOutput::OutputWarningLog(
    const std::string& group_name,
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->appname,
        group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    WarningLogDB warning_log_struct = {
        log_struct
    };

    this->_ExecuteWarningOutputHandlers(warning_log_struct);
}

void LoggerOutput::OutputInfoLog(
    const std::string& group_name,
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->appname,
        group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    InfoLogDB info_log_struct = {
        log_struct
    };

    this->_ExecuteInfoOutputHandlers(info_log_struct);
}

void LoggerOutput::OutputDebugLog(
    const std::string& group_name,
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->appname,
        group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    DebugLogDB debug_log_struct = {
        log_struct
    };

    this->_ExecuteDebugOutputHandlers(debug_log_struct);
}

void LoggerOutput::OutputHackLog(
    const std::string& group_name,
    const char* in_file_name, 
    int in_line_number, 
    const char* in_method_name, 
    const char* in_method_signature,
    std::string& unformatted_string)
{
    LogDB log_struct = {
        this->appname,
        group_name,
        in_file_name,
        in_line_number,
        in_method_name,
        in_method_signature,
        unformatted_string
    };

    HackLogDB hack_log_struct = {
        log_struct
    };

    this->_ExecuteHackOutputHandlers(hack_log_struct);
}

}    // namespace cpplogger
}    // namespace cpputils**/
