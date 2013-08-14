/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_logging.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sun Apr 28 20:48:56 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This class implements the logging methods which is used in cpplogger.

 **/
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "cpputils/cpplogger/cpplogger_logging.h"

namespace cpputils
{
namespace cpplogger
{

bool LoggerLogging::initialized = false; 
std::string LoggerLogging::logging_file_name = CPPLOGGER_LOG_FILE;
std::fstream *LoggerLogging::logging_file_stream = NULL;

#ifdef CPPLOGGER_VLOGGING
bool LoggerLogging::verbose = true;
#else
bool LoggerLogging::verbose = false;
#endif

#ifdef CPPLOGGER_VVLOGGING
bool LoggerLogging::vverbose = true;
#else
bool LoggerLogging::vverbose = false;
#endif

void LoggerLogging::Log(std::string message)
{
    if(LoggerLogging::initialized == false)
    {
        LoggerLogging::Init();
    }
 
    std::string log_message = "[" + this->package_name + "] " + message;

    if(logging_file_stream->is_open())
    {
        logging_file_stream->write(
	    log_message.c_str(), log_message.size());
        logging_file_stream->flush();
    }
}

void LoggerLogging::SetLogFileName(const std::string& in_file_name)
{
    logging_file_name = in_file_name;
}

bool LoggerLogging::Init()
{
    if(LoggerLogging::initialized) 
    {
        return true;
    }

    logging_file_stream = new std::fstream(
        logging_file_name.c_str(),
	    std::fstream::in | std::fstream::out | std::fstream::app);

    if(!logging_file_stream->is_open())
    {
        std::string error_msg = 
            "Error in opening cpplogger logging file. "
            "file is :- " + logging_file_name;
        throw std::runtime_error(error_msg.c_str());
    	return false;
    }

    LoggerLogging::initialized = true;
    return true;
}

void LoggerLogging::CleanUp()
{
    if(initialized == true)
    {
        LoggerLogging::logging_file_stream->close();
        LoggerLogging::initialized = false;
    }
}

LoggerLogging::LoggerLogging(std::string in_package_name)
{
    this->package_name = in_package_name;
}

LoggerLogging::LoggerLogging(const char* in_package_name)
{
    this->package_name = std::string(in_package_name);
}

LoggerLogging::~LoggerLogging()
{
    if(LoggerLogging::initialized)
    {
        LoggerLogging::CleanUp();
    }
}

bool LoggerLogging::IsLogVerbose()
{
    return LoggerLogging::vverbose || LoggerLogging::verbose;
}

bool LoggerLogging::IsLogVVerbose()
{
    return LoggerLogging::vverbose;
}

void LoggerLogging::CPPLOGGERLog(const char* type, const char* file, int line, 
    const char* func, std::string message)
{
    if(strcmp(type, "INFO") == 0)
    {
        if(LoggerLogging::IsLogVerbose())
        {
            this->Log("[" + std::string(type) + "] [" + std::string(file) + 
                ":" + std::to_string(line) + "] [" + std::string(func) + 
                "] [" + message + "] \n");
        }
    }
    else if(strcmp(type, "DEBUG") == 0)
    {
        if(LoggerLogging::IsLogVVerbose())
        {
            // Creating stack trace.
            const int max_backtrace_len = 50;
            void *backtrace_buff[max_backtrace_len];
            size_t backtrace_size;
            char **backtrace_strings;

            backtrace_size = backtrace(backtrace_buff, max_backtrace_len);
            backtrace_strings = 
                backtrace_symbols(backtrace_buff, backtrace_size);
            
            std::string log_msg;
            log_msg = message + "\n";

            // not printing logging call stack
            for(unsigned int i = 1; i < backtrace_size; i++)
            {
                log_msg = log_msg + backtrace_strings[i] + "\n";
            }
            log_msg = log_msg + "\n";     

            this->Log("[" + std::string(type) + "] [" + std::string(file) + 
                ":" + std::to_string(line) + "] [" + std::string(func) + 
                "] [" + log_msg + "] \n");
        }
    }
    else
    {
        this->Log("[" + std::string(type) + "] [" + std::string(file) + ":" +
            std::to_string(line) + "] [" + std::string(func) + "] [" + 
            message + "] \n");

    }
}

}    // namespace cpplogger
}    // namespace cpputils
