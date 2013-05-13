/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sat Apr 20 20:00:43 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file declares Logger class.
 
 Logger class provides an API to log message of different types. It provides 
 five types of logging messages as described below.

 Fatal - For Fatal Errors, Always logs and terminate programs.
 Error - For Error Messages, Always logs else logging is not off.
 Warn - For Warning Messages, Always logs else logging is not off.
 Info - For Information Messages, logs if log level is FINE or above.
 Debug - For Debug Messages, logs if log level is FINER or above.
 Hack - For Lot more debug info such as method call arguments, process info,
    thread info etc. logs if log level is FINEST.
 
 Logger divides all log messages into groups and each group is described by
 component, package or module name. 
 
 Group name is represent in '.' format, same as java package name format.
 Group name allows chars [a-z] and '.' .
 Example - cpputils.cpplogger.testclass

 CPPLogger provides four logging level are DEFAULT, FINE, FINER and FINEST 
 in increasing order of logging.

 DEFAULT - Only logs Fatal, Error and Warn messages.
 FINE    - Logs Fatal, Error, Warn and Info messages.
 FINER   - Logs Fatal, Error, Warn, Info and Debug messages.
 FINEST  - Logs Fatal, Error, Warn, Info, Debug and Hack messages.
 
 CPPLogger provides three methods to check logging level for current group.
 IsFine(), IsFiner() and IsFinest().

 These method should be used as if condition to check current logging level 
 and log messages accordingly. It will increase performance as logging function
 call overhead will decrease if logging is not enable at that level.

 If logging level is FINE then IsFine method only return true.
 If logging level is FINER then IsFine and IsFiner method will return true.
 If logging level is FINEST then IsFine, IsFiner and IsFinest method will
 return true.

 Logger uses the pre-defined macros to log messages.
 __FILE__ , __LINE__, __FUNCTION__, __PRETTY_FUNCTION__

 log api syntax is (Each logging api has the same syntax):-

 Fatal(message, args...)
 Error(message, args...)
 Warning(message, args...)
 Info(message, args...)
 Debug(message, args...)
 Hack(message, args...)

 CPPLogger gets more info from pre defined macros and stack trace. We want
 user to insert as less as possible info to enter. So we are asking for only
 message string and message variables.

 For each group only one Logger object will exist. When we call
 Logger::CreateInstance(group_name) Logger checks that Logger object
 exist for given group or not, if it exist return its pointer otherwise create
 new one, store it in cache and returns it.

 **/

#ifndef CPPUTILS_CPPLOGGER_H_
#define CPPUTILS_CPPLOGGER_H_

#if __STDC_VERSION__ < 199901L
    #if __GNUC__ >= 2
        #define __func__ __FUNCTION__
        #define __pretty_func__ __PRETTY_FUNCTION__
    #else
        #define __func__ "<unknown_func>"
        #define __pretty_func__ "<unknow_func_signature>"
    #endif
#endif

// Preprocessor macros used by Logger function call.

#ifdef Critical
#error Critical directive is used by CPPLOGGER. Redefinition of this \
directive can cause unusual behaviour.

#else
#define Critical(X, ...) Log("CRITICAL", __FILE__, __LINE__, __func__, \
    __pretty_func__, X, ##__VA_ARGS__)
#endif

#ifdef Error
#error Error directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.

#else
#define Error(X, ...) Log("ERROR", __FILE__, __LINE__, __func__, \
    __pretty_func__, X, ##__VA_ARGS__)
#endif

#ifdef Warning
#error Warning directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.

#else
#define Warning(X, ...) Log("WARNING", __FILE__, __LINE__, __func__, \
    __pretty_func__, X, ##__VA_ARGS__)
#endif

#ifdef Info
#error Info directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.

#else
#define Info(X, ...) Log("INFO", __FILE__, __LINE__, __func__, \
    __pretty_func__, X, ##__VA_ARGS__)
#endif

#ifdef Debug
#error Debug directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.

#else
#define Debug(X, ...) Log("DEBUG", __FILE__, __LINE__, __func__, \
    __pretty_func__, X, ##__VA_ARGS__)
#endif


#ifdef Hack
#error Hack directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.

#else
#define Hack(X, ...) Log("HACK", __FILE__, __LINE__, __func__, \
    __pretty_func__, X, ##__VA_ARGS__)
#endif

// Preprocessor used by Logger '<<' operator overloading

#ifdef CRITICAL
#error CRITICAL directive is used by CPPLOGGER. Redefinition of this \
directive can cause unusual behaviour.
#else
#define CRITICAL "CRITICAL", __FILE__, __LINE__, __func__, __pretty_func__
#endif

#ifdef ERROR
#error ERROR directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.
#else
#define ERROR "ERROR", __FILE__, __LINE__, __func__, __pretty_func__
#endif

#ifdef WARNING
#error WARNING directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.
#else
#define WARNING "WARNING", __FILE__, __LINE__, __func__, __pretty_func__
#endif

#ifdef INFO
#error INFO directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.
#else
#define INFO "INFO", __FILE__, __LINE__, __func__, __pretty_func__
#endif

#ifdef DEBUG
#error DEBUG directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.
#else
#define DEBUG "DEBUG", __FILE__, __LINE__, __func__, __pretty_func__
#endif

#ifdef HACK
#error HACK directive is used by CPPLOGGER. Redefinition of this directive \
can cause unusual behaviour.
#else
#define HACK "HACK", __FILE__, __LINE__, __func__, __pretty_func__
#endif


#include <execinfo.h>

#include <cstdarg>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#include "cpplogger/cpplogger_logging.h"
#include "cpplogger/cpplogger_utils.h"
#include "cpplogger/cpplogger_conf.h"
#include "cpplogger/cpplogger_handler.h"

