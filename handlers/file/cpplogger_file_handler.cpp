/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_file_handler.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Thu May  2 00:05:49 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 Implements LoggerFileHandler class.

 **/


#include <cstdarg>
#include <ctime>
#include <cctype>

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

#include "cpputils/cpplogger/cpplogger_handler.h"
#include "cpplogger_file_handler.h"

namespace cpputils
{
namespace cpplogger
{

LoggerLogging LoggerFileHandler::logger(
    "cpputils.cpplogger.LoggerFileHandler"); 

bool LoggerFileHandler::initialized = false;

std::string LoggerFileHandler::log_file_name = "";
std::fstream* LoggerFileHandler::log_file_stream = NULL;

std::string LoggerFileHandler::default_log_format = 
    "[%timestamp] [%type] [%package] [%file:%line] [%method] "
    "[%methodsignature] [%message]";

std::map<std::string, std::string>*
    LoggerFileHandler::log_formats_map = NULL;

std::map<std::string, std::string>* 
    LoggerFileHandler::log_format_specifiers_map = NULL;

std::string LoggerFileHandler::GetLogFileName()
{
    return log_file_name; 
}

void LoggerFileHandler::SetLogFileName(const std::string& in_log_file)
{
    if(in_log_file == "")
    {
        logger.LogError("Log file name can not be empty string.");
        return;
    }
    
    // Checking input file exists or not.
    if(LoggerUtils::IsFileAccessible(in_log_file) == false)
    {
        logger.LogError("Input file does not exists or not accessible.");
        return;
    }

    if(initialized == false)
    {
        std::string error_msg =
            "LoggerFileHandler needs to be initialized before this call"
            "Input log file is " + in_log_file;
        logger.LogError(error_msg);
        throw std::runtime_error(error_msg);
    }

    if(log_file_name.compare(in_log_file) != 0) 
    { 
        // Closing current log file stream.
        log_file_stream->close();
        delete log_file_stream;
        log_file_stream = new std::fstream();

        log_file_name = in_log_file;
        // Opening new stream for writing.
        log_file_stream->open(in_log_file.c_str(), 
            std::fstream::out | std::fstream::app);

        if(log_file_stream->good() == false)
        {
            logger.LogError(
                "Log file stream was not created successfully. It may be"
                " log file does not have write access.");
            return;
        }
    }
}

bool LoggerFileHandler::GetLogFileStream(std::fstream** in_log_file_stream)
{
    if(initialized == false)
    {
        logger.LogError(
            "Init was not called yet. Init needs to be called before getting"
            " log file stream from file handler.");
        logger.LogDebug("stack trace");
        return false;
    }

    if(log_file_stream->good() == false)
    {
        logger.LogError("log file stream does not exists or was not created"
            " successful.");
        logger.LogDebug("stack trace");
        return false;
    }

    *in_log_file_stream = log_file_stream;
    return true;
}

bool LoggerFileHandler::IsLogFormatExists(const std::string& in_log_format)
{
    if(in_log_format == "")
    {
        logger.LogWarning("Log format string can not be empty.");
        return false;
    }

    if(log_formats_map->count(in_log_format) == 0)
    {
        return false;
    }
    return true;
}

void LoggerFileHandler::SetLogFormat(
    const std::string& in_log_format_name,
    const std::string& in_log_format_value
)
{
    if(in_log_format_name == "" || in_log_format_value == "")
    {
        logger.LogError(
            "Log format name string or value string can not be empty.");
        return;
    }

    (*log_formats_map)[in_log_format_name] = in_log_format_value;
}

std::string LoggerFileHandler::GetLogFormat(
    const std::string& in_log_format_name
)
{
    if(in_log_format_name == "")
    {
        logger.LogError("Log format name string can not be empty.");
        return "";
    }

    std::map<std::string, std::string>::iterator str_str_map_it;
    if((str_str_map_it = log_formats_map->find(in_log_format_name)) 
        == log_formats_map->end())
    {
        return "";
    }
    return str_str_map_it->second;
}

void LoggerFileHandler::SetFormatSpecifier(
    const std::string& in_specifier_name,
    const std::string& in_specifier_value
)
{
    if(in_specifier_name == "" || in_specifier_value == "")
    {
        logger.LogError(
            "Format specifier name or value can not be empty string.");
        return;
    }

    (*log_format_specifiers_map)[in_specifier_name] = in_specifier_value;
}

std::string LoggerFileHandler::GetFormatSpecifier(
    const std::string& in_specifier_name
)
{
    if(in_specifier_name == "")
    {
        logger.LogError("Format specifier name can not be empty string.");
        return "";
    }

    std::map<std::string, std::string>::iterator str_str_map_it;
    if((str_str_map_it = log_format_specifiers_map->find(in_specifier_name)) 
        == log_format_specifiers_map->end())
    {
        return " ";
    }
    return str_str_map_it->second;
}

void LoggerFileHandler::SetCommonFormatSpecifiers(const LogDB& in_log_db)
{
    // Adding current time - Timestamp format - YYYY-MM-DDThh:mm:ssTZD
    time_t timer;
    time(&timer);
    struct tm* timeinfo = localtime(&timer);
    char time_buff[50];
    strftime(time_buff, 50, "%Y-%m-%dT%H:%M:%S%Z%z\0", timeinfo);
    SetFormatSpecifier("timestamp", std::string(time_buff));
    
    SetFormatSpecifier("package", in_log_db.package_name);
    SetFormatSpecifier("file", in_log_db.file_name);
    SetFormatSpecifier("method", in_log_db.method_name);
    SetFormatSpecifier("methodsignature", in_log_db.method_signature);
    SetFormatSpecifier("message", in_log_db.message);

    std::ostringstream oss;
    oss << in_log_db.line_number;
    SetFormatSpecifier("line", oss.str());
}

void LoggerFileHandler::ParseLogFormat(
    std::string& out_parsed_log,
    const std::string& in_log_format
)
{
    int idx = 0;
    int log_format_str_size = in_log_format.size();
    while(idx < log_format_str_size)
    {
        char ch = in_log_format.at(idx);
        if(ch == '%')
        {
            // If '%' is the last char.
            if(idx+1 == log_format_str_size)
            {
                out_parsed_log.push_back(' ');
                break;
            }

            // Getting specifier string from log format.   
            std::string specifier_str;
            while(idx+1 < log_format_str_size && 
                isalpha(in_log_format.at(idx+1)))
            {
                specifier_str.push_back(in_log_format.at(idx+1));
                ++idx;  
            }

            // Putting specifier value into the out string.
            out_parsed_log.append(GetFormatSpecifier(specifier_str));
        }
        else
        {
            out_parsed_log.push_back(ch);
        }
        ++idx;
    }
}

bool LoggerFileHandler::IsValidLogFormat(const std::string& in_log_format_value)
{
    if(in_log_format_value == "")
    {
        return false;
    }
    return true;
}

bool LoggerFileHandler::IsValidLogFileName(const std::string& in_log_file)
{
    if(in_log_file == "")
    {
        return false;
    }
    return true;
}

void LoggerFileHandler::PrintCriticalLog(const CriticalLogDB& critical_log_db)
{
    logger.LogInfo("Printing the critical log.");

    log_format_specifiers_map->clear();
    SetCommonFormatSpecifiers(critical_log_db.common_properties);
    SetFormatSpecifier("type", "CRITICAL");

    std::string parsed_message;
    ParseLogFormat(parsed_message, GetLogFormat("LogCriticalFormat"));

    // Print message and stack trace to logging file and std error console.
    if(log_file_stream->good())
    {
        *log_file_stream << parsed_message;
        *log_file_stream << "\n";
        std::cerr << parsed_message << std::endl;
        log_file_stream->flush();

        // Adding backtrace.
        for(size_t i = 0; i < critical_log_db.backtrace_strings_size; i++)
        {
            *log_file_stream << critical_log_db.backtrace_strings[i];
            *log_file_stream << "\n";
            std::cerr << critical_log_db.backtrace_strings[i] << std::endl;
            log_file_stream->flush();
        }
    }
    else
    {
        logger.LogWarning("Log file stream is not in good state");
        return;
    }
}

void LoggerFileHandler::PrintErrorLog(const ErrorLogDB& error_log_db)
{
    log_format_specifiers_map->clear();
    SetCommonFormatSpecifiers(error_log_db.common_properties);
    SetFormatSpecifier("type", "ERROR");

    std::string parsed_message;
    ParseLogFormat(parsed_message, GetLogFormat("LogErrorFormat"));

    if(log_file_stream->good())
    {
        *log_file_stream << parsed_message;
        *log_file_stream << "\n";
        log_file_stream->flush();
    }
    else
    {
        logger.LogWarning("Log file stream is not in good state");
        return;
    }
}

void LoggerFileHandler::PrintWarningLog(const WarningLogDB& warning_log_db)
{
    log_format_specifiers_map->clear();
    SetCommonFormatSpecifiers(warning_log_db.common_properties);
    SetFormatSpecifier("type", "WARNING");

    std::string parsed_message;
    ParseLogFormat(parsed_message, GetLogFormat("LogWarningFormat"));

    if(log_file_stream->good())
    {
        *log_file_stream << parsed_message;
        *log_file_stream << "\n";
        log_file_stream->flush();
    }
    else
    {
        logger.LogWarning("Log file stream is not in good state");
        return;
    }
}

void LoggerFileHandler::PrintInfoLog(const InfoLogDB& info_log_db)
{
    log_format_specifiers_map->clear();
    SetCommonFormatSpecifiers(info_log_db.common_properties);
    SetFormatSpecifier("type", "INFO");

    std::string parsed_message;
    ParseLogFormat(parsed_message, GetLogFormat("LogInfoFormat"));

    if(log_file_stream->good())
    {
        *log_file_stream << parsed_message;
        *log_file_stream << "\n";
        log_file_stream->flush();
    }
    else
    {
        logger.LogWarning("Log file stream is not in good state");
        return;
    }
}

void LoggerFileHandler::PrintDebugLog(const DebugLogDB& debug_log_db)
{
    log_format_specifiers_map->clear();
    SetCommonFormatSpecifiers(debug_log_db.common_properties);
    SetFormatSpecifier("type", "DEBUG");

    std::string parsed_message;
    ParseLogFormat(parsed_message, GetLogFormat("LogDebugFormat"));

    if(log_file_stream->good())
    {
        *log_file_stream << parsed_message;
        *log_file_stream << "\n";
        log_file_stream->flush();
    }
    else
    {
        logger.LogWarning("Log file stream is not in good state");
        return;
    }
}

void LoggerFileHandler::PrintHackLog(const HackLogDB& hack_log_db)
{
    log_format_specifiers_map->clear();
    SetCommonFormatSpecifiers(hack_log_db.common_properties);
    SetFormatSpecifier("type", "HACK");

    std::string parsed_message;
    ParseLogFormat(parsed_message, GetLogFormat("LogHackFormat"));

    if(log_file_stream->good())
    {
        *log_file_stream << parsed_message;
        *log_file_stream << "\n";
        log_file_stream->flush();
    }
    else
    {
        logger.LogWarning("Log file stream is not in good state");
        return;
    }
}

bool LoggerFileHandler::Init(const std::string& in_app_name)
{
    if(initialized == false)
    {
        // Registering handler properties.
        LoggerConfig& config_obj = 
            LoggerConfig::GetLoggerConfigObject(in_app_name);

        config_obj.RegisterProperty("LogFile", IsValidLogFileName);

        std::vector<std::string>::iterator vstr_it;
        std::vector<std::string> log_formats = {
            "LogFormat", "LogCriticalFormat", "LogErrorFormat",
            "LogWarningFormat", "LogInfoFormat", "LogDebugFormat",
            "LogHackFormat"
        };

        for(vstr_it = log_formats.begin(); 
            vstr_it != log_formats.end(); 
            vstr_it++)
        {
            config_obj.RegisterProperty(*vstr_it, IsValidLogFormat);
        }

        // Check log file name exists otherwise get it from configuration.
        // If log file name is not found this method terminates.
        if(log_file_name == "")
        {
            // Loading log file from configuration file.
            if(config_obj.IsPropertyExists("LogFile"))
            {
                log_file_name = config_obj.GetPropertyValue("LogFile");
            }
            else
            {
                logger.LogWarning(
                    "Log file name does not exist. Add LogFile property in"
                    " configuration file.");
                return false;
            }
        }

        // Initializing data structures.
        log_file_stream = new std::fstream();
        LoggerFileHandler::log_formats_map =
            new std::map<std::string, std::string>();
        LoggerFileHandler::log_format_specifiers_map =
            new std::map<std::string, std::string>();
        
        // Loading log formats from configuration file, if not found check for
        // default format in configuration file, if default format also does not
        // exist set default format defined in this class.
        for(vstr_it = log_formats.begin(); 
            vstr_it != log_formats.end(); 
            vstr_it++)
        {
            if(config_obj.IsPropertyExists(*vstr_it))
            {
                SetLogFormat(*vstr_it, config_obj.GetPropertyValue(*vstr_it));
            }
            else if(config_obj.IsPropertyExists("LogFormat"))
            { 
                SetLogFormat(*vstr_it, 
                    config_obj.GetPropertyValue("LogFormat"));
            }
            else
            {
                SetLogFormat(*vstr_it, default_log_format);
            }
        }

        // Opening log file for writing.
        log_file_stream->open(GetLogFileName(), 
            std::fstream::out | std::fstream::app);

        if(log_file_stream->fail())
        {
            logger.LogError("Error in opening log file stream.");
            return false;
        }

        if(logger.IsLogVerbose())
        {
            logger.LogInfo("File Handler was initialized successful.");
        }

        initialized = true;
    }
    return true;
}

void LoggerFileHandler::CleanUp(const std::string& in_app_name)
{
    if(initialized == true)
    {
        // Closing log file stream.
        log_file_stream->flush();
        log_file_stream->close();
        log_file_name = "";
        
        // Deleting heap memory.
        delete log_formats_map;
        delete log_format_specifiers_map;
        delete log_file_stream;
        
        // UnRegistering handler properties.
        LoggerConfig& config_obj = 
            LoggerConfig::GetLoggerConfigObject(in_app_name);

        config_obj.UnRegisterProperty("LogFile");

        std::vector<std::string>::iterator vstr_it;
        std::vector<std::string> log_formats = {
            "LogFormat", "LogCriticalFormat", "LogErrorFormat",
            "LogWarningFormat", "LogInfoFormat", "LogDebugFormat",
            "LogHackFormat"
        };

        for(vstr_it = log_formats.begin(); 
            vstr_it != log_formats.end(); 
            vstr_it++)
        {
            config_obj.UnRegisterProperty(*vstr_it);
        }

        initialized = false;
    }
}

}    // namespace cpplogger
}    // namespace cpputils

using namespace cpputils::cpplogger;
CPPLOGGER_DECLARE_HANDLER(file) = {
    LoggerFileHandler::Init,
    LoggerFileHandler::CleanUp,
    LoggerFileHandler::PrintCriticalLog,
    LoggerFileHandler::PrintErrorLog,
    LoggerFileHandler::PrintWarningLog,
    LoggerFileHandler::PrintInfoLog,
    LoggerFileHandler::PrintDebugLog,
    LoggerFileHandler::PrintHackLog
};

