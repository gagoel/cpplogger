/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_handler.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed Apr 24 17:37:48 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file declares LoggerHandler class. LoggerHandler class declares all 
 data structures and methods require to create handler. This file is a single
 interface to create cpplogger handler. 
 This class inherits LoggerConf class to access cpplogger configuration. 

 **/

#ifndef CPPUTILS_CPPLOGGER_HANDLER_H_
#define CPPUTILS_CPPLOGGER_HANDLER_H_

#include <dlfcn.h>

#include <iostream>
#include <map>
#include <string>

#include "cpplogger/cpplogger_logging.h"
#include "cpplogger/cpplogger_conf.h"

// Handler registers handler by using preprocessor CPPLOGGER_DECLARE_HANDLER
// Example -
// CPPLOGGER_DECLARE_HANDLER(handler_name) = {
//    Init,
//    CleanUp,
//    PrintCriticalLog,
//    PrintErrorLog,
//    PrintWarningLog,
//    PrintInfoLog,
//    PrintDebugLog,
//    PrintHackLog
// };
// Handler methods syntax are :-
//    bool (*Init) ();
//    void (*CleanUp) ();
//    void (*PrintCriticalLog) (const CriticalLogDB& critical_log_db);
//    void (*PrintErrorLog) (const ErrorLogDB& error_log_db);
//    void (*PrintWarningLog) (const WarningLogDB& warning_log_db);
//    void (*PrintInfoLog) (const InfoLogDB& info_log_db);
//    void (*PrintDebugLog) (const DebugLogDB& debug_log_db);
//    void (*PrintHackLog) (const HackLogDB& hack_log_db);

#define CPPLOGGER_DECLARE_HANDLER(name) \
    cpputils::cpplogger::OutputHandler name ## _cpplogger_handler

namespace cpputils
{
namespace cpplogger
{

typedef struct _log_db
{
    std::string& package_name;
    const char* file_name;
    int line_number;
    const char* method_name;
    const char* method_signature;
    std::string& message;
}LogDB;

typedef struct _critical_log_db
{
    LogDB common_properties;
    char **backtrace_strings;
    size_t backtrace_strings_size;
}CriticalLogDB;

typedef struct _error_log_db
{
    LogDB common_properties;
}ErrorLogDB;

typedef struct _warning_log_db
{
    LogDB common_properties;
}WarningLogDB;

typedef struct _info_log_db
{
    LogDB common_properties;
}InfoLogDB;

typedef struct _debug_log_db
{
    LogDB common_properties;
}DebugLogDB;

typedef struct _hack_log_db
{
    LogDB common_properties;
}HackLogDB;

typedef struct _output_handler
{
    bool (*Init) ();
    void (*CleanUp) ();
    void (*PrintCriticalLog) (const CriticalLogDB& critical_log_db);
    void (*PrintErrorLog) (const ErrorLogDB& error_log_db);
    void (*PrintWarningLog) (const WarningLogDB& warning_log_db);
    void (*PrintInfoLog) (const InfoLogDB& info_log_db);
    void (*PrintDebugLog) (const DebugLogDB& debug_log_db);
    void (*PrintHackLog) (const HackLogDB& hack_log_db);
}OutputHandler;

class LoggerHandler : public LoggerConf
{
    private :
   
        static LoggerLogging logger; 

        // Handler name - handler struct map and getter setter methods.
        static std::map<std::string, OutputHandler*>* handler_name_struct_map;

        // Checks for the input handler library existence and if it does not
        // exist returns false. Handler symbol name constructs from input
        // Handler name and it will be in_handler_name_cpplogger_handler
        // where in_handler_name is handler name.
        // If this symbol name does not exist in the input library, returns
        // false otherwise load handler symbol into cache map.
        static bool LoadHandler(const std::string& in_handler_name,
            const std::string& in_handler_lib_path);

    public :

        // @method
        // Add handler to the cache map.
        // handler name and handler library path string can not be empty.
        // Input library path should be exist and accessible and it should
        // have in_handler_name_cpplogger_handler symbol.
        // @param in_handler_name handler name string can not empty
        // @param in_handler_lib_path handler library should exist and should
        // be accessible.
        // @return true/false handler symbol is loaded successful or not. 
        static bool AddHandler(const std::string& in_handler_name,
            const std::string& in_handler_lib_path);

        // @method
        // Remove input handler from cache if it exist.
        // @param in_handler_name handler name string
        static void RemoveHandler(std::string& in_handler_name);

        // @method
        // Executes critical methods for all loaded handlers 
        // @param in_critical_log_db critical log data structure
        static void ExecuteCriticalHandlers(CriticalLogDB& in_critical_log_db);
        
        // @method
        // Executes error methods for all loaded handlers 
        // @param in_error_log_db error log data structure
        static void ExecuteErrorHandlers(ErrorLogDB& in_error_log_db);
        
        // @method
        // Executes warning methods for all loaded handlers 
        // @param in_warning_log_db warning log data structure
        static void ExecuteWarningHandlers(WarningLogDB& in_warning_log_db);
        
        // @method
        // Executes info methods for all loaded handlers 
        // @param in_info_log_db info log data structure
        static void ExecuteInfoHandlers(InfoLogDB& in_info_log_db);
        
        // @method
        // Executes debug methods for all loaded handlers 
        // @param in_debug_log_db debug log data structure
        static void ExecuteDebugHandlers(DebugLogDB& in_debug_log_db);

        // @method
        // Executes hack methods for all loaded handlers 
        // @param in_hack_log_db hack log data structure
        static void ExecuteHackHandlers(HackLogDB& in_hack_log_db);
        
        // @method
        // Initialize all handler data strucute. Load all the handlers
        // mentioned in the configuration file with syntax -
        // AddHandler   handler_name  handler_lib_name
        // @return true/false handlers initialized successful or not.
        static bool Init();

        // @method
        // Clean up all the heap memory initialized by handler.
        static void CleanUp();

};

}    // namespace cpplogger
}    // namespace cpputils
#endif
