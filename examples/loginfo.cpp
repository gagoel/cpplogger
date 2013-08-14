#include <iostream>
#include <fstream>
#include <string>

#include "cpputils/cpplogger/cpplogger.h"

using namespace cpputils::cpplogger;

void clean_file(std::string in_file_name)
{
    std::fstream file_stream(in_file_name.c_str(), std::fstream::out | \
        std::fstream::trunc);
    file_stream.close();
}

void print_file(std::string in_file_name)
{
    std::fstream file_stream(in_file_name.c_str(), std::fstream::in);

    if(!file_stream.good())
    {
        std::cout << "error in reading input file " << in_file_name;
        exit(EXIT_FAILURE);
    }

    while(!file_stream.eof())
    {
        std::cout << (char)file_stream.get();
    }

    std::cout << std::endl;
    file_stream.close();
}

void write_to_file(std::string in_file_name, std::string in_string)
{
    std::fstream stream(in_file_name, std::fstream::out | 
        std::fstream::app);
    stream.write(in_string.c_str(), in_string.size());
    stream.close();
}

std::string read_file_as_string(std::string in_file_name)
{
    std::fstream stream(in_file_name, std::fstream::in);
    std::string file_string = "";

    stream.seekg(0, std::fstream::beg);
   
    char ch; 
    while((ch = (char)stream.get()) && !stream.eof())
    {
        file_string.push_back(ch);
    }

    stream.close();
    return file_string;
}

void clean_default_logfile()
{
    clean_file(APP_DEFAULT_LOG_FILE);
}

void print_default_logfile()
{
    print_file(APP_DEFAULT_LOG_FILE); 
}

void print_default_config_file()
{
    std::string config_file_name = APP_DEFAULT_CONFIG_FILE; 
    std::cout << std::endl;
    std::cout << "Default Configuration File" << std::endl;
    std::cout << "------------------" << std::endl;
    print_file(APP_DEFAULT_CONFIG_FILE);
    std::cout << std::endl;
    std::cout << "------------------" << std::endl << std::endl;
}

// Create temp configuration file. add default file entries to temp
// configuration file. Returns the configuration file.
std::string get_temp_config_file()
{
    // temp configuration file name.
    std::string default_config_file = APP_DEFAULT_CONFIG_FILE;
    const char *temp_config_file_dir = default_config_file.c_str();
    char *last_slash = (char*) strrchr(temp_config_file_dir, '/');
    if(last_slash)
        *(last_slash + 1) = '\0';
    std::string temp_config_file = std::string(temp_config_file_dir) + 
        "sample_temp_config_file.conf";

    // Opening default and temp config files.
    std::string default_file_string = read_file_as_string(
        APP_DEFAULT_CONFIG_FILE);
    write_to_file(temp_config_file, default_file_string);

    return temp_config_file;
}

void del_config_file(std::string config_file)
{
    remove(config_file.c_str());
}

void print_block_start(std::string sample_name)
{
    int line_size = 80;
    for(int i = 0; i < line_size; i++)
    {
        std::cout << "#";    
    }
    std::cout << std::endl;
    std::cout << "Info Logging Sample '" << sample_name << "' Starting...";
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
    std::cout << "Info Logging Sample '" << sample_name << "' Ending...";
    std::cout << std::endl;

    for(int i = 0; i < line_size; i++)
    {
        std::cout << "#";    
    }
    std::cout << std::endl;
}

void string_sample_with_default_logging()
{
    print_block_start("string_sample_with_default_logging");
    // Sample logging description.
    std::string sample_desc =
        "Sample Code Description :- "
        "string message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManger::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(INFO) << \"This is string sample logging message.\" \n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    // Logging with default configuration entries.
    print_default_config_file();
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_default_logfile();

    LogManager::Init("info_string_sample");
    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    logger(INFO) << "This is sample info logging message for string.";
    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_default_logfile();
    print_block_end("string_sample_with_default_logging");
}

