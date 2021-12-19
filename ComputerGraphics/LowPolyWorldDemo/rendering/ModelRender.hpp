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
//ģ����ɫ��·��
const char* MODEL_SHADER_VS_PATH = "shaders/model.vs_";
const char* MODEL_SHADER_FS_PATH = "shaders/model.fs_";
//����ģ�����������ģ��ʱ��ֵ��һ
const int MODEL_NUM = 13;
//ģ���ļ�·�������ģ��ʱ��ĩβ������ģ��·��
const string MODEL_SOURCE_PATH[] =
{
	"../ModelResource/tree1_maple_tree/MapleTree.obj",
	"../ModelResource/nanosuit/nanosuit.obj",
	"../ModelResource/tree2/Tree low.obj",
	"../ModelResource/tree3/lowpoyltree.obj",
	"../ModelResource/tree4/LowPoly_Tree_v1.obj",
	"../ModelResource/white_tailed_deer/12961_White-Tailed_Deer_v1_l2.obj",
	"../ModelResource/dog1/big_shiba_inu_dog_low_poly.obj",
	"../ModelResource/horse1/horse1.OBJ",
	"../ModelResource/tree5/Lowpoly_tree_sample.obj",
	"../ModelResource/tree6/Death_Tree_6.fbx",
	"../ModelResource/tree7/Ipe.obj",
	"../ModelResource/tree8/Palmera.obj",
	"../ModelResource/tree9/arvore_cerrado_002.obj"
};
//ģ���������»��ߺ�׺_0��ʾ����ͼ��_1��ʾ����ͼ�����ģ��ʱ�����Ƹ�ʽ��ĩβ����ö�ٱ���
enum class ModelIndex
{
	TREE1_MAPLE_TREE_0,
	NANOSUIT_1,
	TREE2_0,
	TREE3_1,
	TREE4_0,
	WHITE_TAILED_DEER_1,
	DOG1_0,
	HORSE1_0,
	TREE5_0,
	TREE6_0,
	TREE7_0,
	TREE8_0,
	TREE9_0
};
//���ڿ����ж���Ⱦ��ǰģ��������ͼ����������ɫ�������ģ��ʱҲҪ��ĩβ������ֵ��ͬ��0����ͼ1����ͼ����
//����ö�ٱ����ĺ�׺
const bool SHADER_TYPE_OF_MODEL[] = { 0,1,0,1,0,1,0,0,0,0,0,0,0 };

