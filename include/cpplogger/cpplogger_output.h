/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_output.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Wed May 22 01:13:31 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file has the defintion of LoggerOutput class. 
 LoggerOutput class is used to output logging message by cpplogger.
 
 **/

#ifndef CPPUTILS_CPPLOGGER_OUTPUT_H_
#define CPPUTILS_CPPLOGGER_OUTPUT_H_

#include <execinfo.h>
#include <dlfcn.h>

#include <iostream>
#include <map>
#include <string>

#include "cpplogger/cpplogger_logging.h"
#include "cpplogger/cpplogger_handler.h"
#include "cpplogger/cpplogger_config.h"

namespace cpputils { namespace cpplogger {

class LoggerOutput
{
    private:

        static LoggerLogging logger;
        static bool initialized;
        static std::map<std::string, LoggerOutput*>* 
            app_name_logger_output_obj_map;

        std::string appname; 
        std::map<std::string, OutputHandler*>* output_handler_name_struct_map;

        // Checks for the input handler library existence and if it does not
        // exist returns false. 
        // Output Handler symbol name constructs from input Handler name and 
        // it will be :
        // <input handler name>_cpplogger_handler
        // 
        // If this symbol name does not exist in the input library, returns
        // false otherwise load handler symbol into cache map.
        bool _LoadOutputHandler(
            const std::string& in_output_handler_name,
            const std::string& in_output_handler_lib_path
        );

        // @method
        // Add handler to the cache map.
        // handler name and handler library path string can not be empty.
        // Input library path should be exist and accessible and it should
        // have in_handler_name_cpplogger_handler symbol.
        // @param in_handler_name handler name string can not empty
        // @param in_handler_lib_path handler library should exist and should
        // be accessible.
        // @return true/false handler symbol is loaded successful or not. 
        bool _AddOutputHandler(
            const std::string& in_handler_name,
            const std::string& in_handler_lib_path
        );

        // @method
        // Remove input handler from cache if it exist.
        // @param in_handler_name handler name string
        void _RemoveOutputHandler(const std::string& in_handler_name);

        // @method
        // Executes critical methods for all loaded handlers 
        // @param in_critical_log_db critical log data structure
        void _ExecuteCriticalOutputHandlers(
            CriticalLogDB& in_critical_log_db
        );
        
        // @method
        // Executes error methods for all loaded handlers 
        // @param in_error_log_db error log data structure
        void _ExecuteErrorOutputHandlers(
            ErrorLogDB& in_error_log_db
        );
        
        // @method
        // Executes warning methods for all loaded handlers 
        // @param in_warning_log_db warning log data structure
        void _ExecuteWarningOutputHandlers(
            WarningLogDB& in_warning_log_db
        );
        
        // @method
        // Executes info methods for all loaded handlers 
        // @param in_info_log_db info log data structure
        void _ExecuteInfoOutputHandlers(
            InfoLogDB& in_info_log_db
        );
        
        // @method
        // Executes debug methods for all loaded handlers 
        // @param in_debug_log_db debug log data structure
        void _ExecuteDebugOutputHandlers(
            DebugLogDB& in_debug_log_db
        );

        // @method
        // Executes hack methods for all loaded handlers 
        // @param in_hack_log_db hack log data structure
        void _ExecuteHackOutputHandlers(
            HackLogDB& in_hack_log_db
        );

        LoggerOutput(const std::string& in_app_name);

    public:
       
        ~LoggerOutput(); 
        
        // @method
        // Initialize all handler data strucute. Load all the handlers
        // mentioned in the configuration file with syntax -
        // AddHandler   handler_name  handler_lib_name
        // @return true/false handlers initialized successful or not.
        static bool Init();

        // @method
        // Clean up all the heap memory initialized by handler.
        static void CleanUp();
        
        // @method
        static LoggerOutput& GetLoggerOutputObject(
            const std::string in_app_name
        );

        bool AddOutputHandler(
            const std::string& in_handler_name,
            const std::string& in_handler_lib_path
        );
        
        void OutputCriticalLog(
            const std::string& group_name,
            const char* in_file_name, 
            int in_line_number, 
            const char* in_method_name, 
            const char* in_method_signature, 
            std::string& in_unformatted_message
        );

        void OutputErrorLog(
            const std::string& group_name,
            const char* in_file_name, 
            int in_line_number, 
            const char* in_method_name, 
            const char* in_method_signature, 
            std::string& in_unformatted_message
        );

        void OutputWarningLog(
            const std::string& group_name,
            const char* in_file_name, 
            int in_line_number, 
            const char* in_method_name, 
            const char* in_method_signature, 
            std::string& in_unformatted_message
        );

        void OutputInfoLog(
            const std::string& group_name,
            const char* in_file_name, 
            int in_line_number, 
            const char* in_method_name, 
            const char* in_method_signature, 
            std::string& in_unformatted_message
        );

        void OutputDebugLog(
            const std::string& group_name,
            const char* in_file_name, 
            int in_line_number, 
            const char* in_method_name, 
            const char* in_method_signature, 
            std::string& in_unformatted_message
        );

        void OutputHackLog(
            const std::string& group_name,
            const char* in_file_name, 
            int in_line_number, 
            const char* in_method_name, 
            const char* in_method_signature, 
            std::string& in_unformatted_message
        );
};

}    // namespace cpplogger
}    // namespace cpputils

#endif