void string_sample_with_fine_logging()
{
    print_block_start("string_sample_with_fine_logging");
    // Logging with FINE loging configuration entries.
    // SetLogLevel cpputils.cpplogger.examples FINE
    std::string temp_config_file = get_temp_config_file();
    std::string config_file_entry = 
        "SetLogLevel cpputils.cpplogger.examples FINE";
    write_to_file(temp_config_file, config_file_entry);
    std::cout << "Configuration file" << std::endl;
    std::cout << "------------------" << std::endl;
    print_file(temp_config_file);
    std::cout << "------------------" << std::endl;

    std::string sample_desc =
        "Sample Code Description :- "
        "string message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManger::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(INFO) << \"This is string sample logging message.\" \n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;
    
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_default_logfile();

    LogManager::Init("info_string_sample", temp_config_file);
    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    logger(INFO) << "This is sample info logging message for string.";
    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_default_logfile();

    del_config_file(temp_config_file);
    print_block_end("string_sample_with_fine_logging");
}

void string_sample_with_finer_logging()
{
    print_block_start("string_sample_with_finer_logging");
    // Logging with FINE loging configuration entries.
    // SetLogLevel cpputils.cpplogger.examples FINE
    std::string temp_config_file = get_temp_config_file();
    std::string config_file_entry = 
        "SetLogLevel cpputils.cpplogger.examples FINER";
    write_to_file(temp_config_file, config_file_entry);
    std::cout << "Configuration file" << std::endl;
    std::cout << "------------------" << std::endl;
    print_file(temp_config_file);
    std::cout << "------------------" << std::endl;

    std::string sample_desc =
        "Sample Code Description :- "
        "string message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManger::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(INFO) << \"This is string sample logging message.\" \n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;
    
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_default_logfile();

    LogManager::Init("info_string_sample", temp_config_file);
    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    logger(INFO) << "This is sample info logging message for string.";
    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_default_logfile();

    del_config_file(temp_config_file);
    print_block_end("string_sample_with_finer_logging");
}

void int_sample()
{
    print_block_start("int_sample");
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_default_logfile();

    std::string sample_desc =
        "Sample Code Description :- " 
        "string and int message logging with shift operator. \n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManager::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(INFO) << \"Int value is :- \" << 393949; \n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    LogManager::Init("INFOLog-Int-Sample");

    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    
    logger(INFO) << "This is sample info logging for int value- " << 
        "Int value is :- " << 393949;

    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_default_logfile();
    print_block_end("int_sample");
}

void float_sample()
{
    print_block_start("float_sample");
    std::cout << "Cleaning Log File...\n" << std::endl;
    clean_default_logfile();

    std::string sample_desc =
        "Sample Code Description :-"
        " integer, float and string message info logging with shift operator."
        "\n\n"
        "Code Snippet :- \n"
        "LogManager::Init(application name string); \n"
        "Logger& logger = "
        "LogManager::GetLogger(\"cpputils.cpplogger.examples\"); \n"
        "logger(INFO) << \"Int value :- \" << 393939 << \"Float value is :- \" "
        "<< 43.38;\n"
        "LogManager::CleanUp(); \n"; 

    std::cout << sample_desc << std::endl;

    LogManager::Init("info_float_sample");
    
    Logger& logger = LogManager::GetLogger("cpputils.cpplogger.examples");
    
    logger(INFO) << "This is sample info logging message for int and " <<
        "float values- " << "Int value :- " << 393939 << " Float value is :- " 
        << 43.38;
    
    LogManager::CleanUp(); 

    std::cout << "Log File Output :-" << std::endl;
    print_default_logfile();
    print_block_end("float_sample");
}

int main()
{
    std::cout << std::endl;
    string_sample_with_default_logging();
    std::cout << std::endl;
    string_sample_with_fine_logging();
    std::cout << std::endl;
    string_sample_with_finer_logging();
    std::cout << std::endl;
    int_sample();
    std::cout << std::endl;
    float_sample();
    std::cout << std::endl;

    return 0;
}