namespace cpputils
{
namespace cpplogger
{

class Logger;

class Logger : public LoggerHandler
{
    private:

        static LoggerLogging logger;
        static std::map<std::string, Logger*>* groupname_loggerobj_map;
        std::string group_name;
        int log_level;
        static bool initialized;

        // Variables used for << operator logging
        std::string cache_log_type;
        std::string cache_file_name;
        int cache_line_number;
        std::string cache_method_name;
        std::string cache_method_signature;
        std::string cache_message_string;
        bool cache_message_logged;  

        // @method
        // This coustructor is called by public Init method which creates
        // new Logger object. I am putting this in private block as user
        // can not create Logger object directly.
        // @param in_group_name - string - group name
        Logger(const std::string& in_group_name);

        void CriticalLog(const char* in_file_name, int in_line_number, 
            const char* in_method_name, const char* in_method_signature, 
            std::string& in_unformatted_message);

        void ErrorLog(const char* in_file_name, int in_line_number, 
            const char* in_method_name, const char* in_method_signature, 
            std::string& in_unformatted_message);

        void WarningLog(const char* in_file_name, int in_line_number, 
            const char* in_method_name, const char* in_method_signature, 
            std::string& in_unformatted_message);

        void InfoLog(const char* in_file_name, int in_line_number, 
            const char* in_method_name, const char* in_method_signature, 
            std::string& in_unformatted_message);

        void DebugLog(const char* in_file_name, int in_line_number, 
            const char* in_method_name, const char* in_method_signature, 
            std::string& in_unformatted_message);

        void HackLog(const char* in_file_name, int in_line_number, 
            const char* in_method_name, const char* in_method_signature, 
            std::string& in_unformatted_message);

        void _Log(const char* in_log_type, const char* in_file_name, 
            int in_line_number, const char* in_method_name, 
            const char* in_method_signature, std::string& in_message);
    public:

        // @Method
        // Saves the last unsaved log if exists. 
        ~Logger();

        // @Method
        // Initiates the << operator overloading logging. Stores the
        // log type and other info in current session. Log the message of
        // previous session if exists and flip the cache state to true
        // (Log is ready to save in next session.)
        // @param in_log_type logging type
        // @return Logger& logger object
        Logger& operator() (
            std::string in_log_type, const char* in_file_name,
            int in_line, const char* in_method_name, 
            const char* in_method_sig);
       
        // @Method
        // Different input paramerter types to support << overloading
        // operator
        // @param in_message - different type of input parameter
        // @return Logger& reference of logger object. 
        Logger& operator<< (bool in_message);
        Logger& operator<< (short in_message);
        Logger& operator<< (unsigned short in_message);
        Logger& operator<< (int in_message);
        Logger& operator<< (unsigned int in_message);
        Logger& operator<< (long in_message);
        Logger& operator<< (unsigned long in_message);
        Logger& operator<< (float in_message);
        Logger& operator<< (double in_message);
        Logger& operator<< (long double in_message);
        Logger& operator<< (const char* in_message);
        Logger& operator<< (const std::string& in_message);
        Logger& operator<< (std::string in_message);

        // @method
        // This method creates cpplogger instance which we use to log messages.
        // This method accepts groupname/module name/package name, groupname
        // syntax is like 'package.class.subclass'. It creates Logger object
        // if Logger object was not created before for this groupname, 
        // otherwise it will get it from cache. When logger object will be
        // created it will initialize logger strucutre and store it into
        // cache.
        //
        // @param string - group name, it can not be empty.
        // @return Logger object - logger object for given group
        static Logger& CreateInstance(const std::string& in_group_name);
        static Logger& CreateInstance(const char* in_group_name);
     
        // @method
        // This method checks for current log level and returns true if it is
        // FINE, FINER or FINEST.
        // @return bool - true if log level FINE or greater.     
        bool IsFine();

        // @method
        // This method checks for current log level and returns true if it is
        // FINER or FINEST.
        // @return bool - true if log level FINER or greater.     
        bool IsFiner();

        // @method
        // This method checks for current log level and returns true if it is
        // FINEST.
        // @return bool - true if log level FINEST.     
        bool IsFinest();

        // @method
        // Don't call this method directly, use macros defined above. These
        // macros take some of file information automatically such as file 
        // name, line number, method name and method signature. So ideally
        // it has one input argument 'message string' and variable arguments
        // for message string variables.
        // @param string - message string
        void Log(const char* in_log_type, const char* in_file_name, 
            int in_line_number, const char* in_method_name, 
            const char* in_method_signature, std::string& in_message, ...);
        void Log(const char* in_log_type, const char* in_file_name, 
            int in_line_number, const char* in_method_name, 
            const char* in_method_signature, const char* in_message, ...);
        
        // @method
        // This is the first method application needs to call, before calling
        // this method application can not use cpplogger. If initialization 
        // was successful it will set 'initialized' flag to true otherwise 
        // it will false. Means 'initialized' flag should be set to ture
        // before procedding with cpplogger.
        // It initializes all the data structures used in cpplogger, read
        // the configuration file which initializes logging level for groups
        // and set output handlers.
        // Handlers name and corresponding library path should be set in
        // configuration file. Handlers name should be seperated by space. 
        // For example - If you want to enable file handler, Add line -
        // AddHandler file /path/to/handler/shared/lib/libfile.so 
        // @return bool - initialization was successful or not.
        static bool Init();

        // @method
        // Free all memory used in logger caching. Flush all open files (
        // logger configuration file, logger logging file etc), and close them.
        static void CleanUp();
};

}    // namespace logger
}    // namespace cpputils
#endif 
