#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Æ½ÐÐ¹â.
 */
class Light {
private:
	glm::vec3 direction;
	glm::vec3 color;
	glm::vec2 bias;

public:
	Light(glm::vec3 _direction, glm::vec3 _color, glm::vec2 _bias) :
		direction(_direction), color(_color), bias(_bias) {}

	glm::vec3 get_direction() {
		return direction;
	}

	glm::vec3 get_color() {
		return color;
	}

	glm::vec2 get_bias() {
		return bias;
	}
};
