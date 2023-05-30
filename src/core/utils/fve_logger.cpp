#include "fve_logger.hpp"

namespace fve {
    
    std::shared_ptr<spdlog::logger> FveLogger::coreLogger;
    std::shared_ptr<spdlog::logger> FveLogger::clientLogger;
    
    void FveLogger::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        coreLogger = spdlog::stdout_color_mt("FVE");
        coreLogger->set_level(spdlog::level::trace);
        clientLogger = spdlog::stdout_color_mt("APP");
        clientLogger->set_level(spdlog::level::trace);
    }
    
}