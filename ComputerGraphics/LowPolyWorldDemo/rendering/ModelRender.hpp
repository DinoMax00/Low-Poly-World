#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../lib/camera.h"
#include "../lib/shader.h"
#include "../config.h"
#include "../lib/model.h"
#include "../LightType.h"

#include <iostream>

const char* SHADER_VS_PATH[] =
{
	"shaders/modelWithoutTextureMap.vs_",
	"shaders/modelWithTextureMap.vs_"
};
const char* SHADER_FS_PATH[] =
{
	"shaders/modelWithoutTextureMap.fs_",
	"shaders/modelWithTextureMap.fs_"
};
const int SHADER_NUM = 2;
const int MODEL_NUM = 6;
const string MODEL_SOURCE_PATH[] =
{
	"../ModelResource/tree1_maple_tree/MapleTree.obj",
	"../ModelResource/nanosuit/nanosuit.obj",
	"../ModelResource/tree2/Tree low.obj",
	"../ModelResource/tree3/lowpoyltree.obj",
	"../ModelResource/tree4/LowPoly_Tree_v1.obj",
	"../ModelResource/white_tailed_deer/12961_White-Tailed_Deer_v1_l2.obj"
};
//�»��ߺ�׺_0��ʾ����ͼ��_1��ʾ����ͼ
enum class ModelIndex
{
	TREE1_MAPLE_TREE_0,
	NANOSUIT_1,
	TREE2_0,
	TREE3_1,
	TREE4_0,
	WHITE_TAILED_DEER_1
};

const int shaderTypeOfModel[] = { 0,1,0,1,0,1 };

