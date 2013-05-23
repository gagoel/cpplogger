/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_file_handler.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Thu May  2 00:05:54 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 Following are the configuration settings used by this handler, which can
 be set directly in the configuration file.
  
 Properties which supported by this handler :-
 LogFile            - Log file path
 LogFormat          - Log fromat which will be used if log type format is 
                      not present.
 LogCriticalFormat  - Log format for critical type logs.
 LogErrorFormat     - Log format for error type logs.
 LogWarningFormat   - Log format for warning type logs.
 LogInfoFormat      - Log format for info type logs.
 LogDebugFormat     - Log format for debug type logs.
 LogHackFormat      - Log format for hack type logs.

 
 All Log formats support following specifier to get variable value.
 
 %timestamp         - YYYY-MM-DDThh:mm:ssTZD (eg 1997-07-16T19:20:30+01:00)
 %type              - Log type CRITICAL, ERROR, WARNING, INFO, DEBUG, HACK
 %package           - Package name
 %file              - File name
 %line              - line number
 %class             - Class name
 %method            - Method name
 %methodsignature   - Method Signature
 %message           - log message

 Debug Log format support following additional specifier to get variable value.

 %debugstack        - Add debug stack trace.

 **/

#include "cpplogger/cpplogger_logging.h"
#include "cpplogger/cpplogger_handler.h"

namespace cpputils
{
namespace cpplogger
{

class LoggerFileHandler
{
    private :
    
        static LoggerLogging logger;

        static bool initialized;

        static std::string log_file_name;
        static std::fstream* log_file_stream;
        static std::string GetLogFileName();

        // Log formats data structures.
        static std::string default_log_format;        
        static std::map<std::string, std::string>* log_formats_map;
 
        // Log format specifier data structure and getter setter methods.
        static std::map<std::string, std::string>* log_format_specifiers_map;
        static void SetCommonFormatSpecifiers(const LogDB& in_log_db);

        // @method
        // Get format specifier.
        // @param in_specifier_name specifier name string.
        // @return specifier value string.
        static std::string GetFormatSpecifier(
            const std::string& in_specifier_name);

    public :

        // @method
        // Parses input log format string as per format specifier set by
        // Print methods.
        // @param out_parsed_log log string after parsing input string
        // @param in_log_format input log string which needs to be parsed. 
        static void ParseLogFormat(std::string& out_parsed_log,
            const std::string& in_log_format);

        // @method
        // Set log file name, Initialization needs to be done after setting
        // log file. If file handler was initialized before calling this
        // method you need to run cleanup before this method.
        // @param in_log_file log file string. 
        static void SetLogFileName(const std::string& in_log_file);
        
        static bool GetLogFileStream(std::fstream** out_log_file_stream);
        
        // @method
        // Log format exists in cache or not.
        // @param in_log_format log format string - LogFormat LogCriticalFormat
        // LogErrorFormat, LogWarningFormat, LogInfoFormat, LogDebugFormat,
        // LogHackFormat.
        // @return true/false format exists or not.
        static bool IsLogFormatExists(const std::string& in_log_format);
        
        // @method
        // Call back method used by handler to check log format property value 
        // validation.
        // @param in_log_format_value format value string.
        // @return true/false log format is valid or not.
        static bool IsValidLogFormat(const std::string& in_log_format_value);

        // @method
        // Sets log format.
        // @param in_log_format_name log format name string.
        // @param in_log_format_value log format value string.
        static void SetLogFormat(const std::string& in_log_format_name,
            const std::string& in_log_format_value);
        
        // @method
        // Get log format value for input format name.
        // @param in_log_format_name format name string.
        // @return log format string.
        static std::string GetLogFormat(const std::string& in_log_format_name);
        
        // @method
        // Set format specifier.
        // @param in_specifier_name specifier name string.
        // @param in_specifier_value specifier value string.
        static void SetFormatSpecifier(
            const std::string& in_specifier_name, 
            const std::string& in_specifier_value);

        // @method
        // Call back method used by handler to check log file property value
        // validation.
        // @param in_log_file log file string
        // @return true/false log file name is valid or not.
        static bool IsValidLogFileName(const std::string& in_log_file);
        
        // @method
        // Saves critical log into log file as per critical log format, set
        // at initialization time.
        // @param in_critical_log_db struct stores all data related to 
        // critical log. 
        static void PrintCriticalLog(const CriticalLogDB& in_critical_log_db);
        
        // @method
        // Saves error log into log file as per error log format, set
        // at initialization time.
        // @param in_error_log_db struct stores all data related to 
        // error log. 
        static void PrintErrorLog(const ErrorLogDB& in_error_log_db);
        
        // @method
        // Saves warning log into log file as per warning log format, set
        // at initialization time.
        // @param in_warning_log_db struct stores all data related to 
        // warning log. 
        static void PrintWarningLog(const WarningLogDB& in_warning_log_db);
        
        // @method
        // Saves info log into log file as per info log format, set
        // at initialization time.
        // @param in_info_log_db struct stores all data related to 
        // info log. 
        static void PrintInfoLog(const InfoLogDB& in_info_log_db);
        
        // @method
        // Saves debug log into log file as per debug log format, set
        // at initialization time.
        // @param in_debug_log_db struct stores all data related to 
        // debug log. 
        static void PrintDebugLog(const DebugLogDB& in_debug_log_db);
        
        // @method
        // Saves hack log into log file as per hack log format, set
        // at initialization time.
        // @param in_hack_log_db struct stores all data related to 
        // hack log. 
        static void PrintHackLog(const HackLogDB& in_hack_log_db);
        
        // @method
        // Registers following properties in configuration.
        // LogFile, LogFormat, LogCriticalFormat, LogErrorFormat,
        // LogWarningFormat, LogInfoFormat, LogDebugFormat,
        // LogHackFormat
        // Initializes all data strucuters.
        // Get all properties value from configuration, if not define
        // in configuration file, set it to default value.
        //
        // Open log file and save file descriptor.
        // Set all log formats value.
        // @return true/false initization was successful or not.  
        static bool Init(const std::string& in_app_name);

        // @method
        // Close log file, free all heap memory, unregister all properties.
        static void CleanUp(const std::string& in_app_name);
};

}    // namespace cpplogger
}    // namespace cpputils
