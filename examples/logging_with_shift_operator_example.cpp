
#include <iostream>
#include <fstream>
#include <string>

#include "cpplogger/cpplogger.h"

using namespace cpputils::cpplogger;

void clean_logfile()
{
    std::string log_file_name = CPPLOGGER_DEFAULT_APP_LOG_FILE;
    std::fstream log_fstream(log_file_name.c_str(), std::fstream::out | \
        std::fstream::trunc);
    log_fstream.close();
}

void print_logfile()
{
    std::string log_file_name = CPPLOGGER_DEFAULT_APP_LOG_FILE; 
    std::fstream log_fstream(log_file_name.c_str(), std::fstream::in);

    if(!log_fstream.good())
    {
        std::cout << "Error in reading log file " << log_file_name;
        exit(EXIT_FAILURE);
    }

    while(!log_fstream.eof())
    {
        std::cout << (char)log_fstream.get();
    }

    std::cout << std::endl;
    log_fstream.close();
}

void string_sample()
{
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_logfile();

    std::string sample_desc =
        "Sample Code Description :- string message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "Logger::Init(); \n"
        "Logger& logger = Logger::CreateInstance(\"cpputils.cpplogger.examples\"); \n"
        "logger(ERROR) << \"This is string sample logging message.\" \n"
        "Logger::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    Logger::Init();
    Logger& logger = Logger::CreateInstance("cpputils.cpplogger.examples");
    logger(ERROR) << "This is string sample logging message.";
    Logger::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_logfile();
}

void int_sample()
{
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_logfile();

    std::string sample_desc =
        "Sample Code Description :- " 
        "string and int message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "Logger::Init(); \n"
        "Logger& logger = Logger::CreateInstance(\"cpputils.cpplogger.examples\"); \n"
        "logger(ERROR) << \"Int value is :- \" << 393949; \n"
        "Logger::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    Logger::Init();
    Logger& logger = Logger::CreateInstance("cpputils.cpplogger.examples");
    logger(ERROR) << "Int value is :- " << 393949;
    Logger::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_logfile();
}

void float_sample()
{
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_logfile();

    std::string sample_desc =
        "Sample Code Description :-"
        " integer, float and string message info logging with shift operator. "
        " It will not log anything as logging level is not FINE or higher \n\n"
        "Code Snippet :- \n"
        "Logger::Init(); \n"
        "Logger& logger = Logger::CreateInstance(\"cpputils.cpplogger.examples\"); \n"
        "logger(INFO) << \"Int value :- \" << 393939 << \"Float value is :- \" "
        "<< 43.38;\n"
        "Logger::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    Logger::Init();
    Logger& logger = Logger::CreateInstance("cpputils.cpplogger.examples");
    logger(INFO) << "Int value :- " << 393939 << " Float value is :- " << 43.38;
    Logger::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_logfile();
}

int main()
{
    string_sample();
    int_sample();
    float_sample();

    return 0;
}
