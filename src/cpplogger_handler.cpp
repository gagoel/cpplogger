/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_handler.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed Apr 24 17:37:39 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 Implements LoggerHandler class.

 **/


#include "cpplogger/cpplogger_handler.h"

namespace cpputils
{
namespace cpplogger
{

LoggerLogging LoggerHandler::logger("cpputils.cpplogger.LoggerHandler");

std::map<std::string, OutputHandler*>* 
    LoggerHandler::handler_name_struct_map = NULL;

bool LoggerHandler::LoadHandler(const std::string& in_handler_name, 
    const std::string& in_handler_lib_path)
{
    if(in_handler_name.size() == 0 || in_handler_lib_path.size() == 0)
    {
        logger.LogError(
            "Handler name string is empty or handler library path is empty.");
        return false;
    }
    
    // Cleaning existing dl error if any exist.
    dlerror();

    void *os_handle;
    os_handle = dlopen(in_handler_lib_path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    
    if(os_handle == NULL)
    {
        std::string error_msg = "Error in loading library " + \
            in_handler_lib_path + " dlerror : " + dlerror();
        logger.LogError(error_msg);
        return false;
    }

    // Creating handler symbolic name from input handler name.
    std::string handler_sym = in_handler_name + \
        std::string("_cpplogger_handler");

    OutputHandler* output_handler = 
        (OutputHandler*) dlsym(os_handle, handler_sym.c_str());

    char* dlsym_error;
    if((dlsym_error = dlerror()) != NULL)
    {
        std::string error_msg = "Error in getting symbol " + \
            in_handler_name + " dlerror : " + std::string(dlsym_error);
        logger.LogError(error_msg);
        return false;
    }
    
    (*handler_name_struct_map)[in_handler_name] = output_handler;
    return true;
}

bool LoggerHandler::AddHandler(const std::string& in_handler_name,
    const std::string& in_handler_lib_path)
{
    if(!LoadHandler(in_handler_name, in_handler_lib_path))
    {
        std::string error_msg = "Error in loading handler " + \
            in_handler_name + " from lib path " + in_handler_lib_path;
        logger.LogError(error_msg);
        return false;
    }

    // Running handler initialization.
    OutputHandler* curr_output_handler = 
        handler_name_struct_map->find(in_handler_name)->second;

    if(curr_output_handler->Init != NULL)
    {
        if(curr_output_handler->Init() == false)
        {
            logger.LogWarning("Initialization falied for handler.");
            return false;
        }
    }
    return true;
}

void LoggerHandler::RemoveHandler(std::string& in_handler_name)
{
    if(in_handler_name.size() == 0)
    {
        logger.LogError("Handler name string can not be empty.");
        return;
    }

    if(handler_name_struct_map->erase(in_handler_name) != 1)
    {
        std::string  warning_msg = in_handler_name + " handler does not have \
            entry in handler_name_struct_map";
        logger.LogWarning(warning_msg);
    }
}

void LoggerHandler::ExecuteCriticalHandlers(
    CriticalLogDB& in_critical_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = handler_name_struct_map->begin();
        map_it != handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintCriticalLog != NULL)
        {
            map_it->second->PrintCriticalLog(in_critical_log_struct);
        }
    }
}

void LoggerHandler::ExecuteErrorHandlers(ErrorLogDB& in_error_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = handler_name_struct_map->begin();
        map_it != handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintErrorLog != NULL)
        {
            map_it->second->PrintErrorLog(in_error_log_struct);
        }
    }
}

void LoggerHandler::ExecuteWarningHandlers(WarningLogDB& in_warning_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = handler_name_struct_map->begin();
        map_it != handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintWarningLog != NULL)
        {
            map_it->second->PrintWarningLog(in_warning_log_struct);
        }
    }
}

void LoggerHandler::ExecuteInfoHandlers(InfoLogDB& in_info_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = handler_name_struct_map->begin();
        map_it != handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintInfoLog != NULL)
        {
            map_it->second->PrintInfoLog(in_info_log_struct);
        }
    }
}

void LoggerHandler::ExecuteDebugHandlers(DebugLogDB& in_debug_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = handler_name_struct_map->begin();
        map_it != handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintDebugLog != NULL)
        {
            map_it->second->PrintDebugLog(in_debug_log_struct);
        }
    }
}

void LoggerHandler::ExecuteHackHandlers(HackLogDB& in_hack_log_struct)
{
    // Running output handlers.
    std::map<std::string, OutputHandler*>::iterator map_it;
    for(map_it = handler_name_struct_map->begin();
        map_it != handler_name_struct_map->end();
        map_it++)
    {
        if(map_it->second->PrintHackLog != NULL)
        {
            map_it->second->PrintHackLog(in_hack_log_struct);
        }
    }
}

bool LoggerHandler::Init()
{
    handler_name_struct_map = new std::map<std::string, OutputHandler*>();

    std::map<std::string, std::string> str_str_map =
        LoggerConf::GetHandlers();

    std::map<std::string, std::string>::iterator str_str_map_it;
    for(str_str_map_it = str_str_map.begin();
        str_str_map_it != str_str_map.end();
        str_str_map_it++)
    {
        if(!AddHandler(str_str_map_it->first, str_str_map_it->second))
        {
            return false;
        }
    }
    return true;
}

void LoggerHandler::CleanUp()
{
    std::map<std::string, OutputHandler*>::iterator str_handler_map_it;

    for(str_handler_map_it = handler_name_struct_map->begin();
        str_handler_map_it != handler_name_struct_map->end();
        str_handler_map_it++)
    {
        if(str_handler_map_it->second->CleanUp != NULL)
        {
            str_handler_map_it->second->CleanUp();
        }
    }

    delete handler_name_struct_map;
}

}    // namespace cpplogger
}    // namespace cpputils

