#ifndef CLOUD_GENERATOR
#define CLOUD_GENERATOR

#include "../objects/Cloud.hpp"
#include "../rendering/CloudRenderer.hpp"
#include "../config.h"

class CloudGenerator {
public:
	CloudRenderer* generate() {
		std::vector<Cloud>* clouds = new std::vector<Cloud>(CLOUD_BASE_COUNT + rand() % (CLOUD_BASE_COUNT / 2));
		for (auto& i : *clouds) i.generate();
		return new CloudRenderer(clouds);
	}
};

#endif // ! CLOUD_GENERATOR

