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
//模型着色器路径
const char* MODEL_SHADER_VS_PATH = "shaders/model.vs_";
const char* MODEL_SHADER_FS_PATH = "shaders/model.fs_";
//加载模型数量，添加模型时该值加一
const int MODEL_NUM = 8;
//模型文件路径，添加模型时在末尾加入新模型路径
const string MODEL_SOURCE_PATH[] =
{
	"../ModelResource/tree1_maple_tree/MapleTree.obj",
	"../ModelResource/nanosuit/nanosuit.obj",
	"../ModelResource/tree2/Tree low.obj",
	"../ModelResource/tree3/lowpoyltree.obj",
	"../ModelResource/tree4/LowPoly_Tree_v1.obj",
	"../ModelResource/white_tailed_deer/12961_White-Tailed_Deer_v1_l2.obj",
	"../ModelResource/dog1/big_shiba_inu_dog_low_poly.obj",
	"../ModelResource/horse1/horse1.OBJ"
};
//模型索引，下划线后缀_0表示无贴图，_1表示有贴图，添加模型时按类似格式在末尾加入枚举变量
enum class ModelIndex
{
	TREE1_MAPLE_TREE_0,
	NANOSUIT_1,
	TREE2_0,
	TREE3_1,
	TREE4_0,
	WHITE_TAILED_DEER_1,
	DOG1_0,
	HORSE1_0
};
//便于快速判断渲染当前模型有无贴图，以设置着色器，添加模型时也要在末尾加上新值，同样0无贴图1有贴图，即
//上面枚举变量的后缀
const bool SHADER_TYPE_OF_MODEL[] = { 0,1,0,1,0,1,0,0 };

class ModelPosition
{
public:
	ModelIndex mi;//模型索引
	glm::mat4 modelMat;//位置、朝向、缩放大小
	ModelPosition(ModelIndex mi, glm::mat4 modelMat)
	{
		this->mi = mi;
		this->modelMat = modelMat;
	}
	ModelPosition()
	{
		this->mi = ModelIndex::TREE1_MAPLE_TREE_0;
		this->modelMat = glm::mat4(1.0f);
	}
};

