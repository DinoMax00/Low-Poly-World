#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../lib/camera.h"
#include "../lib/shader.h"
#include "../LightType.h"
#include "../config.h"
#include <iostream>

const char* SHADOW_SHADER_VS_PATH = "shaders/shadowMappingDepth.vs_";
const char* SHADOW_SHADER_FS_PATH = "shaders/shadowMappingDepth.fs_";
const GLuint SHADOW_WIDTH = 4096;
const GLuint SHADOW_HEIGHT = 4096;

class ShadowMapRender
{
protected:
	Shader* simpleDepthShader;
	GLuint depthMapFBO;
	glm::mat4 lightProjection;
	glm::vec3 lightDir;//平行光方向

	const float lightFrustumLeft = -200.0f;
	const float lightFrustumRight = 200.0f;
	const float lightFrustumDown = -200.0f;
	const float lightFrustumUp = 200.0f;
	const float lightFrustumNear = 1.0f;
	const float lightFrustumFar = 150.0f;
	const glm::vec3 lightTowards = glm::vec3(0.0f);
	const glm::vec3 lightUp = glm::vec3(0.0, 1.0, 0.0);

public:
	glm::mat4 lightSpaceMatrix{};
	GLuint depthMap;

	ShadowMapRender()
	{
		this->simpleDepthShader = new Shader(SHADOW_SHADER_VS_PATH, SHADOW_SHADER_FS_PATH);

		glGenFramebuffers(1, &this->depthMapFBO);

		// create depth texture
		glGenTextures(1, &this->depthMap);
		glBindTexture(GL_TEXTURE_2D, this->depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->lightDir = LIGHT_DIRECTION;
		this->lightDir *= LIGHT_DIR_TO_POS_RATIO;
		this->lightProjection = glm::ortho(this->lightFrustumLeft, this->lightFrustumRight, 
			this->lightFrustumDown, this->lightFrustumUp, this->lightFrustumNear, this->lightFrustumFar);
	}
	void setLightDir(const glm::vec3& ldir)
	{
		this->lightDir = ldir;
	}
	void genShadowMap(const ModelRender& MR, const TerrainRender& TR, CloudRenderer& CR)
	{
		glDisable(GL_MULTISAMPLE); // 关抗锯齿
		glDisable(GL_CULL_FACE);   // 关面剔除

		glm::mat4 lightView = glm::lookAt(-this->lightDir, this->lightTowards, this->lightUp);
		this->lightSpaceMatrix = this->lightProjection * lightView;

		// render scene from light's point of view
		this->simpleDepthShader->use();
		this->simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		//面剔除解决阴影偏移问题，此处因为模型数据缘故不用
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);


		//this->simpleDepthShader->setMat4("model", glm::mat4(1.0f));
		//glBindVertexArray(MR.planeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);


		//this->simpleDepthShader->setMat4("model", glm::mat4(1.0f));
		//glBindVertexArray(TR.VAO);
		//glDrawElements(GL_TRIANGLES, TR.idc_len, GL_UNSIGNED_INT, 0);


		//云的阴影
		for (auto i = 0; i < (int)CR.clouds->size(); i++) 
		{
			Cloud cloud = (*CR.clouds)[i];
			glm::mat4 model = glm::mat4(1.0f);
			CR.pos[i] += CR.moveSpeed / CR.scale[i] * glm::vec3(CR.pos[i].y / 20); // 根据云的大小 高度设置速度
			if (CR.pos[i].x < 0) CR.pos[i].x += MAP_SIZE;
			else if (CR.pos[i].x > MAP_SIZE) CR.pos[i].x -= MAP_SIZE;

			if (CR.pos[i].z < 0) CR.pos[i].z += MAP_SIZE;
			else if (CR.pos[i].z > MAP_SIZE) CR.pos[i].z -= MAP_SIZE;

			model = glm::translate(model, CR.pos[i]);
			model = glm::scale(model, glm::vec3(1.0f * CR.scale[i], 1.0f * CR.scale[i], 0.9f * CR.scale[i]));// make sure to initialize matrix to identity matrix first
			model = glm::rotate(model, glm::radians(CR.rotate[i]), glm::vec3(0.0f, 1.0f, 0.0f));

			this->simpleDepthShader->setMat4("model", model);
			for (auto i = 0; i < Cloud::SPHERE_COUNT; i++) 
			{
				glBindVertexArray(cloud.cloudVAOs[i]);
				glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
				glDrawArrays(GL_TRIANGLES, 0, cloud.spherefaces[i].size());
			}
		}

		for (auto& i : MR.modelPos)
		{
			this->simpleDepthShader->setMat4("model", i.modelMat);
			MR.models[(int)i.mi]->Draw(*this->simpleDepthShader);
		}

		//glCullFace(GL_BACK); // 不要忘记设回原先的culling face
		//glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, WINDOW_W, WINDOW_H);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


		glEnable(GL_MULTISAMPLE); // 抗锯齿
		glEnable(GL_CULL_FACE);   // 面剔除

	}
};