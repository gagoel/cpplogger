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

#include <fstream>
#include <string>
#include <stdexcept>

#include "cpplogger/cpplogger_logging.h"

namespace cpputils
{
namespace cpplogger
{
std::string LoggerLogging::logging_file_name = CPPLOGGER_LOG_FILE;

std::fstream *LoggerLogging::logging_file_stream = NULL;

#ifdef CPPLOGGER_DEBUG
bool LoggerLogging::verbose = true;
#else
bool LoggerLogging::verbose = false;
#endif

bool LoggerLogging::initialized = false; 

void LoggerLogging::Log(std::string message)
{
    if(LoggerLogging::initialized == false)
    {
        LoggerLogging::Init();
    }
 
    std::string log_message = "[" + this->package_name + "] " + message;

    if(LoggerLogging::logging_file_stream->is_open())
    {
        LoggerLogging::logging_file_stream->write(
	    log_message.c_str(), log_message.size());
        LoggerLogging::logging_file_stream->flush();
    }
}

void LoggerLogging::SetLogFileName(const std::string& in_file_name)
{
    logging_file_name = in_file_name;
}

bool LoggerLogging::Init()
{
    if(LoggerLogging::initialized) return true;

    LoggerLogging::logging_file_stream = new std::fstream(
        LoggerLogging::logging_file_name.c_str(),
	    std::fstream::in | std::fstream::out | std::fstream::app);

    if(!LoggerLogging::logging_file_stream->is_open())
    {
        throw std::runtime_error("Error in opening cpplogger logging file");
    	return false;
    }

    LoggerLogging::initialized = true;
    return true;
}

void LoggerLogging::CleanUp()
{
    LoggerLogging::initialized = false;
    LoggerLogging::logging_file_stream->close();
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
    return LoggerLogging::verbose;
}

void LoggerLogging::CPPLOGGERLog(const char* type, const char* file, int line, 
    const char* func, std::string message)
{
    if(strcmp(type, "INFO") || LoggerLogging::IsLogVerbose())
    {
        this->Log("[" + std::string(type) + "] [" + std::string(file) + ":" +
            std::to_string(line) + "] [" + std::string(func) + "] [" + 
            message + "] \n");
    }
}

}    // namespace cpplogger
}    // namespace cpputils
