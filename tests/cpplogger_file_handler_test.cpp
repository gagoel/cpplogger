#include <cstdio>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>

#ifdef HAVE_HANDLER_FILE

#include "cpputils/cpplogger/handlers/file/cpplogger_file_handler.h"
#include "gtest/gtest.h"

using namespace cpputils::cpplogger;

class LoggerFileHandlerTest : public ::testing::Test
{
    protected:

        std::string config_file;
        std::fstream config_file_stream;
        std::string log_file;
        std::fstream log_file_stream;
        std::string cpplogger_log_file;
        std::string app_name = "file-handler-test";

        std::string open_bracket = "[";
        std::string close_bracket = "]";
        std::string space = " ";
        std::string colon = ":";

        std::string test1_package_name = "test_package";
        const char* test1_file_name = "test file name";
        int test1_line_number = 4930;
        const char* test1_method_name = "test method";
        const char* test1_method_sig = "void (*func)()";
        std::string test1_message = "Testing Message."; 

        LogDB test1_common_log_db = {
            app_name,
            test1_package_name,
            test1_file_name,
            test1_line_number,
            test1_method_name,
            test1_method_sig,
            test1_message
        };

        std::string log_file_test1_name = "LogFile";
        std::string log_file_test1_value = std::string(TEST_BUILD_HOME) + \
                   std::string("/cpplogger_file_handler_test.log");
        std::string log_file_test1_in_file = log_file_test1_name + \
            std::string("    ") + log_file_test1_value + std::string("\n");
        
        std::string log_format_test1_name = "LogFormat";
        std::string log_format_test1_value = "%date %time %testing";
        std::string log_format_test1_in_file = log_format_test1_name + \
            std::string("    ") + log_format_test1_value + std::string("\n");

        std::string log_format_test2_name = "LogCriticalFormat";
        std::string log_format_test2_value = "%date %critical %time %testing";
        std::string log_format_test2_in_file = log_format_test2_name + \
            std::string("    ") + log_format_test2_value + std::string("\n");

        std::string log_format_test3_name = "LogInfoFormat";
        std::string log_format_test3_value = "%date %info %time %testing";
        std::string log_format_test3_in_file = log_format_test3_name + \
            std::string("    ") + log_format_test3_value + std::string("\n");

        std::string test1_log_format = 
            "[%type] [%package] [%file:%line] [%method] [%methodsignature] " 
            "[%message]";

    virtual void SetUp()
    {
        cpplogger_log_file = std::string(TEST_BUILD_HOME) + \
            std::string("/cpplogger_file_handler_test_logs.log");
        std::fstream cpplogger_log_fstream(cpplogger_log_file.c_str(),
            std::fstream::out | std::fstream::trunc);
        cpplogger_log_fstream.close();
        LoggerLogging::SetLogFileName(cpplogger_log_file);

        config_file = std::string(TEST_BUILD_HOME) + \
                      std::string("/cpplogger_file_handler_test.conf");
        config_file_stream.open(config_file.c_str(), std::fstream::out | \
            std::fstream::trunc);

        config_file_stream.write(log_file_test1_in_file.c_str(),
            log_file_test1_in_file.size());
        config_file_stream.write(log_format_test1_in_file.c_str(),
            log_format_test1_in_file.size());
        config_file_stream.write(log_format_test2_in_file.c_str(),
            log_format_test2_in_file.size());
        config_file_stream.write(log_format_test3_in_file.c_str(),
            log_format_test3_in_file.size());

        config_file_stream.close();

        log_file = std::string(TEST_BUILD_HOME) + \
                   std::string("/cpplogger_file_handler_test.log");
        log_file_stream.open(log_file.c_str(), std::fstream::out | \
            std::fstream::trunc);
        log_file_stream.close();
    }

    virtual void TearDown()
    {
        remove(config_file.c_str());
        remove(log_file.c_str());
        remove(cpplogger_log_file.c_str());
    }

    std::string GetLogFileFirstLine();
    void ClearLogFileContent();
};

std::string LoggerFileHandlerTest::GetLogFileFirstLine()
{
    int max_line_length = 1000;
    char logged_str[max_line_length];
    std::fstream *log_fstream = new std::fstream();
    log_fstream->open(this->log_file, std::fstream::in);
    log_fstream->getline(logged_str, max_line_length);
    log_fstream->close();
    return std::string(logged_str); 
}

void LoggerFileHandlerTest::ClearLogFileContent()
{
    std::fstream *log_fstream = new std::fstream();
    log_fstream->open(this->log_file, std::fstream::out | std::fstream::trunc);
    log_fstream->close();
}

