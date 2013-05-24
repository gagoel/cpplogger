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

#include "cpplogger/cpplogger_config.h"

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
    const std::string& appname;
    const std::string& package_name;
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
    bool (*Init) (const std::string& in_app_name);
    void (*CleanUp) (const std::string& in_app_name);
    void (*PrintCriticalLog) (const CriticalLogDB& critical_log_db);
    void (*PrintErrorLog) (const ErrorLogDB& error_log_db);
    void (*PrintWarningLog) (const WarningLogDB& warning_log_db);
    void (*PrintInfoLog) (const InfoLogDB& info_log_db);
    void (*PrintDebugLog) (const DebugLogDB& debug_log_db);
    void (*PrintHackLog) (const HackLogDB& hack_log_db);
}OutputHandler;

}    // namespace cpplogger
}    // namespace cpputils
#endif