class ModelRender
{
private:
	Shader* modelShader[2];//0��Ⱦ����ͼģ��,1��Ⱦ����ͼģ��
	vector <Model*> models;
	DirLight dl;
	PointLight pl;
	SpotLight sl;
public:
	ModelRender()
	{
		//����������ɫ������
		for (auto i = 0; i < SHADER_NUM; i++)
		{
			this->modelShader[i] = new Shader(SHADER_VS_PATH[i], SHADER_FS_PATH[i]);
		}
		//��������ģ���ļ�
		for (auto i = 0; i < MODEL_NUM; i++)
		{
			Model* temp = new Model(MODEL_SOURCE_PATH[i]);
			this->models.push_back(temp);
		}
		
		//�Ӹ�Ĭ�Ϲ���
		this->dl.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
		this->dl.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		this->dl.specular = glm::vec3(0.5f, 0.5f, 0.5f);
		this->dl.direction= glm::vec3(-0.2f, -1.0f, -0.3f);
		//Ϊ������ɫ���������ó�ʼ����
		for (auto i = 0; i < SHADER_NUM; i++)
		{
			modelShader[i]->use();

			modelShader[i]->setVec3("dirLight.direction", this->dl.direction);
			modelShader[i]->setVec3("dirLight.ambient", this->dl.ambient);
			modelShader[i]->setVec3("dirLight.diffuse", this->dl.diffuse);
			modelShader[i]->setVec3("dirLight.specular", this->dl.specular);

			modelShader[i]->setVec3("pointLight.position", this->pl.position);
			modelShader[i]->setVec3("pointLight.ambient", this->pl.ambient);
			modelShader[i]->setVec3("pointLight.diffuse", this->pl.diffuse);
			modelShader[i]->setVec3("pointLight.specular", this->pl.specular);
			modelShader[i]->setFloat("pointLight.constant", this->pl.constant);
			modelShader[i]->setFloat("pointLight.linear", this->pl.linear);
			modelShader[i]->setFloat("pointLight.quadratic", this->pl.quadratic);

			modelShader[i]->setVec3("spotLight.position", this->sl.position);
			modelShader[i]->setVec3("spotLight.direction", this->sl.direction);
			modelShader[i]->setVec3("spotLight.ambient", this->sl.ambient);
			modelShader[i]->setVec3("spotLight.diffuse", this->sl.diffuse);
			modelShader[i]->setVec3("spotLight.specular", this->sl.specular);
			modelShader[i]->setFloat("spotLight.constant", this->sl.constant);
			modelShader[i]->setFloat("spotLight.linear", this->sl.linear);
			modelShader[i]->setFloat("spotLight.quadratic", this->sl.quadratic);
			modelShader[i]->setFloat("spotLight.cutOff", this->sl.cutOff);
			modelShader[i]->setFloat("spotLight.outerCutOff", this->sl.outerCutOff);
		}
	}
	void setDirLight(const DirLight& dirl)
	{
		this->dl = dirl;
		for (auto i = 0; i < SHADER_NUM; i++)
		{
			modelShader[i]->use();

			modelShader[i]->setVec3("dirLight.direction", this->dl.direction);
			modelShader[i]->setVec3("dirLight.ambient", this->dl.ambient);
			modelShader[i]->setVec3("dirLight.diffuse", this->dl.diffuse);
			modelShader[i]->setVec3("dirLight.specular", this->dl.specular);
		}
	}
	void setPointLight(const PointLight& poil)
	{
		this->pl = poil;
		for (auto i = 0; i < SHADER_NUM; i++)
		{
			modelShader[i]->use();

			modelShader[i]->setVec3("pointLight.position", this->pl.position);
			modelShader[i]->setVec3("pointLight.ambient", this->pl.ambient);
			modelShader[i]->setVec3("pointLight.diffuse", this->pl.diffuse);
			modelShader[i]->setVec3("pointLight.specular", this->pl.specular);
			modelShader[i]->setFloat("pointLight.constant", this->pl.constant);
			modelShader[i]->setFloat("pointLight.linear", this->pl.linear);
			modelShader[i]->setFloat("pointLight.quadratic", this->pl.quadratic);
		}
	}
	void setSpotLight(const SpotLight& spotl)
	{
		this->sl = spotl;
		for (auto i = 0; i < SHADER_NUM; i++)
		{
			modelShader[i]->use();

			modelShader[i]->setVec3("spotLight.position", this->sl.position);
			modelShader[i]->setVec3("spotLight.direction", this->sl.direction);
			modelShader[i]->setVec3("spotLight.ambient", this->sl.ambient);
			modelShader[i]->setVec3("spotLight.diffuse", this->sl.diffuse);
			modelShader[i]->setVec3("spotLight.specular", this->sl.specular);
			modelShader[i]->setFloat("spotLight.constant", this->sl.constant);
			modelShader[i]->setFloat("spotLight.linear", this->sl.linear);
			modelShader[i]->setFloat("spotLight.quadratic", this->sl.quadratic);
			modelShader[i]->setFloat("spotLight.cutOff", this->sl.cutOff);
			modelShader[i]->setFloat("spotLight.outerCutOff", this->sl.outerCutOff);
		}
	}
	//cameraΪ��ͷ��shaderTypeΪʹ����ɫ�������ͣ�0������ͼģ�ͣ�1������ͼģ�ͣ�modelMatr����ģ��
	//��model���󣬿���λ�á��������ű�����miΪ����ģ�͵ı��
	void render(Camera& camera, const int shaderType, glm::mat4 modelMatr, ModelIndex mi)
	{
		modelShader[shaderType]->use();

		//�����ֵ�Ͳ������������Է�������main.cpp��whileѭ���У�ÿ�λ�ͼǰ
		//�ȸ���setSpotLight�������ȷ����棬����Ҳû��
		modelShader[shaderType]->setVec3("spotLight.position", camera.Position);
		modelShader[shaderType]->setVec3("spotLight.direction", camera.Front);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_H / (float)WINDOW_W, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		modelShader[shaderType]->setMat4("projection", projection);
		modelShader[shaderType]->setMat4("view", view);
		modelShader[shaderType]->setVec3("viewPos", camera.Position);

		modelShader[shaderType]->setMat4("model", modelMatr);
		models[(int)mi]->Draw(*modelShader[shaderType]);
	}
};