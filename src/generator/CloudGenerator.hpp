#ifndef CLOUD_GENERATOR
#define CLOUD_GENERATOR

#include "../objects/Cloud.hpp"
#include "../rendering/CloudRenderer.hpp"
#include <ctime>

class CloudGenerator {
public:
	CloudRenderer* generate() {
		std::vector<Cloud>* clouds = new std::vector<Cloud>(5 + rand() % 5);
		for (auto& i : *clouds) i.generate();
		return new CloudRenderer(clouds);
	}
};

#endif // ! CLOUD_GENERATOR

