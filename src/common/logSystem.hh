#pragma once

#include <ctime>
#include <iomanip>
#include <string>
#include <fstream>
#include <memory>

class Log{
public:
    enum LogLevel{
        None = 0,
        Error,
        Warning,
        Info,
        Debug
    };
    Log();
    Log(const LogLevel _level, const std::string& path = ".", const std::string& logName = "raspberry.log");
    ~Log();
    void debug(const std::string& txt);
    void info(const std::string& txt);
    void warning(const std::string& txt);
    void error(const std::string& txt);
    // template <typename... Types>
	// void debug(const char* txt, Types... var);
    // template <typename... Types>
	// void info(const char* txt, Types... var);
    // template <typename... Types> 
    // void warning(const char* txt, Types... var);
    // template <typename... Types> 
    // void error(const char* txt, Types... var);
private:
    // static constexpr unsigned int maxDataBuffor = {200};
    // char data[maxDataBuffor];
    static std::unique_ptr<std::ofstream> logFiles;
    static unsigned int instancesCounter;
    static std::time_t time_now;
    static LogLevel level;
};

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/')? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define __HEADER__ std::string("(" + std::string(__FUNCTION__) + ":" + std::to_string(__LINE__) + "): ") +
#define TEST_LOG_HEADER Log log;log.info( \
      "-------------------------------------------------# " \
    + std::string(test_info_->test_case_name()) + "." + std::string(test_info_->name()) \
    + " #-------------------------------------------------");
