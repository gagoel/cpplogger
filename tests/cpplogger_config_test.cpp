#include <cstdio>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>

#include "cpplogger/cpplogger_utils.h"
#include "cpplogger/cpplogger_config.h"
#include "gtest/gtest.h"

using namespace cpputils::cpplogger;

class LoggerConfigTest : public ::testing::Test
{
    protected :
        std::string appname;
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
            appname = "cpplogger-config-test";

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

TEST_F(LoggerConfigTest, GetLogLevelString)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);
    
    EXPECT_EQ(test1_log_level, 
        config_obj.GetLogLevelString(test1_package_name));
    EXPECT_EQ(test2_log_level, 
        config_obj.GetLogLevelString(test2_package_name));
    EXPECT_EQ(test3_log_level, 
        config_obj.GetLogLevelString(test3_package_name));
    EXPECT_EQ(test4_log_level, 
        config_obj.GetLogLevelString(test4_package_name));
    EXPECT_EQ(test5_log_level, 
        config_obj.GetLogLevelString(test5_package_name));

    LoggerConfig::CleanUp(); 
}

TEST_F(LoggerConfigTest, GetLogLevelInt)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    EXPECT_EQ(0, config_obj.GetLogLevelInt(test1_package_name));
    EXPECT_EQ(1, config_obj.GetLogLevelInt(test2_package_name));
    EXPECT_EQ(2, config_obj.GetLogLevelInt(test3_package_name));
    EXPECT_EQ(3, config_obj.GetLogLevelInt(test4_package_name));
    EXPECT_EQ(0, config_obj.GetLogLevelInt(test5_package_name));

    LoggerConfig::CleanUp(); 
}

TEST_F(LoggerConfigTest, GetLogLevelEnum)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    EXPECT_EQ(0, config_obj.GetLogLevelInt(test1_package_name));
    EXPECT_EQ(1, config_obj.GetLogLevelInt(test2_package_name));
    EXPECT_EQ(2, config_obj.GetLogLevelInt(test3_package_name));
    EXPECT_EQ(3, config_obj.GetLogLevelInt(test4_package_name));
    EXPECT_EQ(0, config_obj.GetLogLevelInt(test5_package_name));

    LoggerConfig::CleanUp(); 
}

TEST_F(LoggerConfigTest, IsGroupNameExists)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    EXPECT_EQ(true, 
        config_obj.IsGroupNameExists("cpplogger.package.test.default"));
    EXPECT_EQ(true, 
        config_obj.IsGroupNameExists("cpplogger.package.test.fine"));
    EXPECT_EQ(true, 
        config_obj.IsGroupNameExists("cpplogger.package.test.unknown"));
    EXPECT_TRUE(false == 
        config_obj.IsGroupNameExists("cpplogger.package.test.notexist"));

    LoggerConfig::CleanUp();
}

TEST_F(LoggerConfigTest, GetHandlerNameLibPathMap)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    std::map<std::string, std::string> hlp_map =
        config_obj.GetHandlers();

    EXPECT_EQ(handler_test1_lib, hlp_map.find(handler_test1_name)->second);
    EXPECT_EQ(handler_test2_lib, hlp_map.find(handler_test2_name)->second);
    EXPECT_EQ(handler_test3_lib, hlp_map.find(handler_test3_name)->second);
    std::string unknown_handler = "NOT FOUND IN FILE";
    EXPECT_EQ(hlp_map.end(), hlp_map.find(unknown_handler));

    LoggerConfig::CleanUp();
}

TEST_F(LoggerConfigTest, GetHandlerLibPath)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    EXPECT_EQ(handler_test1_lib, config_obj.GetHandlerLibPath(
        handler_test1_name));
    EXPECT_EQ(handler_test2_lib, config_obj.GetHandlerLibPath(
        handler_test2_name));
    EXPECT_EQ(handler_test3_lib, config_obj.GetHandlerLibPath(
        handler_test3_name));

    LoggerConfig::CleanUp();
}

TEST_F(LoggerConfigTest, IsHandlerExists)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    EXPECT_EQ(true, config_obj.IsHandlerExists("file"));
    EXPECT_EQ(true, config_obj.IsHandlerExists("xml"));
    EXPECT_TRUE(false == config_obj.IsHandlerExists("xyz"));

    LoggerConfig::CleanUp();
}

TEST_F(LoggerConfigTest, SetHandler)
{
    LoggerConfig::Init();
    LoggerConfig& config_obj = LoggerConfig::GetLoggerConfigObject(
        this->appname, this->config_file);

    EXPECT_TRUE(false == config_obj.IsHandlerExists("xyz"));
    config_obj.SetHandlerLibPath("xyz", "/temp/xyz.so");
    EXPECT_TRUE(true == config_obj.IsHandlerExists("xyz"));

    LoggerConfig::CleanUp();
}

