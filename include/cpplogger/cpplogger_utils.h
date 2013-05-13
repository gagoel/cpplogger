/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_utils.h
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sun Apr 28 01:55:03 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This file declares class LoggerUtils.
  
 **/

#ifndef CPPUTILS_CPPLOGGER_UTILS_H_
#define CPPUTILS_CPPLOGGER_UTILS_H_

#include "cpplogger/cpplogger_logging.h"

namespace cpputils
{
namespace cpplogger
{

class LoggerUtils
{
    public :

        static LoggerLogging logger;

        static bool IsGroupNameSyntaxOk(const std::string& in_group_name);

        // @Method
        // Parses the input formatted string and replaces the variable values.
	    // Formatted string syntax is same as printf function.
	    // Format specifiers are :
	    // i - All type of integer value, it casts into 'const long'
	    // c - Conatant Char, it casts into 'const char'
	    // s - Constant Char array, it casts into 'const char*'
	    // f - Constant float value, It casts into 'const float', prints only
        //    first six digits, va_arg limitations.
	    // % - begining of specifier, %% will print %
        //
	    // If number of passed variables are more than number of specifiers,
	    // Lefts the extra variables. If number of specifiers are more than
	    // number of variables print specifier as it is.
	    // If unknow specifier passed it prints it as it is.
        //
	    // @param out_unformatted_string returns formatted string after 
	    //     replacing specifier with variable value.
	    // @param in_fomratted_string input formatted string.
	    // @param in_log_vargs_list variable arguments

        static void ParseFormattedString(
	        std::string& out_unformatted_string,
            std::string& in_formatted_string, 
	        va_list& in_log_vargs_list);

        static bool GetKeyValuePair(
	        std::pair<std::string, std::string>& out_key_value_pair,
	        const char *in_str);

	    static bool ParseKeyValueFile(
	        std::multimap<std::string, std::string>& out_key_val_map,
	        const std::string& in_file_path);

        static void PrintMap(std::map<std::string, std::string>& map);

        static bool IsFileAccessible(const std::string& in_file_name);
};

}    // namespace cpplogger
}    // namespace cpputils

#endif
