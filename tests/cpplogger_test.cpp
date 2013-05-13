#include <unistd.h>

#include <cstdio>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>

#ifdef HAVE_HANDLER_FILE
#include "cpplogger/handlers/file/cpplogger_file_handler.h"
#endif

#include "cpplogger/cpplogger_handler.h"
#include "cpplogger/cpplogger_conf.h"
#include "cpplogger/cpplogger.h"
#include "gtest/gtest.h"

using namespace cpputils::cpplogger;

class LoggerTest : public ::testing::Test
{
    protected:

        std::string config_file;
        std::fstream config_file_stream;
        std::string log_file;
        std::fstream log_file_stream;
        std::string cpplogger_log_file;

        std::string test1_package_name = "cpputils.cpplogger.test1";
        std::string test1_critical_log_message = "Critical message test1";
        std::string test1_error_log_message = "Error message test1";
        std::string test1_warning_log_message = "Warning message test1";
        std::string test1_info_log_message = "Info message test1";
        std::string test1_debug_log_message = "Debug message test1";
        std::string test1_hack_log_message = "Hack message test1";

        std::string CreateConfigFile();
        void DeleteConfigFile(); 

        std::string CreateLogFile();
        void DeleteLogFile();

        std::string GetLogFileLine(int line_num);

    virtual void SetUp()
    {
        cpplogger_log_file = std::string(TEST_BUILD_HOME) + \
            std::string("/cpplogger_test_logs.log");
        std::fstream cpplogger_log_fstream(cpplogger_log_file.c_str(),
            std::fstream::out);
        cpplogger_log_fstream.close();
        LoggerLogging::SetLogFileName(cpplogger_log_file);
        
        config_file = CreateConfigFile();
        log_file = CreateLogFile();
    }

    virtual void TearDown()
    {
        DeleteConfigFile();
        DeleteLogFile();
        remove(cpplogger_log_file.c_str());
    }
    
};

std::string LoggerTest::CreateConfigFile()
{
    std::string curr_config_file = std::string(TEST_BUILD_HOME) + \
                                   std::string("/cpplogger_test_temp.conf");
    config_file_stream.open(curr_config_file.c_str(), std::fstream::out | \
                            std::fstream::trunc);
    config_file_stream.close();
    return curr_config_file;
}

void LoggerTest::DeleteConfigFile()
{
    remove(config_file.c_str());
}

std::string LoggerTest::CreateLogFile()
{
    std::string curr_log_file = std::string(TEST_BUILD_HOME) + \
                                std::string("/cpplogger_test_temp.log");
    log_file_stream.open(curr_log_file.c_str(), std::fstream::out | \
                         std::fstream::trunc);
    log_file_stream.close();
    return curr_log_file;
}

void LoggerTest::DeleteLogFile()
{
    remove(log_file.c_str());
}

#ifdef HAVE_HANDLER_FILE
std::string LoggerTest::GetLogFileLine(int line_num)
{
    int max_line_length = 1000;
    char logged_str[max_line_length];
    std::fstream *log_fstream;
    LoggerFileHandler::GetLogFileStream(&log_fstream);
    log_fstream->seekg(0, std::fstream::beg);
    for(int idx = 1; idx <= line_num; idx++)
    {
        log_fstream->getline(logged_str, max_line_length);
        if(log_fstream->fail()) return "";
    }
    return std::string(logged_str); 
}
#endif

#ifdef HAVE_HANDLER_FILE
TEST_F(LoggerTest, Error)
{
    LoggerConf::SetConfigFile(config_file);
    EXPECT_EQ(true, Logger::Init());
    LoggerFileHandler::SetLogFileName(log_file);
    LoggerHandler::AddHandler("file", FILE_HANDLER_LIB);

    // Logging error message.
    Logger log = Logger::CreateInstance(test1_package_name);
    log.Error(test1_error_log_message); int line_num = __LINE__;

    // Testing logged strings.
    std::string log_in_file = LoggerTest::GetLogFileLine(1);

    EXPECT_FALSE(log_in_file.find(test1_package_name) == std::string::npos);
    EXPECT_FALSE(log_in_file.find(test1_error_log_message) 
        == std::string::npos);
    EXPECT_FALSE(log_in_file.find("ERROR") == std::string::npos);
    EXPECT_FALSE(log_in_file.find(__FUNCTION__) == std::string::npos);
    EXPECT_FALSE(log_in_file.find(__FILE__) == std::string::npos);
    EXPECT_FALSE(log_in_file.find(std::to_string(line_num)) 
        == std::string::npos);

    Logger::CleanUp();
}

