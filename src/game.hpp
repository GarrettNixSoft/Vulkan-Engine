#pragma once

#include "core/fve_window.hpp"
#include "core/vulkan/fve_device.hpp"
#include "render/fve_renderer.hpp"
#include "fve_game_object.hpp"
#include "core/vulkan/fve_descriptors.hpp"

#include <vma/vk_mem_alloc.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace fve {

	class Game {
	public:

		Game(FveWindow& window, FveDevice& device);
		~Game();

		void init();
		void run();
	private:
	
	
		FveWindow& window;
		FveDevice& device;
		FveRenderer renderer{window, device};

		// note: order of declarations matters
		std::unique_ptr<FveDescriptorPool> globalPool{};
		std::unique_ptr<FveDescriptorSetLayout> globalSetLayout;
		std::unique_ptr<FveDescriptorSetLayout> texturedSetLayout;

		FveGameObject::Map gameObjects;

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		void loadTextures();
		void loadGameObjects();
	};

}