#include <cstdio>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>

#include "cpputils/cpplogger/cpplogger_utils.h"
#include "gtest/gtest.h"

using namespace cpputils::cpplogger;

class LoggerUtilsTest : public ::testing::Test
{
    protected:

    std::string cpplogger_log_file; 

    virtual void SetUp()
    {
        cpplogger_log_file = std::string(TEST_BUILD_HOME) + \
            std::string("/cpplogger_utils_test.log");
        std::fstream cpplogger_log_fstream(cpplogger_log_file.c_str(),
            std::fstream::out);
        cpplogger_log_fstream.close();
        LoggerLogging::SetLogFileName(cpplogger_log_file);
    }

    virtual void TearDown()
    {
        remove(cpplogger_log_file.c_str());
    }
};


TEST_F(LoggerUtilsTest, IsGroupNameSyntaxOk)
{
    // Empty string does not allow.
    std::string group_name_test1 = "";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test1) == false);

    // Starting char can not be '.'
    std::string group_name_test2 = ".testpackage.testmodule";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test2) == false);

    // Only alphabet and numbers allowed
    std::string group_name_test3 = "testpackage1.testmodule";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test3) == true);

    // two '.' char cannot be together
    std::string group_name_test4 = "testpackage..testmodule";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test4) == false);

    // '.' char cannot be last char.
    std::string group_name_test5 = "testpackage.testmodule.";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test5) == false);

    // Testing other not allowed chars.
    std::string group_name_test6 = "testpackage().testmodule";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test6) == false);

    // Working case
    std::string group_name_test7 = "testpackage.testmodule";
    EXPECT_TRUE(LoggerUtils::IsGroupNameSyntaxOk(group_name_test7) == true);
}

std::string ParseFormattedStringTest(
    std::string formatted_str,
    ...)
{
    va_list valist;
    va_start(valist, formatted_str);

    std::string return_str;
    LoggerUtils::ParseFormattedString(return_str, formatted_str, valist);

    va_end(valist); 
    return return_str;
}

TEST_F(LoggerUtilsTest, ParseFormattedString)
{
    // Char input argument test.
    std::string test1_formatted_string = "Char1 %c and Char2 %c";
    char test1_arg1 = 'a';
    char test1_arg2 = 'b';
    std::string test1_expected_output = "Char1 a and Char2 b";

    EXPECT_EQ(test1_expected_output, ParseFormattedStringTest( 
        test1_formatted_string, test1_arg1, test1_arg2));

    // Int input argument test.
    std::string test2_formatted_string = " Int1 %i and Char1 %c ";
    int test2_arg1 = 122333;
    char test2_arg2 = 'x';
    std::string test2_expected_output = " Int1 122333 and Char1 x ";

    EXPECT_EQ(test2_expected_output, ParseFormattedStringTest( 
        test2_formatted_string, test2_arg1, test2_arg2));

    // Float input argument test.
    // Float print only 6 digits, va_arg limitations.
    std::string test3_formatted_string = " Float %f Int1 %i and Char1 %c ";
    double test3_arg1 = 345.444455555;
    int test3_arg2 = 122333;
    char test3_arg3 = 'x';
    std::string test3_expected_output = " Float 345.444 Int1 122333 ";
    test3_expected_output += "and Char1 x ";

    EXPECT_EQ(test3_expected_output, ParseFormattedStringTest(
        test3_formatted_string, test3_arg1, test3_arg2, test3_arg3));

    // Char string.
    std::string test4_formatted_string = "Char String1 %s and Char1 %c";
    const char* test4_arg1 = "Test4 Argument";
    char test4_arg2 = 'y';
    std::string test4_expected_output = 
        "Char String1 Test4 Argument and Char1 y";

    EXPECT_EQ(test4_expected_output, ParseFormattedStringTest(
        test4_formatted_string, test4_arg1, test4_arg2));

    // '%%' char
    std::string test5_formatted_string = " Two ampresend and one char %%%c ";
    char test5_arg1 = 'a';
    std::string test5_expected_output = " Two ampresend and one char %a ";

    EXPECT_EQ(test5_expected_output, ParseFormattedStringTest(
        test5_formatted_string, test5_arg1));

    // Unknow specifier.
    std::string test6_formatted_string = "Char1 %i and Char2 %t";
    int test6_arg1 = 500;
    std::string test6_expected_output = "Char1 500 and Char2  ";

    EXPECT_EQ(test6_expected_output, ParseFormattedStringTest(
        test6_formatted_string, test6_arg1));
}

TEST_F(LoggerUtilsTest, GetKeyValuePair)
{
    // Working key value pair
    std::pair<std::string, std::string> key_value_pair_test1;
    LoggerUtils::GetKeyValuePair(key_value_pair_test1, " KEY_STRING    VALUE");
    EXPECT_EQ(std::string("KEY_STRING"), key_value_pair_test1.first);
    EXPECT_EQ(std::string("VALUE"), key_value_pair_test1.second);

    // Only key
    std::pair<std::string, std::string> key_value_pair_test2;
    LoggerUtils::GetKeyValuePair(key_value_pair_test2, "ONLY_KEY_STRING");
    EXPECT_EQ(std::string("ONLY_KEY_STRING"), key_value_pair_test2.first);
    EXPECT_EQ(std::string(""), key_value_pair_test2.second);

    // Value with space
    std::pair<std::string, std::string> key_value_pair_test3;
    LoggerUtils::GetKeyValuePair(key_value_pair_test3, "KEY_STRING \
        SPACE SEPERATED STRING");
    EXPECT_EQ(std::string("KEY_STRING"), key_value_pair_test3.first);
    EXPECT_EQ(std::string("SPACE SEPERATED STRING"),
        key_value_pair_test3.second);
}

TEST_F(LoggerUtilsTest, ParseKeyValueFile)
{
    std::string key_value_file = std::string(TEST_BUILD_HOME) + \
        "/cpplogger_test_parsekeyvaluefile.conf";

    std::fstream fd(key_value_file.c_str(), std::fstream::in | 
        std::fstream::out | std::fstream::trunc);

    // Writing test data into file.
    std::string key_test1 = "KEY_NAME";
    std::string value_test1 = "KEY_VALUE";
    fd << key_test1 << " " << value_test1 << std::endl;

    std::string key_test2 = "    KEY_NAME_WITH_SPACE ";
    std::string value_test2 = "  KEY VALUE WITH SPACE";
    fd << key_test2 << " " << value_test2 << std::endl;

    // Closing file.
    fd.close();

    // Key value parsed successfully test.
    std::multimap<std::string, std::string> key_value_map;
    EXPECT_TRUE(LoggerUtils::ParseKeyValueFile(key_value_map, key_value_file) 
        == true);

    // Number of entries in map equal to file entries.
    EXPECT_EQ(2, key_value_map.size());

    // Testing key value parsed and store into map.
    EXPECT_EQ("KEY_VALUE", key_value_map.find("KEY_NAME")->second);
    EXPECT_EQ("KEY VALUE WITH SPACE", key_value_map.find(
        "KEY_NAME_WITH_SPACE")->second);

    // Deleting test conf file.
    remove(key_value_file.c_str());
}
