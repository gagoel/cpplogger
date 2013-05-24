/**
 =============================================================================
 @LICENSE:

 TODO - Add the license.
  
 @FileName                         cpplogger_utils.cpp
 @Creator                          gauravgoel9nov@gmail.com
 @CreationTimestamp                Sun Apr 28 01:55:22 2013
 @FileModifiers                    gauravgoel9nov@gmail.com

 =============================================================================
 **/

/**
 @FileDescription

 This implements LoggerUtils class methods.

 **/

#include <cctype>
#include <climits>
#include <cstdarg>

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

#include "cpplogger/cpplogger_utils.h"

namespace cpputils
{
namespace cpplogger
{

LoggerLogging LoggerUtils::logger("cpputils.cpplogger.LoggerUtils");

bool LoggerUtils::IsGroupNameSyntaxOk(const std::string& in_group_name)
{
    if(in_group_name.size() == 0)
    {
        logger.LogError("Group name can not be empty string.");
        return false;
    }

    unsigned int idx = 0;
    while(idx < in_group_name.size())
    {
        char ch = in_group_name.at(idx);
        if(ch == '.')
        {
            // '.' can not be first char.
            if(idx == 0) return false;

            // '.' can not be last char.
            if(idx == in_group_name.size() - 1) return false;

            // two '.' char can not come together.
            if(in_group_name.at(idx+1) == '.') return false;
        }
        else if(!(isalnum(ch) || ch == '_' || ch == '-')) 
        {
            return false;
        }
        ++idx;
    }
    return true;
}

void LoggerUtils::ParseFormattedString(std::string& out_unformatted_string,
    std::string& in_formatted_string, va_list& in_log_vargs_list)
{
    unsigned int idx = 0;
    unsigned int in_str_size = in_formatted_string.size();
    char ch, next_ch, parsed_ch;
    long parsed_num;
    char* parsed_ch_str;
    std::string parsed_str;
    double parsed_double;
    std::stringstream ss;

    while(idx < in_str_size)
    {
        ch = in_formatted_string.at(idx);

        // Char is specifier and it is not last char.  
        if(ch == '%' && idx+1 != in_str_size)
        {
            next_ch = in_formatted_string.at(++idx);

            switch(next_ch)
            {
                case 'i' :
                    parsed_num = va_arg(in_log_vargs_list, long);
                    ss.str("");    // Cleaning stream.
                    ss << parsed_num;
                    out_unformatted_string.append(ss.str());  
                    break;
                case 'c' :
                    parsed_ch = va_arg(in_log_vargs_list, int);
                    out_unformatted_string.push_back(parsed_ch);
                    break;
                case 's' :
                    parsed_ch_str = va_arg(in_log_vargs_list, char*);
                    out_unformatted_string.append(parsed_ch_str);
                    break;
                case 'f' :
                    parsed_double = va_arg(in_log_vargs_list, double);
                    ss.str("");
                    ss << parsed_double;
                    out_unformatted_string.append(ss.str());
                    break;
                case '%' :
                    out_unformatted_string.push_back('%');
                    break;
                default :
                    out_unformatted_string.push_back(' ');
                    std::string log_warning = "Unknow specifier passed %";
                    log_warning.push_back(next_ch);
                    logger.LogWarning(log_warning);
            }
        }
        else 
        {
            out_unformatted_string.push_back(ch);
        }
        ++idx;
    }
}

bool LoggerUtils::GetKeyValuePair(std::pair<std::string, std::string>& 
    out_key_value_pair, const char *in_str)
{
    int idx = 0;

    while(in_str[idx] != '\0' && isspace(in_str[idx])) ++idx;
    while(in_str[idx] != '\0' && !isspace(in_str[idx]))
    {
        out_key_value_pair.first.push_back(in_str[idx++]);
    }

    while(in_str[idx] != '\0' && isspace(in_str[idx])) ++idx;
    while(in_str[idx] != '\0') 
    {
        out_key_value_pair.second.push_back(in_str[idx++]);
    }
    return true;
}

bool LoggerUtils::ParseKeyValueFile(
    std::multimap<std::string, std::string>& out_key_val_map,
    const std::string& in_file_path)
{
    std::fstream file_stream;
    file_stream.open(in_file_path.c_str(), std::fstream::in);
    if(!file_stream.is_open())
    {
        logger.LogError("Error in opening key value pair file.");
        return false;
    }

    int line_num_counter = 1;

    while(!file_stream.eof() && file_stream.good())
    {
        char current_line[USHRT_MAX];
        file_stream.getline(current_line, USHRT_MAX);

        // checking for empty lines
        // TODO - Proper check for blank line.
        if(current_line[0] == '\0') continue;
            
        std::pair<std::string, std::string> key_val_pair;
        if(!LoggerUtils::GetKeyValuePair(key_val_pair, current_line))
        {
            file_stream.close();
            logger.LogError("Error in getting key value pair on line ");
            return false;
        }
        out_key_val_map.insert(key_val_pair);
        line_num_counter++;
    }

    if(file_stream.bad())
    {
        file_stream.close();
        logger.LogError("Read Write Error occurs on file");
        return false;
    }

    file_stream.close();
    return true;
}

void LoggerUtils::PrintMap(std::map<std::string, std::string>& map)
{
    std::map<std::string, std::string>::iterator map_it;
    for(map_it = map.begin(); map_it != map.end(); map_it++)
    {
        std::cout << "Key: " << map_it->first << " Value: " << \
            map_it->second << std::endl;
    }
    std::cout << std::endl;
}

bool LoggerUtils::IsFileAccessible(const std::string& in_file_name)
{
    std::fstream teststream(in_file_name.c_str(), std::fstream::in);
    if(teststream.good() == false)
    {
        teststream.close();
        return false;
    }
    teststream.close();
    return true;
}

}    // namespace cpplogger
}    // namespace cpputils
