/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_logging.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sun Apr 28 20:48:40 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 LoggerLogging class provides logging methods which is used in cpplogger to
 log messages. It is very small utility and provides very basic method to
 imporve debugging in cpplogger.
 
 CPPLogger runs Init method before using logging which opens logging file.
 CPPLogger needs to run CleanUp method before exit. It will close logging
 file stream.

 Info messages will be logged only if logging is verbose.

 Logging example :-

 LoggerLogging log(package_name);

 log.LogError("error messages");
 log.LogWarning("warning messages");
 log.LogInfo("info messages");
  
 **/

#ifndef CPPUTILS_CPPLOGGER_LOGGING_H_
#define CPPUTILS_CPPLOGGER_LOGGING_H_

#define LogError(X) CPPLOGGERLog("ERROR", __FILE__, __LINE__, __FUNCTION__, X)
#define LogWarning(X) CPPLOGGERLog("WARNING", __FILE__, __LINE__, \
    __FUNCTION__, X)
#define LogInfo(X) CPPLOGGERLog("INFO", __FILE__, __LINE__, __FUNCTION__, X)
#define LogDebug(X) CPPLOGGERLog("DEBUG", __FILE__, __LINE__, __FUNCTION__, X)

#include <execinfo.h>

namespace cpputils
{
namespace cpplogger
{

class LoggerLogging
{
    private :

        static bool initialized;
        static std::string logging_file_name;
        static std::fstream *logging_file_stream;
        static bool verbose;
        static bool vverbose;

        std::string package_name;

        void Log(std::string message);

    public : 
        
        static bool Init();
        static void CleanUp();

        LoggerLogging(std::string in_package_name); 
        LoggerLogging(const char* in_package_name);
        ~LoggerLogging();

        static void SetLogFileName(const std::string& in_file_name);
        void CPPLOGGERLog(const char* type, const char* file, int line,
            const char* func, std::string message);

        bool IsLogVerbose();
        bool IsLogVVerbose();
};

}    // namespace cpplogger
}    // namespace cpputils
#endif
