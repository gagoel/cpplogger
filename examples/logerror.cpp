#include <iostream>
#include <fstream>
#include <string>

#include "cpputils/cpplogger/cpplogger.h"

using namespace cpputils::cpplogger;

void clean_logfile()
{
    std::string log_file_name = APP_DEFAULT_LOG_FILE;
    std::fstream log_fstream(log_file_name.c_str(), std::fstream::out | \
        std::fstream::trunc);
    log_fstream.close();
}

void print_logfile()
{
    std::string log_file_name = APP_DEFAULT_LOG_FILE; 
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

void print_block_start(std::string sample_name)
{
    int line_size = 80;
    for(int i = 0; i < line_size; i++)
    {
        std::cout << "#";    
    }
    std::cout << std::endl;
    std::cout << "Error Logging Sample '" << sample_name << "' Starting...";
    std::cout << std::endl;

    for(int i = 0; i < line_size; i++)
    {
        std::cout << "#";    
    }
    std::cout << std::endl;
}

void print_block_end(std::string sample_name)
{
    int line_size = 80;
    for(int i = 0; i < line_size; i++)
    {
        std::cout << "#";    
    }
    std::cout << std::endl;
    std::cout << "Error Logging Sample '" << sample_name << "' Ending...";
    std::cout << std::endl;

    for(int i = 0; i < line_size; i++)
    {
        std::cout << "#";    
    }
    std::cout << std::endl;
}

void string_sample()
{
    print_block_start("string_sample");
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_logfile();

    std::string sample_desc =
        "Sample Code Description :- "
        "string message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManger::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(ERROR) << \"This is string sample logging message.\" \n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    LogManager::Init("error_string_sample");

    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    logger(ERROR) << "This is sample error logging message for string.";
    
    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_logfile();
    print_block_end("string_sample");
}

void int_sample()
{
    print_block_start("int_sample");
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_logfile();

    std::string sample_desc =
        "Sample Code Description :- " 
        "string and int message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManager::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(ERROR) << \"Int value is :- \" << 393949; \n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    LogManager::Init("ERRORLog-Int-Sample");

    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    
    logger(ERROR) << "This is sample error logging for int value- " << 
        "Int value is :- " << 393949;

    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_logfile();
    print_block_end("int_sample");
}

void float_sample()
{
    print_block_start("float_sample");
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_logfile();

    std::string sample_desc =
        "Sample Code Description :-"
        " integer, float and string message error logging with shift operator."
        "\n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManager::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(ERROR) << \"Int value :- \" << 393939 << \"Float value is :- \" "
        "<< 43.38;\n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    LogManager::Init("error_float_sample");
    
    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    
    logger(ERROR) << "This is sample error logging message for int and " <<
        "float values- " << "Int value :- " << 393939 << " Float value is :- " 
        << 43.38;
    
    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_logfile();
    print_block_end("float_sample");
}

int main()
{
    std::cout << std::endl;
    string_sample();
    std::cout << std::endl;
    int_sample();
    std::cout << std::endl;
    float_sample();
    std::cout << std::endl;

    return 0;
}
