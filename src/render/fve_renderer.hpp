#pragma once

#include "../core/fve_window.hpp"
#include "../core/vulkan/fve_device.hpp"
#include "../core/vulkan/fve_swap_chain.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace fve {

	class FveRenderer {
	public:

		FveRenderer(FveWindow& window, FveDevice& device);
		~FveRenderer();

		VkRenderPass getSwapChainRenderPass() const {
			return swapChain->getRenderPass();
		}

		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when a frame is not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when a frame is not in progress");
			return currentFrameIndex;

		}

		VkCommandBuffer beginFrame();
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		float getAspectRatio() const { return swapChain->extentAspectRatio(); }
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		FveWindow& window;
		FveDevice& device;
		std::unique_ptr<FveSwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex = 0;
		bool isFrameStarted = false;

		FveRenderer(const FveRenderer&) = delete;
		FveRenderer& operator=(const FveRenderer&) = delete;

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
	};

}