TEST_F(LoggerFileHandlerTest, ParseLogFormat)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
   
    EXPECT_EQ(true, LoggerFileHandler::Init(this->app_name));
    LoggerFileHandler::SetLogFileName(this->log_file);
 
    // Adding some specifier and checking for ouput format.
    LoggerFileHandler::SetFormatSpecifier("file", "TestFile");
    LoggerFileHandler::SetFormatSpecifier("date", "2013-10-20T10:22:23+5:30");
    LoggerFileHandler::SetFormatSpecifier("method", "TestMethod");
    std::string output_string;
    std::string formatted_string = 
        "[%file] [[%date]] [NOT SPECIFIER] [%method]";
    std::string expected_string = 
        "[TestFile] [[2013-10-20T10:22:23+5:30]] [NOT SPECIFIER] [TestMethod]";
    LoggerFileHandler::ParseLogFormat(output_string, formatted_string);

    EXPECT_EQ(expected_string, output_string);

    // If Specifier does not exist add a space.
    LoggerFileHandler::SetFormatSpecifier("file", "TestFile1");
    LoggerFileHandler::SetFormatSpecifier("date", "2011-11-20T10:22:23+5:30");
    LoggerFileHandler::SetFormatSpecifier("method", "TestMethod1");
    std::string output_string1;
    std::string formatted_string1 = "[%file] [[%date]] [%unknown] [%method]";
    std::string expected_string1 = 
        "[TestFile1] [[2011-11-20T10:22:23+5:30]] [ ] [TestMethod1]";
    LoggerFileHandler::ParseLogFormat(output_string1, formatted_string1);

    EXPECT_EQ(expected_string1, output_string1);

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, LogFileTests)
{
    std::string temp_log_file = std::string(TEST_BUILD_HOME) + \
        "/cpplogger_file_handler_logfiletests_test.log";
    std::string temp_log_text = "This is for testing log file creation.";

    std::fstream log_file_stream(temp_log_file.c_str(), 
        std::fstream::out | std::fstream::trunc);
    log_file_stream.write(temp_log_text.c_str(), temp_log_text.size());
    log_file_stream.close();

    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandler::SetLogFileName(temp_log_file);

    char result_buff[100];
    std::fstream *find_log_stream = new std::fstream();
    find_log_stream->open(temp_log_file.c_str(), std::fstream::in);
    find_log_stream->getline(result_buff, 100);
    find_log_stream->close();

    EXPECT_EQ(temp_log_text, std::string(result_buff));

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
    if(remove(temp_log_file.c_str()) != 0)
    {
        perror("Error in deleting log file.");
    }
}

