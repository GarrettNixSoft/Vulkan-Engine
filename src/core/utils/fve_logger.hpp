#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace fve {
    
    class FveLogger {
    public:
        
        static void init();
        
        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
        
    private:
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
        
    };
    
}

// Macros for Core Logging
#define FVE_CORE_FATAL(...) FveLogger::getCoreLogger()->fatal(__VA_ARGS__)
#define FVE_CORE_ERROR(...) FveLogger::getCoreLogger()->error(__VA_ARGS__)
#define FVE_CORE_WARN(...)  FveLogger::getCoreLogger()->warn(__VA_ARGS__)
#define FVE_CORE_INFO(...)  FveLogger::getCoreLogger()->info(__VA_ARGS__)
#define FVE_CORE_DEBUG(...) FveLogger::getCoreLogger()->debug(__VA_ARGS__)
#define FVE_CORE_TRACE(...) FveLogger::getCoreLogger()->trace(__VA_ARGS__)

// Macros for Client Logging
#define FVE_FATAL(...) FveLogger::getClientLogger()->fatal(__VA_ARGS__)
#define FVE_ERROR(...) FveLogger::getClientLogger()->error(__VA_ARGS__)
#define FVE_WARN(...)  FveLogger::getClientLogger()->warn(__VA_ARGS__)
#define FVE_INFO(...)  FveLogger::getClientLogger()->info(__VA_ARGS__)
#define FVE_DEBUG(...) FveLogger::getClientLogger()->debug(__VA_ARGS__)
#define FVE_TRACE(...) FveLogger::getClientLogger()->trace(__VA_ARGS__)