TEST_F(LoggerTest, IsFine)
{
    LoggerConf::SetConfigFile(config_file);
    EXPECT_EQ(true, Logger::Init());
    LoggerFileHandler::SetLogFileName(log_file);
    LoggerHandler::AddHandler("file", FILE_HANDLER_LIB);

    // Logging error message.
    Logger log = Logger::CreateInstance(test1_package_name);
    EXPECT_TRUE(log.IsFine() == false);
    LoggerConf::SetLogLevelString(test1_package_name, "FINE");
    EXPECT_TRUE(log.IsFine() == true);
    LoggerConf::SetLogLevelString(test1_package_name, "FINER");
    EXPECT_TRUE(log.IsFine() == true);
    LoggerConf::SetLogLevelString(test1_package_name, "FINEST");
    EXPECT_TRUE(log.IsFine() == true);
    LoggerConf::SetLogLevelString(test1_package_name, "DEFAULT");
    EXPECT_TRUE(log.IsFine() == false);
    
    Logger::CleanUp();
}

TEST_F(LoggerTest, LogDefaultMessage)
{
    LoggerConf::SetConfigFile(config_file);
    EXPECT_EQ(true, Logger::Init());
    LoggerFileHandler::SetLogFileName(log_file);
    LoggerHandler::AddHandler("file", FILE_HANDLER_LIB);

    Logger log = Logger::CreateInstance(test1_package_name);
    EXPECT_TRUE(log.IsFine() == false);
    LoggerConf::SetLogLevelString(test1_package_name, "DEFAULT");
    EXPECT_TRUE(log.IsFine() == false);

    //log.Critical(test1_critical_log_message); - Terminate the program
    log.Info(test1_critical_log_message);
    log.Warning(test1_critical_log_message);
    log.Hack(test1_hack_log_message);
    log.Error(test1_error_log_message);
    log.Debug(test1_debug_log_message);

    //std::string log1 = LoggerTest::GetLogFileLine(1);
    //EXPECT_FALSE(log1.find("CRITICAL") == std::string::npos);
    std::string log2 = LoggerTest::GetLogFileLine(1);
    EXPECT_FALSE(log2.find("WARNING") == std::string::npos);
    std::string log3 = LoggerTest::GetLogFileLine(2);
    EXPECT_FALSE(log3.find("ERROR") == std::string::npos);
    std::string log4 = LoggerTest::GetLogFileLine(3);
    EXPECT_EQ("", log4);

    Logger::CleanUp();
}

TEST_F(LoggerTest, LogFineMessage)
{
    LoggerConf::SetConfigFile(config_file);
    EXPECT_EQ(true, Logger::Init());
    LoggerFileHandler::SetLogFileName(log_file);
    LoggerHandler::AddHandler("file", FILE_HANDLER_LIB);

    Logger log = Logger::CreateInstance(test1_package_name);
    EXPECT_TRUE(log.IsFine() == false);
    LoggerConf::SetLogLevelString(test1_package_name, "FINE");
    EXPECT_TRUE(log.IsFine() == true);

    //log.Critical(test1_critical_log_message); Terminate the program
    log.Info(test1_critical_log_message);
    log.Warning(test1_critical_log_message);
    log.Hack(test1_hack_log_message);
    log.Error(test1_error_log_message);
    log.Debug(test1_debug_log_message);

    //std::string log1 = LoggerTest::GetLogFileLine(1);
    //EXPECT_FALSE(log1.find("CRITICAL") == std::string::npos);
    std::string log2 = LoggerTest::GetLogFileLine(1);
    EXPECT_FALSE(log2.find("INFO") == std::string::npos);
    std::string log3 = LoggerTest::GetLogFileLine(2);
    EXPECT_FALSE(log3.find("WARNING") == std::string::npos);
    std::string log4 = LoggerTest::GetLogFileLine(3);
    EXPECT_FALSE(log4.find("ERROR") == std::string::npos);
    std::string log5 = LoggerTest::GetLogFileLine(4);
    EXPECT_EQ("", log5);

    Logger::CleanUp();
}
#endif
