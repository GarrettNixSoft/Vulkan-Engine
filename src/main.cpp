#include "game.hpp"
#include "core/fve_window.hpp"
#include "core/vulkan/fve_device.hpp"
#include "core/vulkan/fve_memory.hpp"
#include "fve_constants.hpp"
#include "core/fve_globals.hpp"
#include "core/utils/fve_logger.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cassert>

void runGame() {
    
    fve::FveLogger::init();
    fve::FVE_CORE_WARN("Initialized logger!");
    fve::FVE_INFO("Initialized logger!");

    fve::FveWindow window{ fve::WIDTH, fve::HEIGHT, "First Vulkan Game" };
    fve::FveDevice device{ window };

    {
        fve::Game game{ window, device };
        game.run();
    }

    vmaDestroyAllocator(fve::fveAllocator);

}

void waitOnExit() {
    while (std::cin.get() != '\n');
}

int main() {

    try {
        runGame();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        waitOnExit();
        return EXIT_FAILURE;
    }

    extern int BUFFER_ALLOCATIONS;
    if (BUFFER_ALLOCATIONS != 0) fve::FveLogger::getCoreLogger()->error("Buffer alloc/dealloc mismatch!");
    else fve::FVE_CORE_DEBUG("Buffer alloc/dealloc seems to match");
    
    waitOnExit();
    return EXIT_SUCCESS;
}