TEST_F(LoggerFileHandlerTest, GetLogFormat)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);

    EXPECT_EQ("%date %time %testing", 
        LoggerFileHandler::GetLogFormat("LogFormat"));
    EXPECT_EQ("%date %critical %time %testing", 
        LoggerFileHandler::GetLogFormat("LogCriticalFormat"));
    EXPECT_EQ("%date %info %time %testing", 
        LoggerFileHandler::GetLogFormat("LogInfoFormat"));
    EXPECT_EQ("%date %time %testing", 
        LoggerFileHandler::GetLogFormat("LogErrorFormat"));

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, SetLogFormat)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);

    // Setting exist one.
    EXPECT_EQ("%date %time %testing", 
        LoggerFileHandler::GetLogFormat("LogFormat"));
    LoggerFileHandler::SetLogFormat("LogFormat", "changing it.");
    EXPECT_EQ("changing it.", 
        LoggerFileHandler::GetLogFormat("LogFormat"));

    // Setting new one.
    EXPECT_EQ("%date %time %testing", 
        LoggerFileHandler::GetLogFormat("LogErrorFormat"));
    LoggerFileHandler::SetLogFormat("LogErrorFormat", "Adding new format");
    EXPECT_EQ("Adding new format", 
        LoggerFileHandler::GetLogFormat("LogErrorFormat"));

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, IsLogFormatExists)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);

    EXPECT_EQ(true, LoggerFileHandler::IsLogFormatExists("LogFormat"));
    EXPECT_EQ(true, LoggerFileHandler::IsLogFormatExists("LogInfoFormat"));
    EXPECT_TRUE(false == LoggerFileHandler::IsLogFormatExists("xyz"));

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, PrintCriticalLog)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandlerTest::ClearLogFileContent();
    LoggerFileHandler::SetLogFileName(this->log_file);
    LoggerFileHandler::SetLogFormat("LogCriticalFormat", test1_log_format);

    CriticalLogDB test1_critical_log_db = {
        test1_common_log_db
    };
    LoggerFileHandler::PrintCriticalLog(test1_critical_log_db);

    std::string expected_string = "[CRITICAL] ";
    expected_string += 
        open_bracket + this->test1_package_name + close_bracket + space +
        open_bracket + this->test1_file_name + colon +
        std::to_string(this->test1_line_number) + close_bracket + space +
        open_bracket + this->test1_method_name + close_bracket + space +
        open_bracket + this->test1_method_sig + close_bracket + space +
        open_bracket + this->test1_message + close_bracket;
    
    EXPECT_EQ(expected_string, LoggerFileHandlerTest::GetLogFileFirstLine());

    // No need to call LoggerFileHandler cleanup as it was called with
    // PrintCriticalLog method
    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, PrintErrorLog)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandlerTest::ClearLogFileContent();
    LoggerFileHandler::SetLogFileName(this->log_file);
    LoggerFileHandler::SetLogFormat("LogErrorFormat", test1_log_format);

    ErrorLogDB test1_error_log_db = {
        test1_common_log_db
    };
    LoggerFileHandler::PrintErrorLog(test1_error_log_db);

    std::string expected_string = "[ERROR] ";
    expected_string += 
        open_bracket + this->test1_package_name + close_bracket + space +
        open_bracket + this->test1_file_name + colon +
        std::to_string(this->test1_line_number) + close_bracket + space +
        open_bracket + this->test1_method_name + close_bracket + space +
        open_bracket + this->test1_method_sig + close_bracket + space +
        open_bracket + this->test1_message + close_bracket;
    
    EXPECT_EQ(expected_string, GetLogFileFirstLine());

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, PrintWarningLog)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandlerTest::ClearLogFileContent();
    LoggerFileHandler::SetLogFileName(this->log_file);
    LoggerFileHandler::SetLogFormat("LogWarningFormat", test1_log_format);

    WarningLogDB test1_warning_log_db = {
        test1_common_log_db
    };
    LoggerFileHandler::PrintWarningLog(test1_warning_log_db);

    std::string expected_string = "[WARNING] ";
    expected_string += 
        open_bracket + this->test1_package_name + close_bracket + space +
        open_bracket + this->test1_file_name + colon +
        std::to_string(this->test1_line_number) + close_bracket + space +
        open_bracket + this->test1_method_name + close_bracket + space +
        open_bracket + this->test1_method_sig + close_bracket + space +
        open_bracket + this->test1_message + close_bracket;
    
    EXPECT_EQ(expected_string, GetLogFileFirstLine());

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, PrintInfoLog)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandlerTest::ClearLogFileContent();
    LoggerFileHandler::SetLogFileName(this->log_file);
    LoggerFileHandler::SetLogFormat("LogInfoFormat", test1_log_format);

    InfoLogDB test1_info_log_db = {
        test1_common_log_db
    };
    LoggerFileHandler::PrintInfoLog(test1_info_log_db);

    std::string expected_string = "[INFO] ";
    expected_string += 
        open_bracket + this->test1_package_name + close_bracket + space +
        open_bracket + this->test1_file_name + colon +
        std::to_string(this->test1_line_number) + close_bracket + space +
        open_bracket + this->test1_method_name + close_bracket + space +
        open_bracket + this->test1_method_sig + close_bracket + space +
        open_bracket + this->test1_message + close_bracket;
    
    EXPECT_EQ(expected_string, GetLogFileFirstLine());

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, PrintDebugLog)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandlerTest::ClearLogFileContent();
    LoggerFileHandler::SetLogFileName(this->log_file);
    LoggerFileHandler::SetLogFormat("LogDebugFormat", test1_log_format);

    DebugLogDB test1_debug_log_db = {
        test1_common_log_db
    };
    LoggerFileHandler::PrintDebugLog(test1_debug_log_db);

    std::string expected_string = "[DEBUG] ";
    expected_string += 
        open_bracket + this->test1_package_name + close_bracket + space +
        open_bracket + this->test1_file_name + colon +
        std::to_string(this->test1_line_number) + close_bracket + space +
        open_bracket + this->test1_method_name + close_bracket + space +
        open_bracket + this->test1_method_sig + close_bracket + space +
        open_bracket + this->test1_message + close_bracket;
    
    EXPECT_EQ(expected_string, GetLogFileFirstLine());

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}

TEST_F(LoggerFileHandlerTest, PrintHackLog)
{
    LoggerConfig::Init();
    LoggerConfig::GetLoggerConfigObject(this->app_name, this->config_file);
    LoggerFileHandler::Init(this->app_name);
    LoggerFileHandlerTest::ClearLogFileContent();
    LoggerFileHandler::SetLogFileName(this->log_file);
    LoggerFileHandler::SetLogFormat("LogHackFormat", test1_log_format);

    HackLogDB test1_hack_log_db = {
        test1_common_log_db
    };
    LoggerFileHandler::PrintHackLog(test1_hack_log_db);

    std::string expected_string = "[HACK] ";
    expected_string += 
        open_bracket + this->test1_package_name + close_bracket + space +
        open_bracket + this->test1_file_name + colon +
        std::to_string(this->test1_line_number) + close_bracket + space +
        open_bracket + this->test1_method_name + close_bracket + space +
        open_bracket + this->test1_method_sig + close_bracket + space +
        open_bracket + this->test1_message + close_bracket;
    
    EXPECT_EQ(expected_string, GetLogFileFirstLine());

    LoggerFileHandler::CleanUp(this->app_name);
    LoggerConfig::CleanUp();
}
#endif
