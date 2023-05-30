#pragma once

#include "../core/fve_types.hpp"
#include "../core/vulkan/fve_device.hpp"

namespace fve {

	bool loadImageFromFile(FveDevice& device, const char* filePath, AllocatedImage& outImage);

}