class ModelRender
{
protected:
	//模型着色程序
	Shader* modelShader;
	//模型指针数组，存储加载的所有模型
	vector <Model*> models;
	DirLight dl;//方向光
	PointLight pl;//点光源
	SpotLight sl;//聚光
	//modelPos存储所有实际要画的模型的索引及model矩阵信息
	//因为加载的时候会把模型文件夹下所有的模型资源都加载了，但实际用的时候可能发现有的模型效果
	//不好，不一定把所有模型都画出来，为了避免来回增删模型资源文件的麻烦，直接在modelPos中记录
	//实际要画的模型的信息
	vector <ModelPosition> modelPos;
	//阴影测试平面，正式运行时不用管
	GLuint testPlaneVAO;
	GLuint testPlaneUBI;

public:
	friend class ShadowMapRender;
	ModelRender()
	{
		//加载着色器程序
		this->modelShader = new Shader(MODEL_SHADER_VS_PATH, MODEL_SHADER_FS_PATH);
		//加载所有模型文件
		for (auto i = 0; i < MODEL_NUM; i++)
		{
			Model* temp = new Model(MODEL_SOURCE_PATH[i]);
			this->models.push_back(temp);
		}
		//加个默认光照
		this->dl.ambient = LIGHT_BIAS.x * LIGHT_COLOR;
		this->dl.diffuse = LIGHT_BIAS.x * LIGHT_COLOR;
		this->dl.specular = LIGHT_BIAS.x * LIGHT_COLOR;
		this->dl.direction = LIGHT_DIRECTION;
		//用方向光设置阴影时需假定一个光源位置，这里使用-this->dl.direction，
		//因此要拉高光源位置，原方向向量乘100倍，其他光照计算时会将方向向量规范化，
		//因此不受影响
		this->dl.direction *= LIGHT_DIR_TO_POS_RATIO;
		//为着色器程序设置初始光照
		this->setDirLight(this->dl);
		this->setPointLight(this->pl);
		this->setSpotLight(this->sl);
		//生成测试平面
		this->genTestPlane();
		//初始化modelPos变量，选择出所有要画的模型，并确定这些模型相应的在地图中的
		//位置、大小、朝向信息
		this->initModelPosition();
	}
	void initModelPosition()
	{
		glm::mat4 model;
		ModelPosition tempMP;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		tempMP = ModelPosition(ModelIndex::TREE1_MAPLE_TREE_0, model);
		this->modelPos.push_back(tempMP);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 5.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		tempMP = ModelPosition(ModelIndex::TREE2_0, model);
		this->modelPos.push_back(tempMP);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, -5.0f, 50.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		tempMP = ModelPosition(ModelIndex::TREE3_1, model);
		this->modelPos.push_back(tempMP);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(80.0f, 0.0f, 80.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		tempMP = ModelPosition(ModelIndex::TREE4_0, model);
		this->modelPos.push_back(tempMP);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		tempMP = ModelPosition(ModelIndex::WHITE_TAILED_DEER_1, model);
		this->modelPos.push_back(tempMP);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, 15.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		tempMP = ModelPosition(ModelIndex::DOG1_0, model);
		this->modelPos.push_back(tempMP);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(60.0f, 40.0f, 60.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		tempMP = ModelPosition(ModelIndex::HORSE1_0, model);
		this->modelPos.push_back(tempMP);
	}
	void genTestPlane()
	{
		float planeVertices[] =
		{
			// positions            // normals         // texcoords
			 150.0f, -0.5f,  150.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f,  150.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

			 150.0f, -0.5f,  150.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
			 150.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};

		GLuint planeVBO;
		glGenVertexArrays(1, &this->testPlaneVAO);
		glGenBuffers(1, &planeVBO);

		glBindVertexArray(this->testPlaneVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		Material groundm;
		groundm.Ka = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		groundm.Kd = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		groundm.Ks = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		groundm.shininess = 32.0f;

		glGenBuffers(1, &this->testPlaneUBI);
		glBindBuffer(GL_UNIFORM_BUFFER, this->testPlaneUBI);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(groundm), (void*)(&groundm), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	void setDirLight(const DirLight& dirl)
	{
		this->dl = dirl;
		modelShader->use();

		modelShader->setVec3("dirLight.direction", this->dl.direction);
		modelShader->setVec3("dirLight.ambient", this->dl.ambient);
		modelShader->setVec3("dirLight.diffuse", this->dl.diffuse);
		modelShader->setVec3("dirLight.specular", this->dl.specular);
	}
	void setPointLight(const PointLight& poil)
	{
		this->pl = poil;
		modelShader->use();

		modelShader->setVec3("pointLight.position", this->pl.position);
		modelShader->setVec3("pointLight.ambient", this->pl.ambient);
		modelShader->setVec3("pointLight.diffuse", this->pl.diffuse);
		modelShader->setVec3("pointLight.specular", this->pl.specular);
		modelShader->setFloat("pointLight.constant", this->pl.constant);
		modelShader->setFloat("pointLight.linear", this->pl.linear);
		modelShader->setFloat("pointLight.quadratic", this->pl.quadratic);
	}
	void setSpotLight(const SpotLight& spotl)
	{
		this->sl = spotl;
		modelShader->use();

		modelShader->setVec3("spotLight.position", this->sl.position);
		modelShader->setVec3("spotLight.direction", this->sl.direction);
		modelShader->setVec3("spotLight.ambient", this->sl.ambient);
		modelShader->setVec3("spotLight.diffuse", this->sl.diffuse);
		modelShader->setVec3("spotLight.specular", this->sl.specular);
		modelShader->setFloat("spotLight.constant", this->sl.constant);
		modelShader->setFloat("spotLight.linear", this->sl.linear);
		modelShader->setFloat("spotLight.quadratic", this->sl.quadratic);
		modelShader->setFloat("spotLight.cutOff", this->sl.cutOff);
		modelShader->setFloat("spotLight.outerCutOff", this->sl.outerCutOff);
	}
	//camera为镜头，shaderType为使用着色器的类型，0画无贴图模型，1画有贴图模型，modelMatr设置模型
	//的model矩阵，控制位置、朝向、缩放倍数，mi为绘制模型的编号
	void render(Camera& camera, const bool& shaderType, const glm::mat4& modelMatr, const ModelIndex& mi)
	{
		this->modelShader->use();
		this->modelShader->setBool("useTextureMap", shaderType);
		//设置手电筒，这个操作可以放在外面main.cpp的while循环中，每次画图前
		//先更新setSpotLight，现在先放里面，反正也没用
		this->modelShader->setVec3("spotLight.position", camera.Position);
		this->modelShader->setVec3("spotLight.direction", camera.Front);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_W / (float)WINDOW_H, NEAR_PLANE, FAR_PLANE);
		glm::mat4 view = camera.GetViewMatrix();

		this->modelShader->setMat4("projection", projection);
		this->modelShader->setMat4("view", view);
		this->modelShader->setVec3("viewPos", camera.Position);

		this->modelShader->setMat4("model", modelMatr);
		this->models[(int)mi]->Draw(*this->modelShader);
	}
	void draw(Camera& camera, const GLuint& depthMap, const glm::mat4& lightSpaceMatrix)
	{
		glDisable(GL_MULTISAMPLE); // 关抗锯齿
		glDisable(GL_CULL_FACE);   // 关面剔除

		this->modelShader->use();
		//设置阴影贴图的使用
		this->modelShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		this->modelShader->setInt("shadowMap", SHADOW_MAP_TEXTURE_INDEX);
		glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_TEXTURE_INDEX);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		for (auto& i : this->modelPos)
		{
			this->render(camera, SHADER_TYPE_OF_MODEL[(int)i.mi], i.modelMat, i.mi);
		}

		this->modelShader->setBool("useTextureMap", 0);

		//this->modelShader->setMat4("model", glm::mat4(1.0f));
		//glBindVertexArray(this->testPlaneVAO);
		//glBindBufferRange(GL_UNIFORM_BUFFER, 0, this->testPlaneUBI, 0, sizeof(Material));
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glEnable(GL_MULTISAMPLE); // 抗锯齿
		glEnable(GL_CULL_FACE);   // 面剔除
	}
};