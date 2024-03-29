#pragma once

#include "assets/fve_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace fve {

	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{1.0f, 1.0f, 1.0f};
		glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	struct PointLightComponent {
		float lightIntensity = 1.0f;
	};

	struct TextureComponent {
		std::string textureName;
	};

	class FveGameObject {
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, FveGameObject>;

		static FveGameObject createGameObject() {
			static id_t currentId = 0;
			return FveGameObject{ currentId++ };
		}

		static FveGameObject makePointLight(float lightIntensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.0f));

		id_t getId() { return id; }

		FveGameObject(const FveGameObject&) = delete;
		FveGameObject& operator=(const FveGameObject&) = delete;
		FveGameObject(FveGameObject&&) = default;
		FveGameObject& operator=(FveGameObject&&) = default;

		glm::vec3 color{};
		TransformComponent transform{};

		// optional pointer components
		FveModel* model = nullptr;
		std::unique_ptr<PointLightComponent> pointLight = nullptr;
		std::shared_ptr<TextureComponent> texture = nullptr;

	private:
		id_t id;

		FveGameObject(id_t objId) : id{ objId } {}
	};
}