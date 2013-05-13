#include <cstdio>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>

#include "cpplogger/cpplogger_utils.h"
#include "cpplogger/cpplogger_conf.h"
#include "gtest/gtest.h"

using namespace cpputils::cpplogger;

class LoggerConfTest : public ::testing::Test, public LoggerConf
{
    protected :
        std::string config_file;
        std::string cpplogger_log_file;
        std::fstream config_fstream;
        int testcase_count = 5;

        std::string test1_package_name = "cpplogger.package.test.default";
        std::string test1_log_level = "DEFAULT";
        std::string test1_in_file = std::string("SetLogLevel ") + 
            test1_package_name + std::string("    ") + \
            test1_log_level + std::string("\n");
            
        std::string test2_package_name = "cpplogger.package.test.fine";
        std::string test2_log_level = "FINE";
        std::string test2_in_file = std::string("SetLogLevel ") + 
            test2_package_name + std::string("    ") +\
            test2_log_level + std::string("\n");

        std::string test3_package_name = "cpplogger.package.test.finer";
        std::string test3_log_level = "FINER";
        std::string test3_in_file = std::string("SetLogLevel ") + 
            test3_package_name + std::string("    ") +\
            test3_log_level + std::string("\n");

        std::string test4_package_name = "cpplogger.package.test.finest";
        std::string test4_log_level = "FINEST";
        std::string test4_in_file = std::string("SetLogLevel ") + 
            test4_package_name + std::string("    ") +\
            test4_log_level + std::string("\n");

        std::string test5_package_name = "cpplogger.package.test.unknown";
        std::string test5_log_level = "DEFAULT";
        std::string test5_in_file = std::string("SetLogLevel ") + 
            test5_package_name + std::string("    ") +\
            test5_log_level + std::string("\n");

        std::string handler_test1_name = "file";
        std::string handler_test1_lib = "/lib/path/libfile.so";
        std::string handler_test1_in_file = std::string("AddHandler ") + 
            handler_test1_name + std::string(" ") + handler_test1_lib + \
            std::string("\n");

        std::string handler_test2_name = "xml";
        std::string handler_test2_lib = "/lib/path/libxml.so";
        std::string handler_test2_in_file = std::string("AddHandler ") + 
            handler_test2_name + std::string(" ") + handler_test2_lib + \
            std::string("\n");

        std::string handler_test3_name = "ui";
        std::string handler_test3_lib = "/lib/path/libxml.so";
        std::string handler_test3_in_file = std::string("AddHandler ") + 
            handler_test3_name + std::string(" ") + handler_test3_lib + \
            std::string("\n");

        virtual void SetUp()
        {
            cpplogger_log_file = std::string(TEST_BUILD_HOME) + \
                std::string("/cpplogger_conf_test.log");
            std::fstream cpplogger_log_fstream(cpplogger_log_file.c_str(),
                std::fstream::out);
            cpplogger_log_fstream.close();
            LoggerLogging::SetLogFileName(cpplogger_log_file);
            
            config_file = std::string(TEST_BUILD_HOME) + \
                          std::string("/cpplogger_conf_test.conf");
            config_fstream.open(config_file.c_str(), 
                std::fstream::in | std::fstream::out | std::fstream::trunc);

            config_fstream.write(test1_in_file.c_str(), test1_in_file.size());
            config_fstream.write(test2_in_file.c_str(), test2_in_file.size());
            config_fstream.write(test3_in_file.c_str(), test3_in_file.size());
            config_fstream.write(test4_in_file.c_str(), test4_in_file.size());
            config_fstream.write(test5_in_file.c_str(), test5_in_file.size());

            config_fstream.write(handler_test1_in_file.c_str(),
                handler_test1_in_file.size());
            config_fstream.write(handler_test2_in_file.c_str(),
                handler_test2_in_file.size());
            config_fstream.write(handler_test3_in_file.c_str(),
                handler_test3_in_file.size());

            config_fstream.close();
        }

        virtual void TearDown()
        {
            remove(cpplogger_log_file.c_str());
            remove(config_file.c_str());
        }
};

TEST_F(LoggerConfTest, GetLogLevelString)
{
    LoggerConf::SetConfigFile(this->config_file);
    
    LoggerConf::Init();

    std::map<std::string, LogLevel> str_loglevel_map = 
        LoggerConf::GetGroups(); 

    EXPECT_EQ(testcase_count, str_loglevel_map.size());

    EXPECT_EQ(test1_log_level, 
        LoggerConf::GetLogLevelString(test1_package_name));
    EXPECT_EQ(test2_log_level, 
        LoggerConf::GetLogLevelString(test2_package_name));
    EXPECT_EQ(test3_log_level, 
        LoggerConf::GetLogLevelString(test3_package_name));
    EXPECT_EQ(test4_log_level, 
        LoggerConf::GetLogLevelString(test4_package_name));
    EXPECT_EQ(test5_log_level, 
        LoggerConf::GetLogLevelString(test5_package_name));

    LoggerConf::CleanUp(); 
}