class ModelPosition
{
public:
	ModelIndex mi;//ģ������
	glm::mat4 modelMat;//λ�á��������Ŵ�С
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
	//ģ����ɫ����
	Shader* modelShader;
	//ģ��ָ�����飬�洢���ص�����ģ��
	vector <Model*> models;
	DirLight dl;//�����
	PointLight pl;//���Դ
	SpotLight sl;//�۹�
	//modelPos�洢����ʵ��Ҫ����ģ�͵�������model������Ϣ
	//��Ϊ���ص�ʱ����ģ���ļ��������е�ģ����Դ�������ˣ���ʵ���õ�ʱ����ܷ����е�ģ��Ч��
	//���ã���һ��������ģ�Ͷ���������Ϊ�˱���������ɾģ����Դ�ļ����鷳��ֱ����modelPos�м�¼
	//ʵ��Ҫ����ģ�͵���Ϣ
	vector <ModelPosition> modelPos;
	//��Ӱ����ƽ�棬��ʽ����ʱ���ù�
	GLuint testPlaneVAO;
	GLuint testPlaneUBI;

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
	void initModelPosition()
	{
		this->addModelPosition(ModelIndex::TREE1_MAPLE_TREE_0, 30.0f, 10.0f, 30.0f, 0.3f);
		this->addModelPosition(ModelIndex::TREE2_0, 20.0f, 10.0f, 20.0f, 0.1f);
		this->addModelPosition(ModelIndex::TREE3_1, 50.0f, 10.0f, 50.0f, 4.0f);
		this->addModelPosition(ModelIndex::TREE4_0, 80.0f, 10.0f, 80.0f, 0.2f, -90.0f, AXIS_X);
		this->addModelPosition(ModelIndex::WHITE_TAILED_DEER_1, 100.0f, 10.0f, 100.0f, 0.4f, -90.0f, AXIS_X);
		this->addModelPosition(ModelIndex::DOG1_0, 15.0f, 10.0f, 15.0f, 0.4f, -90.0f, AXIS_X);
		this->addModelPosition(ModelIndex::HORSE1_0, 60.0f, 40.0f, 60.0f, 0.01f);
		this->addModelPosition(ModelIndex::TREE5_0, 30.0f, 20.0f, 30.0f, 3.0f);
		this->addModelPosition(ModelIndex::TREE6_0, 30.0f, 30.0f, 30.0f, 3.0f);
		this->addModelPosition(ModelIndex::TREE7_0, 30.0f, 40.0f, 30.0f, 3.0f);
		this->addModelPosition(ModelIndex::TREE8_0, 30.0f, 50.0f, 30.0f, 3.0f);
		this->addModelPosition(ModelIndex::TREE9_0, 30.0f, 60.0f, 30.0f, 3.0f);
	}

public:
	friend class ShadowMapRender;
	ModelRender()
	{
		//������ɫ������
		this->modelShader = new Shader(MODEL_SHADER_VS_PATH, MODEL_SHADER_FS_PATH);
		//��������ģ���ļ�
		for (auto i = 0; i < MODEL_NUM; i++)
		{
			Model* temp = new Model(MODEL_SOURCE_PATH[i]);
			this->models.push_back(temp);
		}
		//�Ӹ�Ĭ�Ϲ���
		this->dl.ambient = LIGHT_BIAS.x * LIGHT_COLOR;
		this->dl.diffuse = LIGHT_BIAS.x * LIGHT_COLOR;
		this->dl.specular = LIGHT_BIAS.x * LIGHT_COLOR;
		this->dl.direction = LIGHT_DIRECTION;
		//�÷����������Ӱʱ��ٶ�һ����Դλ�ã�����ʹ��-this->dl.direction��
		//���Ҫ���߹�Դλ�ã�ԭ����������100�����������ռ���ʱ�Ὣ���������淶����
		//��˲���Ӱ��
		this->dl.direction *= LIGHT_DIR_TO_POS_RATIO;
		//Ϊ��ɫ���������ó�ʼ����
		this->setDirLight(this->dl);
		this->setPointLight(this->pl);
		this->setSpotLight(this->sl);
		//���ɲ���ƽ��
		this->genTestPlane();
		//��ʼ��modelPos������ѡ�������Ҫ����ģ�ͣ���ȷ����Щģ����Ӧ���ڵ�ͼ�е�
		//λ�á���С��������Ϣ
		this->initModelPosition();
	}
	//������Ϊmi��ģ�ͷŵ�λ��(x, y, z)������scaleRatio����С������axis��תrotateAngle�Ƕȣ�Ĭ�ϲ���ת��
	void addModelPosition(const ModelIndex& mi, const float& x, const float& y, const float& z,
		const float& scaleRatio, const float& rotateAngle = 0.0f, const glm::vec3& axis = glm::vec3(1.0f, 0.0f, 0.0f))
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::scale(model, glm::vec3(scaleRatio, scaleRatio, scaleRatio));
		model = glm::rotate(model, glm::radians(rotateAngle), axis);
		this->modelPos.push_back(ModelPosition(mi, model));
	}
	//���׵ĸ�ģ��λ�õĺ��������Ҫ�ö��ﶯ����������ǿ����
	void changeModelPosition(const int& index, const float& x, const float& y, const float& z,
		const float& scaleRatio, const float& rotateAngle = 0.0f, const glm::vec3& axis = glm::vec3(1.0f, 0.0f, 0.0f))
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::scale(model, glm::vec3(scaleRatio, scaleRatio, scaleRatio));
		model = glm::rotate(model, glm::radians(rotateAngle), axis);
		this->modelPos[index].modelMat = model;
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
	//cameraΪ��ͷ��shaderTypeΪʹ����ɫ�������ͣ�0������ͼģ�ͣ�1������ͼģ�ͣ�modelMatr����ģ��
	//��model���󣬿���λ�á��������ű�����miΪ����ģ�͵ı��
	void render(Camera& camera, const bool& shaderType, const glm::mat4& modelMatr, const ModelIndex& mi)
	{
		this->modelShader->use();
		this->modelShader->setBool("useTextureMap", shaderType);
		//�����ֵ�Ͳ������������Է�������main.cpp��whileѭ���У�ÿ�λ�ͼǰ
		//�ȸ���setSpotLight�������ȷ����棬����Ҳû��
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
		glDisable(GL_MULTISAMPLE); // �ؿ����
		glDisable(GL_CULL_FACE);   // �����޳�

		this->modelShader->use();
		//������Ӱ��ͼ��ʹ��
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

		glEnable(GL_MULTISAMPLE); // �����
		glEnable(GL_CULL_FACE);   // ���޳�
	}
};