#include "logSystem.hh"
#include <thread>

std::unique_ptr<std::ofstream> Log::logFiles(nullptr);
unsigned int Log::instancesCounter = 0;
std::time_t Log::time_now = std::time(nullptr);
Log::LogLevel Log::level = Log::LogLevel::Info;

Log::Log(){
        if(!logFiles)
            throw std::invalid_argument("Log file is not initialised.");
        instancesCounter++;
    }

Log::Log(const LogLevel _level, const std::string& path, const std::string& logName){
        if(logFiles)
            throw std::invalid_argument("Log file is already initialised.");
        logFiles.reset(new std::ofstream(std::string(path + "/" + logName)));
        level = _level;
        instancesCounter++;
        (*logFiles) << "Logs created: " << std::put_time(std::localtime(&time_now), "%y-%m-%d %OH::%OM::%OS\n") << std::endl;
    }

Log::~Log(){
    instancesCounter--;
    if(instancesCounter == 0){
        logFiles->close();
        logFiles.release();
    }
}

void Log::debug(const std::string& txt){
    if(level >= LogLevel::Debug)
        (*logFiles) << std::put_time(std::localtime(&time_now), "%OH::%OM::%OS ") << std::hex << std::this_thread::get_id() << "   DEBUG: " << txt << std::endl;
}

void Log::info(const std::string& txt){
    if(level >= LogLevel::Info)
        (*logFiles) << std::put_time(std::localtime(&time_now), "%OH::%OM::%OS ") << std::hex << std::this_thread::get_id() << "    INFO: " << txt << std::endl;
}

void Log::warning(const std::string& txt){
    if(level >= LogLevel::Warning)
        (*logFiles) << std::put_time(std::localtime(&time_now), "%OH::%OM::%OS ") << std::hex << std::this_thread::get_id() << " WARNING: " << txt << std::endl;
}

void Log::error(const std::string& txt){
    if(level >= LogLevel::Error)
        (*logFiles) << std::put_time(std::localtime(&time_now), "%OH::%OM::%OS ") << std::hex << std::this_thread::get_id() << "   ERROR: " << txt << std::endl;
}