TEST_F(LoggerConfTest, GetLogLevelInt)
{
    LoggerConf::SetConfigFile(this->config_file);
    
    LoggerConf::Init();

    std::map<std::string, LogLevel> str_loglevel_map = 
        LoggerConf::GetGroups(); 
    EXPECT_EQ(testcase_count, str_loglevel_map.size());

    EXPECT_EQ(0, LoggerConf::GetLogLevelInt(test1_package_name));
    EXPECT_EQ(1, LoggerConf::GetLogLevelInt(test2_package_name));
    EXPECT_EQ(2, LoggerConf::GetLogLevelInt(test3_package_name));
    EXPECT_EQ(3, LoggerConf::GetLogLevelInt(test4_package_name));
    EXPECT_EQ(0, LoggerConf::GetLogLevelInt(test5_package_name));

    LoggerConf::CleanUp(); 
}

TEST_F(LoggerConfTest, GetLogLevelEnum)
{
    LoggerConf::SetConfigFile(this->config_file);
    
    LoggerConf::Init();

    std::map<std::string, LogLevel> str_loglevel_map = 
        LoggerConf::GetGroups(); 
    EXPECT_EQ(testcase_count, str_loglevel_map.size());

    EXPECT_EQ(DEFAULT, LoggerConf::GetLogLevelInt(test1_package_name));
    EXPECT_EQ(FINE, LoggerConf::GetLogLevelInt(test2_package_name));
    EXPECT_EQ(FINER, LoggerConf::GetLogLevelInt(test3_package_name));
    EXPECT_EQ(FINEST, LoggerConf::GetLogLevelInt(test4_package_name));
    EXPECT_EQ(DEFAULT, LoggerConf::GetLogLevelInt(test5_package_name));

    LoggerConf::CleanUp(); 
}

TEST_F(LoggerConfTest, IsGroupNameExists)
{
    LoggerConf::SetConfigFile(this->config_file);   
    LoggerConf::Init();

    EXPECT_EQ(true, 
        LoggerConf::IsGroupNameExists("cpplogger.package.test.default"));
    EXPECT_EQ(true, 
        LoggerConf::IsGroupNameExists("cpplogger.package.test.fine"));
    EXPECT_EQ(true, 
        LoggerConf::IsGroupNameExists("cpplogger.package.test.unknown"));
    EXPECT_TRUE(false == 
        LoggerConf::IsGroupNameExists("cpplogger.package.test.notexist"));

    LoggerConf::CleanUp();
}

TEST_F(LoggerConfTest, GetHandlerNameLibPathMap)
{
    LoggerConf::SetConfigFile(this->config_file);
    LoggerConf::Init();

    std::map<std::string, std::string> hlp_map =
        LoggerConf::GetHandlers();

    EXPECT_EQ(handler_test1_lib, hlp_map.find(handler_test1_name)->second);
    EXPECT_EQ(handler_test2_lib, hlp_map.find(handler_test2_name)->second);
    EXPECT_EQ(handler_test3_lib, hlp_map.find(handler_test3_name)->second);
    std::string unknown_handler = "NOT FOUND IN FILE";
    EXPECT_EQ(hlp_map.end(), hlp_map.find(unknown_handler));

    LoggerConf::CleanUp();
}

TEST_F(LoggerConfTest, GetHandlerLibPath)
{

    LoggerConf::SetConfigFile(this->config_file);
    LoggerConf::Init();

    EXPECT_EQ(handler_test1_lib, GetHandlerLibPath(
        handler_test1_name));
    EXPECT_EQ(handler_test2_lib, GetHandlerLibPath(
        handler_test2_name));
    EXPECT_EQ(handler_test3_lib, GetHandlerLibPath(
        handler_test3_name));

    LoggerConf::CleanUp();
}

TEST_F(LoggerConfTest, IsHandlerExists)
{
    LoggerConf::SetConfigFile(this->config_file);
    LoggerConf::Init();

    EXPECT_EQ(true, LoggerConf::IsHandlerExists("file"));
    EXPECT_EQ(true, LoggerConf::IsHandlerExists("xml"));
    EXPECT_TRUE(false == LoggerConf::IsHandlerExists("xyz"));

    LoggerConf::CleanUp();
}

TEST_F(LoggerConfTest, SetHandler)
{
    LoggerConf::SetConfigFile(this->config_file);
    LoggerConf::Init();

    EXPECT_TRUE(false == LoggerConf::IsHandlerExists("xyz"));
    LoggerConf::SetHandlerLibPath("xyz", "/temp/xyz.so");
    EXPECT_TRUE(true == LoggerConf::IsHandlerExists("xyz"));

    LoggerConf::CleanUp();
}

