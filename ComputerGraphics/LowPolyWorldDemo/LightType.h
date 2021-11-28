#ifndef LIGHTTYPE_H
#define LIGHTTYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirLight
{
public:
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    DirLight()
    {
        this->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        this->diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        this->specular = glm::vec3(0.0f, 0.0f, 0.0f);
        this->direction = glm::vec3(0.0f, -1.0f, 0.0f);
    }
};
class PointLight
{
public:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    PointLight()
    {
        this->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        this->diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        this->specular = glm::vec3(0.0f, 0.0f, 0.0f);
        this->position = glm::vec3(0.0f, 1.0f, 0.0f);
        this->constant = 1.0f;
        this->linear = 0.09f;
        this->quadratic = 0.032f;
    }
};

class SpotLight
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    SpotLight()
    {
        this->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        this->diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        this->specular = glm::vec3(0.0f, 0.0f, 0.0f);
        this->position = glm::vec3(0.0f, 1.0f, 0.0f);
        this->constant = 1.0f;
        this->linear = 0.09f;
        this->quadratic = 0.032f;
        this->direction = glm::vec3(0.0f, -1.0f, 0.0f);
        this->cutOff = glm::cos(glm::radians(12.5f));
        this->outerCutOff = glm::cos(glm::radians(15.0f));
    }
};

#endif // !LIGHTTYPE_H
